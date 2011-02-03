<?php
// $Id;

/**
 * @file
 *   Views field handler for Sharepoimt fields.
 */

/**
 * Views field handler for Sharepoint fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, otherwise the parent render function is going to
 * fail.
 */
class views_handler_field_sharepoint extends views_handler_field {

  /**
   * Called to add the field to a query.
   */
  function query() {  
    $this->field_alias = $this->real_field;  
  }  

  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {
    if (isset($values->{$this->options['real_field']})) {
      $value = $values->{$this->options['real_field']};
      return check_plain($value);
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['real_field'] = array(
      'default' => ''
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    //Authentication details
    $authParams = array('login' => 'contoso\Administrator',
    					'password' => 'pass@word1',
    					'trace' => 1
    					);

    /* Path to the Lists.asmx WSDL file */
    $wsdl = $this->view->display['default']->display_options['query']['options']['sharepoint_views_server_url'].'/Lists.asmx?WSDL';

    //Creating the SOAP client and initializing the GetListItems method parameters
    $client = new SoapClient($wsdl, $authParams);

    $info = sharepoint_get_splist_info($client, $this->view->display['default']->display_options['query']['options']['sharepoint_views_list_name']);

    $field_options = array();
    foreach ($info->view_fields as $field) {
      $fields['ows_'.$field['Name']] = $field['Name'];
    }
    
    $form['real_field'] = array(
      '#title' => t('Sharepoint field'),
      '#type' => 'select',
      '#default_value' => $this->options['real_field'],
      '#options' => $fields,
    );
    parent::options_form($form, $form_state);
  }
  
  function options_submit(&$form, &$form_state) { 
    $this->definition['sharepoint']['real field'] = $form_state['values']['options']['real_field'];
  }
}
