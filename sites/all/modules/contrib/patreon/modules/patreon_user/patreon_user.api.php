<?php

/**
 * @file
 * Hooks provided by the Patreon User Module.
 */

/**
 * Amend the fields to be stored against the Drupal user before it is saved.
 *
 * @param array $fields
 *   An array of fields that will be passed to user_save() to create a user.
 * @param array $patreon_account
 *   The Patreon account returned by patreon_fetch_user().
 */
function hook_patreon_user_create_user_alter(&$fields, $patreon_account) {
  $fields['my_custom_field'] = TRUE;
}
