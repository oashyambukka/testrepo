<?php

function markup($value) {
  $element = array(
    '#type' => 'markup',
    '#value' => t($value),
  );
  return $element;
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

function textfield($title, $length = null) {
  $element = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t($title),
    '#length' => $length,
  );
  return $element;
}

function email($title = null) {
  $element = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => ($title) ? t($title) : t('Email'),
    '#length' => 100,
    // @TODO Add validation
  );
  return $element;
}

function textfield_sin() {
  $element = array(
    '#type' => 'fieldset',
    '#title' => t('Social Security Number'),
    '#attributes' => array('class' => 'sin-field'),
    '#tree' => TRUE,
  );
  $el = array(
    '#type' => 'textfield',
    '#size' => 3,
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

function radios($title, $choices) {
  $element = array(
    '#type' => 'radios',
    '#title' => t($title),
    '#options' => $choices, //drupal_map_assoc($choices),
    '#required' => TRUE,
  );
  return $element;
}

function date_popup($title) {
  $element = array(
    '#type' => 'date_popup',
    '#title' => t($title),
    '#required' => TRUE,
  );
  return $element;
}

function select($title, $choices) {
  $element = array(
    '#type' => 'select',
    '#title' => t($title),
    '#options' => drupal_map_assoc($choices),
    '#required' => TRUE,
  );
  return $element;
}

function state($title = null) {
  $element = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => ($title) ? t($title) : t('State'),
    '#options' => drupal_map_assoc(array('AB', 'AL')),
  );
  return $element;
}

function checkboxes_yesno($title) {
  $element = array(
    '#type' => 'checkboxes',
    '#title' => t($title),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#required' => TRUE,
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
  $elements = array();
  $elements['address1'] = textfield('Address');
  $elements['address2'] = textfield('');
  $elements['address2']['#required'] = FALSE;
  $elements['city'] = textfield('City');
  $elements['state'] = textfield('State');
  $elements['zip'] = textfield('Zip Code');
  return $elements;
}
