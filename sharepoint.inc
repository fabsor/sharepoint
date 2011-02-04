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

/**
 * Get information for a list.
 * You can optionally pass in  view and get data for just the view.
 * @param SoapClient $client the client to use for fetching the list.
 * @param string $list the list name. Only Human-readable list names works currently.
 * @param string $view the view to fetch information on.
 *
 */
function sharepoint_get_splist_info($client, $list, $view = '') {
  $data = $client->GetListAndView(array('listName' => $list, 'viewName' => $view));
  $xml = simplexml_load_string($data->GetListAndViewResult->any);
  $list_info = new stdClass;
  $list_info->list_field_info = _sharepoint_traverse_field_info($xml->List->Fields->Field);
  $list_info->list_info['RegionalSettings'] = array();
  foreach ($xml->List->RegionalSettings[0] as $key => $setting) {
    $list_info->list_info['RegionalSettings'][$key] = _sharepoint_get_value($setting);
  }
  $list_info->list_info['ServerSettings'] = array();
  foreach ($xml->List->ServerSettings[0] as $key => $setting) {
    $list_info->list_info['ServerSettings'][$key] = _sharepoint_get_value($setting);
  }

  $list_info->view_fields = _sharepoint_traverse_field_info($xml->View->ViewFields->FieldRef);
  $list_info->list_info['ListSettings'] = _sharepoint_get_attributes($xml->List);
  return $list_info;
}

function sharepoint_get_splist_items($client, $listname, $view = '', $query = '', $rowLimit = 10, $queryOptions = NULL, $webId = NULL) {
  $parameters = array(
    'listName' => $listname,
    'view' => $view,
    'query' => $query,
    'viewFields' => '',
    'rowLimit' => $rowLimit,
    'queryOptions' => $queryOptions,
    'webID' => $webId,
  );
  try {
    $response = $client->GetListItems($parameters);
    // SimpleXML does not support elements with : in them, so we just
    // strip them out.
    $data = str_replace(array('rs:', 'z:'),'', $response->GetListItemsResult->any);
    $xml = simplexml_load_string($data);

    // The first item is always the list definition. Let's make that into an object.
    $list = new stdClass;
    foreach ($xml->data->attributes() as $key => $value) {
      $list->{$key} = (string)$value;
    }
    $items = array();
    // The rest of the elements are list items. Go through them.
    foreach ($xml->data->row as $item) {
      $item_attributes = $item->attributes();
      $attributes = array();
      foreach ($item_attributes as $key => $value) {
        $attributes[$key] = (string) $value;
      }
      $items[] = $attributes;
    }
    $list->items = $items;
  }
  catch(SoapFault $fault) {
    _sharepoint_throw_connection_error($fault);
  }
  return  $list;
}

/**
 * Traverse field information.
 * @param SimpleXMLElement $field_info
 * @return array the field information.
 */
function _sharepoint_traverse_field_info($field_info) {
  $fields = array();
  foreach ($field_info as $data) {
    $fields[] = _sharepoint_get_attributes($data);
  }
  return $fields;
}

/**
 * Get all attributes for a field.
 * @param SimpleXMLElement $element
 */
function _sharepoint_get_attributes($element) {
  $field = array();
  foreach ($element->attributes() as $key => $attribute) {
    $field[$key] = _sharepoint_get_value($attribute);
  }
  return $field;
}

/**
 * Convert a sharepoint value to the right data type,
 * e.g. string, boolean...
 * @param $sp_value some value from sharepoint.
 */
function _sharepoint_get_value($sp_value) {
  // Is this a boolean?
  $sp_value = strtolower($sp_value) == 'true' || strtolower($sp_value) == 'false' ? ($sp_value == 'TRUE') : (string)$sp_value;
  // It totally was! Let's not do anything more here then.
  if (is_bool($sp_value)) {
    return $sp_value;
  }
  // We can't deal with references to other data currently,
  // so let's just strip it out for now.
  return preg_replace('/(.*);#/', '', $sp_value, 1);
}

/**
 * Download a sharepoint file.
 * @param $list_info the information about the list.
 * @param $item the item containing the file.
 * @param $location the location to download the file to.
 */
function sharepoint_download_splist_item_document($item, $location = 'public://sharepoint') {
  if (isset($item['ows_FileRef'])) {
    $filepath = sharepoint_get_splist_item_filepath($item);
    // Download the file.
    $options = array(
      'headers' => array(
        'Authorization' => 'Basic ' .
          base64_encode(variable_get('sharepoint_username', '') . ':' .
                variable_get('sharepoint_password', '')),
      ),
    );
    $response = drupal_http_request($filepath, $options);

    if ($response->code == 200 || $response->code == 201) {
      // Save the data.
      $local_path = $location . basename($filepath);
      file_save_data($response->data, $local_path);
      return $local_path;
    }
    else {
      throw new SharePointException(t('Could not download file from server. The file was: ') . $filepath, $response->code);
    }
  }
  return FALSE;
}

function sharepoint_get_splist_collection($soapClient, $show_hidden = FALSE) {
  $lists = array();
  try {
    $sxml = simplexml_load_string($soapClient->GetListCollection()->GetListCollectionResult->any);
    foreach ($sxml as $list) {
      $list_attributes = $list->attributes();
      $hidden = (string) $list_attributes->Hidden;
      if (($show_hidden && $hidden == 'True') || $hidden == 'False') {
        $attributes = array();
        foreach ($list_attributes as $key => $value) {
          $attributes[$key] = (string) $value;
        }
        $lists[$attributes['ID']] = $attributes;
      }
    }
    return $lists;
  }
  catch(SoapFault $fault) {
    _sharepoint_throw_connection_error($fault);
  }
}

function sharepoint_get_spsearch_result($soapClient, $keys) {
  $results = array();
	$parameters = array(
    'queryXml' => "<QueryPacket xmlns='urn:Microsoft.Search.Query'><Query><Range><StartAt>1</StartAt><Count>50</Count></Range><Context><QueryText	type='STRING'>".$keys."</QueryText></Context></Query></QueryPacket>",
  );
  try {
		$response = $soapClient->Query($parameters)->QueryResult;
		$sxml = simplexml_load_string($response);

		// If the result does not return any items , don't try to process any further
		if ($sxml === FALSE || $sxml->Response->Status != 'SUCCESS') {
			return $results;
		}
		foreach ($sxml->Response->Range->Results->Document as $res) {
			$results[] = array(
		    'link' => url($res->Action->LinkUrl, array('absolute' => TRUE)),
			  //'type' => check_plain('node'),
			  'title' => $res->Title,
				//'user' => theme('admin'),
		    'date' => strtotime($res->Date),
			  //'node' => $node,
				//'extra' => $extra,
				'score' => $res->attributes()->relevance,
				'snippet' => 'description of '.$res->Title,
				//search_excerpt($keys, $node->body),
			);
		}
	}
	catch(SoapFault $fault) {
    _sharepoint_throw_connection_error($fault);
  }
	return $results;
}

/**
 * Get the absolute path to a sharepoint item file.
 * @param $item the list item
 * @return string the file path.
 */
function sharepoint_get_splist_item_filepath($item) {
  $path = variable_get('sharepoint_serviceurl','') . '/';
  $parts = explode('/', _sharepoint_get_value($item['ows_FileRef']));
  foreach ($parts as &$part) {
    $part = rawurlencode($part);
  }
  return $path . implode('/', $parts);
}