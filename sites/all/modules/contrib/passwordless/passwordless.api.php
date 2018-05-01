<?php
/**
 * @file
 * Describe hooks provided by the Passwordless module.
 */

/**
 * Allows other modules to customize the landing page after a successful login.
 *
 * @param $redirect
 *   The path of the post-login landing page. The assumption is that only one
 *   enabled module will implement this hook. If more than one module
 *   implements it, the heaviest one will win.
 *
 * @see passwordless_reset()
 */
function hook_passwordless_login_redirect_alter(&$redirect) {
  $redirect = '<front>';
}