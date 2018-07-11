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

The module uses the Libraries module (https://www.drupal.org/project/libraries)
to manage the use of the Patreon PHP library. If the module is enabled via
drush, this will be automatically downloaded into your codebase. Otherwise, it
will need manually extracting into the libraries/patreon folder.

-- INSTALLATION --

It is recommended that the module is installed via drush (drush en patreon) to
ensure the correct downloading of the required library. But it can be installed
as usual; see https://drupal.org/node/895232 for further information.

-- CONFIGURATION --

A valid client id and secret key must be added to the form at
/dmin/config/services/patreon, and access to the creator account when
prompted.

-- CUSTOMIZATION --

This module provides a bridge between Drupal and the Patreon API, allowing
other modules to provide functionality reliant on the API. Once configured,
the module provides three main functions to obtain data from Patreon:

* patreon_fetch_user()
* patreon_fetch_campaign()
* patreon_fetch_campaign_and_patrons()

Each returns an array of data obtained from the Patreon API. Values can be
pulled from the returned results using the patreon_get_value_by_key() helper
function.

The functions correspond to the documented functions provided by the patreon.php
library, and each uses the default user access token stored in the variable
patreon_access_token when the creator authorises their account. Other access
tokens can be passed to the function to obtain patron information.

Custom modules can implement their own authorisation processes by using the
patreon_authorise_account() and patreon_get_token() functions.

-- TROUBLESHOOTING --

If you are implementing a custom authorisation process and using
patreon_authorise_account() fails to redirect the user to Patreon's
authorisation page, check that you have added your callback URL to the
allowed URLs by implementing hook_patreon_allowed_callbacks_alter().

-- CONTACT --

Current maintainer:

* Dale Smith (MrDaleSmith) - https://drupal.org/user/2612656