<?php

/**
 * @file
 * Drush integration for patreon.
 */

/**
 * Implements hook_drush_command().
 */
function patreon_drush_command() {
  $items = array();

  $items['pat-download'] = array(
    'callback' => 'patreon_drush_lib_download',
    'description' => dt('Downloads the required Patreon library from https://github.com/Patreon'),
    'aliases' => array('patd'),
    'arguments' => array(
      'path' => dt('Optional. A path to the patreon module. If omitted Drush will use the default location.'),
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function patreon_drush_help($section) {
  switch ($section) {
    case 'drush:pat-download':
      return dt("Downloads the required Patreon library from "
              . "https://github.com/Patreon.");
  }
}

/**
 * Download the Patreon library from github.
 */
function patreon_drush_lib_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/patreon';
  }
  if (substr($path, -7) == 'patreon' && !is_dir($path)) {
    drush_mkdir($path);
  }
  if (is_dir($path . '/src')) {
    drush_log(dt('Patreon already present at @path. No download required.', array('@path' => $path)), 'ok');
  }
  elseif (drush_op('chdir', $path) &&
      drush_shell_exec('wget ' . PATREON_DOWNLOAD_URL . '  -O patreon.zip') &&
      drush_shell_exec('unzip patreon.zip') &&
      drush_shell_exec('mv patreon-php-0.1.0/* .') &&
      drush_shell_exec('rm -rf patreon-php-0.1.0')) {
    drush_log(dt('The latest Patreon library has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Patreon library to @path', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_patreon_post_pm_enable() {
  $extensions = func_get_args();
  // Deal with comma delimited extension list.
  if (strpos($extensions[0], ',') !== FALSE) {
    $extensions = explode(',', $extensions[0]);
  }

  if (in_array('patreon', $extensions) && !drush_get_option('skip')) {
    patreon_drush_lib_download();
  }
}
