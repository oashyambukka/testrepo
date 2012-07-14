<?php

/**
 * Callback for creating account resources.
 *
 * @param array $data
 * @param string $notify (yes, no, etc)
 * @return object
 */
function _accountresource_create($data) {
  // Check for duplicates
  if ($account = user_load(array('mail' => $data['mail']))) {
    return services_error('Invalid attribute "mail". Account already exists.', 406);
  }
  if ($account = oa_services_account_load($data['rmaxid'])) {
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
    'rmaxid',
  );
  foreach ($data AS $k => $v) {
    if (!in_array($k, $allowed_keys)) {
      return services_error('Invalid field "'. $k .'" to insert. Account was not created.', 406);
    }
  }

  $data['status'] = 1;
  $data['notify'] = $notify;
  $data['roles'] = array(2 => 2, 4 => 4);
  $data['notified'] = $notify == 'yes';

  if ($account = user_save(array(), $data, 'OnAssignment')) {
    // Not sure why I need this query -rp
    // @TODO Verify still required
    db_query("INSERT INTO {profile_values} (fid, uid, value) VALUES (%d, %d, '%s')", 1, $account->uid, $data['rmaxid']);

    // Send notification email
    oa_portal_user_mail_notify('register_admin_created', $account);

    return (object)array(
      'rmaxid' => $account->rmaxid,
      'account' => $account,
      'uri' => services_resource_uri(array('account', $account->rmaxid)),
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
    'account' => oa_services_account_load($rmaxid),
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
  $account = oa_services_account_load($rmaxid);
  if (!$account->uid) {
    return services_error('No account found to update.', 406);
  }

  $allowed_keys = array(
    'mail',
    'name',
    'status',
  );
  foreach ($data AS $k => $v) {
    if (!in_array($k, $allowed_keys)) {
      return services_error('Invalid field "'. $k .'" to update. Account was not updated.', 406);
    }
  }

  if ($account = user_save($account, $data)) {
    return (object) array(
      'rmaxid' => $rmaxid,
      'account' => $account,
      'uri' => services_resource_uri(array('account', $rmaxid)),
    );
  }
  else {
    return services_error('Something went wrong. Account was not updated.', 406);
  }
}

/**
 * Callback for creating message resources.
 *
 * @param array $data
 * @return object
 */
function _messageresource_create($data) {
  // Check for missing required attributes
  $allowed_keys = array(
    'rmaxid',
    'subject',
    'body',
  );
  foreach ($allowed_keys AS $k) {
    if (!isset($data[$k])) {
      return services_error('Missing message attribute "'. $k .'". Message was not created.', 406);
    }
  }


  foreach ($data AS $k => $v) {
    if (!in_array($k, $allowed_keys)) {
      return services_error('Invalid field "'. $k .'" to insert. Message was not created.', 406);
    }
  }

  $data['timestamp'] = time();
  $data['was_read'] = 0;

  if ($message = oa_services_message_create($data)) {
    return (object)array(
      'msg_id' => $message->msg_id,
      'message' => $message,
    );
  }
  else {
    return services_error('Something went wrong. No message created.', 406);
  }
}