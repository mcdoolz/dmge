<?php
/**
 * @file
 * dmge_maps.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function dmge_maps_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['views-library-resources_block'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'library-resources_block',
    'module' => 'views',
    'node_types' => array(),
    'roles' => array(),
    'themes' => array(
      'adminimal' => array(
        'pages' => '',
        'region' => '',
        'status' => 0,
        'theme' => 'adminimal',
        'title' => '<none>',
        'visibility' => 0,
        'weight' => 0,
      ),
      'dmge' => array(
        'pages' => '',
        'region' => '',
        'status' => 0,
        'theme' => 'dmge',
        'title' => '<none>',
        'visibility' => 0,
        'weight' => 0,
      ),
    ),
  );

  return $export;
}
