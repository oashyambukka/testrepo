<?php

/**
 * Callback for creating account resources.
 *
 * @param array $data
 * @param string $notify (yes, no, etc)
 * @return object
 */
function _accountresource_create($data, $notify) {
  // Check for duplicates
  if ($account = user_load(array('mail' => $data['mail']))) {
    return services_error('Invalid attribute "mail". Account already exists.', 406);
  }
  if ($account = oa_rmax_get_account($data['rmaxid'])) {
    return services_error('Invalid attribute "rmaxid". Account already exists.', 406);
  }
  // Check for missing required attributes
  foreach (array('mail', 'name', 'rmaxid') AS $k) {
    if (!isset($data[$k])) {
      return services_error('Missing account attribute "data['. $k .']". Account was not created.', 406);
    }
  }

  $allowed_keys = array(
    'mail',
    'name',
    'profile_first_name',
    'profile_last_name',
    'profile_middle_initial',
    'profile_street_address1',
    'profile_street_address2',
    'profile_city',
    'profile_state',
    'profile_code',
    'profile_recruiter',
    'profile_office',
    'rmaxid',
  );
  foreach ($data AS $k => $v) {
    if (!in_array($k, $allowed_keys)) {
      return services_error('Invalid field "'. $k .'" to insert. Account was not created.', 406);
    }
  }

  $data['profile_rmaxid'] = $data['rmaxid'];
  unset($data['rmaxid']);
  $data['status'] = 1;
  $data['notify'] = $notify;
  $data['roles'] = array(2, 8);
  $data['notified'] = ($notify == 'yes');

  if ($account = user_save(array(), $data, 'OA Information')) {
    // Not sure why I need this query -rp
    db_query("INSERT INTO {profile_values} (fid, uid, value) VALUES (%d, %d, '%s')", 13, $account->uid, $account->profile_rmaxid);

    // Send notification email
    if ($notify == 'yes') {
      _user_mail_notify('register_admin_created', $account);
    }
    
    return (object)array(
      'rmaxid' => $account->profile_rmaxid,
      'account' => $account,
      'uri' => services_resource_uri(array('account', $account->profile_rmaxid)),
    );
  }
  else {
    return services_error('Something went wrong. No account created.', 406);
  }
}

/**
 * Callback for retrieving account resources.
 *
 * @param int $rmaxid
 * @return object
 */
function _accountresource_retrieve($rmaxid) {
  return (object)array(
    'account' => oa_rmax_get_account($rmaxid),
  );
}

/**
 * Callback for updating account resources.
 *
 * @param int $rmaxid
 * @param array $data
 * @return object
 */
function _accountresource_update($rmaxid, $data) {
  $account = oa_rmax_get_account($rmaxid);
  if (!$account->uid) {
    return services_error('No account found to update.', 406);
  }

  $allowed_keys = array(
    'mail',
    'name',
    'profile_first_name',
    'profile_last_name',
    'profile_middle_initial',
    'profile_street_address1',
    'profile_street_address2',
    'profile_city',
    'profile_state',
    'profile_code',
    'profile_recruiter',
    'profile_office',
  );
  foreach ($data AS $k => $v) {
    if (!in_array($k, $allowed_keys)) {
      return services_error('Invalid field "'. $k .'" to update. Account was not updated.', 406);
    }
  }

  if ($account = user_save($account, $data)) {
    return (object)array(
      'rmaxid' => $rmaxid,
      'account' => $account,
      'uri' => services_resource_uri(array('account', $rmaxid)),
    );
  }
  else {
    return services_error('Something went wrong. Account was not updated.', 406);
  }
}
