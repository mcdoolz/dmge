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
  $field_group->identifier = 'group_artist_link|taxonomy_term|artist_link|form';
  $field_group->group_name = 'group_artist_link';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'artist_link';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Artist Link',
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
        'classes' => 'group-artist-link field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_artist_link|taxonomy_term|artist_link|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_artist_tags|node|artist|default';
  $field_group->group_name = 'group_artist_tags';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'artist';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Artist Tags',
    'weight' => '4',
    'children' => array(
      0 => 'field_artist_tags',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Artist Tags',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'artist-tags',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_artist_tags|node|artist|default'] = $field_group;

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
  t('Artist Link');
  t('Artist Tags');

  return $field_groups;
}
