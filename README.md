OnAssignment Portal - Architecture 
============

Important terms
---------------

1. Drupal: Open source content management system (CMS) used as a framework for certain On Assignment web sites, including the OA Portal
2. OA Portal module OR oa_portal module: custom Drupal module
3. Portal: referrs to Drupal site instance running the OA Portal Drupal module
4. RMax/RecruitMax: On Assignment recruitment management application
5. UCM: Oracle document management system (DMS) implemented by Sena Systems for On Assignment
6. PDF Form: One unique PDF document corresponding to a unique UCM Form ID
7. Portal form: Drupal form corresponding to one unique URI. Typically used to capture input to populate multiple PDF Forms.
8. Drupal contributed module: Drupal module sourced from drupal.org. Published for use under GPL 2 open source license.
9. Field: Drupal Form API field, typically corresponding to PDF Form field(s) and/or RecruitMax fields
10. Portal Main Page: Portal overview page accessible to Candidates only

Code and other digital assets
-----------------------------

1. Web root (depends on server environment) <web root>: /var/www/html/portal.oahealthcare.com
2. Drupal core: <web root>/
3. "OA Portal" Drupal module: <web root>/sites/all/modules/oahealthportal
4. UI CSS for OA Portal module: <web root>/sites/all/modules/oahealthportal/css
5. UI JS for OA Portal module: <web root>/sites/all/modules/oahealthportal/js
6. Drupal templates for OA Portal module: <web root>/sites/all/modules/oahealthportal/templates
7. Soap WSDLs in use by OA Portal module: <web root>/sites/all/modules/oahealthportal/wsdl

Installation
------------

### Pre-existing Drupal site

1. Take precautionary backups of both the Drupal codebase and MySQL database
2. Enable the Profile module, and create a hidden field called "rmaxid".
3. Unpack the OA module in the correct custom module location (<web root>/sites/all/modules/)
4. Verify and enable Drupal contributed module dependencies are met by visiting /admin/build/modules. Download missing contributed modules where appropriate. 
5. Enable OA module
6. Enable Candidate Menu block in Left sidebar region. Designate visibility settings to "Show on only the listed pages." and specify those pages as "portal portal/*" (note: two items on separate lines)

### New Drupal site

1. Download and extract Drupal 6. Create MySQL database and user. Install Drupal normally. 
2. Unpack the OA module in the correct custom module location (<web root>/sites/all/modules/)
3. Verify and enable Drupal contributed module dependencies are met by visiting /admin/build/modules. Download and extract missing contributed modules where appropriate. 
4. Enable OA module
5. Enable Candidate Menu block in Left sidebar region. Designate visibility settings to "Show on only the listed pages." and specify those pages as "portal portal/*" (note: two items on separate lines)
6. Using Backup & Migrate module, restore from a recent backup.


Data storage/access
-------------------

1. User account (one per candidate): standard Drupal account. $account = user_load($uid);
2. RMax ID stored as $account->rmaxid profile field. Not editable by user.
3. Candidate objects: $candidate = get_candidate($rmaxid, $uid); currently loads everything. Statically cached per page request.
4. Phase Status keys:
  - open
  - locked
5. Section Status keys:
  - open
  - under-review
  - closed
  - locked
6. UCM messages: ingested from UCM, stored in {ucm_messages}
  - msg_id (autoincrement), rmaxid, subject, body, timestamp, was_read (default=0)
  - Unread message count per Candidate: $candidate->get_unread_message_count();
  - Load messages oa_portal_get_messages($rmaxid, $read = 0, $limit = 100)
  - Load one message: oa_portal_get_message($rmaxid, $msg_id)
  - Mark message as read: oa_portal_mark_message_read($msg_id, $js = FALSE) (Note: uses currently logged in user's RMaxID. Doubles as a page callback.)
  - Delete message: oa_portal_message_delete($rmaxid, $msg_id)
7. Autosaved forms: stores serialized form state values per RMaxID & Form ID in {autosaved_forms}. 
  - Get autosaved form: oa_portal_get_autosaved_form($form_id, $path, $uid)
  - See: <web root>/sites/all/modules/oahealthportal/js/autosave.js


Major Portal workflows
----------------------

1. Initial Candidate application process
  1. Portal: form captures candidate particulars
  2. Portal: form dupe-checks email address with RecruitMax ("check" method)
  3. Portal: form data and attachments POSTed to RecruitMax via Soap web service ("save" method)
  4. RecruitMax: candidate record created

2. Portal Account creation by RecruitMax
  1. RecruitMax: POST request to Portal: email, rmaxid
  2. Portal account created with rmaxid. $account->must_update_pass set to 1.
  3. Portal triggers welcome email by POST request to UCM. One-time login link included.

3. Candidate: first visit
  1. Portal: Candidate logs in with one-time login link
  2. Portal: Candidate prompted to set new password. If user attempts to visit any page other than /update-password, they will be redirected to /update-password.
  3. Portal: Upon successful submission, $account->must_update_pass set to 0 and Candidate redirected to Portal main page

4. Candidate: forgotten password
  1. Portal: Candidate visits and populates forgot password form
  2. Portal: $account->must_update_pass set to 1.
  3. Portal: Email request POSTed to UCM via Soap web service with one-time login link
  4. Portal: Candidate proceeds to step through #3 workflow.

5. Portal: Candidate Form finalization on Portal
  1. Candidate clicks on open phase section, Drupal form shown
  2. Candidate populates fields with information, form is checked for valid input as focus moves to further fields
  3. Form autosaves on input focus change. Serialized values retrieved to prepopulate form should candidate leave the form before finalization is complete. 
  4. User finalizes form, submitted via AJAX. Portal takes captured data, merges with RecruitMax Profile and/or other supplementary data where necessary. Data POSTed to UCM via Soap web service.
  5. AJAX response contains URL of further portal stage. jQuery used to bind this URL redirect action to "Continue" button.
  6. Document Download box and "Continue" button are exposed

6. UCM Nag email ingestion
  1. UCM: email POSTed to Portal via RESTful web service (see below)
  2. Portal: message stored in {ucm_messages}:  msg_id, rmaxid, subject, body, timestamp, was_read

9. UCM: Candidate status or document state change
  The Portal is generated based on live data. RMax profiles, compliance records and modality states are loaded for every page load.


Maintenance and future considerations
-------------------------------------

1. Modifying existing forms

  Scope: Field labels, field keys, field descriptions, field validation rules

  Changes may be reflected directly in OA Module code (likely in oa_portal.form.inc). Field key changes must match PDF Forms.

2. Adding new forms

  1. Add form particulars to function oa_portal_forms_info() in oa_portal.module
  2. Add form callback to oa_portal.forms.inc following the pattern of oa_portal_<phase id>_<section id>_form($form_state)
  3. Add form submission callback
  4. Clear Drupal's cache to expose form menu item


Drupal Web Service Documentation
--------------------------------

RESTful web services are exposed to allow the creation of Accounts and Messages.

Session-based authentication is used. You must first connect, then login. The following user/password combinations are used for the TEST environment:

- UCM - Un!5i3dc0N#n7
- RecruitMax - $ecRu!m@XX

### 1. Connect

Params: none

Returns: array:
- 'sessid'
- 'user': anonymous $user object

POST http://dev.portal.oahealthcare.com/api/system/connect

### Login

Header:
- 'Cookie' = $data->session_name . '=' . $data->sessid; from Connect step above

Params:
- $data: array with username, password of RecruitMax user

Returns array:
- 'sessid'
- 'session_name'
- 'user': $user object

POST http://dev.portal.oahealthcare.com/api/user/login + body data


### Creating Accounts

Header:
- 'Cookie' = $data->session_name . '=' . $data->sessid; from Connect/Login steps above

Params:
- $data: array with name, mail, rmaxid

Returns array:
- 'rmaxid'
- 'account' object
- 'uri': resource URI

POST http://dev.portal.oahealthcare.com/api/account + body data


### Creating Messages

Header:
- 'Cookie' = $data->session_name . '=' . $data->sessid; from Connect/Login steps above

Params:
- $data: array with rmaxid, subject, body

Returns array:
- 'msg_id'
- 'message' object
- 'uri': resource URI

POST http://dev.portal.oahealthcare.com/api/message + body data