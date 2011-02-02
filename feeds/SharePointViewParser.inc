<?php
//$Id$

/**
 * Parses a given file as a CSV file.
 */
class SharePointViewParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    // This is not really much of a parser, so we can do all parsing within
    // this function.
    $list = $fetcher_result->getRaw();
    $rows = array();
    foreach ($list->items as $items) {
      $row['title'] = 'Some Title';
      $rows[] = $row;
    }
    return $rows;
  }

  public function  configDefaults() {
    return array();
  }
}
