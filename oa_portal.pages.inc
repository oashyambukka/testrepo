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

/**
 * Page callback for file downloading per dID
 */
function oa_portal_download_file($dID) {
  $result = oa_portal_soap('GetFileByID', array('dID' => $dID), TRUE);
  $headers = array(
    'Content-Type: ' . $result->GetFileByIDResult->FileInfo->dFormat,
    'Content-Length: ' . $result->GetFileByIDResult->FileInfo->dFileSize,
    'Content-Disposition: attachment; filename="' . $result->GetFileByIDResult->FileInfo->dOriginalName .'"',
  );
  foreach ($headers as $header) {
    $header = preg_replace('/\r?\n(?!\t| )/', '', $header);
    drupal_set_header($header);
  }
  print base64_decode($result->GetFileByIDResult->downloadFile->fileContent);
  exit();
}

/**
 * Page callback for forms autosaving
 */
function oa_portal_autosave() {
  global $user;
  dsm($_POST, 'autosave $_POST');
  // Bad request
  // return drupal_json(array('status' => 400));

  // Unauthorized
  // return drupal_json(array('status' => 401));

  // Server error
  // return drupal_json(array('status' => 500));

  return drupal_json(array('status' => 200));
}