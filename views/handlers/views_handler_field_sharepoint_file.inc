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
class views_handler_field_sharepoint_file extends views_handler_field {
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
    if (isset($values->{'filepath'})) {
      $value = $values->{'filepath'};
      return check_plain(_sharepoint_get_value($value));
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }
}
