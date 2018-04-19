<?php

/**
 * @file
 * Provide Views data, handler and plugins for google drive calls.
 */

/**
 * Implements hook_views_data().
 */
function droogle_views_data() {
  $data['google_drive']['table']['group'] = t('Google Drive');

  $data['google_drive']['table']['base'] = array(
    'title' => t('Google Drive query'),
    'help' => t('Connects to a Google Drive endpoint.'),
    'query class' => 'droogle_query',
  );

  // For list of filterable fields:
  // @see https://developers.google.com/drive/web/search-parameters
  $data['google_drive']['title'] = array(
    'title' => t('Title'),
    'help' => t("The title of the document"),
    'field' => array(
      'handler' => 'droogle_handler_field',
    ),
    'filter' => array(
      'handler' => 'droogle_handler_filter_string',
    ),
  );

  $data['google_drive']['full_text'] = array(
    'title' => t('Full Text'),
    'help' => t("The full text of the document"),
    'real field' => 'fullText',
    'filter' => array(
      'handler' => 'droogle_handler_filter_string_contains',
    ),
  );

  $data['google_drive']['mime_type'] = array(
    'title' => t('Mime Type'),
    'help' => t("The mime type of the document"),
    'real field' => 'mimeType',
    'field' => array(
      'handler' => 'droogle_handler_field',
    ),
    'filter' => array(
      'handler' => 'droogle_handler_filter_string',
    ),
  );

  $data['google_drive']['alternate_link'] = array(
    'title' => t('Link'),
    'help' => t("The link of the document"),
    'real field' => 'alternateLink',
    'field' => array(
      'handler' => 'droogle_handler_field',
    ),
  );

  $data['google_drive']['created_date'] = array(
    'title' => t('Created Date'),
    'help' => t("The date the document was created"),
    'real field' => 'createdDate',
    'field' => array(
      'handler' => 'droogle_handler_field_date',
    ),
  );

  $data['google_drive']['last_viewed_by_me_date'] = array(
    'title' => t('Last Viewed By Me Date'),
    'help' => t("The date the document was last viewed by me"),
    'real field' => 'lastViewedByMeDate',
    'field' => array(
      'handler' => 'droogle_handler_field_date',
    ),
    'filter' => array(
      'handler' => 'droogle_handler_filter_date',
    ),
  );

  $data['google_drive']['marked_viewed_by_me_date'] = array(
    'title' => t('Marked Viewed By Me Date'),
    'help' => t("The date the document was marked viewed by me"),
    'real field' => 'markedViewedByMeDate',
    'field' => array(
      'handler' => 'droogle_handler_field_date',
    ),
  );

  $data['google_drive']['modified_by_me_date'] = array(
    'title' => t('Modified By Me Date'),
    'help' => t("The date the document was modified by me"),
    'real field' => 'modifiedByMeDate',
    'field' => array(
      'handler' => 'droogle_handler_field_date',
    ),
  );

  $data['google_drive']['modified_date'] = array(
    'title' => t('Modified Date'),
    'help' => t("The date the document was modified"),
    'real field' => 'modifiedDate',
    'field' => array(
      'handler' => 'droogle_handler_field_date',
    ),
    'filter' => array(
      'handler' => 'droogle_handler_filter_date',
    ),
  );

  $data['google_drive']['icon_link'] = array(
    'title' => t('Icon Link'),
    'help' => t("Link to the icon"),
    'real field' => 'iconLink',
    'field' => array(
      'handler' => 'droogle_handler_field',
    ),
  );

  $data['google_drive']['thumbnail_link'] = array(
    'title' => t('Thumbnail Link'),
    'help' => t("Link to the thumbnail"),
    'real field' => 'thumbnailLink',
    'field' => array(
      'handler' => 'droogle_handler_field',
    ),
  );

  $data['google_drive']['trashed'] = array(
    'title' => t('Trashed'),
    'help' => t("Whether the document has been trashed."),
    'filter' => array(
      'handler' => 'droogle_handler_filter_boolean',
    ),
  );

  $data['google_drive']['starred'] = array(
    'title' => t('Starred'),
    'help' => t("Whether the document has been starred."),
    'filter' => array(
      'handler' => 'droogle_handler_filter_boolean',
    ),
  );


  $data['google_drive']['sharedWithMe'] = array(
    'title' => t('Shared With Me'),
    'help' => t("Whether the document has been shared with me."),
    'filter' => array(
      'handler' => 'droogle_handler_filter_boolean',
    ),
  );

  $data['google_drive']['parents'] = array(
    'title' => t('Parents'),
    'help' => t("Parent folder ID (use 'root' for top level)"),
    'filter' => array(
      'handler' => 'droogle_handler_filter_in',
    ),
  );

  $data['google_drive']['owners'] = array(
    'title' => t('Owners'),
    'help' => t("The owners of the document"),
    'filter' => array(
      'handler' => 'droogle_handler_filter_in',
    ),
  );

  $data['google_drive']['writers'] = array(
    'title' => t('Owners'),
    'help' => t("The writers of the document"),
    'filter' => array(
      'handler' => 'droogle_handler_filter_in',
    ),
  );

  $data['google_drive']['readers'] = array(
    'title' => t('Readers'),
    'help' => t("The readers of the document"),
    'filter' => array(
      'handler' => 'droogle_handler_filter_in',
    ),
  );

  if (module_exists('og_context')) {
    // Configure what group is inputted.
    $data['google_drive']['og_group'] = array(
      'title' => t('OG Group'),
      'help' => t("Organic Group Context"),
      'argument' => array(
        'handler' => 'droogle_handler_argument_og_group',
     ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function droogle_views_plugins() {
  return array(
    // Query plugins
    'query' => array(
      'droogle_query' => array(
        'title' => t('Google Drive Query'),
        'help' => t('Query will be generated and run against a Google Drive.'),
        'handler' => 'droogle_plugin_query',
      ),
    ),
    'pager' => array(
      'drive' => array(
        'title' => t('Paged output, drive pager'),
        'short title' => t('Mini'),
        'help' => t('Pager for google drive.'),
        'handler' => 'droogle_plugin_pager',
        'help topic' => 'pager-mini',
        'uses options' => TRUE,
        'parent' => 'full',
      ),
    ),
  );
}
