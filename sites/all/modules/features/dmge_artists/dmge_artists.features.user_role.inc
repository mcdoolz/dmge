<?php
/**
 * @file
 * dmge_artists.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function dmge_artists_user_default_roles() {
  $roles = array();

  // Exported role: artist.
  $roles['artist'] = array(
    'name' => 'artist',
    'weight' => 4,
  );

  return $roles;
}
