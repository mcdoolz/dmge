<?php
/**
 * @file
 * dmge_artists.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function dmge_artists_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'artists';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Artists';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Artists';
  $handler->display->display_options['css_class'] = 'artists';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'artist';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
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
  /* Field: Content: Artist Rating */
  $handler->display->display_options['fields']['field_artist_rating']['id'] = 'field_artist_rating';
  $handler->display->display_options['fields']['field_artist_rating']['table'] = 'field_data_field_artist_rating';
  $handler->display->display_options['fields']['field_artist_rating']['field'] = 'field_artist_rating';
  $handler->display->display_options['fields']['field_artist_rating']['label'] = '';
  $handler->display->display_options['fields']['field_artist_rating']['element_type'] = '0';
  $handler->display->display_options['fields']['field_artist_rating']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artist_rating']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_artist_rating']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_artist_rating']['settings'] = array(
    'displayed' => '3',
    'displayed_just_voted' => '2',
    'field_formatter_class' => '',
    'field_formatter_class_remove_defaults' => 0,
  );
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
    'artist' => 'artist',
  );

  /* Display: Artist Admin Page */
  $handler = $view->new_display('page', 'Artist Admin Page', 'artist_admin_page');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['path'] = 'admin/content/artists';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Artists';
  $handler->display->display_options['menu']['name'] = 'management';

  /* Display: Artist Block */
  $handler = $view->new_display('block', 'Artist Block', 'artist_block');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Vote results */
  $handler->display->display_options['relationships']['votingapi_cache']['id'] = 'votingapi_cache';
  $handler->display->display_options['relationships']['votingapi_cache']['table'] = 'node';
  $handler->display->display_options['relationships']['votingapi_cache']['field'] = 'votingapi_cache';
  $handler->display->display_options['relationships']['votingapi_cache']['votingapi'] = array(
    'value_type' => 'points',
    'tag' => 'vote',
    'function' => 'sum',
  );
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Vote results: Function */
  $handler->display->display_options['sorts']['function']['id'] = 'function';
  $handler->display->display_options['sorts']['function']['table'] = 'votingapi_cache';
  $handler->display->display_options['sorts']['function']['field'] = 'function';
  $handler->display->display_options['sorts']['function']['relationship'] = 'votingapi_cache';
  $handler->display->display_options['sorts']['function']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['function']['expose']['label'] = 'Rating';
  $export['artists'] = $view;

  return $export;
}
