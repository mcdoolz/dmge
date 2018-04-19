<?php

/**
 *  @file media_youtube/includes/MediaYouTubeStreamWrapper.inc
 *
 *  Create a Droogle Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $droogle = new DroogleStreamWrapper('droogle://id/sdfasdfsd12434afsa');
 */
class DroogleStreamWrapper extends MediaReadOnlyStreamWrapper {

  // Overrides $base_url defined in MediaReadOnlyStreamWrapper.
  protected $base_url = 'https://docs.google.com';
  protected $file_info = NULL;

  /**
   * Returns a url in the format "https://docs.google.com/a/../document/d/.../edit?usp=drivesdk".
   *
   * Overrides interpolateUrl() defined in MediaReadOnlyStreamWrapper.
   * This is an exact copy of the function in MediaReadOnlyStreamWrapper,
   * here in case that example is redefined or removed.
   */
  function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      $url  = $this->base_url;
      if (!empty($parameters['domain'])) {
        $url .= '/a/' . $parameters['domain'];
      }
      $url .= '/' . $parameters['type'] . '/d/' . $parameters['id'] . '/edit';
      return $url;
    }
  }

  function getFileInfo() {
    return droogle_gfile_load($parameters['id']);
  }

  static function getMimeType($uri, $mapping = NULL) {
    // Since this is a static method, cannot use $this.
    $matches = array();
    preg_match('/id\/([^\/]+)/', $uri, $matches);
    if (!empty($matches[1]) && ($file_info = droogle_gfile_load($matches[1]))) {
      return $file_info['mimeType'];
    }
    return 'application/vnd.google-apps.unknown';
  }

  function getTarget($f) {
    return FALSE;
  }

  function getOriginalThumbnailPath() {
    if ($file_info = $this->getFileInfo()) {
      return $file_info['thumbnailLink'];
    }
  }

  /**
   * Copied with minimal modifications from media_vimeo.
   */
  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    // There's no need to hide thumbnails, always use the public system rather
    // than file_default_scheme().
    $local_path = 'public://media-droogle/' . check_plain($parts['id']) . '.jpg';

    if (!file_exists($local_path)) {
      // getOriginalThumbnailPath throws exception when oEmbed returns error.
      try {
        $dirname = drupal_dirname($local_path);
        file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
        $response = drupal_http_request($this->getOriginalThumbnailPath());

        if (!isset($response->error)) {
          file_unmanaged_save_data($response->data, $local_path, TRUE);
        }
        else {
          @copy($this->getOriginalThumbnailPath(), $local_path);
        }
      }
      catch (Exception $e) {
        // In the event of an endpoint error, use the mime type icon provided
        // by the Media module.

        $icon_dir = variable_get('media_icon_base_directory', 'public://media-icons') . '/' . variable_get('media_icon_set', 'default');
        $local_path = file_icon_path($this->getMimeType($this->uri), $icon_dir);
      }
    }
    return $local_path;
  }
}
