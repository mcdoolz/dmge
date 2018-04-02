<?php
/**
 * @file
 * mc.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function mc_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ctas_inner_wrapper|paragraphs_item|cta|default';
  $field_group->group_name = 'group_ctas_inner_wrapper';
  $field_group->entity_type = 'paragraphs_item';
  $field_group->bundle = 'cta';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Calls to Action Inner Wrapper',
    'weight' => '2',
    'children' => array(
      0 => 'field_cta',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Calls to Action Inner Wrapper',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'ctas-inner-wrapper',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_ctas_inner_wrapper|paragraphs_item|cta|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_page|node|page|form';
  $field_group->group_name = 'group_page';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Page',
    'weight' => '0',
    'children' => array(
      0 => 'title',
      1 => 'path',
      2 => 'field_content',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-page field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_page|node|page|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Calls to Action Inner Wrapper');
  t('Page');

  return $field_groups;
}
