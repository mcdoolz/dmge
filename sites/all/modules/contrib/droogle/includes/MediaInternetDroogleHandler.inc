<?php

/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle Vimeo videos.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetDroogleHandler extends MediaInternetBaseHandler {
  public function parse($embedCode) {
    if (($parts = droogle_parse_url($embedCode)) && self::validId($parts['id'])) {
      $url = 'droogle:/';
      foreach ($parts as $key => $part) {
        $url .= '/' . $key . '/' . $part;
      }
      return file_stream_wrapper_uri_normalize($url);
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    // Try to default the file name to the video's title.
    if (empty($file->fid) && $info = $this->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }

    return $file;
  }

  /**
   * Returns information about the media.
   *
   * See http://www.oembed.com.
   *
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    return NULL;
  }

  /**
   * Check if a drive ID is valid.
   *
   * @return boolean
   *   TRUE if the drive ID is valid, FALSE otherwise.
   */
  static public function validId($id) {
    return droogle_gfile_load($id) ? TRUE : FALSE;
  }
}
