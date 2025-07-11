<?php

/**
 * @file
 * Webform module file component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_file() {
  // If private file storage is enabled, make it the default for security
  // reasons. See: https://www.drupal.org/psa-2016-003
  $available_schemes = file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE);
  $scheme = isset($available_schemes['private']) ? 'private' : 'public';

  return array(
    'name' => '',
    'form_key' => NULL,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'filtering' => array(
        'types' => array('gif', 'jpg', 'png'),
        'addextensions' => '',
        'size' => '2 MB',
      ),
      'rename' => '',
      'scheme' => $scheme,
      'directory' => '',
      'progress_indicator' => 'throbber',
      'title_display' => 0,
      'description' => '',
      'description_above' => FALSE,
      'attributes' => array(),
      'private' => FALSE,
      'analysis' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_file() {
  return array(
    'webform_edit_file_extensions' => array(
      'render element' => 'element',
      'file' => 'components/file.inc',
    ),
    'webform_display_file' => array(
      'render element' => 'element',
      'file' => 'components/file.inc',
    ),
    'webform_managed_file' => array(
      'render element' => 'element',
      'file' => 'components/file.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_file($component) {
  $form = array();
  $form['#element_validate'] = array('_webform_edit_file_check_directory');
  $form['#after_build'] = array('_webform_edit_file_check_directory');

  $form['validation']['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Max upload size'),
    '#default_value' => $component['extra']['filtering']['size'],
    '#description' => t('Enter the max file size a user may upload such as 2 MB or 800 KB. Your server has a max upload size of @size.', array('@size' => format_size(file_upload_max_size()))),
    '#size' => 10,
    '#parents' => array('extra', 'filtering', 'size'),
    '#element_validate' => array('_webform_edit_file_size_validate'),
    '#weight' => 1,
  );

  $form['validation']['extensions'] = array(
    '#element_validate' => array('_webform_edit_file_extensions_validate'),
    '#parents' => array('extra', 'filtering'),
    '#theme' => 'webform_edit_file_extensions',
    '#theme_wrappers' => array('form_element'),
    '#title' => t('Allowed file extensions'),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'webform') . '/js/webform-admin.js'),
      'css' => array(drupal_get_path('module', 'webform') . '/css/webform-admin.css'),
    ),
    '#type' => 'webform_file_extensions',
    '#weight' => 2,
  );

  // Find the list of all currently valid extensions.
  $current_types = isset($component['extra']['filtering']['types']) ? $component['extra']['filtering']['types'] : array();

  $types = array('gif', 'jpg', 'png');
  $form['validation']['extensions']['types']['webimages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Web images'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('bmp', 'eps', 'tif', 'pict', 'psd');
  $form['validation']['extensions']['types']['desktopimages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Desktop images'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('txt', 'rtf', 'html', 'pdf', 'doc', 'docx', 'odt', 'ppt', 'pptx', 'odp', 'xls', 'xlsx', 'ods', 'xml');
  $form['validation']['extensions']['types']['documents'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Documents'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('avi', 'mov', 'mp3', 'ogg', 'wav');
  $form['validation']['extensions']['types']['media'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Media'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $types = array('bz2', 'dmg', 'gz', 'jar', 'rar', 'sit', 'tar', 'zip');
  $form['validation']['extensions']['types']['archives'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Archives'),
    '#options' => drupal_map_assoc($types),
    '#default_value' => array_intersect($current_types, $types),
  );

  $form['validation']['extensions']['addextensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional extensions'),
    '#default_value' => $component['extra']['filtering']['addextensions'],
    '#description' => t('Enter a list of additional file extensions for this upload field, separated by commas.<br /> Entered extensions will be appended to checked items above.'),
    '#size' => 20,
    '#weight' => 3,
  );

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['extra']['scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $component['extra']['scheme'],
    '#description' => t('Public files upload destination is dangerous for forms that are available to anonymous and/or untrusted users. For more information, see <a href="@psa">DRUPAL-PSA-2016-003</a>.', array('@psa' => 'https://www.drupal.org/psa-2016-003')),
    '#weight' => 4,
    '#access' => count($scheme_options) > 1,
  );
  $form['extra']['directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Upload directory'),
    '#default_value' => $component['extra']['directory'],
    '#description' => t('You may optionally specify a sub-directory to store your files.') . ' ' . theme('webform_token_help'),
    '#weight' => 5,
    '#field_prefix' => 'webform/',
  );

  $form['extra']['rename'] = array(
    '#type' => 'textfield',
    '#title' => t('Rename files'),
    '#default_value' => $component['extra']['rename'],
    '#description' => t('You may optionally use tokens to create a pattern used to rename files upon submission. Omit the extension; it will be added automatically.') . ' ' . theme('webform_token_help', array('groups' => array('node', 'submission'))),
    '#weight' => 6,
    '#element_validate' => array('_webform_edit_file_rename_validate'),
    '#access' => webform_variable_get('webform_token_access'),
  );

  $form['display']['progress_indicator'] = array(
    '#type' => 'radios',
    '#title' => t('Progress indicator'),
    '#options' => array(
      'throbber' => t('Throbber'),
      'bar' => t('Bar with progress meter'),
    ),
    '#default_value' => $component['extra']['progress_indicator'],
    '#description' => t('The throbber display does not show the status of uploads but takes up less space. The progress bar is helpful for monitoring progress on large uploads.'),
    '#weight' => 16,
    '#access' => file_progress_implementation(),
    '#parents' => array('extra', 'progress_indicator'),
  );

  // TODO: Make managed_file respect the "size" parameter.
  /*
  $form['display']['width'] = array(
  '#type' => 'textfield',
  '#title' => t('Width'),
  '#default_value' => $component['extra']['width'],
  '#description' => t('Width of the file field.') . ' ' . t('Leaving blank will use the default size.'),
  '#size' => 5,
  '#maxlength' => 10,
  '#weight' => 4,
  '#parents' => array('extra', 'width')
  );
   */

  return $form;
}

/**
 * A Form API element validate function to ensure that the rename string is
 * either empty or contains at least one token.
 */
function _webform_edit_file_rename_validate($element, &$form_state, $form) {
  $rename = trim($form_state['values']['extra']['rename']);
  form_set_value($element, $rename, $form_state);
  if (strlen($rename) && !count(token_scan($rename))) {
    form_error($element, t('To create unique file names, use at least one token in the file name pattern.'));
  }
}

/**
 * A Form API element validate function to check filesize is valid.
 */
function _webform_edit_file_size_validate($element) {
  if (!empty($element['#value'])) {
    $set_filesize = parse_size($element['#value']);
    if ($set_filesize == FALSE) {
      form_error($element, t('File size @value is not a valid file size. Use a value such as 2 MB or 800 KB.', array('@value' => $element['#value'])));
    }
    else {
      $max_filesize = parse_size(file_upload_max_size());
      if ($max_filesize < $set_filesize) {
        form_error($element, t('An upload size of @value is too large. You are allowed to upload files that are @max or less.', array('@value' => $element['#value'], '@max' => format_size($max_filesize))));
      }
    }
  }
}

/**
 * A Form API after build and validate function.
 *
 * Ensure that the destination directory exists and is writable.
 */
function _webform_edit_file_check_directory($element) {
  $scheme = $element['extra']['scheme']['#value'];
  $directory = $element['extra']['directory']['#value'];

  $destination_dir = file_stream_wrapper_uri_normalize($scheme . '://webform/' . $directory);
  $tokenized_dir = drupal_strtolower(webform_replace_tokens($destination_dir, $element['#node']));

  // Sanity check input to prevent use parent (../) directories.
  if (preg_match('/\.\.[\/\\\]/', $tokenized_dir . '/')) {
    form_error($element['extra']['directory'], t('The save directory %directory is not valid.', array('%directory' => $tokenized_dir)));
  }
  else {
    if (!file_prepare_directory($tokenized_dir, FILE_CREATE_DIRECTORY)) {
      form_error($element['extra']['directory'], t('The save directory %directory could not be created. Check that the webform files directory is writable.', array('%directory' => $tokenized_dir)));
    }
  }

  return $element;
}

/**
 * A Form API element validate function.
 *
 * Change the submitted values of the component so that all filtering extensions
 * are saved as a single array.
 */
function _webform_edit_file_extensions_validate($element, &$form_state) {
  // Predefined types.
  $extensions = array();
  foreach (element_children($element['types']) as $category) {
    foreach (array_keys($element['types'][$category]['#value']) as $extension) {
      if ($element['types'][$category][$extension]['#value']) {
        $extensions[] = $extension;
        // "jpeg" is an exception. It is allowed anytime "jpg" is allowed.
        if ($extension == 'jpg') {
          $extensions[] = 'jpeg';
        }
      }
    }
  }

  // Additional types.
  $additional_extensions = explode(',', $element['addextensions']['#value']);
  foreach ($additional_extensions as $extension) {
    $clean_extension = drupal_strtolower(trim($extension));
    if (!empty($clean_extension) && !in_array($clean_extension, $extensions)) {
      $extensions[] = $clean_extension;
    }
  }

  form_set_value($element['types'], $extensions, $form_state);
}

/**
 * Output the list of allowed extensions as checkboxes.
 */
function theme_webform_edit_file_extensions($variables) {
  $element = $variables['element'];

  // Format the components into a table.
  $rows = array();
  foreach (element_children($element['types']) as $filtergroup) {
    $row = array();
    if ($element['types'][$filtergroup]['#type'] == 'checkboxes') {
      $select_link = ' <a href="#" class="webform-select-link webform-select-link-' . $filtergroup . '">(' . t('select') . ')</a>';
      $row[] = $element['types'][$filtergroup]['#title'];
      $row[] = array('data' => $select_link, 'width' => 40);
      $row[] = array('data' => drupal_render_children($element['types'][$filtergroup]), 'class' => array('webform-file-extensions', 'webform-select-group-' . $filtergroup));
      $rows[] = array('data' => $row);
      unset($element['types'][$filtergroup]);
    }
  }

  // Add the row for additional types.
  if (!isset($element['addextensions']['#access']) || $element['addextensions']['#access']) {
    $row = array();
    $title = $element['addextensions']['#title'];
    $element['addextensions']['#title'] = NULL;
    $row[] = array('data' => $title, 'colspan' => 2);
    $row[] = drupal_render($element['addextensions']);
    $rows[] = $row;
  }

  $header = array(array('data' => t('Category'), 'colspan' => '2'), array('data' => t('Types')));

  // Create the table inside the form.
  $element['types']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('webform-file-extensions')),
  );

  return drupal_render_children($element);
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_file($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  // Cap the upload size according to the PHP limit.
  $max_filesize = parse_size(file_upload_max_size());
  $set_filesize = $component['extra']['filtering']['size'];
  if (!empty($set_filesize) && parse_size($set_filesize) < $max_filesize) {
    $max_filesize = parse_size($set_filesize);
  }

  $element = array(
    '#type' => 'managed_file',
    '#theme' => 'webform_managed_file',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['required'],
    '#default_value' => isset($value[0]) ? $value[0] : NULL,
    '#attributes' => $component['extra']['attributes'],
    '#upload_validators' => array(
      'file_validate_size' => array($max_filesize),
      'file_validate_extensions' => array(implode(' ', $component['extra']['filtering']['types'])),
    ),
    '#pre_render' => array_merge(element_info_property('managed_file', '#pre_render'), array('webform_file_allow_access')),
    '#upload_location' => $component['extra']['scheme'] . '://webform/' . ($filter
                                                                              ? drupal_strtolower(webform_replace_tokens($component['extra']['directory'], $node))
                                                                              : $component['extra']['directory']),
    '#progress_indicator' => $component['extra']['progress_indicator'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description'),
  );

  if ($filter) {
    $element['#description'] = theme('file_upload_help', array('description' => $element['#description'], 'upload_validators' => $element['#upload_validators']));
  }

  return $element;
}

/**
 * Returns HTML for a webform managed file element.
 *
 * See #2495821 and #2497909. The core theme_file_managed_file creates a
 * wrapper around the element with the element's id, thereby creating 2 elements
 * with the same id.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the file.
 *
 * @return string
 */
function theme_webform_managed_file($variables) {
  $element = $variables['element'];

  $attributes = array();

  // For webform use, do not add the id to the wrapper.
  // @code
  // if (isset($element['#id'])) {
  //   $attributes['id'] = $element['#id'];
  // }
  // @endcode
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-managed-file';

  // This wrapper is required to apply JS behaviors and CSS styling.
  $output = '';
  $output .= '<div' . drupal_attributes($attributes) . '>';
  $output .= drupal_render_children($element);
  $output .= '</div>';
  return $output;
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_file($component, $value) {
  $fid = is_array($value)
            ? (!empty($value['fid']) ? $value['fid'] : '')
            : (!empty($value) ? $value : '');
  // Extend access to this file, even if the submission has not been saved yet. This may happen when
  // previewing a private file which was selected but not explicitly uploaded, and then previewed.
  if ($fid) {
    $_SESSION['webform_files'][$fid] = $fid;
  }
  return $fid;
}

/**
 * Pre-render callback to allow access to uploaded files.
 *
 * Files that have not yet been saved into a submission must be accessible to
 * the user who uploaded it, but no one else. After the submission is saved,
 * access is granted through the file_usage table. Before then, we use a
 * $_SESSION value to record a user's upload.
 *
 * @see webform_file_download()
 */
function webform_file_allow_access($element) {
  if (!empty($element['#value']['fid'])) {
    $fid = $element['#value']['fid'];
    $_SESSION['webform_files'][$fid] = $fid;
  }

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_file($component, $value, $format = 'html', $submission = array()) {
  $fid = isset($value[0]) ? $value[0] : NULL;
  return array(
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#value' => $fid ? webform_get_file($fid) : NULL,
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_file',
    '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
    '#format' => $format,
    '#translatable' => array('title'),
  );
}

/**
 * Format the output of text data for this component.
 */
function theme_webform_display_file($variables) {
  $element = $variables['element'];

  $file = $element['#value'];
  $url = !empty($file) ? webform_file_url($file->uri) : t('no upload');
  return !empty($file) ? ($element['#format'] == 'text' ? $url : l($file->filename, $url)) : ' ';
}

/**
 * Implements _webform_delete_component().
 */
function _webform_delete_file($component, $value) {
  // Delete an individual submission file.
  if (!empty($value[0]) && ($file = webform_get_file($value[0]))) {
    file_usage_delete($file, 'webform');
    file_delete($file);
  }
}

/**
 * Implements _webform_attachments_component().
 */
function _webform_attachments_file($component, $value) {
  $file = (array) webform_get_file($value[0]);
  $files = array($file);
  return $files;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_file($component, $sids = array(), $single = FALSE, $join = NULL) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('no', 'data'))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid']);

  if (count($sids)) {
    $query->condition('wsd.sid', $sids, 'IN');
  }

  if ($join) {
    $query->innerJoin($join, 'ws2_', 'wsd.sid = ws2_.sid');
  }

  $nonblanks = 0;
  $sizetotal = 0;
  $submissions = 0;

  $result = $query->execute();
  foreach ($result as $data) {
    $file = webform_get_file($data['data']);
    if (isset($file->filesize)) {
      $nonblanks++;
      $sizetotal += $file->filesize;
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User uploaded file'), $nonblanks);
  $other[0] = array(t('Average uploaded file size'), ($sizetotal != 0 ? (int) (($sizetotal / $nonblanks) / 1024) . ' KB' : '0'));
  return array(
    'table_rows' => $rows,
    'other_data' => $other,
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_file($component, $value) {
  $output = '';
  $file = webform_get_file($value[0]);
  if (!empty($file->fid)) {
    $output = '<a href="' . webform_file_url($file->uri) . '">' . check_plain(webform_file_name($file->uri)) . '</a>';
    $output .= ' (' . (int) ($file->filesize / 1024) . ' KB)';
  }
  return $output;
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_file($component, $export_options) {
  $header = array();
  // Two columns in header.
  $header[0] = array('', '');
  $header[1] = array($export_options['header_keys'] ? $component['form_key'] : $component['name'], '');
  $header[2] = array(t('Name'), t('Filesize (KB)'));
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_file($component, $export_options, $value) {
  $file = webform_get_file($value[0]);
  return empty($file->filename) ? array('', '') : array(webform_file_url($file->uri), (int) ($file->filesize / 1024));
}

/**
 * Helper function to create proper file names for uploaded file.
 */
function webform_file_name($filepath) {
  if (!empty($filepath)) {
    $info = pathinfo($filepath);
    $file_name = $info['basename'];
  }
  return isset($file_name) ? $file_name : '';
}

/**
 * Helper function to create proper URLs for uploaded file.
 */
function webform_file_url($uri) {
  if (!empty($uri)) {
    $file_url = file_create_url($uri);
  }
  return isset($file_url) ? $file_url : '';
}

/**
 * Helper function to load a file from the database.
 */
function webform_get_file($fid) {
  // Simple check to prevent loading of NULL values, which throws an entity
  // system error.
  return $fid ? file_load($fid) : FALSE;
}

/**
 * Given a submission with file_usage set, add or remove file usage entries.
 */
function webform_file_usage_adjust($submission) {
  if (isset($submission->file_usage)) {
    $files = file_load_multiple($submission->file_usage['added_fids']);
    foreach ($files as $file) {
      $file->status = 1;
      file_save($file);
      file_usage_add($file, 'webform', 'submission', $submission->sid);
    }

    $files = file_load_multiple($submission->file_usage['deleted_fids']);
    foreach ($files as $file) {
      file_usage_delete($file, 'webform', 'submission', $submission->sid);
      file_delete($file);
    }
  }
}

/**
 * Rename any files which are eligible for renaming, if this submission is being
 * submitted for the first time.
 */
function webform_file_rename($node, $submission) {
  if (isset($submission->file_usage)) {
    foreach ($submission->file_usage['renameable'] as $cid => $fids) {
      foreach ($fids as $fid) {
        webform_file_process_rename($node, $submission, $node->webform['components'][$cid], $fid);
      }
    }
  }
}

/**
 * Renames the uploaded file name using tokens.
 *
 * @param $node
 *   The webform node object.
 * @param $submission
 *   The webform submission object.
 * @param $component
 *   Component settings array for which fid is going to be processed.
 * @param $fid
 *   A file id to be processed.
 */
function webform_file_process_rename($node, $submission, $component, $fid) {
  $file = webform_get_file($fid);

  if ($file) {
    // Get the destination uri.
    $destination_dir = $component['extra']['scheme'] . '://webform/' . drupal_strtolower(webform_replace_tokens($component['extra']['directory'], $node));
    $destination_dir = file_stream_wrapper_uri_normalize($destination_dir);

    // Get the file extension.
    $info = pathinfo($file->uri);
    $extension = $info['extension'];

    // Prepare new file name without extension.
    $new_file_name = webform_replace_tokens($component['extra']['rename'], $node, $submission, NULL, TRUE);
    $new_file_name = trim($new_file_name);
    $new_file_name = _webform_transliterate($new_file_name);
    $new_file_name = str_replace('/', '_', $new_file_name);
    $new_file_name = preg_replace('/[^a-zA-Z0-9_\- ]/', '', $new_file_name);
    if (strlen($new_file_name)) {
      // Prepare the new uri with new filename.
      $destination = "$destination_dir/$new_file_name.$extension";

      // Compare the uri and Rename the file name.
      if ($file->uri != $destination) {
        file_move($file, $destination, FILE_EXISTS_RENAME);
      }
    }
  }
}
