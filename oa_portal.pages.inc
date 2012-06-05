<?php

/**
 * Page callback for main status page
 */
function oa_portal_page() {
  global $user;
  $account = user_load($user->uid);

  if ($messages = oa_portal_get_messages($account->rmaxid)) {
    $output .= theme('oap_messages', $messages);
  }

  $output .= '<div id="oap-status-legend"><div class="oap-status-icon oap-icon-closed">You Have Completed!</div>';
  $output .= '<div class="oap-status-icon oap-icon-open">Requires Your Attention!</div>';
  $output .= '<div class="oap-status-icon oap-icon-under-review">Currently Under Review!</div></div>';

  $candidate = get_candidate($account->rmaxid);
  dsm($candidate, '$candidate object');

  $p_completed = 0;

  foreach ($candidate->p AS $pk => $phase) {
    $rows = array();
    $s_completed = 0;
    $s_ct = 0;
    
    // Check completion date for compliance
    if ($p[$pk]['#status'] == 'locked') {
      $p_completed++;
    }
    foreach (element_children($phase) AS $sk) {
      $section = $phase[$sk];
      $s_ct++;

     if ($candidate->p[$pk]['#status'] == 'locked') {
       $rows[] = theme('oap_phase_row', $section['#title'], 'phase-closed');
     }
     else {
        $status = $candidate->get_section_status($pk, $sk);
        if ($status == 'closed' || $status == 'locked') {
          $s_completed++;
        }
        $t = ($status == 'locked') ? '<span>'. $section['#title'] .'</span>' : l($section['#title'], 'portal/'. $pk .'/'. $sk);
        $rows[] = theme('oap_phase_row', $t, $status);
     }
    }

    $output .= theme('oap_phase', $pk, $phase['#title'], $phase['#status'], implode("\n", $rows), "$s_completed of $s_ct");
  }
  $output .= '<strong><em>('. $p_completed .' of 3 complete)</em></strong>';
  return $output;
}

/**
 * Page callback for PDF docs page
 */
function oa_portal_docs_page() {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);
  $docs = $candidate->get_all_docs();
//  dsm($docs, 'docs');
  $items = array();
  foreach ($docs AS $doc) {
    $items[] = oa_portal_doc_link($doc);
  }
  return theme('item_list', $items);
}

/**
 * Page callback for Forms page
 */
function oa_portal_forms_page() {
  global $user;
  $output = '<p>Our self-service forms library lets you download additional documents while on assignment. It also gives you quick access to many of the forms required for the healthcare industry.</p>';
  $output .= '<p>Note: You will need <a href="http://get.adobe.com/reader/">Acrobat Reader</a>.</p>';
  $items = array();
  $items[] = l('Tax Credit Form', 'todo');
  $items[] = l('Permanent Tax Home Notification', 'todo');
  $items[] = l('Timesheet', 'todo');
  foreach ($items AS $item) {
    $output .= '<li><span class="file-format pdf"></span>'. $item .'<span class="updated">Updated: 12/15/2012</span></li>';
  }
  return $output;
}

/**
 * Page callback for Tutorial FAQs page
 */
function oa_portal_tutorial_page() {
  global $user;
  jquery_ui_add(array('ui.core', 'ui.tabs'));
  drupal_add_js("Drupal.behaviors.tutorialTabs = function (context) { $('#tabs > ul').tabs(); }", 'inline');
  $output = '<p>If you are ready to begin working with On Assignment Healthcare Staffing, we\'re ready to start working with you. See our tutorial below on how our recruiting process works.</p>';
  $output .= '<p>If our tutorial doesn\'t answer your questions, see the frequently asked questions tab.</p>';
  $output .= '<div id="tabs"><ul><li><a href="#tutorial"><span>Tutorial</span></a></li><li><a href="#faq"><span>FAQs</span></a></li></ul>';
  $output .= '  <div id="tutorial"><img src="/sites/oahealth.resonancedev.ca/modules/oahealthportal/img/tutorial.png" /></div>';
  $output .= '  <div id="faq">FAQ content goes here,</div>';
  $output .= '</div>';
  return $output;
}

/**
 * Page callback for file downloading per dDocName
 */
function oa_portal_download_file($dDocName) {
  global $user;
  $account = user_load($user->uid);
  // To return this document, the dDocName must begin with their rmaxid
  if (strpos($dDocName, $account->rmaxid) !== 0) {
    drupal_access_denied();
    return;
  }
  $file = oa_portal_ucm_getfilebyname($dDocName);
  if (!$file->FileInfo->dFileSize) {
    drupal_not_found();
    return;
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
  if ($messages = oa_portal_get_messages($account->rmaxid, 'all')) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Selected Message(s)'),
    );
    foreach($messages as $message) {
      $unread = ($message->was_read) ? 'read' : 'unread';
      $checkboxes[$message->msg_id] = '';
      $form[$message->msg_id]['date'] = array(
        '#value' => '<div class="'. $unread .'">'. format_interval($time - $message->timestamp) .' ago</div>',
      );
      $form[$message->msg_id]['subject'] = array(
        '#value' => '<div class="'. $unread .'">'. l($message->subject, 'portal/messages/'. $message->msg_id) .'</div>',
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
  global $user;
  $account = user_load($user->uid);
  foreach ($form_state['values']['checkboxes'] AS $k => $v) {
    if ($k == $v) {
      oa_portal_message_delete($account->rmaxid, $v);
    }
  }
  drupal_set_message(t('Selected messages deleted.'));
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

/**
 * Mark a specific message as read for a user.
 *
 * @param integer $rmaxid
 *   The RMax ID of the user.
 * @param integer $msg_id
 *   The Message ID
 * @return
 *   An object containing the individual message
 */
function oa_portal_mark_message_read($msg_id, $js = FALSE) {
  global $user;
  $account = user_load($user->uid);
  db_query("UPDATE {ucm_messages} SET was_read = 1 WHERE rmaxid = %d AND msg_id = %d LIMIT 1", $account->rmaxid, $msg_id);

  if ($js) {
    drupal_json('200');
    exit();
  }
}
