<?php

/**
 * @file
 * Views query plugin to build and transmit a SPARQL query to an endpoint.
 */

class droogle_plugin_query extends views_plugin_query {
  public $query_string = array();

  function use_pager() {
    return TRUE;
  }

  /**
   * Add filter for query string later
   */
  function add_where($group, $field, $value, $operator, $escape = TRUE) {
    $value = $escape ? "'" . str_replace(array("'", '\\'), array("\'", "\\\\"), $value) . "'" : $value;
    $this->query_string[] = "$field $operator $value";
  }

  /**
   * Add filter for query string later
   */
  function add_in($group, $field, $value, $escape = TRUE) {
    $value = $escape ? "'" . str_replace(array("'", '\\'), array("\'", "\\\\"), $value) . "'" : $value;
    $this->query_string[] = "$value in $field";
  }

  /**
   * Builds the necessary info to execute the query.
   */
  function build(&$view) {
    // Store the view in the object to be able to use it later.
    $this->view = $view;

    $view->init_pager();

    // Let the pager modify the query to add limits.
    $this->pager->query();
    $this->list_files_params = array();
      // Max number allowed is 1k.
    $this->list_files_params['maxResults'] = $this->limit ? $this->limit : 1000;
    // Make the query string if filtering the query.
    if ($this->query_string) {
      $this->list_files_params['q'] = implode(' and ', $this->query_string);
    }
    if (!empty($_GET['page_token'])) {
      $this->list_files_params['pageToken'] = $_GET['page_token'];
    }
  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   *
   * $view->result should contain an array of objects. The array must use a
   * numeric index starting at 0.
   *
   * @param view $view
   *   The view which is executed.
   */
  function execute(&$view) {
    $start = microtime(TRUE);
    $successful = FALSE;
    $connection_settings = droogle_get_connection_settings(TRUE, !empty($this->og_group) ? $this->og_group : array());
    if ($return_result = droogle_gdrive_connect('droogle', $connection_settings['client_id'], $connection_settings['client_secret'], $connection_settings['refresh_token'])) {
      $client = $return_result['client'];
      $service = $return_result['service'];
      $files = $service->files->listFiles($this->list_files_params);
      if ($file_list = $files->getItems()) {
        foreach ($file_list as $key => $file) {
          $object = (object) ((array) $file);
          $view->result[] = $object;
        }
        $view->total_rows = count($view->result);
        $successful = TRUE;
      }
    }
    if (!$successful) {
      $view->result = array();
      $view->total_rows = 0;
      $view->next_page_token = '';
      $view->current_page_token = NULL;
    }
    else {
      $view->next_page_token = $files->getNextPageToken();
      $view->current_page_token = isset($this->list_files_params['pageToken']) ? $this->list_files_params['pageToken'] : NULL;
    }
    $view->execute_time = microtime(TRUE) - $start;
  }
}
