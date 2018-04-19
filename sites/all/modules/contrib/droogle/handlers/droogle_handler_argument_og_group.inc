<?php

/**
 * @file
 * Definition of droogle_handler_argument_og_group.
 */

/**
 * Basic argument handler for arguments that are numeric. Incorporates
 * break_phrase.
 *
 * @ingroup views_argument_handlers
 */
class droogle_handler_argument_og_group extends views_handler_argument {

  function query($group_by = FALSE) {
    $this->query->og_group = array(
      'group_type' => !empty($this->options['default_argument_options']['group_type']) ? $this->options['default_argument_options']['group_type'] :
'node',
      'gid' => $this->argument,
    );
  }

  /**
   * Validate that this argument works. By default, all arguments are valid.
   */
  function validate_arg($arg) {
    // By using % in URLs, arguments could be validated twice; this eases
    // that pain.
    if (isset($this->argument_validated)) {
      return $this->argument_validated;
    }
    // Make sure that this argument contains fields.
    $entity_type = !empty($this->options['default_argument_options']['group_type']) ? $this->options['default_argument_options']['group_type'] : 'node';
    if (!droogle_get_connection_settings_from_og(array('group_type' => $entity_type, 'gid' => $arg))) {
      $this->argument_validated = FALSE;
    }
    return parent::validate_arg($arg);
  }

}
