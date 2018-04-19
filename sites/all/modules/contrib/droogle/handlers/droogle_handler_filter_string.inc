<?php

/**
 * @file
 * Defination of droogle_handler_filter_string.
 */

/**
 * Base string filter for droogle.
 *
 * @ingroup views_filter_handlers
 */
class droogle_handler_filter_string extends views_handler_filter_string {

  /**
   * This kind of construct makes it relatively easy for a child class
   * to add or remove functionality by overriding this function and
   * adding/removing items from this array.
   */
  function operators() {
    $all_operators = parent::operators();
    $operators = array();
    $operators['='] = $all_operators['='];
    $operators['!='] = $all_operators['!='];
    $operators['contains'] = $all_operators['contains'];
    return $operators;
  }

  /**
   * Add this filter to the query.
   *
   * Due to the nature of fapi, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator
   * and $this->value respectively.
   */
  function query() {
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}();
    }
  }

  function op_equal($field = NULL) {
    $this->query->add_where($this->options['group'], $this->real_field, $this->value, $this->operator);
  }

  function op_contains($field = NULL) {
    if ($this->value) {
      $this->query->add_where($this->options['group'], $this->real_field, $this->value, $this->operator);
    }
  }
}
