<?php

/**
 * @file
 * Defination of droogle_handler_filter_string.
 */

/**
 * String filter for droogle that only does contains..
 *
 * @ingroup views_filter_handlers
 */
class droogle_handler_filter_string_contains extends droogle_handler_filter_string {

  function operators() {
    $all_operators = parent::operators();
    $operators = array();
    $operators['contains'] = $all_operators['contains'];
    return $operators;
  }
}
