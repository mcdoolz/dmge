<?php
/**
 * @file
 * mc.multifield.inc
 */

/**
 * Implements hook_multifield_default_multifield().
 */
function mc_multifield_default_multifield() {
  $export = array();

  $multifield = new stdClass();
  $multifield->disabled = FALSE; /* Edit this to true to make a default multifield disabled initially */
  $multifield->api_version = 1;
  $multifield->machine_name = 'field_cta';
  $multifield->label = 'field_cta';
  $multifield->description = '';
  $export['field_cta'] = $multifield;

  $multifield = new stdClass();
  $multifield->disabled = FALSE; /* Edit this to true to make a default multifield disabled initially */
  $multifield->api_version = 1;
  $multifield->machine_name = 'field_slides';
  $multifield->label = 'field_slides';
  $multifield->description = '';
  $export['field_slides'] = $multifield;

  return $export;
}
