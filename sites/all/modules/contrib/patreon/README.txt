-- SUMMARY --

The Patreon module implements the patreon.php library
(https://github.com/Patreon/patreon-php) to connect a Drupal site with the
Patreon API. This allows other modules to access data from an authorised Patreon
(https://www.patreon.com) account, either the creator or their patrons.

-- REQUIREMENTS --

A client key and secret must be obtained by registering at
https://www.patreon.com/platform/documentation/clients. The module's endpoint
(<your site>/patreon/oauth) must be registered as an allowed redirect
destination in the client application.

The Patreon API library has a dependency on composer, which means that this
module must also be installed using composer. You can find instructions for
managing a Drupal 7 site using composer at https://www.drupal.org/node/2718229.

-- INSTALLATION --

The Patreon API library has a dependency on composer, which means that this
module must also be installed using composer. You can find instructions for
managing a Drupal 7 site using composer at https://www.drupal.org/node/2718229.

-- CONFIGURATION --

A valid client id and secret key must be added to the form at
/admin/config/services/patreon, and access to the creator account when
prompted.

-- CUSTOMIZATION --

This module provides a bridge between Drupal and the Patreon API, allowing
other modules to provide functionality reliant on the API. Once configured,
the module provides three main functions to obtain data from Patreon:

* patreon_fetch_user()
* patreon_fetch_campaign()
* patreon_fetch_page_of_pledges()

Each returns an \Art4\JsonApiClient\Document obtained from the Patreon API. Values can be
pulled from the returned results using the bridge method get_value_by_key(). The
patreon_get_value_by_key() function has been depreciated and will error if a return is
sent to it.

The functions correspond to the documented functions provided by the patreon.php
library, and each uses the default user access token stored in the variable
patreon_access_token when the creator authorises their account. Other access
tokens can be passed to the function to obtain patron information.

Custom modules can implement their own authorisation processes by using the
patreon_authorise_account() and patreon_get_tokens() functions.

A new PatreonBridge.php entity now exists to provide a bridge between Drupal and the API.
The Drupal functions can be bypassed in favour of direct interaction with this object. A
new instance can be instantiated using the patreon_get_bridge() function.

-- DEPRECIATION --
The function patreon_fetch_campaign_and_patrons() has been depreciated at the
API side and cannot be used. It will return no values if used.

The function patreon_get_value_by_key() has been depreciated as the API return is no
longer an array.

The function patreon_get_token() has been depreciated in favour of patreon_get_tokens(),
which will return all tokens provided by the API. If used, it will still return an
access token, but will log a depreciation warning.

The function patreon_check_response() has been depreciated in favour of proper error
handling within the bridge method apiFetch(). This function will return TRUE and log
a depreciation warning.

The function patreon_iterator_search() has been depreciated as it is no longer required by
the module. It will continue to return results, and log a depreciation warning.

All depreciated functions will be removed in future versions of the module.

-- TROUBLESHOOTING --

If you are implementing a custom authorisation process and using
patreon_authorise_account() fails to redirect the user to Patreon's
authorisation page, check that you have added your callback URL to the
allowed URLs by implementing hook_patreon_allowed_callbacks_alter().

-- CONTACT --

Current maintainer:

* Dale Smith (MrDaleSmith) - https://drupal.org/user/2612656