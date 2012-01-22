<?php

/**
 * Form callback for onboarding form
 */
function oa_portal_p1_application_form($form_state) {
  $form = array();
  $fields = new oafieds();

  $form['intro'] = array(
    '#type' => 'markup',
    '#value' => t('If you are ready to begin working.....'),
  );
  
  // Contact Information
  $form['contact'] = fieldset('Contact Information');

  $form['contact']['emerg1_name'] = textfield('Name', array('#prefix' => 'Emergency Contact 1'));
  $form['contact']['emerg1_phone'] = textfield('Phone');
  $form['contact']['emerg2_name'] = textfield('Name', array('#prefix' => 'Emergency Contact 2'));
  $form['contact']['emerg2_phone'] = textfield('Phone');

  // Education
  $form['education'] = textfield('Education');
  $edu_fields = array();
  $edu_fields['school'] = textfield('School', array('#options' => array('opt1' => 'Opt 1')));
  $edu_fields['grad_status'] = array(
    '#type' => 'radios',
    '#title' => t('Did you graduate'),
    '#options' => array(1 => 'Yes', 0 => 'No'),
    '#required' => TRUE,
  );
  $edu_fields['degree'] = array(
    '#type' => 'select',
    '#title' => t('Degree Earned'),
    '#options' => array('associates' => 'Associates'),
    '#required' => TRUE,
  );
  $edu_fields['city'] = textfield('City');
  $edu_fields['state'] = textfield('State');
  $edu_fields['year'] = textfield('Year');
  $edu_fields['major'] = textfield('Major');
  $edu_fields['minor'] = textfield('Minor');
  $edu_fields['coursework1'] = textfield('Relevant Course Work');
  $edu_fields['coursework2'] = textfield();

  // @TODO Enable multiple entry
  $form['education']['edu'][] = $edu_fields;
  $form['education']['edu'][] = $edu_fields;
  
  // Experience
  $form['experience'] = fieldset('Experience');
  
  $exp_fields['employer'] = textfield('Employer');
  $exp_fields['city'] = textfield('City');
  $exp_fields['state'] = textfield('State', 2);
  $exp_fields['start_date'] = array(
    '#type' => 'date',
    '#title' => t('Start Date'),
  );
  $exp_fields['end_date'] = array(
    '#type' => 'date',
    '#title' => t('End Date'),
  );
  $exp_fields['present'] = array(
    '#type' => 'checkbox',
    '#title' => t('Present'),
  );
  $exp_fields['title'] = textfield('Title');
  $exp_fields['salary'] = textfield('Salary');
  $exp_fields['salary_period'] = array(
    '#type' => 'radios',
    '#title' => t('Salary'),
    '#options' => array('annual' => t('Annual'), 'hourly' => t('Hourly'))
  )
  $exp_fields['description'] = textarea('Description', 5);

  $form['experience']['exp'][] = $exp_fields;
  
  // Preferences
  $form['preferences'] = fieldset('Preferences');
  $form['preferences']['distance'] = textfield('How far are you willing to commute on a daily basis?', 3, array(
    '#suffix' => t('Miles'),
  ));
  
  $form[''][''] = array(
    '#type' => '',
    '#title' => t(''),
    '#options' => array('opt1' => 'Opt 1'),
  );
  
  
  // Licensures & Certifications
  $form['licecerts'] = fieldset('Licensures/Certifications');

  $lc_fields['name'] = textfield('Licensure/Certification');
  $lc_fields['state'] = textfield('State', 2);
  $lc_fields['number'] = textfield('License Number', 12);
  $lc_fields['expiry_date'] = array(
    '#type' => 'date',
    '#title' => t('Expires'),
    '#required' => TRUE,
  );
  $form['licecerts']['lc'][] = $lc_fields;
  
  
  $form['proforgs'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#tree' => TRUE,
  );
  $form[''][''] = array(
    '#type' => '',
    '#title' => t(''),
    '#options' => array('opt1' => 'Opt 1'),
  );
  
  $form['terms'] = terms();

  oa_portal_submit(&$form);
  return $form;
}

function oa_portal_submit(&$form) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Finalize'),
  );
  // DO something else to enable the fancy inline form submission
}


function oa_portal_p1_contact_form() {
  $form['contact']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#required' => TRUE,
  );

  $form['contact']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#required' => TRUE,
  );
  
  
}