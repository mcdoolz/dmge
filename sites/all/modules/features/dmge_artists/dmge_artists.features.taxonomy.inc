<?php
/**
 * @file
 * dmge_artists.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dmge_artists_taxonomy_default_vocabularies() {
  return array(
    'artist_link' => array(
      'name' => 'Artist Link',
      'machine_name' => 'artist_link',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'artist_tags' => array(
      'name' => 'Artist Tags',
      'machine_name' => 'artist_tags',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
