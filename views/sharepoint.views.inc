<?php
// $Id;

/**
 * @file
 *   Hooks for Views API integration
 */

/**
 * Implementation of hook_views_data().
 */
function sharepoint_views_data() {
  $data = array();
  
  // Base data
  $data['sharepoint']['table']['group']  = t('Sharepoint');
  $data['sharepoint']['table']['base'] = array(
    'title' => t('Sharepoint data'),
    'help' => t('Query Sharepoint data.'),
    'query class' => 'views_plugin_query_sharepoint'
  );
  $data['sharepoint']['num_records'] = array(
    'title' => t('Number of Records'),
    'help' => t('The number of records you want to return.'),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric_sharepoint',
      'sharepoint' => array(
        'arg' => 'num_records',
       ),
    ),
  );

  $data['sharepoint']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of this list item.'),
    'field' => array(
      'handler' => 'views_handler_field_sharepoint',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_plugins().
 */
function sharepoint_views_plugins() {
  return array(
    'module' => 'sharepoint',
    'query' => array(
      'views_plugin_query_sharepoint' => array(
        'title' => t('Sharepoint Query'),
        'help' => t('Sharepoint query object.'),
        'handler' => 'views_plugin_query_sharepoint',
        'path' => drupal_get_path('module', 'sharepoint') .'/views/plugins',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function sharepoint_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'sharepoint') . '/views/handlers',
    ),
    'handlers' => array(
      'views_handler_filter_numeric_sharepoint' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
      'views_handler_field_sharepoint' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
