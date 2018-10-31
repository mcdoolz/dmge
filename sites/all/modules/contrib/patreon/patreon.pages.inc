<?php
/**
 * @file
 * Page callbacks for Patreon module.
 */

use Drupal\patreon\PatreonUnauthorizedException;

/**
 * Implements hook_form().
 */
function patreon_admin_form($form, &$form_state) {
  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('OAuth Settings'),
    '#description' => t('To enable OAuth based access for patreon, you must <a href="@url">register this site</a> with Patreon and add the provided keys here.', array('@url' => PATREON_REGISTER_OAUTH_URL)),
  );
  $form['oauth']['endpoint'] = array(
    '#markup' => t('<p>When registering with Patreon, you must add @url as your application endpoint.</p>', array('@url' => patreon_get_callback_url())),
  );
  $form['oauth']['patreon_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Patreon Client ID'),
    '#default_value' => variable_get('patreon_client_id', NULL),
    '#required' => TRUE,
  );
  $form['oauth']['patreon_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#default_value' => variable_get('patreon_client_secret', NULL),
    '#required' => TRUE,
  );
  $form['oauth']['description'] = array(
    '#markup' => t('<p>When you submit this form, you will be redirected to the Patreon website to authorise your account.</p>'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'patreon_admin_form_submit';

  return $form;
}

/**
 * Form submit handler for adding a Patreon account.
 */
function patreon_admin_form_submit($form, &$form_state) {
  $client_id = $form_state['values']['patreon_client_id'];
  patreon_authorise_account($client_id);
}

/**
 * Callback to obtain a valid Oauth token.
 */
function patreon_oauth_callback() {
  if (user_is_logged_in() && $code = $_GET['code']) {
    $tokens = NULL;

    if ($bridge = patreon_get_bridge()) {
      try {
        $tokens = patreon_get_tokens($code);
      }
      catch (PatreonUnauthorizedException $e) {
        $message = t('The site has failed to load the Patreon client with the error :error. Please ensure you have a valid API Key and Secret entered at <a href="/admin/config/services/patreon">/admin/config/services/patreon</a>.', array(
          ':error' => $e->getMessage(),
        ));

        drupal_set_message($message);
        watchdog('patreon', $message, array(), WATCHDOG_ERROR);
      }
      catch (\Exception $e) {
        $message = t('The site has failed to load the Patreon client with the error :error.', array(
          ':error' => $e->getMessage(),
        ));

        drupal_set_message($message);
        watchdog('patreon', $message, array(), WATCHDOG_ERROR);
      }

      if ($tokens) {
        foreach (array('access', 'refresh') as $token_name) {
          $name = $token_name . '_token';
          if (array_key_exists($name, $tokens)) {
            variable_set('patreon_' . $token_name . '_token', $tokens[$name]);

            if ($token_name == 'access') {
              try {
                $user = patreon_fetch_user($tokens[$name]);

                if ($id = $bridge->getValueByKey($user, 'data.id')) {
                  variable_set('patreon_creator_id', $id);
                }
              }
              catch (\Exception $e) {
                $message = t('There was an error fetching data from the Patreon API: :error.', array(
                  ':error' => $e->getMessage(),
                ));

                drupal_set_message($message);
                watchdog('patreon', $message, array(), WATCHDOG_ERROR);
              }
            }
          }
        }
      }
    }
    drupal_goto('admin/config/services/patreon');
  }

  drupal_not_found();
}
