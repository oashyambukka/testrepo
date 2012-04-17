<?php

/**
 * Page callback for main status page
 */
function oa_portal_page() {
  global $user;
  $account = user_load($user->uid);
  $profile = oa_portal_ucm_profileview($account->rmaxid);
//  dsm($profile, 'RMax Profile');

  if ($messages = oa_portal_get_messages($account->rmaxid)) {
    $output .= theme('oap_messages', $messages);
  }

  $output .= '<div id="oap-status-legend"><div class="oap-status-icon oap-icon-closed">You Have Completed!</div>';
  $output .= '<div class="oap-status-icon oap-icon-open">Requires Your Attention!</div>';
  $output .= '<div class="oap-status-icon oap-icon-under-review">Currently Under Review!</div></div>';

  $p = oa_portal_get_candidate_data($user->uid);
  foreach (element_children($p) AS $pk) {
    $phase = $p[$pk];
    $name = $phase['#title'];
    $rows = array();
    $completed = 0;
    $ct = 0;
    if ($phase['#status'] == 'completed') {
      $completed++;
    }

    foreach (element_children($phase) AS $sk) {
      $section = $phase[$sk];
      $ct++;
      $rows[] = theme('oap_phase_row', l($section['#title'], 'portal/'. $pk .'/'. $sk), $section['#status']);
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

  // Bad request
  // return drupal_json(array('status' => 400));

  // Unauthorized
  // return drupal_json(array('status' => 401));

  // Server error
  // return drupal_json(array('status' => 500));


  $path = $_POST['q'];
  $form_id = $_POST['form_id'];

  unset($_POST['q'], $_POST['op'], $_POST['form_build_id']);
  $serialized = serialize($_POST);

  // Currently, each user can have only one autosave form at a particular path.
  db_query("DELETE FROM {autosaved_forms} WHERE form_id = '%s' AND path = '%s' AND uid = %d", $form_id, $path, $user->uid);
  db_query("INSERT INTO {autosaved_forms} (form_id, path, uid, timestamp, serialized) VALUES ('%s', '%s', %d, %d, '%s')", $form_id, $path, $user->uid, time(), $serialized);
  watchdog('rpoa', 'Autosave captured from '. $_POST['q'] .'Post: <pre>'. print_r($_POST, TRUE) .'</pre>');
  exit();
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

/**
 * Page callback for messages page
 */
function oa_portal_messages_page($form_state) {
  global $user;
  $account = user_load($user->uid);
  $time = time();
  $form = array();
  $checkboxes = array();
  if ($messages = oa_portal_get_messages($rmaxid, 'all')) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Selected Message(s)'),
    );
    foreach($messages as $message) {
      $checkboxes[$message->msg_id] = '';
      $form[$message->msg_id]['date'] = array(
        '#value' => format_interval($time - $message->date),
      );
      $form[$message->msg_id]['subject'] = array(
        '#value' => $message->subject,
      );
    }

    $form['checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => $checkboxes, 
    );
    $form['#theme'] = 'oap_messages_page';
  }
  else {
    $form['empty'] = array(
      '#value' => t('There are no messages to display.'),
    );
  }
  return $form;
}

function oa_portal_messages_page_submit($form, &$form_state) {
  dsm($form_state['values'], '@TODO Delete selected messages from $form_state[values]');
}

/**
 * Page callback for individual messages page
 */
function oa_portal_message_page($msg_id) {
  global $user;
  $account = user_load($user->uid);
  if ($message = oa_portal_get_message($account->rmaxid, $msg_id)) {
    // Mark as read if it wasn't previously
    if (!$message->was_read) {
      oa_portal_mark_message_read($msg_id);
    }

    return theme('oap_message_page', format_interval(time() - $message->timestamp), $message->subject, $message->body);
  }
  else {
    drupal_not_found();
  }
}
