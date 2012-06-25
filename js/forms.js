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