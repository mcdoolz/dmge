<?php
/**
 * @file
 * Hook and helper functions for the Minify JS module.
 */

/**
 * Helper function for form table:
 *  - admin/config/development/performance/js.
 */
function minifyjs_manage_javascript_files_form($form, &$form_state) {
  drupal_set_title(t('Manage Javascript Files'));

  // Build the rows of the table.
  $files = minifyjs_load_all_files();
  $form  = array();
  $rows  = array();
  if (count($files)) {
    foreach ($files as $fid => $file) {
      $rows[$fid] = array(
        l($file->uri, $file->uri, array('attributes' => array('target' => '_blank'))),
        date('Y-m-d', $file->modified),
        minifyjs_format_filesize($file->size),
        ($file->minified_size > 0) ? minifyjs_format_filesize($file->minified_size) : '-',
        ($file->minified_size > 0) ? round(($file->size - $file->minified_size) / $file->size * 100, 2) . '%' : '-',
        ($file->minified_modified > 0) ? date('Y-m-d', $file->minified_modified) : '-',
        (!empty($file->minified_uri)) ? l(basename($file->minified_uri), file_create_url($file->minified_uri), array('attributes' => array('target' => '_blank'))) : '-',
        minifyjs_operations($file),
      );
    }
  }

  // The table.
  $form['files'] = array(
    '#type'    => 'tableselect',
    '#header'  => array(
      t('Original File'),
      t('Last Modified'),
      t('Original Size'),
      t('Minified Size'),
      t('Savings'),
      t('Last Minified'),
      t('Minified File'),
      t('Operations'),
    ),
    '#options' => $rows,
    '#empty'   => t('No files have been found. Please scan using the action link above.'),
  );

  // Bulk minify button.
  if (count($files)) {
    $form['actions'] = array(
      '#type'       => 'container',
      '#attributes' => array(
        'class' => array('container-inline'),
      ),
    );
    $form['actions']['action'] = array(
      '#type'       => 'select',
      '#options'    => array(
        'minify'      => t('Minify (and re-minify)'),
        'minify_skip' => t('Minify (and skip minified)'),
        'restore'     => t('Restore'),
      ),
    );
    $form['actions']['go'] = array(
      '#type'       => 'submit',
      '#value'      => t('Perform action on selected files'),
      '#submit'     => array('minifyjs_manage_javascript_files_form_submit'),
    );
  }

  return $form;
}

/**
 * Helper function for form table (form submit):
 *  - admin/config/development/performance/js.
 */
function minifyjs_manage_javascript_files_form_submit($form, &$form_state) {
  if (count($form_state['values']['files'])) {
    $files = minifyjs_load_all_files();

    // Build operations
    $operations = array();
    foreach ($form_state['values']['files'] as $fid => $selected) {
      if ($selected) {
        switch ($form_state['values']['action']) {

          // minify all files.
          case 'minify':
            $operations[] = array('minifyjs_minify_file_operation', array($fid));
            break;

          // minify files that have not yet been minified.
          case 'minify_skip':
            $file = $files[$fid];
            if (!$file->minified_uri) {
              $operations[] = array('minifyjs_minify_file_operation', array($fid));
            }
            break;

          // restore un-minified version of a file.
          case 'restore':
            $operations[] = array('minifyjs_remove_minified_file_operation', array($fid));
            break;
        }
      }
    }

    // Build the batch.
    $batch = array(
      'operations'    => $operations,
      'file'          => drupal_get_path('module', 'minifyjs') . '/minifyjs.admin.inc',
      'error_message' => t('There was an unexpected error while processing the batch.'),
      'finished'      => 'minifyjs_batch_finished',
    );
    switch ($form_state['values']['action']) {
      case 'minify':
        $batch['title']        = t('Minifying Javascript Files.');
        $batch['init_message'] = t('Initializing minify javascript files batch.');
        break;

      case 'restore':
        $batch['title']        = t('Restoring Un-Minified Javascript Files.');
        $batch['init_message'] = t('Initializing restore un-minified javascript files batch.');
        break;

    }

    // Start the batch.
    batch_set($batch);
  }
}

/**
 * Helper function for batch, minify file operation.
 */
function minifyjs_minify_file_operation($fid, &$context) {

  // Set the context message
  $files = minifyjs_load_all_files();
  $file  = $files[$fid];
  $context['message'] = t('Minifying file - %file', array('%file' => $file->uri));

  // Perform the minify
  $result = minifyjs_minify_file($fid);
  if ($result === TRUE) {
    $context['results']['success'][] = t('File %file was minified successfully.', array('%file' => $file->uri));
  }
  else {
    $context['results']['errors'][] = $result;
  }
}

/**
 * Helper function for batch, remove minified file operation.
 */
function minifyjs_remove_minified_file_operation($fid, &$context) {

  // Set the context message
  $files = minifyjs_load_all_files();
  $file  = $files[$fid];
  $context['message'] = t('Restoring un-minified file - %file', array('%file' => $file->uri));

  // Perform the restore
  $result = minifyjs_remove_minified_file($fid);
  if ($result === TRUE) {
    $context['results']['success'][] = t('File %file was restored successfully.', array('%file' => $file->uri));
  }
  else {
    $context['results']['errors'][] = $result;
  }
}

/**
 * Helper function for batch, batch finished.
 */
function minifyjs_batch_finished($success, $results, $operations) {

  // Output successful operations.
  if (isset($results['success'])) {
    if (count($results['success'])) {
      foreach ($results['success'] as $success) {
        drupal_set_message($success);
      }
    }
  }

  // Output errors.
  if (isset($results['errors'])) {
    if (count($results['errors'])) {
      foreach ($results['errors'] as $error) {
        drupal_set_message($error, 'error');
      }
    }
  }

  // Clear the cache so this change will be reflected in
  // minifyjs_load_all_files()
  cache_clear_all(MINIFYJS_CACHE_CID, 'cache');

  drupal_goto('admin/config/development/performance/js');
}

/**
 * Helper function to scan the file system for javascript files:
 *  - admin/config/development/performance/js/scan.
 */
function minifyjs_scan_for_javascript_files() {

  // Recursive scan of the entire doc root to find .js files (excluding .min.js).
  $directory = new RecursiveDirectoryIterator(DRUPAL_ROOT, FilesystemIterator::FOLLOW_SYMLINKS);
  $iterator  = new RecursiveIteratorIterator($directory);
  $regex     = new RegexIterator($iterator, '/^.+(?<!\.min)\.js$/i');

  // Process files.
  $new_files = array();
  $old_files = array();
  $existing  = minifyjs_load_all_files();

  foreach ($regex as $info) {
    $new_absolute = $info->getPathname();
    $new_relative = str_replace(DRUPAL_ROOT . '/', '', $new_absolute);

    // Loop existing and see if it already exists from previous scans.
    $exists = FALSE;
    foreach ($existing as $file) {
      if ($file->uri == $new_relative) {
        $exists = TRUE;
        $old_files[$new_relative] = TRUE;
        break;
      }
    }

    // File not found in the existing array, so it's new.
    if (!$exists) {
      $new_files[$new_absolute] = TRUE;
    }
  }

  // Build a list of files that currently exist in the minifyjs_file table but
  // no longer exist in the file system. These files should be removed.
  $remove_files = array();
  foreach ($existing as $file) {
    if (!isset($old_files[$file->uri])) {
      minifyjs_remove_file($file->uri);
    }
  }

  // Add all new files to the database.
  foreach ($new_files as $file => $junk) {
    db_insert('minifyjs_file')
      ->fields(
        array(
          'uri'      => str_replace(DRUPAL_ROOT . '/', '', $file),
          'size'     => filesize($file),
          'modified' => filemtime($file),
        )
      )
      ->execute();
  }

  // Clear the cache so all of these new files will be picked up.
  cache_clear_all(MINIFYJS_CACHE_CID, 'cache');

  drupal_goto('admin/config/development/performance/js');
}

/**
 * Helper function to create the minified file:
 *  - admin/config/development/performance/js/%js/minify.
 */
function minifyjs_create_minified_file($fid) {
  $result = minifyjs_minify_file($fid, TRUE);

  if ($result === TRUE) {
    drupal_set_message(t('File was minified successfully.'));
  }
  else {
    drupal_set_message($result, 'error');
  }

  drupal_goto('admin/config/development/performance/js');
}

/**
 * Helper function to restore the un-minified file:
 *  - admin/config/development/performance/js/%js/restore.
 */
function minifyjs_restore_original_file($fid) {
  $result = minifyjs_remove_minified_file($fid, TRUE);

  if ($result === TRUE) {
    drupal_set_message(t('File was restored successfully.'));
  }
  else {
    drupal_set_message($result, 'error');
  }

  drupal_goto('admin/config/development/performance/js');
}

/**
 * Helper function to format the filesize.
 */
function minifyjs_format_filesize($size) {
  $suffixes   = array('', 'k', 'M', 'G', 'T');
  $base       = log($size) / log(1024);
  $base_floor = floor($base);

  return round(pow(1024, $base - $base_floor), 2) . $suffixes[$base_floor];
}

/**
 * Helper function to return the operations available for the file.
 */
function minifyjs_operations($file) {
  $operations = array();

  if (empty($file->minified_uri)) {
    $operations[] = l(t('Minify'), 'admin/config/development/performance/js/' . $file->fid . '/minify');
  }
  else {
    $operations[] = l(t('Re-Minify'), 'admin/config/development/performance/js/' . $file->fid . '/minify');
    $operations[] = l(t('Restore'), 'admin/config/development/performance/js/' . $file->fid . '/restore');
  }

  return implode(' | ', $operations);
}

/**
 * Helper function that sends the JS off to be minified, handles the response,
 * stores the file in the filesystem and stores the file info in the managed
 * file tables.
 */
function minifyjs_minify_file($fid, $reset = FALSE) {
  global $user;

  // Load the file by fid.
  $files = minifyjs_load_all_files();
  $file  = $files[$fid];
  $js    = file_get_contents(DRUPAL_ROOT . '/' . $file->uri);

  // Minify the JS
  $minifier = new Patchwork\JSqueeze();
  $minified = $minifier->squeeze($js, TRUE, FALSE);

  // Create the directory tree if it doesn't exist.
  $minifyjs_folder = 'public://minifyjs/' . dirname($file->uri);
  if (!is_dir($minifyjs_folder)) {
    mkdir($minifyjs_folder, variable_get('file_chmod_directory', 0775), TRUE);
  }

  // Save the file first to the /tmp folder and then copy to the
  // public filesystem.
  $file_name = str_replace('.js', '.min.js', basename($file->uri));
  $tmp_file  = '/tmp/' . $file_name;
  $file_uri  = 'public://minifyjs/' . dirname($file->uri) . '/' . $file_name;
  if (file_put_contents($tmp_file, $minified)) {
    if (copy($tmp_file, $file_uri)) {

      // save the file in the managed file table.
      if (empty($file->minified_uri)) {
        $file           = new stdClass();
        $file->fid      = NULL;
        $file->uri      = $file_uri;
        $file->filename = $file_name;
        $file->filemime = file_get_mimetype($file->uri);
        $file->uid      = $user->uid;
        $file->status   = FILE_STATUS_PERMANENT;
        $file           = file_save($file);
        file_usage_add($file, 'minifyjs', 'node', 1);
      }

      $filesize = filesize($file_uri);

      // update the minifyjs table
      db_update('minifyjs_file')
        ->fields(
          array(
            'minified_uri'      => $file_uri,
            'minified_size'     => ($filesize) ? $filesize : 0,
            'minified_modified' => REQUEST_TIME,
          )
        )
        ->condition('fid', $fid)
        ->execute();

      // Clean up /tmp folder
      unlink($tmp_file);

      // Clear the cache so this change will be reflected in
      // minifyjs_load_all_files()
      if ($reset) {
        cache_clear_all(MINIFYJS_CACHE_CID, 'cache');
      }

      return TRUE;
    }
    else {
      return t('Could not copy the file from the /tmp folder.');
    }
  }
  else {
    return t('Could not save the file - %file', array('%file' => '/tmp/' . $file_name));
  }
}

/**
 * Helper function removes the file, the entry in the file_managed table and
 * sets the file status as unminified.
 */
function minifyjs_remove_minified_file($fid, $reset = FALSE) {

  // Get minified uri from the minifyjs_file table.
  $result = db_select('minifyjs_file', 'm')
    ->fields('m', array('minified_uri'))
    ->condition('m.fid', $fid)
    ->execute();
  if ($result->rowCount()) {
    $file = $result->fetchObject();

    // Get the fid of the minified table.
    $result = db_select('file_managed', 'f')
      ->fields('f', array('fid'))
      ->condition('f.uri', $file->minified_uri)
      ->execute();
    if ($result->rowCount()) {
      $file = $result->fetchObject();

      // Remove the file from the file_managed table
      $file = file_load($file->fid);
      file_delete($file, TRUE);

      // Set the file status to non-minfied.
      db_update('minifyjs_file')
        ->fields(
          array(
            'minified_uri'      => '',
            'minified_size'     => 0,
            'minified_modified' => 0,
          )
        )
        ->condition('fid', $fid)
        ->execute();

      // Clear the cache so this change will be reflected in
      // minifyjs_load_all_files()
      if ($reset) {
        cache_clear_all(MINIFYJS_CACHE_CID, 'cache');
      }

      return TRUE;
    }
  }

  return t('File not found. Check that the file ID is correct.');
}

/**
 * Helper function removes the file, the entry in the file_managed table and
 * the entry in the minifyjs_file.
 */
function minifyjs_remove_file($file_uri) {

  // Get the fid and minified uri of the file
  $result = db_select('minifyjs_file', 'm')
    ->fields('m', array('fid', 'minified_uri'))
    ->condition('m.uri', $file_uri)
    ->execute();
  if ($result->rowCount()) {
    $file = $result->fetchObject();

    // Handle the minified file, if applicable.
    if (!empty($file->minified_uri)) {

      // Get the fid of the minified file.
      $result = db_select('file_managed', 'f')
        ->fields('f', array('fid'))
        ->condition('f.uri', $file->minified_uri)
        ->execute();
      if ($result->rowCount()) {
        $minified_file = $result->fetchObject();

        // Remove the file from the file_managed table
        $minified_file = file_load($minified_file->fid);
        file_delete($minified_file, TRUE);
      }
    }

    // Remove the file from minifyjs_file table.
    db_delete('minifyjs_file')
      ->condition('fid', $file->fid)
      ->execute();

    return TRUE;
  }
}