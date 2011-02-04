<?php
// $Id$
/**
 * Classes used by the Sharepoint module.
 */


/**
 * Empty SharePointException classes, that might be extended
 * later.
 */

class SharePointException extends Exception {}

// Thrown when the Drupal Sharepoint module is not configured.
class SharePointNotConfiguredException extends SharePointException {}

// Thrown when the server cannot be reached.
class SharePointConnectionException extends SharePointException {}

class SharePointParseException extends SharePointException {}

