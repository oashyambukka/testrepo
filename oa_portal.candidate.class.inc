<?php

/**
 * Factory caching function for Candidate objects
 */
function get_candidate($rmaxid, $reset = FALSE) {
  static $candidates = array();
  if (is_object($candidates[$rmaxid]) && !$reset) {
    return $candidates[$rmaxid];
  }
  $candidates[$rmaxid] = new Candidate($rmaxid);
  return $candidates[$rmaxid];
}

class Candidate {
  public $compliance;
  public $compliance_object;
  public $complianceRecordID;
  public $modality_name;
  public $p;
  public $profile;
  public $rmaxid;

  /**
   * Constructor
   * Get RMax profile by RMaxID
   */
  function __construct($rmaxid) {
    $this->rmaxid = $rmaxid;
    $this->load_profile();
    $this->load_compliance();
  }

  function load_profile() {
    // Get RMax Profile from UCM
    $result = oa_portal_soap('profileview', array('schViewName' => 'vOA_Profile', 'whereClause' => "rmaxid='". $this->rmaxid ."'"));
    $profile = $result->profileviewResult->profiledata;
    $profile->name = $profile->fname .' '. $profile->lname;
    $address = $profile->addressline1;
    $address .= ($profile->addressline2) ? ' '. $profile->addressline2 : NULL;
    $address .= ($profile->addressline3) ? ' '. $profile->addressline3 : NULL;
    $address .= ' '. $profile->city .', '. $profile->state .' '. $profile->postal;
    $profile->address = $address;
    $this->profile = $profile;
    return $profile;
  }

  function load_compliance() {
    // @TODO Use actual compliance ID
    $this->p = $p = oa_portal_forms_info();

    $query = 'xRM_ID <matches> `'. $this->rmaxid .'` <AND> dDocType <matches> `Rmax`  <AND>  dDocAccount <matches> `Recruit/QA`  <AND>  xContentType <matches> `Compliance`';
    $result = oa_portal_soap('AdvancedSearch', array('queryText' => $query, 'sortField' => 'dInDate', 'sortOrder' => 'Desc', 'resultCount' => '1'));
    $this->compliance_object = (is_array($result->AdvancedSearchResult->SearchResults)) ? $result->AdvancedSearchResult->SearchResults[0] : $result->AdvancedSearchResult->SearchResults;
    $this->complianceRecordID = $this->compliance_object->dDocName;
    $this->modality_name = $this->compliance_object->dDocTitle;
    $this->compliance = $compliance = oa_portal_get_modality_formtypes($this->rmaxid, $this->complianceRecordID);

    // @TODO Use AdvancedSearch or otherwise to filter by file category for checklists, licensures & certs, etc.
    $all_checklists = array_intersect(range(480, 532) + array(547), array_merge($compliance['open'], $compliance['under-review'], $compliance['closed']));
    $open_checklists = array_intersect(range(480, 532) + array(547), $compliance['open']);
    $p['p1']['checklist']['#formtypes'] = $all_checklists;
    $p['p1']['checklist']['#formtypes_missing'] = $open_checklists;
  
    // @TODO Use AdvancedSearch or otherwise to filter by file category for checklists, licensures & certs, etc.
    $missing_licensures = array(); //array_intersect(range(480, 532) + array(547), $compliance[2]['missing']);
    $p['p2']['licensure_certs']['#formtypes'] = $missing_licensures;


    $p['p2']['#status'] = 'locked';
    $p['p3']['#status'] = 'locked';
    if ($compliance['p1']['approved_date']) {
      $p['p2']['#status'] = 'open';
    }
    if ($compliance['p2']['approved_date']) {
      $p['p3']['#status'] = 'open';
    }
    $this->p = $p;
    return $compliance;
  }


  /**
   * Status for Portal section items
   */
  function get_section_status($phase, $section) {
    $p = $this->p;
    $compliance = $this->compliance;

    // If section is marked as read-only, verify phase is open
    if ($p[$phase][$section]['#status'] == 'read-only') {
      if (($section == 'p2' && $compliance[1]['approved_date']) || ($section == 'p3' && $compliance[2]['approved_date'])) {
        return 'open';
      }
      return 'closed';
    }

    // Global access checking - If phase or section is closed
    if ($p[$phase]['#status'] == 'closed' || $p[$phase][$section]['#status'] == 'closed') {
      return 'closed';
    }

    // Check if dependencies are not met
    if (is_array($p[$phase][$section]['#dependencies'])) {
      if (count(array_intersect($p[$phase][$section]['#dependencies'], $compliance['open']))) {
        dsm('Cant open '. $phase .' - '. $section .' due to missing dependencies');
        return 'closed';
      }
    }

    // Counting on #formtypes at this point. 
    if (is_array($p[$phase][$section]['#formtypes'])) {
      // Check for open forms first. Those are missing
      if (count(array_intersect($p[$phase][$section]['#formtypes'], $compliance['open']))) {
        return 'open';
      }
      // Otherwise we probably have some files under review
      if (count(array_intersect($p[$phase][$section]['#formtypes'], $compliance['under-review']))) {
        return 'under-review';
      }
      // Everything is closed. All clear.
      if (count(array_intersect($p[$phase][$section]['#formtypes'], $compliance['closed']))) {
        return 'closed';
      }
    }  
    if (!is_array($p[$phase][$section]['#formtypes'])) {
      dsm('Cant give a status for '. $phase .' - '. $section .' due to missing formtypes');
    }
    else {
      dsm('Something unexpected happened.');
    }
    return NULL;
  }

}
