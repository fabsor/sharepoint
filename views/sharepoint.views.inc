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

