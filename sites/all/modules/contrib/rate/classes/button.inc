<?php

class RateButton
{
  /**
   * Button identification.
   *
   * Can be strings or numbers, like 'up' or '5' for the last fivestar star.
   *
   * @var string
   */
  protected $id;

  /**
   * Label.
   *
   * @var string
   */
  protected $label;

  /**
   * VotingAPI value.
   *
   * @var int
   */
  protected $value;

  /**
   * Rate token. This token is used by the client to vote on this button.
   *
   * @var string
   */
  protected $token;

  /**
   * HTML mode for button label.
   *
   * @var bool
   */
  protected $html;

  /**
   * Classes for link.
   *
   * @var string
   */
  protected $class;

  /**
   * Create a new button.
   *
   * @param string $id
   * @param string $label
   * @param int $value
   * @param string $token
   */
  public function __construct($id, $label, $value, $token, $html = FALSE, $class = '') {
    $this->id = $id;
    $this->label = $label;
    $this->value = $value;
    $this->token = $token;
    $this->html = $html;
    $this->class = $class;
  }

  /**
   * Get HTML code for button.
   */
  public function __toString() {
    $classes = array('rate-button', "button{$this->id}");
    if ($this->class) {
      $classes[] = $this->class;
    }
    return theme('link', array(
      'text' => $this->label,
      'path' => $_GET['q'],
      'options' => array(
        'attributes' => array(
          'rel' => 'nofollow',
          'class' => $classes,
          'data-token' => $this->token,
        ),
        'query' => array(
          'rate' => $this->token,
        ),
        'html' => $this->html,
      ),
    ));
  }

  /**
   * Check if the user has clicked this button.
   */
  public function hasClicked() {
    if (isset($_GET['rate']) && $_GET['rate'] == $this->token) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Get VotingAPI value.
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Get label.
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * Add class.
   *
   * @param string $class
   */
  public function addClass($class) {
    $classes = array_map('trim', explode(' ', $this->class));
    $classes[] = $class;
    $this->class = implode(' ', array_filter(array_unique($classes)));
  }
}
