<?php

/**
 * @file
 * Hooks provided by the Patreon Module.
 */

/**
 * Define allowed callback URLs for Pattreon to redirect to after authorisation.
 *
 * @param array $allowed
 *   Array of allowed URLs.
 */
function hook_patreon_allowed_callbacks_alter(&$allowed) {

  // My module specifies an alternative callback URL to handle a custom
  // post authorisation process. patreon_authorise_account() needs to be
  // informed that this callback URl is allowed. This is done by adding it to
  // the allowed array.
  $allowed[] = 'https://example.com/callback';
}
