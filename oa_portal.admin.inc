<?php

/**
 * Menu callback for admin page
 */
function oa_portal_admin() {
  include_once('oa_portal.forms.inc');
  include_once('oa_portal.pages.inc');

  $output = drupal_get_form('oa_portal_soap_file_form');
  
  $p = oa_portal_api();
  foreach ($p AS $pk => $phase) {
    $output .= '<h3>'. $phase['#title'] .'</h3>';
    $rows = array();
    foreach ($phase AS $sk => $section) {
      if (substr($sk, 0, 1) !== '#') {
        $rows[] = array(l($section['#title'], 'portal/'. $pk .'/'. $sk), 'oa_portal_'. $pk .'_'. $sk .'_form', function_exists('oa_portal_'. $pk .'_'. $sk .'_form'), function_exists('oa_portal_'. $pk .'_'. $sk .'_form_validate'), function_exists('oa_portal_'. $pk .'_'. $sk .'_form_submit'));
      }
    }
    $output .= theme('table', array('Section', 'Form callback', 'form', 'validation', 'submission'), $rows);
  }

  $output .= '<pre>'. $code .'</pre>';
  return $output;
}
