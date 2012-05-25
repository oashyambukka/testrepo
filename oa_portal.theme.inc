<?php

/**
 * Preprocess function for Messages block
 */
function oa_portal_preprocess_oap_messages(&$vars) {
  global $user;
  $account = user_load($user->uid);
  $time = time();

  $vars['tip'] = theme('oap_tooltip', 'messages', 'Messages tooltip message goes here.');
  $vars['count'] = count($vars['messages']);
  // array(
  //   array('date' => 'Today', 'teaser' => 'Please complete the first phase of your application by clicking on...'),
  //   array('date' => 'Yesterday', 'teaser' => 'Congratulations and Welcome to On Assignment Healthcare...')
  // )

  foreach ($vars['messages'] AS $message) {
    $vars['messages_rendered'] .= theme('oap_message_row', format_interval(time() - $message->timestamp), $message->subject, $message->body, l('More', 'portal/messages/'. $message->msg_id, array('attributes' => array('msg_id' => $message->msg_id))));
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
  foreach(element_children($vars['checkboxes']) AS $mid) {
    $rows[] = array(drupal_render($vars['checkboxes'][$mid]), drupal_render($vars[$mid]['date']), drupal_render($vars[$mid]['subject']));
  }

  $output = drupal_render($vars['delete']);
  $output .= theme('table', array(), $rows);
  $output .= drupal_render($vars);
  return $output;
}


/**
 * Preprocess function for Phases
 */
function oa_portal_preprocess_oap_phase(&$vars) {
  $vars['status_tip'] = theme('oap_tooltip', 'phase-'. $vars['status']);
}

/**
 * Preprocess function for Phase rows
 */
function oa_portal_preprocess_oap_phase_row(&$vars) {
  $vars['status_tip'] = theme('oap_tooltip', 'section-'. $vars['status']);
}

function oa_portal_preprocess_oap_candidate_menu(&$vars) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);
 
  $vars['modality_name'] = substr($candidate->modality_name, strpos($candidate->modality_name, ' - ') + 2);

  $links = array();
  $links[] = array('title' => 'My OA Healthcare', 'href' => 'portal');
  $links[] = array('title' => 'My Documents', 'href' => 'portal/docs');

  $unread_count = $candidate->get_unread_message_count();
  $count = (count($unread_count)) ? ' ('. $unread_count .')': '';
  $links[] = array('title' => 'Message Center'. $count, 'href' => 'portal/messages');

  if ($candidate->modality_compliance[2]['approved_date']) {
    $links[] = array('title' => 'Events Calendar', 'href' => 'todo');
    $links[] = array('title' => 'OTC', 'href' => 'todo');
    $links[] = array('title' => 'Forms', 'href' => 'portal/forms');
  }

  $links[] = array('title' => 'FAQs/Support', 'href' => 'portal/tutorial');
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
 * Preprocess function for Tooltips
 */
function oa_portal_preprocess_oap_tooltip(&$vars) {
  switch ($vars['status']) {
    case 'phase-locked':
      $vars['body'] = 'Phase locked body';
    break;
    case 'section-closed':
      $vars['body'] = 'Section closed body';
    break;
    case 'section-under-review':
      $vars['body'] = 'Section under review body';
    break;
    case 'section-open':
      $vars['body'] = 'Section open body';
    break;
    case 'messages':
      $vars['body'] = 'Messages tooltip body';
    break;
    default:
      $vars['body'] = 'No body set!';
  }

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
