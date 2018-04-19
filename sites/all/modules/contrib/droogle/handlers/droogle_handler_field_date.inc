<?php

/**
 * @file
 * Definition of views_handler_field_date.
 */

/**
 * A handler to provide proper displays for dates.
 *
 * @ingroup views_field_handlers
 */
class droogle_handler_field_date extends views_handler_field_date {

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Add 'additional' fields to the query.
   *
   * @param $fields
   * An array of fields. The key is an identifier used to later find the
   * field alias used. The value is either a string in which case it's
   * assumed to be a field on this handler's table; or it's an array in the
   * form of
   * @code array('table' => $tablename, 'field' => $fieldname) @endcode
   */
  function add_additional_fields($fields = NULL) {
  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
  }

  /**
   * Determine if this field is click sortable.
   */
  function click_sortable() {
    return FALSE;
  }

  /**
   * Get the value that's supposed to be rendered.
   *
   * This api exists so that other modules can easy set the values of the field
   * without having the need to change the render method as well.
   *
   * @param $values
   *   An object containing all retrieved values.
   * @param $field
   *   Optional name of the field where the value is stored.
   */
  function get_value($values, $field = NULL) {
    $alias = isset($field) ? $this->aliases[$field] : $this->field_alias;
    if (isset($values->{$alias})) {
      return strtotime($values->{$alias});
    }
  }


}
