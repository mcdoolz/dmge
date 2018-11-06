<?php
/**
 * @file
 * Page callbacks for Patreon User module.
 */

use Drupal\patreon\PatreonUnauthorizedException;

/**
 * Oauth callback to handle user registration / login.
 */
function patreon_user_oauth_callback() {
  $settings = variable_get('patreon_user_registration', PATREON_USER_NO_LOGIN);
  $tokens = array();

  if ($settings != PATREON_USER_NO_LOGIN) {
    if (user_is_anonymous() && $code = $_GET['code']) {
      try {
        $tokens = patreon_get_tokens($code, patreon_user_get_callback_url());
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
        $token_data = array();

        foreach (array('access', 'refresh') as $token_name) {
          $name = $token_name . '_token';
          if (array_key_exists($name, $tokens)) {
            $token_data['field_user_patreon_' . $token_name . '_token'] = $tokens[$name];
          }
        }
        if (isset($token_data['field_user_patreon_access_token']) && isset($token_data['field_user_patreon_refresh_token'])) {
          // This is where we die.
          if ($patreon_account = patreon_fetch_user($token_data['field_user_patreon_access_token'], $token_data['field_user_patreon_refresh_token'], patreon_user_get_callback_url())) {
            if ($bridge = patreon_get_bridge()) {
              if ($patreon_id = $bridge->getValueByKey($patreon_account, 'data.id')) {
                if (patreon_user_can_login($patreon_account)) {
                  if ($account = patreon_user_get_user($patreon_account)) {
                    if ($wrapper = entity_metadata_wrapper('user', $account)) {
                      foreach ($token_data as $field => $data) {
                        if ($wrapper->__isset($field)) {
                          $wrapper->{$field} = $data;
                        }
                      }
                      $wrapper->save();

                      if (!user_is_blocked($account->name)) {
                        try {
                          patreon_user_assign_roles($account, $patreon_account);
                        }
                        catch (\Exception $e) {
                          watchdog('patreon_user', 'Error trying to update user roles for user :uid', array(
                            ':uid' => $account->uid,
                          ), WATCHDOG_ERROR);
                        }

                        $login_method = variable_get('patreon_user_login_method', PATREON_USER_COPY_ACCOUNT);

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
        }
      }

      // API error. Redirect to /user.
      drupal_set_message(t('There was an error contacting Patreon. Please try again or contact and administrator'), 'error');
      drupal_goto('user');
    }
  }

  return drupal_not_found();
}
