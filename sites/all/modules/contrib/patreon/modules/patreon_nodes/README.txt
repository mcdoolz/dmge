-- SUMMARY --

The Patreon Nodes module creates new node view access permissions for node
types configured as being used for Patreon content in the content type admin
form at /admin/structure/types/manage/<type>

-- REQUIREMENTS --

The module has no dependencies. It is a sub-module of the Patreon module, but
does not require that module to be enabled for its functionality. However, it
is recommended for use with the Patreon User module, which allows users from
the Patreon site to log in to Drupal and creates new roles for those users.

If you are using another module to handle node view permissions - such as
https://www.drupal.org/project/nodeaccess or
https://www.drupal.org/project/node_view_permissions - you may not require this
module, as you will already be able to grant custom node view permissions on a
per role basis.

-- INSTALLATION --

The module can be installed as usual; see https://drupal.org/node/895232 for
further information. It is a sub-module of the Patreon module.

-- CONFIGURATION --

A new section is added to the content type admin form at
/admin/structure/types/manage/<type> where administrators can identify the
node type as being controlled by this module. Once enabled, permissions to
allow users to view all controlled nodes or only controlled nodes of a specific
type.

When new content types are enabled, new permissions will be created, which will
need to be granted to roles before users can view the content type.

-- CUSTOMIZATION --

This module does not currently offer opportunity to customise, beyond
additional implementations of node grants access code.

-- TROUBLESHOOTING --

Node permissions can behave unexpectedly when used with heavy caching: if
content display behaves unexpectedly, it is worth clearing caches to see if the
issue is resolved.

-- CONTACT --

Current maintainer:

* Dale Smith (MrDaleSmith) - https://drupal.org/user/2612656