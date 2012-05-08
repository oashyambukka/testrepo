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
  public $modality_compliance;
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

  private function load_profile() {
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

  private function load_compliance() {
    $p = oa_portal_forms_info();

    // Get Compliance object
    $query = 'xRM_ID <matches> `'. $this->rmaxid .'` <AND> dDocType <matches> `Rmax`  <AND>  dDocAccount <matches> `Recruit/QA`  <AND>  xContentType <matches> `Compliance`';
    $result = oa_portal_soap('AdvancedSearch', array('queryText' => $query, 'sortField' => 'dInDate', 'sortOrder' => 'Desc', 'resultCount' => '1'));
    $this->compliance_object = (is_array($result->AdvancedSearchResult->SearchResults)) ? $result->AdvancedSearchResult->SearchResults[0] : $result->AdvancedSearchResult->SearchResults;
    $this->complianceRecordID = $this->compliance_object->dDocName;
    $this->modality_name = $this->compliance_object->dDocTitle;

    // Load $modality_compliance document counts
    $params = array(
      'rmaxid' => $this->rmaxid,
      'complianceRecordID' => $this->complianceRecordID,
      'doNotUpdate' => 1,
    );
    $results = oa_portal_soap('drupalcounts', $params);
    if ($results->drupalcountsResult->StatusInfo->statusCode === -1) {
      oa_portal_503();
    }

    $dcr = $results->drupalcountsResult;
    $modality_compliance = array();
    foreach (array(1, 2, 3) AS $k) {
      $modality_compliance[$k]['approved_date'] = $dcr->{'phase'. $k .'ApprovedDate'};
    }
    $modality_compliance['open'] = array_merge(
      ($dcr->expired == 0) ? array() : explode(',', $dcr->expired),
      ($dcr->missing == 0) ? array() : explode(',', $dcr->missing),
      ($dcr->rejected == 0) ? array() : explode(',', $dcr->rejected)
    );
    $modality_compliance['under-review'] = array_merge(
      ($dcr->inprogress == 0) ? array() : explode(',', $dcr->inprogress),
      ($dcr->pendingreview == 0) ? array() : explode(',', $dcr->pendingreview)
    );
    $modality_compliance['closed'] = ($dcr->approved == 0) ? array() : explode(',', $dcr->approved);
    $modality_compliance['complianceApprovedDate'] = $dcr->complianceApprovedDate;
    $modality_compliance['drupalcountsResult'] = $dcr;


    $p['p2']['#status'] = 'locked';
    $p['p3']['#status'] = 'locked';
    if ($modality_compliance['p1']['approved_date']) {
      $p['p2']['#status'] = 'open';
    }
    if ($modality_compliance['p2']['approved_date']) {
      $p['p3']['#status'] = 'open';
    }

    // Produce counts for open, pr, and closed formids
    foreach ($p AS $pk => $phase) {
      foreach (element_children($phase) AS $sk) {
        $section = $phase[$sk];
        if (count($p[$pk][$sk]['#formtypes'])) {
          $p[$pk][$sk]['#formtypes_open'] = array_intersect($p[$pk][$sk]['#formtypes'], $modality_compliance['open']);
          $p[$pk][$sk]['#formtypes_pr'] = array_intersect($p[$pk][$sk]['#formtypes'], $modality_compliance['under-review']);
          $p[$pk][$sk]['#formtypes_closed'] = array_intersect($p[$pk][$sk]['#formtypes'], $modality_compliance['closed']);
        }
      }
    }
    $this->modality_compliance = $modality_compliance;
    $this->p = $p;
  }


  /**
   * Status for Portal section items
   */
  public function get_section_status($phase, $section) {
    $p = $this->p;
    $modality_compliance = $this->modality_compliance;

    // If section is marked as read-only, verify phase is open
    if ($p[$phase][$section]['#status'] == 'read-only') {
      if (($section == 'p2' && $modality_compliance[1]['approved_date']) || ($section == 'p3' && $modality_compliance[2]['approved_date'])) {
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
      if (count(array_intersect($p[$phase][$section]['#dependencies'], $modality_compliance['open']))) {
        dsm('Cant open '. $phase .' - '. $section .' due to missing dependencies');
        return 'closed';
      }
    }

    // Counting on #formtypes at this point. 
    if (is_array($p[$phase][$section]['#formtypes'])) {
      // Check for open forms first. Those are missing
      if (count(array_intersect($p[$phase][$section]['#formtypes'], $modality_compliance['open']))) {
        return 'open';
      }
      // Otherwise we probably have some files under review
      if (count(array_intersect($p[$phase][$section]['#formtypes'], $modality_compliance['under-review']))) {
        return 'under-review';
      }
      // Everything is closed. All clear.
      if (count(array_intersect($p[$phase][$section]['#formtypes'], $modality_compliance['closed']))) {
        return 'closed';
      }
    }  
    if (!is_array($p[$phase][$section]['#formtypes'])) {
      dsm('Cant give a status for '. $phase .' - '. $section .' due to missing formtypes');
    }
    return 'unknown';
  }

  public function get_unread_message_count() {
    return db_result(db_query("SELECT count(*) FROM {ucm_messages} WHERE rmaxid = %d", $this->rmaxid));
  }

  public function get_all_docs() {
    $query = 'xRM_ID <matches> `'. $this->rmaxid .'` <AND> <not>xStatus <matches> `Missing`';
    $result = oa_portal_soap('AdvancedSearch', array('queryText' => $query, 'sortField' => 'dDocTitle', 'sortOrder' => 'Asc', 'resultCount' => '200'));
    return (is_array($result->AdvancedSearchResult->SearchResults)) ? $result->AdvancedSearchResult->SearchResults : array($result->AdvancedSearchResult->SearchResults);
  }

}
