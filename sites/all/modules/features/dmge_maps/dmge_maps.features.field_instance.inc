<?php
/**
 * @file
 * dmge_maps.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function dmge_maps_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-resource-field_map_author'.
  $field_instances['node-resource-field_map_author'] = array(
    'bundle' => 'resource',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'field_formatter_class' => '',
          'field_formatter_class_remove_defaults' => '',
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_map_author',
    'label' => 'Map Author',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'apply_chosen' => '',
      ),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-resource-field_map_link'.
  $field_instances['node-resource-field_map_link'] = array(
    'bundle' => 'resource',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If the map file is a Patreon supported unit, copy and paste the link to the file here.  Patreon will do the rest.

If it is not listed on Patreon, you can paste a share link from Google Drive, or Dropbox, or any number of file hosting services.

If you wish, you can paste a YouTube link here as well, but loading videos from YouTube is janky at best.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(
          'field_formatter_class' => '',
          'field_formatter_class_remove_defaults' => '',
        ),
        'type' => 'link_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_map_link',
    'label' => 'Map Link',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => '_blank',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 0,
      'rel_remove' => 'default',
      'title' => 'none',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 0,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-resource-field_map_rating'.
  $field_instances['node-resource-field_map_rating'] = array(
    'bundle' => 'resource',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'rate',
        'settings' => array(
          'displayed' => 3,
          'displayed_just_voted' => 2,
          'field_formatter_class' => '',
          'field_formatter_class_remove_defaults' => '',
        ),
        'type' => 'rate_format_full',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'fences_wrapper' => 'div_div_div',
    'field_name' => 'field_map_rating',
    'label' => 'Map Rating',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'rate',
      'settings' => array(),
      'type' => 'rate_widget_map',
      'weight' => 7,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('If the map file is a Patreon supported unit, copy and paste the link to the file here.  Patreon will do the rest.

If it is not listed on Patreon, you can paste a share link from Google Drive, or Dropbox, or any number of file hosting services.

If you wish, you can paste a YouTube link here as well, but loading videos from YouTube is janky at best.');
  t('Map Author');
  t('Map Link');
  t('Map Rating');

  return $field_instances;
}
