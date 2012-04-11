<?php

/**
 * Page callback for main status page
 */
function oa_portal_page() {
  global $user;
  $account = user_load($user->uid);
  $profile = oa_portal_ucm_profileview($account->rmaxid);
//  dsm($profile, 'RMax Profile');

  $output .= '<div id="oap-status-legend"><div class="oap-status-icon oap-icon-closed">You Have Completed!</div>';
  $output .= '<div class="oap-status-icon oap-icon-open">Requires Your Attention!</div>';
  $output .= '<div class="oap-status-icon oap-icon-under-review">Currently Under Review!</div></div>';

  $p = oa_portal_get_candidate_data($user->uid);
  foreach ($p AS $pk => $phase) {
    $name = $phase['#title'];
    $rows = array();
    $completed = 0;
    $ct = 0;
    if ($phase['#status'] == 'completed') {
      $completed++;
    }

    foreach ($phase AS $sk => $section) {
      if (substr($sk, 0, 1) !== '#') {
        $ct++;
        $rows[] = theme('oap_phase_row', l($section['#title'], 'portal/'. $pk .'/'. $sk), $section['#status']);
      }
    }
    $output .= theme('oap_phase', $pk, $phase['#title'], $phase['#status'], implode("\n", $rows), "$completed of $ct");
  }
  return $output;
}

/**
 * Page callback for file downloading per dDocName
 */
function oa_portal_download_file($dDocName) {
  $file = oa_portal_ucm_getfilebyname($dDocName);
  if (!$file->FileInfo->dFileSize) {
    drupal_not_found();
  }
  $headers = array(
    'Content-Type: ' . $file->FileInfo->dFormat,
    'Content-Length: ' . $file->FileInfo->dFileSize,
    'Content-Disposition: attachment; filename="' . $file->FileInfo->dOriginalName .'"',
  );
  foreach ($headers as $header) {
    $header = preg_replace('/\r?\n(?!\t| )/', '', $header);
    drupal_set_header($header);
  }
  print $file->downloadFile->fileContent;
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

  return drupal_set_message(t('The form has been submitted. name="@first @last", year of birth=@year_of_birth', 
    array('@first' => $form_state['values']['first'], '@last' => $form_state['values']['last'], '@year_of_birth' => $form_state['values']['year_of_birth'])));
}

/**
 * Page callback for Fax Coverlsheets
 */
function oa_portal_ucm_createcoversheet($formtype) {
  global $user;
  $account = user_load($user->uid);
  $params = array(
    'rmaxid' => (string) $account->rmaxid,
    'formtype' => (string) $formtype,
  );
  $result = oa_portal_soap('createcoversheet', $params, TRUE);
  if ($result->createcoversheetResult->StatusInfo->statusCode === -1) {
    drupal_not_found();
    die();
  }

  $headers = array(
    'Content-Type: application/pdf',
    'Content-Length: '. strlen($result->createcoversheetResult->downloadedFile->fileContent),
    'Content-Disposition: attachment; filename="' . $result->createcoversheetResult->downloadedFile->fileName .'"',
  );
  foreach ($headers as $header) {
    $header = preg_replace('/\r?\n(?!\t| )/', '', $header);
    drupal_set_header($header);
  }
  print $result->createcoversheetResult->downloadedFile->fileContent;
  exit();
}
