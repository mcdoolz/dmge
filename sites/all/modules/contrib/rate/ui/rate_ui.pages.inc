<?php
/**
 * @file
 * Main file for the different page callbacks provided by Rate.
 */

/**
 * Page callback for widgets list.
 */
function rate_ui_widgets_page() {
  $output = '';

  // Select rate widgets.
  $widgets = db_select('rate_widget', 'w')
      ->fields('w', array('wid', 'type', 'name'))
      ->orderBy('w.name', 'asc')
      ->execute()
      ->fetchAll();

  $header = array(
    t('Title'),
    t('Name'),
    t('Operations')
  );

  $rows = array();
  foreach ($widgets as $id => $widget) {
    $edit = l(t('Edit'), "admin/structure/rate/widget/{$widget->type}/edit");
    $delete = l(t('Delete'), "admin/structure/rate/widget/{$widget->type}/delete");
    $rows[] = array(
      check_plain($widget->name),
      check_plain($widget->type),
      "$edit $delete",
    );
  }
  if ($rows) {
    $table_variables = array(
      'header' => $header,
      'rows' => $rows,
    );
    $output .= theme('table', $table_variables);
  }
  else {
    $output .= '<p>' . t('No widgets defined, yet.') . '</p>';
  }

  return $output;
}
