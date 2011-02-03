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
    'query class' => 'sharepoint_views_plugin_query'
  );

  return $data;
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

    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function sharepoint_views_plugins() {
  return array(
    'module' => 'sharepoint',
    'query' => array(
      'sharepoint_views_plugin_query' => array(
        'title' => t('Sharepoint Query'),
        'help' => t('Sharepoint query object.'),
        'handler' => 'sharepoint_views_plugin_query',
        'path' => drupal_get_path('module', 'sharepoint') .'/views/plugins',
      ),
    ),
  );
}

