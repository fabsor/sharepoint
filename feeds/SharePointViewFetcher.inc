<?php
// $Id$

/**
 * @file
 * SharePoint View Fetcher Plugin for Feeds.
 */

/**
 * Fetcher Result Class for SharePoint Views.
 */
class SharePointViewFetcherResult extends FeedsFetcherResult {
  public $list;
  public $view;

  /**
   * Constructor.
   */
  public function __construct($list, $view) {
    $this->list = $list;
    $this->view = $view;
    parent::__construct('');
  }

  /**
   * Overrides FeedsFetcherResult::getRaw();
   */
  public function getRaw() {
    // Let's just fail miserably if we don't have a connection.
    $client = sharepoint_get_client('lists');
    return sharepoint_get_splist_items($client, $this->list, $this->view);
  }
}

/**
 * Fetches data via HTTP.
 */
class SharePointViewFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    return new SharePointViewFetcherResult($this->config['list'], $this->config['view']);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'list' => '',
      'view' => FALSE,
      'num_records' => 0,
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

   $form['num_records'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of records'),
    '#default_value' => $this->config['num_records'],
    '#size' => 30,
    '#maxlength' => 5,
    '#description' => t('The number of records to retrieve. Enter 0 for no limit'),
  );
    return $form;
  }
}
