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
 
   /**
   * Constructor; Create the basic query object and fill with default values.
   */
  function init($base_table, $base_field, $options) {
    parent::init($base_table, $base_field, $options);
  }

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
    
    //Authentication details
    $authParams = array('login' => 'contoso\Administrator',
    					'password' => 'pass@word1',
    					'trace' => 1
    					);

    /* Path to the Lists.asmx WSDL file */
    $wsdl = $this->options['sharepoint_views_server_url'].'/Lists.asmx?WSDL';

    //Creating the SOAP client and initializing the GetListItems method parameters
    $client = new SoapClient($wsdl, $authParams);

    $items = sharepoint_get_splist_items($client, 'Test lib');
    
    foreach ($items->items as $key => $item) {
      $view->result[$key] = (object) $item;
    }

    // Set row counter and execute time
    $view->total_rows = count($items->ItemCount);
    $view->execute_time = microtime() - $start;

    // Someday we may have a pager but not today
    $view->pager['current_page'] = 0;
    
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['sharepoint_views_server_url'] = array(
      'default' => 'test',
    );
    $options['sharepoint_views_list_name'] = array(
      'default' => 'test',
    );
    $options['sharepoint_views_list_view'] = array(
      'default' => 'test',
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

    $form['sharepoint_views_list_view'] = array(
      '#type' => 'textfield',
      '#title' => t('View'),
      '#default_value' => $this->options['sharepoint_views_list_view'],
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The name of the view you wish to use. If not specified the default view will be used.'),
    );
  }

  function options_submit(&$form, &$form_state) { 
    views_invalidate_cache();  
  }
}
