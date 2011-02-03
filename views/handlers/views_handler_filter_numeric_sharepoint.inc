<?php
// $Id;

/**
 * @file
 *   Views filter handler for Sharepoint numeric filters.
 */

/**
 * Views filter handler for last.fm text filters.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, otherwise other fuctions will fail.
 */
class views_handler_filter_numeric_sharepoint extends views_handler_filter_numeric {

  /**
   * Called to add the field to a query.
   */
  function query() {  
    $this->field_alias = $this->real_field;

    // Add the argument value and method. These will be included in the final
    // last.fm API call in the query string.
    if (isset($this->value, $this->definition['sharepoint'])) {
      $this->query->args[$this->definition['sharepoint']['arg']] = $this->value;
    }
  }

  /**
   * The last.fm API doesn't support anything but equality.
   */
  function operators() {
    $operators = array(
      '=' => array(
        'title' => t('Is equal to'),
        'short' => t('='),
        'method' => 'op_equal',
        'values' => 1,
      ),
    );
    
    return $operators;
  }

  /**
   * Handle the 'left' side of the exposed options form.
   *
   * We remove the operator option since we only have one operators available.
   */
  function expose_form_left(&$form, &$form_state) {
    parent::expose_form_left($form, $form_state);
    $form['expose']['use_operator']['#access'] = FALSE;
    $form['expose']['operator']['#access'] = FALSE;
  }
  
  /**
   * Remove the optional checkbox since it makes no sense in our context.
   */
  function expose_form_right(&$form, &$form_state) {
    parent::expose_form_right($form, $form_state);
    $form['expose']['optional']['#access'] = FALSE;
  }
  
  /**
   * Remove the case sensitive option, since it is irrelevant. 
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['case']['#access'] = FALSE;
  }
}
