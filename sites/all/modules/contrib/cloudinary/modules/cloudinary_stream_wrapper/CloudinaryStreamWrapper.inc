<?php

/**
 * @file
 * Drupal's streamwrapper implementation for Cloudinary service.
 */

/**
 * Implement DrupalStreamWrapperInterface with cloudinary[.folder]://.
 */
class CloudinaryStreamWrapper implements DrupalStreamWrapperInterface {
  /**
   * Instance URI (stream).
   *
   * A stream is referenced as "scheme://target".
   *
   * @var String
   */
  protected $uri;

  /**
   * Folder name as a prefix name of public_id.
   *
   * @var String
   */
  protected $folderName = NULL;

  /**
   * The resource type of Cloudinary (image, raw).
   *
   * @var String
   */
  protected $resourceType = CLOUDINARY_STREAM_WRAPPER_RESOURCE_RAW;

  /**
   * The pointer to the next read or write.
   *
   * @var Int
   */
  protected $streamPointer = 0;

  /**
   * A buffer for reading/wrting.
   *
   * @var String
   */
  protected $streamData = NULL;

  /**
   * This $stream_write property is flagged for data written.
   *
   * @var Boolean
   */
  protected $streamWrite = FALSE;

  /**
   * List of files in a given directory.
   */
  protected $directoryList = array();

  /**
   * A current file resource of Cloudinary.
   *
   * @var Array
   */
  protected $resource = NULL;

  /**
   * Base implementation of setUri().
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Base implementation of getUri().
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Object constructor.
   *
   * Load Cloudinary PHP SDK & initialize Cloudinary configuration.
   */
  public function __construct() {
    // Load Cloudinary PHP SDK.
    libraries_load('cloudinary');
  }

  /**
   * Check uri is an image style.
   */
  protected function imageStylePaths($uri) {
    $paths = explode('/', $this->getTarget($uri));
    $target = array_shift($paths);
    if ($target == 'styles') {
      return $paths;
    }

    return FALSE;
  }

  /**
   * Load file or directory resource for Cloudinary.
   */
  protected function loadResource($uri, $reset = FALSE) {
    // Process image style.
    $paths = $this->imageStylePaths($uri);
    if (!empty($paths)) {
      $style_name = array_shift($paths);
      $scheme = array_shift($paths);
      if (empty($scheme)) {
        return FALSE;
      }
      $path = implode('/', $paths);
      $ori_uri = $scheme . '://' . $path;
      $public_id = $this->getPublicId($ori_uri);
      $resource = cloudinary_stream_wrapper_resource($public_id, array('resource_type' => CLOUDINARY_STREAM_WRAPPER_RESOURCE_IMAGE));

      if (!$resource || $resource['mode'] != CLOUDINARY_STREAM_WRAPPER_FILE) {
        return FALSE;
      }

      // It should be add width and height of original image as parameters.
      $data = cloudinary_stream_wrapper_transformation($style_name, $resource);
      if (!empty($data)) {
        $trans = \Cloudinary::generate_transformation_string($data);
        $trans = '/upload/' . trim($trans, '/') . '/';
        $resource['url'] = str_replace('/upload/', $trans, $resource['url']);
        $resource['secure_url'] = str_replace('/upload/', $trans, $resource['secure_url']);

        // Calculate image width and height with Drupal Image style API.
        $dimensions = array(
          'width' => $resource['width'],
          'height' => $resource['height'],
        );
        image_style_transform_dimensions($style_name, $dimensions);
        $resource = array_merge($resource, $dimensions);
      }

      $this->resource = $resource;
    }
    elseif (!$this->resource || $reset) {
      $public_id = $this->getPublicId($uri);
      $this->resource = cloudinary_stream_wrapper_resource($public_id, array('resource_type' => $this->resourceType));
    }

    return $this->resource;
  }

  /**
   * Get file stream data from Cloudinary by http url.
   */
  protected function streamReadCloudinary() {
    $resource = $this->loadResource($this->uri);
    if (!$resource || empty($resource['url'])) {
      return FALSE;
    }

    $result = drupal_http_request($resource['url']);
    if (!$result->data) {
      return FALSE;
    }

    return $result->data;
  }

  /**
   * Get file status.
   *
   * @return array
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-stat.php
   */
  protected function stat() {
    $resource = $this->loadResource($this->uri);
    if (!$resource) {
      return FALSE;
    }

    $stat = array();
    $stat[0] = $stat['dev'] = 0;
    $stat[1] = $stat['ino'] = 0;
    $stat[2] = $stat['mode'] = $resource['mode'];
    $stat[3] = $stat['nlink'] = 0;
    $stat[4] = $stat['uid'] = 0;
    $stat[5] = $stat['gid'] = 0;
    $stat[6] = $stat['rdev'] = 0;
    $stat[7] = $stat['size'] = $resource['bytes'];
    $stat[8] = $stat['atime'] = $resource['timestamp'];
    $stat[9] = $stat['mtime'] = $resource['timestamp'];
    $stat[10] = $stat['ctime'] = $resource['timestamp'];
    $stat[11] = $stat['blksize'] = 0;
    $stat[12] = $stat['blocks'] = 0;

    return $stat;
  }

  /**
   * Flush the stream buffers.
   */
  protected function flush() {
    $this->folderName = NULL;
    $this->directoryList = array();
    $this->streamData = NULL;
    $this->streamPointer = 0;
    $this->streamWrite = FALSE;
  }

  /**
   * Returns the local writable target of the resource within the stream.
   *
   * This function should be used in place of calls to realpath() or similar
   * functions when attempting to determine the location of a file. While
   * functions like realpath() may return the location of a read-only file, this
   * method may return a URI or path suitable for writing that is completely
   * separate from the URI used for reading.
   *
   * @param string $uri
   *   Optional URI.
   *
   * @return string
   *   Returns a string representing a location suitable for writing of a file,
   *   or FALSE if unable to write to the file such as with read-only streams.
   */
  protected function getTarget($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    list($scheme, $target) = explode('://', $uri, 2);

    // Check the scheme that include folder.
    $pos = strpos($scheme, '.');
    if ($pos) {
      $this->folderName = trim(substr($scheme, $pos + 1));
    }

    // Remove erroneous leading or trailing, forward-slashes and backslashes.
    return trim($target, '\/');
  }

  /**
   * Returns a Cloudinary public_id.
   */
  protected function getPublicId($uri = NULL) {
    $public_id = $this->getTarget($uri);

    // If scheme include folder, prepend it.
    if ($this->folderName) {
      $public_id = $this->folderName . '/' . $public_id;
    }

    if (cloudinary_stream_wrapper_is_image($public_id)) {
      $this->resourceType = CLOUDINARY_STREAM_WRAPPER_RESOURCE_IMAGE;
      $public_id = preg_replace('/(.*)\.(jpe?g|png|gif|bmp)$/i', '\1', $public_id);
    }

    return $public_id;
  }

  /**
   * Returns a web accessible URL for the resource.
   *
   * @return string
   *   A web accessible URL for the resource.
   */
  public function getExternalUrl() {
    $resource = $this->loadResource($this->uri);

    if (!$resource) {
      return FALSE;
    }

    if ((isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on')
      || (isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] === 'https')) {
      return $resource['secure_url'];
    }

    return $resource['url'];
  }

  /**
   * Base implementation of getMimeType().
   */
  static public function getMimeType($uri, $mapping = NULL) {
    if (!isset($mapping)) {
      // The default file map, defined in file.mimetypes.inc is quite big.
      // We only load it when necessary.
      include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
      $mapping = file_mimetype_mapping();
    }

    $extension = '';
    $file_parts = explode('.', drupal_basename($uri));

    // Remove the first part: a full filename should not match an extension.
    array_shift($file_parts);

    // Iterate over the file parts, trying to find a match.
    // For my.awesome.image.jpeg, we try:
    // - jpeg
    // - image.jpeg, and
    // - awesome.image.jpeg
    while ($additional_part = array_pop($file_parts)) {
      $extension = strtolower($additional_part . ($extension ? '.' . $extension : ''));
      if (isset($mapping['extensions'][$extension])) {
        return $mapping['mimetypes'][$mapping['extensions'][$extension]];
      }
    }

    return 'application/octet-stream';
  }

  /**
   * Base implementation of chmod().
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * Base implementation of realpath().
   */
  public function realpath() {
    return trim($this->uri, '\/');
  }

  /**
   * Support for fopen(), file_get_contents(), file_put_contents() etc.
   *
   * @param string $uri
   *   A string containing the URI to the file to open.
   * @param string $mode
   *   The file mode ("r", "wb" etc.).
   * @param string $options
   *   A bit mask of STREAM_USE_PATH and STREAM_REPORT_ERRORS.
   * @param string $opened_path
   *   A string containing the path actually opened.
   *
   * @return bool
   *   Returns TRUE if file was opened successfully.
   *
   * @see http://php.net/manual/streamwrapper.stream-open.php
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    $this->uri = $uri;

    // If this stream is being opened for writing, clear the object buffer
    // Return true as we'll create the object on flush call.
    if (strpbrk($mode, 'wax')) {
      $this->flush();
      $this->streamWrite = TRUE;
      return TRUE;
    }

    $resource = $this->loadResource($uri);

    if ($resource) {
      $this->flush();
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Support for flock().
   *
   * @param string $operation
   *   One of the following:
   *   - LOCK_SH to acquire a shared lock (reader).
   *   - LOCK_EX to acquire an exclusive lock (writer).
   *   - LOCK_UN to release a lock (shared or exclusive).
   *   - LOCK_NB if you don't want flock() to block while locking (not
   *     supported on Windows).
   *
   * @return bool
   *   Always returns TRUE at the present time.
   *
   * @see http://php.net/manual/streamwrapper.stream-lock.php
   */
  public function stream_lock($operation) {
    return FALSE;
  }

  /**
   * Support for fread(), file_get_contents() etc.
   *
   * @param int $count
   *   Maximum number of bytes to be read.
   *
   * @return string
   *   The string that was read, or FALSE in case of an error.
   *
   * @see http://php.net/manual/streamwrapper.stream-read.php
   */
  public function stream_read($count) {
    if (!$this->streamData) {
      $stream_data = $this->streamReadCloudinary();

      if (!$stream_data) {
        return FALSE;
      }

      $this->streamData = $stream_data;
    }

    $data = substr($this->streamData, $this->streamPointer, $count);
    $this->streamPointer += $count;

    return $data;
  }

  /**
   * Support for fwrite(), file_put_contents() etc.
   *
   * @param string $data
   *   The string to be written.
   *
   * @return int
   *   The number of bytes written (integer).
   *
   * @see http://php.net/manual/streamwrapper.stream-write.php
   */
  public function stream_write($data) {
    // Write when flushed.
    $this->streamWrite = TRUE;
    $this->streamData .= $data;
    // Calculate date size.
    $bytes = strlen($data);
    $this->streamPointer += $bytes;

    return $bytes;
  }

  /**
   * Support for feof().
   *
   * @return bool
   *   TRUE if end-of-file has been reached.
   *
   * @see http://php.net/manual/streamwrapper.stream-eof.php
   */
  public function stream_eof() {
    if (!$this->streamData) {
      $stream_data = $this->streamReadCloudinary();

      if (!$stream_data) {
        return TRUE;
      }

      $this->streamData = $stream_data;
    }

    return $this->streamPointer >= strlen($this->streamData);
  }

  /**
   * Support for fseek().
   *
   * @param int $offset
   *   The byte offset to got to.
   * @param string $whence
   *   SEEK_SET, SEEK_CUR, or SEEK_END.
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/streamwrapper.stream-seek.php
   */
  public function stream_seek($offset, $whence) {
    $seek = FALSE;

    switch ($whence) {
      case SEEK_SET:
        if (strlen($this->streamData) >= $offset && $offset >= 0) {
          $this->streamPointer = $offset;
          $seek = TRUE;
        }
        break;

      case SEEK_CUR:
        if ($offset >= 0) {
          $this->streamPointer += $offset;
          $seek = TRUE;
        }
        break;

      case SEEK_END:
        if (strlen($this->streamData) + $offset >= 0) {
          $this->streamPointer = strlen($this->streamData) + $offset;
          $seek = TRUE;
        }
        break;
    }

    return $seek;
  }

  /**
   * Support for fflush().
   *
   * @return bool
   *   TRUE if data was successfully stored (or there was no data to store).
   *
   * @see http://php.net/manual/streamwrapper.stream-flush.php
   */
  public function stream_flush() {
    if ($this->streamWrite) {
      $public_id = $this->getPublicId($this->uri);
      $base64_data = 'data:' . self::getMimeType($this->uri) . ';base64,' . base64_encode($this->streamData);
      $dirname = dirname($public_id);
      if ($dirname == '.') {
        $dirname = '';
      }

      $options = array(
        'public_id' => $public_id,
        'resource_type' => $this->resourceType,
        'tags' => CLOUDINARY_STREAM_WRAPPER_FOLDER_TAG_PREFIX . $dirname,
      );

      if (cloudinary_stream_wrapper_create_file($base64_data, $options)) {
        // Unset resource of static variables after new file uploaded.
        cloudinary_stream_wrapper_resource($public_id, $options, TRUE);

        return TRUE;
      }
    }

    $this->flush();

    return FALSE;
  }

  /**
   * Support for ftell().
   *
   * @return int
   *   The current offset in bytes from the beginning of file.
   *
   * @see http://php.net/manual/streamwrapper.stream-tell.php
   */
  public function stream_tell() {
    return $this->streamPointer;
  }

  /**
   * Support for fstat().
   *
   * @return array
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/streamwrapper.stream-stat.php
   */
  public function stream_stat() {
    return $this->stat();
  }

  /**
   * Support for fclose().
   *
   * @return bool
   *   TRUE if stream was successfully closed.
   *
   * @see http://php.net/manual/streamwrapper.stream-close.php
   */
  public function stream_close() {
    $this->flush();

    return TRUE;
  }

  /**
   * Support for unlink().
   *
   * @param string $uri
   *   A string containing the URI to the resource to delete.
   *
   * @return bool
   *   TRUE if resource was successfully deleted.
   *
   * @see http://php.net/manual/streamwrapper.unlink.php
   */
  public function unlink($uri) {
    // If uri is an image style then ignore it.
    $paths = $this->imageStylePaths($uri);
    if ($paths !== FALSE) {
      return TRUE;
    }

    $resource = $this->loadResource($uri);

    if ($resource) {
      return cloudinary_stream_wrapper_delete_resource($resource);
    }

    return FALSE;
  }

  /**
   * Support for rename().
   *
   * @param string $from_uri
   *   The URI to the file to rename.
   * @param string $to_uri
   *   The new URI for file.
   *
   * @return bool
   *   TRUE if file was successfully renamed.
   *
   * @see http://php.net/manual/streamwrapper.rename.php
   */
  public function rename($from_uri, $to_uri) {
    // Check from_uri exist on Cloudinary.
    $from_resource = $this->loadResource($from_uri);

    if (!$from_resource) {
      return FALSE;
    }
    // Doesn't support folder rename.
    elseif ($from_resource['mode'] != CLOUDINARY_STREAM_WRAPPER_FILE) {
      return FALSE;
    }

    // Check to_uri exist on Cloudinary.
    $to_resource = $this->loadResource($to_uri, TRUE);

    if ($to_resource) {
      return FALSE;
    }

    // Return false if different resource type.
    $to_resource_type = cloudinary_stream_wrapper_is_image($to_uri) ? CLOUDINARY_STREAM_WRAPPER_RESOURCE_IMAGE : CLOUDINARY_STREAM_WRAPPER_RESOURCE_RAW;

    if ($from_resource['resource_type' != $to_resource_type]) {
      return FALSE;
    }

    $to_public_id = $this->getPublicId($to_uri);

    return cloudinary_stream_wrapper_rename_file($from_resource, $to_public_id);
  }

  /**
   * Gets the name of the directory from a given path.
   *
   * This method is usually accessed through drupal_dirname(), which wraps
   * around the PHP dirname() function because it does not support stream
   * wrappers.
   *
   * @param string $uri
   *   A URI or path.
   *
   * @return string
   *   A string containing the directory name.
   *
   * @see drupal_dirname()
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $target  = $this->getTarget($uri);
    $dirname = dirname($target);

    if ($dirname == '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }

  /**
   * Support for mkdir().
   *
   * @param string $uri
   *   A string containing the URI to the directory to create.
   * @param string $mode
   *   Permission flags - see mkdir().
   * @param string $options
   *   A bit mask of STREAM_REPORT_ERRORS and STREAM_MKDIR_RECURSIVE.
   *
   * @return bool
   *   TRUE if directory was successfully created.
   *
   * @see http://php.net/manual/streamwrapper.mkdir.php
   */
  public function mkdir($uri, $mode, $options) {
    $resource = $this->loadResource($uri);

    if ($resource) {
      return TRUE;
    }

    $public_id = $this->getPublicId($uri);

    return cloudinary_stream_wrapper_create_folder($public_id);
  }

  /**
   * Support for rmdir().
   *
   * @param string $uri
   *   A string containing the URI to the directory to delete.
   * @param string $options
   *   A bit mask of STREAM_REPORT_ERRORS.
   *
   * @return bool
   *   TRUE if directory was successfully removed.
   *
   * @see http://php.net/manual/streamwrapper.rmdir.php
   */
  public function rmdir($uri, $options) {
    $resource = $this->loadResource($uri);

    if ($resource) {
      return cloudinary_stream_wrapper_delete_folder($resource);
    }

    return FALSE;
  }

  /**
   * Support for stat().
   *
   * @param string $uri
   *   A string containing the URI to get information about.
   * @param string $flags
   *   A bit mask of STREAM_URL_STAT_LINK and STREAM_URL_STAT_QUIET.
   *
   * @return array
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/streamwrapper.url-stat.php
   */
  public function url_stat($uri, $flags) {
    $this->uri = $uri;

    return $this->stat();
  }

  /**
   * Support for opendir().
   *
   * @param string $uri
   *   A string containing the URI to the directory to open.
   * @param string $options
   *   Unknown (parameter is not documented in PHP Manual).
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/streamwrapper.dir-opendir.php
   */
  public function dir_opendir($uri, $options) {
    $resource = $this->loadResource($uri);

    if ($resource) {
      $list = array('.', '..');

      if (isset($this->resource['folders']) && !empty($this->resource['folders'])) {
        $list = array_merge($list, $this->resource['folders']);
      }

      if (isset($this->resource['files']) && !empty($this->resource['files'])) {
        $list = array_merge($list, $this->resource['files']);
      }

      // Append default file "sample.jpg" into root folder.
      $public_id = $this->getPublicId($uri);
      if ('' == $public_id && !in_array(CLOUDINARY_STREAM_WRAPPER_SAMPLE, $list)) {
        $list[] = CLOUDINARY_STREAM_WRAPPER_SAMPLE;
      }

      sort($list);
      $this->directoryList = $list;

      return TRUE;
    }

    return FALSE;
  }

  /**
   * Support for readdir().
   *
   * @return string
   *   The next filename, or FALSE if there are no more files in the directory.
   *
   * @see http://php.net/manual/streamwrapper.dir-readdir.php
   */
  public function dir_readdir() {
    $filename = current($this->directoryList);

    if ($filename !== FALSE) {
      next($this->directoryList);
    }

    return $filename;
  }

  /**
   * Support for rewinddir().
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/streamwrapper.dir-rewinddir.php
   */
  public function dir_rewinddir() {
    reset($this->directoryList);

    return TRUE;
  }

  /**
   * Support for closedir().
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/streamwrapper.dir-closedir.php
   */
  public function dir_closedir() {
    $this->directoryList = array();

    return TRUE;
  }

}
