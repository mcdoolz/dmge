<?php
/**
 * @file
 * dmge_artists.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dmge_artists_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_artist|node|artist|form';
  $field_group->group_name = 'group_artist';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'artist';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Artist',
    'weight' => '0',
    'children' => array(
      0 => 'field_artist_tags',
      1 => 'field_image',
      2 => 'field_copy',
      3 => 'field_artist_rating',
      4 => 'title',
      5 => 'path',
      6 => 'field_artist_links',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'classes' => 'group-artist field-group-div',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_artist|node|artist|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Artist');

  return $field_groups;
}
