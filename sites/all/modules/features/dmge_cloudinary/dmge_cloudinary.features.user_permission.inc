<?php
/**
 * @file
 * dmge_cloudinary.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function dmge_cloudinary_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer cloudinary'.
  $permissions['administer cloudinary'] = array(
    'name' => 'administer cloudinary',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'cloudinary_sdk',
  );

  return $permissions;
}
