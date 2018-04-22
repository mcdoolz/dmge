<?php
/**
 * @file
 * dmge_maps.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function dmge_maps_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'library';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Library';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Library';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Map Rating */
  $handler->display->display_options['fields']['field_map_rating']['id'] = 'field_map_rating';
  $handler->display->display_options['fields']['field_map_rating']['table'] = 'field_data_field_map_rating';
  $handler->display->display_options['fields']['field_map_rating']['field'] = 'field_map_rating';
  $handler->display->display_options['fields']['field_map_rating']['label'] = '';
  $handler->display->display_options['fields']['field_map_rating']['element_type'] = '0';
  $handler->display->display_options['fields']['field_map_rating']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_map_rating']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_map_rating']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_map_rating']['settings'] = array(
    'displayed' => '3',
    'displayed_just_voted' => '2',
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  $handler->display->display_options['fields']['field_map_rating']['field_api_classes'] = TRUE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'resource' => 'resource',
  );

  /* Display: Resources Admin Page */
  $handler = $view->new_display('page', 'Resources Admin Page', 'resources_admin_page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['path'] = 'admin/content/resources';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Library';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Resources Block */
  $handler = $view->new_display('block', 'Resources Block', 'resources_block');
  $export['library'] = $view;

  return $export;
}
