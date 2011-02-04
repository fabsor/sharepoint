<?php
// $Id$

/**
 * @file
 * Admin-page callbacks for the sharepoint module.
 */


/**
 * Menu callback at admin/config/services/sharepoint
 */
function sharepoint_server_connection_settings_former() {
  $form['sharepoint_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#default_value' =>  variable_get('sharepoint_server_url', ''),
    '#size' => 100,
    '#maxlength' => 100,
    '#description' => t("The base URL to the sharepoint webservice endpoint (including the port number if it's not running on port 80), but excluding _vti_bin.<br />E.g. %example_url", array('%example_url' => 'http://sharepoint.example.com:5966/')),
  );

  $form['sharepoint_server_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' =>  variable_get('sharepoint_server_username', ''),
    '#size' => 100,
    '#maxlength' => 300,
    '#description' => t('The username for the sharepoint webservice.'),
  );
  
  $form['sharepoint_server_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' =>  variable_get('sharepoint_server_password', ''),
    '#size' => 100,
    '#maxlength' => 300,
    '#description' => t('The password for the sharepoint webservice.'),
  );

  return system_settings_form($form);
}

/**
 * Menu callback at admin/config/services/sharepoint/lists
 */
function sharepoint_admin_lists() {
  // if (!sharepoint_is_configured()) {
  //   throw new SharePointNotConfiguredException('Sharepoint module is not configured.');
  // }
  module_load_include('inc', 'sharepoint');

  $options = array();
  try {
    $client = sharepoint_get_client('lists');
    $lists = sharepoint_get_splist_collection($client);
    foreach ($lists as $id => $list) {
      $options[$id] = $list['Title'];
    }
  } 
  catch(SharePointException $exception) {
    drupal_set_message(t('The connection to the SharePoint server could not be established!'), 'error');
  }

  $form['sharepoint_listname'] = array(
    '#type' => 'select',
    '#title' => t('List Name'),
    '#default_value' =>  variable_get('sharepoint_listname', ''),
    '#options' => $options,
    '#description' => t('The name of the List you want to publish on your site'),
  );
 
  $form['sharepoint_view'] = array(
    '#type' => 'textfield',
    '#title' => t('View'),
    '#default_value' =>  variable_get('sharepoint_view', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('The name of the view to use'),
  );
  
  $form['sharepoint_num_records'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of records'),
    '#default_value' =>  variable_get('sharepoint_num_records', ''),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The number of records to retrieve'),
  );
  
  $form['sharepoint_CAMLquery'] = array(
    '#type' => 'textarea',
    '#title' => t('CAML query (optional)'),
    '#default_value' =>  variable_get('sharepoint_CAMLquery', ''),
    '#rows' => 10,
    '#maxlength' => 300,
    '#description' => t('An optional CAML query to filter the list items. Leave empty to get all list items.'),
  );
  
  return system_settings_form($form);
}
