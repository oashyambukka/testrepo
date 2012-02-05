<?php

/**
 * Preprocess function for Messages block
 */
function oa_portal_preprocess_oap_messages(&$vars) {
  $vars['tip'] = 'Tooltip message goes here. We may want to generalize tooltip markup generation.';
  $vars['count'] = '2';
  $vars['messages'] = '';
  foreach (array(
      array('date' => 'Today', 'teaser' => 'Please complete the first phase of your application by clicking on...'),
      array('date' => 'Yesterday', 'teaser' => 'Congratulations and Welcome to On Assignment Healthcare...')
    ) AS $message) {
    $vars['messages'] .= theme('oap_message_row', $message['date'], $message['teaser'], l('More', 'messages/messageid'));
  }
}

/**
 * Preprocess function for Phases
 */
function oa_portal_preprocess_oap_phase(&$vars) {
  switch ($vars['status']) {
    default:
      $vars['status_tip'] = 'Tooltip message goes here. We may want to generalize tooltip markup generation.';
  }
}

/**
 * Preprocess function for Phase rows
 */
function oa_portal_preprocess_oap_phase_row(&$vars) {
  switch ($vars['status']) {
    default:
      $vars['status_tip'] = 'Tooltip message goes here. We may want to generalize tooltip markup generation.';
  }
}
