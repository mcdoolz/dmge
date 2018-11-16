<?php
/**
 * @file
 * dmge_maps.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dmge_maps_taxonomy_default_vocabularies() {
  return array(
    'resource_link' => array(
      'name' => 'Resource Link',
      'machine_name' => 'resource_link',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'resource_tags' => array(
      'name' => 'Resource Tags',
      'machine_name' => 'resource_tags',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'resource_type' => array(
      'name' => 'Resource Type',
      'machine_name' => 'resource_type',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
