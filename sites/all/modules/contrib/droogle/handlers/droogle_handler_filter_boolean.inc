<?php

/**
 * @file
 * Definition of droogle_handler_filter_boolean.
 */

/**
 * Simple filter to handle matching of boolean values
 */
class droogle_handler_filter_boolean extends views_handler_filter_boolean_operator {

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    if (empty($this->value)) {
      $this->query->add_where($this->options['group'], $this->real_field, 'false', '=', FALSE);
    }
    else {
      $this->query->add_where($this->options['group'], $this->real_field, 'true', '=', FALSE);
    }
  }
}
