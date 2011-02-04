<?php
// $Id$

/**
 * API functions for the Sharepoint module.
 */


/**
 * Get a sharepoint client pointing to a particular sharepoint web service.
 *
 * @param string $service the web service.
 *
 * @return SoapClient a SoapClient ready for use.
 */
function sharepoint_get_client($service = 'lists') {
  if (!sharepoint_is_configured()) {
    throw new SharePointNotConfiguredException('Sharepoint module is not configured.');
  }

  //Authentication details
  $authParams = array(
    'login'    => variable_get('sharepoint_server_username', FALSE),
    'password' => variable_get('sharepoint_server_password', FALSE),
    'trace'    => TRUE,
  );

  // Typical format for sharepoint web-services URLs:
  // - http://172.10.0.1/_vti_bin/lists.asmx
  //   <-- server URL -->         <svc>
  $host = variable_get('sharepoint_server_url', '');
  $wsdl = "{$host}/_vti_bin/{$service}.asmx?WSDL";
  try {
    //Creating the SOAP client and initializing the GetListItems method parameters
    $soapClient = new SoapClient($wsdl, $authParams);
    return $soapClient;
  }
  catch(SoapFault $fault) {
    // Throw faults as a SharePoint_Exception.
    throw new SharePointConnectionException(t('Could not connect to SharePoint Server:') . $fault->getMessage());
  }
}
