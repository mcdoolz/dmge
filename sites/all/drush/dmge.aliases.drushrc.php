<?php

/**
 * @file
 * Drush Aliases for mykm.ca.
 */

$aliases['live'] = array(
  'uri' => 'dmge.net',
  'root' => '/home/dmge/public_html',
  'remote-host' => 'dmge.net',
  'remote-user' => 'dmge',
  'ssh-options' => '-o PasswordAuthentication=no -i /home/mcdoolz/.ssh/authorized_keys/dmge_rsa',
  'path-aliases' => array(
    '%dump-dir' => '/tmp/.drush/drush-dumps',
    '%files' => 'sites/default/files',
  ),
);
