<?php
/**
 * @file
 * This file contains the differtent Rate UI forms.
 */

/**
 * Implements hook_form().
 *
 * Form callback for add or edit rate widget form.
 */
function rate_ui_widget_form($form, &$form_state, $widget_type = NULL) {
  $step = isset($form_state['step']) ? $form_state['step'] : 1;
  $form['#step'] = $step;

  $form['#rate_wid'] = 0;
  $form['#rate_widget_name'] = $widget_type;
  if ($widget_type) {
    if (empty($form_state['storage'])) {
      $form_state['storage'] = array();
      $form['#rate_wid'] = rate_ui_widget_load_widget($widget_type, $form_state['storage']);
    }
    elseif (!empty($form_state['storage']['wid'])) {
      // Also set the wid when storage is not empty.
      $form['#rate_wid'] = $form_state['storage']['wid'];
    }
  }

  drupal_add_js(drupal_get_path('module', 'rate_ui') . '/admin.js', 'file');
  drupal_add_css(drupal_get_path('module', 'rate_ui') . '/admin.css');

  switch ($step) {
    case 1:
      $title = t('General settings');
      $form = rate_ui_widget_form_step_general($form, $form_state, $widget_type);
      break;
    case 2:
      $title = t('Voting buttons');
      $form = rate_ui_widget_form_step_buttons($form, $form_state, $widget_type);
      break;
    case 3:
      $title = t('Descriptions');
      $form = rate_ui_widget_form_step_descriptions($form, $form_state, $widget_type);
      break;
    case 4:
      $title = t('Theming options');
      $form = rate_ui_widget_form_step_theming($form, $form_state, $widget_type);
      break;
    case 5:
      $title = t('Images');
      $form = rate_ui_widget_form_step_images($form, $form_state, $widget_type);
      break;
    case 6:
      $title = t('Layout');
      $form = rate_ui_widget_form_step_layout($form, $form_state, $widget_type);
      break;
  }

  drupal_set_title(t('Step @step of @total: @title', array(
    '@step' => $step,
    '@total' => 6,
    '@title' => $title
  )));
  return $form;
}

/**
 * Generate form fields for the first step in the rate widget form.
 */
function rate_ui_widget_form_step_general($form, &$form_state, $widget_type = NULL) {

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($form_state['storage']['name']) ? $form_state['storage']['name'] : NULL,
    '#required' => TRUE,
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($form_state['storage']['type']) ? $form_state['storage']['type'] : NULL,
    '#maxlength' => 32,
    '#disabled' => !empty($widget_type),
    '#machine_name' => array(
      'exists' => '_rate_get_widgets',
    ),
    '#description' => t('A unique machine-readable name for this widget. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $metadata = votingapi_metadata();
  $options = array();
  foreach ($metadata['value_types'] as $value_type => $info) {
    $options[$value_type] = $info['name'];
  }
  $form['mode'] = array(
    '#type' => 'radios',
    '#title' => t('VotingAPI mode'),
    '#options' => $options,
    '#default_value' => isset($form_state['storage']['mode']) ? $form_state['storage']['mode'] : NULL,
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Generate form fields for the second step in the rate widget form.
 */
function rate_ui_widget_form_step_buttons($form, &$form_state, $widget_type = NULL) {

  $form['buttons'] = array(
    '#prefix' => '<div id="rate-buttons">',
    '#suffix' => '</div>',
  );
  $form['buttons']['list'] = array(
    '#theme' => 'rate_ui_buttons',
  );

  // Set or update the number of buttons.
  !empty($form_state['storage']['buttoncount']) or $form_state['storage']['buttoncount'] = 2;

  switch ($form_state['triggering_element']['#value']) {
    case t('Add'):
      ++$form_state['storage']['buttoncount'];
      break;
    case t('Remove'):
      --$form_state['storage']['buttoncount'];
      break;
  }

  // Create form elements for buttons.
  for ($i = 1; $i <= $form_state['storage']['buttoncount']; ++$i) {
    $form['buttons']['list'][$i] = array();
    $form['buttons']['list'][$i]["button{$i}_label"] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#size' => 16,
      '#default_value' => isset($form_state['storage']["button{$i}_label"]) ? $form_state['storage']["button{$i}_label"] : '',
    );
    $form['buttons']['list'][$i]["button{$i}_value"] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#size' => 6,
      '#default_value' => isset($form_state['storage']["button{$i}_value"]) ? $form_state['storage']["button{$i}_value"] : '',
    );
    $form['buttons']['list'][$i]["button{$i}_description"] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#default_value' => isset($form_state['storage']["button{$i}_description"]) ? $form_state['storage']["button{$i}_description"] : '',
    );
  }

  $form['buttons']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#submit' => array('rate_ui_widget_form_submit'),
    '#ajax' => array(
      'callback' => 'rate_ui_widget_form_ajax',
      'wrapper' => 'rate-buttons',
      'effect' => 'fade',
    ),
  );

  if ($form_state['storage']['buttoncount'] > 1) {
    $form['buttons']['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('rate_ui_widget_form_submit'),
      '#ajax' => array(
        'callback' => 'rate_ui_widget_form_ajax',
        'wrapper' => 'rate-buttons',
        'effect' => 'fade',
      ),
    );
  }

  // Add formfields for the revoke button.
  $form['buttons']['revoke'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revoke button'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['buttons']['revoke']['fields'] = array(
    '#theme' => 'rate_ui_buttons',
  );

  $form['buttons']['revoke']['fields'][0] = array();
  $form['buttons']['revoke']['fields'][0]["button0_label"] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#size' => 16,
    '#default_value' => isset($form_state['storage']["button0_label"]) ? $form_state['storage']["button0_label"] : '',
  );

  $form['buttons']['revoke']['fields'][0]["button0_value"] = array(
    '#markup' => t('N/A'),
  );

  $form['buttons']['revoke']['fields'][0]["button0_description"] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => isset($form_state['storage']["button0_description"]) ? $form_state['storage']["button0_description"] : '',
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
  );

  $form['actions']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Generate form fields for the thirth step in the rate widget form.
 */
function rate_ui_widget_form_step_descriptions($form, &$form_state, $widget_type = NULL) {

  $form['descriptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Descriptions'),
    '#collapsible' => FALSE,
  );

  $form['descriptions']['desc_norating'] = array(
    '#type' => 'textfield',
    '#title' => t('No rating'),
    '#default_value' => isset($form_state['storage']['desc_norating']) ? $form_state['storage']['desc_norating'] : 'No one voted',
  );

  $form['descriptions']['desc_notvoted'] = array(
    '#type' => 'textfield',
    '#title' => t('Not voted'),
    '#default_value' => isset($form_state['storage']['desc_notvoted']) ? $form_state['storage']['desc_notvoted'] : 'Average rating: @rating',
  );

  $form['descriptions']['desc_voted'] = array(
    '#type' => 'textfield',
    '#title' => t('Already voted'),
    '#default_value' => isset($form_state['storage']['desc_voted']) ? $form_state['storage']['desc_voted'] : 'You voted @vote',
  );

  $form['descriptions']['desc_justvoted'] = array(
    '#type' => 'textfield',
    '#title' => t('Just voted'),
    '#default_value' => isset($form_state['storage']['desc_justvoted']) ? $form_state['storage']['desc_justvoted'] : 'Thank you for your vote!',
  );

  $form['descriptions']['desc_mouseover'] = array(
    '#type' => 'textfield',
    '#title' => t('On mouse over'),
    '#default_value' => isset($form_state['storage']['desc_mouseover']) ? $form_state['storage']['desc_mouseover'] : '',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
  );
  $form['actions']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Generate form fields for the fourth step in the rate widget form.
 */
function rate_ui_widget_form_step_theming($form, &$form_state, $widget_type = NULL) {

  $options = array(
    FALSE => t('Text'),
    TRUE => t('Images'),
  );
  $form['images'] = array(
    '#type' => 'radios',
    '#title' => t('Display type of options'),
    '#default_value' => isset($form_state['storage']['images']) ? $form_state['storage']['images'] : TRUE,
    '#options' => $options,
  );

  $options = array(
    TRUE => t('Use sprites'),
    FALSE => t('Use image tags'),
  );

  $form['sprites'] = array(
    '#type' => 'radios',
    '#title' => t('Sprites'),
    '#default_value' => isset($form_state['storage']['sprites']) ? $form_state['storage']['sprites'] : TRUE,
    '#options' => $options,
  );

  $options = array(
    TRUE => t('Different for each button'),
    FALSE => t('Same for all buttons'),
  );

  if (empty($form_state['storage']['button0_label'])) {
    $description = '';
  }
  else {
    $description = 'Does not include the revoke button.';
  }

  $form['separate_images'] = array(
    '#type' => 'radios',
    '#title' => t('Separate images?'),
    '#default_value' => isset($form_state['storage']['separate_images']) ? $form_state['storage']['separate_images'] : TRUE,
    '#options' => $options,
    '#description' => t($description),
  );

  $options = array(
    'upload' => t('Upload images'),
    'filepath' => t('Provide filepath'),
    'spritegenerator' => t('Use CSS from sprite generator'),
  );

  $form['imagesource'] = array(
    '#type' => 'radios',
    '#title' => t('Image source'),
    '#default_value' => isset($form_state['storage']['imagesource']) ? $form_state['storage']['imagesource'] : 'upload',
    '#options' => $options,
  );

  $options = array(
    0 => t('Don\'t highlight button'),
    1 => t('Highlight button'),
    1 + 2 => t('Highlight button and buttons before'),
    1 + 4 => t('Highlight button and buttons after'),
    1 + 2 + 4 => t('Highlight all buttons'),
  );

  $form['highlight_voted'] = array(
    '#type' => 'radios',
    '#title' => t('Highlight voted button?'),
    '#default_value' => isset($form_state['storage']['highlight_voted']) ? $form_state['storage']['highlight_voted'] : 1,
    '#options' => $options,
  );

  $options = array(
    0 => t('Don\'t highlight button'),
    1 => t('Highlight button'),
    1 + 2 => t('Highlight button and buttons before'),
    1 + 4 => t('Highlight button and buttons after'),
    1 + 2 + 4 => t('Highlight all buttons'),
  );

  $form['highlight_mouseover'] = array(
    '#type' => 'radios',
    '#title' => t('Highlight on mouseover?'),
    '#default_value' => isset($form_state['storage']['highlight_mouseover']) ? $form_state['storage']['highlight_mouseover'] : 0,
    '#options' => $options,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
  );

  $form['actions']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Generate form fields for the fifth step in the rate widget form.
 */
function rate_ui_widget_form_step_images($form, &$form_state, $widget_type = NULL) {
  $sprites = $form_state['storage']['sprites'];
  $separate = $form_state['storage']['separate_images'];
  $imagesource = $form_state['storage']['imagesource'];

  if ($imagesource == 'spritegenerator') {
    // Start with the revoke button?
    $start = empty($form_state['storage']['button0_label']) ? 1 : 0;
    $end = $form_state['storage']['buttoncount'];
    $form['sprites'] = rate_ui_sprite_classes_table($start, $end);
  }
  else {
    if ($sprites) {
      if ($imagesource == 'upload') {
        $form['sprite_file'] = array(
          '#type' => 'managed_file',
          '#title' => t('Sprite image'),
          '#upload_location' => 'public://rate',
          '#upload_validators' => array(
            'file_validate_extensions' => array('jpg png gif'),
          ),
          '#progress_indicator' => 'bar',
          '#pre_render' => array('file_managed_file_pre_render'),
          '#default_value' => isset($form_state['storage']['sprite_file']) ? $form_state['storage']['sprite_file'] : 0,
        );
      }
      elseif ($imagesource == 'filepath') {
        $form['sprite_filepath'] = array(
          '#type' => 'textfield',
          '#title' => t('Sprite image'),
          '#default_value' => isset($form_state['storage']['sprite_filepath']) ? $form_state['storage']['sprite_filepath'] : '',
          '#description' => t('Provide a filepath to the image, relative to the Drupal basedir. Do not use a leading slash.')
        );
      }
    }

    $form['buttons'] = array();

    // Start with the revoke button?
    $start = empty($form_state['storage']['button0_label']) ? 1 : 0;

    // Separate images for each button?
    $end = $separate ? $form_state['storage']['buttoncount'] : 1;

    for ($i = $start; $i <= $end; ++$i) {
      if ($i == 0) {
        $title = t('Revoke button');
      }
      elseif (!$separate) {
        $title = t('Vote buttons');
      }
      else {
        if (empty($form_state['storage']["button{$i}_label"])) {
          // Label field is not filled in, do not use this button.
          continue;
        }
        $title = t('Button %label', array('%label' => $form_state['storage']["button{$i}_label"]));
      }
      $form['buttons'][$i] = array(
        '#type' => 'fieldset',
        '#title' => $title,
        '#collapsible' => TRUE,
      );

      if ($sprites) {
        $form['buttons'][$i]['#theme'] = 'rate_ui_sprites';
        $form['buttons'][$i]['#imagesource'] = $imagesource;
      }

      $kinds = array(
        'default',
        'highlighted',
        'default_voted',
        'highlighted_voted',
        'disabled',
        'disabled_voted',
      );
      foreach ($kinds as $kind) {
        $kind_name = str_replace('_', ' ', $kind);
        if ($sprites) {
          $form['buttons'][$i]["button{$i}_{$kind}"] = array(
            '#type' => 'fieldset',
            '#title' => t($kind_name),
          );
          $form['buttons'][$i]["button{$i}_{$kind}"]["button{$i}_{$kind}_x"] = array(
            '#type' => 'textfield',
            '#title' => t('X offset'),
            '#size' => 6,
            '#default_value' => isset($form_state['storage']["button{$i}_{$kind}_x"]) ? $form_state['storage']["button{$i}_{$kind}_x"] : '',
          );

          $form['buttons'][$i]["button{$i}_{$kind}"]["button{$i}_{$kind}_y"] = array(
            '#type' => 'textfield',
            '#title' => t('Y offset'),
            '#size' => 6,
            '#default_value' => isset($form_state['storage']["button{$i}_{$kind}_y"]) ? $form_state['storage']["button{$i}_{$kind}_y"] : '',
          );

          $form['buttons'][$i]["button{$i}_{$kind}"]["button{$i}_{$kind}_width"] = array(
            '#type' => 'textfield',
            '#title' => t('Width'),
            '#size' => 6,
            '#default_value' => isset($form_state['storage']["button{$i}_{$kind}_width"]) ? $form_state['storage']["button{$i}_{$kind}_width"] : '',
          );

          $form['buttons'][$i]["button{$i}_{$kind}"]["button{$i}_{$kind}_height"] = array(
            '#type' => 'textfield',
            '#title' => t('Height'),
            '#size' => 6,
            '#default_value' => isset($form_state['storage']["button{$i}_{$kind}_height"]) ? $form_state['storage']["button{$i}_{$kind}_height"] : '',
          );
        }
        elseif ($imagesource == 'upload') {
          $form['buttons'][$i]["button{$i}_{$kind}_file"] = array(
            '#type' => 'managed_file',
            '#title' => t($kind_name),
            '#upload_location' => 'public://rate',
            '#upload_validators' => array(
              'file_validate_extensions' => array('jpg png gif'),
            ),
            '#progress_indicator' => 'bar',
            '#pre_render' => array('file_managed_file_pre_render'),
            '#default_value' => isset($form_state['storage']["button{$i}_{$kind}_file"]) ? $form_state['storage']["button{$i}_{$kind}_file"] : 0,
          );
        }
        elseif ($imagesource == 'filepath') {
          $form['buttons'][$i]["button{$i}_{$kind}_filepath"] = array(
            '#type' => 'textfield',
            '#title' => t($kind_name),
            '#default_value' => isset($form_state['storage']["button{$i}_{$kind}_filepath"]) ? $form_state['storage']["button{$i}_{$kind}_filepath"] : '',
            '#description' => t('Provide a filepath to the image, relative to the Drupal basedir. Do not use a leading slash.')
          );
        }
      }
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
  );

  $form['actions']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Generate form fields for the sixth step in the rate widget form.
 */
function rate_ui_widget_form_step_layout($form, &$form_state, $widget_type = NULL) {

  $modes = array(
    t('!abbr = full', array('!abbr' => t('f', array(), array('context' => 'rate viewmodes')))),
    t('!abbr = compact', array('!abbr' => t('c', array(), array('context' => 'rate viewmodes')))),
    t('!abbr = full disabled', array('!abbr' => t('fd', array(), array('context' => 'rate viewmodes')))),
    t('!abbr = compact disabled', array('!abbr' => t('cd', array(), array('context' => 'rate viewmodes')))),
    t('!abbr = closed', array('!abbr' => t('cl', array(), array('context' => 'rate viewmodes')))),
  );
  $modes = theme('item_list', array('items' => $modes));
  $form['info'] = array(
    '#markup' => t('<p>Check which elements should be included in the widget. Modes are: </p>!modes', array(
      '!modes' => $modes,
    )),
  );

  $form['elements'] = array(
    '#theme' => 'rate_ui_layout',
  );

  $weight_options = array();
  for ($i = 1; $i <= 255; ++$i) {
    $weight_options[$i] = $i;
  }
  $weight = 0;
  $elements = rate_ui_widget_elements($form_state['storage']);
  foreach ($elements as $type => $name) {
    $default = 0;
    if (preg_match('/^(button|rating)/', $type)) {
      $default = 31; // all formatters
    }
    if (preg_match('/^(description)/', $type)) {
      $default |= 1; // full
      $default |= 4; // full disabled
    }
    $form['elements'][$type] = array();
    $form['elements'][$type]["element_{$type}"] = array(
      '#markup' => $name,
    );

    $form['elements'][$type]['mode'] = array();
    $form['elements'][$type]['mode']["element_{$type}_1"] = array(
      '#type' => 'checkbox',
      '#title' => t('f', array(), array('context' => 'rate viewmodes')),
      '#default_value' => isset($form_state['storage']["element_{$type}_1"]) ? $form_state['storage']["element_{$type}_1"] : $default & 1,
    );
    $form['elements'][$type]['mode']["element_{$type}_2"] = array(
      '#type' => 'checkbox',
      '#title' => t('c', array(), array('context' => 'rate viewmodes')),
      '#default_value' => isset($form_state['storage']["element_{$type}_2"]) ? $form_state['storage']["element_{$type}_2"] : $default & 2,
    );
    $form['elements'][$type]['mode']["element_{$type}_4"] = array(
      '#type' => 'checkbox',
      '#title' => t('fd', array(), array('context' => 'rate viewmodes')),
      '#default_value' => isset($form_state['storage']["element_{$type}_4"]) ? $form_state['storage']["element_{$type}_4"] : $default & 4,
    );
    $form['elements'][$type]['mode']["element_{$type}_8"] = array(
      '#type' => 'checkbox',
      '#title' => t('cd', array(), array('context' => 'rate viewmodes')),
      '#default_value' => isset($form_state['storage']["element_{$type}_8"]) ? $form_state['storage']["element_{$type}_8"] : $default & 8,
    );
    $form['elements'][$type]['mode']["element_{$type}_16"] = array(
      '#type' => 'checkbox',
      '#title' => t('cl', array(), array('context' => 'rate viewmodes')),
      '#default_value' => isset($form_state['storage']["element_{$type}_16"]) ? $form_state['storage']["element_{$type}_16"] : $default & 16,
    );

    $form['elements'][$type]["element_{$type}_prefix"] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix HTML'),
      '#default_value' => isset($form_state['storage']["element_{$type}_prefix"]) ? $form_state['storage']["element_{$type}_prefix"] : '',
    );

    $form['elements'][$type]["element_{$type}_suffix"] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix HTML'),
      '#default_value' => isset($form_state['storage']["element_{$type}_suffix"]) ? $form_state['storage']["element_{$type}_suffix"] : '',
    );

    $form['elements'][$type]["element_{$type}_weight"] = array(
      '#type' => 'select',
      '#title' => t('Weight'),
      '#options' => $weight_options,
      '#default_value' => isset($form_state['storage']["element_{$type}_weight"]) ? $form_state['storage']["element_{$type}_weight"] : ++$weight,
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 *
 * Submit handler for the rate widget form.
 */
function rate_ui_widget_form_submit($form, &$form_state) {
  $form_state['storage'] = isset($form_state['storage']) ? $form_state['storage'] : array();
  $form_state['storage'] = array_merge($form_state['storage'], $form_state['values']);

  // This var is set to TRUE when we are ready to save the widget.
  $save = FALSE;

  // We can skip step 5 if we don't use images.
  // The next step is step 6 when we are on step 4 and clicking "next" or
  // when we are on step 6 and clicking "previous".
  if (($form['#step'] == 4 && $form_state['triggering_element']['#id'] == 'edit-next')
   || ($form['#step'] == 6 && $form_state['triggering_element']['#id'] == 'edit-previous')) {
    if (!$form_state['storage']['images']) {
      // We use text links.
      $form['#step'] += $form_state['triggering_element']['#id'] == 'edit-previous' ? -1 : 1;
    }
  }

  switch ($form_state['triggering_element']['#id']) {
    case 'edit-next':
      $form_state['step'] = $form['#step'] + 1;
      break;
    case 'edit-previous':
      $form_state['step'] = $form['#step'] - 1;
      break;
    case 'edit-submit':
      $save = TRUE;
      break;
  }

  if ($save) {
    if ($wid = rate_ui_widget_save_widget($form['#rate_wid'], $form_state['storage'])) {
      rate_ui_widget_save_buttons($wid, $form_state['storage']);
      rate_ui_widget_save_layout($wid, $form_state['storage']);
      drupal_set_message(t('The widget has been saved succesfully.'));
    }
    else {
      drupal_set_message(t('The widget could not be saved.'), 'error');
    }
    $form_state['redirect'] = 'admin/structure/rate';
  }
  else {
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Save widget settings from storage array into database.
 *
 * @param int $wid
 * @param array $storage
 */
function rate_ui_widget_save_widget($wid, $storage) {
  $widget = new stdClass();
  $widget->type = $storage['type'];
  $widget->name = $storage['name'];
  $widget->mode = $storage['mode'];
  $widget->sprites = (int) !empty($storage['sprites']);
  $widget->highlight_voted = $storage['highlight_voted'];
  $widget->highlight_mouseover = $storage['highlight_mouseover'];
  $widget->desc_norating = $storage['desc_norating'];
  $widget->desc_notvoted = $storage['desc_notvoted'];
  $widget->desc_voted = $storage['desc_voted'];
  $widget->desc_justvoted = $storage['desc_justvoted'];
  $widget->desc_mouseover = $storage['desc_mouseover'];
  if ($storage['images'] && $storage['imagesource'] != 'spritegenerator') {
    $widget->css_file = rate_ui_write_css($storage);
  }
  $widget->js_file = NULL;

  if ($wid) {
    $widget->wid = $wid;
    drupal_write_record('rate_widget', $widget, array('wid'));
  }
  else {
    drupal_write_record('rate_widget', $widget);
  }
  return $widget->wid;
}

/**
 * Save widget buttons from storage array into database.
 *
 * @param int $wid
 * @param array $storage
 */
function rate_ui_widget_save_buttons($wid, $storage) {
  $images = $storage['images'];
  $sprites = $storage['sprites'];
  $separate = $storage['separate_images'];
  $imagesource = $storage['imagesource'];

  // Delete old configuration.
  db_delete('rate_widget_button')
    ->condition('wid', $wid)
    ->execute();

  // Start with the revoke button?
  $start = empty($storage['button0_label']) ? 1 : 0;
  $end = $storage['buttoncount'];

  for ($i = $start; $i <= $end; ++$i) {
    if (empty($storage["button{$i}_label"])) {
      continue;
    }
    $button = new stdClass();
    $button->wid = $wid;
    $button->num = $i;
    $button->label = $storage["button{$i}_label"];
    $button->value = $storage["button{$i}_value"];
    $button->description = $storage["button{$i}_description"];

    foreach (array('default', 'highlighted', 'default_voted', 'highlighted_voted', 'disabled', 'disabled_voted') as $kind) {

      // If we do not have separate buttons, the fields for all buttons have
      // "button1" in the name, except for the revoke button.
      $num = $separate && $i > 1 ? $i : 1;

      if ($imagesource == 'spritegenerator') {
        $button->{"img_$kind"} = '';
      }
      elseif ($images && $sprites) {
        $x = $storage["button{$num}_{$kind}_x"];
        $y = $storage["button{$num}_{$kind}_y"];
        $width = $storage["button{$num}_{$kind}_width"];
        $height = $storage["button{$num}_{$kind}_height"];
        if ($imagesource == 'upload') {
          $source = $storage['sprite_file'];
          db_update('file_managed')->fields(array(
            'status' => FILE_STATUS_PERMANENT,
          ))->condition('fid', $source)->execute();
          $file = new stdClass();
          $file->fid = $source;
          file_usage_add($file, 'rate', 'rate_widget', $wid);
        }
        else {
          $source = $storage['sprite_filepath'];
        }
        $button->{"img_$kind"} = "$source@$x,$y:$width,$height";
      }
      elseif ($images) {
        if ($imagesource == 'upload') {
          $source = $storage["button{$num}_{$kind}_file"];
          db_update('file_managed')->fields(array(
            'status' => FILE_STATUS_PERMANENT,
          ))->condition('fid', $source)->execute();
          $file = new stdClass();
          $file->fid = $source;
          file_usage_add($file, 'rate', 'rate_widget', $wid);
        }
        else {
          $source = $storage["button{$num}_{$kind}_filepath"];
        }
        $button->{"img_$kind"} = $source;
      }
    }
    drupal_write_record('rate_widget_button', $button);
  }
}

/**
 * Save widget layout from storage array into database.
 *
 * @param int $wid
 * @param array $storage
 */
function rate_ui_widget_save_layout($wid, $storage) {
  if ($wid) {
    // Delete old configuration.
    db_delete('rate_widget_element')
      ->condition('wid', $wid)
      ->execute();
  }

  $elements = rate_ui_widget_elements($storage);
  foreach ($elements as $type => $name) {
    $mode = 0;
    foreach (array(1, 2, 4, 8, 16) as $i) {
      if (!empty($storage["element_{$type}_{$i}"])) {
        $mode |= $i;
      }
    }
    if ($mode) {
      $element = new stdClass();
      $element->wid = $wid;
      $element->type = $type;
      $element->prefix = $storage["element_{$type}_prefix"];
      $element->suffix = $storage["element_{$type}_suffix"];
      $element->weight = $storage["element_{$type}_weight"];
      $element->mode = $mode;
      drupal_write_record('rate_widget_element', $element);
    }
  }
}

/**
 * Load widget settings from database into storage array.
 *
 * @param int $wid
 * @param array $storage
 * @return int
 */
function rate_ui_widget_load_widget($type, &$storage) {

  // Select widget by type.
  $widget = db_select('rate_widget', 'w')
    ->fields('w', array('wid', 'type', 'name', 'mode', 'sprites',
      'highlight_voted',  'highlight_mouseover', 'desc_norating',
      'desc_notvoted', 'desc_voted', 'desc_justvoted', 'desc_mouseover',
      'css_file', 'js_file'))
    ->condition('w.type', $type)
    ->execute()
    ->fetchObject();

    if (!$widget) {
    return $storage;
  }

  $storage = (array) $widget;

  rate_ui_widget_load_buttons($storage['wid'], $storage);
  rate_ui_widget_load_layout($storage['wid'], $storage);

  return $storage['wid'];
}

/**
 * Load widget buttons from database into storage array.
 *
 * @param int $wid
 * @param array $storage
 */
function rate_ui_widget_load_buttons($wid, &$storage) {
  $storage['separate_images'] = TRUE;
  $storage['buttoncount'] = 1;

  // Select rate widget by widget id.
  $buttons = db_select('rate_widget_button', 'b')
    ->fields('b', array('num', 'label', 'value', 'description', 'width',
      'height', 'img_default', 'img_highlighted', 'img_default_voted',
      'img_highlighted_voted', 'img_disabled', 'img_disabled_voted'))
    ->condition('b.wid', $wid)
    ->execute()
    ->fetchAll();

  foreach ($buttons as $button) {
    $num = $button->num;
    $storage['buttoncount'] = max($storage['buttoncount'], $num);

    $storage["button{$num}_label"] = $button->label;
    $storage["button{$num}_value"] = $button->value;
    $storage["button{$num}_description"] = $button->description;

    $kinds = array(
      'default',
      'highlighted',
      'default_voted',
      'highlighted_voted',
      'disabled',
      'disabled_voted',
    );
    foreach ($kinds as $kind) {
      if (empty($button->{"img_$kind"})) {
        $storage['images'] = FALSE;
      }
      elseif (preg_match('/^(.*)\\@([0-9]+),([0-9]+)\\:([0-9]+),([0-9]+)$/', $button->{"img_$kind"},  $match)) {
        $storage['sprites'] = TRUE;
        if (is_numeric($match[1])) {
          $storage['upload'] = TRUE;
          $storage['sprite_file'] = $match[1];
        }
        else {
          $storage['upload'] = FALSE;
          $storage['sprite_filepath'] = $match[1];
        }
        $storage["button{$num}_{$kind}_x"] = $match[2];
        $storage["button{$num}_{$kind}_y"] = $match[3];
        $storage["button{$num}_{$kind}_width"] = $match[4];
        $storage["button{$num}_{$kind}_height"] = $match[5];
      }
      elseif (is_numeric($button->{"img_$kind"})) {
        $storage["button{$num}_{$kind}_file"] = $button->{"img_$kind"};
      }
      else {
        $storage["button{$num}_{$kind}_filepath"] = $button->{"img_$kind"};
      }
    }
  }
}

/**
 * Load widget layout from database into storage array.
 *
 * @param int $wid
 * @param array $storage
 */
function rate_ui_widget_load_layout($wid, &$storage) {
  $elements = rate_ui_widget_elements($storage);
  foreach ($elements as $type => $name) {
    $storage["element_{$type}"] = 0;
    foreach (array(1, 2, 4, 8, 16) as $i) {
      $storage["element_{$type}_{$i}"] = FALSE;
    }
    $storage["element_{$type}_prefix"] = '';
    $storage["element_{$type}_suffix"] = '';
    $storage["element_{$type}_weight"] = 128;
  }
  $elements = db_select('rate_widget_element', 'e')
    ->fields('e', array('type', 'prefix', 'suffix', 'weight', 'mode'))
    ->condition('e.wid', $wid)
    ->execute()
    ->fetchAll();
  foreach ($elements as $element) {
    $storage["element_{$element->type}"] = $element->mode;
    foreach (array(1, 2, 4, 8, 16) as $i) {
      $storage["element_{$element->type}_{$i}"] = (bool) ($element->mode & $i);
    }
    $storage["element_{$element->type}_prefix"] = $element->prefix;
    $storage["element_{$element->type}_suffix"] = $element->suffix;
    $storage["element_{$element->type}_weight"] = $element->weight;
  }
}


/**
 * Ajax callback for the add / remove buttons on the Rate widget form.
 */
function rate_ui_widget_form_ajax($form, $form_state) {
  return $form['buttons'];
}

/**
 * Generate a list of available widget elements based on current form values.
 *
 * @param array $storage
 *   Values from $form_state['storage'].
 */
function rate_ui_widget_elements($storage) {
  $elements = array();

  // Add option counts.
  for ($i = 1; $i <= $storage['buttoncount']; ++$i) {
    if (empty($storage["button{$i}_label"])) {
      // Button label is empty. Do not use this button.
      continue;
    }
    $elements["button$i"] = t('Button %label', array('%label' => $storage["button{$i}_label"]));
    if ($storage['mode'] == 'option') {
      $elements["button{$i}_count"] = t('Button %label vote count', array('%label' => $storage["button{$i}_label"]));
    }
  }

  // Check for thumbs up / down.
  if ($storage['mode'] == 'points' && $storage['buttoncount'] == 2) {
    if (min($storage['button1_value'], $storage['button2_value']) == -1
    && max($storage['button1_value'], $storage['button2_value']) == 1) {
      $elements['up_count'] = t('Up count');
      $elements['up_percent'] = t('Up percent');
      $elements['down_count'] = t('Down count');
      $elements['down_percent'] = t('Down percent');
    }
  }

  if (!empty($storage['button0_label'])) {
    $elements['button0'] = t('Revoke button');
  }

  if ($storage['mode'] == 'percent') {
    $elements['rating'] = t('Average rating');
  }
  if ($storage['mode'] == 'points') {
    $elements['rating'] = t('Points');
  }

  $elements['description'] = t('Description');
  $elements['count'] = t('Vote count');

  return $elements;
}

/**
 * Write CSS file for widget.
 *
 * @param array $storage
 * @return string Filepath to CSS file
 */
function rate_ui_write_css($storage) {
  $css = array();

  $type = $storage['type'];
  $images = $storage['images'];
  $sprites = $storage['sprites'];
  $separate = $storage['separate_images'];
  $imagesource = $storage['imagesource'];

  if ($images && $sprites) {
    // Start with the revoke button?
    $start = empty($storage['button0_label']) ? 1 : 0;

    for ($i = $start; $i <= $storage['buttoncount']; ++$i) {
      if (empty($storage["button{$i}_label"])) {
        // Button label is empty. Do not use this button.
        continue;
      }

      $kinds = array(
        'default' => '',
        'highlighted' => '.highlighted',
        'default_voted' => '.voted',
        'highlighted_voted' => '.voted.highlighted',
        'disabled' => '.disabled',
        'disabled_voted' => '.voted.disabled',
      );
      foreach ($kinds as $kind => $extra_classes) {

        // If we do not have separate buttons, the fields for all buttons have
        // "button1" in the name, except for the revoke button.
        $num = $separate && $i > 1 ? $i : 1;

        $x = (int) $storage["button{$num}_{$kind}_x"];
        $y = (int) $storage["button{$num}_{$kind}_y"];
        $width = (int) $storage["button{$num}_{$kind}_width"];
        $height = (int) $storage["button{$num}_{$kind}_height"];
        $source = $imagesource == 'upload' ? $storage['sprite_file'] : $storage['sprite_filepath'];

        if (is_numeric($source) && ($file = file_load($source))) {
          $source = file_create_url($file->uri);
        }
        else {
          $source = url($source);
        }

        $definition = ".rate-widget-$type .button$i$extra_classes";
        $css[$definition] = array();
        $css[$definition]['display'] = 'inline-block';
        $css[$definition]['width'] = "{$width}px";
        $css[$definition]['height'] = "{$height}px";
        $css[$definition]['text-indent'] = '-9999px';
        $css[$definition]['background'] = "url($source) -{$x}px -{$y}px";
      }
    }
  }

  if ($css) {
    $output = '';
    foreach ($css as $selector => $definitions) {
      $data = array();
      foreach ($definitions as $property => $value) {
        $data[] = "  $property: $value;";
      }
      $definitions = implode("\n", $data);
      $output .= "$selector {\n{$definitions}\n}\n";
    }
    $filename = 'public://rate/' . $storage['type'] . '.css';
    file_put_contents($filename, $output);
    return $filename;
  }
}

/**
 * Implements hook_form().
 *
 * Delete widget form.
 */
function rate_ui_delete_form($form, &$form_state, $widget_type) {

  // Get widget by widget type.
  $widget = db_select('rate_widget', 'w')
    ->fields('w', array('wid', 'name'))
    ->condition('w.type', $widget_type)
    ->execute()
    ->fetchObject();
  if (!$widget) {
    print drupal_not_found();
    module_invoke_all('exit') & exit;
  }
  $form['#widget_type'] = $widget_type;
  $form['#widget_wid'] = $widget->wid;
  $form['#widget_name'] = $widget->name;
  $form['info'] = array(
    '#markup' => '<p>' . t('Are you sure you want to delete the widget %name?', array(
      '%name' => $widget->name,
    )) . '</p>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['cancel'] = array(
    '#markup' => l(t('cancel'), 'admin/structure/rate'),
  );
  return $form;
}

/**
 * Implements hook_form_submit().
 *
 * Delete widget submit form.
 */
function rate_ui_delete_form_submit($form, &$form_state) {
  // @todo: Delete uploaded files
  db_delete('rate_widget')
    ->condition('wid', $form['#widget_wid'])
    ->execute();

  db_delete('rate_widget_button')
    ->condition('wid', $form['#widget_wid'])
    ->execute();

  db_delete('rate_widget_element')
    ->condition('wid', $form['#widget_wid'])
    ->execute();

  drupal_set_message(t('The rate widget %name has been deleted.', array(
    '%name' => $form['#widget_name'],
  )));

  $form_state['redirect'] = 'admin/structure/rate';
}

/**
 * Generate formfields to inform about classes for sprite generators.
 *
 * @param int $start First button number
 * @param int $end Last button number
 */
function rate_ui_sprite_classes_table($start, $end) {
  $form = array();

  $form['buttons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Button classes'),
    '#collapsible' => TRUE,
    '#description' => t('Theming can be separated per button. The list below outlines the button classes and possible combinations with or without highlighted classes.'),
  );
  $items = array();
  for ($i = $start; $i <= $end; ++$i) {
    $item = array('data' => ".button$i");
    if ($i > 0) {
      $item['children'] = array('(default)', '.highlighted');
    }
    $items[] = $item;
  }
  $form['buttons']['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  $form['widget'] = array(
    '#type' => 'fieldset',
    '#title' => t('Widget classes'),
    '#collapsible' => TRUE,
    '#description' => t('The list below outlines all possible combinations of widget classes (i.e. ".voted.user-rating.enabled").'),
  );
  $items = array(
    array('data' => '.voted', 'children' => array(
      array('data' => '.average-rating', 'children' => array(
        '.enabled', '.disabled',
      )),
      array('data' => '.user-rating', 'children' => array(
        '.enabled', '.disabled',
      )),
    )),
    array('data' => '.not-voted', 'children' => array(
      array('data' => '.average-rating', 'children' => array(
        '.enabled', '.disabled',
      )),
      array('data' => '.user-rating', 'children' => array(
        '.enabled', '.disabled',
      )),
    )),
  );
  $form['widget']['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  return $form;
}
