<?php

/**
 * Preprocess function for Messages block
 */
function oa_portal_preprocess_oap_messages(&$vars) {
  global $user;
  $account = user_load($user->uid);
  $time = time();
  $vars['tip'] = theme('oap_tooltip', 'messages', 'messagetip', 'Messages tooltip message goes here.');
  $path = drupal_get_path('module', 'oa_portal');
  $vars['tols'] = '<img class="oap-tooltip-icons" id="messagetip" src="'. $path .'/img/tooltiphelp_lrg.png"/>';

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
  //constructing the tooltip ID;
  $tipID = str_replace(' ', '_', $vars['title']);
  $vars['status_tip'] = theme('oap_tooltip', 'phase-'. $vars['status'], $tipID);
  $path = drupal_get_path('module', 'oa_portal');
  $vars['tip'] = ($vars['status']=='open') ? '<img class="oap-tooltip-icons" id="'. $tipID .'" src="'. $path .'/img/tooltiphelp_lrg.png"/>' : ' ';
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
  $candidate = get_candidate($account->rmaxid, $account->uid);
 
  $vars['modality_name'] = substr($candidate->modality_name, strpos($candidate->modality_name, ' - ') + 2);

  $links = array();
  $links[] = array('title' => 'My OA Healthcare', 'href' => 'portal');
  $links[] = array('title' => 'My Documents', 'href' => 'portal/docs');

  $unread_count = $candidate->get_unread_message_count();
  $count = (count($unread_count)) ? ' ('. $unread_count .')': '';
  $links[] = array('title' => 'Message Center'. $count, 'href' => 'portal/messages');

  if ($candidate->profile->status == 'Candidate') {
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
  $candidate = get_candidate($account->rmaxid, $account->uid);
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
    $items[] = oa_portal_doc_link($doc);
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

/**
 * Preprocess function for Dynamic HTML PDF generation
 */
function oa_portal_preprocess_oap_pdf_wrapper(&$vars) {
  $candidate = $vars['candidate'];
  $vars['name'] = $candidate->profile->name;
  $vars['rmaxid'] = $candidate->rmaxid;
}

/**
 * Preprocess function for Dynamic HTML PDF generation -- Checklists
 */
function oa_portal_preprocess_oap_pdf_wrapper_checklist(&$vars) {
  $candidate = $vars['candidate'];
  $vars['name'] = $candidate->profile->name;
  $vars['rmaxid'] = $candidate->rmaxid;
}

/**
 * Preprocess function for Dynamic HTML PDF generation
 */
function oa_portal_preprocess_oap_pdf_item_checkbox(&$vars) {
  $vars['element']['#value'] = ($vars['element']['#value']) ? 'Yes' : 'No';
}

function theme_oap_skill_radio($element) {
  _form_set_class($element, array('form-radio'));
  $output = '<input type="radio" ';
  $output .= 'id="' . $element['#id'] . '" ';
  $output .= 'name="' . $element['#name'] . '" ';
  $output .= 'value="' . $element['#return_value'] . '" ';
  $output .= (check_plain($element['#value']) == $element['#return_value']) ? ' checked="checked" ' : ' ';
  $output .= drupal_attributes($element['#attributes']) . ' />';
  unset($element['#title']);
  if (!is_null($element['#title'])) {
    $output = '<label class="option" for="' . $element['#id'] . '">' . $output . ' ' . $element['#title'] . '</label>';
  }
  return $output;
}

function theme_oap_skill_radios($element) {
  $keys = array_keys($element['#options']);
  $type = $element[$keys[0]]['#type'];
  $output .= '<div class="oap-skill-radios">';
  foreach ($keys as $key) {
    $output .= theme('oap_skill_radio', $element[$key]);
  }
  $output .= '</div>';
  return $output;
}