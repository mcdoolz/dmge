<?php

/**
 * @file
 * Definition of droogle_handler_filter_in.
 */

/**
 * Allows in operations only
 */
class droogle_handler_filter_in extends views_handler_filter {

  function query() {
    $this->query->add_in($this->options['group'], $this->real_field, $this->value);
  }

  /**
   * Options form subform for setting options.
   *
   * This should be overridden by all child classes and it must
   * define $form['value']
   *
   * @see options_form()
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => empty($form_state['exposed']) ? t('Value') : '',
      '#default_value' => $this->value,
    );
  }
}
