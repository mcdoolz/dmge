<?php

abstract class RateWidget
{
  /**
   * Machine name for this widget.
   *
   * @var string
   */
  protected $type;

  /**
   * Display mode / formatter name (i.e. "rate_full").
   *
   * @var string
   */
  protected $mode;

  /**
   * Entity type.
   *
   * @var string
   */
  protected $entity_type;

  /**
   * Entity id.
   *
   * @var int
   */
  protected $entity_id;

  /**
   * Voting API value type.
   *
   * @var string
   */
  protected $value_type;

  /**
   * Field API field name.
   *
   * @var string
   */
  protected $field_name;

  /**
   * VotingAPI tag.
   *
   * @var string
   */
  protected $tag;

  /**
   * Voters account.
   *
   * @var object
   */
  protected $account;

  /**
   * Elements of the rate widget as keyed array where the names are the element
   * identifiers (like 'button1') and the values are the elements, which must
   * be casted to strings.
   *
   * @var array
   */
  protected $elements;

  /**
   * Displayed rating.
   *
   * This setting reflects the current state. Options are:
   * - RATE_AVERAGE
   * - RATE_USER
   *
   * @var int
   */
  protected $displayed;

  /**
   * Rating to display directly after voting.
   *
   * This setting comes directly from the widget configuration. Options are:
   * - RATE_AVERAGE
   * - RATE_USER
   * - RATE_USER_OR_AVERAGE
   *
   * @var int
   */
  protected $displayed_just_voted;

  /**
   * Did the user just click on a rate button?
   *
   * @var bool
   */
  protected $just_voted;

  /**
   * Voting result for display.
   *
   * @var array
   */
  protected $results;

  /**
   * Displayed rating.
   *
   * @var float
   */
  protected $rating;

  /**
   * Vote count.
   *
   * @var int
   */
  protected $count;

  /**
   * Has the user voted?
   *
   * @var bool
   */
  protected $voted;

  /**
   * Create a new Rate widget.
   *
   * @param array $attributes
   *   Associative array with the following keys:
   *   - type: Machine name for this widget.
   *   - entity_type: Entity type
   *   - entity_id: Entity id for the entity we are rating on.
   *   - mode: Display mode.
   *   - account: Voters account.
   */
  public function __construct($attributes) {
    global $user;

    $this->type = $attributes['type'];
    $this->entity_type = $attributes['entity_type'];
    $this->entity_id = $attributes['entity_id'];
    $this->mode = $attributes['mode'];
    $this->field_name = $attributes['field_name'];
    $this->account = empty($attributes['account']) ? $user : $attributes['account'];
    $this->displayed = empty($attributes['displayed']) ? RATE_AVERAGE : $attributes['displayed'];
    $this->displayed_just_voted = empty($attributes['displayed_just_voted']) ? RATE_USER_OR_AVERAGE : $attributes['displayed_just_voted'];
    $this->elements = array();
    $this->just_voted = FALSE;

    $this->tag = isset($attributes['tag']) ? $attributes['tag'] : 'vote';

    $this->load();
    $this->processVote();

    if ($this->just_voted) {
      $this->displayed = $this->displayed_just_voted;
    }

    $this->results = $this->getAverageResults();
    $user_vote = $this->getUserResults();
    if ($user_vote) {
      $this->voted = TRUE;
      $this->results[] = array(
        'function' => 'user',
        'value' => $user_vote,
      );
    }
    else {
      $this->voted = FALSE;
    }

    $average_rating = NULL;
    $user_rating = NULL;
    $this->rating = 0;
    $this->count = 0;
    foreach ($this->results as $result) {
      switch ($result['function']) {
        case 'average':
          $average_rating = $result['value'];
        case 'user':
          $user_rating = $result['value'];
          break;
        case 'count':
          $this->count = $result['value'];
          break;
      }
    }

    switch ($this->displayed) {
      case RATE_USER_OR_AVERAGE:
      case RATE_USER:
        if (!is_null($user_rating)) {
          $this->rating = $user_rating;
          $this->displayed = RATE_USER;
          break;
        }
      case RATE_AVERAGE:
        $this->rating = $average_rating;
        break;
    }

    $this->calculateResults();
    $this->init();
  }

  /**
   * Load the widget.
   *
   * Set up the buttons, tag, value type etc.
   */
  abstract protected function load();

  /**
   * Initialize the widget.
   *
   * This function is used to set additional elements after the widget is
   * loaded and results have been fetched.
   */
  protected function init() {

  }

  /**
   * Generate HTML code for widget.
   *
   * @return string
   */
  abstract public function __toString();

  /**
   * Get JS scripts required for the widget.
   *
   * @return array
   */
  public function getJS() {
    return array();
  }

  /**
   * Get CSS files required for the widget.
   *
   * @return array
   */
  public function getCSS() {
    return array();
  }

  /**
   * Save vote.
   *
   * @param int $value
   */
  public function clickButton($value) {

  }

  /**
   * Check if user may view this widget.
   */
  public function viewAccess() {
    return TRUE;
  }

  /**
   * Generate a rate token
   *
   * This function is very similar to drupal_get_token(). The reason not to use
   * drupal_get_token() is that that function uses session_id(), which generates
   * different session id's for each page request when not logged in.
   *
   * @param string $values
   * @return string
   */
  final protected function getToken($value = '') {
    global $user;
    $value .= $this->entity_type;
    $value .= $this->entity_id;
    return drupal_hmac_base64($value, $user->uid . drupal_get_private_key() . drupal_get_hash_salt());
  }

  /**
   * Check if the user has clicked a button on this widget and return that
   * button.
   *
   * @return mixed
   *   Instance of RateButton or FALSE.
   */
  protected function clickedButton() {
    foreach ($this->elements as $element) {
      if ($element instanceof RateButton && $element->hasClicked()) {
        return $element;
      }
    }
    return FALSE;
  }

  /**
   * Process the users vote (if applicable).
   */
  public function processVote() {
    if ($button = $this->clickedButton()) {
      $this->just_voted = TRUE;
      $this->vote($button->getValue());
    }
  }

  /**
   * Save a vote.
   *
   * @param int $value
   */
  public function vote($value) {
    $votes = array(
      'entity_type' => $this->entity_type,
      'entity_id' => $this->entity_id,
      'value_type' => $this->value_type,
      'value' => $value,
      'tag' => $this->tag,
    );

    $criteria = NULL;

    // Call hook_rate_vote_alter().
    $redirect = FALSE;
    $save = TRUE;
    $context = array(
      'redirect' => &$redirect,
      'save' => &$save,
      'criteria' => &$criteria,
      'widget' => &$this,
    );
    drupal_alter('rate_vote', $votes, $context);

    if ($save) {
      votingapi_set_votes($votes, $criteria);
    }
  }

  /**
   * Get average voting results.
   *
   * @return array
   */
  protected function getAverageResults() {
    $criteria = array(
      'entity_type' => $this->entity_type,
      'entity_id' => $this->entity_id,
      'tag' => $this->tag,
      'value_type' => $this->value_type,
    );
    return votingapi_select_results($criteria);
  }

  /**
   * Get users vote.
   *
   * @return int Button value
   */
  protected function getUserResults() {
    $criteria = array(
      'entity_type' => $this->entity_type,
      'entity_id' => $this->entity_id,
      'tag' => $this->tag,
      'value_type' => $this->value_type,
      'uid' => $this->account->uid,
    );
    if (!$this->account->uid) {
      $criteria += array('vote_source' => ip_address());
    }
    if ($results = votingapi_select_votes($criteria)) {
      // Check the anonymous window. We should not display this vote when its anonymous and casted too long ago.
      $anonymous_window = variable_get('votingapi_anonymous_window', 86400);
      // Use a minimum of 5 seconds. Needed to get the anonymous vote directly after the user has voted.
      $anonymous_window = max(5, $anonymous_window);
      if ($this->account->uid || $user_vote[0]['timestamp'] > REQUEST_TIME - $anonymous_window || $anonymous_window == -1) {
        return $results[0]['value'];
      }
    }
    return NULL;
  }

  /**
   * Add elements with results.
   */
  protected function calculateResults() {

    // Add rating.
    $this->elements['rating'] = round($this->rating);

    // Add vote count.
    $this->elements['count'] = $this->count;

    // Add thumbs up / down values.
    if ($this->value_type == 'points') {
      $up = NULL;
      $down = NULL;
      foreach ($this->elements as $name => $element) {
        if ($element instanceof RateButton) {
          if ($element->getValue() == 1) {
            $up = $name;
          }
          if ($element->getValue() == -1) {
            $down = $name;
          }
        }
      }
      if ($up && $down) {
        $down_count = (int) ($this->count - $this->rating) / 2;
        $up_count = $this->count - $down_count;
        if ($up_count == $down_count) {
          $up_percent = 50;
          $down_percent = 50;
        }
        else {
          $up_percent = ($up_count / $this->count) * 100;
          $down_percent = 100 - $up_percent;
        }
        $this->elements['up_count'] = $up_count;
        $this->elements['up_percent'] = $up_percent;
        $this->elements['down_count'] = $down_count;
        $this->elements['down_percent'] = $down_percent;
      }
    }

    // Add button counts for option widgets.
    foreach ($this->elements as $name => $element) {
      if ($element instanceof RateButton) {
        $value = $element->getValue();
        foreach ($this->results as $result) {
          if ($result['function'] == 'option-' . $value) {
            $this->elements["{$name}_count"] = $result['value'];
          }
        }
      }
    }
  }

  protected function getButtonLabelByValue($value) {
    foreach ($this->elements as $element) {
      if ($element instanceof RateButton && $element->getValue() == $value) {
        return $element->getLabel();
      }
    }
  }
}
