<?php
/**
 * @file
 * dmge_cloudinary.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dmge_cloudinary_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cloudinary_image_style_preview_image';
  $strongarm->value = FALSE;
  $export['cloudinary_image_style_preview_image'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cloudinary_sdk_api_key';
  $strongarm->value = '688726213211648';
  $export['cloudinary_sdk_api_key'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cloudinary_sdk_api_secret';
  $strongarm->value = 'quk3sv4FXVXV3xcwqOVveFCfHj0';
  $export['cloudinary_sdk_api_secret'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cloudinary_sdk_cloud_name';
  $strongarm->value = 'dmge';
  $export['cloudinary_sdk_cloud_name'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cloudinary_stream_wrapper_folders';
  $strongarm->value = array(
    'samples' => 0,
  );
  $export['cloudinary_stream_wrapper_folders'] = $strongarm;

  return $export;
}
