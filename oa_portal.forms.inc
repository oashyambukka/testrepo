<?php

include_once('oa_portal.fields.inc');

/**
 * Form callback for Password change form
 */
function oa_portal_change_password_form($form_state) {
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
  $account = user_load(array('uid' => $user->uid));
  $account->pass = $form_state['values']['pass'];
  user_save($account, array('pass' => $form_state['values']['pass']));
  drupal_set_message(t('Your password has been updated.'));
  $form_state['redirect'] = 'portal';
}

/**
 * Form callback for Contact
 */
function oa_portal_p1_contact_form() {
  $form['contact'] = fieldset('Contact Information');
  $form['contact']['firstname'] = textfield('First Name', 15, 20);
  $form['contact']['initial'] = textfield('Middle Initial', 3, 1);
  $form['contact']['initial']['#required'] = FALSE;
  $form['contact']['lastname'] = textfield('Last Name', 20, 30);
  $form['contact']['address'] = address();
  $form['contact']['primary_phone'] = textfield('Primary Phone', 15, 10);
  $form['contact']['mobile_phone'] = textfield('Cell Phone', 15, 10);
  $form['contact']['email'] = textfield('Email');
  $form['contact']['email']['#rules'][] = 'email';
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Contact
 */
/*
function oa_portal_p1_contact_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Background/Reference Check
 */
function oa_portal_p1_contact_form_submit($form, &$form_state) {
  // @TODO Post to RMax/UCM
}


/**
 * Form callback for Application form
 */
function oa_portal_p1_application_form($form_state) {
  $form = array();
  
  // Contact Information
  $form['contact'] = fieldset('Contact Information');
  $form['contact']['#prefix'] = '<p><br>If you are ready to begin working with On Assignment Healthcare Staffing, we\'re ready to start working with you.  Please complete the application and one of our experienced recruiters will contact you soon.</p><br>
  <p>Please be assured that On Assignment maintains a strict privacy policy.  We will not share your information with any third party without your authorization and will only contact you for the specific purpose of serving your request.</p><br><br>';
  $form['contact']['emerg1_name'] = textfield('Name', 30, 50);
  $form['contact']['emerg1_name']['#prefix'] = t('<h1>Emergency Contact 1</h1>');
  $form['contact']['emerg1_phone'] = textfield('Phone', 15, 15);
  $form['contact']['emerg2_name'] = textfield('Name', 30, 50);
  $form['contact']['emerg2_name']['#prefix'] = t('<h1>Emergency Contact 2</h1>');
  $form['contact']['emerg2_phone'] = textfield('Phone', 15, 15);

  // Education
  $form['education'] = fieldset('Education', 'Please do not note the year high school diploma was received');

  $edu_fields['school'] = textfield('School', array('#options' => array('opt1' => 'Opt 1')));
  $edu_fields['rdo_grad_status'] = radios('Did you graduate', array(1 => 'Yes', 0 => 'No'));
  $edu_fields['degree'] = select('Degree Earned', array('Associates'));
  $edu_fields['city'] = textfield('City', 30);
  $edu_fields['state'] = state();
  $edu_fields['year'] = textfield('Year', 10, 4);
  $edu_fields['major'] = textfield('Major', 30);
  $edu_fields['minor'] = textfield('Minor', 30);
  $edu_fields['coursework1'] = textfield('Relevant Course Work', 25);
  $edu_fields['coursework2'] = textfield('', 25);
  $edu_fields['coursework3'] = textfield('', 25);
  $edu_fields['coursework4'] = textfield('', 25);
  $form['education']['edu'] = oa_portal_set($edu_fields, 'Add another school', 3);

  // Experience
  $form['experience'] = fieldset('Experience');

  $form['experience']['chk_recent_grad'] = checkbox('Check box if you are a recent college graduate');  

  $exp_fields['employer'] = textfield('Employer',75);
  $exp_fields['city'] = textfield('City');
  $exp_fields['state'] = state('State');
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
  $form['experience']['exp'] = oa_portal_set($exp_fields, 'Add another job', 3);


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
  $form['licecerts'] = fieldset('Licensures/Certifications');

  $lc_fields['name'] = textfield('Licensure/Certification');
  $lc_fields['state'] = state();
  $lc_fields['number'] = textfield('License Number', 12);
  $lc_fields['expiry_date'] = date_popup('Expires');
  $form['licecerts']['lc'] = oa_portal_set($lc_fields, 'Add another Licensure/Certification', 4);

  $form['proforgs'] = fieldset('Professional Organizations');
  $proforgs['chk_none'] = checkbox('I am not a member of any organizations');
  $proforgs['org_name'] = textfield('Organization');
  $proforgs['org_role'] = textfield('Role');
  $proforgs['org_start'] = date_popup('From');
  $proforgs['org_end'] = date_popup('To');

  $form['proforgs']['orgs'] = oa_portal_set($proforgs, 'Add another Organization', 4);
  
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
  $docs = array('ucmid123' => 'Doc 123', 'ucmid456' => 'Doc 456');
  oa_portal_submit($form, $docs);
  return $form;
}

/**
 * Form validation callback for Application
 */
/*
function oa_portal_p1_application_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Application
 */
function oa_portal_p1_application_form_submit($form, &$form_state) {
  // @TODO Post to RMax/UCM
//  $values = $form_state['values'];
//  dsm($values);
}

/**
 * Form callback for Checklist
 */
function oa_portal_p1_checklist_form($form_state) {
  $form = array();
  $form['directions'] = markup('<p>Directions: Indicate your level of experience by selecting the numbers below as follows:<br>
    1 = Can Function Independently<br>
    2 = Experienced, but May Need Review<br>
    3 = Limited Experience<br><br></p>');

  $p1_formtypes = oa_portal_get_modality_formtypes(1, ($_GET['modality']) ? $_GET['modality'] : 'T004615');
  $checklist_formtypes = array_intersect(range(480, 532) + array(547), $p1_formtypes);
  $form['#formtypes'] = array(517);
  foreach ($form['#formtypes'] AS $cft) {
    $fn = 'oa_portal_p1_checklist_form_'. $cft;
    $form[$cft] = $fn();
  }

  $docs = array('ucmid123' => 'Doc 123', 'ucmid456' => 'Doc 456');
  
  oa_portal_submit($form, $docs);
  dsm($form, 'Skills Checklist $form');
  return $form;
}

/**
 * Form validation callback for Checklist
 */
/*
function oa_portal_p1_checklist_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Checklist
 */
function oa_portal_p1_checklist_form_submit($form, &$form_state) {
  dsm($form, '$form in submission');
  // @TODO Post to UCM
  dsm($form_state['values'], '$form_state values');
}

function oa_portal_p1_checklist_form_480() {
  $elements = fieldset('Acute Care Self Assessment Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_481() {
  $elements = fieldset('Adult Pediatric and Psychiatrics Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_482() {
  $elements = fieldset('Cardiac Cath Lab Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_483() {
  $elements = fieldset('Case Manager Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_484() {
  $elements = fieldset('Certified Nursing Assistant Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_485() {
  $elements = fieldset('Clerical Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_486() {
  $elements = fieldset('Critical Care Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_487() {
  $elements = fieldset('Cytology');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_488() {
  $elements = fieldset('Dental Assistant Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_489() {
  $elements = fieldset('Dental Hygenist Skills');

  oa_portal_checklist($elements);
  return $elements;
}

// 490 - Echo Tech Skills - likely out of scope

function oa_portal_p1_checklist_form_491() {
  $elements = fieldset('Endoscopy Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_492() {
  $elements = fieldset('ER Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_493() {
  $elements = fieldset('Essential/Practical Skills Checklist');
  $skills = array(
    'Core Skills' => array(
      'rdo_item1' => 'Admission of a Patient',
      'rdo_item2' => 'Transfer of a Patient',
      'rdo_item3' => 'Discharge of a Patient',
    ),
    'Cardiovascular' => array(
      'rdo_item1' => 'Doppler',
      'rdo_item2' => 'Lead placement',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_494() {
  $elements = fieldset('Hemodialysis Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_495() {
  $elements = fieldset('Histology Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_496() {
  $elements = fieldset('Histotech Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_497() {
  $elements = fieldset('Interventional Radiology Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_498() {
  $elements = fieldset('Labor & Delivery Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_499() {
  $elements = fieldset('Laboratory Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_500() {
  $elements = fieldset('LVN Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_501() {
  $elements = fieldset('Mammography/Ultrasound Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_502() {
  $elements = fieldset('Medical Assistant Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_503() {
  $elements = fieldset('Medical Technology Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_504() {
  $elements = fieldset('MedSurg Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_505() {
  $elements = fieldset('MedTech Assistant Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_506() {
  $elements = fieldset('NICU Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_507() {
  $elements = fieldset('Nurse Case Manager Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_508() {
  $elements = fieldset('Nurse Practicioner');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_509() {
  $elements = fieldset('Nursery Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_510() {
  $elements = fieldset('Occupational Therapy Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_511() {
  $elements = fieldset('Operating Room Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_512() {
  $elements = fieldset('Optical Skills Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_513() {
  $elements = fieldset('PACU Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_514() {
  $elements = fieldset('Pediatrics Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_515() {
  $elements = fieldset('Pharmacist Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_516() {
  $elements = fieldset('Pharmacy Tech Skills');

  oa_portal_checklist($elements);
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

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_519() {
  $elements = fieldset('PICU Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_520() {
  $elements = fieldset('Post Anesthesia Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

// 521 - Psych Skills - likely out of scope

function oa_portal_p1_checklist_form_522() {
  $elements = fieldset('Radiology Skills');

  oa_portal_checklist($elements);
  return $elements;
}

// 523 - Rehab Skills - likely out of scope

function oa_portal_p1_checklist_form_524() {
  $elements = fieldset('Reprocessing Tech Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_525() {
  $elements = fieldset('Respitory Therapist Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_526() {
  $elements = fieldset('Respitory Tech Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_527() {
  $elements = fieldset('RN/LPN Clinical Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_528() {
  $elements = fieldset('Speech Therapy Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_529() {
  $elements = fieldset('Speech Therapy Skills');

  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_530() {
  $elements = fieldset('Stepdown/PCU/Tele Skills');
  
  oa_portal_checklist($elements);
  return $elements;
}

function oa_portal_p1_checklist_form_531() {
  $elements = fieldset('Sterile Processing Tech Skills');

  oa_portal_checklist($elements);
  return $elements;
}


function oa_portal_p1_checklist_form_532() {
  $elements = fieldset('Surgical Processing Tech Skills');

  oa_portal_checklist($elements);
  return $elements;
}

// 533 - VaxAmerica Orientation Checklist - likely out of scope
// 540 - Cath Lab Skills - likely out of scope

function oa_portal_p1_checklist_form_547() {
  $elements = fieldset('Certified Occupational Therapy Assistant Skills');

  oa_portal_checklist($elements);
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
 */
function oa_portal_p2_professional_orgs_form($form_state) {
  $form['prof_orgs'] = fieldset('Professional Organizations');
  $form['prof_orgs']['chk_no_orgs'] = checkbox('I am not a member of any organizations');

  $org = array('#tree' => TRUE);
  $org['org_name'] = textfield('Organization');
  $org['org_role'] = textfield('Role');
  $org['org_date_from'] = date_popup('From');
  $org['org_date_to'] = date_popup('To');
  $form['prof_orgs']['org1']['#tree'] = TRUE;
  $form['prof_orgs']['org1'] = $org;
  $form['prof_orgs']['org2']['#tree'] = TRUE;
  $form['prof_orgs']['org2'] = $org;

  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Professional Organizations
 */
/*
function oa_portal_p2_professional_orgs_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Professional Organizations
 */
function oa_portal_p2_professional_orgs_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}

/**
 * Form callback for Licensure Certifications
 */
function oa_portal_p2_licensure_certs_form($form_state) {
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['lc'] = fieldset('Licensure/Certification');
  $form['lc']['#description'] = t('Upload a copy of your licensure/certification. .pdf, .jpg and .doc are acceptable formats.');
  // @TODO Get list of Licensures
  $form['lc']['lc_licensure'] = select('Select which certificate you are uploading', array('item1', 'item2'));
  $form['lc']['lc_file'] = array(
    '#type' => 'file',
    '#title' => t('Upload file'),
    '#required' => TRUE,
    '#size' => 80,
  );
  $form['lc']['uploaded'] = markup('<h3>Uploaded files</h3>');
  $form['lc']['chk_fax_preference'] = checkbox('Check this box if you prefer to fax copies of your licensures/certifications');

  // @TODO Get cover page doc ID
  $form['lc']['fax'] = markup('<PDF ICON> Please include this cover letter and fax it to (818) 555-5555. <cover page link here>');

  // @TODO Slightly different Finalize mechanism. Form to be submitted potentially more than once.
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Licensure Certifications
 */
/*
function oa_portal_p2_licensure_certs_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Licensure Certifications
 */
function oa_portal_p2_licensure_certs_form_submit($form, &$form_state) {
  // @TODO Post individual files to UCM, mark as finalized either after checkbox has been checked, or 
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
  $fmr['toformeraddr'] = date_popup('To', 'm-Y')
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
 
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Background/Reference Check
 */
/*
function oa_portal_p2_background_reference_form_validate($form, &$form_state) {
}
*/

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
  
  
}

/**
 * Form callback for Capabilities Questionnaire
 */
function oa_portal_p2_capabilities_form($form_state) {
  $form = array();
  
  $form['jcq'] = fieldset('Job Capabilities Questionnaire');
  $form['jcq']['#description'] = t('As a candidate for employment at On Assignment1, you have met the basic requirements of skills, experience, education and availability needed by our Clients.  Clients request that our employees work at their location for a specific period of time to complete various work duties.  As an On Assignment employee, you will typically be assigned to positions in healthcare, research and development and/or many different types of facilities.  These positions may involve working with various potentially hazardous substances and physical agents, including, but not limited to, the following:<br><br>
<ul><li>Various organic volatile and non-volatile chemicals, corrosives, potential carcinogens, mutagens and teratogens</li>
<li>Biological agents, bloodborne pathogens and radioactive sources and materials</li>
<li>Heat, cold, pressure (including compressed gasses), and noise</li>
<li>Working with glassware, pipetting, repetitive tasks</li>
</ul><br>These assignments may also include physical requirements that involve sitting/standing/walking for extended periods, bending and stooping, lifting or moving heavy objects and lifting up to 50 lbs. above the shoulders.  They may also involve repetitive tasks such as using a keyboard, operating equipment or manipulating small objects.  A safe working environment is important not only to you, but also to On Assignment and our Clients.  You are expected to take all reasonable safety precautions, and to follow all On Assignment and client safety policies and procedures.  This may include, but is not limited to Standard Operating Procedures, appropriate use of personal protective equipment and devices, and personal hygiene.  To assist us in assuring that we select the best possible assignment for you, please complete the following Job Capabilities Questionnaire.<br><br>');
  $form['jcq']['performance'] = cap_yne('Can you perform all of these and similar tasks with reasonable accommodation?');
  $form['jcq']['health'] = cap_yne('Do you have any health reasons that would prohibit you from performing these or similar tasks?');
  $form['jcq']['restrictions'] = cap_yne('Do you have any restrictions preventing you from performing these tasks?');
  $form['jcq']['allergies'] = cap_yne('Do you have any known allergies?', 'If yes, list');
  $form['jcq']['allergies']['rdo_dermatitis'] = radios_yesno('Do you have dermatitis?');
  $form['jcq']['allergies']['rdo_latex'] = radios_yesno('Are you allergic to latex gloves?');
  $form['jcq']['preference'] = cap_yne('Are there any assignments that you cannot or prefer not to do?');
  $form['jcq']['chk_certify'] = checkbox('I certify this information is correct, and understand that withholding or giving false information will result in refusal to hire or disciplinary action up to, and including, termination.');
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Capabilities Questionnaire
 */
/*
function oa_portal_p2_capabilities_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Capabilities Questionnaire
 */
function oa_portal_p2_capabilities_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}

/**
 * Form callback for Policies & Agreements
 */
function oa_portal_p2_policies_form($form_state) {
  $form = array();
  $form['substance_abuse'] = fieldset('Substance Abuse Testing Consent');
  $form['substance_abuse']['testing'] = acknowledgement('I understand that I may be offered a position with On Assignment...');
  $form['substance_abuse']['sample'] = acknowledgement('I agree to provide an appropriate sample...');

  $form['employment'] = fieldset('Employment Agreement');
  $form['employment']['personelle'] = acknowledgement('On Assignment Staffing Services, Inc.....');
  $form['employment']['ca_addendum'] = acknowledgement('Addendum for CA...');
  
  $form['direct_hire'] = fieldset('Direct Hire Candidate Agreement');
  $form['direct_hire']['relationship'] = acknowledgement('On Assignment Staffing Services...');
  $form['questions'] = markup('If you have any questions..');
  
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Policies & Agreements
 */
/*
function oa_portal_p2_policies_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Policies & Agreements
 */
function oa_portal_p2_policies_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}


/**
 * Form callback for Medical Authorization and Disclosures
 */
function oa_portal_p2_medical_form($form_state) {
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['auth'] = fieldset('Authorization For Use Or Disclosure Of Health Information');
  $form['auth']['disclosure'] = acknowledgement('I authorize the use or disclosure of my health information...');
  $form['auth']['providers'] = textfield('As described in section 1, list specific providers of information if applicable', 80);
  $form['auth']['info'] = textfield('As described in section 3, list specific information that may be used or disclosed if applicable', 80);
  $form['auth']['dates'] = textfield('As described in section 7, list date or event per 3) if applicable', 80);
  
  // @TODO Add help tooltip
  $form['tb'] = fieldset('TB');
  $form['tb']['rdo_state_12_negative_chest'] = radios('', array(
    'stage_12' => 'TB Vaccine Stage 1 & Stage 2',
    'negative_chest' => 'Negative Chest X-ray (every 3 years)',
    ));

  // @TODO Add help tooltip
  $form['mmr'] = fieldset('MMR');
  $form['mmr']['rdo_mmr_12_rubella_titer'] = radios('', array(
    'mmr_12' => 'MMR #1 / #2 if born after 1957',
    'rubella_titer' => 'Rubela Titer / Rubeola Titer / Mumps Titer',
    ));

  $form['mmr']['certificate'] = array(
    '#type' => 'file',
    '#title' => t('Certificate'),
    '#required' => TRUE,
    '#size' => 80,
  );
  $form['mmr']['chk_fax_preference'] = checkbox('I prefer to fax my certificate. Please fax with the cover letter below to 800-555-5555');

  $form['cardiac'] = fieldset('Cardiac Life Support');
  $form['cardiac']['#tree'] = TRUE;
  $form['cardiac']['rdo_cardiac'] = radios('', array('BLS', 'CPR', 'ACLS'), TRUE);
  $form['cardiac']['certificate'] = array(
    '#type' => 'file',
    '#title' => t('Certificate'),
    '#required' => TRUE,
    '#size' => 80,
  );

  $form['vari'] = fieldset('Varivax/Varicella');
  $form['vari']['rdo_vari'] = radios('', array(
    'vari_vaccine' => 'Varivax Vaccine #1',
    'stmt' => 'Varicella Immunity Stmt/Hist. of Disease',
    'titer' => 'Varicella Titer'));
  
  $form['hepb'] = fieldset('Hepatitis B Vaccination');
  $form['hepb']['#tree'] = TRUE;
  $form['hepb']['rdo_vaccination'] = radios('', array(
    'hep123' => 'Hep B Series 1, 2 & 3',
    'titer' => 'Hep B Titer',
    'declination' => 'Hep B Declination'));
  $form['hepb']['acknowledgement'] = acknowledgement(
    'Hepatitis B Vaccination Declination - Please complete Bloodborne Training before...',
    'I acknowledge, understand, and accept this Agreement/Statement. *This is a reg..'
  );
  
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Medical Authorization and Disclosures
 */
/*
function oa_portal_p2_medical_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Medical Authorization and Disclosures
 */
function oa_portal_p2_medical_form_submit($form, &$form_state) {
  // @TODO Post to UCM
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
  $form['delivery'] = fieldset('Paycheck Delivery');
  $form['delivery']['rdo_delivery_option'] = radios('', array(
    'yes' => '<strong>I do</strong> wish to sign up for Direct Deposit or CashPay at this time and will complete a paycheck delivery method below.',
    'no' => '<strong>I do</strong> wish to sign up for Direct Deposit or CashPay at this time. I would prefer to have my pay check mailed to me. I understand that there may be a delay to my paycheck delivery via the US Postal Service which is beyond the control of On Assignment.',
  ));
  $form['delivery']['policy'] = markup('<p>Check Replacement Policy<br/>On Assignment requires that you wait 5 business days from the date the check was mailed before requesting a replacement check. Additionally, if a check replacement is needed, you will be rquired to sign up for Direct Deposit or CashPay.</p>');

  $form['dd'] = fieldset('Direct Deposit');
  $form['dd']['bank_name'] = textfield('Bank Name',40);
  $form['dd']['bank_city'] = textfield('Bank City',40);
  $form['dd']['bank_state'] = state('State');
  $form['dd']['bank_phone'] = textfield('Bank Phone');
  $form['dd']['bank_account'] = textfield('Bank Account');
  $form['dd']['bank_route'] = textfield('Routing #');
  $form['dd']['rdo_account_type'] = radios('Account Type', array('Checking', 'Savings'), TRUE);
  
  $form['cp'] = fieldset('CashPay Enrollment');
  $form['cp']['#description'] = 'Please select the card type and English or Spanish below:';
  $form['cp']['rdo_cc_type'] = radios('', array('Visa', 'Instant Issue (Issue at Employment Location)'), TRUE);
  $form['cp']['rdo_cc_lang'] = radios('', array('English', 'Spanish'), TRUE);
  $form['cp']['cc_number']['#tree'] = TREE;
  $cc_num = textfield('', 4, 4);
  $form['cp']['cc_number'][] = $cc_num;
  $form['cp']['cc_number'][0]['#prefix'] = '<div class="field-cc">';
  $form['cp']['cc_number'][] = $cc_num;
  $form['cp']['cc_number'][] = $cc_num;
  $form['cp']['cc_number'][] = $cc_num;
  $form['cp']['cc_number'][3]['#suffix'] = '</div>';

  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Paycheck Method
 */
/*
function oa_portal_p3_paycheck_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Paycheck Method
 */
function oa_portal_p3_paycheck_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}

/**
 * Form callback for W4
 */
function oa_portal_p3_w4_form($form_state) {
  $form = array();
  $form['w4'] = fieldset('W4');
  $form['w4']['#description'] = 'If you would like to reference the W4 Personal Allowances Worksheet, Deductions and Adjustments Worksheet, and Two-Earners/Multiple Jobs Worksheet please <a href="to-do">click here</a>.';
  
  $form['w4']['rdo_marital_status'] = radios('', array(
    'Single', 'Married', 'Married, but withhold at higher Single rate'
  ), TRUE);
  $form['w4']['marital_status']['#description'] = t('<strong>Note:</strong> If married, but legally seperated, or spouse is a nonresident alien, check the "Single" box');
  $form['w4']['different_name'] = markup('<p><strong>If your last name differs from that shown on your social security card, check here.<br/>You must call 1-800-772-1213 for a replacement.</strong></p>');
  $form['w4']['allowances'] = textfield('Total number of allowances you are claiming',6);
  $form['w4']['amount_withheld'] = textfield('Additional amount, if any, you want withheld from each paycheck',6);
  $form['w4']['exempt_terms'] = markup('<strong>I claim exemption from withholding for 2012, and I certify that I meet both of the following conditions for exemption:</strong><br/>
  <ul><li>Last year I had a right to a refund of all federal income tax withheld because I had no tax liability, and </li><li>This year I expect a refund of all federal income tax withheld because I expect to have no tax liability</li></ul><strong>');
  $form['w4']['exempt'] = textfield('If you meet both conditions, write "Exempt" here', 6);
  $form['w4']['chk_certify'] = checkbox('Under penalties of perjury, I declare that I have examined this certificate and, to the best of my knowledge and belief, it is true, correct, and complete.');
  
  
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for W4
 */
/*
function oa_portal_p3_w4_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for W4
 */
function oa_portal_p3_w4_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}

/**
 * Form callback for I-9
 */
function oa_portal_p3_i9_form($form_state) {
  $form = array();
  $form['#description'] = t('');

  $form['i9'] = fieldset('I9');
  $form['i9']['rdo_citizentype'] = radios('I attest, under penalty of perjury, that I am (check one of the following)',
    array('citizen' => t('A citizen of the United States'), 'national' => t('A noncitizen national of the United States'), 'resident' => t('A lawful permanent resident'), 'alien' => t('An alien authorized to work') ));
  $form['i9']['until'] = date_popup('Until (expiration date if applicable)');
  $form['i9']['chk_certify'] = checkbox('I am aware that federal law provides for imprisonment and/or fines for false statements or use of false documents in connection with the completion of this form.');								

  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for I-9
 */
/*
function oa_portal_p3_i9_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for I-9
 */
function oa_portal_p3_i9_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}

/**
 * Form callback for EEO Form
 */
function oa_portal_p3_eeo_form($form_state) {
  $form = array();
  $form['#description'] = t('');
  
  $form['eeo'] = fieldset('EEO');
  $form['eeo']['#description'] = t('Please fill out the Affirmative Action date below. To ensure that On Assignment complies with pertinent hiring practices, On Assignment must keep records about our applicants for employment.  This questionnaire will be kept in a confidential file, seperate from the application for employment.  Failure to provide this information will in no way adversely affect your candidacy for this position.  However, we would greatly appreciate your willingness to submit this information for our commitment to equal opportunity.<br/><br/><small><em>Note that all definitions as listed below were provuded by the U.S. Federal Government Equal Employment Opportunity Commission (EEOC)</em></small>');
  $form['eeo']['rdo_ethnicity'] = radios('Ethnicity Code', array('hispanic' => t('Hispanic or Latino'),'asian' => t('Asian'),'white' => t('White'),'american_indian' => t('American Indian or Alaskan Native'),'black' => t('Black'),'two_more' => t('Two or more races'),'hawaiian' => t('Native Hawaiian or Pacific Islander') ) );
  $form['eeo']['rdo_gender'] = radios('Gender', array('male' => t('Male'), 'female' => t('Female')));
  $form['eeo']['rdo_handicap'] = radios_yesno('Handicap Status');
  $form['eeo']['handicap']['#suffix'] = t('<em><small>(Any person who has a physical or mental impairment that substantially limits one or more major life activities, has a record of such impairments or is regarded as having such impairments)</small></em>');
  $form['eeo']['chk_veteran'] = checkboxes('Veteran Status', array('armed_forces' => t('Armed Forces Service Medal Veteran'), 'disabled' => t('Disabled Veteran'), 'protected' => t('Other Protected Veteran'), 'seperated' => t('Recently Seperated Veteran')) );
  $form['eeo']['veteran']['#description'] = t('<small><em>If you are unsure of your Veteran status under these definitions, contact your recruiter.</em></small>');
  $form['eeo']['rdo_age'] = radios_yesno('Age');
  $form['eeo']['age']['#description'] = t('<small><em>Are you over age 40 but under age 70?</em></small>');
  
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for EEO Form
 */
/*
function oa_portal_p3_eeo_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for EEO Form
 */
function oa_portal_p3_eeo_form_submit($form, &$form_state) {
  // @TODO Post to UCM/RMax
}

/**
 * Form callback for Work Policy
 */
function oa_portal_p3_work_policy_form($form_state) {
  $form = array();
  $form['#description'] = t('');
  
  $form['harassment'] = fieldset('Harassment Policy Acknowledgement');
  $form['safety'] = fieldset('Safety Policy Acknowledgement');
  $form['stock_plan'] = fieldset('On Assignment, Inc.\'s Employee Stock Purchase Plan');
  
  
  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Work Policy
 */
/*
function oa_portal_p3_work_policy_form_validate($form, &$form_state) {
}
*/

/**
 * Form submission callback for Work Policy
 */
function oa_portal_p3_work_policy_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}


function oa_portal_submit(&$form, $docs = array()) {
  $form['#attributes'] = array('class' => 'oaportal');
  $form['#ajax'] = array('enabled' => TRUE);
  $form['docs'] = oa_portal_doc_download($docs);
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Finalize'),
  );
  // @TODO DO something else to enable the fancy inline form submission
  
  // @TODO Query UCM for existing submitted document
}

function oa_portal_checklist(&$elements, $skills) {
  foreach ($skills AS $section => $skills) {
    $elements[$section] = fieldset($section);
    $elements[$section]['#tree'] = FALSE;
    foreach ($skills AS $k => $v) {
      $elements[$section][$k] = checklist_skill($v);
    }
  }
}

function oa_portal_doc_download($docs) {
  return (count($docs)) ? array('#value' => theme('oap_doc_download', $docs)) : NULL;
}

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
    $elements[] = $sub_elements;
  }

  $elements[0]['start']['#value'] = '<div class="add-another-set cf">';
  $elements[($ct - 1)]['end']['#value'] = '</div>';
  return $elements;
}
