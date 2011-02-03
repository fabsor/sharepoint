<?php
//$Id$

/**
 * Parses a given file as a CSV file.
 */
class SharePointViewParser extends FeedsParser {
  private $list_info;
  
  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $list = $fetcher_result->getRaw();
    
    // Fetch information about the list, so that we can filter out stuff that
    // does not belong.
    if (!isset($this->list_info)) {
      $client = sharepoint_get_client('lists');
      $this->list_info = sharepoint_get_splist_info($client, $fetcher_result->list, '');
    }
    
    $rows = array();
    // We need to filter out the stuff that we want to have.
    foreach ($list->items as $item) {
      foreach ($this->list_info->view_fields as $field) {
        if (isset($item['ows_' . $field['Name']])) {
          $row[$field['Name']] = _sharepoint_get_value($item['ows_' . $field['Name']]);
        }
      }
      // We still need som hidden fields, like the file path.
      // We will append everything we have to the file path
      // and make it absolute.
      if (isset($item['ows_FileRef'])) {
        $row['filepath'] = sharepoint_get_splist_item_filepath($item);
        $row['local_file'] = sharepoint_download_splist_item_document($item);
      }
      $rows[] = $row;
    }
    return new FeedsParserResult($rows);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'list' => '',
      'view' => '',
      'download_files' => FALSE,
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $options = array();
    try {
      $client = sharepoint_get_client('lists');
      $lists = sharepoint_get_splist_collection($client);
      foreach ($lists as $id => $list) {
        // We need to use the Human Readable Name here, because we can't get
        // the list information otherwise.
        $options[$list['Title']] = $list['Title'];
      }
    }
    catch(SharePointException $error) {
      drupal_set_message(t('The connection to the SharePoint server could not be established!'), 'error');
    }


    $form['list'] = array(
      '#type' => 'select',
      '#title' => t('List Name'),
      '#default_value' => $this->config['list'],
      '#options' => $options,
      '#required' => TRUE,
      '#description' => t('The name of the List you want to publish on your site'),
    );

    $form['view'] = array(
      '#type' => 'textfield',
      '#title' => t('View'),
      '#default_value' => $this->config['view'],
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The name of the view to use'),
    );

    $form['download_files'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->config['download_files'],
      '#title' => t('Download Files in this view'),
      '#description' => t('Check this box if you want to download the files in this view.
        Do this if you want to attach the files to the content you create.')
    );

    return $form;
  }
  
  /**
   * Return mapping sources.
   */
  public function getMappingSources() {
    $client = sharepoint_get_client('lists');
    $this->list_info = sharepoint_get_splist_info($client, $this->config['list'], '');
    $mappingResources = array();
    foreach ($this->list_info->view_fields as $field) {
      $mappingResources[$field['Name']] = array(
        'name' => $field['Name'],
      );
    }
    $mappingResources['filepath'] = array(
      'name' => t('File Path'),
      'description' => t('The SharePoint File Path'),
    );
    $mappingResources['local_file'] = array(
      'name' => t('Local File'),
      'description' => t('The SharePoint File stored locally'),
    );
    return $mappingResources + parent::getMappingSources();
  }
}
