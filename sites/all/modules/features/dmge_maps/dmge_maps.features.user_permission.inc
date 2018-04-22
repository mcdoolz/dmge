<?php
/**
 * @file
 * dmge_maps.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function dmge_maps_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create resource content'.
  $permissions['create resource content'] = array(
    'name' => 'create resource content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any resource content'.
  $permissions['delete any resource content'] = array(
    'name' => 'delete any resource content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own resource content'.
  $permissions['delete own resource content'] = array(
    'name' => 'delete own resource content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in resource_link'.
  $permissions['delete terms in resource_link'] = array(
    'name' => 'delete terms in resource_link',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit any resource content'.
  $permissions['edit any resource content'] = array(
    'name' => 'edit any resource content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own resource content'.
  $permissions['edit own resource content'] = array(
    'name' => 'edit own resource content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in resource_link'.
  $permissions['edit terms in resource_link'] = array(
    'name' => 'edit terms in resource_link',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  return $permissions;
}
