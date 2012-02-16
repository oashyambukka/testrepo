<?php

function markup($value) {
  return array('#value' => t($value));
}

function fieldset($title, $description = NULL) {
  $element = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t($title),
    '#description' => $description,
    '#collapsible' => TRUE,
  );
  return $element;
}

function textfield($title, $size = 60, $maxlength = null) {
  $element = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t($title),
    '#size' => $size,
    '#maxlength' => $maxlength,
    '#filters' => array('trim'),
  );
  return $element;
}

function email($title = null) {
  $element = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => ($title) ? t($title) : t('Email'),
    '#maxlength' => 100,
    '#attributes' => array('class' => 'textfield-email'),
    '#rules' => array('email'),
    '#filters' => array('trim'),
  );
  return $element;
}

function textfield_sin() {
  $element = array(
    '#type' => 'fieldset',
    '#title' => t('Social Security Number:'),
    '#attributes' => array('class' => 'sin-field'),
    '#tree' => TRUE,
  );
  $el = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#rules' => array('numeric'),
    '#filters' => array('trim'),
  );
  $element[] = $el;
  $element[] = $el;
  $element[] = $el;
  return $element;
}

function textarea($title) {
  $element = array(
    '#type' => 'textarea',
    '#required' => TRUE,
    '#title' => t($title),
  );
  return $element;
}

function checkbox($title) {
  $element = array(
    '#type' => 'checkbox',
    '#title' => t($title),
  );
  return $element;
}

function checkboxes($title, $choices, $map = FALSE) {
  $element = array(
    '#type' => 'checkboxes',
    '#title' => t($title),
    '#options' => ($map) ? drupal_map_assoc($choices) : $choices,
    '#required' => TRUE,
  );
  return $element;
}

function radios($title, $choices, $map = FALSE) {
  $element = array(
    '#type' => 'radios',
    '#title' => t($title),
    '#options' => ($map) ? drupal_map_assoc($choices) : $choices,
    '#required' => TRUE,
  );
  return $element;
}

function date_popup($title, $size = 13) {
  $element = array(
    '#type' => 'date_popup',
    '#date_format' => 'd-m-Y',
    '#prefix' => '<div class="date-popup">',
    '#title' => t($title),
    '#suffix' => '</div>',
    '#size' => $size,
    '#required' => TRUE,
  );
  return $element;
}

function select($title, $choices) {
  $element = array(
    '#type' => 'select',
    '#title' => t($title),
    '#options' => $options = array('none' => t('- Select -')) + drupal_map_assoc($choices),
    '#required' => TRUE,
    '#rules' => array('not_none'),
  );
  return $element;
}

function state($title = 'State') {
  $element = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t($title),
    '#options' => array('none' => t('- Select -')) + drupal_map_assoc(array('AK', 'AL', 'AR', 'AZ', 'CA', 'CO', 'CT', 'DC', 'DE', 'FL', 'GA', 'HI', 'IA', 'ID', 'IL', 'IN', 'KS', 'KY', 'LA', 'MA', 'MD', 'ME', 'MI', 'MN', 'MO', 'MS', 'MT', 'NC', 'ND', 'NE', 'NH', 'NJ', 'NM', 'NV', 'NY', 'OH', 'OK', 'OR', 'PA', 'RI', 'SC', 'SD', 'TN', 'TX', 'UT', 'VA', 'VT', 'WA', 'WI', 'WV', 'WY')),
    '#attributes' => array('class' => 'select-state'),
    '#rules' => array('not_none'),
  );
  return $element;
}

function radios_yesno($title) {
  $element = array(
    '#type' => 'radios',
    '#title' => t($title),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#required' => TRUE,
    '#attributes' => array('class' => 'radios-yes-no'),
  );
  return $element;
}

function radios_123($title) {
  $element = array(
    '#type' => 'radios',
    '#title' => t($title),
    '#options' => array(1, 2, 3),
    '#required' => TRUE,
    '#attributes' => array('class' => 'radios-123'),
  );
  return $element;
}

function terms($desc) {
  $element = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => array(
      1 => t('I accept'), 0 => t('I do not accept')
    ),
    '#prefix' => '<div class="oa-terms-field">'. $desc,
    '#suffix' => '</div>',
  );
  return $element;
}

function address() {
  $elements = array('#tree' => TRUE);
  $elements['address1'] = textfield('Address', 30);
  $elements['address1']['#prefix'] = '<div class="field-address">';
  $elements['address2'] = textfield('', 30);
  $elements['address2']['#required'] = FALSE;
  $elements['city'] = textfield('City');
  $elements['state'] = state();
  $elements['zip'] = textfield('Zip Code', 10);
  $elements['zip']['#suffix'] = '</div>';
  $elements['zip']['#rules'][] = 'numeric';
  $elements['zip']['#rules'][] = 'length[5]';
  return $elements;
}

function cap_yne($title, $yn_title = 'If no, explain') {
  $elements = array('#tree' => TRUE);
  $elements['rdo_yn'] = radios_yesno($title);
  $elements['rdo_yn']['#prefix'] = '<div class="field-yes-no-explain">';
  $elements['explain'] = textfield($yn_title);
  $elements['explain']['#suffix'] = '</div>';
  return $elements;
}

function acknowledgement($description) {
  $element = checkbox('I acknowledge, understand , and accept this Agreement/Statement.');
  $element['#prefix'] = '<div class="field-acknowledgement"><div class="acknowledgement-description">'. $description .'</div>';
  $element['#suffix'] = '</div>';
  return $element;
}
