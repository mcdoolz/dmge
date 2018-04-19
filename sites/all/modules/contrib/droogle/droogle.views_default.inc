<?php
/**
 * @file
 * droogle.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function droogle_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'droogle_browser';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'google_drive';
  $view->human_name = 'Drive Browser';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'drive';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'icon_link' => 'icon_link',
    'alternate_link' => 'alternate_link',
  );
  /* Field: Google Drive: Icon Link */
  $handler->display->display_options['fields']['icon_link']['id'] = 'icon_link';
  $handler->display->display_options['fields']['icon_link']['table'] = 'google_drive';
  $handler->display->display_options['fields']['icon_link']['field'] = 'icon_link';
  $handler->display->display_options['fields']['icon_link']['label'] = '';
  $handler->display->display_options['fields']['icon_link']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['icon_link']['alter']['text'] = '<img src="[icon_link]" />';
  $handler->display->display_options['fields']['icon_link']['element_label_colon'] = FALSE;
  /* Field: Google Drive: Link */
  $handler->display->display_options['fields']['alternate_link']['id'] = 'alternate_link';
  $handler->display->display_options['fields']['alternate_link']['table'] = 'google_drive';
  $handler->display->display_options['fields']['alternate_link']['field'] = 'alternate_link';
  $handler->display->display_options['fields']['alternate_link']['label'] = '';
  $handler->display->display_options['fields']['alternate_link']['exclude'] = TRUE;
  $handler->display->display_options['fields']['alternate_link']['element_label_colon'] = FALSE;
  /* Field: Google Drive: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'google_drive';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[alternate_link]';
  $handler->display->display_options['fields']['title']['element_type'] = '0';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = '0';
  /* Contextual filter: Google Drive: OG Group */
  $handler->display->display_options['arguments']['og_group']['id'] = 'og_group';
  $handler->display->display_options['arguments']['og_group']['table'] = 'google_drive';
  $handler->display->display_options['arguments']['og_group']['field'] = 'og_group';
  $handler->display->display_options['arguments']['og_group']['default_action'] = 'default';
  $handler->display->display_options['arguments']['og_group']['default_argument_type'] = 'og_context';
  $handler->display->display_options['arguments']['og_group']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['og_group']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['og_group']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Google Drive: Mime Type */
  $handler->display->display_options['filters']['mime_type']['id'] = 'mime_type';
  $handler->display->display_options['filters']['mime_type']['table'] = 'google_drive';
  $handler->display->display_options['filters']['mime_type']['field'] = 'mime_type';
  $handler->display->display_options['filters']['mime_type']['operator'] = '!=';
  $handler->display->display_options['filters']['mime_type']['value'] = 'application/vnd.google-apps.folder';
  /* Filter criterion: Google Drive: Trashed */
  $handler->display->display_options['filters']['trashed']['id'] = 'trashed';
  $handler->display->display_options['filters']['trashed']['table'] = 'google_drive';
  $handler->display->display_options['filters']['trashed']['field'] = 'trashed';
  $handler->display->display_options['filters']['trashed']['value'] = '0';
  /* Filter criterion: Google Drive: Parents */
  $handler->display->display_options['filters']['parents']['id'] = 'parents';
  $handler->display->display_options['filters']['parents']['table'] = 'google_drive';
  $handler->display->display_options['filters']['parents']['field'] = 'parents';
  $handler->display->display_options['filters']['parents']['value'] = 'root';
  /* Filter criterion: Google Drive: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'google_drive';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  $export[$view->name] = $view;

  return $export;
}
