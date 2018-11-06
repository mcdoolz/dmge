<?php
/**
 * @file
 * Page callbacks for Patreon User module.
 */

/**
 * Oauth callback to handle user registration / login.
 */
function patreon_user_oauth_callback() {
  $settings = variable_get('patreon_user_registration', PATREON_USER_NO_LOGIN);

  if ($settings != PATREON_USER_NO_LOGIN) {
    if (user_is_anonymous() && $code = $_GET['code']) {
      if ($token = patreon_get_token($code, patreon_user_get_callback_url())) {
        if ($patreon_account = patreon_fetch_user($token)) {
          if ($patreon_id = patreon_get_value_by_key($patreon_account, 'id')) {
            if (patreon_user_can_login($patreon_account)) {
              if ($account = patreon_user_get_user($patreon_account)) {
                if ($wrapper = entity_metadata_wrapper('user', $account)) {
                  $wrapper->field_user_patreon_token = $token;
                  $wrapper->save();

                  if (!user_is_blocked($account->name)) {
                    $login_method = variable_get('patreon_user_registration_method', PATREON_USER_COPY_ACCOUNT);

                    if ($login_method == PATREON_USER_SINGLE_SIGN_ON) {
                      $fake_state = array('uid' => $account->uid);
                      user_login_submit(array(), $fake_state);
                      drupal_goto('user');
                    }
                    else {
                      $fake_state = array(
                        'values' => array(
                          'account' => $account,
                        ),
                      );
                      module_load_include('inc', 'user', 'user.pages');
                      user_pass_submit(array(), $fake_state);
                      drupal_goto('user');
                    }
                  }
                  else {
                    drupal_set_message(t('Your account is blocked. Please contact an administrator.'), 'error');
                    drupal_goto('user');
                  }
                }
              }

              drupal_set_message(t('There was a problem creating your account. Please contact an administrator.'), 'error');
              drupal_goto('user');
            }
            else {
              $message = (variable_get('patreon_user_registration', PATREON_USER_NO_LOGIN) == PATREON_USER_ONLY_PATRONS) ? t('Only patrons may log in via Patreon.') : t('log on via Patreon is not enabled at present.');
              $message .= ' ' . t('Please contact an administrator if you feel this is in error.');
              drupal_set_message($message, 'error');
              drupal_goto('user');
            }
          }
        }
      }

      // API error. Redirect to /user.
      drupal_set_message(t('There was an error contacting Patreon. Please try again or contact and administrator'), 'error');
      drupal_goto('user');
    }
  }

  return drupal_not_found();
}
