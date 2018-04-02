<?php
/**
 * @file
 * mc.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mc_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "multifield" && $api == "") {
    return array("version" => "");
  }
  if ($module == "multifield" && $api == "multifield") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function mc_image_default_styles() {
  $styles = array();

  // Exported image style: 1000x600.
  $styles['1000x600'] = array(
    'label' => '1000x600',
    'effects' => array(
      6 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 1000,
          'height' => 600,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 150x200.
  $styles['150x200'] = array(
    'label' => '150x200',
    'effects' => array(
      12 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 150,
          'height' => 200,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 1900x823.
  $styles['1900x823'] = array(
    'label' => '1900x823',
    'effects' => array(
      8 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 1900,
          'height' => 823,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 1920x820.
  $styles['1920x820'] = array(
    'label' => '1920x820',
    'effects' => array(
      4 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 1920,
          'height' => 820,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 200x200.
  $styles['200x200'] = array(
    'label' => '200x200',
    'effects' => array(
      11 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 200,
          'height' => 200,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 2048x1280.
  $styles['2048x1280'] = array(
    'label' => '2048x1280',
    'effects' => array(
      1 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 2048,
          'height' => 1280,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 275x275.
  $styles['275x275'] = array(
    'label' => '275x275',
    'effects' => array(
      10 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 275,
          'height' => 275,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 32x32.
  $styles['32x32'] = array(
    'label' => '32x32',
    'effects' => array(
      2 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 32,
          'height' => 32,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 360x275.
  $styles['360x275'] = array(
    'label' => '360x275',
    'effects' => array(
      3 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 360,
          'height' => 275,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 365x300.
  $styles['365x300'] = array(
    'label' => '365x300',
    'effects' => array(
      9 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 365,
          'height' => 300,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 460x460.
  $styles['460x460'] = array(
    'label' => '460x460',
    'effects' => array(
      7 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 460,
          'height' => 460,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: 90x90.
  $styles['90x90'] = array(
    'label' => '90x90',
    'effects' => array(
      5 => array(
        'name' => 'smartcrop_scale_and_crop',
        'data' => array(
          'width' => 90,
          'height' => 90,
          'upscale' => 1,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function mc_node_info() {
  $items = array(
    'page' => array(
      'name' => t('Page'),
      'base' => 'node_content',
      'description' => t('Use <em>basic pages</em> for your static content, such as an \'About us\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_paragraphs_info().
 */
function mc_paragraphs_info() {
  $items = array(
    'block' => array(
      'name' => 'Block',
      'bundle' => 'block',
      'locked' => '1',
    ),
    'copy' => array(
      'name' => 'Copy',
      'bundle' => 'copy',
      'locked' => '1',
    ),
    'cta' => array(
      'name' => 'Calls to Action',
      'bundle' => 'cta',
      'locked' => '1',
    ),
    'image' => array(
      'name' => 'Image',
      'bundle' => 'image',
      'locked' => '1',
    ),
    'slider' => array(
      'name' => 'Slider',
      'bundle' => 'slider',
      'locked' => '1',
    ),
    'sxs' => array(
      'name' => 'Side by Side',
      'bundle' => 'sxs',
      'locked' => '1',
    ),
  );
  return $items;
}
