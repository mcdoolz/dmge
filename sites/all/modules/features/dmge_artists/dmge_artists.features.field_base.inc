<?php
/**
 * @file
 * dmge_artists.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function dmge_artists_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_artist_links'.
  $field_bases['field_artist_links'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_artist_links',
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

  // Exported field_base: 'field_artist_rating'.
  $field_bases['field_artist_rating'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_artist_rating',
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
      'tag' => 'artist_rating',
    ),
    'translatable' => 0,
    'type' => 'rate',
  );

  // Exported field_base: 'field_artist_tags'.
  $field_bases['field_artist_tags'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_artist_tags',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'artist_tags',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  return $field_bases;
}
