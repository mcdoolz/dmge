<?php
/**
 * @file
 * dmge_maps.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function dmge_maps_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_map_author'.
  $field_bases['field_map_author'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_map_author',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'direction' => 'ASC',
          'property' => 'title',
          'type' => 'property',
        ),
        'target_bundles' => array(
          'artist' => 'artist',
        ),
      ),
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_map_link'.
  $field_bases['field_map_link'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_map_link',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
  );

  // Exported field_base: 'field_map_rating'.
  $field_bases['field_map_rating'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_map_rating',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'rate',
    'settings' => array(
      'allow_voting_by_author' => TRUE,
      'noperm_behavior' => 3,
      'permissions' => array(
        'allow_voting_by_author' => 1,
        'noperm_behavior' => 3,
        'roles' => array(
          1 => 0,
          2 => 0,
          3 => 0,
          4 => 0,
          5 => 0,
          6 => 0,
        ),
      ),
      'roles' => array(),
      'tag' => 'map_rating',
    ),
    'translatable' => 0,
    'type' => 'rate',
  );

  return $field_bases;
}
