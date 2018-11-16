<?php
/**
 * @file
 * dmge_maps.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dmge_maps_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_resource_link|taxonomy_term|resource_link|form';
  $field_group->group_name = 'group_resource_link';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'resource_link';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Resource Link',
    'weight' => '0',
    'children' => array(
      0 => 'field_fontawesome_classname',
      1 => 'path',
      2 => 'name',
      3 => 'description',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-resource-link field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_resource_link|taxonomy_term|resource_link|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_resource|node|resource|form';
  $field_group->group_name = 'group_resource';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'resource';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Resource',
    'weight' => '0',
    'children' => array(
      0 => 'field_map_author',
      1 => 'field_map_rating',
      2 => 'field_resource_type',
      3 => 'field_resource_links',
      4 => 'field_resource_tags',
      5 => 'title',
      6 => 'path',
      7 => 'field_image',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-resource field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_resource|node|resource|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Resource');
  t('Resource Link');

  return $field_groups;
}
