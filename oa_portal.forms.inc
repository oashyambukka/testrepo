<?php

include_once('oa_portal.fields.inc');

/**
 * Form callback for Password change form
 */
function oa_portal_change_password_form($form_state) {
  _user_password_dynamic_validation();
  $form = array();
  $form['pass'] = array(
    '#type' => 'password_confirm',
    '#prefix' => '<div class="description">For your account security, please change your password. This password requires a minimum of 7 characters, is case sensitive, and supports special characters.</div>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  return $form;
}

/**
 * Form submission callback for Password change form
 */
function oa_portal_change_password_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $account->pass = $form_state['values']['pass'];
  user_save($account, array('pass' => $form_state['values']['pass'], 'must_update_pass' => 0));
  drupal_set_message(t('Your password has been updated.'));
  $form_state['redirect'] = 'portal';
}

/**
 * Form callback for Application form
 */
function oa_portal_p1_application_form($form_state) {
  $form = array();

  $form['#description'] = '<p><br>If you are ready to begin working with On Assignment Healthcare Staffing, we\'re ready to start working with you.  Please complete the application and one of our experienced recruiters will contact you soon.</p><br>
  <p>Please be assured that On Assignment maintains a strict privacy policy.  We will not share your information with any third party without your authorization and will only contact you for the specific purpose of serving your request.</p><br><br>';
  
  // Contact Information
  $form['emergency_contact'] = fieldset('Contact Information');
  $form['emergency_contact']['emerg1_name'] = textfield('Name', 30, 50);
  $form['emergency_contact']['emerg1_name']['#prefix'] = t('<h1>Emergency Contact 1</h1>');
  $form['emergency_contact']['emerg1_phone'] = phone();
  $form['emergency_contact']['emerg2_name'] = textfield('Name', 30, 50);
  $form['emergency_contact']['emerg2_name']['#prefix'] = t('<h1>Emergency Contact 2</h1>');
  $form['emergency_contact']['emerg2_phone'] = phone();

  // Education
  $form['education'] = fieldset('Education', 'Please do not note the year high school diploma was received');

  $edu_fields['school'] = textfield('School', array('#options' => array('opt1' => 'Opt 1')));
  $edu_fields['rdo_grad_status'] = radios('Did you graduate', array(1 => 'Yes', 0 => 'No'));
  $edu_fields['degree'] = select('Degree Earned', array('highschool' => 'High school diploma', 'associates' => 'Associates'));
  unset($edu_fields['degree']['#required'], $edu_fields['degree']['#rules']);
  $edu_fields['city'] = textfield('City', 30);
  $edu_fields['state'] = state();
  unset($edu_fields['state']['#required'], $edu_fields['state']['#rules']);
  $edu_fields['year'] = textfield('Year', 10, 4);
  $edu_fields['major'] = textfield('Major', 30);
  $edu_fields['minor'] = textfield('Minor', 30);
  $edu_fields['coursework1'] = textfield('Relevant Course Work', 25);
  $edu_fields['coursework2'] = textfield('', 25);
  $edu_fields['coursework2']['#required'] = FALSE;
  $edu_fields['coursework3'] = textfield('', 25);
  $edu_fields['coursework3']['#required'] = FALSE;
  $edu_fields['coursework4'] = textfield('', 25);
  $edu_fields['coursework4']['#required'] = FALSE;
  $form['education']['edu'] = oa_portal_set($edu_fields, 'Add another school', 3);

  // Experience
  $form['experience'] = fieldset('Experience');

  $form['experience']['chk_recent_grad'] = checkbox('Check box if you are a recent college graduate');  

  $exp_fields['employer'] = textfield('Employer',75);
  $exp_fields['city'] = textfield('City');
  $exp_fields['state'] = state('State');
  unset($exp_fields['state']['#required'], $exp_fields['state']['#rules']);
  $exp_fields['empdatefrom'] = date_popup('Start Date', 'm-Y');
  $exp_fields['empdateto'] = date_popup('End Date', 'm-Y');
  $exp_fields['chk_present'] = checkbox('Present');
  $exp_fields['posheld'] = textfield('Title');
  $exp_fields['salary'] = textfield('Salary');
  $exp_fields['rdo_salary_period'] = radios('Salary', array('annual' => t('Annual'), 'hourly' => t('Hourly')));
  $exp_fields['description'] = textarea('Description', 5);
  $exp_fields['supname'] = textfield('Immediate Supervisor');
  $exp_fields['rdo_may_contact'] = radios_yesno('May we contact?');
  $exp_fields['reason'] = textfield('Reason for leaving');
  $form['experience']['exp'] = oa_portal_set($exp_fields, 'Add another job', 4);

  // Preferences
  $form['preferences'] = fieldset('Preferences');
  $form['preferences']['distance'] = textfield('How far are you willing to commute on a daily basis?', 3, array(
    '#field_suffix' => t('Miles'),
  ));

  $form['preferences']['chk_type'] = checkboxes('Check preferred employment type', array('Contract', 'Contract-to-hire', 'Direct Hire'), TRUE);
  $form['preferences']['chk_work_days'] = checkboxes('Check the days you are willing to work', array('Any', 'M', 'T', 'W', 'Th', 'F', 'S', 'Su'), TRUE);
  $form['preferences']['chk_shifts'] = checkboxes('Check the shifts you are willing to work', array('Any', 'Days', 'Nights', 'Evenings', 'Weekends'), TRUE);
  $form['preferences']['chk_ptft'] = checkboxes('Would you prefer to work', array('Either', 'Part-time', 'Full-time'));
  $form['preferences']['rdo_legal_right'] = radios_yesno('Can you, after employment, submit verification of your legal right to work in the U.S.?');
  $form['preferences']['rdo_felony'] = radios_yesno('Have you been convicted of a felony within the last seven years?');
  $form['preferences']['felony']['#suffix'] = t('(An affirmative response to this question will not necessarily disqualify you from employment)<br \>MA residents should not answer this question');
  $form['preferences']['rdo_drug_alcohol_screen'] = radios_yesno('As a condition of employment, you may be required to take and pass a drug and/or alcohol screen. Testing will be done at the company\'s expense and administered by a testing facility designated by the company. Results of any testing will be kept strictly confidential. If requested, are you willing to take the drug and/or alcohol screen?');
  $form['preferences']['referral'] = select('How did you hear about On Assignment Healthcare Staffing?',
    array('onassignment.com', 'other'));

  $form['preferences']['experience'] = textfield('Total years of experience');
  
  // Licensures & Certifications
  $form['licensures_certs'] = fieldset('Licensures/Certifications');

  $lc_fields['name'] = textfield('Licensure/Certification');
  $lc_fields['state'] = state();
  unset($lc_fields['state']['#required'], $lc_fields['state']['#rules']);
  $lc_fields['number'] = textfield('License Number', 12);
  $lc_fields['expiry_date'] = date_popup('Expires');
  $form['licensures_certs']['lc'] = oa_portal_set($lc_fields, 'Add another Licensure/Certification', 12);

  $form['organizations'] = fieldset('Professional Organizations');
  $form['organizations']['chk_none'] = checkbox('I am not a member of any organizations');
  $proforgs['org_name'] = textfield('Organization');
  $proforgs['org_role'] = textfield('Role');
  $proforgs['org_start'] = date_popup('From');
  $proforgs['org_end'] = date_popup('To');

  $form['organizations']['orgs'] = oa_portal_set($proforgs, 'Add another Organization', 3);
  
  $terms_desc = '<p>I certify that the information provided on this Application is accurate. I understand 
  that the withholding of information or the giving of false information on this Application may 
  result in a refusal to hire or disciplinary action including, but not limited to, termination. 
  I understand and agree that if I am offered employment by the company, it will be on an at-will basis. 
  This means that either the Company or I may terminate the employment relationship at any time, 
  for any reason, with or without cause or notice. I also understand and agree that only an officer 
  of the Company can enter into an agreement on any other terms and he/she can only do so in writing 
  signed by the officer and me. I have read the above before signing this Application.</p>
  <p>I further understand and waive my right of privacy in this investigation and release and hold 
  harmless On Assignment from any liability</p>
  <p>I agree that any decision to hire me is contingent upon the results of my report, and certify that all
  statements and answers on my Application, resume, or interview are true and complete to the best of my
  knowledge. I understand that if any statements are false or that if information has been omitted, this will
  be cause for disqualification and immediate termination of my employment. I further authorize 
  On Assignment to check my conviction record as needed, on a continuous basis as it relates to my employment.</p>
  <p>I authorize On Assignment to release any employment records, including health records submitted 
  to On Assignment to any customer of On Assignment for consideration of employment at customer facility.</p>';
  
  $form['terms'] = terms($terms_desc);
  return $form;
}

/**
 * Form submission callback for Application
 */
function oa_portal_p1_application_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);
  
  /**
   * @TODO Post all sections in 2 Application to UCM
   *
   */
  dsm($form_state['values'], 'Process & post 2 to UCM');
  $values = $form_state['values'];
  
  $body_string = drupal_render(oa_portal_html_to_pdf_change_type($values, $form));
  $html_string = theme('oap_pdf_wrapper', $candidate, 'Application', $body_string);
  $result = oa_portal_submitdrupalform($account->rmaxid, $formtype, $html_string);
  if ($result->StatusInfo->statusCode === 0) {
    $form_state['redirect'] = 'portal/p1/checklist';
  }
  else {
    drupal_set_message('An error occured. Please try again later.');
    $form_state['redirect'] = 'portal';
  }
}

/**
 * Form callback for Checklist
 */
function oa_portal_p1_checklist_form($form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);
  $form = array();
  $form['directions'] = markup('<p>Directions: Indicate your level of experience by selecting the numbers below as follows:<br>
    1 = No Experience<br>
    2 = Limited Experience<br>
    3 = Experienced<br>
    4 = Highly Skilled<br><br></p>');

  foreach ($candidate->p['p1']['checklist']['#formtypes_open'] AS $cft) {
    $fn = 'oa_portal_p1_checklist_form_'. $cft;
    if (function_exists($fn)) {
      $form[$cft] = $fn();
    }
    else {
      dsm('We are missing formtype #'. $cft);
    }
  }

  return $form;
}

/**
 * Form submission callback for Checklist
 */
function oa_portal_p1_checklist_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);

  // Process all open formtypes
  $error = FALSE;
  foreach ($form['#formtypes_open'] AS $formtype) {
    $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'][$formtype], $form[$formtype]));
    $html_string = theme('oap_pdf_wrapper_checklist', $candidate, $form[$formtype]['#title'], $body_string);
    dsm($html_string, 'PDF string');
    $result = oa_portal_submitdrupalform($account->rmaxid, $formtype, $html_string);
    if ($result->StatusInfo->statusCode !== 0) {
      $error = TRUE;
    }
  }
  if ($error) {
    drupal_set_message('An error occured. Please try again later.');
  }
  else {
    $form_state['redirect'] = 'portal';
  }
}

function oa_portal_p1_checklist_form_480() {
  $elements = fieldset('Acute Care Self Assessment Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_481() {
  $elements = fieldset('Adult Pediatric and Psychiatrics Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_482() {
  $elements = fieldset('Cardiac Cath Lab Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_483() {
  $elements = fieldset('Case Manager Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_484() {
  $elements = fieldset('Certified Nursing Assistant Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_485() {
  $elements = fieldset('Clerical Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_486() {
  $elements = fieldset('Critical Care Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_487() {
  $elements = fieldset('Cytology');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_488() {
  $elements = fieldset('Dental Assistant Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_489() {
  $elements = fieldset('Dental Hygenist Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

// 490 - Echo Tech Skills - likely out of scope

function oa_portal_p1_checklist_form_491() {
  $elements = fieldset('Endoscopy Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_492() {
  $elements = fieldset('ER Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_493() {
  $elements = fieldset('Essential/Practical Skills Checklist');
  $skills = array(
    'Core Skills' => array(
      'Admission of a Patient',
      'Transfer of a Patient',
      'Discharge of a Patient',
    ),
    'Cardiovascular' => array(
      'Doppler',
      'Lead placement',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_494() {
  $elements = fieldset('Hemodialysis Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_495() {
  $elements = fieldset('Histology Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_496() {
  $elements = fieldset('Histotech Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_497() {
  $elements = fieldset('Interventional Radiology Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_498() {
  $elements = fieldset('Labor & Delivery Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_499() {
  $elements = fieldset('Laboratory Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_500() {
  $elements = fieldset('LVN Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_501() {
  $elements = fieldset('Mammography/Ultrasound Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_502() {
  $elements = fieldset('Medical Assistant Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_503() {
  $elements = fieldset('Medical Technology Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_504() {
  $elements = fieldset('MedSurg Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_505() {
  $elements = fieldset('MedTech Assistant Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_506() {
  $elements = fieldset('NICU Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_507() {
  $elements = fieldset('Nurse Case Manager Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_508() {
  $elements = fieldset('Nurse Practicioner');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_509() {
  $elements = fieldset('Nursery Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_510() {
  $elements = fieldset('Occupational Therapy Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_511() {
  $elements = fieldset('Operating Room Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_512() {
  $elements = fieldset('Optical Skills Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_513() {
  $elements = fieldset('PACU Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_514() {
  $elements = fieldset('Pediatrics Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_515() {
  $elements = fieldset('Pharmacist Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_516() {
  $elements = fieldset('Pharmacy Tech Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_517() {
  $elements = fieldset('Phlebotomy Skills');

  $skills = array(
    'Procedures' => array(
      'phlebotomy' => 'Phlebotomy',
      'pediatrics' => 'Pediatrics',
      'geriatrics' => 'Geriatrics',
      'general' => 'General',
      'bloodculturecollection' => 'Blood Culture Collection',
      'bloodalcoholcollection' => 'Blood Alcohol Collection',
      'heelsticks' => 'Heel Sticks',
      'fingersticks' => 'Finger Sticks',
      'butterfly' => 'Butterfly',
      'tubetypesuses' => 'Knowledge of Tube Types and Uses (I.e. anticoagulants, tube order, etc.)',
      'centifugeoperation' => 'Centifuge Operation',
      'specimenprocessing' => 'Specimen Processing',
      'slidepreperation' => 'Slide/Manual Differentiation Preperation',
      'safetyguidelines' => 'Basic Safety Guidelines (gloves, googles, masks, when needed)',
      'infectioncontrol' => 'Infection Control Knowledge',
      'hazcom' => 'Haz Com Knowledge',
      ),
    'Data Preparation' => array(
      'dataentry' => 'Data Entry',
      'labeling' => 'Labeling',
      'phonework' => 'Phone Work (I.e. results reporting, stat requests, etc.)',
      'compexpdisc' => 'Computer Experience: DISC',
      'compexppace' => 'Computer Experience: PACE',
      'compexpversys' => 'Computer Experience: VERSYS',
      'compexpmedmgr' => 'Computer Experience: MED MGR',
      'compexpsycare' => 'Computer Experience: SYNCARE',
      'compexpother' => 'Computer Experience: Other',
      'emrsystem' => 'EMR Systems (Electronic Medical Records)',
    ),
    'Setting' => array(
      'cliniclab' => 'Clinic Lab',
      'hospitallab' => 'Hospital Lab',
      'mdofficelab' => 'MD Office Lab',
      'bloodbank' => 'Blood Bank',
      'mobileunit' => 'Mobile Unit',
      'homehealth' => 'Home Health',
      'other' => 'Other',
    )
  );

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_518() {
  $elements = fieldset('Physical Therapy Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_519() {
  $elements = fieldset('PICU Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_520() {
  $elements = fieldset('Post Anesthesia Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

// 521 - Psych Skills - likely out of scope

function oa_portal_p1_checklist_form_522() {
  $elements = fieldset('Radiology Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

// 523 - Rehab Skills - likely out of scope

function oa_portal_p1_checklist_form_524() {
  $elements = fieldset('Reprocessing Tech Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_525() {
  $elements = fieldset('Respitory Therapist Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_526() {
  $elements = fieldset('Respitory Tech Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_527() {
  $elements = fieldset('RN/LPN Clinical Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_528() {
  $elements = fieldset('Speech Therapy Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_529() {
  $elements = fieldset('Speech Therapy Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_530() {
  $elements = fieldset('Stepdown/PCU/Tele Skills');
  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_531() {
  $elements = fieldset('Sterile Processing Tech Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}


function oa_portal_p1_checklist_form_532() {
  $elements = fieldset('Surgical Processing Tech Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

// 533 - VaxAmerica Orientation Checklist - likely out of scope
// 540 - Cath Lab Skills - likely out of scope

function oa_portal_p1_checklist_form_547() {
  $elements = fieldset('Certified Occupational Therapy Assistant Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}


/**
 * Missing forms on UCM
 * - CT
 * - LPN
 * - Radiology/MRI
 * - Radiology CNMT
 * - PT Assistant
 * - Radiation
 * - Radiology Sonography
 * - L&D Skills
 */


/**
 * Form callback for Professional Organizations
 * Content is prepopulated from Application submission
 */
function oa_portal_p2_professional_orgs_form($form_state) {
  $form = array();

  $form['prof_orgs'] = fieldset('Professional Organizations');
  $form['prof_orgs']['chk_no_orgs'] = checkbox('I am not a member of any organizations');

  // @TODO Pre-populate with Professional Organizations from Application stage
  $proforgs = array('#tree' => TRUE);
  $proforgs['org_name'] = textfield('Organization');
  $proforgs['org_role'] = textfield('Role');
  $proforgs['org_start'] = date_popup('From');
  $proforgs['org_end'] = date_popup('To');

  $form['prof_orgs']['orgs1'] = $proforgs;
  $form['prof_orgs']['orgs2'] = $proforgs;

  return $form;
}

/**
 * Form callback for Licensure Certifications
 */
function oa_portal_p2_licensure_certs_form($form_state) {
  global $user;
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['lc'] = fieldset('Licensure/Certification');
  $form['lc']['#description'] = t('Upload a copy of your licensure/certification. .pdf, .jpg and .doc are acceptable formats.');

  $all_cl = oa_portal_get_category_formids('Certifications') + oa_portal_get_category_formids('Licensure');

  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);

  $options = array();
  foreach ($candidate->p['p2']['licensure_certs']['#formtypes_open'] AS $ft) {
    $options['Required'][$ft] = $all_cl[$ft];
    unset($all_cl[$ft]);
  }

  $options['Other'][61] = t('Other');
  $options['Other'] = $options['Other'] + $all_cl;

  $form['lc']['lc_licensure'] = select('Select which certificate you are uploading', $options, FALSE);
  $form['lc']['upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload file'),
//    '#required' => TRUE,
    '#size' => 80,
  );
  $form['lc']['uploaded'] = markup('<h3>Uploaded files</h3>');
  $form['lc']['chk_fax_preference'] = checkbox('Check this box if you prefer to fax copies of your licensures/certifications');

  $form['lc']['fax'] = markup('<div id="cover-letter-box" class="oap-doc-download set-hide">Please include this cover letter and fax it to (818) 555-5555. <a href="/portal/createcoversheet/" id="cover-letter-link">Cover Letter</a></div>');

  $form['#docs'] = $candidate->get_docs_not_missing($candidate->p['p2']['licensure_certs']['#formtypes']);
  if (is_array($form['#docs']) && count($form['#docs'])) {
    $form['docs'] = array(
      '#value' => theme('oap_doc_download', $form['#docs']),
      '#weight' => 1997,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Finalize'),
    '#weight' => 1998,
  );
  return $form;
}

/**
 * Form submission callback for Licensure Certifications
 */
function oa_portal_p2_licensure_certs_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  if (isset($_FILES['files']) && is_uploaded_file($_FILES['files']['tmp_name']['lc'])) {
    $filepath = $_FILES['files']['tmp_name']['lc'];
    $filesize = $_FILES['files']['size']['lc'];
    $filename = $_FILES['files']['name']['lc'];
    $handle = fopen($filepath, "r");
    $contents = fread($handle, $filesize);
    fclose($handle);


// RS: Currently, there is an "Other (Misc) Certification" formtype with the id #61.
// I still have to fully document the approach, but the general idea for handling "other form types" is to set their xFormType, 
// but do NOT set their dDocName. This will allow for a dynamically created dDocName and, at the same time, not restrict the number 
// of items can be set to a single xFormType. The downside to this is that it will not be derivable what the dDocName (i.e., unique identifier) 
// is for any "other" document.

    $params = array(
      'dDocName' => ($form_state['values']['lc']['lc_licensure'] == 61) ? '' : $account->rmaxid .'_'. $form_state['values']['lc']['lc_licensure'],
      'dDocTitle' => $form_state['values']['lc']['lc_licensure'] .' - '. $account->rmaxid,
      'dDocType' => 'Rmax',
      'dDocAuthor' => 'weblogic',
      'dSecurityGroup' => 'Form',
      'dDocAccount' => 'Recruit/QA',
      'xContentType' => 'Form',
      'xFormType' => $form_state['values']['lc']['lc_licensure'],
      'xRM_ID' => $account->rmaxid,
      'xStatus' => 'Pending Review',
      'xComments' => '',
      'xRejectReason' => '',
      'xRejectOther' => '',
      'xSuccessAudit' => '',
    );
    $params['primaryFile'] = array(
      'fileName'=> $_FILES['files']['name']['lc'],
      'fileContent' => $contents,
    );
    $result = oa_portal_soap('oacheckin', $params);
    if ($result->oacheckinResult->StatusInfo->statusCode === 0) {
      drupal_set_message(t('Your file upload was successful.'));
    }
    else {
      drupal_set_message(t('An error occured. Please try again.'));
    }
  }

  $candidate = get_candidate($account->rmaxid, TRUE);
  if (count($candidate->p['p2']['licensure_certs']['#formtypes_open']) == 0 || $form_state['values']['lc']['chk_fax_preference'] == 1) {
    $form_state['redirect'] = 'portal/p2/background_reference';
  }
  else {
    $form_state['redirect'] = 'portal/p2/licensure_certs';
  }
}

/**
 * Form callback for Background/Reference Check
 */
function oa_portal_p2_background_reference_form($form_state) {
  $form = array();
  $form[233] = fieldset('Background Investigation Consent');
  $form[233]['#description'] = '<p>I hereby authorize On Assignment, and/or its agents to make an independent investigation of my background, references, character, past employment, education, credit history, motor vehicle operation history, criminal or police records, including those maintained by both public and private organizations and all public records for the purpose of confirming the information contained on my Application and/or obtaining other information which may be material to my qualifications for employment.  I release On Assignment and/or its agents and any person or entity, which provides information pursuant to this authorization, from any and all liabilities, claims or law suits in regards to the information obtained from any and all above referenced sources used.  I understand that all or part of this information, including my social security number may be released to clients as part of the hiring process, and agree to the release of any part or all of this information including my social security number.</p><br>
<p>This is a consumer notification that a Background Report will be requested and obtained, and that the report will be used for the purpose of evaluating me for employment, promotion, reassignment or retention as an employee.</p><br>
<p>As a resident of Minnesota and Oklahoma only, I have a right to obtain a copy of this report by checking this box.</p><br>
<p>NOTICE TO CALIFORNIA CANDIDATES</p><p><span style="font-size:90%">You have a right to obtain a copy of any consumer report or investigative consumer report obtained by On Assignment, Inc. by checking the box provided below. The report will be provided to you within three (3) business days after we receive the requested reports related to the matter investigated.  I request to receive a free copy of this report by checking this box.  Under section 1786.22 of the California Civil Code, you may view the file maintained on you by GIS (the search company) during normal business hours.  You may also obtain a copy of this file upon submitting proper identification and paying the costs of duplication services, by appearing at GIS in person or by mail.  You may also receive a summary of the file by telephone.  The agency is required to have personnel available to explain your file to you and the agency must explain to you any coded information appearing in your file.  If you appear in person, a person of your choice may accompany you, provided that this person furnishes proper identification.</span></p><br><br>
<p><strong>The following is my true and complete legal name and all information is true and correct to the best of my knowledge:</strong></p>';

  $form[233]['receivereport'] = checkbox('I request to receive a free copy of this report by checking this box. Under section 1786.22 of the California Civil Code, you may view the file maintained on you by GIS (the search company) during normal business hours. You may also obtain a copy of this file upon submitting proper identification and paying the costs of duplication services, by appearing at GIS in person or by mail. You may also receive a summary of the file by telephone. The agency is required to have personnel available to explain your file to you and the agency must explain to you any coded information appearing in your file. If you appear in person, a person of your choice may accompany you, provided that this person furnishes proper identification.');
  $form[233]['receivereport']['#prefix'] = 'NOTICE TO CALIFORNIA CANDIDATES<br /><p>You have a right to obtain a copy of any consumer report or investigative consumer report obtained by On Assignment, Inc by checking the box provided below. The report will be provided to you within three (3) business days after we receive the requested reports related to the matter investigated.</p>';

  $form[233]['maiden_name'] = textfield('Maiden/other names used');
  $form[233]['dob'] = date_popup('Date of birth');
  $form[233]['ssn'] = textfield_ssn();
  $form[233]['dlnumber'] = textfield('Driver License Number', 20);
  $form[233]['stateoflicense'] = state('Driver License State');
  $form[233]['frompresentaddr'] = date_popup('Lived at your current address since', 'm-Y');
  $form[233]['frompresentaddr']['#suffix'] = '<strong>Note: We need 7 years of address history. Please add all addresses for the past 7 years.</strong>';
  $form[233]['topresentaddr'] = array(
    '#type' => 'value',
    '#value' => 'Present',
  );
  $fmr = array();
  $fmr['fromformeraddr'] = date_popup('From', 'm-Y');
  $fmr['toformeraddr'] = date_popup('To', 'm-Y');
  $fmr = $fmr + address();
  unset($fmr['address2']);
  $form[233]['former_addresses'] = oa_portal_set($fmr, 'Add another address', 7);
  $form[233]['chk_certification'] = checkbox('I certify...');

  $ref = array('#tree' => TRUE);
  $ref['employer'] = select('Company', array('Company A', 'Company B'));
  $ref['supname'] = textfield('Supervisor');
  $ref['title'] = textfield('Supervisor\'s Title');
  $ref['mailingaddress'] = textfield('Mailing Address', 30);
  $ref['zip'] = textfield('Zip Code', 10);
  $ref['phone'] = phone();
  $ref['email'] = email();
  $ref['chk_certification'] = checkbox('I hereby give permission to the above named employer to release information to On Assignment regarding my performance while employed at that facility');

  $form[476] = fieldset('Reference Check 1');
  $form[476]['ref'] = $ref;
  $form[477] = fieldset('Reference Check 2');
  $form[477]['ref'] = $ref;
 
  return $form;
}

/**
 * Form submission callback for Background/Reference Check
 */
function oa_portal_p2_background_reference_form_submit($form, &$form_state) {
  /**
   * @TODO Post 233 Background Consent to UCM
   *
   * name - full name
   * receivereport
   * fullname
   * maidenname
   * dob
   * ssn
   * dlnumber
   * stateoflicense
   * presentaddress
   * frompresentaddr
   * topresentaddr
   * citypresentaddr
   * zippresentaddr
   *
   * Former Addresses (N: 1-7):
   * formeraddressN
   * fromformeraddrN
   * toformeraddrN
   * cityformeraddrN
   * zipformeraddrN
   */
  dsm($form_state['values'][233], 'Process & post 233 to UCM');

  /**
   * @TODO Post 476 & 477 Professional Reference 1/2 to UCM
   *
   * name - fullname
   * applicantname - fullname
   * employer - From Application->Experience
   *
   * NOTE: All contact fields relate to the Company/Supervisor, not the applicant
   * mailingaddress
   * supname
   * posheld - From Application->Experience
   * empdatefrom
   * empdateto
   * city - From Application->Experience
   * state - From Application->Experience
   * zip
   * title
   * phone
   * email
   * datesigned - Date signed
   */
  dsm($form_state['values'][476], 'Process & post 476 to UCM');
  dsm($form_state['values'][477], 'Process & post 477 to UCM');
  
  // @TODO Store Maiden Name for future use
  
  
  $error = FALSE;
  foreach ($form['#formtypes'] AS $formtype) {
    if (!$success = oa_portal_ucm_createpdfform($account->rmaxid, $formtype, $form_state['values'][$formtype])) {
      $error = TRUE;
    }
  }  
  if ($error) {
    drupal_set_message('An error occured. Please try again.');
  }
  else {
    $form_state['redirect'] = 'portal/p2/capabilities';
  }
}

/**
 * Form callback for Capabilities Questionnaire
 */
function oa_portal_p2_capabilities_form($form_state) {
  $form = array();
  
  $form[5] = fieldset('Job Capabilities Questionnaire');
  $form[5]['#description'] = t('As a candidate for employment at On Assignment1, you have met the basic requirements of skills, experience, education and availability needed by our Clients.  Clients request that our employees work at their location for a specific period of time to complete various work duties.  As an On Assignment employee, you will typically be assigned to positions in healthcare, research and development and/or many different types of facilities.  These positions may involve working with various potentially hazardous substances and physical agents, including, but not limited to, the following:<br><br>
<ul><li>Various organic volatile and non-volatile chemicals, corrosives, potential carcinogens, mutagens and teratogens</li>
<li>Biological agents, bloodborne pathogens and radioactive sources and materials</li>
<li>Heat, cold, pressure (including compressed gasses), and noise</li>
<li>Working with glassware, pipetting, repetitive tasks</li>
</ul><br>These assignments may also include physical requirements that involve sitting/standing/walking for extended periods, bending and stooping, lifting or moving heavy objects and lifting up to 50 lbs. above the shoulders.  They may also involve repetitive tasks such as using a keyboard, operating equipment or manipulating small objects.  A safe working environment is important not only to you, but also to On Assignment and our Clients.  You are expected to take all reasonable safety precautions, and to follow all On Assignment and client safety policies and procedures.  This may include, but is not limited to Standard Operating Procedures, appropriate use of personal protective equipment and devices, and personal hygiene.  To assist us in assuring that we select the best possible assignment for you, please complete the following Job Capabilities Questionnaire.<br><br>');
  $form[5]['performance'] = cap_yne('Can you perform all of these and similar tasks with reasonable accommodation?');
  $form[5]['health'] = cap_yne('Do you have any health reasons that would prohibit you from performing these or similar tasks?');
  $form[5]['restrictions'] = cap_yne('Do you have any restrictions preventing you from performing these tasks?');
  $form[5]['allergies'] = cap_yne('Do you have any known allergies?', 'If yes, list');
  $form[5]['allergies']['rdo_dermatitis'] = radios_yesno('Do you have dermatitis?');
  $form[5]['allergies']['rdo_latex'] = radios_yesno('Are you allergic to latex gloves?');
  $form[5]['preference'] = cap_yne('Are there any assignments that you cannot or prefer not to do?');
  $form[5]['chk_certify'] = checkbox('I certify this information is correct, and understand that withholding or giving false information will result in refusal to hire or disciplinary action up to, and including, termination.');
  return $form;
}

/**
 * Form submission callback for Capabilities Questionnaire
 */
function oa_portal_p2_capabilities_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  /**
   * @TODO Post 5 Job Capabilities Questionnaire to UCM
   * @TODO needs PDF fields
   */
  dsm($form_state['values'][5], 'Process & post 5 to UCM');
  if ($success = oa_portal_ucm_createpdfform($account->rmaxid, 5, $form_state['values'][5])) {
    $form_state['redirect'] = 'portal/p2/policies';
  }  
  else {
    drupal_set_message('An error occured. Please try again.');
  }  
}

/**
 * Form callback for Policies & Agreements
 */
function oa_portal_p2_policies_form($form_state) {
  $form = array();
  $form[256] = fieldset('Substance Abuse Testing Consent');
  $form[256]['testing'] = acknowledgement('I understand that I may be offered a position with On Assignment...');
  $form[256]['sample'] = acknowledgement('I agree to provide an appropriate sample...');

  $form[433] = fieldset('Employment Agreement');
  $form[433]['personelle'] = acknowledgement('On Assignment Staffing Services, Inc. (“On Assignment”) is engaged in the business of providing life sciences, healthcare and laboratory support services to other parties (“Client(s)”), who have requested contract professionals to fill their staffing needs. ______________________________ (“Personnel”) wishes to enter into this Employment Agreement with On Assignment.
  On Assignment and Personnel agree as follows:
  1. Scope of Employment Relationship.
  a) Assignment. Personnel is employed by On Assignment for a specific assignment with Client (“Assignment”).
  b) Supervision. Client will direct and supervise Personnel while assigned to Client.
  c) Policies and Procedures. Personnel agrees to abide by the policies and procedures of Client including, but not limited to: safety,
  confidentiality and HIPAA (Health Insurance Portability and Accountability Act of 1996). Personnel will notify his/her On Assignment
  representative if there is a conflict with On Assignment policies.
  d) Best Efforts. Personnel agrees to use his/her best efforts in performing services to Client under this Agreement. Personnel understands that
  inappropriate behavior can damage On Assignment’s relationship with the Client. Suspicious, illegal, or unauthorized behavior that comes to the attention of On Assignment may be reported to the appropriate law enforcement or regulatory agencies. Such behavior and poor performance may be cause for termination of the Assignment with Client or termination of employment with On Assignment.
  e) No interference. Personnel acknowledges that there is no contract or condition that would prohibit or interfere with Personnel’s ability to work for On Assignment in the capacity as intended.
  f) Legal Obligation. Personnel is not authorized to incur any legal obligation on behalf of On Assignment, execute any agreement, exercise any authority or otherwise sign any document on behalf of On Assignment.
  g) Work-related issues. Personnel shall immediately inform On Assignment of all work-related issues that arise.
  h) Termination of Assignment. Personnel shall provide On Assignment with timely notice of any intended employment changes, such as
  termination (please note that two weeks notice prior to termination is a professional courtesy).
  i) Request for Time off. Personnel must request time off for days off or appointments at least 24 hours in advance; all requests must be directed
  to On Assignment. In cases of illness or emergency, Personnel must inform On Assignment that he/ she will not be able to attend the scheduled
  Assignment at least thirty minutes prior to commencement time.
  j) Personal items. On Assignment does not assume liability for Personnel’s personal items at the Client site and/or while on Assignment.
  k) Driving. Personnel is prohibited from driving any vehicle (including, but not limited to personal car, rental car, or Client-owned car) for work-
  related duties without express written authorization from On Assignment. “Work-related duties” include responsibilities directed by Client for which Personnel will be compensated.
  2. Background Check. Personnel’s employment with On Assignment and placement with Client conditioned upon satisfactory results of a criminal background check, drug test, credit history, driving record, medical history, any other tests that may be appropriate for the Assignment or otherwise required by Client. Personnel authorizes On Assignment to perform tests and checks and provide relevant results to Client, as required.
  3. Current Certifications. Personnel must possess and provide On Assignment with copies of current licenses, certifications, registrations (as appropriate to the state in which the Assignment is located), and regulatory compliance documents, as appropriate. The documents must be provided prior to the commencement of the Assignment; failure to provide or maintain these documents may result in the cancellation of the Assignment. Personnel may be liable to On Assignment for consequential damages resulting from falsifying, failing to provide or failing to maintain these documents.
  4. Termination. As the employment relationship between Personnel and On Assignment is an employment at will, any party may terminate the employment relationship or an Assignment at any time, for any reason, with or without notice. On Assignment has no responsibility to pay any further salary or expenses if the term of an Assignment is cut short by any party, for any reason.
  5. Payment. On Assignment shall pay Personnel the mutually agreed upon rate for hours worked, unless otherwise agreed. Personnel will be paid weekly, only for actual hours worked as accurately documented in a Client-approved timesheet, submitted to On Assignment each week in a timely manner. On Assignment will not reimburse any expense incurred by Personnel unless the expense has been approved in advance and in writing by On Assignment.
  6. Indemnification. Personnel understands that On Assignment may have immediate expenses associated with placing Personnel at an Assignment including, but not limited to, purchases of glasses and safety shoes or costs of medical exams. If Personnel voluntarily leaves an Assignment prior to the original planned completion date or if Personnel is terminated from the Assignment for cause, Personnel understands that he/she may be required to reimburse On Assignment for these reasonable expenses. On Assignment will not be liable to Personnel for any damages, losses, expenses, inconveniences, or loss of alternative employment as a result of Client’s actions or Client-initiated changes to the Assignment.
  7. Confidential Information. Personnel acknowledges and agrees that in performing obligations under this Agreement he/she may have access to valuable trade secrets and confidential information of Client or On Assignment including, but not limited to, trade secrets, processes, formulae, inventions, techniques, marketing plans, strategies, forecasts, computer programs, patentable or copyrightable material, patient information, and customer lists (“Confidential Information”). This information will include, without limitation, any commercially valuable information created by, discovered or developed by, or made known to Personnel during the period of employment. Personnel agrees that he/she shall preserve and protect the confidentiality of the Confidential Information and that any confidential material shall remain the sole property of On Assignment or Client and shall be returned immediately upon request or upon termination of employment. All documents, records, apparatus, equipment and other physical property, or reproductions thereof, furnished to Personnel by On Assignment or its Clients or produced in connection with the performance of Assignments, whether or not pertaining to Confidential Information, shall be and remain the sole property of On Assignment or its Clients and shall be returned immediately upon request or upon termination of Personnel’s employment for any reason. All Confidential Information, inventions, patents or copyrights, and other rights in connection therewith, developed during the course of an Assignment shall be the sole property of On Assignment or its Clients and their respective assigns. The obligations set forth in this section shall survive the termination of this Agreement.
  8. Non-Competition Agreement. Personnel agrees that he/she will not accept any offer of employment (either as an employee, as an independent contractor, or through another staffing agency) with any Client for whom Personnel performed services while employed by On Assignment without On Assignment’s express written consent during the Assignment and for a period of 180 days after the termination of the Assignment.');

  $form[433]['personnelinitials'] = initial('Addendum for California personnel
    Section 2870 of the California Labor Code states: 
    A. Any provision in an employment agreement which provides that an employee shall assign, or offer to assign, any of his or her rights in an invention to his or her employer shall not apply to an invention that the employee developed entirely on his or her own time without using the employer’s equipment, supplies, facilities, or trade secret information except for those inventions that either:
    1.) Relate at the time of conception or reduction to practice of the invention to the employer’s business, or actual or demonstrably anticipated research or development of the employer; or
    2.) Result from any work performed by the employee for the employer.
    B. To the extent a provision in an employment agreement purports to require an employee to assign an invention otherwise excluded from being required to be assigned under subdivision (a), then provision is against the public policy of this state and is unenforceable.
    Accordingly, On Assignment does not require Personnel to assign any invention prohibited by Section 2870 of the California Labor Code.');


  $form[432] = fieldset('Direct Hire Candidate Agreement');
  $form[432]['relationship'] = acknowledgement('On Assignment Staffing Services...');
  $form['questions'] = markup('If you have any questions..');

  return $form;
}

/**
 * Form submission callback for Policies & Agreements
 */
function oa_portal_p2_policies_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);

  /**
   * @TODO Post 433 Employment Agreement to UCM
   *
   * personnelname - From Profile
   * signature
   * personneladdress - From Profile
   * personnelphone - From Profile
   * timestamp - current date
   * personnelinitials
   */
  // PDF Field => RMax Profile Field
  
  foreach (array(
    'personnelname' => 'name',
    'personneladdress1' => 'addressline1',
    'personneladdress2' => 'addressline2',
    'personneladdress3' => 'addressline3',
    ) AS $pdff => $rmaxf) {
    $form_state['values'][433][$pdff] = $candidate->profile->{$rmaxf};
  }
  
  // Others
  unset($form_state['values'][433]['personelle']);
  $form_state['values'][433]['timestamp'] = date('Y-m-d');

  $pdf_html_string = oa_portal_html_to_pdf_string($account->rmaxid, 433, $form_state['values'][433], $form);

  /**
   * @TODO Post 256 Substance Abuse Testing Consent to UCM
   * @TODO needs PDF fields
   * 
   */
  dsm($form_state['values'][256], 'Process & post 256 to UCM');

  /**
   * @TODO Post 432 Direct Hire Agreement to UCM
   * @TODO needs PDF fields
   * 
   */
  dsm($form_state['values'][432], 'Process & post 432 to UCM');

  $error = FALSE;
  foreach ($form['#formtypes_open'] AS $formtype) {
    if ($success = oa_portal_ucm_createpdfform($account->rmaxid, $formtype, $form_state['values'][$formtype])) {
      dsm($success, $formtype .' success');
    }
    else {
      $error = TRUE;
    }
  }  
  if ($error) {
    drupal_set_message('An error occured. Please try again.');
  }
  else {
    $form_state['redirect'] = 'portal/p2/medical';
  }
}

/**
 * Form callback for Medical Authorization and Disclosures
 */
function oa_portal_p2_medical_form($form_state) {
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form[263] = fieldset('Authorization For Use Or Disclosure Of Health Information');
  $form[263]['disclosure'] = acknowledgement('I authorize the use or disclosure of my health information...');
  $form[263]['providers'] = textfield('As described in section 1, list specific providers of information if applicable', 80);
  $form[263]['info'] = textfield('As described in section 3, list specific information that may be used or disclosed if applicable', 80);
  $form[263]['dates'] = textfield('As described in section 7, list date or event per 3) if applicable', 80);
  
  // @TODO Add help tooltip
  $form['tb'] = fieldset('TB');
  
  // @TODO Which PDF for TB Stage 1&2?
  $form['tb']['tb_option'] = radios('', array(
    291 => 'TB Vaccine Stage 1 & Stage 2',
    288 => 'Negative Chest X-ray (every 3 years)',
    ));
  $form['tb']['tb_file'] = medical_file('Certificate');
  $form['tb']['tb_fax'] = fax_preference_checkbox();

  // @TODO Add help tooltip
  // MMR: 261 & 262
  // Titer: 284 & 285
  $form['mmr'] = fieldset('MMR');
  $form['mmr']['mmr_option'] = radios('', array(
    'mmr' => 'MMR #1 / #2 if born after 1957',
    'rubella_titer' => 'Rubela Titer / Rubeola Titer / Mumps Titer',
    ));

  $form['mmr']['mmr_file_261'] = medical_file('MMR #1 Certificate');
  $form['mmr']['mmr_file_262'] = medical_file('MMR #2 Certificate');
  $form['mmr']['mmr_file_284'] = medical_file('Rubella Titer Certificate');
  $form['mmr']['mmr_file_285'] = medical_file('Rubeola Titer Certificate');
  $form['mmr']['mmr_fax'] = fax_preference_checkbox();

  // @TODO Which PDF(s) for Cardiac Life Support?
  // CLS Option #1: BLS - 19
  // CLS Option #2: CPR - 37
  // CLS Option #3: ACLS - 13
  $form['cls'] = fieldset('Cardiac Life Support');
  $form['cls']['#tree'] = TRUE;
  $form['cls']['cls_option'] = radios('', array(
    19 => 'BLS',
    37 => 'CPR',
    13 => 'ACLS'));

  // @TODO Hide this field until cls_option is specified
  $form['cls']['cls_file'] = medical_file('Certificate');
  $form['cls']['cls_fax'] = fax_preference_checkbox();
  
  // @TODO Which PDF(s) for Varivax/Varicella?
  // Varivax/Varicella PDFs: 295, 296, 297, 298, 299
  // #1: Varivax Immunization #1 - 298
  // #2: Varicella Immunity Stmt / Hist of Disease - 296
  // #3: Varicella Titer - 295
  // 297 ?
  // 299 ?

  $form['varivax'] = fieldset('Varivax/Varicella');
  $form['varivax']['varivax_choice'] = radios('', array(
    298 => 'Varivax Vaccine #1',
    296 => 'Varicella Immunity Stmt/Hist. of Disease',
    295 => 'Varicella Titer'));

  // @TODO Show when varivax_option is specified
  $form['varivax']['varivax_file'] = medical_file('Certificate');
  $form['varivax']['tb_fax'] = fax_preference_checkbox();

  // @TODO Which PDF(s) for Hepatitis B?
  // #1: Hep B Series 1, 2 & 3 - 270, 271, 272
  // #2: Hep B Antigen Titer - 274
  // #3: Declination - 275

  $form['hepb'] = fieldset('Hepatitis B Vaccination');
  $form['hepb']['#tree'] = TRUE;
  $form['hepb']['hepb_option'] = radios('', array(
    'hep123' => 'Hep B Series 1, 2 & 3',
    274 => 'Hep B Titer',
    275 => 'Hep B Declination'));

  // @TODO Show when hepb_option = hep123
  $form['hepb']['hepb_file_1'] = medical_file('Hep B Series 1 Certificate');
  // @TODO Show when hepb_option = hep123
  $form['hepb']['hepb_file_2'] = medical_file('Hep B Series 2 Certificate');
  // @TODO Show when hepb_option = hep123
  $form['hepb']['hepb_file_3'] = medical_file('Hep B Series 3 Certificate');
  // @TODO Show when hepb_option = 274
  $form['hepb']['hepb_file_274'] = medical_file('Hep B Titer Certificate');
  $form['hepb']['hepb_fax'] = fax_preference_checkbox();
  
  // @TODO Hide this field unless cls_option is specified
  $form['hepb']['declination'] = acknowledgement(
    'Hepatitis B Vaccination Declination - Please complete Bloodborne Training before...',
    'I acknowledge, understand, and accept this Agreement/Statement. *This is a reg..'
  );
  $form['hepb']['declination']['#prefix'] = '<div class="set-hide">' . $form['hepb']['declination']['#prefix'];
  $form['hepb']['declination']['#suffix'] = $form['hepb']['declination']['#suffix'] .'</div>';
  return $form;
}

/**
 * Form submission callback for Medical Authorization and Disclosures
 */
function oa_portal_p2_medical_form_submit($form, &$form_state) {
  // @TODO Upload ['mmr']['mmr_file'] to 261 if ['mmr']['mmr_option'] == 'mmr'
  // @TODO Upload ['mmr']['mmr_file'] to 284 if ['mmr']['mmr_option'] == 'rubella_titer'
  /**
   * @TODO Post 263 Health Disclosure Agreement to UCM
   * @TODO needs PDF fields
   * 
   */
  dsm($form_state['values'][263], 'Process & post 263 to UCM');

  /**
   * @TODO Post Everything else to UCM
   * @TODO needs PDF(s) and fields
   * 
   */
  dsm($form_state['values'], 'Process & post everything else to UCM');

}

/**
 * Form callback for Competency Test
 */
function oa_portal_p2_competency_form($form_state) {
  $form = array();
  $form['#description'] = t('You are required to complete....');

  // @TODO Get a test link from ___?
  $form['take_the_test'] = markup('TAKE THE TEST');
  return $form;
}

/**
 * Form callback for Paycheck Method
 */
function oa_portal_p3_paycheck_form($form_state) {
  $form = array();

  // @TODO Add JS: expose either Direct Deposit or CashPay Enrollment conditionally based on rdo_delivery_option status
  $form['delivery'] = fieldset('Paycheck Delivery');
  $form['delivery']['#collapsible'] = FALSE;
  $form['delivery']['rdo_delivery_option'] = radios('', array(
    'directdeposit' => '<strong>I do</strong> wish to sign up for Direct Deposit at this time and will complete the section below.',
    'cashpay' => '<strong>I do</strong> wish to sign up for CashPay at this time and will complete the section below.',
    'no' => '<strong>I do not</strong> wish to sign up for Direct Deposit or CashPay at this time. I would prefer to have my pay check mailed to me. I understand that there may be a delay to my paycheck delivery via the US Postal Service which is beyond the control of On Assignment.',
  ));
  $form['delivery']['policy'] = markup('<p>Check Replacement Policy<br/>On Assignment requires that you wait 5 business days from the date the check was mailed before requesting a replacement check. Additionally, if a check replacement is needed, you will be rquired to sign up for Direct Deposit or CashPay.</p>');

  $form[536] = fieldset('Direct Deposit');
  $form[536]['#attributes']['id'] = 'fieldset-directdeposit';
  $form[536]['#collapsed'] = TRUE;
  $form[536]['bank_name'] = textfield('Bank Name',40);
  $form[536]['bank_city'] = textfield('Bank City',40);
  $form[536]['bank_state'] = state('State');
  $form[536]['bank_phone'] = textfield('Bank Phone');
  $form[536]['bank_account'] = textfield('Bank Account');
  $form[536]['bank_route'] = textfield('Routing #');
  $form[536]['rdo_account_type'] = radios('Account Type', array('Checking', 'Savings'), TRUE);
  // No fields required.
  foreach ($form[536] AS $k => $v) {
    if (in_array($v['#type'], array('textfield', 'radios', 'select'))) {
      $form[536][$k]['#required'] = null;
    }
  }

  // @TODO To be removed. Not collecting Credit Card data.
  /*
  $form[537] = fieldset('CashPay Enrollment');
  $form[537]['#attributes']['id'] = 'fieldset-cashpay';
  $form[537]['#collapsed'] = TRUE;
  $form[537]['#description'] = 'Please select the card type and English or Spanish below:';
  $form[537]['rdo_cc_type'] = radios('', array('Visa', 'Instant Issue (Issue at Employment Location)'), TRUE);
  $form[537]['rdo_cc_lang'] = radios('', array('English', 'Spanish'), TRUE);
  $form[537]['cc_number']['#tree'] = TREE;
  $cc_num = textfield('', 4, 4);
  $form[537]['cc_number'][] = $cc_num;
  $form[537]['cc_number'][0]['#prefix'] = '<div class="field-cc">';
  $form[537]['cc_number'][] = $cc_num;
  $form[537]['cc_number'][] = $cc_num;
  $form[537]['cc_number'][] = $cc_num;
  $form[537]['cc_number'][3]['#suffix'] = '</div>';


  // No fields required.
  foreach ($form[537] AS $k => $v) {
    if ($v['#type'] == 'textfield' || $v['#type'] == 'radios') {
      $form[537][$k]['#required'] = null;
    }
  }
  */

  return $form;
}

/**
 * Form submission callback for Paycheck Method
 */
function oa_portal_p3_paycheck_form_validate($form, &$form_state) {
  if ($form_state['values']['rdo_delivery_option'] == 'directdeposit') { 
    // @TODO: require verify Direct Deposit fields are complete
  }
  if ($form_state['values']['rdo_delivery_option'] == 'cashpay') {
    // @TODO: require verify CashPay Enrollment fields are complete
  }
}

/**
 * Form submission callback for Paycheck Method
 */
function oa_portal_p3_paycheck_form_submit($form, &$form_state) {
  if ($form_state['values']['rdo_delivery_option'] == 'directdeposit') {
    /**
     * @TODO Post 536 Direct Deposit to UCM
     * @TODO needs PDF fields
     * 
     */
    dsm($form_state['values'][536], 'Process & post 536 to UCM');
    if ($success = oa_portal_ucm_createpdfform($account->rmaxid, 536, $form_state['values'][536])) {
      $form_state['redirect'] = 'portal/p3/w4';
    }
    else {
      drupal_set_message('An error occured. Please try again.');
    }
  }
  /*
  elseif ($form_state['values']['rdo_delivery_option'] == 'cashpay') {
    **
     * @TODO Post 537 Cashpay Enrollment to UCM
     * @TODO needs PDF fields
     * 
     *
    dsm($form_state['values'][537], 'Process & post 537 to UCM');
    if ($success = oa_portal_ucm_createpdfform($account->rmaxid, 537, $form_state['values'][537])) {
      $form_state['redirect'] = 'portal/p3/w4';
    }
    else {
      drupal_set_message('An error occured. Please try again.');
    }
  }
  */
  else {
    // @TODO Mark this section as complete anyway.
  }
}

/**
 * Form callback for W4
 */
function oa_portal_p3_w4_form($form_state) {
  $form = array();
  $form[474] = fieldset('W4');

  $form[474]['#description'] = 'If you would like to reference the W4 Personal Allowances Worksheet, Deductions and Adjustments Worksheet, and Two-Earners/Multiple Jobs Worksheet please <a href="to-do">click here</a>.';

  $form[474]['ssn'] = textfield_ssn();
  // $form[474]['dob'] = date_popup('Date of Birth');

  $form[474]['rdo_marital_status'] = radios('', array('Single', 'Married', 'Married, but withhold at higher Single rate'), TRUE);
  $form[474]['marital_status']['#description'] = t('<strong>Note:</strong> If married, but legally seperated, or spouse is a nonresident alien, check the "Single" box');
  $form[474]['different_name'] = markup('<p><strong>If your last name differs from that shown on your social security card, check here.<br/>You must call 1-800-772-1213 for a replacement.</strong></p>');
  $form[474]['totalclaims'] = textfield('Total number of allowances you are claiming',6);
  $form[474]['addtnlamount'] = textfield('Additional amount, if any, you want withheld from each paycheck',6);
  $form[474]['exempt_desc'] = markup('<strong>I claim exemption from withholding for 2012, and I certify that I meet both of the following conditions for exemption:</strong><br/>
  <ul><li>Last year I had a right to a refund of all federal income tax withheld because I had no tax liability, and </li><li>This year I expect a refund of all federal income tax withheld because I expect to have no tax liability</li></ul><strong>');
  $form[474]['exempt'] = textfield('If you meet both conditions, write "Exempt" here', 6);
  $form[474]['chk_certify'] = checkbox('Under penalties of perjury, I declare that I have examined this certificate and, to the best of my knowledge and belief, it is true, correct, and complete.');

  return $form;
}

/**
 * Form submission callback for W4
 */
function oa_portal_p3_w4_form_submit($form, &$form_state) {
  /**
   * @TODO Post 474 2011 W4 to UCM
   *
   * firstnameandinitial - From Profile
   * lastname - From Profile
   * ssn
   * address - From Profile
   * citystatezip - From Profile
   * @TODO Note: missing from PDF doc: Marital status (marital_status above)
   * totalclaims
   * addtnlamount
   * exempt
   */
  dsm($form_state['values'][474], 'Process & post 477 to UCM');
  if ($success = oa_portal_ucm_createpdfform($account->rmaxid, 474, $form_state['values'][474])) {
    $form_state['redirect'] = 'portal/p3/i9';
  }
  else {
    drupal_set_message('An error occured. Please try again.');
  }
}

/**
 * Form callback for I-9
 */
function oa_portal_p3_i9_form($form_state) {
  $form = array();
  $form[461] = fieldset('I9');
  $form[461]['ssn'] = textfield_ssn();
  $form[461]['dob'] = date_popup('Date of Birth');
  $form[461]['maiden'] = textfield('Maiden/other names used');
  $form[461]['citizentype'] = radios('I attest, under penalty of perjury, that I am (check one of the following)',
    array('citizen' => t('A citizen of the United States'), 'national' => t('A noncitizen national of the United States'), 'resident' => t('A lawful permanent resident'), 'alien' => t('An alien authorized to work') ));
  $form[461]['expiry'] = date_popup('Until (expiration date if applicable)');
  $form[461]['expiry']['#required'] = FALSE;
  $form[461]['chk_certify'] = checkbox('I am aware that federal law provides for imprisonment and/or fines for false statements or use of false documents in connection with the completion of this form.');

  return $form;
}

/**
 * Form submission callback for I-9
 */
function oa_portal_p3_i9_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid);

  /**
   * lastname - From Profile
   * firstname - From Profile
   * initial - From Profile
   * maiden
   * address - From Profile
   * apt - From Profile
   * dob
   * city - From Profile
   * state - From Profile
   * zip - From Profile
   * ssn
   * citizentype
   * expiry - @TODO Missing from PDF: Expiration date of employment status
   * appsignature - From Profile
   * apptimestamp - current date/time
   */

  // PDF Field => RMax Profile Field

  foreach (array(
    'lastname' => 'lname',
    'firstname' => 'fname',
    'address' => 'address',
    'city' => 'city',
    'state' => 'state',
    'zip' => 'postal',
    ) AS $pdff => $rmaxf) {
    $form_state['values'][461][$pdff] = $candidate->profile->{$rmaxf};
  }
  
  // No initial for now. Not in RMax profile.
  $form_state['values'][461]['initial'] = '';
  
  $form_state['values'][461]['dob'] = date('m/d/Y', strtotime($form_state['values'][461]['dob']));
  $form_state['values'][461]['appsignature'] = $candidate->profile->name;
  $form_state['values'][461]['apptimestamp'] = date('m-d-Y g:i:s A');

  unset($form_state['values'][461]['chk_certify']);

  dsm($form_state['values'][461], 'Process & post 461 to UCM');
  if ($success = oa_portal_ucm_createpdfform($account->rmaxid, 461, $form_state['values'][461])) {
    $form_state['redirect'] = 'portal/p3/eeo';
  }
  else {
    drupal_set_message('An error occured. Please try again.');
  }
}

/**
 * Form callback for EEO Form
 */
function oa_portal_p3_eeo_form($form_state) {
  $form = array();
  $form['#description'] = t('');
  
  $form[1] = fieldset('EEO');
  $form[1]['#description'] = t('Please fill out the Affirmative Action date below. To ensure that On Assignment complies with pertinent hiring practices, On Assignment must keep records about our applicants for employment.  This questionnaire will be kept in a confidential file, seperate from the application for employment.  Failure to provide this information will in no way adversely affect your candidacy for this position.  However, we would greatly appreciate your willingness to submit this information for our commitment to equal opportunity.<br/><br/><small><em>Note that all definitions as listed below were provuded by the U.S. Federal Government Equal Employment Opportunity Commission (EEOC)</em></small>');
  $form[1]['rdo_ethnicity'] = radios('Ethnicity Code', array('hispanic' => t('Hispanic or Latino'),'asian' => t('Asian'),'white' => t('White'),'american_indian' => t('American Indian or Alaskan Native'),'black' => t('Black'),'two_more' => t('Two or more races'),'hawaiian' => t('Native Hawaiian or Pacific Islander') ) );
  $form[1]['rdo_gender'] = radios('Gender', array('male' => t('Male'), 'female' => t('Female')));
  $form[1]['rdo_handicap'] = radios_yesno('Handicap Status');
  $form[1]['handicap']['#suffix'] = t('<em><small>(Any person who has a physical or mental impairment that substantially limits one or more major life activities, has a record of such impairments or is regarded as having such impairments)</small></em>');
  $form[1]['chk_veteran'] = checkboxes('Veteran Status', array('armed_forces' => t('Armed Forces Service Medal Veteran'), 'disabled' => t('Disabled Veteran'), 'protected' => t('Other Protected Veteran'), 'seperated' => t('Recently Seperated Veteran')) );
  $form[1]['veteran']['#description'] = t('<small><em>If you are unsure of your Veteran status under these definitions, contact your recruiter.</em></small>');
  $form[1]['rdo_age'] = radios_yesno('Age');
  $form[1]['age']['#description'] = t('<small><em>Are you over age 40 but under age 70?</em></small>');
  
  return $form;
}

/**
 * Form submission callback for EEO Form
 */
function oa_portal_p3_eeo_form_submit($form, &$form_state) {
  /**
   * @TODO Post 1 EEO to UCM
   * @TODO needs PDF fields
   * 
   */
  dsm($form_state['values'][1], 'Process & post 1 EEO to UCM');
  if ($success = oa_portal_ucm_createpdfform($account->rmaxid, 1, $form_state['values'][1])) {
    $form_state['redirect'] = 'portal/p3/work_policy';
  }
  else {
    drupal_set_message('An error occured. Please try again.');
  }
}

/**
 * Form callback for Work Policy
 */
function oa_portal_p3_work_policy_form($form_state) {
  $form = array();

  // All sections on this form submit to Formtype 466
  $form['harassment'] = fieldset('Harassment Policy Acknowledgement');
  $form['harassment']['ack'] = acknowledgement('In an effort to reiterate On Assignment\'s long-standing policy against illegal harassment in the workplace, enclosed is a copy of the On Assignment Harassment Policy. Sexual harassment or any other form of harassment based on a protected characteristic (e.g. age, race, religion etc.) is against the law and will not be tolerated at On Assignment. Please sign and date the enclosed acknowledgment that you have received and agree to abide by the policy.
    Our policy against harassment applies to all of our employees, including temporary professionals, as well as all applicants and other persons providing services pursuant to a contract. The policy also applies when our temporary professionals are assigned to work at a client’s facility, regardless of whether or not the person who is engaged in improper conduct is one of our employees or temporary professionals. If you are being subjected to illegal harassment, we want to know about it so that we can act to investigate and address the situation as quickly as possible.
    It is your responsibility, however, to report the problem to us, and we encourage you to use the attached Complaint Form to do so.
    If you have any questions about this policy, please contact the Human Resources Director at 818-878-7900.',
    'I acknowledge receipt of On Assignment\'s Harassment Policy and written complaint form and I Agree to abide by the policy.');

  $form['safety'] = fieldset('Safety Policy Acknowledgement');
  $form['safety']['ack'] = acknowledgement('I have been provided with training on health and safety and healthcare topics appropriate for my assignments. This may have included Illness and Injury Prevention, Hazard Communication, General Safety, Bloodborne Pathogens and Tuberculosis, HIPAA and Age-Specific Care. I have been informed that other training programs are available if specifically required for my duties. I have been informed that this training contains information that is a regulatory requirement, and will assist me in working safely on my job. I have also been informed that I will need to receive site-specific training on similar topics every time I go on a new assignment and need to contact my Staffing Consultant if I do not receive it or understand it.
  I acknowledge that I will successfully complete this training on my own with no assistance before I am assigned to my first job. I acknowledge that I will abide by On Assignment Health and Safety Policies, and all of the applicable policies and guidelines of the client at each assignment. When this training is completed, along with any training that I may receive while on various assignments, I will let my Staffing Consultant know if I did not understand it or if I do not think that it is adequate to safely perform my duties.
  I understand that working safely is a condition of employment at On Assignment, and failure to follow established policies and procedures may result in disciplinary action up to and including termination.');

  $form['stock_plan'] = fieldset('On Assignment, Inc.\'s Employee Stock Purchase Plan');
  $form['stock_plan']['ack'] = acknowledgement('Despite any representations which may have been made to the contrary by any person in any manner, I understand that as of April 1, 2007, I am not eligible for participation in On Assignment, Inc.’s Employee Stock Purchase Plan (“ESPP”). I further agree to hold On Assignment, Inc. and its subsidiaries, along with its employees, agents, directors and representatives, harmless for any representation which may have been made regarding my eligibility for participation in the ESPP.');

  return $form;
}

/**
 * Form submission callback for Work Policy
 */
function oa_portal_p3_work_policy_form_submit($form, &$form_state) {
  /**
   * @TODO Post 466 Work Policy Acknowledgement to UCM. Applies to all sections in this form.
   *
   */
  dsm($form_state['values'], 'Process & post 466 to UCM');
  $fields = $form_state['values'];
  if ($success = oa_portal_ucm_createpdfform($account->rmaxid, 466, $fields)) {
    $form_state['redirect'] = 'portal';
  }
  else {
    drupal_set_message('An error occured. Please try again.');
  }  
}

/**
 * Form helper functions
 */
 
 
/**
 * Implements hook_form_alter().
 */
function oa_portal_form_alter(&$form, &$form_state, $form_id) {
  global $user;
  if (substr($form_id, 0, 11) == 'oa_portal_p' && $form_id != 'oa_portal_p2_licensure_certs_form') {
    $form['#attributes'] = array('class' => 'oaportal');
    $form['#ajax'] = array('enabled' => TRUE, 'disable_redirect' => TRUE);

    $account = user_load($user->uid);
    $candidate = get_candidate($account->rmaxid);
    dsm($candidate, '$candidate object');
    $p = $candidate->p;

    $form['#formtypes'] = $p[arg(1)][arg(2)]['#formtypes'];
    $form['#formtypes_open'] = $p[arg(1)][arg(2)]['#formtypes_open'];
    $form['#formtypes_pr'] = $p[arg(1)][arg(2)]['#formtypes_pr'];
    $form['#formtypes_closed'] = $p[arg(1)][arg(2)]['#formtypes_closed'];

    // For each fieldset, check to ensure formids are indeed open.
    // Numeric fieldsets are easy, others may indicate more than one formid apply
    foreach ($form AS $ek => $element) {
      if ($element['#type'] == 'fieldset') {
        if (is_numeric($ek)) {
          if (!in_array($ek, $form['#formtypes_open']) && FALSE) { // @TODO REMOVE!
            // Set this fieldset as empty and locked
            $form[$ek] = fieldset_locked($element['#title']);
          }
        }
        elseif (in_array($ek, array('tb', 'mmr', 'cls', 'varivax', 'hepb'))) {
          if (
            ($ek == 'tb' && !count(array_intersect(array(291, 288), $form['#formtypes_open']))) ||
            ($ek == 'mmr' && !count(array_intersect(array(261, 262, 284, 285), $form['#formtypes_open']))) ||
            ($ek == 'cls' && !count(array_intersect(array(19, 37, 13), $form['#formtypes_open']))) ||
            ($ek == 'varivax' && !count(array_intersect(array(298, 296, 295), $form['#formtypes_open']))) ||
            ($ek == 'hepb' && !count(array_intersect(array(270, 271, 272, 274, 275), $form['#formtypes_open'])))
            ) {
            // Set this fieldset as empty and locked
            $form[$ek] = fieldset_locked($element['#title']);
            dsm($element, $ek .' - Closed special case fieldset. Not open.');
          }
          else {
            dsm($element, $ek .' - Special case fieldset. Did not close.');
          }
        }
      }
    }

    // @TODO If all formtypes are submitted, expose continue button only
    $something_to_submit = FALSE;
    foreach ($form AS $ek => $element) {
      if ($element['#type'] == 'fieldset') {
        $something_to_submit = TRUE;
      }
    }

    if ($something_to_submit) {
      $form['finalize'] = array(
        '#type' => 'button',
        '#value' => t('Finalize'),
        '#weight' => 1998,
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#prefix' => '<div id="finalize-confirm-box" style="display:none;"><p>Once you finalize your application, you will not be able to edit it anymore.</p><p>Are you sure you want to finalize?</p><div id="finalize-cancel">No, I need to make changes</div>',
        '#value' => t("Yes, I'm done"),
        '#suffix' => '</div>',
        '#weight' => 1999,
      );
      $form['throbber'] = array(
        '#value' => '<div id="form-throbber" class="set-hide"></div>',
        '#weight' => 1998,
      );
      $form['continue'] = array(
        '#type' => 'button',
        '#value' => t('Continue'),
        '#weight' => 1998,
        '#attributes' => array('class' => 'set-hide'),
      );
    }
    else {
      $form['continue'] = array(
        '#type' => 'button',
        '#value' => t('Continue'),
        '#weight' => 1998,
        '#attributes' => array('class' => 'portal-continue'),
      );
    }

    if (count($form['#formtypes'])) {
      $form['#docs'] = $candidate->get_docs_not_missing($form['#formtypes']);
      if (is_array($form['#docs']) && count($form['#docs'])) {
        $form['docs'] = array(
          '#value' => theme('oap_doc_download', $form['#docs']),
          '#weight' => 1997,
        );
      }
    }


    // Add settings for Autosaving
    $path = drupal_get_path('module', 'oa_portal');
    drupal_add_js($path . '/js/jquery.field.js');
    drupal_add_js($path . '/js/autosave.js');
    
    $settings['autosave']['url'] = url('portal/autosave');
    $settings['autosave']['q'] = $_GET['q'];
    $settings['autosave']['form_id'] = $form_id;
    $settings['autosave']['form_id_id'] = $form['#id'];

    // If an autosaved version of the form exists, make it available via javascript.
    if ($autosaved_form = oa_portal_get_autosaved_form($form_id, $_GET['q'], $user->uid)) {
      $settings['autosave'] = array_merge($settings['autosave'], array(
        'serialized' => unserialize($autosaved_form['serialized']),
        'saved_date' => format_date($autosaved_form['timestamp'], 'medium'),
      ));
    }
    drupal_add_js($settings, 'setting');

    dsm($form, 'Completed $form');
  }

}

/**
 * Skills Checklist constructor
 */
function oa_portal_checklist(&$elements, $skills) {
  if (count($skills)) {
    $sections = 0;
    foreach ($skills AS $section => $skills) {
      $sections++;
      $elements[$section] = fieldset($section);
      $elements[$section]['#tree'] = TRUE;
      foreach ($skills AS $k => $v) {
        $elements[$section][$sections .'_'. $k] = checklist_skill($v);
      }
    }
  }
}


/**
 * Multi-Fieldgroup constructor
 */
function oa_portal_set($sub_elements, $text, $ct = 2) {
  $sub_elements['#tree'] = TRUE;
  $sub_elements['start'] = array(
    '#value' => '<div class="add-another-set set-hide cf">',
    '#weight' => -1000,
  );
  $sub_elements['end'] = array(
    '#value' => '<div class="add-another-link cf">'. t($text) .'</div></div>',
    '#weight' => 1999,
  );
  
  $elements = array();
  for ($i = 1; $i <= $ct; $i++) {
    if ($i == 2) {
      foreach ($sub_elements AS $k => $v) {
        if ($v['#required']) {
          $sub_elements[$k]['#required'] = FALSE;
        }
      }
    }
    $elements[] = $sub_elements;
  }

  $elements[0]['start']['#value'] = '<div class="add-another-set cf">';
  $elements[($ct - 1)]['end']['#value'] = '</div>';
  return $elements;
}
