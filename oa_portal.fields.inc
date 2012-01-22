<?php

function textfield($title, $opt1, $opt2) {
  $element = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t($title),
  );
  if (int($opt1)) {
    $element['#length'] = $opt1;
    $opt = $opt2;
  }
  else {
    $opt = $opt1;
  }
  foreach ($opt AS $k => $v) {
    $element[$k] = $v;
  }
  return $element;
}

function terms() {
  $element = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => array(
      1 => t('I accept'), 0 => t('I do not accept')
    ),
    '#prefix' => 'I certify that the information........',
  );
  foreach ($ops AS $k => $v) {
    $element[$k] = $v;
  }
  return $element;
  
}
