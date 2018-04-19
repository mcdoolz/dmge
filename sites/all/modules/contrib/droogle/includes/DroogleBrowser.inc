<?php

/**
 * @file
 */

class DroogleBrowser extends MediaBrowserPlugin {
  /**
   * Implements MediaBrowserPluginInterface::access().
   */
  public function access($account = NULL) {
    return TRUE;
  }

  /**
   * Implements MediaBrowserPlugin::view()
   */
  public function view() {
    $build['form'] = drupal_get_form('droogle_media_add');
    return $build;
  }
}