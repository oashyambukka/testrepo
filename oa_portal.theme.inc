<?php

/**
 * Preprocess function for Messages block
 */
function oa_portal_preprocess_oap_messages(&$vars) {
  global $user;
  $account = user_load($user->uid);
  $time = time();

  $vars['tip'] = theme('oap_tooltip', 'some-status', 'Tooltip message goes here. We may want to generalize tooltip markup generation.');
  $vars['count'] = count($vars['messages']);
  // array(
  //   array('date' => 'Today', 'teaser' => 'Please complete the first phase of your application by clicking on...'),
  //   array('date' => 'Yesterday', 'teaser' => 'Congratulations and Welcome to On Assignment Healthcare...')
  // )

  foreach ($vars['messages'] AS $message) {
    $vars['messages'] .= theme('oap_message_row', format_interval(time() - $message->timestamp), $message->subject, $message->body, l('More', 'messages/'. $message->msg_id));
  }
}

/**
 * Preprocess function for Messages block
 */
function theme_oap_messages_page(&$vars) {
  global $user;
  $account = user_load($user->uid);
  $time = time();

  $rows = array();
  foreach(element_children($form['checkboxes']) as $uid) {
    $rows[] = array(drupal_render($form['checkboxes'][$uid]), drupal_render($form[$uid]['first_name']), drupal_render($form[$uid]['last_name']));
  }

  if (!count($rows)) {
    // No messages found
  }
  
  $output = theme('table', array(), $rows);
  return $output . drupal_render($form);
}


/**
 * Preprocess function for Phases
 */
function oa_portal_preprocess_oap_phase(&$vars) {
  switch ($vars['status']) {
    default:
      $vars['status_tip'] = theme('oap_tooltip', 'some-status', 'Tooltip message goes here. We may want to generalize tooltip markup generation.');
  }
}

/**
 * Preprocess function for Phase rows
 */
function oa_portal_preprocess_oap_phase_row(&$vars) {
  switch ($vars['status']) {
    default:
      $vars['status_tip'] = theme('oap_tooltip', 'some-status', 'Tooltip message goes here. We may want to generalize tooltip markup generation.');
  }
}


function oa_portal_preprocess_oap_candidate_menu(&$vars) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);
 
  $vars['modality_name'] = $candidate->modality_name;

  $links = array();
  $links[] = array('title' => 'My Documents', 'href' => 'portal');

  $unread_count = $candidate->get_unread_message_count();
  $count = (count($unread_count)) ? ' ('. $unread_count .')': '';
  $links[] = array('title' => 'Message Center'. $count, 'href' => 'messages');
  $links[] = array('title' => 'Events Calendar', 'href' => 'todo');
  $links[] = array('title' => 'OTC', 'href' => 'todo');
  $links[] = array('title' => 'Forms', 'href' => 'forms');
  $links[] = array('title' => 'FAQs/Support', 'href' => 'faq');
  $links[] = array('title' => 'Log Out', 'href' => 'logout');

  $vars['links'] = theme('links', $links, array('class' => 'menu'));
}

/**
 * Preprocess function for Phase Status block
 */
function oa_portal_preprocess_oap_phase_status(&$vars) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);
  $phase = $candidate->p[arg(1)];
  
  $vars['id'] = arg(1);
  $vars['status'] = $phase['#status'];
  $vars['title'] = $phase['#title'];
  
  $items = array();
  foreach (element_children($phase) AS $sk) {
    $section = $phase[$sk];
    $items[] = theme('oap_phase_row', $section['#title'], $candidate->get_section_status(arg(1), $sk));
  }
  $vars['phases'] = implode("\n", $items);
}

/**
 * Preprocess function for Phase Status block
 */
function oa_portal_preprocess_oap_doc_download(&$vars) {
  $items = array();
  foreach ($vars['docs'] AS $id => $doc) {
    $items[] = l($doc->dDocName, 'portal/file/'. $doc->dDocName);
  }
  $vars['items'] = theme('item_list', $items);
}

function theme_pdf_textfield($element) {
  $class = array('form-text');
  $output = '';

  if (isset($element['#field_prefix'])) {
    $output .= '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ';
  }

  $output .= '<td id="' . $element['#id'] . '" ' . drupal_attributes($element['#attributes']) . '>'.  check_plain($element['#value']) ."</td>\n";

  if (isset($element['#field_suffix'])) {
    $output .= ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>';
  }

  return theme('pdf_form_element', $element, $output);
}

function theme_pdf_form_element($element, $value) {
  return $output; '<tr><td id="' . $element['#id'] . '">' . $element['#title'] .'</td>'. $value ."</tr>\n";
}
