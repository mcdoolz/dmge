<?php

/**
 * @file
 * Variable module integration for the Site Map module.
 */

/**
 * Implements hook_variable_info().
 */
function site_map_variable_info($options) {
  $variable['site_map_page_title'] = array(
    'type' => 'string',
    'title' => t('Site map title', array(), $options),
    'description' => t('Page title that will be used on the <a href="@link">site map page</a>. It should only be set to override the translatable built-in "Site map" title. Note: If you set something here, the title will no longer be translatable.', array('@link' => url('sitemap'))),
    'default' => '',
  );
  $variable['site_map_message'] = array(
    'type' => 'text_format',
    'title' => t('Site map message'),
    'description' => t('Message that will be used on the <a href="@link">site map page</a>.', array('@link' => url('sitemap'))),
    'default' => t('The site map provides an overview of all website content.'),
  );
  $variable['site_map_show_menus'] = array(
    'type' => 'array',
    'title' => t('Menus to include in the site map', array(), $options),
    'description' => t('Menus to include that will be used on the <a href="@link">site map page</a>.', array('@link' => url('sitemap'))),
    'default' => array(),
  );
  $variable['site_map_rss_front'] = array(
    'type' => 'string',
    'title' => t('RSS feed for front page', array(), $options),
    'description' => t('The RSS feed for the front page, default is rss.xml.'),
    'default' => 'rss.xml',
  );

  return $variable;
}
