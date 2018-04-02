<?php
/**
 * @file
 * block_attributes_fe_block_test.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function block_attributes_fe_block_test_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['user-online'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'online',
    'module' => 'user',
    'node_types' => array(),
    'options' => 'a:3:{s:10:"attributes";a:6:{s:9:"accesskey";s:1:"A";s:5:"align";s:5:"right";s:5:"class";a:3:{i:0;s:15:"fe_block-class1";i:1;s:15:"fe_block-class2";i:2;s:15:"fe_block-class3";}s:2:"id";s:12:"fe_block-id1";s:5:"style";s:45:"font-weight: bold;text-decoration: underline;";s:5:"title";s:32:"FE Block - Block lvl title test1";}s:16:"title_attributes";a:5:{s:5:"align";s:6:"center";s:5:"class";a:3:{i:0;s:21:"fe_block_title-class1";i:1;s:21:"fe_block_title-class2";i:2;s:21:"fe_block_title-class3";}s:2:"id";s:18:"fe_block_title-id1";s:5:"style";s:37:"font-size: 11px; font-weight: normal;";s:5:"title";s:34:"FE Block - Block Title title test1";}s:18:"content_attributes";a:4:{s:5:"align";s:4:"left";s:2:"id";s:20:"fe_block_content-id1";s:5:"style";s:19:"text-align: center;";s:5:"title";s:36:"FE Block - Block Content title test1";}}',
    'pages' => '<front>',
    'roles' => array(),
    'themes' => array(
      'bartik' => array(
        'region' => 'content',
        'status' => 1,
        'theme' => 'bartik',
        'weight' => -7,
      ),
    ),
    'title' => 'Block Attributes Test Who\'s Online with FE Block',
    'visibility' => 1,
  );

  return $export;
}
