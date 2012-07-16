<div id="oa-phase-<?php print $id; ?>" class="oap-phase oap-phase-status-<?php print $status; ?>">
  <div class="oap-phase-title-status">
    <h3><?php print $title . $tip; ?></h3>
    <div class="oap-status-icon oap-status-<?php print $status; ?>"><?php print $status_tip; ?></div>
  </div>
  <div class="oa-phase-phases">
    <?php print $phases; ?>
  </div>
  <div class="oa-phase-complete">(<?php print $completed; ?> complete)</div>
</div>