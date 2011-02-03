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
}
