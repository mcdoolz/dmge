<?php
/**
 * @file
 * dmge_artists.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function dmge_artists_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create artist content'.
  $permissions['create artist content'] = array(
    'name' => 'create artist content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any artist content'.
  $permissions['delete any artist content'] = array(
    'name' => 'delete any artist content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own artist content'.
  $permissions['delete own artist content'] = array(
    'name' => 'delete own artist content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in artist_link'.
  $permissions['delete terms in artist_link'] = array(
    'name' => 'delete terms in artist_link',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in artist_tags'.
  $permissions['delete terms in artist_tags'] = array(
    'name' => 'delete terms in artist_tags',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit any artist content'.
  $permissions['edit any artist content'] = array(
    'name' => 'edit any artist content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own artist content'.
  $permissions['edit own artist content'] = array(
    'name' => 'edit own artist content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in artist_link'.
  $permissions['edit terms in artist_link'] = array(
    'name' => 'edit terms in artist_link',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in artist_tags'.
  $permissions['edit terms in artist_tags'] = array(
    'name' => 'edit terms in artist_tags',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  return $permissions;
}
