Drupal.behaviors.p2_licensure_certs = function (context) {
  $('form#oa-portal-p2-licensure-certs-form .oap-doc-download.set-hide:last').show();

  $('#edit-lc-chk-fax-preference').change(function() {
    if ($('#edit-lc-chk-fax-preference').attr('checked')) {
      $('#edit-lc-upload-wrapper').hide();
      $('#cover-letter-box').slideDown();
    }
    else {
      $('#edit-lc-upload-wrapper').show();
      $('#cover-letter-box').slideUp();
    }
    
  });

  $('#edit-lc-lc-licensure').change(function() {
    if ($(this).val() != 'none') {
      $('#cover-letter-link').attr('href', '/portal/createcoversheet/' + $(this).val());
    }
  });
}

Drupal.behaviors.p2_capabilities = function (context) {
  // @TODO
}
Drupal.behaviors.p2_medical = function (context) {
  // If fax pref box checked, hide file upload.
  $('input[name="tb[tb_option]"]').change(function() {
    if (!$('#edit-tb-tb-fax').attr('checked')) {
      $('#edit-tb-tb-file-wrapper').parent().show();
    }
    $('#edit-tb-tb-fax-wrapper').parent().show();
  });
  $('#edit-tb-tb-fax').change(function() {
    if ($(this).attr('checked')) {
      $('#edit-tb-tb-file-wrapper').parent().hide();
    }
    else {
      $('#edit-tb-tb-file-wrapper').parent().show();
    }
  });

  $('input[name="mmr[mmr_option]"]').change(function() {
    $('#edit-mmr-mmr-file-284-wrapper').parent().hide();
    $('#edit-mmr-mmr-file-285-wrapper').parent().hide();
    $('#edit-mmr-mmr-file-261-wrapper').parent().hide();
    $('#edit-mmr-mmr-file-262-wrapper').parent().hide();
    $('#edit-mmr-mmr-fax-wrapper').parent().show();
    if (!$('#edit-mmr-mmr-fax').attr('checked')) {
      if ($(this).val() == 'mmr') {
        $('#edit-mmr-mmr-file-261-wrapper').parent().show();
        $('#edit-mmr-mmr-file-262-wrapper').parent().show();
      }
      else if ($(this).val() == 'rubella_titer') {
        $('#edit-mmr-mmr-file-284-wrapper').parent().show();
        $('#edit-mmr-mmr-file-285-wrapper').parent().show();
      }
    }
  });

  $('#edit-mmr-mmr-fax').change(function() {
    if ($(this).attr('checked')) {
      $('#edit-mmr-mmr-file-284-wrapper').parent().hide();
      $('#edit-mmr-mmr-file-285-wrapper').parent().hide();
      $('#edit-mmr-mmr-file-261-wrapper').parent().hide();
      $('#edit-mmr-mmr-file-262-wrapper').parent().hide();
    }
    else {
      if (!$('#edit-mmr-mmr-fax').attr('checked')) {
        if ($('input[name="mmr[mmr_option]"]:checked').val() == 'mmr') {
          $('#edit-mmr-mmr-file-261-wrapper').parent().show();
          $('#edit-mmr-mmr-file-262-wrapper').parent().show();
        }
        else if ($('input[name="mmr[mmr_option]"]:checked').val() == 'rubella_titer') {
          $('#edit-mmr-mmr-file-284-wrapper').parent().show();
          $('#edit-mmr-mmr-file-285-wrapper').parent().show();
        }
      }
    }
  });


  $('input[name="cls[cls_option]"]').change(function() {
    if (!$('#edit-cls-cls-fax').attr('checked')) {
      $('#edit-cls-cls-file-wrapper').parent().show();
    }
    $('#edit-cls-cls-fax-wrapper').parent().show();
  });
  $('#edit-cls-cls-fax').change(function() {
    if ($(this).attr('checked')) {
      $('#edit-cls-cls-file-wrapper').parent().hide();
    }
    else {
      $('#edit-cls-cls-file-wrapper').parent().show();
    }
  });


  $('input[name="varivax[varivax_option]"]').change(function() {
    if (!$('#edit-varivax-varivax-fax').attr('checked')) {
      $('#edit-varivax-varivax-file-wrapper').parent().show();
    }
    $('#edit-varivax-varivax-fax-wrapper').parent().show();
  });
  $('#edit-varivax-varivax-fax').change(function() {
    if ($(this).attr('checked')) {
      $('#edit-varivax-varivax-file-wrapper').parent().hide();
    }
    else {
      $('#edit-varivax-varivax-file-wrapper').parent().show();
    }
  });


  $('input[name="hepb[hepb_option]"]').change(function() {
    $('#edit-hepb-hepb-file-1-wrapper').parent().hide();
    $('#edit-hepb-hepb-file-2-wrapper').parent().hide();
    $('#edit-hepb-hepb-file-3-wrapper').parent().hide();
    $('#edit-hepb-hepb-file-274-wrapper').parent().hide();
    $('#edit-hepb-declination-wrapper').parent().parent().hide();
    $('#edit-hepb-hepb-fax-wrapper').parent().show();

    if (!$('#edit-hepb-hepb-fax').attr('checked')) {
      if ($(this).val() == 'hep123') {
        console.log('Showing 123');
        $('#edit-hepb-hepb-file-1-wrapper').parent().show();
        $('#edit-hepb-hepb-file-2-wrapper').parent().show();
        $('#edit-hepb-hepb-file-3-wrapper').parent().show();
      }
      else if ($(this).val() == '274') {
        console.log('Showing 274');
        $('#edit-hepb-hepb-file-274-wrapper').parent().show();
      }
      else if ($(this).val() == '275') {
        console.log('Showing 275');
        $('#edit-hepb-declination-wrapper').parent().parent().show();
        $('#edit-hepb-hepb-fax-wrapper').parent().hide();
      }
    }
  });

  $('#edit-hepb-hepb-fax').change(function() {
    if (!$(this).attr('checked')) {
      if ($('input[name="hepb[hepb_option]"]:checked').val() == 'hep123') {
        $('#edit-hepb-hepb-file-1-wrapper').parent().show();
        $('#edit-hepb-hepb-file-2-wrapper').parent().show();
        $('#edit-hepb-hepb-file-3-wrapper').parent().show();
      }
      else if ($('input[name="hepb[hepb_option]"]:checked').val() == 274) {
        $('#edit-hepb-hepb-file-274-wrapper').parent().show();
      }
      // else if ($('input[name="hepb[hepb_option]"]:checked').val() == 275) {
      //   $('#edit-hepb-declination-wrapper').parent().parent().show();
      // }
    }
    else {
      $('#edit-hepb-hepb-file-1-wrapper').parent().hide();
      $('#edit-hepb-hepb-file-2-wrapper').parent().hide();
      $('#edit-hepb-hepb-file-3-wrapper').parent().hide();
      $('#edit-hepb-hepb-file-274-wrapper').parent().hide();
    }
  });


}

Drupal.behaviors.p2_competency = function (context) {
  // @TODO Store click action of external link
}

// @TODO Verify still required
Drupal.behaviors.p3_paycheck = function (context) {
  $('#edit-paycheck-delivery-delivery-option-directdeposit-wrapper input, #edit-paycheck-delivery-delivery-option-no-wrapper input, #edit-paycheck-delivery-delivery-option-cashpay-wrapper input').change(function () {
    fieldset_change(this);
  });

  function fieldset_hide(id) {
    if (!$('#fieldset-' + id).hasClass('collapsed')) {
      $('#fieldset-' + id + ' legend a').click();
    }
  }
  function fieldset_show(id) {
    if ($('#fieldset-' + id).hasClass('collapsed')) {
      $('#fieldset-' + id + ' legend a').click();
    }
  }

  function fieldset_change(element) {
    var value = $(element).val();
    // if (value == 'cashpay') {
    //   fieldset_hide('directdeposit');
    //   fieldset_show('cashpay');
    // }
    if (value == 'directdeposit') {
      // fieldset_hide('cashpay');
      fieldset_show('directdeposit');
    }
    if (value == 'no') {
      // fieldset_hide('cashpay');
      fieldset_hide('directdeposit');
    }
  }
}


Drupal.behaviors.p3_w4 = function (context) {
}
Drupal.behaviors.p3_i9 = function (context) {
  // @TODO Show Expiry field only if Alien
  $('#edit-461-citizentype-citizen-wrapper input, #edit-461-citizentype-national-wrapper input, #edit-461-citizentype-resident-wrapper input, #edit-461-citizentype-alien-wrapper input').change(function () {
    if ($(this).val() == 'alien') {
      $('#edit-461-expiry-wrapper').parent().parent().show();
    }
    else {
      $('#edit-461-expiry-wrapper').parent().parent().hide();
    }
  });
  
  // @TODO Show file upload warning (TBD)
  $('#i9-verification-docs').click(function() {
    $('#i9-verification-box').show();
  });
  $('#i9-verification-close').click(function() {
    $('#i9-verification-box').hide();
  });
}
Drupal.behaviors.p3_eeo = function (context) {
}
Drupal.behaviors.p3_work_policy = function (context) {
}