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
      $rows[] = $row;
    }
    return $rows;
  }
  
  /**
   * Determine if this field should be included or not.
   * @param string $field the field name
   */
  private function field_in_view($field) {
  }

  public function  configDefaults() {
    return array();
  }
}
