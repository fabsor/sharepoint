<?php
// $Id;

/**
 * @file
 *   Views query plugin for the Sharepoint SOAP API.
 */

/**
 * Views query plugin for the Sharepoint SOAP API.
 */
class views_plugin_query_sharepoint extends views_plugin_query {
 
  // Our argument array. Anything that needs to be passed to the API gets
  // added to this array through the argumet and filter handlers. This array
  // then gets passed to url() along with $api_url and becomes the query 
  // string. 
  var $args;
  
  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  function init($base_table, $base_field) { }

  function use_pager() {
    return FALSE;
  }

  /**
   * Generate a query and a countquery from all of the information supplied
   * to the object.
   *
   * @param $get_count
   *   Provide a countquery if this is true, otherwise provide a normal query.
   */
  function query($view, $get_count = FALSE) {  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   */
  function execute(&$view) {
    $start = microtime();
    
    $this->request->execute();

    // Set row counter and execute time
    $view->total_rows = count($data->{'@attr'}->total);
    $view->execute_time = microtime() - $start;

    // Someday we may have a pager but not today
    $view->pager['current_page'] = 0;
    
  //  kpr($view);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['sharepoint_views_server_url'] = array(
      'default' => '',
    );
    $options['sharepoint_views_list_name'] = array(
      'default' => '',
    );
    $options['sharepoint_list_view'] = array(
      'default' => '',
    );

    return $options;
  }

  /**
   * Add settings for the ui.
   */
  function options_form(&$form, &$form_state) {
    $form['sharepoint_views_server_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Base URL'),
      '#default_value' => $this->options['sharepoint_views_server_url'],
      '#size' => 100,
      '#maxlength' => 100,
      '#description' => t('The base URL to the sharepoint web, including _vti_bin'),
    );

    $form['sharepoint_views_list_name'] = array(
      '#type' => 'textfield',
      '#title' => t('List name'),
      '#default_value' => $this->options['sharepoint_views_list_name'],
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The name of the list whose data you want to retrieve'),
    );

    $form['sharepoint_server_view'] = array(
      '#type' => 'textfield',
      '#title' => t('View'),
      '#default_value' => $this->options['sharepoint_views_view'],
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The name of the view you wish to use. If not specified the default view will be used.'),
    );
  }
}
