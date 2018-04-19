<?php

/**
 * @file
 * Views field handler that renders SPARQL query results as returned from ARC.
 */

class droogle_handler_field extends views_handler_field {
  /**
   * Determine if this field can allow advanced rendering.
   *
   * Fields can set this to FALSE if they do not wish to allow
   * token based rewriting or link-making.
   */
  function allow_advanced_render() {
    return TRUE;
  }

  function init(&$view, &$options) {
    parent::init($view, $options);
  }

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
   * Get this field's label.
   */
  function label() {
    if (!isset($this->options['label'])) {
      return '';
    }
    return $this->options['label'];
  }


  /**
   * Return the class of the field.
   */
  function element_classes($row_index = NULL) {
    $classes = explode(' ', $this->options['element_class']);
    foreach ($classes as &$class) {
      $class = $this->tokenize_value($class, $row_index);
      $class = views_clean_css_identifier($class);
    }
    return implode(' ', $classes);
  }

  /**
   * Return the class of the field's wrapper.
   */
  function element_wrapper_classes($row_index = NULL) {
    $classes = explode(' ', $this->options['element_wrapper_class']);
    foreach ($classes as &$class) {
      $class = $this->tokenize_value($class, $row_index);
      $class = views_clean_css_identifier($class);
    }
    return implode(' ', $classes);
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
      return $values->{$alias};
    }
  }

  /**
   * Run before any fields are rendered.
   *
   * This gives the handlers some time to set up before any handler has
   * been rendered.
   *
   * @param $values
   *   An array of all objects returned from the query.
   */
  function pre_render(&$values) { }

  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {
    $value = $this->get_value($values);
    return $this->sanitize_value($value);
  }
}
