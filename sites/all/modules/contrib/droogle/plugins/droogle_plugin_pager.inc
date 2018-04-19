<?php

/**
 * @file
 * Definition of droogle_plugin_pager.
 */

/**
 * The plugin to handle droogle pager.
 *
 * @ingroup views_pager_plugins
 */
class droogle_plugin_pager extends views_plugin_pager_full {
  function summary_title() {
    if (!empty($this->options['offset'])) {
      return format_plural($this->options['items_per_page'], 'Droogle pager, @count item, skip @skip', 'Droogle pager, @count items, skip @skip', array('@count' => $this->options['items_per_page'], '@skip' => $this->options['offset']));
    }
    return format_plural($this->options['items_per_page'], 'Droogle pager, @count item', 'Droogle pager, @count items', array('@count' => $this->options['items_per_page']));
  }

  /**
   * Overrides views_plugin_pager_full::option_definition().
   *
   * Overrides the full pager options form by deleting unused settings.
   */
  function option_definition() {
    $options = parent::option_definition();

    unset($options['quantity']);
    unset($options['tags']['first']);
    unset($options['tags']['last']);
    $options['tags']['previous']['default'] = '‹‹';
    $options['tags']['next']['default'] = '››';

    return $options;
  }

  /**
   * Overrides views_plugin_pager_full::options_form().
   *
   * Overrides the full pager options form by deleting unused settings.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['quantity']);
    unset($form['tags']['first']);
    unset($form['tags']['last']);
    unset($form['offset']);
    unset($form['id']);
    unset($form['total_pages']);
  }

  /**
   * Overrides views_plugin_pager_full::render().
   *
   * Overrides the full pager renderer by changing the theme function
   * and leaving out variables that are not used in the droogle pager.
   */
  function render($input) {
    $pager_theme = views_theme_functions('droogle_views_pager', $this->view, $this->display);
    return theme($pager_theme, array(
      'current_page_token' => $this->view->current_page_token,
      'next_page_token' => $this->view->next_page_token,
    ));
  }
}
