<?php

include_once('oa_portal.fields.inc');

/**
 * Form callback for Contact
 */
function oa_portal_p1_contact_form() {
  $form['contact'] = fieldset('Contact Information');
  $form['contact']['first_name'] = textfield('First Name');
  $form['contact']['middle'] = textfield('Middle Initial');
  $form['contact']['middle']['#required'] = FALSE;
  $form['contact']['address'] = address();
  $form['contact']['primary_phone'] = textfield('Primary Phone');
  $form['contact']['mobile_phone'] = textfield('Cell Phone');
  $form['contact']['email'] = textfield('Email');
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
/*
function oa_portal_p1_contact_form_submit($form, &$form_state) {
}
*/


/**
 * Form callback for Application form
 */
function oa_portal_p1_application_form($form_state) {
  $form = array();
  $form['#description'] = 'If you are ready to begin working.....';
  
  // Contact Information
  $form['contact'] = fieldset('Contact Information');

  $form['contact']['emerg1_name'] = textfield('Name', array('#prefix' => 'Emergency Contact 1'));
  $form['contact']['emerg1_phone'] = textfield('Phone');
  $form['contact']['emerg2_name'] = textfield('Name', array('#prefix' => 'Emergency Contact 2'));
  $form['contact']['emerg2_phone'] = textfield('Phone');

  // Education
  $form['education'] = fieldset('Education', 'Please do not note the year high school diploma was received');

  $edu_fields = array();
  $edu_fields['school'] = textfield('School', array('#options' => array('opt1' => 'Opt 1')));
  $edu_fields['grad_status'] = radios('Did you graduate', array(1 => 'Yes', 0 => 'No'));
  $edu_fields['degree'] = select('Degree Earned', array('Associates'));
  $edu_fields['city'] = textfield('City');
  $edu_fields['state'] = state();
  $edu_fields['year'] = textfield('Year');
  $edu_fields['major'] = textfield('Major');
  $edu_fields['minor'] = textfield('Minor');
  $edu_fields['coursework1'] = textfield('Relevant Course Work');
  $edu_fields['coursework2'] = textfield('');

  // @TODO Enable multiple entry
  $form['education']['edu'][] = $edu_fields;
  $form['education']['edu'][] = $edu_fields;
  
  // Experience
  $form['experience'] = fieldset('Experience');

  $form['experience']['recent_grad'] = checkbox('Check box if you are a recent college graduate');  
  $exp_fields['employer'] = textfield('Employer');
  $exp_fields['city'] = textfield('City');
  $exp_fields['state'] = state('State');
  $exp_fields['start_date'] = date_popup('Start Date');
  $exp_fields['end_date'] = date_popup('End Date');
  $exp_fields['present'] = checkbox('Present');
  $exp_fields['title'] = textfield('Title');
  $exp_fields['salary'] = textfield('Salary');
  $exp_fields['salary_period'] = radios('Salary', array('annual' => t('Annual'), 'hourly' => t('Hourly')));
  $exp_fields['description'] = textarea('Description', 5);
  $exp_fields['supervisor'] = textfield('Immediate Supervisor');
  $exp_fields['may_contact'] = checkboxes_yesno('May we contact?');
  $exp_fields['reason'] = textfield('Reason for leaving');
  $form['experience']['exp'][] = $exp_fields;
  
  
  // Preferences
  $form['preferences'] = fieldset('Preferences');
  $form['preferences']['distance'] = textfield('How far are you willing to commute on a daily basis?', 3, array(
    '#field_suffix' => t('Miles'),
  ));
  
  $form['preferences']['type'] = checkboxes('Check preferred employment type', array('Contract', 'Contract-to-hire', 'Direct Hire'), TRUE);
  $form['preferences']['work_days'] = checkboxes('Check the days you are willing to work', array('Any', 'M', 'T', 'W', 'Th', 'F', 'S', 'Su'), TRUE);
  $form['preferences']['shifts'] = checkboxes('Check the shifts you are willing to work', array('Any', 'Days', 'Nights', 'Evenings', 'Weekends'), TRUE);
  $form['preferences']['ptft'] = checkboxes('Would you prefer to work', array('Either', 'Part-time', 'Full-time'));
  $form['preferences']['legal_right'] = checkboxes_yesno('Can you, after employment, submit verification of your legal right to work in the U.S.?');
  $form['preferences']['felony'] = checkboxes_yesno('Have you been convicted of a felony within the last seven years?');
  $form['preferences']['felony']['#suffix'] = t('(An affirmative response to this question will not necessarily disqualify you from employment)<br \>MA residents should not answer this question');
  $form['preferences']['drug_alcohol_screen'] = checkboxes_yesno('As a condition of employment, you may be required to take and pass a drug and/or alcohol screen. Testing will be done at the company\'s expense and administered by a testing facility designated by the company. Results of any testing will be kept strictly confidential. If requested, are you willing to take the drug and/or alcohol screen?');
  $form['preferences']['referral'] = select('How did you hear about On Assignment Healthcare Staffing?',
    array('onassignment.com', 'other'));
  
  
  $form['preferences']['experience'] = textfield('Total years of experience');
  
  // Licensures & Certifications
  $form['licecerts'] = fieldset('Licensures/Certifications');

  $lc_fields['name'] = textfield('Licensure/Certification');
  $lc_fields['state'] = state();
  $lc_fields['number'] = textfield('License Number', 12);
  $lc_fields['expiry_date'] = date_popup('Expires');

  $form['licecerts']['lc'][] = $lc_fields;
  
  
  $form['proforgs'] = fieldset('Professional Organizations');
  $proforgs['none'] = checkbox('I am not a member of any organizations');
  $proforgs['org_name'] = textfield('Organization');
  $proforgs['org_role'] = textfield('Role');
  $proforgs['org_start'] = date_popup('From');
  $proforgs['org_end'] = date_popup('To');
  $form['proforgs']['orgs'][] = $proforgs;

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

  oa_portal_submit($form);
  return $form;
}

/**
 * Form validation callback for Application
 */
function oa_portal_p1_application_form_validate($form, &$form_state) {
}

/**
 * Form submission callback for Application
 */
function oa_portal_p1_application_form_submit($form, &$form_state) {
}


/**
 * Form callback for Checklist
 */
function oa_portal_p1_checklist_form($form_state) {
  $form = array();
  $form['#description'] = t('This profile is for use...');

  // @TODO Get items per modality
  $form['skills'] = fieldset('Essential/Practical Skills Checklist');
  $form['skills']['core'] = fieldset('Core Skills');
  $form['skills']['core']['item1'] = radios_123('Admission of a Patient');
  $form['skills']['core']['item2'] = radios_123('Transfer of a Patient');
  $form['skills']['core']['item3'] = radios_123('Discharge of a Patient');
  $form['skills']['cardio'] = fieldset('Cardiovascular');
  $form['skills']['cardio']['item1'] = radios_123('Doppler');
  $form['skills']['cardio']['item2'] = radios_123('Lead placement');

  oa_portal_submit(&$form);
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
  // @TODO Post to UCM
}


/**
 * Form callback for Professional Organizations
 */
function oa_portal_p2_professional_orgs_form($form_state) {
  $form['prof_orgs'] = fieldset('Professional Organizations');
  $form['prof_orgs']['no_orgs'] = checkbox('I am not a member of any organizations');
  $org['org_name'] = textfield('Organization');
  $org['org_role'] = textfield('Role');
  $org['org_date_from'] = date_popup('From');
  $org['org_date_to'] = date_popup('To');
  $form['prof_orgs']['org1']['#tree'] = TRUE;
  $form['prof_orgs']['org1'] = $org;
  $form['prof_orgs']['org2']['#tree'] = TRUE;
  $form['prof_orgs']['org2'] = $org;

  oa_portal_submit(&$form);
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
/*
function oa_portal_p2_professional_orgs_form_submit($form, &$form_state) {
  // @TODO Post to UCM
}
*/

/**
 * Form callback for Licensure Certifications
 */
function oa_portal_p2_licensure_certs_form($form_state) {
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");

  $form['lc'] = fieldset('Licensure/Certification');
  $form['lc']['description'] = t('Upload a copy of your licensure/certification. .pdf, .jpg and .doc are acceptable formats.');
  // @TODO Get list of Licensures
  $form['lc']['lc_licensure'] = select('Select which certificate you are uploading', array('item1', 'item2'));
  $form['lc']['lc_file'] = array(
    '#type' => 'file',
    '#title' => t('Upload file'),
    '#required' => TRUE,
    '#size' => 80,
  );
  $form['lc']['uploaded'] = markup('<h3>Upload files</h3>');
  $form['lc']['fax_preference'] = checkbox('Check this box if you prefer to fax copies of your licensures/certifications');

  // @TODO Get cover page doc ID
  $form['lc']['fax'] = markup('<PDF ICON> Please include this cover letter and fax it to (818) 555-5555. <cover page link here>')

  // @TODO Slightly different Finalize mechanism. Form to be submitted potentially more than once.
  oa_portal_submit(&$form);
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
  $form['#description'] = t('');

  // 
  $form['consent'] = fieldset('Background Investigation Consent');
  $form['consent']['maiden_name'] = textfield('Maiden/other names used');
  $form['consent']['dob'] = date_popup('Date of birth');
  $form['consent']['sin'] = textfield_sin();
  $form['consent']['dl_number'] = textfield('Driver License Number');
  $form['consent']['dl_state'] = state('Driver License State');
  $form['consent']['residence_state'] = textfield('Lived at your current address since');
  $form['consent']['disclaimer'] = markup('Note: We need 7 years of address history. Please add all addresses for the past 7 years.');
  $form['consent']['certification'] = checkbox('I certify...');
  
  $form['reference1'] = fieldset('Reference Check 1');
  
  $ref = array();
  $ref['company'] = select('Company', array('Company A', 'Company B'));
  $ref['supervisor'] = textfield('Supervisor');
  $ref['address'] = address();
  $ref['phone'] = textfield('Phone');
  $ref['email'] = email();
  $ref['certification'] = checkbox('I hereby give permission to the above named employer to release information to On Assignment regarding my performance while employed at that facility');
  
  oa_portal_submit(&$form);
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
/*
function oa_portal_p2_background_reference_form($form, &$form_state) {
}
*/

/**
 * Form callback for Capabilities Questionnaire
 */
function oa_portal_p2_capabilities_form($form_state) {
  $form = array();
  
  $form['jcq'] = fieldset('Job Capabilities Questionnaire');
  $form['jcq']['#description'] = t('As a candidate for employment...');
  $form['jcq']['performance'] = cap_yne('Can you perform all of these and similar tasks with reasonable accommodation?');
  $form['jcq']['health'] = cap_yne('Do you have any health reasons that would prohibit you from performing these or similar tasks?');
  $form['jcq']['restrictions'] = cap_yne('Do you have any restrictions preventing you from performing these tasks?');
  $form['jcq']['allergies'] = cap_yne('Do you have any known allergies?', 'If yes, list');
  $form['jcq']['allergies']['dermatitis'] = radios_yesno('Do you have dermatitis?');
  $form['jcq']['allergies']['latex'] = radios_yesno('Are you allergic to latex gloves?');
  $form['jcq']['preference'] = cap_yne('Are there any assignments that you cannot or prefer not to do?');
  $form['jcq']['certify'] = checkbox('I certify this information is correct, and understand that withholding or giving false information will result in refusal to hire or disciplinary action up to, and including, termination.');
  oa_portal_submit(&$form);
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
  
  oa_portal_submit(&$form);
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





function oa_portal_submit(&$form) {
  $form['#attributes'] = array('class' => 'oaportal');
  $form['#ajax'] = array('enabled' => TRUE);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Finalize'),
  );
  // DO something else to enable the fancy inline form submission
}


