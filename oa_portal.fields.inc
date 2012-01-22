<?php

function fieldset($title, $description = NULL) {
  $element = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t($title),
    '#description' => $description,
  );
  return $element;
}

function textfield($title, $opt1 = NULL, $opt2 = NULL) {
  $element = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t($title),
  );
  if (is_int($opt1)) {
    $element['#length'] = $opt1;
    $opt = ($opt2) ? $opt2 : array();
  }
  else {
    $opt = ($opt1) ? $opt1 : array();
  }
  foreach ($opt AS $k => $v) {
    $element[$k] = $v;
  }
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

function checkboxes($title, $choices) {
  $element = array(
    '#type' => 'checkboxes',
    '#title' => t($title),
    '#options' => drupal_map_assoc($choices),
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
    '#description' => $desc,
  );
  return $element;
}
