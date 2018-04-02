<?php

/**
 * @file
 * Drush Aliases for mykm.ca.
 */

$aliases['live'] = array(
  'uri' => 'weedster.ca',
  'root' => '/home/weedster/public_html',
  'remote-host' => 'weedster.ca',
  'remote-user' => 'weedster',
  'ssh-options' => '-o PasswordAuthentication=no -i /home/mcdoolz/.ssh/authorized_keys/weedster_rsa',
  'path-aliases' => array(
    '%dump-dir' => '/tmp/.drush/drush-dumps',
    '%files' => 'sites/default/files',
  ),
);
