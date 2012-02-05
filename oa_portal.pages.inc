<?php

/**
 * Page callback for main status page
 */
function oa_portal_page() {
  global $user;
  $account = user_load($user);
  $output .= '<div id="oap-status-legend"><div class="oap-status-icon oap-icon-completed">You Have Completed!</div>';
  $output .= '<div class="oap-status-icon oap-icon-completed">Requires Your Attention!</div></div>';

  $p = oa_portal_api();
  foreach ($p AS $pk => $phase) {
    $name = $phase['#title'];
    $rows = array();
    foreach ($phase AS $sk => $section) {
      if (substr($sk, 0, 1) !== '#') {
        $rows[] = theme('oap_phase_row', l($section['#title'], 'portal/'. $pk .'/'. $sk), 'some-status');
      }
    }
    $output .= theme('oap_phase', $pk, $phase['#title'], 'some-status', implode("\n", $rows));
  }
  $output .= '<div id="oap-complete">(1 of 3 complete)</div>';
  return $output;
}