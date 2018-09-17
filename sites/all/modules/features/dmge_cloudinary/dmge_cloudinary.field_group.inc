<?php
/**
 * @file
 * dmge_cloudinary.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dmge_cloudinary_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cloudinary|user|user|default';
  $field_group->group_name = 'group_cloudinary';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Cloudinary',
    'weight' => '4',
    'children' => array(
      0 => 'field_cloudinary_cloudname',
      1 => 'field_cloudinary_apikey',
      2 => 'field_cloudinary_apisecret',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Cloudinary',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-cloudinary field-group-fieldset',
        'description' => 'Your Cloudinary API details if you\'ve entered any.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_cloudinary|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_cloudinary|user|user|form';
  $field_group->group_name = 'group_cloudinary';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Cloudinary',
    'weight' => '6',
    'children' => array(
      0 => 'field_cloudinary_cloudname',
      1 => 'field_cloudinary_apikey',
      2 => 'field_cloudinary_apisecret',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Cloudinary',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-cloudinary field-group-fieldset',
        'description' => 'Register for an account with Cloudinary and you can use their service to store and convert resources live for the DMGE.
<br><img src="http://ddmge.ffs/sites/all/modules/contrib/cloudinary/modules/cloudinary_sdk/images/api.png">',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_cloudinary|user|user|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Cloudinary');

  return $field_groups;
}
