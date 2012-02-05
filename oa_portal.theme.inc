<?php

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
