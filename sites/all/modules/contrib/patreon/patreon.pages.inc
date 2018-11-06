<?php
/**
 * @file
 * Page callbacks for Patreon module.
 */

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
 * Form validation for adding a Patreon account.
 */
function patreon_admin_form_validate($form, &$form_state) {
  $library = libraries_load(PATREON_LIBRARY);
  if (!$library['loaded']) {
    form_set_error('', t('You must install the Patreon library before configuring Oauth'));
  }
}

/**
 * Form submit handler for adding a Patrone account.
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
    if ($token = patreon_get_token($code)) {
      variable_set('patreon_access_token', $token);

      if ($user = patreon_fetch_user($token)) {
        $id = patreon_get_value_by_key($user, 'id');
        variable_set('patreon_creator_id', $id);
      }
      else {
        drupal_set_message(t('There was an error fetching data from the Patreon API. Please try again.'), 'error');
      }
    }
    else {
      drupal_set_message(t('The connection to Patreon failed. Please try again.'), 'error');
    }
    drupal_goto('admin/config/services/patreon');
  }

  drupal_not_found();
}
