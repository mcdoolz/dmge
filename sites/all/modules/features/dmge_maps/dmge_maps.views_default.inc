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
  $handler->display->display_options['css_class'] = 'library resources maps';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'title_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['row_class'] = 'resource';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
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
  $handler->display->display_options['fields']['field_map_rating']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_map_rating']['element_wrapper_class'] = 'resource-rating';
  $handler->display->display_options['fields']['field_map_rating']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_map_rating']['settings'] = array(
    'displayed' => '3',
    'displayed_just_voted' => '2',
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
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
  $handler->display->display_options['fields']['field_map_rating']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_map_rating']['element_wrapper_class'] = 'resource-rating';
  $handler->display->display_options['fields']['field_map_rating']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_map_rating']['settings'] = array(
    'displayed' => '3',
    'displayed_just_voted' => '2',
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'skip_permission_check' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'log' => 0,
      ),
    ),
  );
  /* Field: Content: Author uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'node';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: Content: Map Author */
  $handler->display->display_options['fields']['field_map_author']['id'] = 'field_map_author';
  $handler->display->display_options['fields']['field_map_author']['table'] = 'field_data_field_map_author';
  $handler->display->display_options['fields']['field_map_author']['field'] = 'field_map_author';
  $handler->display->display_options['fields']['field_map_author']['label'] = '';
  $handler->display->display_options['fields']['field_map_author']['element_type'] = '0';
  $handler->display->display_options['fields']['field_map_author']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_map_author']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_map_author']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_map_author']['settings'] = array(
    'bypass_access' => 0,
    'link' => 1,
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  $handler->display->display_options['fields']['field_map_author']['field_api_classes'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  /* Filter criterion: Content: Author uid */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'node';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author uid';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    6 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: Content: Map Author (field_map_author) */
  $handler->display->display_options['filters']['field_map_author_target_id']['id'] = 'field_map_author_target_id';
  $handler->display->display_options['filters']['field_map_author_target_id']['table'] = 'field_data_field_map_author';
  $handler->display->display_options['filters']['field_map_author_target_id']['field'] = 'field_map_author_target_id';
  $handler->display->display_options['filters']['field_map_author_target_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_map_author_target_id']['expose']['operator_id'] = 'field_map_author_target_id_op';
  $handler->display->display_options['filters']['field_map_author_target_id']['expose']['label'] = 'Map Author (field_map_author)';
  $handler->display->display_options['filters']['field_map_author_target_id']['expose']['operator'] = 'field_map_author_target_id_op';
  $handler->display->display_options['filters']['field_map_author_target_id']['expose']['identifier'] = 'field_map_author_target_id';
  $handler->display->display_options['filters']['field_map_author_target_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    6 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
    7 => 0,
    8 => 0,
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
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_map_author_target_id']['id'] = 'field_map_author_target_id';
  $handler->display->display_options['relationships']['field_map_author_target_id']['table'] = 'field_data_field_map_author';
  $handler->display->display_options['relationships']['field_map_author_target_id']['field'] = 'field_map_author_target_id';
  $handler->display->display_options['relationships']['field_map_author_target_id']['label'] = 'Map Author';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = '0';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'resource-title';
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  /* Field: Content: Map Rating */
  $handler->display->display_options['fields']['field_map_rating']['id'] = 'field_map_rating';
  $handler->display->display_options['fields']['field_map_rating']['table'] = 'field_data_field_map_rating';
  $handler->display->display_options['fields']['field_map_rating']['field'] = 'field_map_rating';
  $handler->display->display_options['fields']['field_map_rating']['label'] = '';
  $handler->display->display_options['fields']['field_map_rating']['element_type'] = '0';
  $handler->display->display_options['fields']['field_map_rating']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_map_rating']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_map_rating']['element_wrapper_class'] = 'resource-rating';
  $handler->display->display_options['fields']['field_map_rating']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_map_rating']['settings'] = array(
    'displayed' => '1',
    'displayed_just_voted' => '1',
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_map_author_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['element_type'] = '0';
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title_1']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['title_1']['element_wrapper_class'] = 'resource-author';
  $handler->display->display_options['fields']['title_1']['element_default_classes'] = FALSE;
  /* Field: Content: Resource Tags */
  $handler->display->display_options['fields']['field_resource_tags']['id'] = 'field_resource_tags';
  $handler->display->display_options['fields']['field_resource_tags']['table'] = 'field_data_field_resource_tags';
  $handler->display->display_options['fields']['field_resource_tags']['field'] = 'field_resource_tags';
  $handler->display->display_options['fields']['field_resource_tags']['label'] = '';
  $handler->display->display_options['fields']['field_resource_tags']['element_type'] = '0';
  $handler->display->display_options['fields']['field_resource_tags']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_resource_tags']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_resource_tags']['element_wrapper_class'] = 'resource-tags';
  $handler->display->display_options['fields']['field_resource_tags']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_resource_tags']['settings'] = array(
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  $handler->display->display_options['fields']['field_resource_tags']['delta_limit'] = '10';
  $handler->display->display_options['fields']['field_resource_tags']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_resource_tags']['multi_type'] = 'ul';
  $handler->display->display_options['fields']['field_resource_tags']['separator'] = '';
  /* Field: Content: Resource Type */
  $handler->display->display_options['fields']['field_resource_type']['id'] = 'field_resource_type';
  $handler->display->display_options['fields']['field_resource_type']['table'] = 'field_data_field_resource_type';
  $handler->display->display_options['fields']['field_resource_type']['field'] = 'field_resource_type';
  $handler->display->display_options['fields']['field_resource_type']['label'] = '';
  $handler->display->display_options['fields']['field_resource_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_resource_type']['element_type'] = '0';
  $handler->display->display_options['fields']['field_resource_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_resource_type']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_resource_type']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_resource_type']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_resource_type']['settings'] = array(
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  $handler->display->display_options['fields']['field_resource_type']['field_api_classes'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['description'] = 'You can search by tag, artist, filename, etc.';
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'combine';
  $handler->display->display_options['filters']['combine']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    6 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
    7 => 0,
    8 => 0,
  );
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'title' => 'title',
    'title_1' => 'title_1',
    'field_resource_tags' => 'field_resource_tags',
    'field_resource_type' => 'field_resource_type',
  );

  /* Display: Maps of the Artist Block */
  $handler = $view->new_display('block', 'Maps of the Artist Block', 'author_maps_block');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Map Author (field_map_author) */
  $handler->display->display_options['arguments']['field_map_author_target_id']['id'] = 'field_map_author_target_id';
  $handler->display->display_options['arguments']['field_map_author_target_id']['table'] = 'field_data_field_map_author';
  $handler->display->display_options['arguments']['field_map_author_target_id']['field'] = 'field_map_author_target_id';
  $handler->display->display_options['arguments']['field_map_author_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_map_author_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_map_author_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_map_author_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_map_author_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_map_author_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_map_author_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_map_author_target_id']['validate_options']['types'] = array(
    'artist' => 'artist',
  );

  /* Display: Maps of the User */
  $handler = $view->new_display('block', 'Maps of the User', 'my_resources_block');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: A few good maps */
  $handler = $view->new_display('block', 'A few good maps', 'frontpage_maps');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'frontpage resources';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '6';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = '0';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'resource-title';
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  /* Field: Field: Image */
  $handler->display->display_options['fields']['field_image']['id'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['table'] = 'field_data_field_image';
  $handler->display->display_options['fields']['field_image']['field'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['label'] = '';
  $handler->display->display_options['fields']['field_image']['element_type'] = '0';
  $handler->display->display_options['fields']['field_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_image']['element_wrapper_class'] = 'resource-image';
  $handler->display->display_options['fields']['field_image']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['settings'] = array(
    'image_style' => '275x275',
    'image_link' => 'content',
    'field_formatter_class' => 'resource-image',
    'field_formatter_class_remove_defaults' => 1,
  );
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  /* Filter criterion: Field: Image (field_image:fid) */
  $handler->display->display_options['filters']['field_image_fid']['id'] = 'field_image_fid';
  $handler->display->display_options['filters']['field_image_fid']['table'] = 'field_data_field_image';
  $handler->display->display_options['filters']['field_image_fid']['field'] = 'field_image_fid';
  $handler->display->display_options['filters']['field_image_fid']['operator'] = 'not empty';

  /* Display: Editor Library */
  $handler = $view->new_display('block', 'Editor Library', 'editor_library');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = '0';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'resource-title';
  $handler->display->display_options['fields']['title']['element_default_classes'] = FALSE;
  /* Field: Field: Image */
  $handler->display->display_options['fields']['field_image']['id'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['table'] = 'field_data_field_image';
  $handler->display->display_options['fields']['field_image']['field'] = 'field_image';
  $handler->display->display_options['fields']['field_image']['label'] = '';
  $handler->display->display_options['fields']['field_image']['element_type'] = '0';
  $handler->display->display_options['fields']['field_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_image']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_image']['element_wrapper_class'] = 'resource-image';
  $handler->display->display_options['fields']['field_image']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_image']['settings'] = array(
    'image_style' => '150x200',
    'image_link' => '',
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  /* Field: Content: Resource Links */
  $handler->display->display_options['fields']['field_resource_links']['id'] = 'field_resource_links';
  $handler->display->display_options['fields']['field_resource_links']['table'] = 'field_data_field_resource_links';
  $handler->display->display_options['fields']['field_resource_links']['field'] = 'field_resource_links';
  $handler->display->display_options['fields']['field_resource_links']['label'] = '';
  $handler->display->display_options['fields']['field_resource_links']['element_type'] = '0';
  $handler->display->display_options['fields']['field_resource_links']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_resource_links']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_resource_links']['element_wrapper_class'] = 'resource-link';
  $handler->display->display_options['fields']['field_resource_links']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_resource_links']['click_sort_column'] = 'field_resource_link_url';
  $handler->display->display_options['fields']['field_resource_links']['settings'] = array(
    'view_mode' => 'default',
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 1,
  );
  $handler->display->display_options['fields']['field_resource_links']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_resource_links']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_resource_links']['multi_type'] = 'ul';
  $handler->display->display_options['fields']['field_resource_links']['separator'] = '';
  /* Field: Content: Resource Tags */
  $handler->display->display_options['fields']['field_resource_tags']['id'] = 'field_resource_tags';
  $handler->display->display_options['fields']['field_resource_tags']['table'] = 'field_data_field_resource_tags';
  $handler->display->display_options['fields']['field_resource_tags']['field'] = 'field_resource_tags';
  $handler->display->display_options['fields']['field_resource_tags']['label'] = '';
  $handler->display->display_options['fields']['field_resource_tags']['element_type'] = '0';
  $handler->display->display_options['fields']['field_resource_tags']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_resource_tags']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_resource_tags']['element_wrapper_class'] = 'resource-tags';
  $handler->display->display_options['fields']['field_resource_tags']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_resource_tags']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_resource_tags']['settings'] = array(
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
  $handler->display->display_options['fields']['field_resource_tags']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_resource_tags']['multi_type'] = 'ul';
  $handler->display->display_options['fields']['field_resource_tags']['separator'] = '';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  $export['library'] = $view;

  return $export;
}
