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

  $exp_fields['employer'] = textfield('Employer', 75);
  $exp_fields['city'] = textfield('City');
  $exp_fields['state'] = state('State');
  unset($exp_fields['state']['#required'], $exp_fields['state']['#rules']);
  $exp_fields['empdatefrom'] = date_popup('Start Date', 'm-Y');
  $exp_fields['empdatefrom']['#date_year_range'] = '-40:0';
  $exp_fields['empdateto'] = date_popup('End Date', 'm-Y');
  $exp_fields['empdateto']['#date_year_range'] = '-40:0';
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
  $proforgs['org_name']['#required'] = FALSE;
  $proforgs['org_role'] = textfield('Role');
  $proforgs['org_role']['#required'] = FALSE;
  $proforgs['org_start'] = date_popup('From');
  $proforgs['org_start']['#required'] = FALSE;
  $proforgs['org_end'] = date_popup('To');
  $proforgs['org_end']['#required'] = FALSE;

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
  
  $form['organizations']['application_terms'] = terms($terms_desc);
  return $form;
}

/**
 * Form submission callback for Application
 */
function oa_portal_p1_application_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);
  $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'], $form));
  $html_string = theme('oap_pdf_wrapper', $candidate, 'Application', $body_string);
  $result = oa_portal_submitdrupalform($account->rmaxid, 2, $html_string);
  if ($result->StatusInfo->statusCode === 0) {
    $account->oa_references = array();
    foreach ($form_state['values']['experience']['exp'] AS $emp) {
      $account->oa_references[] = $emp['employer'];
    }
    foreach ($form_state['values']['education']['edu'] AS $edu) {
      $account->oa_references[] = $edu['school'];
    }
    $account = user_save($account, array('oa_references' => $account->oa_references));
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
  $candidate = get_candidate($account->rmaxid, $account->uid);
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
  $candidate = get_candidate($account->rmaxid, $account->uid);

  // Process all open formtypes
  $error = FALSE;
  foreach ($form['#formtypes_open'] AS $formtype) {
    $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'][$formtype], $form[$formtype]));
    $html_string = theme('oap_pdf_wrapper_checklist', $candidate, $form[$formtype]['#title'], $body_string);
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
  $elements = fieldset('Certified Nursing Assistant Skills Checklist');
  $skills = array(
    'Vital Measurements' => array(
      'Blood Glucose Monitoring',
      'Measure and Record I & O\'s',
      'Measure Blood Pressure',
      'Measure Orthostatic BP',
      'Measure Pulse',
      'Measure Respirations',
      'Measure Temperature ‐ Axillary',
      'Measure Temperature ‐ Oral',
      'Measure Temperature ‐ Rectal',
      'Measure Temperature ‐ Tympanic',
      ),
    'Patient Care and ADL\'s' => array(
      'Administer Enemas',
      'Bed Making Occupied',
      'Bed Making Unoccupied',
      'Bedpan',
      'Bedside Commode',
      'Care of a Combatitive Patient',
      'Care of a Confused Patient',
      'Care of a Suicidal Patient',
      'Complete Bed Bath',
      'Denture Care',
      'Foley Care',
      'Foot Care',
      'Oral Hygiene',
      'Post Mortem Care',
      'Provide Perineal Care',
      'Range of Motion',
      'Shower with Assist',
      'Whirlpool',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_485() {
  $elements = fieldset('Clerical Skills Checklist');
  $skills = array(
    'Accounting' => array(
      'Accounts Payable',
      'Accounts Receivable',
      'Assistant Bookkeeper',
      'Automated Payroll',
      'Billing',
      'Certified Public Accountant',
      'Credit/Collections',
      'Deposits',
      'Financial Statements',
      'Full Charge Bookkeeper',
      'General Ledger',
      'Inventory',
      'Manual Payroll',
      'Pegboard',
      'Posting',
      'Profit/Loss',
      'Quarterly Reports',
      'Reconcilliation',
      'Statistics',
      'Teller',
      'Trial Balance',
    ),
    'Front Office' => array(
      'Chart Analysis',
      'EMR Systems',
      'File Clerk',
      'Information Release',
      'Medical Records',
      'Reception General Office',
      'Reception Medical Office',
      'Scheduling Computerized',
      'Scheduling, Manual',
      'Switchboard Operator',
    ),
    'Billing' => array(
      'CHAMPUS',
      'CHAMPUS VA',
      'Commercial Billing',
      'Government Billing',
      'Managed Care Billing',
      'Med. Assist',
      'Medicare A',
      'Medicare B',
      'Railroad Med.',
      'Workers Comp',
    ),
    'Coding' => array(
      'CPT',
      'ICD‐9',
    ),
    'Specialties' => array(
      'Adjustment Requests',
      'Billing',
      'Cash Application',
      'Charge Entry',
      'Coordination of Benefits',
      'Credentialing',
      'Credit/Collections',
      'Customer Service',
      'EOBs',
      'Geriatrics',
      'HCFA1500',
      'Mental Health',
      'Orthopedics',
      'Pediatrics',
      'Pre Certs',
      'Radiology',
      'Receipt Entry',
      'Referrals',
      'Surgery',
      'UB 82 & 92',
      'Utilization Review',
    ),
    'Desktop Publishing' => array(
      'Pagemaker',
      'Quark',
    ),
    'Secretarial' => array(
      'Administrative',
      'Correspondence',
      'Executive',
      'Legal',
      'Medical',
      'Minutes',
      'Shorthands/Speedwriting',
      'Transcription/Dictation',
    ),
    'Data Entry' => array(
      'Alpha',
      'Keypunch/hr',
      'Numeric',
      'Reverse Keypad',
    ),
    'Office Settings' => array(
      'Chiropractic Clinic',
      'Dental Clinic',
      'General Office',
      'Home Office',
      'Hospital',
      'Insurance Company',
      'Medical Clinic',
    ),
    'Software Systems' => array(
      'DISC',
      'PACE',
      'VERSYS',
    ),
    'Spreadsheet' => array(
      'Excel',
      'Lotus',
    ),
    'Word Processing' => array(
      'PFS‐Prof. Write',
      'Word',
      'Word Perfect',
    ),
    'Contact Management/Email' => array(
      'ACT',
      'IBM AS/400',
      'IBM PC/Compatible',
      'McIntosh',
    ),
    'IBM & Computer Software Disk Operating System' => array(
      'DOS',
      'Local Area Network',
      'Microsoft Windows',
      'Windows NT',
    ),
    'Database Management' => array(
      'Access',
      'Dbase',
    ),
    'Graphics/Drawing' => array(
      'Adobe Illustrator',
      'Gemdraw',
      'Persuasion',
      'Power Point',
    ),
    'Integrated' => array(
      'Microsoft Works',
      'Q & A',
    ),
  );

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_486() {
  $elements = fieldset('Critical Care Skills Checklist');
  $skills = array(
    'Cardiovascular' => array(
      'Abdominal Aortic Aneurysm/Bypass',
      'Acute MI',
      'Angioplasty',
      'Bacterial Endocarditis',
      'Cardiac Arrest',
      'Cardiac Catheterization',
      'Carotid Endartectomy',
      'Congestive Heart Failure',
      'Continuous Cardiac Monitoring',
      'Femoral-Arterial Sheath/Removal',
      'Fem-Pop Bypass',
      'Fresh Open Heart Surgery',
      'Intra Aortic Balloon Pump',
      'Stent Placement',
      'Vascular Surgeries',
    ),
    'Pulmonary' => array(
      'ARDS',
      'Asphyxiation (Near Drowning)',
      'Aspiration',
      'COPD',
      'Endotracheal Tube',
      'Epiglottis',
      'Epistaxis',
      'Laryngeal Spasm',
      'Lobectomy',
      'Oral Airway',
      'Pneumonectomy',
      'Pneumothorax',
      'Pulmonary Edema',
      'Pulmonary Embolism',
      'Status Asthmaticus',
      'Tracheostomy',
      'Tuberculosis',
      'Ventilator/Weaning',
    ),
    'Neurological' => array(
      'Aneurysm Precautions',
      'Basal Skull Fractures',
      'Closed Head Injuries',
      'Coma',
      'Craniotomy',
      'CVA',
      'Delirium Tremors',
      'Encephalitis',
      'Heat Exhaustion',
      'Intracranial Pressure Monitoring',
      'Meningitis',
      'Multiple Sclerosis',
      'Psychiatric Disorders',
      'Seizures',
      'Spinal Cord Injury',
      'Status Epilepticus',
    ),
    'Renal/Genitourinary' => array(
      'Acute Renal Failure',
      'AV Fistula',
      'Hemodialysis',
      'Nephrostomy Tube',
      'Peritoneal Dialysis',
      'Peritoneal Lavage',
      'Renal Transplant',
      'Suprapubic Catheter',
      'TURP',
    ),
    'Endocrine/Metabolic' => array(
      'Adrenal Disorders',
      'Diabetic Ketoacidosis',
      'Hyperthyroidism',
      'Hypothyroidism',
      'Pituitary Disorders',
      'Thyroidectomy',
    ),
    'Infectious Disease' => array(
      'C. Difficile',
      'Hepatitis',
      'HIV',
      'MRSA',
      'VRE',
    ),
    'Orthopedics' => array(
      'Amputation',
      'Brace/Splint Application',
      'Crutchfield Tongs',
      'Fracture/Dislocation',
      'Halo Traction',
      'Osteoporosis',
      'Pinned Fractures',
      'Total Joint Replacements',
    ),
    'Wound Management' => array(
      'Burns',
      'Gunshot Wound',
      'Lacerations',
      'Multiple Abdominal Wounds & Drains',
      'Stab Wound',
    ),
    'Shock' => array(
      'Anaphylactic',
      'Cardiogenic',
      'Hypovolemic',
      'Neurogenic',
      'Septic',
    ),
    'Pacemaker' => array(
      'External',
      'Permanent',
      'Temporary',
      'Transthoracic',
    ),
    'Hemodynamic Monitoring' => array(
      'Cardiac Index',
      'Cardiac Output',
      'CVP',
      'MAP',
      'Swan Ganz',
      'Wedge Pressures',
    ),
    'Gastrointestinal' => array(
      'Abdominal Trauma',
      'Acute Abdominal Pain',
      'Acute Cholecystitis',
      'Bowel Obstruction',
      'Cirrhosis',
      'Gastric Lavage',
      'Gastrostomy Tube',
      'GI Bleed',
      'GI Surgery',
      'Jejunostomy Tube',
      'Liver Failure',
      'Liver Transplant',
      'Nasogastric Tube',
      'Pancreatitis',
      'PEG Tube',
      'Salem Sump',
    ),
    'Intravenous Therapy' => array(
      'Broviac',
      'Central Line',
      'Groshong',
      'Hickman',
      'Portacath',
    ),
    'Oncology' => array(
      'Bone Marrow Transplants',
      'Chemotherapy',
      'Leukemia',
      'Lymphoma',
      'Radiation Implants',
    ),
    'Medical Therapy' => array(
      'Adenosine',
      'Aminophylline',
      'Antibiotics',
      'Anticoagulants',
      'Anticonvulsants',
      'Antihypertensives',
      'Atropine',
      'Charcoal',
      'Chemotherapy',
      'Cordarone',
      'Diuretics',
      'Dopamine',
      'EMR Systems',
      'Epinephrine',
      'Inocor',
      'Inotropes',
      'Insulin Infusions',
      'Nitroglycerine',
      'Steroids',
      'TPA',
      'Vasopressin',
    ),
    'OB/GYN (ER ONLY)' => array(
      'Placenta Previa',
      'Precipitous Delivery',
      'Rape Victim',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_487() {
  $elements = fieldset('Cytology Skills Checklist');
  $skills = array(
    'Non-Gyn' => array(
      'Asetic Fluid',
      'Bronchial',
      'Esophageal',
      'Gastric',
      'Spinal',
      'Urine',
    ),
    'Fixations' => array(
      '95% Alcohol',
      'Carbowax',
      'Cytolyt',
      'Filter Prep',
      'Saccomono',
    ),
    'Thin Prep' => array(
      'Gyn',
      'Non‐Gyn',
      'Preventative Maintenance',
    ),
    'Fine Needle Aspirations' => array(
      'Assist',
      'Collection',
      'Make Smears',
      'Onsite Procedures',
      'Preliminary Diagnosis',
      'Staining',
    ),
    'Special Stains/Organisms' => array(
      'GMS',
      'Iron',
      'PAP',
    ),
    'Other Skills' => array(
      'DNA Ploidy',
      'EMR Systems',
      'FISH',
      'Flow Cytometry',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_488() {
  $elements = fieldset('Dental Assistant Skills Checklist');
  $skills = array(
    'Front Office' => array(
      'Appointments',
      'Chart Analysis',
      'EMR Systems',
      'Filing',
      'Reception General Office',
      'Reception Medical Office',
      'Scheduling Computerized',
      'Scheduling, Manual',
    ),
    'Proficient In' => array(
      'Administer Fluoride',
      'Amalgam',
      'Apply bleaching agents',
      'Case Histories',
      'Cement Removal',
      'Cerec',
      'Communicating with Dentist',
      'Composites',
      'Crown Prep',
      'Digital X‐Rays',
      'Extraction Procedures',
      'Extraction Set Ups',
      'Flouride Treatments',
      'Impressions',
      'Lab Work',
      'Perform Sterlization Procedures',
      'Polishes',
      'Polishing of Restoration',
      'Pour & Shape Impressions & Models',
      'Root Canal',
      'Sealants',
      'Suture Removal',
      'Taking Adequate Medical History',
      'Taking Adequate X‐Rays',
    ),
    'Specialty' => array(
      'Endodontic',
      'General',
      'Oral Surgery',
      'Orthodontic',
      'Pediatrics',
      'Pedodontic',
      'Periodontic',
      'Prosthodontics',
    ),
    'X-Rays' => array(
      'Bitewing',
      'Digital',
      'Panorex',
      'Periapical',
    ),
  );

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_489() {
  $elements = fieldset('Dental Hygienist Skills Checklist');
  $skills = array(
    'Proficient In' => array(
      'Ability to Identify & Treat Gum Diesease',
      'Administer Fluoride',
      'Amalgam',
      'Case Histories',
      'Charting',
      'Communicating with Dentist',
      'Digital X‐Rays',
      'Local Anesthesia',
      'Monitor Nitrous Oxide',
      'Nitrous Oxide',
      'Oral Health Communication with patient',
      'Perform Sterlization Procedures',
      'Periodontal Charting',
      'Piezo Electric',
      'Polishing of Restoration',
      'Preliminary Charting',
      'Probing',
      'Prophylaxis',
      'Recall Appointments',
      'Removing Marginal Overhangs',
      'Root Planning',
      'Sealants',
      'Taking Adequate Medical History',
      'Taking Adequate X‐Rays',
      'Ultrasonic Scaler',
    ),
    'Specialty' => array(
      'Endodontic',
      'General',
      'Oral Surgery',
      'Orthodontic',
      'Pediatrics',
      'Pedodontic',
      'Periodontic',
      'Prosthodontics',
    ),
    'X-Rays' => array(
      'Bitewing',
      'Digital',
      'Panorex',
      'Periapical',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

// 490 - Echo Tech Skills - likely out of scope

function oa_portal_p1_checklist_form_491() {
  $elements = fieldset('Endoscopy Skills Checklist');
  $skills = array(
    'Cardiovascular' => array(
      'Assessment - Ausculation (rate, rhythm)',
      'Assessment - Blood pressure/non-invasive',
      'Assessment - Heart sounds/murmurs',
      'Assessment - Perfusion',
      'Assessment - pulses/circulation',
      'Assist with Code',
      'Interpret - Arterial Blood Gasses',
      'Monitoring - EKG Interpretation',
      'Perform - Cardiac resuscitation',
      'Perform - Emergency defibrillation',
      'Perform - setup and run 12-lead EKG',
    ),
    'Pulmonary' => array(
      'Airway Management - Endotracheal/suctioning',
      'Airway Management - Endotracheal/suctioning - in-line',
      'Airway Management - Endotracheal/suctioning - Open ET catheter',
      'Airway Management - Nasal/suctioning',
      'Airway Management - Oropharyngeal/suctioning',
      'Airway Management - Tracheostomy/suctioning',
      'Assessment - Adventitious breath sounds',
      'Assessment - Breath sounds',
      'Assessment - Rate and work of breathing',
      'Assessment- Respiratory Complications',
      'Assist with Bronchoscopy',
      'Assist with Emergency tracheostomy',
      'Assist with setup/insertion of Chest Tube',
      'Care of child - Foreign body aspiration',
      'Care of child - Respiratory Failure',
      'Care of child - Status Asthmaticus',
      'Care of Intubated Patient',
      'Care of patient - Aspiration',
      'Care of patient - Bronchoscopy',
      'Care of patient - Laryngospasm',
      'Care of patient - Pulmonary embolism',
      'Care of patient - Status asthmaticus',
      'Chest Tube - Use of Pleurevac or Thoraclex',
      'Chest Tube - Use of Water seal drainage system',
      'Establishing an airway',
      'Management - Endotracheal/suctioning - Open ET Catheter',
      'Oxygen therapy and medication delivery via Bag (anesthesia) and mask',
      'Oxygen therapy and medication delivery via Bag (self-inflating) and mask',
      'Oxygen therapy and medication delivery via Bag and mask',
      'Oxygen therapy and medication delivery via Bulb syringe',
      'Oxygen therapy and medication delivery via CPAP (nasal prongs)',
      'Oxygen therapy and medication delivery via ET tube',
      'Oxygen therapy and medication delivery via External CPAP',
      'Oxygen therapy and medication delivery via Face mask',
      'Oxygen therapy and medication delivery via Inhalers',
      'Oxygen therapy and medication delivery via Isolette',
      'Oxygen therapy and medication delivery via Nasal canula',
      'Oxygen therapy and medication delivery via Nebulizer',
      'Oxygen therapy and medication delivery via Oral airway',
      'Oxygen therapy and medication delivery via Oxyhood',
      'Oxygen therapy and medication delivery via Portable O2 tank',
      'Oxygen therapy and medication delivery via Tent',
      'Oxygen therapy and medication delivery via T-piece',
      'Oxygen therapy and medication delivery via Trach collar',
      'Oxygen therapy and medication delivery via Transtracheal cannulation',
      'Perform - Obtain sputum specimen',
      'Pulse Oximetry',
      'Teach and understand Incentive spirometry',
      'Ventilate with Ambu-Bag',
    ),
    'Gastrointestinal' => array(
      'Assessment - Abdominal/bowel sounds',
      'Care of patient - Colostomy',
      'Care of patient - Ileostomy',
      'Interpret - blood chemistry',
      'Perform - Administer Tube Feeding',
      'Perform - NG and sump tubes to suction',
      'Perform - placement of NG tube',
      'Perform - Test for occult blood',
    ),
    'Pain Management' => array(
      'Assessment - Pain level/tolerance',
      'Assist with delivery of anesthesia - Coaching patient',
      'Assist with delivery of anesthesia - Documentation of anesthesia',
      'Assist with delivery of anesthesia - Intravascular injection',
      'Assist with delivery of anesthesia - Positioning patient',
      'Care of child - IV Conscious Sedation',
      'Care of patient - IV Conscious Sedation',
      'Medications- Conscious Sedation Reversal Medications',
    ),
    'Endoscopic' => array(
      'Bronchoscopy',
      'Colonoscopy',
      'Esophagoscopy',
      'Gastroscopy',
      'Sigmoidoscopy',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Assessment - Site and patency',
      'Equipment - IV Pump',
      'Perform - Draw venous blood',
      'Perform - Rate calculation',
      'Perform - start IV (angiocath)',
      'Perform - start IV (butterfly)',
      'Perform - start IV (heparin lock)',
    ),
    'JCAHO/OSHA' => array(
      'Handwashing',
      'Isolation Techniques',
      'Understand JCAHO National Patient Safety Goals',
      'Understand JCAHO Prohibited Abbreviations',
      'Universal Precautions',
      'Use of Restraints',
    ),
  );

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_492() {
  $elements = fieldset('ER Skills Checklist');
  $skills = array(
    'Cardiovascular' => array(
      'Assessment ‐ Ausculation (rate, rhythm)',
      'Assessment ‐ Doppler',
      'Assessment ‐ Heart sounds/murmurs',
      'Assist with Arterial line insertion',
      'Assist with Central line insertion',
      'Assist with setup/insertion of Cardioversion',
      'Assist with setup/insertion of PA catheter/Swan‐Ganz',
      'Assist with setup/insertion of Pacemaker',
      'Care of patient ‐ Acute MI',
      'Care of patient ‐ AICD',
      'Care of patient ‐ Aneurysm',
      'Care of patient ‐ Angina',
      'Care of patient ‐ Cardiac Arrest',
      'Care of patient ‐ Congestive Heart Failure',
      'Care of patient ‐ Myocarditis',
      'Care of patient ‐ Pericardiocentesis',
      'Medications ‐ Adenosine',
      'Medications ‐ Amiodarone',
      'Medications ‐ Atropine',
      'Medications ‐ Bicarbonate',
      'Medications ‐ Bretylium (Bretyol)',
      'Medications ‐ Cardizem',
      'Medications ‐ Digoxin',
      'Medications ‐ Dobutamine',
      'Medications ‐ Dopamine',
      'Medications ‐ Epinepherine',
      'Medications ‐ Esmolol',
      'Medications ‐ Lasix',
      'Medications ‐ Lidocaine',
      'Medications ‐ Nipride',
      'Medications ‐ Nitroglycerin',
      'Medications ‐ Procainamide',
      'Medications ‐ Sodium Bicarbonate',
      'Medications ‐ Thrombolytic therapy',
      'Medications ‐ Verapamil',
      'Monitoring ‐ A‐line (radial)',
      'Monitoring ‐ Arrhythmia interpretation',
      'Monitoring ‐ Basic 12 lead interpretation',
      'Monitoring ‐ CVP',
    ),
    'Pulmonary' => array(
      'Airway Management ‐ Endotracheal/suctioning',
      'Airway Management ‐ Nasal/suctioning',
      'Airway Management ‐ Oropharyngeal/suctioning',
      'Airway Management ‐ Tracheostomy/suctioning',
      'Apnea monitor',
      'Assessment ‐ Breath sounds',
      'Assessment ‐ Rate and work of breathing',
      'Assist with extubation',
      'Assist with intubation',
      'Assist with thoracentisis',
      'Care of patient ‐ Aspiration',
      'Care of patient ‐ COPD',
      'Care of patient ‐ Hemopneumothorax',
      'Care of patient ‐ Laryngospasm',
      'Care of patient ‐ on ventilator',
      'Care of patient ‐ Pneumonia',
      'Care of patient ‐ Pneumothorax',
      'Care of patient ‐ Pulmonary edema',
      'Care of patient ‐ Pulmonary embolism',
      'Care of patient ‐ Tension pneumothorax',
      'Care of patient ‐ Tuberculosis',
      'Chest Tube ‐ Use of Pleurevac or Thoraclex',
      'Chest Tube ‐ Use of Water seal drainage system',
      'Interpret ‐ Arterial Blood Gases',
      'Obtain ABG\'s ‐ Arterial line',
      'Obtain ABG\'s ‐ Femoral artery',
      'Obtain ABG\'s ‐ Radial artery',
      'Oxygen therapy and medication delivery via Bag and mask',
      'Oxygen therapy and medication delivery via ET tube',
      'Oxygen therapy and medication delivery via External CPAP',
      'Oxygen therapy and medication delivery via Face mask',
      'Oxygen therapy and medication delivery via Inhalers',
      'Oxygen therapy and medication delivery via Nasal canula',
      'Oxygen therapy and medication delivery via Nebulizer',
      'Oxygen therapy and medication delivery via Portable O2 tank',
      'Oxygen therapy and medication delivery via T‐piece',
      'Oxygen therapy and medication delivery via Trach collar',
      'Perform ‐ Obtain sputum specimen',
      'Pulse Oximetry',
    ),
    'Neurological' => array(
      'Assessment ‐ Glasgow coma scale',
      'Assessment ‐ Level of consciousness',
      'Assessment ‐ Reflex/motor deficits',
      'Assessment ‐ visual or communication deficits',
      'Assist with lumbar puncture',
      'Care of patient ‐ Basal skull fracture',
      'Care of patient ‐ Closed head injury',
      'Care of patient ‐ CVA',
      'Care of patient ‐ DT\'s',
      'Care of patient ‐ Encephalitis',
      'Care of patient ‐ External VP shunts',
      'Care of patient ‐ Meningitis',
      'Care of patient ‐ Neuromuscular disease',
      'Care of patient ‐ Overdose',
      'Care of patient ‐ Seizures',
      'Care of patient ‐ Spinal cord injury',
      'ICP Management ‐ Medications',
      'ICP Management ‐ Monitoring',
      'ICP Management ‐ Positioning',
      'ICP Management ‐ Regulation of ICP',
      'ICP Management ‐ Temperature control',
      'ICP Management ‐ Ventilation',
      'Medications ‐ Anticonvulsants',
    ),
    'Gastrointestinal' => array(
      'Assessment ‐ Abdominal/bowel sounds',
      'Assessment ‐ Fluid balance',
      'Assessment ‐ Nutrition',
      'Care of patient ‐ Abdominal trauma',
      'Care of patient ‐ Bowel Obstruction',
      'Care of patient ‐ GI bleeding',
      'Care of patient ‐ Hepatitis',
      'Care of patient ‐ Liver failure',
      'Interpret ‐ blood chemistry',
      'Medications ‐ Antiemetics',
      'Medications ‐ Antispasmodic',
      'Medications ‐ Charcoal',
      'Medications ‐ Ipecac',
      'Perform ‐ placement of NG tube',
      'Perform ‐ Salem pump to suction',
      'Perform ‐ Saline lavage',
    ),
    'Renal/Genitourinary' => array(
      'Assessment ‐ Fluid and Electorlyte balance',
      'Care of patient ‐ Acute Renal Failure',
      'Care of patient ‐ Peritoneal lavage',
      'Care of patient ‐ Renal trauma',
      'Care of patient ‐ Urinary tract infection',
      'Interpret ‐ BUN and Creatinine',
      'Interpret ‐ Serum electrolytes',
      'Perform ‐ Insert and Care for Foley',
      'Perform ‐ Urine specimen collection (routine)',
    ),
    'Endocrine/Metabolic' => array(
      'Assessment ‐ S/S diabetic coma',
      'Assessment ‐ S/S insulin reaction',
      'Care of patient ‐ Diabetic Ketoacidosis',
      'Interpret ‐ Blood Glucose',
      'Medications ‐ Insulin',
      'Medications ‐ Oral hypoglycemics',
      'Perform ‐ Blood Glucose Monitoring: Finger Stick',
    ),
    'EENT' => array(
      'Assessment ‐ Visual acuity',
      'Perform ‐ application of eye patch',
      'Perform ‐ Ear irrigation',
      'Perform ‐ Eye irrigation',
      'Perform ‐ Morgan lens irrigation',
      'Perform ‐ Nasal packing',
      'Perform ‐ removal of contact lens',
      'Perform ‐ Setup fluorescent/Woods lamp exam',
    ),
    'Trauma' => array(
      'Assessment ‐ Burns ‐ Dehydration',
      'Assessment ‐ Burns ‐ Rule of nines',
      'Assessment ‐ Champion trauma score',
      'Assessment ‐ Poison index',
      'Assessment ‐ Triage',
      'Burns',
      'Care of patient ‐ Bites, animal',
      'Care of patient ‐ Bites, human',
      'Care of patient ‐ Bites, venomous snake',
      'Care of patient ‐ Bites, venomous spider',
      'Care of patient ‐ Burns, 1st degree',
      'Care of patient ‐ Burns, 2nd degree',
      'Care of patient ‐ Burns, 3rd degree',
      'Care of patient ‐ Electrocution',
      'Care of patient ‐ Gunshot/stab wound',
      'Care of patient ‐ Hazardous material exposure',
      'Care of patient ‐ Heat exhaustion/stroke',
      'Care of patient ‐ Hypothermia',
      'Care of patient ‐ Major trauma',
      'Care of patient ‐ Minor trauma',
      'Care of patient ‐ Radiation exposure',
      'Care of patient ‐ Shock, Anaphylactic',
      'Care of patient ‐ Shock, Cardiogenic',
      'Care of patient ‐ Shock, Hypovolemic',
      'Care of patient ‐ Shock, Neurogenic',
      'Care of patient ‐ Shock, Septic',
      'Care of patient ‐ Traumatic amputation',
      'Perform ‐ Air transport of trauma patient',
      'Perform ‐ Application of mast suit',
      'Perform ‐ Ground transport',
    ),
    'Infectious Disease' => array(
      'Care of patient ‐ AIDS',
      'Interpret ‐ CBC/differential',
      'Perform ‐ Fever Management',
      'Perform ‐ Isolation',
    ),
    'Woman\'s Health' => array(
      'Assessment ‐ Assist with pelvic exam',
      'Assessment ‐ Recognize and report acts of violence',
      'Care of patient ‐ Abruptio placenta',
      'Care of patient ‐ DIC',
      'Care of patient ‐ Hemorrhage',
      'Care of patient ‐ Placenta Previa',
      'Care of patient ‐ Precipitous Delivery',
      'Care of patient ‐ Preeclampsia/eclampsia',
      'Care of patient ‐ spontaneous abortion',
      'Perform ‐ Setup of pelvic tray',
      'Perform ‐ Setup of rape kit',
    ),
    'Orthopedics' => array(
      'Assessment ‐ Circulation Checks',
      'Assessment ‐ Gait',
      'Assessment ‐ Range of Motion',
      'Assessment ‐ Skin',
      'Assist with placement of cast',
      'Care of patient ‐ Ankle brace',
      'Care of patient ‐ Ankle splint',
      'Care of patient ‐ Cast',
      'Care of patient ‐ Knee immobilizer',
      'Care of patient ‐ Pinned fractures',
      'Care of patient ‐ Wrist splint',
      'Equipment ‐ Support Device ‐ Cane',
      'Equipment ‐ Support Device ‐ Cervical collar',
      'Equipment ‐ Support Device ‐ Sling',
      'Equipment ‐ Support Device ‐ Transfer board',
    ),
    'Wound Management' => array(
      'Assist with staples',
      'Assist with sutures',
      'Perform ‐ Application of steristrips',
      'Perform ‐ setup culdocentesis tray',
      'Perform ‐ setup suture tray',
      'Perform ‐ staple removal',
      'Perform ‐ suture removal',
    ),
    'Pain Management' => array(
      'Assessment ‐ Pain level/tolerance',
      'Care of child ‐ Epidural anesthesia/analgesia',
      'Care of patient ‐ Epidural anesthesia/analgesia',
      'Care of patient ‐ IV Conscious Sedation',
    ),
    'Neonatal/Pediatrics' => array(
      'Assessment ‐ S/S child abuse and neglect',
      'Care of child ‐ Epiglottitis',
      'Care of child ‐ Near drowning',
      'Care of child ‐ Overdose/poison ingestion',
      'Care of child ‐ Status asthmaticus',
      'Care of child ‐ Status epilepticus',
      'Perform ‐ Obtain consent to treat',
      'Perform ‐ Pediatric CPR/Code',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Assist with cutdown',
      'Care of patient ‐ angiography',
      'Care of patient ‐ Broviac',
      'Care of patient ‐ Groshong',
      'Care of patient ‐ Hickman',
      'Care of patient ‐ Peripheral line/dressing',
      'Care of patient ‐ Portacath',
      'Perform ‐ Administer blood products ‐ autotransfusion',
      'Perform ‐ Administer Cryoprecipitate',
      'Perform ‐ administer Packed RBC\'s',
      'Perform ‐ administer Plasma/Albumin',
      'Perform ‐ administer Whole blood',
      'Perform ‐ Draw venous blood',
      'Perform ‐ start IV (angiocath)',
      'Perform ‐ start IV (butterfly)',
      'Perform ‐ start IV (heparin lock)',
    ),
    'Miscellaneous' => array(
      'AMA procedures',
      'Suicide precautions',
    ),
  );
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
  $elements = fieldset('Histology Skills Checklist');
  $skills = array(
    'Skills' => array(
      'Assist with Frozen Sectioning',
      'Assist with Gross Sectioning',
      'Bone Marrow Biopsy & Slides',
      'Collection of Tissue Specimens',
      'Concept of Fixation',
      'Coverslipping',
      'Decalcify Bone & Hard Tissue',
      'Electronic Medical Records Systems',
      'Embed Tissue',
      'Filing of Slides & Blocks',
      'Microtome & Disposable Blades',
      'Preparation of Stains/Reagents',
      'Principle of Hematoxylin & Eosin Stain',
      'Proper Labeling of Reagents',
      'Quality Control & Regulation',
      'Running Tissue Processor',
      'Special Stain Techniques: Group I',
      'Special Stain Techniques: Group II',
      'Special Stain Techniques: Group III',
      'Special Stain Techniques: Group IV',
      'Special Stain Techniques: Group V',
      'Storage Time of Wet Tissues & How to Dispose of Them',
      'Universal Histology Precautions',
    ),
    'Cytology' => array(
      'FNA Collections',
      'GYN Prep/Staining',
      'Non‐Gyn Prep/Staining',
    ),
  );
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
  $elements = fieldset('Labor and Delivery Skills Checklist');
  $skills = array(
    'Neurological' => array(
      'Medications - Anticonvulsants',
    ),
    'Ante partum' => array(
      'Assessment - Assess for comfort',
      'Assessment - Breathing/relaxing techniques',
      'Assessment - Coaching',
      'Assessment - Positioning',
      'Assist with sonogram',
      'Interpret - Amniotic fluid index',
      'Interpret - Biophysical profile',
      'Perform - Delivery table setup',
      'Perform - Foley and Straight Catheter insertion',
      'Perform - Sonogram',
    ),
    'Labor Assessment' => array(
      'Assessment - Auscultate fetal heart rate - Doppler',
      'Assessment - Auscultate fetal heart rate - Fetoscope',
      'Assessment - Determine fetal position',
      'Assessment - Document FHR patterns',
      'Assessment - FHR - Baseline',
      'Assessment - FHR - Early decelerations',
      'Assessment - FHR - Late decelerations',
      'Assessment - FHR - Prolonged decelerations',
      'Assessment - FHR - Variable decelerations',
      'Assessment - FHR - Variablity',
      'Assessment - Maternal - Deep Tendon Reflexes (DTR\'s)',
      'Assessment - Maternal - Edema',
      'Assessment - Maternal - Norms for perinatal vital signs',
      'Assessment - Maternal - Perform admission risk assessment',
      'Assessment - Maternal - Presence of clonus',
      'Assessment - Maternal - Progression of Labor - Contraction characteristics',
      'Assessment - Maternal - Progression of Labor - Dilation',
      'Assessment - Maternal - Progression of Labor - Effacement',
      'Assessment - Maternal - Progression of Labor - Fetal presentation',
      'Assessment - Maternal - Progression of Labor - Station',
      'Assessment - Maternal - Progression of Labor - Status of Membranes',
      'Assessment - Maternal - Progression of Labor - Sterile speculum exam',
      'Assessment - Maternal - Progression of Labor - Vaginal exam',
      'Assessment - Rupture of Membranes - Fern Test',
      'Assessment - Rupture of Membranes - Nitrazine',
      'Medications - Assist with Prostin gel',
      'Medications - Administer/Monitor IV Drips - Antibiotics',
      'Medications - Administer/Monitor IV Drips - Antihypertensives',
      'Medications - Administer/Monitor IV Drips - Heparin',
      'Medications - Administer/Monitor IV Drips - Magnesium Sulfate',
      'Medications - Administer/Monitor IV Drips - Narcotics',
      'Medications - Administer/Monitor IV Drips - Oxytocin',
      'Medications - Cervidil insertion',
      'Medications - Labor suppressants',
      'Medications - Use of Cytotec',
      'Medications - Use of prostin suppositories',
      'Perform - Amnioinfusion for meconium',
      'Perform - Amnioinfusion for variable decelerations',
      'Perform - Assist with artificial rupture of membranes - prolapsed cord',
      'Perform - Assist with artificial rupture of membranes - recognize potential complications',
      'Perform - Assist with artificial rupture of membranes - vesa previa',
      'Perform - Collect blood/urine specimens',
      'Perform - Collect vaginal cultures - Chlamydia',
      'Perform - Collect vaginal cultures - Fluid',
      'Perform - Collect vaginal cultures - Group B Strep',
      'Perform - Collect vaginal cultures - Herpes',
      'Perform - Document labor status',
      'Perform - External Fetal Monitor - Doppler',
      'Perform - External Fetal Monitor - Phono or abdominal transducer',
      'Perform - External Fetal Monitor - Tocotransducer, ultrasound',
      'Perform - IM/SC Injections',
      'Perform - Internal Fetal Monitor - Intrauterine pressure catheter - Fluid Filled',
      'Perform - Internal Fetal Monitor - Intrauterine pressure catheter - Spiral electrode',
      'Perform - Internal Fetal Monitor - Intrauterine pressure catheter - Transducer tipped',
      'Perform - Leopold\'s maneuvers',
      'Perform - Toxicology studies',
    ),
    'Complication of Pregnancy' => array(
      'Assessment - Identify common cardiac arrhythmias',
      'Assessment - Identify normal cardiac rhythms',
      'Care of patient - Abruptio placenta',
      'Care of patient - Asthma',
      'Care of patient - Cardiac disease',
      'Care of patient - Chorioamnioitis',
      'Care of patient - Chronic hypertension',
      'Care of patient - Collagen vascular disease',
      'Care of patient - Diabetes',
      'Care of patient - Eclampsia',
      'Care of patient - HBV',
      'Care of patient - HELLP Syndrome',
      'Care of patient - Hemolytic anemia',
      'Care of patient - Hemorrhage',
      'Care of patient - HIV Positive',
      'Care of patient - Hypertension',
      'Care of patient - Malpresentation',
      'Care of patient - Multiple gestation',
      'Care of patient - Other infections',
      'Care of patient - Placenta previa',
      'Care of patient - Preeclampsia',
      'Care of patient - Premature labor',
      'Care of patient - Pyelonephritis',
      'Care of patient - RH disease',
      'Care of patient - Sickle cell disease',
      'Medications - Indomethacin',
      'Medications - Insulin',
      'Medications - Magnesium Sulfate',
      'Medications - Procardia',
      'Medications - Ritodrine',
      'Medications - Terbutaline',
      'Monitoring - Central venous lines',
      'Monitoring - Invasive hemodynamic monitoring',
      'Monitoring - Patients with General Anesthsia',
      'Monitoring - Patients with Regional Anesthesia - Epidural',
      'Monitoring - Patients with Regional Anesthesia - Local infiltration',
      'Monitoring - Patients with Regional Anesthesia - Spinal',
      'Monitoring - PICC lines',
      'Monitoring - Pulmonary artery catheters',
      'Perform - Assist with external version',
      'Perform - Assist with fetal scalp sampling',
      'Perform - Assist with forceps vaginal delivery',
      'Perform - Assist with percutaneous umbilical sampling',
      'Perform - Assist with spontaneous vaginal delivery',
      'Perform - Assist with umbilical blood sampling',
      'Perform - Assist with vacuum extraction delivery',
      'Perform - Circulate for Cesarean delivery',
      'Perform - Circulate, scrub for bilateral tubal ligation',
      'Perform - Conduct contraction stress test - breast stimulation',
      'Perform - Conduct contraction stress test - Oxytocin challenge',
      'Perform - Conduct non-stress test - Stimulate fetus',
      'Perform - Conduct non-stress test - Vibroacoustic samples',
      'Perform - Glucose reflectometer',
      'Perform - Patient education on fetal movement counts',
      'Perform - Scrub for Cesarean section',
      'Perform - Setup for Cesarean section',
    ),
    'Infant Post Delivery' => array(
      'Assessment - APGAR',
      'Assessment - Ballard',
      'Assessment - Dubowitz',
      'Assessment - Finnegan scoring',
      'Assessment - Initial vital signs',
      'Assessment - Risk Factors - IDM',
      'Assessment - Risk Factors LGA, SGA, IUGR',
      'Medications - Eye prophylaxis',
      'Medications - Vitamin K',
      'Perform - Assist with initial breast feeding',
      'Perform - Assist with meconium staining',
      'Perform - Bath and teach',
      'Perform - Cardiac respiratory monitor placement',
      'Perform - Circumcision care',
      'Perform - Cord care',
      'Perform - Discharge teaching',
      'Perform - Heel stick glucose determination',
      'Perform - Infant identification',
      'Perform - Neonatal resuscitation',
      'Perform - Obtain hematocrit',
      'Perform - Obtain neonatal toxicology screen',
      'Perform - Phototherapy',
      'Perform - Promote bonding behaviors',
      'Perform - Suctioning - bulb',
      'Perform - Suctioning - delee',
      'Perform - Suctioning - wall',
    ),
    'Post Partum' => array(
      'Assessment - Bladder distention',
      'Assessment - Breast feeding - Latch on',
      'Assessment - Breast feeding - Positioning',
      'Assessment - DVT',
      'Assessment - Episiotomy',
      'Assessment - Fluid Balance',
      'Assessment - Fundal height',
      'Assessment - Locia amount',
      'Assessment - Maternal Vital Signs',
      'Perform - Fundal massage',
    ),
    'Pain Management' => array(
      'Assessment - Pain level/tolerance',
      'Assist with delivery of anesthesia - Anesthesia toxicity',
      'Assist with delivery of anesthesia - Coaching patient',
      'Assist with delivery of anesthesia - Documentation of anesthesia',
      'Assist with delivery of anesthesia - Epidural block',
      'Assist with delivery of anesthesia - Fluid challenge',
      'Assist with delivery of anesthesia - Hypotension',
      'Assist with delivery of anesthesia - Intrathecal narcotics',
      'Assist with delivery of anesthesia - Intravascular injection',
      'Assist with delivery of anesthesia - Positioning patient',
      'Assist with delivery of anesthesia - S/S of dural puncture',
      'Assist with delivery of anesthesia - Spinal anesthesia',
      'Care of child - Epidural anesthesia/analgesia',
      'Care of patient - IV Conscious Sedation',
      'Care of patient - Patient Controlled Analgesia (PCA)',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Care of patient - Central line/catheter/dressing',
      'Care of patient - Peripheral line/dressing',
      'Perform - Administer Cryoprecipitate',
      'Perform - administer Packed RBC\'s',
      'Perform - administer Plasma/Albumin',
      'Perform - administer Whole blood',
      'Perform - Draw blood from central line',
      'Perform - Draw venous blood',
      'Perform - start IV (angiocath)',
      'Perform - start IV (butterfly)',
      'Perform - start IV (heparin lock)',
    ),
  );
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
  $elements = fieldset('Medical Assistant Skills Checklist');
  $skills = array(
    'Orthopedics' => array(
      'Equipment ‐ Support Device ‐ Wheel Chair',
    ),
    'Emergencies' => array(
      'Assist physician with patient ‐ Bleeding',
      'Assist physician with patient ‐ Burns',
      'Assist physician with patient ‐ Cardiac/Respiratory arrest',
      'Assist physician with patient ‐ Choking',
      'Assist physician with patient ‐ Fracture',
      'Assist physician with patient ‐ Poisoning',
    ),
    'Lab Tests' => array(
      'Perform ‐ Capillary blood collection',
      'Perform ‐ Hearing screen',
      'Perform ‐ Pregnancy test',
      'Perform ‐ Sputum collection',
      'Perform ‐ Stool collection',
      'Perform ‐ Throat swab',
      'Perform ‐ Urinalysis',
      'Perform ‐ Vaginal swab',
      'Perform ‐ Venipuncture',
      'Perform ‐ Vision test',
      'Perform ‐ Wound swab',
    ),
    'Body Systems' => array(
      'Cardiovascular system',
      'Digestive system',
      'Endocrine system',
      'Nervous system',
    ),
    'General' => array(
      'Perform ‐ Administer a nebulizer treatment',
      'Perform ‐ Administer oxygen',
      'Perform ‐ Measure vital signs',
      'Perform ‐ Setup cast supplies',
      'Perform ‐ Use a stethoscope',
      'Perform ‐ Weight measurement',
    ),
    'Body Systems' => array(
      'Reproductive system',
      'Respiratory system',
      'Urinary system',
    ),
    'Office Duties' => array(
      'Perform ‐ Answer phone',
      'Perform ‐ Coding',
      'Perform ‐ Invoicing',
      'Perform ‐ Medical billing',
      'Perform ‐ Medical filing',
      'Perform ‐ Medical record organization',
      'Perform ‐ Order supplies',
      'Perform ‐ Schedule patient appointment',
      'Perform ‐ Transcription',
    ),
    'Venipuncture Methods/Procedures' => array(
      'Perform ‐ Basic and specialized specimen handling',
      'Perform ‐ Specimen identification and labeling',
    ),
    'Monitor/Chart' => array(
      'Document care ‐ computer',
    ),
    'Age Groups' => array(
      '12‐18 years old (Adolescents)',
      '1‐3 years old (Toddler)',
      '18‐39 years old (Young Adults)',
      '3‐5 years old (Preschool)',
      '39‐64 years old (Middle Adults)',
      '5‐12 years old (School Age Children)',
      '64+ years old (Older Adults)',
    ),
  );
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
  $elements = fieldset('NICU Skills Checklist');
  $skills = array(
    'Cardiovascular' => array(
      'Assessment - Ausculation (rate, rhythm, volume)',
      'Assessment - Blood pressure/invasive (A-line)',
      'Assessment - Blood pressure/non-invasive',
      'Assessment - Heart sounds/murmurs',
      'Assessment - Perfusion',
      'Assessment - pulses/circulation',
      'Care of neonate - Cardiac arrest',
      'Care of neonate - Cardiac transplant',
      'Care of neonate - Cardiomyopathy',
      'Care of neonate - Congenital Heart defects',
      'Care of neonate - Hemodynamic instability',
      'Care of neonate - Hypovolemic shock',
      'Care of neonate - Post Cardiac surgery',
      'Care of neonate - Post Invertentional cardiac cath',
      'Medications - Dobutamine',
      'Medications - Dopamine',
      'Medications - Epinepherine',
      'Medications - Nipride',
      'Medications - Sodium Bicarbonate',
      'Monitoring - CVP',
      'Monitoring - EKG Interpretation',
      'Perform - Cardiac resuscitation',
      'Perform - Emergency defibrillation',
    ),
    'Pulmonary' => array(
      'Airway Management - Endotracheal/suctioning',
      'Airway Management - Endotracheal/suctioning - in-line',
      'Airway Management - Endotracheal/suctioning - Open ET catheter',
      'Airway Management - Nasal/suctioning',
      'Airway Management - Oropharyngeal/suctioning',
      'Airway Management - Tracheostomy/suctioning',
      'Apnea monitor',
      'Assessment - Breath sounds',
      'Assessment - Rate and work of breathing',
      'Assist with extubation',
      'Assist with intubation',
      'Assist with removal of Chest Tube',
      'Assist with setup/insertion of Chest Tube',
      'Assist with thoracentisis',
      'Care of neonate - Bronchopulmonary dysplasia (BPD)',
      'Care of neonate - Cardiogenic/Hypovolemic shock',
      'Care of neonate - Diaphragmatic hernia',
      'Care of neonate - Fresh tracheostomy',
      'Care of neonate - Meconium aspiration',
      'Care of neonate - Persistent pulmonary hypertension (PPHN)',
      'Care of neonate - Pneumothorax',
      'Care of neonate - Respiratory distress syndrome',
      'Care of neonate - Respiratory failure',
      'ECMO',
      'Endotracheal tube stabalization',
      'Interpret - Arterial Blood Gases',
      'Interpret - X-ray reports',
      'Medications - Aminophylline',
      'Medications - Prostaglandin',
      'Obtain ABG\'s - Arterial line',
      'Obtain ABG\'s - Heel stick',
      'Obtain ABG\'s - Peripheral',
      'Obtain ABG\'s - Umbilical line',
      'Oxygen therapy and medication delivery via Bag (anesthesia) and mask',
      'Oxygen therapy and medication delivery via Bag (self-inflating) and mask',
      'Oxygen therapy and medication delivery via Bag and mask',
      'Oxygen therapy and medication delivery via Bulb syringe',
      'Oxygen therapy and medication delivery via CPAP (nasal prongs)',
      'Oxygen therapy and medication delivery via ET tube',
      'Oxygen therapy and medication delivery via Face mask',
      'Oxygen therapy and medication delivery via Nasal canula',
      'Oxygen therapy and medication delivery via Nebulizer',
      'Oxygen therapy and medication delivery via Oxyhood',
      'Oxygen therapy and medication delivery via Tent',
      'Oxygen therapy and medication delivery via Trach collar',
      'Use of artificial surfactent',
      'Ventilator care - CPAP/PEEP',
      'Ventilator care - High frequency jet ventilator',
      'Ventilator care - Home ventilator',
      'Ventilator care - IMV',
      'Ventilator care - Oscillating',
      'Ventilator care - Pressure ventilator',
      'Ventilator care - Volume ventilator',
      'Ventilator care - Weaning',
    ),
    'Neurological' => array(
      'Care of neonate - Brain death/organ procurement',
      'Care of neonate - Externalized VP shunt/reservoirs',
      'Care of neonate - Increased intracranial pressure',
      'Care of neonate - Meningitis',
      'Care of neonate - Seizures',
      'ICP Management - Monitoring',
      'Medications - Anticonvulsants',
      'Monitoring - Intracranial pressure',
    ),
    'Gastrointestinal' => array(
      'Assessment - Abdominal girth',
      'Assessment - Abdominal/bowel sounds',
      'Assessment - Palate',
      'Assessment - Suck/Swallow',
      'Care of neonate - Cleft Palate',
      'Care of neonate - Colostomy/ileostomy',
      'Care of neonate - Gastroschisis/omphalocele',
      'Care of neonate - GI Bleeding',
      'Care of neonate - Inguinal hernia',
      'Care of neonate - Necrotizing enterocolitis (NEC)',
      'Care of neonate - Post abdominal surgery',
      'Care of neonate - Reflux precautions',
      'Care of neonate - Tracheoesophogeal fistula',
      'Feedings - Bottle',
      'Feedings - Breast',
      'Feedings - Breast milk handling/storage',
      'Feedings - Gavage',
      'Feedings - Hospital grade electric breast pump',
      'Monitoring - Gastrostomy tube',
      'Monitoring - Jejunostomy tube',
      'Perform - placement of NG tube',
      'Perform - Test for occult blood',
    ),
    'Renal/Genitourinary' => array(
      'Perform - Urine specimen collection (catheter)',
      'Perform - Urine specimen collection (diaper/bag)',
    ),
    'Endocrine/Metabolic' => array(
      'Assessment - Finnegan',
      'Assessment - Fluid and Electrolyte balance',
      'Assist with Supra-pubic tap',
      'Care of neonate - Acute Renal Failure',
      'Care of neonate - DIC',
      'Care of neonate - Disorders of internal/external organs',
      'Care of neonate - Drug addiction/withdrawal',
      'Care of neonate - Hypo/Hyperglycemia',
      'Care of neonate - Hypo/hyperkalemia',
      'Care of neonate - Hypo/hypernatremia',
      'Care of neonate - Malformation of the GU tract',
      'Care of neonate - Peritoneal dialysis',
      'Collect and Interpret - Labstix',
      'Collect and Interpret - Occult blood',
      'Collect and Interpret - Urine Glucose',
      'Collect and Interpret - Urine pH',
      'Collect and Interpret - Urine Specific Gravity',
      'Interpret - Bilirubin',
      'Monitor - Phototherapy for jaundice',
      'Perform - Post circumcision care',
    ),
    'Infectious Disease' => array(
      'Care of neonate - Hepatitis surface antigen+ mother',
      'Care of neonate - HIV+ mother',
      'Care of neonate - Neonatal sepsis',
      'Interpret - CBC/differential',
      'Interpret - Culture reports',
      'Interpret - Maternal lab results',
      'Medications - DPT',
      'Medications - HBIG',
      'Medications - HBV',
      'Medications - HIB',
      'Medications - Polio vaccine',
      'Medications - RespiGam',
      'Perform - Assist with lumbar puncture',
      'Perform - Collect culture specimens',
      'Perform - Isolation',
      'Perform - Understand and follow Universal Precautions',
    ),
    'Pain Management' => array(
      'Assessment - Pain level/tolerance',
      'Care of neonate - with sedation',
      'Care of patient - Narcotic analgesia',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Assessment - Site and patency',
      'Care of neonate - Broviac',
      'Care of neonate - Groshong',
      'Care of neonate - Hickman',
      'Care of neonate - Percutaneous arterial line',
      'Care of neonate - Percutaneous venous line',
      'Care of neonate - Peripheral line/dressing',
      'Care of neonate - PICC',
      'Care of neonate - Portacath',
      'Care of neonate - Quinton',
      'Care of neonate - Umbilical artery line',
      'Care of neonate - Umbilical venous line',
      'Equipment - IV Pump',
      'Equipment - Syringe Pump',
      'Perform - Administer Cryoprecipitate',
      'Perform - Administer Hyperalimentation/TPN',
      'Perform - Administer Lipids',
      'Perform - administer Packed RBC\'s',
      'Perform - administer Plasma/Albumin',
      'Perform - administer Whole blood',
      'Perform - Discontinue IV Therapy',
      'Perform - Draw blood from central line',
      'Perform - Draw venous blood',
      'Perform - Dressing and tubing change',
      'Perform - Rate calculation',
      'Perform - start IV (angiocath)',
      'Perform - start IV (butterfly)',
      'Perform - start IV (heparin lock)',
    ),
    'Miscellaneous' => array(
      'Assessment - APGAR score',
      'Assessment - Eye exam (r/o retinopathy)',
      'Assessment - Gestational Age (Ballard)',
      'Assessment - Gestational Age (Dubowitz)',
      'Assessment - Maternal history',
      'Assessment - screening for hearing loss',
      'Assessment - weight (bed scale, scale)',
      'Bereavement/postmortem care',
      'Medications - Emergency drug action and reaction',
      'Medications - Eye prophylaxis',
      'Medications - Neonatal drug actions and reactions',
      'Perform - Calculate dosage',
      'Perform - Cord care',
      'Perform - Neonatal skin care',
      'Perform - obtain consents (for immunizations, procedures, treatments)',
      'Perform - prepare neonate for transfer/transport',
      'Perform - use of positioning devices',
      'Thermoregulation - Isolette with humidity',
      'Thermoregulation - Radiant warmer',
      'Thermoregulation - Temperature (axillary, rectal, skin)',
      'Thermoregulation - wean to open crib/bassinet',
    ),
    'Medical Proficiency' => array(
      'Catheter care',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_507() {
  $elements = fieldset('Nurse Case Manager Skills Checklist');
  $skills = array(
    'Skilled Environments' => array(
      'Acute Care / Hospital',
      'Managed Care',
      'Other Environments',
      'SNF / Long Term Care',
    ),
    'Case Management' => array(
      'Ability to Prioritize',
      'Appropriateness of Care - Special Diagnosis and Therapeutic Procedures',
      'Computer Skills',
      'Concurrent Review',
      'CPT',
      'CSR - Continued Stay Review',
      'Disability Review',
      'Discharge Screens - Specific indicators of patients ability',
      'DP / DCP - Discharge Planning',
      'DRG - Diagnosis Related Grouping (exempt and non exempt states)',
      'ECD - 9 - CM',
      'EMR Systems',
      'Intensity of Service - Diagnosis & Therapeutic Services',
      'ISD - A Interqual - Criteria for Hospital Admissions',
      'Long Term/Short Term Disability Case Management',
      'MCM - Medical Case Management',
      'Miliman and Robertson Criteria',
      'Negotiation Skills',
      'PA - Physician Advisor',
      'PAC - Pre-Admission Certification',
      'PPR - Prospective Procedure Review',
      'Retrospective Review',
      'SIMS - Integral - Criteria for SSO Waiver',
      'SSO',
      'Telephonic Case Management',
      'Verbal Communication Skills',
      'Workers Compensation Case Management',
      'Workers Compensation Review',
      'Written Communication Skills',
    ),
    'Age Specific' => array(
      'Adolescent (12-18 years)',
      'Elderly (>60 years)',
      'Infant (Birth to 1 year)',
      'Mature Adult (30-60 years)',
      'Pre-School (3-6 years)',
      'School Age (6-12 years)',
      'Toddler (1-3 years)',
      'Young Adult (18-30 years)',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_508() {
  $elements = fieldset('Nurse Practitioner Skills Checklist');
  $skills = array(
    'Clinical Specialty Areas' => array(
      'Acute care hospital',
      'Ambulatory clinic',
      'Corporate worksite',
      'Correctional facility',
      'Emergency',
      'Government health agency',
      'Health department',
      'HMO',
      'Homeless',
      'Inpatient care',
      'Migrant Workers',
      'Mobile medical unit',
      'Nursing home / LTC',
      'Occupational health',
      'Pediatrics',
      'Physician office',
      'Private practice',
      'Psychiatric facility',
      'Rural health clinic',
      'School',
      'Specialty - Arteriosclerotic Heart Disease',
      'Specialty - Cardiac',
      'Specialty - Diabetes',
      'Specialty - Dyslipidemia',
      'Specialty - Emergency',
      'Specialty - Gastroenterology',
      'Specialty - Hypertension',
      'Specialty - Orthopedics',
      'Specialty - Osteomylitis',
      'Specialty - Peripheral neuropathy',
      'Specialty - Peripheral Vascular Disease',
      'Urgent care',
      'Woman\'s health clinic',
    ),
    'Miscellaneous' => array(
      'Confidentiality',
      'Continuing education',
      'Cultural awareness',
      'Discharge planning/ collaboration',
      'Familiarity with standards (CLIA waivers)',
      'Familiarity with standards (DQIP)',
      'Familiarity with standards (HEDIS)',
      'Familiarity with standards (HIPAA)',
      'Familiarity with standards (JCAHO)',
      'Familiarity with standards (OSHA)',
      'Leadership and teaching skills',
      'Legal and ethical issues',
      'Public health and health promotion',
      'Quality Assurance',
      'Research',
      'Self evaluation to improve patient care',
      'Working within scope of practice',
    ),
    'Medical Proficiency' => array(
      'Allergic reaction',
      'Anaphylaxis reaction',
      'Auditory',
      'Bedridden patients',
      'Catheter care',
      'Cesation of substance abuse',
      'Collaboration - Consulting with Pharmacist',
      'Collaboration - Consulting with Physician',
      'Collaboration - Consulting with Therapy (OT/PT/SPT)',
      'Collaboration - Dietician',
      'Collaboration - Family, support systems, community resources',
      'Collaboration - Referral to Pastorial services',
      'Comprehensive physical assessment',
      'Contractures',
      'Dermal ulcers',
      'Diabetes',
      'Diagnose Acute Diseases',
      'Diagnose Chronic Diseases',
      'Differential diagnosis',
      'Dispensing medication',
      'Documentation',
      'Enemas/Suppositories/Fecal Impaction',
      'Excision',
      'Explanation to patient/family',
      'Hemoglobinmeter',
      'Incision and Drainage',
      'Incontinence Care (Bowel/Bladder)',
      'Injections',
      'Interpret - 12-lead EKG\'s',
      'Interpret - Radiographic',
      'Intubations',
      'IPPB machine (CPAP)',
      'Knowledge of drug interactions',
      'Knowledge of pharmacokinetics',
      'Knowledge of pharmacology',
      'Knowledge of potential adverse reactions',
      'Knowledge of side effects',
      'Liquid oxygen',
      'Local infiltration',
      'Management of controlled substances (DEA)',
      'Medical Nutrition therapy',
      'Mental Health',
      'Monitoring drug therapy',
      'Naso-gastric (NG) Tubes',
      'Obesity management',
      'Obtain Family history',
      'Obtain Informed consent',
      'Order/Perform/Interpret screening and diagnostic tests',
      'Oxygen administration',
      'Pap Smears',
      'Pregnancy test',
      'Single layer wound closure',
      'Splinting of extremities',
      'Sputum collection',
      'Staph Infections',
      'Tracheostomies',
      'Tympanometry',
      'Ultrasound',
      'Urine collection',
      'Venipuncture',
      'Visual acuity',
      'Wound care',
    ),
    'Age Groups' => array(
      '12-18 years old (Adolescents)',
      '1-3 years old (Toddler)',
      '18-39 years old (Young Adults)',
      '3-5 years old (Preschool)',
      '39-64 years old (Middle Adults)',
      '5-12 years old (School Age Children)',
      '64+ years old (Older Adults)',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_509() {
  $elements = fieldset('Nursery Skills Checklist');
  $skills = array(
    'Normal Neonatal Care' => array(
      'Assessment ‐ Ballard scale',
      'Assessment ‐ Circumference',
      'Assessment ‐ Dubowitz scale',
      'Assessment ‐ Length',
      'Assessment ‐ Neonatal jaundice',
      'Assessment ‐ Reflexes',
      'Assessment ‐ Vital Signs',
      'Assessment ‐ Weight',
      'Assist with circumcision ‐ Assess site post op',
      'Assist with circumcision ‐ Teach circumcision care to patients',
      'Perform ‐ Bath Infant',
      'Perform ‐ Culture suspect infectious neonate',
      'Perform ‐ Discharge procedure',
      'Perform ‐ Incubator/isolettes',
      'Perform ‐ Infant indentification',
      'Perform ‐ Monitor bladder and bowel patterns',
      'Perform ‐ Neonate CPR',
      'Perform ‐ Obtain urine specimen',
      'Perform ‐ Phototherapy',
      'Perform ‐ Test stool for blood',
      'Perform ‐ Test stool for reducing substances',
      'Perform ‐ Thermo‐neutral environment',
    ),
    'Neonatal/Pediatrics' => array(
      'Assessment ‐ S/S child abuse and neglect',
      'Care of child ‐ Epiglottitis',
      'Care of child ‐ Near drowning',
      'Care of child ‐ Overdose/poison ingestion',
      'Care of child ‐ Status asthmaticus',
      'Care of child ‐ Status epilepticus',
      'Care of infant or child ‐ Bronchopulmonary Dysplasia (BPD)',
      'Care of infant or child ‐ Croup',
      'Care of infant or child ‐ Epiglottitis',
      'Care of infant or child ‐ Meconium aspiration',
      'Care of infant or child ‐ Near drowning',
      'Care of infant or child ‐ PIE',
      'Care of infant or child ‐ PPHN',
      'Care of infant or child ‐ RDS',
      'Care of infant or child ‐ RSV',
      'Care of infant or child ‐ Transient tachypnea of the newborn',
      'Perform ‐ Assist with ECMO',
      'Perform ‐ Assist with high risk delivery',
      'Perform ‐ Capillary blood gasses',
      'Perform ‐ O2 Delivery',
      'Perform ‐ Obtain consent to treat',
      'Perform ‐ Pediatric CPR/Code',
      'Perform ‐ Umbilical blood gasses',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Assessment ‐ Site and patency',
      'Assist with cutdown',
      'Care of child ‐ Broviac',
      'Care of child ‐ Groshong',
      'Care of child ‐ Hickman',
      'Care of child ‐ Peripheral line/dressing',
      'Care of child ‐ Portacath',
      'Care of child ‐ Quinton',
      'Care of neonate ‐ Broviac',
      'Care of neonate ‐ Groshong',
      'Care of neonate ‐ Hickman',
      'Care of neonate ‐ Percutaneous arterial line',
      'Care of neonate ‐ Percutaneous venous line',
      'Care of neonate ‐ Peripheral line/dressing',
      'Care of neonate ‐ PICC',
      'Care of neonate ‐ Portacath',
      'Care of neonate ‐ Quinton',
      'Care of neonate ‐ Umbilical artery line',
      'Care of neonate ‐ Umbilical venous line',
      'Care of patient ‐ angiography',
      'Care of patient ‐ Broviac',
      'Care of patient ‐ Groshong',
      'Care of patient ‐ Hickman',
      'Care of patient ‐ Peripheral line/dressing',
      'Care of patient ‐ Portacath',
      'Care of patient ‐ Quinton',
      'Equipment ‐ IV Pump',
      'Equipment ‐ Syringe Pump',
      'Perform ‐ Administer ‐ Cardiac drugs',
      'Perform ‐ Administer ‐ Fluids',
      'Perform ‐ Administer ‐ Muscle relaxants',
      'Perform ‐ Administer ‐ Plasma expanders',
      'Perform ‐ Administer ‐ Vasoactive drugs',
      'Perform ‐ Administer blood products ‐ autotransfusion',
      'Perform ‐ Administer Cryoprecipitate',
      'Perform ‐ Administer Hyperalimentation/TPN',
      'Perform ‐ Administer Lipids',
      'Perform ‐ administer Packed RBC\'s',
      'Perform ‐ administer Plasma/Albumin',
      'Perform ‐ administer Whole blood',
      'Perform ‐ Discontinue IV Therapy',
      'Perform ‐ Draw blood from central line',
      'Perform ‐ Draw venous blood',
      'Perform ‐ Dressing and tubing change',
      'Perform ‐ Rate calculation',
      'Perform ‐ start IV (angiocath)',
      'Perform ‐ start IV (butterfly)',
      'Perform ‐ start IV (heparin lock)',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_510() {
  $elements = fieldset('Occupational Therapy Skills Checklist');
  $skills = array(
    'Therapy - Pediatrics' => array(
      'Care of child ‐ Burn Care',
      'Care of child ‐ Cerebral Palsy',
      'Care of child ‐ Cystic Fibrosis',
      'Care of child ‐ Muscular Dystrophy',
      'Care of child ‐ Spina Bifida',
      'Care of child ‐ Traumatic Brain Injury',
      'Critical Care Procedures',
      'ICU Procedures',
      'Isolation Precautions',
      'Neurologic ‐ Congenital Defects',
      'Neurologic ‐ Developmental Disabilities Sequencing Test',
      'Neurologic ‐ Head Injury',
      'Neurologic ‐ Neurodevelopmenat Deficits',
      'Neurologic ‐ Neurodevelopmental Techniques',
      'Neurologic ‐ Sensory Integrative Deficits',
      'Neurologic ‐ Spinal Cord Injury',
      'Neurologic ‐ Visual Perceptual Disorders',
      'NICU',
      'Orthopedics ‐ Birth Defects',
      'Orthopedics ‐ Developmental Diseases of Bone',
      'Orthopedics ‐ Fractures',
      'Procedure / Treatment ‐ Adaptive Equipment Assessment',
      'Procedure / Treatment ‐ Adaptive Equipment Training',
      'Procedure / Treatment ‐ Dynamic Splints',
      'Procedure / Treatment ‐ Feeding',
      'Procedure / Treatment ‐ Postural Balance Training',
      'Procedure / Treatment ‐ Resting Splints',
      'Procedure / Treatment ‐ Sensory Integration Techniques',
      'Procedure / Treatment ‐ Splinting ‐ Hand',
      'Procedure / Treatment ‐ Splinting ‐ Lower Extremity',
      'Procedure / Treatment ‐ Splinting ‐ Wrist',
      'Procedure / Treatment ‐ Vision Therapy',
      'Prosthetics ‐ Lower Extremity Prosthetics',
      'Prosthetics ‐ Upper Extremity Prosthetics',
      'SICU Procedures',
    ),
    'Therapy - Adult' => array(
      'Burn Management',
      'Cardiac Rehab',
      'Care of patient ‐ Traumatic Brain Injury',
      'Care of patient ‐ Ventilator Dependent / Assisted',
      'CCU Procedures',
      'ICU Procedures',
      'Neurologic ‐ Adaptive Equipment ‐ Wheelchair',
      'Neurologic ‐ Cognitive disorders',
      'Neurologic ‐ Head Trauma',
      'Neurologic ‐ Neuromuscular disease',
      'Neurologic ‐ Spinal Cord Injury',
      'Neurologic ‐ Stroke Rehabilitation',
      'Orthopedic ‐ Amputations',
      'Orthopedic ‐ Back Injuries/Surgeries',
      'Orthopedic ‐ Fractures',
      'Orthopedic ‐ Hand Injury',
      'Orthopedic ‐ Osteoarthritis',
      'Orthopedic ‐ Total Joint Replacement',
      'Procedure / Treatment ‐ Activities of Daily Living',
      'Procedure / Treatment ‐ Adaptive Equipment Assessment',
      'Procedure / Treatment ‐ Adaptive Equipment Training',
      'Procedure / Treatment ‐ Cognitive Techniques',
      'Procedure / Treatment ‐ Community Re‐entry',
      'Procedure / Treatment ‐ Cryotherapy',
      'Procedure / Treatment ‐ Debridement',
      'Procedure / Treatment ‐ Exercise Programs',
      'Procedure / Treatment ‐ Feeding Techniques',
      'Procedure / Treatment ‐ Functional ‐ Dynamic splinting',
      'Procedure / Treatment ‐ Functional Capacity Evaluation',
      'Procedure / Treatment ‐ Gait Training',
      'Procedure / Treatment ‐ Group Treatment',
      'Procedure / Treatment ‐ Home Safety Evaluation',
      'Procedure / Treatment ‐ Hydrotherapy',
      'Procedure / Treatment ‐ Iontophoresis',
      'Procedure / Treatment ‐ Joint Mobilization',
      'Procedure / Treatment ‐ Muscle Energy Techniques',
      'Procedure / Treatment ‐ Myofacial release',
      'Procedure / Treatment ‐ Neurodevelopment Techniques',
      'Procedure / Treatment ‐ Phonophoresis',
      'Procedure / Treatment ‐ Postural Education',
      'Procedure / Treatment ‐ Soft Tissue Manipulation',
      'Procedure / Treatment ‐ Splinting ‐ Elbow',
      'Procedure / Treatment ‐ Splinting ‐ Hand',
      'Procedure / Treatment ‐ Splinting ‐ Wrist',
      'Procedure / Treatment ‐ Sterilization Techniques',
      'Procedure / Treatment ‐ TENS',
      'Procedure / Treatment ‐ Transfers',
      'Procedure / Treatment ‐ Ultrasound',
      'Procedure / Treatment ‐ Vision Therapy',
      'Procedure / Treatment ‐ Whirlpool',
      'Procedure / Treatment ‐ Work Capacity Evaluation',
      'Procedure / Treatment ‐ Wound Management',
      'Prosthetics ‐ Above Knee',
      'Prosthetics ‐ Ankle/Foot Orthosis',
      'Prosthetics ‐ Below Knee',
    ),
    'Therapy - Geriatrics' => array(
      'Burn Management',
      'Cardiac Rehab',
      'Cardiovascular Disease',
      'Diabetes',
      'Exercise Programs',
      'Geriatric Psych',
      'ICU Procedures',
      'Neurologic ‐ Adaptive Equipment ‐ Wheelchair',
      'Neurologic ‐ Alzheimers / Dementia',
      'Neurologic ‐ Cognitive Disorders',
      'Neurologic ‐ Head Trauma',
      'Neurologic ‐ Neuromuscular Disease',
      'Neurologic ‐ Spinal Cord Injury',
      'Neurologic ‐ Stroke Rehabilitation',
      'Orthopedic ‐ Amputation',
      'Orthopedic ‐ Arthritis',
      'Orthopedic ‐ Fractures',
      'Orthopedic ‐ Hand Injuries',
      'Orthopedic ‐ Osteoarthritis',
      'Orthopedic ‐ Post Operative Care',
      'Orthopedic ‐ Shoulder Injury',
      'Orthopedic ‐ Total Joint Replacement',
      'Pacemakers',
      'Procedure / Treatment ‐ Activities of Daily Living',
      'Procedure / Treatment ‐ Adaptive Equipment Assessment',
      'Procedure / Treatment ‐ Adaptive Equipment Training',
      'Procedure / Treatment ‐ Cognitive Rehab',
      'Procedure / Treatment ‐ Community Re‐entry',
      'Procedure / Treatment ‐ Documentation ‐ Medicare A',
      'Procedure / Treatment ‐ Documentation ‐ Medicare B',
      'Procedure / Treatment ‐ Documentation ‐ State Healthcare',
      'Procedure / Treatment ‐ Feeding Techniques',
      'Procedure / Treatment ‐ Functional ‐ Dynamic Splinting',
      'Procedure / Treatment ‐ Gait Training',
      'Procedure / Treatment ‐ Home Safety Evaluation',
      'Procedure / Treatment ‐ Hydrotherapy',
      'Procedure / Treatment ‐ Massage',
      'Procedure / Treatment ‐ Neurodevelopment Testing',
      'Procedure / Treatment ‐ Resting Splints',
      'Procedure / Treatment ‐ Splinting ‐ Hand',
      'Procedure / Treatment ‐ Splinting ‐ Wrist',
      'Procedure / Treatment ‐ Sterilization Techniques',
      'Procedure / Treatment ‐ TENS',
      'Procedure / Treatment ‐ Transfers',
      'Procedure / Treatment ‐ Ultrasound',
      'Procedure / Treatment ‐ Vision Therapy',
      'Procedure / Treatment ‐ Whirlpool',
      'Procedure / Treatment ‐ Wound Management',
      'Prosthetics ‐ Above Knee',
      'Prosthetics ‐ Ankle/Foot Orthosis',
      'Prosthetics ‐ Below Knee',
      'Upper Extremity Prosthetics',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_511() {
  $elements = fieldset('OR Skills Checklist');
  $skills = array(
    'EENT' => array(
      'Adenoidectomy',
      'Caldwell - Luc',
      'Cleft lip/palate repair',
      'Closed reduction nasal fracture',
      'Ethmoidectomy',
      'Excision of salivary gland tumor',
      'Fenestration procedure',
      'Frontal flap sinus procedure',
      'Glossectomy',
      'Laryngectomy',
      'Mandibulectomy',
      'Mastoidectomy',
      'Maxillary advancement with hip graft',
      'Maxillectomy',
      'Myringoplasty',
      'Myringotomy/with PE tube insertion',
      'Nasal polypectomy',
      'Open Reduction Facial Fracture',
      'Open Reduction Nasal Fracture',
      'Parotidectomy',
      'Pharyngeal flap procedure',
      'Radical neck dissection',
      'Ranulectomy',
      'Rhinoplasty/septoplasty',
      'Selective osteotomy of maxilla/mandible',
      'Sinus endoscopy',
      'Sinusotomy',
      'Stapedectomy',
      'Submucous resection',
      'Tonsillectomy',
      'Tracheostomy',
      'Tympanoplasty',
    ),
    'Trauma' => array(
      'Burns',
      'Gunshot/Stab wounds - Abdomen',
      'Gunshot/Stab wounds - Chest',
      'Gunshot/Stab wounds - Head',
      'Motor Vehicle Accident (multiple injuries)',
      'Traumatic amputation',
    ),
    'Orthopedics' => array(
      'Acetabular/pelvic ORIF',
      'Achilles tendon repair',
      'Amputation of extremity',
      'Anterior cruciate ligament repair',
      'Application of external fixator',
      'Application of external fixator (Extremities)',
      'Application of external fixator (Pelvis)',
      'Application of halo traction',
      'Arthoscopy - Ankle',
      'Arthoscopy - Elbow',
      'Arthoscopy - Knee',
      'Arthoscopy - Shoulder',
      'Arthotomy',
      'Bipolar/unipolar hips',
      'Bunionectomy',
      'Calcaneal reconstruction',
      'Capsulorrhaphy',
      'Care of patient - Total hip replacement',
      'Carpal tunnel release',
      'Closed reduction fracture',
      'Hand surgery with implants',
      'Harrington rod instrumentation and/or Dwyer procedure',
      'Heel cord lenghtening',
      'Hip compression nails and lag screws - Jewett',
      'Hip compression nails and lag screws - Kuntsher rod',
      'Hip compression nails and lag screws - Lottes',
      'Hip compression nails and lag screws - Rush',
      'Hip compression nails and lag screws - Schneider',
      'Hip compression nails and lag screws - Zimmer',
      'Iliac crest bone graft',
      'Intramedullary rods - Extraction',
      'Intramedullary rods - Femoral',
      'Intramedullary rods - Humeral',
      'Intramedullary rods - Insertion',
      'Intramedullary rods - Supracondylar',
      'Intramedullary rods - Tibial',
      'Olecranon bursa, excision of',
      'ORIF, internal fixation with compression set',
      'Patellectomy',
      'Putti Platt/Bankart procedure/rotator cuff repair',
      'Reduction with compression sets',
      'Reimplantation of digits',
      'Repair hammer toes',
      'Sacro-iliac (SI) joint screws',
      'Sharrard procedure',
      'Spica cast, application of',
      'Spinal fusion',
      'Tendons transplant (hand or foot)',
      'Total joint replacement/revision - Total Hip',
      'Total joint replacement/revision - Total Knee',
      'Total joint replacement/revision - Total Shoulder',
    ),
    'Endoscopic' => array(
      'Bronchoscopy',
      'Colonoscopy',
      'Culdoscopy',
      'Cystoscopy',
      'Esophagoscopy',
      'Gastroscopy',
      'Hysteroscopy',
      'Laparoscopic Procedure - Appendectomy',
      'Laparoscopic Procedure - Cholecystectomy/cholangiogram',
      'Laparoscopic Procedure - Colon resection',
      'Laparoscopic Procedure - Hernia repair',
      'Laparoscopic Procedure - Nissen fundoplication',
      'Laparoscopic Procedure - Salpingo-oophorectomy',
      'Laparoscopic Procedure - Tubal ligation',
      'Laparoscopic Procedure - Vaginal hysterectomy',
      'Laryngoscopy & Microlaryngoscopy',
      'Mediastinoscopy',
      'Pelviscopy',
      'Sigmoidoscopy',
      'Thoracoscopy',
    ),
    'General Surgery' => array(
      'Abdominal perineal resection',
      'Adrenalectomy',
      'Anal fissurectomy',
      'Appendectomy',
      'Breast biopsy',
      'Coloectomy',
      'Colostomy/ileostomy',
      'Gastrectomy',
      'Gastroplasty',
      'Hemorrhoidectomy',
      'Hepatic resection',
      'Herniorrphaphy - femoral, inguinal, umbilical',
      'Hiatal herniorrhaphy, transabdominal/transthoracic',
      'Hickman/Groshong/Portacath insertion',
      'Hydrocelectomy',
      'Imperforate anus reconstruction',
      'Lumbar sympathectomy',
      'Omphalocele repair',
      'Pancreatectomy/pancreatogram',
      'Pilonidal cystectomy',
      'Portal caval shunt',
      'Pyloric stenosis',
      'Radical mastectomy',
      'Saphenous vein ligation and stripping',
      'Sentinel node biopsy',
      'Splenectomy',
      'Tenchkoff catheter placement',
      'Thyroglossal duct cyst excision',
      'Thyroidectomy',
      'Tracheostomy',
      'Vagotomy',
    ),
    'Gynecology' => array(
      'Cesarean section',
      'Colpotomy',
      'Dilation and curettage (D & C)',
      'Hysterectomy, abdominal',
      'Hysterectomy, vaginal',
      'Marshall - Marchetti',
      'Marsupialization',
      'Ovarian Cystectomy',
      'Radium insertion',
      'Sacral spinus fixation',
      'Shirodkar procedure',
      'Suction curettage',
      'Vaginal reconstruction',
      'Vaginectomy/vulvectomy',
    ),
    'Neurology' => array(
      'Anterior cervical fusion',
      'Anterior laparoscopic spine procedures',
      'Anterior lumbar interbody fusion (ALIF)',
      'A-V malformation',
      'Burr holes for subdural hematoma',
      'Carotid ligation',
    ),
    'Ophthamology' => array(
      'Cataract extraction with IOL',
    ),
    'Neurology' => array(
      'Cervical sympathectomy',
    ),
    'Ophthamology' => array(
      'Corneal transplant',
    ),
    'Neurology' => array(
      'Craniectomy for decompression fracture',
      'Cranioplasty/craniotomy - clipping of aneurysm',
      'Cranioplasty/craniotomy - Tumor excision',
      'Cranioplasty/craniotomy - using stealth equipment',
    ),
    'Ophthamology' => array(
      'Dacryocystectomy',
      'Dacryocystorhinostomy',
    ),
    'Neurology' => array(
      'Disectomy',
      'Hypophysectomy',
      'Insertion nerve stimulators/medication pumps',
    ),
    'Ophthamology' => array(
      'Iridectomy',
    ),
    'Neurology' => array(
      'Laminectomy',
    ),
    'Ophthamology' => array(
      'Lid and muscle procedures',
    ),
    'Neurology' => array(
      'Myelomeningocele',
    ),
    'Ophthamology' => array(
      'Orbital implant',
    ),
    'Neurology' => array(
      'Pedicle screw insertion',
    ),
    'Ophthamology' => array(
      'Phaco emulsification',
    ),
    'Neurology' => array(
      'Posterior lumbar interbody fusion (PLIF)',
    ),
    'Ophthamology' => array(
      'Pterygium repair',
      'Recession resection',
      'Repair orbital blowout fracture',
      'Scleral buckle',
    ),
    'Neurology' => array(
      'Shunt procedure (VP, VA/LP)',
      'Spine fusion',
      'Ulnar nerve transfer',
      'Ventriculography procedure/ventriculoscopy',
      'Ventriculostomy',
    ),
    'Ophthamology' => array(
      'Vitrectomy',
    ),
    'Oral' => array(
      'Closed reduction facial fractures/wiring',
      'Excision odontoma',
      'Extraction deciduous teeth',
      'Extraction of impacted molars',
      'Fractured jaw, mandibular or zygomatic',
      'LeFort osteotomies',
      'Maxillary procedure with graft',
      'Pediatric denistry',
      'Sagittal osteotomy',
      'TMJ exploration',
      'TMJ with arthoplasty',
    ),
    'Plastics' => array(
      'Abdominal lipectomy',
      'Blepharoplasty',
      'Face Lift',
      'Mammoplasty',
      'Mammoplasty - Augmentation',
      'Mammoplasty - Reduction',
      'Mammoplasty - Tramflaps with reconstructive mammoplasty',
      'Mentoplasty',
      'Otoplasty',
      'Pedicle grafts',
      'Scar revisions',
      'Splint thickness skin grafting',
      'Tissue expanders',
    ),
    'Thoracic Open Heart' => array(
      'Cervical rib excision',
      'Chamberlain procedure',
      'Closed thoracotomy',
      'Correction pectus excavatum',
      'Esophagectomy',
      'Heller procedure',
      'Mitral commissurotomy',
      'Open Heart Procedures - Mitral or Aortic valve replacement',
      'Open Heart Procedures - Patent ductus arteriosus',
      'Open Heart Procedures - Septal defect repairs',
      'Open Heart Procedures - Tetralogy of Fallot',
      'Pacemaker implantation - endocardial',
      'Pacemaker implantation - myocardial',
      'Pericardiectomy',
      'Resection coarctation aorta',
      'Rib resection',
      'Thoracoplasty',
      'Tracheal resection',
      'Transthoracic diaphragmatic herniorrhaphy',
    ),
    'Transplant' => array(
      'Bone',
      'Bone Marrow',
      'Corneal',
      'Harvesting',
      'Heart',
      'Kidney',
      'Liver',
      'Lung',
      'Multi-organ',
      'Pancreas',
      'Skin',
    ),
    'Urology' => array(
      'Adult Circumcision',
      'Cystectomy',
      'Cystoscopy/Ureteroscopy',
      'Hypospadias repair',
      'Implants: penile, testicular',
      'Lithotripsy',
      'Nephrectomy',
      'Nephrolithotomy',
      'Orchiopexy',
      'Prostatectomy - Perineal',
      'Prostatectomy - Supra-pubic',
      'Pyeloplasty',
      'Radical node dissection',
      'Scott incontinence device',
      'TURP',
      'Ureterolithotomy',
      'Vasectomy',
      'Vasovasostomy',
      'Waterhouse procedure',
    ),
    'Vascular' => array(
      'Aortic aneurysm with graft replacement',
      'A-V Access graft',
      'Endarterectomy/carotid - femoral',
      'Peripheral vascular bypass procedure',
      'Resection carotid aneurysm with graft',
      'Thrombectomy/embolectomy',
      'Vena cava fliter/umbrella',
      'Vena cava ligation',
    ),
    'Laser' => array(
      'CO2',
      'Eye',
      'Yag',
    ),
    'Equipment' => array(
      'Argon beam coagulator',
      'Bair Hugger',
      'Bed Scale',
      'Blood/Fluid warmer',
      'Camera/video system - Camera controller',
      'Camera/video system - Light source',
      'Camera/video system - Printers',
      'Camera/video system - VCR\'s',
      'Cell saver',
      'Cidex soak',
      'Dermatome - Brown',
      'Dermatome - Padgett',
      'Dermatome - Zimmer',
      'Disposable grounding pads',
      'Drills - 3M Maxi-Driver',
      'Drills - Codman craniotome',
      'Drills - Hall air driver',
      'Drills - Hall neurotome',
      'Drills - Midax Rex/Anspach',
      'Drills - Minidriver',
      'Drills - Stryker - Large Battery',
      'Drills - Stryker - Small Battery',
      'Electrosurgical unit',
      'Emerson thoracic pump',
      'Ethylene oxide sterilizer - AMSCO',
      'Eye magnet',
      'Flash autoclave - AMSCO',
      'Fracture table - Chick table/Marquet table',
      'Fracture table - Jackson table',
      'Fracture table - Rush table/Skyton table',
      'Hypo/hyperthermia unit',
      'Intestinal stapling device - EEA',
      'Intestinal stapling device - GIA',
      'Intestinal stapling device - LDS',
      'Intestinal stapling device - TA',
      'Kreiselman resuscitator',
      'Surgairtome',
      'Synthes A-O Drill',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_512() {
  $elements = fieldset('Optical Skills Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_513() {
  $elements = fieldset('PACU Skills Checklist');
  $skills = array(
    'Cardiovascular' => array(
      'Assessment - Doppler',
      'Assessment - pulses/circulation',
      'Assist with Arterial line insertion',
      'Assist with Central line insertion',
      'Care of patient - Aneurysm',
      'Care of patient - Hypertension',
      'Care of patient - Hypotension',
      'Care of patient - Post Angioplasty',
      'Care of patient - Post Athrectomy (DCA)',
      'Care of patient - Post CABG',
      'Care of patient - Post cardiac cath',
      'Care of patient - Post cardiac surgery',
      'Care of patient - Post intracoronary stent placement',
      'Care of patient - Post percutaneous balloon valvuloplasty',
      'Care of patient - Post rotoblade',
      'Care of patient - Post stent placement',
      'Care of patient - Thrombophlebitis',
      'Medications - Digoxin',
      'Medications - Dobutamine',
      'Medications - Dopamine',
      'Monitoring - A-line (radial)',
      'Monitoring - Arrhythmia interpretation',
      'Monitoring - Cardiac Output',
      'Monitoring - CVP',
      'Pacemaker - External',
      'Pacemaker - Permanent',
      'Perform - Controlled cardioversion',
      'Perform - CVP Line Removal',
      'Perform - Set up CVP line',
    ),
    'Pulmonary' => array(
      'Airway Management - Endotracheal/suctioning',
      'Airway Management - Nasal/suctioning',
      'Airway Management - Oropharyngeal/suctioning',
      'Airway Management - Tracheostomy/suctioning',
      'Assessment - Breath sounds',
      'Assist with extubation',
      'Assist with intubation',
      'Assist with removal of Chest Tube',
      'Assist with setup/insertion of Chest Tube',
      'Care of patient - Aspiration',
      'Care of patient - Fresh tracheostomy',
      'Care of patient - Laryngospasm',
      'Care of patient - on ventilator',
      'Care of patient - Pneumothorax',
      'Chest Tube - Use of Pleurevac or Thoraclex',
      'Chest Tube - Use of Water seal drainage system',
      'Establishing an airway',
      'Interpret - Arterial Blood Gases',
      'Medications - Steroids',
      'Obtain ABG\'s - Arterial line',
      'Oxygen therapy and medication delivery via Bag and mask',
      'Oxygen therapy and medication delivery via Face mask',
      'Oxygen therapy and medication delivery via Nasal canula',
      'Oxygen therapy and medication delivery via Tent',
      'Oxygen therapy and medication delivery via Trach collar',
      'Pulse Oximetry',
    ),
    'Neurological' => array(
      'Assessment - Level of consciousness',
      'Care of patient - Post craniotomy',
      'Equipment - Hyper/hypothermia blanket',
      'Medications - Valium',
      'Monitoring - Intracranial pressure',
      'Monitoring - Seizure precautions',
    ),
    'Gastrointestinal' => array(
      'Care of patient - Colostomy',
      'Medications - Antiemetics',
      'Perform - Gomco/Emerson Suction',
      'Perform - placement of NG tube',
    ),
    'Renal/Genitourinary' => array(
      'Care of patient - Renal transplant',
      'Care of patient - Supra-pubic Catheter',
      'Insert and Care for 3-way Foley',
      'Perform - Insert and Care for Foley',
      'Perform - Insert and care for straight cather',
    ),
    'Trauma' => array(
      'Care of patient - Hypothermia',
      'Care of patient - Shock, Anaphylactic',
    ),
    'Infectious Disease' => array(
      'Care of patient - AIDS',
      'Perform - Isolation',
      'Perform - Understand and follow Universal Precautions',
    ),
    'Orthopedics' => array(
      'Care of patient - Cast',
      'Care of patient - Total hip replacement',
      'Care of patient - Total knee replacement',
      'Equipment - Continous Passive Motion (CPM)',
      'Equipment - Support Device - Traction',
    ),
    'Pain Management' => array(
      'Assessment - Pain level/tolerance',
      'Assist with delivery of anesthesia - Anesthesia toxicity',
      'Assist with delivery of anesthesia - Documentation of anesthesia',
      'Care of patient - Epidural anesthesia/analgesia',
      'Care of patient - Narcotic analgesia',
    ),
    'Respiratory Treatments' => array(
      'Airway Management - Nasal airway placement',
      'Airway Management - Nasal airway/suctioning',
      'Airway Management - Oral airway placement',
      'Airway Management - Oropharyngeal/suctioning',
      'Airway Management - Tracheostomy/suctioning',
      'Interpret - Arterial Blood Gasses',
      'Perform - draw arterial blood gas - Arterial Line',
      'Perform - Medication Delivery - Aerosol setup mask',
      'Perform - Oral /Nasal Airway Removal',
    ),
    'General Anesthesia' => array(
      'Perform - Patient Stimulation and Stir-up',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Perform - Dressing and tubing change',
    ),
    'Miscellaneous' => array(
      'Care of patient - Hypovolemic shock',
    ),
    'Equipment' => array(
      'Fracture table - Jackson table',
      'Fracture table - Rush table/Skyton table',
    ),
  );  
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_514() {
  $elements = fieldset('Pediatrics Skills Checklist');
  $skills = array(
    'Cardiovascular' => array(
      'Assessment - Ausculation (rate, rhythm, volume)',
      'Assessment - Blood pressure/non-invasive',
      'Assessment - Perfusion',
      'Care of child - Bacterial Endocarditis',
      'Care of child - Cardiac Arrest',
      'Care of child - Cardiomyopathy',
      'Care of child - Congenital heart defects',
      'Care of child - Congestive Heart Failure',
      'Care of child - Myocarditis',
      'Care of child - Pericarditis',
      'Care of child - Post cardiac cath',
      'Care of child - Post cardiac surgery',
      'Care of child - Rheumatic fever',
      'Care of child - Shock',
      'Interpret - Arterial Blood Gasses',
      'Interpret - Hemoglobin and Hematocrit',
      'Medications - Digoxin',
      'Monitoring - Basic 12 lead interpretation',
      'Monitoring - Cardiac Output',
    ),
    'Pulmonary' => array(
      'Airway Management - Nasal/suctioning',
      'Airway Management - Oropharyngeal/suctioning',
      'Airway Management - Tracheostomy/suctioning',
      'Apnea monitor',
      'Assessment - Breath sounds',
      'Assessment - Rate and work of breathing',
      'Care of child - Asthma',
      'Care of child - Bronchiolitis (RSV)',
      'Care of child - Bronchopulmonary dysplasia (BPD)',
      'Care of child - Cystic Fibrosis',
      'Care of child - Epiglotitis',
      'Care of child - LTB/croup',
      'Care of child - Pertussis',
      'Care of child - Pneumonia',
      'Care of child - Tonsillitis',
      'Care of child - Tuberculosis',
      'Chest Tube - Use of Pleurevac or Thoraclex',
      'Chest Tube - Use of Water seal drainage system',
      'End tidal CO2',
      'Medications - Alupent',
      'Medications - Aminophylline',
      'Medications - Isuprel',
      'Medications - Ventolin',
      'Oxygen therapy and medication delivery via Bulb syringe',
      'Oxygen therapy and medication delivery via Face mask',
      'Oxygen therapy and medication delivery via Isolette',
      'Oxygen therapy and medication delivery via Nasal canula',
      'Oxygen therapy and medication delivery via Oxyhood',
      'Oxygen therapy and medication delivery via Tent',
      'Oxygen therapy and medication delivery via Trach collar',
      'Perform - Chest physiotherapy',
      'Pulse Oximetry',
      'Water seal drainage system',
    ),
    'Neurological' => array(
      'Assessment - Level of consciousness',
      'Assist with lumbar puncture',
      'Care of child - Battered child syndrome',
      'Care of child - Closed head trauma',
      'Care of child - Encephalitis',
      'Care of child - Febrile seizures',
      'Care of child - Meningitis',
      'Care of child - Multiple sclerosis',
      'Care of child - Multiple trauma',
      'Care of child - Near drowning',
      'Care of child - Neuromuscular disease',
      'Care of child - Spinal cord injury',
      'Medications - Carbmazepine',
      'Medications - Clonazepam',
      'Medications - Dilantin',
      'Medications - Phenobarbital',
      'Medications - Valium',
      'Monitoring - Intracranial pressure',
    ),
    'Gastrointestinal' => array(
      'Assessment - Abdominal/bowel sounds',
      'Assessment - Nutrition',
      'Care of child - Anal fissure',
      'Care of child - Cleft lip/palate',
      'Care of child - Colostomy',
      'Care of child - Diaphragmatic hernia',
      'Care of child - Failure To Thrive (FTT)',
      'Care of child - Gastroenteritis/dehydration',
      'Care of child - GE reflux',
      'Care of child - GI bleeding',
      'Care of child - Ileostomy',
      'Care of child - Internal parasites',
      'Care of child - Necrotizing enterocolitis (NEC)',
      'Care of child - Pyloric stenosis',
      'Care of child - Surgical abdomen',
      'Care of child - Ulcerative colitis',
      'Equipment - Penrose drains',
      'Feedings - Bottle',
      'Feedings - Breast',
      'Feedings - Central hyperalimentation',
      'Feedings - Gastrostomy/button',
      'Feedings - Gavage',
      'Feedings - I-tubes',
      'Feedings - Jejunal',
      'Feedings - Peripheral hyperalimentation',
      'Perform - NG and sump tubes to suction',
      'Perform - placement of NG tube',
      'Perform - Wound irrigation/dressing change',
    ),
    'Renal/Genitourinary' => array(
      'Assessment - Fluid and Electorlyte balance',
      'Assist with Supra-pubic tap',
      'Care of child - Circumcision',
      'Care of child - Glomerularnephritis',
      'Care of child - Hemodialysis',
      'Care of child - Hemolytic Uremic Syndrome (HUS)',
      'Care of child - Hypospadias',
      'Care of child - Illeal conduit urethral',
      'Care of child - Infantile polycystic disease',
      'Care of child - Kidney transplant',
      'Care of child - Nephrotic syndrome',
      'Care of child - Peritoneal dialysis',
      'Care of child - Renal failure',
      'Care of child - Urinary tract infection',
      'Care of child - Wilms Tumor',
      'Insert and Care for indwelling catheter',
      'Insert and Care for straight catheter',
      'Interpret - BUN and Creatinine',
      'Interpret - Serum electrolytes',
      'Interpret - Urinalysis',
      'Perform - Insert and Care for Foley',
      'Perform - Urine specimen collection (routine)',
    ),
    'Endocrine/Metabolic' => array(
      'Assessment - Fluid and Electrolyte balance',
      'Care of child - Adrenal disorders',
      'Care of child - Cushing\'s syndrome',
      'Care of child - Juvenile diabetes',
      'Care of child - Pituitary disorders',
      'Care of child - Thyroid malfunction',
      'Interpret - Blood Glucose',
      'Interpret - Thyroid Studies',
      'Medications - Growth hormone',
      'Medications - Insulin',
      'Medications - Thyroid',
    ),
    'Hematology/Oncology' => array(
      'Assessment - Nutritional status',
      'Care of child - Anemia',
      'Care of child - Bone Marrow Transplant',
      'Care of child - Depressed immune system',
      'Care of child - Disseminated Intravascular Coagulation (DIC)',
      'Care of child - Hemophilia',
      'Care of child - Hodgkin\'s disease',
      'Care of child - Infectious Mononucleosis',
      'Care of child - Leukemia',
      'Care of child - Malignanat tumors',
      'Care of child - Sickle cell anemia',
      'Care of child - spleen trauma/splenectomy',
      'Interpret - blood counts',
      'Medications - Chemotherapy',
      'Medications - Prednisone',
      'Perform - Reverse isolation',
    ),
    'Infectious Disease' => array(
      'Care of child - AIDS',
      'Care of child - Childhood communicable diseases',
      'Care of child - Cytomegalovirus (CMV)',
      'Care of child - Hepatitis',
      'Care of child - Kawasaki Disease',
      'Care of child - Lyme Disease',
      'Interpret - Blood counts',
      'Perform - Fever Management',
      'Perform - Isolation',
    ),
    'Orthopedics' => array(
      'Assist with placement of cast',
      'Care of child - Pinned fractures',
      'Equipment - Support Device - Traction',
    ),
    'Wound Management' => array(
      'Assessment - skin for impending breakdown',
      'Assessment - stasis ulcers',
      'Assessment - surgical wound healing',
      'Care of child - 1st degree burns',
      'Care of child - 2nd degree burns',
      'Care of child - 3rd degree burns',
      'Care of child - Pressure sores',
      'Care of child - Staged decubitis ulcers',
      'Care of child - Surgical wounds with drain(s)',
      'Care of child - Traumatic wound care',
      'Equipment - Air fluidized, low air loss beds',
      'Perform - sterile dressing change',
      'Perform - wound care/irrigation',
    ),
    'Pain Management' => array(
      'Assessment - Pain level/tolerance',
      'Care of child - IV Conscious Sedation',
      'Care of child - Narcotic analgesia',
      'Care of child - Epidural anesthesia/analgesia',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Assist with cutdown',
      'Care of child - Broviac',
      'Care of child - Groshong',
      'Care of child - Hickman',
      'Care of child - Peripheral line/dressing',
      'Care of child - Portacath',
      'Care of neonate - Quinton',
      'Perform - Administer blood products - autotransfusion',
      'Perform - Administer Cryoprecipitate',
      'Perform - administer Packed RBC\'s',
      'Perform - administer Whole blood',
      'Perform - Draw blood from central line',
      'Perform - Draw venous blood',
      'Perform - start IV (angiocath)',
      'Perform - start IV (butterfly)',
      'Perform - start IV (heparin lock)',
    ),
    'Miscellaneous' => array(
      'Assessment - Immunization schedule',
      'Assessment - Normal growth and development',
      'Assessment - Normal laboratory values',
      'Assessment - Recognize signs of abuse or neglect',
      'Care of child - Anorexia/bulimia',
      'Care of child - Craniofacial reconstruction',
      'Care of child - Depression',
      'Care of child - ENT surgery',
      'Care of child - Eye surgery',
      'Care of child - Ingestion of foreign body',
      'Care of child - Ingestion of poison or toxin',
      'Care of child - Plastic surgery',
      'Care of child - Suicide threats/actions',
      'Eye/ear instillations',
      'Knowledge of emergency drugs',
      'Knowledge of routine pediatric drugs',
      'Metered dose inhaler',
      'Perform - Calculate dosage',
    ),
    'Equipment' => array(
      'Application of splints',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_515() {
  $elements = fieldset('Pharmacist Skills');

  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_516() {
  $elements = fieldset('Pharmacy Tech Skills Checklist');
  $skills = array(
    'Other Skills' => array(
      '3rd Party Billing',
      'Cashier',
      'Consulting',
      'Controlled Substance Investigation',
      'EMR Systems',
      'Experimental Research',
    ),
    'General Areas' => array(
      'EMR Systems',
      'HMO',
      'Home Health',
      'Hospital In‐patient',
      'Hospital Out Patient',
      'Independent',
      'Industry',
      'Joint Commission Certified',
      'Long Term Care',
      'Mail Order',
      'Nuclear',
      'Retail',
    ),
    'Specific Skills' => array(
      'Antibiotics',
      'Baker Machine',
      'Cad Cassettes',
      'Chemotherapy',
      'Clinical Rounds',
      'Hyperailmentation',
      'IV Piggyback',
      'IV Push Medications',
      'IV Therapy Compounding',
      'Mixers for TPN',
      'Packaging Anlineoplastic Agents',
      'Pain Controlled Analgesia',
      'Prepacking',
      'Unit Dose Cart Fill',
    ),
    'Extemporary Dose Preparation' => array(
      'Geriatric',
      'Pediatric',
    ),
    'Computer Systems' => array(
      '3M',
      'Cemer (Mega Source)',
      'Condor',
      'HBOC',
      'IBAX Series 5000',
      'Legacy(Coram)',
      'Medi‐Tech',
      'Mesta‐Med Plus',
      'Osco',
      'PAL',
      'Pharmworks',
      'Pix',
      'QS‐1',
      'Renlar',
      'Saint (1st data)',
      'SMS',
      'Technicon',
      'Walgreens',
      'Zadol',
    ),
    'Management Experience' => array(
      'Hospital',
      'Infusion',
      'Nuclear',
      'Retail',
      'Supervised Pharmacist',
      'Supervised Pharmacy Techs',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

// @TODO Needs refactoring.
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
  $elements = fieldset('Physical Therapy Skills Checklist');
  $skills = array(
    'Orthopedics' => array(
      'Bracing',
      'Equipment ‐ Support Device ‐ Crutch training',
      'Equipment ‐ Support Device ‐ Sling',
      'Orthotics',
    ),
    'Wound Management' => array(
      'Care of child ‐ Traumatic wound care',
    ),
    'Respiratory Treatments' => array(
      'Perform ‐ Chest physiotherapy',
    ),
    'Therapy - Pediatrics' => array(
      'Care of child ‐ Burn Care',
      'Care of child ‐ Cerebral Palsy',
      'Care of child ‐ Cystic Fibrosis',
      'Care of child ‐ Muscular Dystrophy',
      'Care of child ‐ Spina Bifida',
      'Care of child ‐ Trauma',
      'Care of child ‐ Traumatic Brain Injury',
      'Critical Care Procedures',
      'Hearing Impairments',
      'Isolation Precautions',
      'Neurologic ‐ Congenital Defects',
      'Neurologic ‐ Developmental Disabilities Sequencing Test',
      'Neurologic ‐ Head Injury',
      'Neurologic ‐ Neurodevelopmenat Deficits',
      'Neurologic ‐ Neurodevelopmental Techniques',
      'Neurologic ‐ Sensory Integrative Deficits',
      'Neurologic ‐ Spinal Cord Injury',
      'Neurologic ‐ Visual Perceptual Disorders',
      'NICU',
      'Orthopedics ‐ Birth Defects',
      'Orthopedics ‐ Developmental Diseases of Bone',
      'Orthopedics ‐ Fractures',
      'Procedure / Treatment ‐ Adaptive Equipment Assessment',
      'Procedure / Treatment ‐ Adaptive Equipment Training',
      'Procedure / Treatment ‐ CPM Machine',
      'Procedure / Treatment ‐ Dynamic Splints',
      'Procedure / Treatment ‐ Hydrotherapy',
      'Procedure / Treatment ‐ Postural Balance Training',
      'Procedure / Treatment ‐ Resting Splints',
      'Procedure / Treatment ‐ Sensory Integration Techniques',
      'Procedure / Treatment ‐ Splinting ‐ Hand',
      'Procedure / Treatment ‐ Splinting ‐ Lower Extremity',
      'Procedure / Treatment ‐ Splinting ‐ Wrist',
      'Procedure / Treatment ‐ Vision Therapy',
      'Prosthetics ‐ Lower Extremity Prosthetics',
      'Prosthetics ‐ Upper Extremity Prosthetics',
      'SICU Procedures',
    ),
    'Therapy - Adult' => array(
      'Burn Management',
      'Cardiac Rehab',
      'Care of patient ‐ Degenerative Disease',
      'Care of patient ‐ Multiple Sclerosis',
      'Care of patient ‐ Traumatic Brain Injury',
      'CCU Procedures',
      'ICU Procedures',
      'Neurologic ‐ Adaptive Equipment ‐ Wheelchair',
      'Neurologic ‐ Cognitive disorders',
      'Neurologic ‐ Head Trauma',
      'Neurologic ‐ Neuromuscular disease',
      'Neurologic ‐ Spinal Cord Injury',
      'Neurologic ‐ Stroke Rehabilitation',
      'Orthopedic ‐ Amputations',
      'Orthopedic ‐ Back Injuries/Surgeries',
      'Orthopedic ‐ Hand Injury',
      'Orthopedic ‐ Hip Fractures/Injuries',
      'Orthopedic ‐ Knee Injuries',
      'Orthopedic ‐ Neck Injuries/Surgeries',
      'Orthopedic ‐ Post operative care',
      'Orthopedic ‐ Shoulder Injury',
      'Orthopedic ‐ TMJ',
      'Orthopedic ‐ Total Hip Replacement',
      'Orthopedic ‐ Total Knee Replacement',
      'Orthopedic ‐ Upper Extremity Joint Replacement',
      'Procedure / Treatment ‐ Activities of Daily Living',
      'Procedure / Treatment ‐ Acupressure',
      'Procedure / Treatment ‐ Cervical Traction',
      'Procedure / Treatment ‐ CPM Machine',
      'Procedure / Treatment ‐ Cranialsacral Techniques',
      'Procedure / Treatment ‐ Cryotherapy',
      'Procedure / Treatment ‐ Diathermy',
      'Procedure / Treatment ‐ Electrical Stimulation',
      'Procedure / Treatment ‐ Extremity Mobilization',
      'Procedure / Treatment ‐ Functional ‐ Dynamic splinting',
      'Procedure / Treatment ‐ Functional Capacity Evaluation',
      'Procedure / Treatment ‐ Gait Training',
      'Procedure / Treatment ‐ Hubbard Tank',
      'Procedure / Treatment ‐ Hydrotherapy',
      'Procedure / Treatment ‐ Lumbar Traction',
      'Procedure / Treatment ‐ Massage',
      'Procedure / Treatment ‐ Muscle Energy Techniques',
      'Procedure / Treatment ‐ Myofacial release',
      'Procedure / Treatment ‐ Spinal Mobilization',
      'Procedure / Treatment ‐ Splinting ‐ Hand',
      'Procedure / Treatment ‐ Splinting ‐ Wrist',
      'Procedure / Treatment ‐ TENS',
      'Procedure / Treatment ‐ Therapuetic Pool',
      'Procedure / Treatment ‐ Transfers',
      'Procedure / Treatment ‐ Ultrasound',
      'Procedure / Treatment ‐ Whirlpool',
      'Procedure / Treatment ‐ Work Capacity Evaluation',
      'Prosthetics ‐ Above Knee',
      'Prosthetics ‐ Ankle/Foot Orthosis',
      'Prosthetics ‐ Below Knee',
      'Sports Medicine ‐ Athletic Injuries',
      'Sports Medicine ‐ Biodex',
      'Sports Medicine ‐ Cybex',
      'Sports Medicine ‐ LIDO',
      'Sports Medicine ‐ Nautilus/Eagle',
      'Sports Medicine ‐ Orthotron',
      'Sports Medicine ‐ Taping',
    ),
    'Therapy - Geriatrics' => array(
      'Burn Management',
      'Cardiac Rehab',
      'Exercise Programs',
      'ICU Procedures',
      'Neurologic ‐ Adaptive Equipment ‐ Wheelchair',
      'Neurologic ‐ Alzheimers / Dementia',
      'Neurologic ‐ Cognitive Disorders',
      'Neurologic ‐ Head Trauma',
      'Neurologic ‐ Neuromuscular Disease',
      'Neurologic ‐ Spinal Cord Injury',
      'Neurologic ‐ Stroke Rehabilitation',
      'Orthopedic ‐ Amputation',
      'Orthopedic ‐ Arthritis',
      'Orthopedic ‐ Back Injuries/Surgeries',
      'Orthopedic ‐ Hand Injuries',
      'Orthopedic ‐ Hip Fractures/Injuries',
      'Orthopedic ‐ Knee Injuries',
      'Orthopedic ‐ Neck Injuries/Surgeries',
      'Orthopedic ‐ Osteoarthritis',
      'Orthopedic ‐ Post Operative Care',
      'Orthopedic ‐ Shoulder Injury',
      'Orthopedic ‐ Total Hip Replacement',
      'Orthopedic ‐ Total Knee Replacement',
      'Orthopedic ‐ Upper Extremity Joint Replacement',
      'Procedure / Treatment ‐ Activities of Daily Living',
      'Procedure / Treatment ‐ Acupressure',
      'Procedure / Treatment ‐ Cervical Traction',
      'Procedure / Treatment ‐ CPM Machine',
      'Procedure / Treatment ‐ Cranialsacral Techniques',
      'Procedure / Treatment ‐ Cryotherapy',
      'Procedure / Treatment ‐ Diathermy',
      'Procedure / Treatment ‐ Documentation ‐ Medicare A',
      'Procedure / Treatment ‐ Documentation ‐ Medicare B',
      'Procedure / Treatment ‐ Documentation ‐ State Healthcare',
      'Procedure / Treatment ‐ Electrical Stimulation',
      'Procedure / Treatment ‐ Extremity Mobilization',
      'Procedure / Treatment ‐ Gait Training',
      'Procedure / Treatment ‐ Hubbard Tank',
      'Procedure / Treatment ‐ Hydrotherapy',
      'Procedure / Treatment ‐ Lumbar Traction',
      'Procedure / Treatment ‐ Massage',
      'Procedure / Treatment ‐ Spinal Mobilization',
      'Procedure / Treatment ‐ Splinting ‐ Hand',
      'Procedure / Treatment ‐ Splinting ‐ Wrist',
      'Procedure / Treatment ‐ Sterilization Techniques',
      'Procedure / Treatment ‐ TENS',
      'Procedure / Treatment ‐ Therapuetic Pool',
      'Procedure / Treatment ‐ Transfers',
      'Procedure / Treatment ‐ Ultrasound',
      'Procedure / Treatment ‐ Whirlpool',
      'Prosthetics ‐ Above Knee',
      'Prosthetics ‐ Ankle/Foot Orthosis',
      'Prosthetics ‐ Below Knee',
      'Upper Extremity Prosthetics',
    ),
    'Medical Proficiency' => array(
      'Documentation',
    ),
  );
  oa_portal_checklist($elements, $skills);
  return $elements;
}

function oa_portal_p1_checklist_form_519() {
  $elements = fieldset('PICU Skills Checklist');
  $skills = array(
    'Cardiovascular' => array(
      'Assessment - Ausculation (rate, rhythm, volume)',
      'Assessment - Blood pressure/invasive (A-line)',
      'Assessment - Heart sounds/murmurs',
      'Assessment - Perfusion',
      'Care of child - Bacterial Endocarditis',
      'Care of child - Cardiac Arrest',
      'Care of child - Cardiac Transplant',
      'Care of child - Cardiogenic shock',
      'Care of child - Cardiomyopathy',
      'Care of child - Congenital heart defects',
      'Care of child - Congestive Heart Failure',
      'Care of child - Myocarditis',
      'Care of child - Pericarditis',
      'Care of child - Post cardiac cath',
      'Care of child - Post cardiac surgery',
      'Care of child - Rheumatic fever',
      'Care of child - Shock',
      'Medications - Digoxin',
      'Medications - Dobutamine',
      'Medications - Dopamine',
      'Medications - Epinepherine',
      'Medications - Nipride',
      'Medications - Nitroglycerin',
      'Medications - Sodium Bicarbonate',
      'Monitoring - CVP',
      'Monitoring - EKG Interpretation',
      'Monitoring - Pulmonary artery/capillary wedge',
      'Monitoring - Right arterial/left arterial',
      'Perform - Controlled cardioversion',
      'Perform - Emergency defibrillation',
    ),
    'Pulmonary' => array(
      'Airway Management - Endotracheal/suctioning',
      'Airway Management - Nasal/suctioning',
      'Airway Management - Oropharyngeal/suctioning',
      'Airway Management - Tracheostomy/suctioning',
      'Apnea monitor',
      'Assessment - Breath sounds',
      'Assessment - Rate and work of breathing',
      'Assist with Emergency tracheostomy',
      'Assist with setup/insertion of Chest Tube',
      'Care of child - Asthma',
      'Care of child - Bronchiolitis (RSV)',
      'Care of child - Bronchopulmonary dysplasia (BPD)',
      'Care of child - Cystic Fibrosis',
      'Care of child - Epiglotitis',
      'Care of child - Flail Chest',
      'Care of child - Foreign body aspiration',
      'Care of child - Fresh Tracheostomy',
      'Care of child - Hemothorax',
      'Care of child - LTB/croup',
      'Care of child - Pertussis',
      'Care of child - Pneumonia',
      'Care of child - RDS',
      'Care of child - Respiratory Failure',
      'Care of child - Status Asthmaticus',
      'Care of child - Tonsillitis',
      'Care of child - Tuberculosis',
      'ECMO',
      'End tidal CO2',
      'Interpret - Arterial Blood Gases',
      'Interpret - X-ray reports',
      'Medications - Alupent',
      'Medications - Aminophylline',
      'Medications - Isuprel',
      'Medications - Prostaglandin',
      'Medications - Ventolin',
      'Oxygen therapy and medication delivery via Bag and mask',
      'Oxygen therapy and medication delivery via Bulb syringe',
      'Oxygen therapy and medication delivery via Face mask',
      'Oxygen therapy and medication delivery via Isolette',
      'Oxygen therapy and medication delivery via Nasal canula',
      'Oxygen therapy and medication delivery via Tent',
      'Oxygen therapy and medication delivery via Trach collar',
      'Perform - Chest physiotherapy',
      'Pulse Oximetry',
      'Ventilator care - CPAP/PEEP',
      'Ventilator care - High frequency jet ventilator',
      'Ventilator care - Home ventilator',
      'Ventilator care - IMV',
      'Ventilator care - Pressure ventilator',
      'Ventilator care - Volume ventilator',
      'Ventilator care - Weaning',
      'Water seal drainage system',
    ),
    'Neurological' => array(
      'Assessment - Level of consciousness',
      'Assist with lumbar puncture',
      'Care of child - Brain death',
      'Care of child - Closed head trauma',
      'Care of child - Coma',
      'Care of child - Encephalitis',
      'Care of child - External VP Shunts',
      'Care of child - Head trauma',
      'Care of child - Meningitis',
      'Care of child - Near drowning',
      'Care of child - Neuromuscular disease',
      'Care of child - Post Neurosurgery',
      'Care of child - Spinal cord injury',
      'Care of child - Status epilepticus',
      'ICP Management - Medications',
      'ICP Management - Positioning',
      'ICP Management - Regulation of ICP',
      'ICP Management - Temperature control',
      'ICP Management - Ventilation',
      'Medications - Carbmazepine',
      'Medications - Clonazepam',
      'Medications - Dilantin',
      'Medications - Phenobarbital',
      'Medications - Valium',
      'Monitoring - Intracranial pressure',
      'Monitoring - Seizure precautions',
      'Monitoring - Spinal precautions',
    ),
    'Gastrointestinal' => array(
      'Assessment - Abdominal/bowel sounds',
      'Assessment - Nutrition',
      'Care of child - Cleft lip/palate',
      'Care of child - Colostomy',
      'Care of child - Diaphragmatic hernia',
      'Care of child - Failure To Thrive (FTT)',
      'Care of child - Fresh ostomy',
      'Care of child - Gastroenteritis/dehydration',
      'Care of child - GE reflux',
      'Care of child - GI bleeding',
      'Care of child - Ileostomy',
      'Care of child - Internal parasites',
      'Care of child - Necrotizing enterocolitis (NEC)',
      'Care of child - Pyloric stenosis',
      'Care of child - Surgical abdomen',
      'Care of child - Ulcerative colitis',
      'Feedings - Bottle',
      'Feedings - Central hyperalimentation',
      'Feedings - Gastrostomy/button',
      'Feedings - Gavage',
      'Feedings - I-tubes',
      'Feedings - Jejunal',
      'Feedings - Peripheral hyperalimentation',
      'Monitoring - Penrose drain',
      'Perform - placement of NG tube',
      'Perform - Salem pump to suction',
      'Perform - Wound irrigation/dressing change',
    ),
    'Renal/Genitourinary' => array(
      'Assessment - Fluid and Electorlyte balance',
      'Assist with Supra-pubic tap',
      'Care of child - Acute Renal Failure',
      'Care of child - CAVH',
      'Care of child - Diabetes Insipidus',
      'Care of child - Hemodialysis',
      'Care of child - Hemolytic Uremic Syndrome (HUS)',
      'Care of child - Illeal conduit urethral',
      'Care of child - Infantile polycystic disease',
      'Care of child - Kidney transplant',
      'Care of child - Nephrotic syndrome',
      'Care of child - Peritoneal dialysis',
      'Care of child - Post Renal transplant',
      'Care of child - SIADH',
      'Care of child - Wilms Tumor',
      'Insert and Care for 3-way Foley',
      'Insert and Care for indwelling catheter',
      'Insert and Care for straight catheter',
      'Insert and Care for Supra-pubic catheter',
      'Interpret - BUN and Creatinine',
      'Interpret - Serum electrolytes',
      'Interpret - Urinalysis',
      'Perform - Insert and Care for Foley',
      'Perform - Urine specimen collection (routine)',
    ),
    'Endocrine/Metabolic' => array(
      'Assessment - Fluid and Electrolyte balance',
      'Care of child - Adrenal disorders',
      'Care of child - Cushing\'s syndrome',
      'Care of child - Juvenile diabetes',
      'Care of child - Pituitary disorders',
      'Care of child - Thyroid malfunction',
      'Interpret - Blood Glucose',
      'Interpret - Thyroid Studies',
      'Medications - Growth hormone',
      'Medications - Insulin',
      'Medications - Thyroid',
      'Perform - Blood Glucose Monitoring: Finger Stick',
    ),
    'Hematology/Oncology' => array(
      'Assessment - Nutritional status',
      'Care of child - Anemia',
      'Care of child - Bone Marrow Transplant',
      'Care of child - Depressed immune system',
      'Care of child - Disseminated Intravascular Coagulation (DIC)',
      'Care of child - Hemophilia',
      'Care of child - Hodgkin\'s disease',
      'Care of child - Infectious Mononucleosis',
      'Care of child - Leukemia',
      'Care of child - Malignanat tumors',
      'Care of child - Sickle cell anemia',
      'Care of child - spleen trauma/splenectomy',
      'Interpret - blood chemistry',
      'Interpret - blood counts',
      'Medications - Chemotherapy',
      'Medications - Prednisone',
      'Perform - Reverse isolation',
    ),
    'Infectious Disease' => array(
      'Care of child - AIDS',
      'Care of child - Childhood communicable diseases',
      'Care of child - Cytomegalovirus (CMV)',
      'Care of child - Hepatitis',
      'Care of child - Kawasaki Disease',
      'Care of child - Lyme Disease',
      'Interpret - Blood counts',
      'Perform - Fever Management',
      'Perform - Isolation',
      'Perform - Understand and follow Universal Precautions',
    ),
    'Orthopedics' => array(
      'Care of child - Cervical traction',
      'Care of child - Osteogenic sarcoma',
      'Care of child - Pinned fractures',
      'Care of child - Post vertebral surgery',
      'Equipment - Specialized orthopedic bed',
    ),
    'Wound Management' => array(
      'Assessment - skin for impending breakdown',
      'Assessment - stasis ulcers',
      'Assessment - surgical wound healing',
      'Care of child - 1st degree burns',
      'Care of child - 2nd degree burns',
      'Care of child - 3rd degree burns',
      'Care of child - Gunshot wound',
      'Care of child - Staged decubitis ulcers',
      'Care of child - Surgical wounds with drain(s)',
      'Care of child - Traumatic wound care',
      'Equipment - Air fluidized, low air loss beds',
      'Perform - sterile dressing change',
      'Perform - wound care/irrigation',
    ),
    'Pain Management' => array(
      'Assessment - Pain level/tolerance',
      'Care of child - Epidural anesthesia/analgesia',
      'Care of patient - IV Conscious Sedation',
      'Care of patient - Narcotic analgesia',
    ),
    'Respiratory Treatments' => array(
      'Monitor Medication Effects - Corticosteroids',
    ),
    'Phlebotomy/IV Therapy' => array(
      'Assessment - Site and patency',
      'Assist with cutdown',
      'Care of child - Broviac',
      'Care of child - Groshong',
      'Care of child - Hickman',
      'Care of child - Peripheral line/dressing',
      'Care of child - Portacath',
      'Care of child - Quinton',
      'Perform - Administer blood products - autotransfusion',
      'Perform - Administer Cryoprecipitate',
      'Perform - administer Packed RBC\'s',
      'Perform - administer Whole blood',
      'Perform - Draw blood from central line',
      'Perform - Draw venous blood',
      'Perform - start IV (angiocath)',
      'Perform - start IV (butterfly)',
      'Perform - start IV (heparin lock)',
    ),
    'Miscellaneous' => array(
      'Assessment - Immunization schedule',
      'Assessment - Normal growth and development',
      'Assessment - Normal laboratory values',
      'Assessment - Recognize signs of abuse or neglect',
      'Calculation of Pediatric doses',
      'Care of child - Anorexia/bulimia',
      'Care of child - Craniofacial reconstruction',
      'Care of child - Depression',
      'Care of child - ENT surgery',
      'Care of child - Eye surgery',
      'Care of child - Ingestion of foreign body',
      'Care of child - Ingestion of poison or toxin',
      'Care of child - Plastic surgery',
      'Care of child - Suicide threats/actions',
      'Eye/ear instillations',
      'Knowledge of emergency drugs',
      'Knowledge of routine pediatric drugs',
      'Medications - Emergency drug action and reaction',
      'Metered dose inhaler',
      'Perform - obtain consents (for immunizations, procedures, treatments)',
    ),
  );
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
  $candidate = get_candidate($account->rmaxid, $account->uid);

  $options = array();
  foreach ($candidate->p['p2']['licensure_certs']['#formtypes_open'] AS $ft) {
    $options['Required'][$ft] = $all_cl[$ft];
    unset($all_cl[$ft]);
  }

  $options['Other'][61] = t('Other');
  $options['Other'] = $options['Other'] + $all_cl;

  $form['lc']['lc_licensure'] = select('Select which certificate you are uploading', $options, FALSE);
  unset($form['lc']['lc_licensure']['#required']);
  unset($form['lc']['lc_licensure']['#rules']);
  $form['lc']['upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload file'),
//    '#required' => TRUE,
    '#size' => 80,
  );
  $form['lc']['chk_fax_preference'] = checkbox('Check this box if you prefer to fax copies of your licensures/certifications');
  $form['lc']['fax'] = markup('<div id="cover-letter-box" class="oap-fax-download set-hide">Please include this cover letter and fax it to (818) 555-5555. <a href="/portal/createcoversheet/" id="cover-letter-link">Cover Letter</a></div>');

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
  $candidate = get_candidate($account->rmaxid, $account->uid);

  // If fax preferred, suppress all open formtypes
  if ($form_state['values']['lc']['chk_fax_preference'] == 1) {
    foreach ($candidate->p['p2']['licensure_certs']['#formtypes_open'] AS $ft) {
      oap_account_add_suppressed($account, $ft);
    }
  }

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
    // Delay a bit so we'll catch the files on the way back. 
    sleep(6);
  }

  $candidate = get_candidate($account->rmaxid, $account->uid, TRUE);
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
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);
  $form = array();
  $form[233] = fieldset('Background Investigation Consent');
  $form[233]['ack'] = acknowledgement('<p>I hereby authorize On Assignment, and/or its agents to make an independent investigation of my background, references, character, past employment, education, credit history, motor vehicle operation history, criminal or police records, including those maintained by both public and private organizations and all public records for the purpose of confirming the information contained on my Application and/or obtaining other information which may be material to my qualifications for employment.  I release On Assignment and/or its agents and any person or entity, which provides information pursuant to this authorization, from any and all liabilities, claims or law suits in regards to the information obtained from any and all above referenced sources used.  I understand that all or part of this information, including my social security number may be released to clients as part of the hiring process, and agree to the release of any part or all of this information including my social security number.</p><br />
<p>This is a consumer notification that a Background Report will be requested and obtained, and that the report will be used for the purpose of evaluating me for employment, promotion, reassignment or retention as an employee.</p><br />');

  $form[233]['receivereport'] = checkbox('I request to receive a free copy of this report by checking this box.'); 
  $form[233]['receivereport']['#prefix'] = '<p>As a resident of Minnesota and Oklahoma only, I have a right to obtain a copy of this report by checking the box provided below.</p><br />
<p>NOTICE TO CALIFORNIA CANDIDATES<br /><p>You have a right to obtain a copy of any consumer report or investigative consumer report obtained by On Assignment, Inc by checking the box provided below. The report will be provided to you within three (3) business days after we receive the requested reports related to the matter investigated. Under section 1786.22 of the California Civil Code, you may view the file maintained on you by GIS (the search company) during normal business hours. You may also obtain a copy of this file upon submitting proper identification and paying the costs of duplication services, by appearing at GIS in person or by mail. You may also receive a summary of the file by telephone. The agency is required to have personnel available to explain your file to you and the agency must explain to you any coded information appearing in your file. If you appear in person, a person of your choice may accompany you, provided that this person furnishes proper identification.</p>';

  $form[233]['is_correct'] = markup('<p><strong>The following is my true and complete legal name and all information is true and correct to the best of my knowledge:</strong></p>');
  $form[233]['maiden_name'] = textfield('Maiden/other names used');
  $form[233]['maiden_name']['#required'] = FALSE;
  $form[233]['dob'] = date_popup('Date of birth');
  $form[233]['dob']['#date_year_range'] = '-14:-80';
  $form[233]['ssn'] = textfield_ssn();
  $form[233]['dlnumber'] = textfield('Driver License Number', 20);
  $form[233]['stateoflicense'] = state('Driver License State');
  $form[233]['frompresentaddr'] = date_popup('Lived at your current address since', 'm-Y');
  $form[233]['frompresentaddr']['#date_year_range'] = '0:-65';
  $form[233]['frompresentaddr']['#suffix'] = '<strong>Note: We need 7 years of address history. Please add all addresses for the past 7 years.</strong>';
  $form[233]['topresentaddr'] = array(
    '#type' => 'value',
    '#value' => 'Present',
  );
  $fmr = array();
  // $fmr['title'] = markup('<h4>Former Address</h4>');
  $fmr['title'] = markup('<h1>Former Address</h1>');
  $fmr['fromformeraddr'] = date_popup('From', 'm-Y');
  $fmr['fromformeraddr']['#date_year_range'] = '0:-65';
  $fmr['toformeraddr'] = date_popup('To', 'm-Y');
  $fmr['toformeraddr']['#date_year_range'] = '0:-65';
  $fmr = $fmr + address();
  unset($fmr['address2']);
  // None are required
  foreach ($fmr AS $k => $v) {
    unset($fmr[$k]['#required'], $fmr[$k]['#rules']);
  }
  $form[233]['former_addresses'] = oa_portal_set($fmr, 'Add another address', 7);
  $form[233]['chk_certification'] = checkbox('I certify...');

  $ref = array('#tree' => TRUE);
  $ref['employer'] = select('Company', $account->oa_references + array('Other'));
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
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);
  
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
  foreach ($form['#formtypes_open'] AS $formtype) {
    $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'][$formtype], $form[$formtype]));
    $html_string = theme('oap_pdf_wrapper', $candidate, 'Needs title', $body_string);
    $result = oa_portal_submitdrupalform($account->rmaxid, $formtype, $html_string);
    if ($result->StatusInfo->statusCode !== 0) {
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
  $candidate = get_candidate($account->rmaxid, $account->uid);

  /**
   * @TODO Post 5 Job Capabilities Questionnaire to UCM
   */
  dsm($form_state['values'][5], 'Process & post 5 to UCM');
  $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'][5], $form[5]));
  $html_string = theme('oap_pdf_wrapper', $candidate, $form[5]['#title'], $body_string);
  dsm($html_string, 'PDF string');
  $result = oa_portal_submitdrupalform($account->rmaxid, 5, $html_string);
  if ($result->StatusInfo->statusCode === 0) {
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
  $candidate = get_candidate($account->rmaxid, $account->uid);

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

  // Inject data from the RMax Profile
  // PDF Field => RMax Profile Field
  // @TODO Move into form callback as hidden fields?
  foreach (array(
    // 'personnelname' => 'name',
    'personneladdress1' => 'addressline1',
    'personneladdress2' => 'addressline2',
    'personneladdress3' => 'addressline3',
    ) AS $pdff => $rmaxf) {
    $form_state['values'][433][$pdff] = $candidate->profile->{$rmaxf};
  }
  
  // Create fake form items for the data we're injecting into $values
  // $form[433]['personnelname']['#title'] = 'Name';
  $form[433]['personneladdress1']['#title'] = 'Address 1';
  $form[433]['personneladdress2']['#title'] = 'Address 2';
  $form[433]['personneladdress3']['#title'] = 'Address 3';

  $error = FALSE;
  foreach ($form['#formtypes_open'] AS $formtype) {
    $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'][$formtype], $form[$formtype]));
    $html_string = theme('oap_pdf_wrapper', $candidate, 'Needs title', $body_string);
    dsm($html_string, 'PDF string');
    $result = oa_portal_submitdrupalform($account->rmaxid, $formtype, $html_string);
    if ($result->StatusInfo->statusCode !== 0) {
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
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);

  $form = array();
  // $form['#description'] = t('You are required to complete....');

  // @TODO Get a test link from ___?
  $form['take_the_test'] = markup('<p>Please click on the link below and follow the instructions to complete the required Competency Exams for your specialty.</p>
    <p>Login Information:<br />
    Link to the Competency Exam testing site: <a href="http://oaallied.nursecompetency.com">http://oaallied.nursecompetency.com</a><br />
    User Login: enter your email address<br />
    Password: pass123</p>

    <p>You will be prompted to create an account by entering the following information:
    <ul>
    <li>Primary Role: (Your Recruiter will tell you which Role to select)</li>
    <li>First Name: <strong>'. $candidate->profile->fname .'</strong></li>
    <li>Last Name: <strong>'. $candidate->profile->lname .'</strong></li>
    <li>Email Address: <strong>'. $account->mail .'</strong></li>
    <li>Confirm Email Address: <strong>'. $account->mail .'</strong></li>
    <li><strong>Click Register</strong></li>
    </ul></p>

    <p>Continue to follow the prompts to complete the required Competency Exams.</p>
    <p>Important: If you exit the tests before submitting it for grading, it will not save and you will need to take them again. You will be given different questions on the tests the next time you log in. On Assignment requires a minimum score of 80% on all tests. You may retake the tests as many times as you’d like in order to meet the score requirements. Once you’ve created an account you will receive your Login and Password which you will use when returning to the site.</p>
    <p>If you have questions or need assistance logging in to the test site, please call Employee Services at (800)-995-7378.</p>');
  return $form;
}

/**
 * Form callback for Paycheck Method
 */
function oa_portal_p3_paycheck_form($form_state) {
  $form = array();

  // @TODO Add JS: expose either Direct Deposit or CashPay Enrollment conditionally based on rdo_delivery_option status
  $form['paycheck_delivery'] = fieldset('Paycheck Delivery');
  $form['paycheck_delivery']['delivery_option'] = radios('', array(
    'directdeposit' => '<strong>I do</strong> wish to sign up for Direct Deposit at this time and will complete the section below.',
    // 'cashpay' => '<strong>I do</strong> wish to sign up for CashPay at this time and will complete the section below.',
    'no' => '<strong>I do not</strong> wish to sign up for Direct Deposit at this time. I would prefer to have my pay check mailed to me. I understand that there may be a delay to my paycheck delivery via the US Postal Service which is beyond the control of On Assignment.',
  ));
  $form['paycheck_delivery']['policy'] = markup('<p>Check Replacement Policy<br/>On Assignment requires that you wait 5 business days from the date the check was mailed before requesting a replacement check. Additionally, if a check replacement is needed, you will be rquired to sign up for Direct Deposit or CashPay.</p>');

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
  if ($form_state['values']['paycheck_delivery']['delivery_option'] == 'directdeposit') { 
    foreach(array('bank_name', 'bank_city', 'bank_state', 'bank_phone', 'bank_account', 'bank_route', 'account_type') AS $k) {
      if (!$form_state['values'][536][$k]) {
        form_set_error('536]['. $k, 'You must specify a value for '. $form[536][$k]['#title']);
      }
    }
  }
}

/**
 * Form submission callback for Paycheck Method
 */
function oa_portal_p3_paycheck_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);
  
  if ($form_state['values']['paycheck_delivery']['delivery_option'] == 'directdeposit') {
    dsm($form_state['values'][536], 'Process & post 536 to UCM');
    $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'][536], $form[536]));
    $html_string = theme('oap_pdf_wrapper', $candidate, 'Direct Deposit', $body_string);
    $result = oa_portal_submitdrupalform($account->rmaxid, 536, $html_string);
    if ($result->StatusInfo->statusCode === 0) {
      $form_state['redirect'] = 'portal/p3/w4';
    }
    else {
      drupal_set_message('An error occured. Please try again.');
    }
  }
  else {
    // @TODO Mark this section as complete anyway.
    // Suppress 536/537 DirectDeposit/CashPay
    oap_account_add_suppressed($account, 536);
    oap_account_add_suppressed($account, 537);
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
  $form[474]['totalclaims'] = textfield('Total number of allowances you are claiming', 6);
  $form[474]['addtnlamount'] = textfield('Additional amount, if any, you want withheld from each paycheck', 6);
  $form[474]['exempt_desc'] = markup('<strong>I claim exemption from withholding for 2012, and I certify that I meet both of the following conditions for exemption:</strong><br/>
  <ul><li>Last year I had a right to a refund of all federal income tax withheld because I had no tax liability, and </li><li>This year I expect a refund of all federal income tax withheld because I expect to have no tax liability</li></ul><strong>');
  $form[474]['exempt'] = textfield('If you meet both conditions, write "Exempt" here', 6);
  $form[474]['chk_certify'] = checkbox('Under penalties of perjury, I declare that I have examined this certificate and, to the best of my knowledge and belief, it is true, correct, and complete.');
  $form[474]['firstnameandinitial'] = hidden($candidate->profile->fname);
  $form[474]['lastname'] = hidden($candidate->profile->lname);
  $form[474]['address'] = hidden($candidate->profile->address);
  $form[474]['citystatezip'] = hidden($candidate->profile->city .', '. $candidate->profile->state .' '. $candidate->profile->postal);
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
  $form[461]['dob']['#date_year_range'] = '-80:-14';
  $form[461]['maiden'] = textfield('Maiden/other names used');
  $form[461]['maiden']['#required'] = FALSE;
  $form[461]['citizentype'] = radios('I attest, under penalty of perjury, that I am (check one of the following)',
    array('citizen' => t('A citizen of the United States'), 'national' => t('A noncitizen national of the United States'), 'resident' => t('A lawful permanent resident'), 'alien' => t('An alien authorized to work') ));
  $form[461]['expiry'] = date_popup('Until (expiration date if applicable)');
  $form[461]['expiry']['#required'] = FALSE;
  $form[461]['expiry']['#prefix'] = '<div class="set-hide" id="i9-expiry">';
  $form[461]['expiry']['#suffix'] = '</div>';
  $form[461]['certify'] = checkbox('I am aware that federal law provides for imprisonment and/or fines for false statements or use of false documents in connection with the completion of this form.');
  $form[461]['verification'] = checkbox('I am aware that I must provide two (2) verification documents prior to employment. <span id="i9-verification-docs" class="fake-link">Click Here</span> to view a list of acceptable documents.</div>');
  $form[461]['verification']['#suffix'] = '<div id="i9-verification-box" class="set-hide"><div id="i9-verification"><span id="i9-verification-close" class="fake-link">CLOSE [X]</span><img src="/sites/all/modules/oahealthportal/img/i9-verification-docs.png" width="600" /></div></div>';
  return $form;
}

/**
 * Form submission callback for I-9
 */
function oa_portal_p3_i9_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);

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
  $form_state['values'][461]['expiry'] = ($form_state['values'][461]['expiry']) ? date('m/d/Y', strtotime($form_state['values'][461]['expiry'])) : NULL;
  $form_state['values'][461]['appsignature'] = $candidate->profile->name;
  $form_state['values'][461]['apptimestamp'] = date('m-d-Y g:i:s A');
  $form_state['values'][461]['ssn'] = implode($form_state['values'][461]['ssn'], '-');
  unset($form_state['values'][461]['certify'], $form_state['values'][461]['verification']);

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
  $form[1]['#description'] = t('Please fill out the Affirmative Action date below. To ensure that On Assignment complies with pertinent hiring practices, On Assignment must keep records about our applicants for employment.  This questionnaire will be kept in a confidential file, seperate from the application for employment.  Failure to provide this information will in no way adversely affect your candidacy for this position.  However, we would greatly appreciate your willingness to submit this information for our commitment to equal opportunity.<br/><br/><em>Note that all definitions as listed below were provuded by the U.S. Federal Government Equal Employment Opportunity Commission (EEOC)</em>');
  $form[1]['ethnicity'] = radios('Ethnicity Code', array('hispanic' => t('Hispanic or Latino'),'asian' => t('Asian'),'white' => t('White'),'american_indian' => t('American Indian or Alaskan Native'),'black' => t('Black'),'two_more' => t('Two or more races'),'hawaiian' => t('Native Hawaiian or Pacific Islander') ) );
  $form[1]['gender'] = radios('Gender', array('male' => t('Male'), 'female' => t('Female')));
  $form[1]['handicap'] = radios_yesno('Handicap Status');
  $form[1]['handicap']['#suffix'] = t('<em>(Any person who has a physical or mental impairment that substantially limits one or more major life activities, has a record of such impairments or is regarded as having such impairments)</em>');
  $form[1]['veteran'] = checkboxes('Veteran Status', array('armed_forces' => t('Armed Forces Service Medal Veteran'), 'disabled' => t('Disabled Veteran'), 'protected' => t('Other Protected Veteran'), 'seperated' => t('Recently Seperated Veteran')) );
  $form[1]['veteran']['#description'] = t('<em>If you are unsure of your Veteran status under these definitions, contact your recruiter.</em>');
  $form[1]['veteran']['#required'] = FALSE;
  $form[1]['age'] = radios_yesno('Age');
  $form[1]['age']['#description'] = '<em>'. t('Are you over age 40 but under age 70?') .'</em>';
  
  return $form;
}

/**
 * Form submission callback for EEO Form
 */
function oa_portal_p3_eeo_form_submit($form, &$form_state) {
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);
  $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'][1], $form[1]));
  $html_string = theme('oap_pdf_wrapper', $candidate, 'EEO', $body_string);
  dsm($html_string, 'PDF string');
  $result = oa_portal_submitdrupalform($account->rmaxid, 1, $html_string);
  if ($result->StatusInfo->statusCode === 0) {
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
  global $user;
  $account = user_load($user->uid);
  $candidate = get_candidate($account->rmaxid, $account->uid);

  /**
   * Post 466 Work Policy Acknowledgement to UCM. Applies to all sections in this form.
   */
  dsm($form_state['values'], 'Process & post 466 to UCM');

  $body_string = drupal_render(oa_portal_html_to_pdf_change_type($form_state['values'], $form));
  $html_string = theme('oap_pdf_wrapper', $candidate, 'Work Policy Acknowledgement', $body_string);
  $result = oa_portal_submitdrupalform($account->rmaxid, 466, $html_string);
  if ($result->StatusInfo->statusCode === 0) {
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
    $candidate = get_candidate($account->rmaxid, $account->uid);
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
        // Numeric fieldsets (most)
        if (is_numeric($ek) && !in_array($ek, $form['#formtypes_open'])) {
          // Set this fieldset as empty and locked
          $form[$ek] = fieldset_locked($element['#title']);
        }
        // Application
        elseif (in_array($ek, array('emergency_contact', 'education', 'experience', 'preferences', 'licensures_certs', 'organizations', 'application_terms')) 
          && !count(array_intersect(array(2), $form['#formtypes_open']))) {
          // Set this fieldset as empty and locked
          $form[$ek] = fieldset_locked($element['#title']);
          unset($form['application_terms']);
        }
        // Medical
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
          }
        }
        // Paycheck - 536
        elseif ($ek == 'paycheck_delivery' && !count(array_intersect(array(536), $form['#formtypes_open']))) {
          unset($form[$ek]);
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
      if (arg(3) == 'docs') {
        sleep(5);
      }
      $form['#docs'] = $candidate->get_docs_not_missing($form['#formtypes']);
      $form['docs'] = array(
        '#value' => theme('oap_doc_download', $form['#docs']),
        '#weight' => 1997,
      );

      // Return json if we need it. Kind of filthy, but necessary.
      if (arg(3) == 'docs') {
        drupal_json(array('docs_markup' => drupal_render($form['docs'])));
        exit();
      }
      if (count($form['#formtypes_open'])) {
        $form['docs']['#prefix'] = '<div class="set-hide">';
        $form['docs']['#suffix'] = '</div>';
      }
    }



    // Add settings for Autosaving
    if ($form_id != 'oa_portal_p1_checklist_form') {
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
    }

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
      $elements[$section]['#prefix'] = '<div class="checklist-section"><div class="checklist-scale">1 2 3 4</div>';
      $elements[$section]['#suffix'] = '</div>';
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
