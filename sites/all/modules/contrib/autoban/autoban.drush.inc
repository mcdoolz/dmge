<?php

/**
 * @file
 * Drush support for autoban module.
 */

/**
 * Implements hook_drush_command().
 */
function autoban_drush_command() {
  $commands = array();

  $commands['autoban-ban-all'] = array(
    'description' => 'Autoban ban all.',
    'aliases' => array('autoban'),
  );

  return $commands;
}

/**
 * Drush command logic.
 * drush_[COMMAND_NAME]().
 */
function drush_autoban_ban_all() {
  $count = autoban_ban_all();
  $message = $count ? dt('Autoban: IP ban count=@count', array('@count' => $count)) : dt('Autoban: no IP banned');  
  drush_print($message);
}
