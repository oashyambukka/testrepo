<?php

/**
 * Menu callback for admin page
 */
function oa_portal_admin() {
  include_once('oa_portal.forms.inc');

  $output = drupal_get_form('oa_portal_soap_file_form');
  
  $p = oa_portal_api();
  foreach ($p AS $pk => $phase) {
    $output .= '<h3>'. $phase['#title'] .'</h3>';
    $rows = array();
    foreach ($phase AS $sk => $section) {
      if (substr($sk, 0, 1) !== '#') {
        $rows[] = array(l($section['#title'], 'portal/'. $pk .'/'. $sk), 'oa_portal_'. $pk .'_'. $sk .'_form');
      }
    }
    $output .= theme('table', array('Section', 'Form callback'), $rows);
  }
  return $output;
}
