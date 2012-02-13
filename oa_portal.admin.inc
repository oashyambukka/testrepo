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
/*
       if (!function_exists('oa_portal_'. $pk .'_'. $sk .'_form')) {
         $code .= '/**
 * Form callback for '. $section['#title'] .'
 */
/*
function oa_portal_'. $pk .'_'. $sk .'_form($form_state) {
  $form = array();
  $form[\'#description\'] = t(\'\');
  
  // 
  $form[\'\'] = fieldset(\'\');
  $form[\'\'][\'\'] = textfield(\'\');
  $form[\'\'][\'\'] = textarea(\'\');
  $form[\'\'][\'\'] = array(
    \'#type\' => \'\',
    \'#title\' => t(\'\'),
    \'#options\' => array(\'opt1\' => \'Opt 1\'),
    \'#required\' => TRUE,
  );

  oa_portal_submit(&$form);
  return $form;
}
*/';
         $code .= '/**
 * Form validation callback for '. $section['#title'] .'
 */
/*
function oa_portal_'. $pk .'_'. $sk .'_form_validate($form, &$form_state) {
  if ($form_state[\'values\'][\'name\'] == \'\') {
    form_set_error(\'\', t(\'You must select a name for this group of settings.\'));
  }
}
*/

';

         $code .= '/**
 * Form submission callback for '. $section['#title'] .'
 */
/*
function oa_portal_'. $pk .'_'. $sk .'_form_submit($form, &$form_state) {

}
*/
';
        }
*/
      }
    }
    $output .= theme('table', array('Section', 'Form callback', 'form', 'validation', 'submission'), $rows);
  }

  $output .= '<pre>'. $code .'</pre>';
  return $output;
}
