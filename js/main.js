Drupal.behaviors.tooltips = function (context) {
  // @TODO Verify operaional
  $(".oap-tooltip-trigger", context).click(
    function() {
      $(".oap-tooltip").css("display","block");
    },
    function(){
      $(".oap-tooltip").css("display","none");
  });

  $(".oap-tooltip .close", context).click(function() {
    $(".oap-tooltip").css("display","none");
  });

});

Drupal.behaviors.addAnotherLink = function (context) {
  $(".add-another-link").click(function() {
    $(this).hide();
    $(this).parent().next().slideDown();
  });
});

// @TODO Verify still required
Drupal.behaviors.paycheckOptions = function (context) {
  $('#edit-delivery-rdo-delivery-option-directdeposit-wrapper input, #edit-delivery-rdo-delivery-option-no-wrapper input, #edit-delivery-rdo-delivery-option-cashpay-wrapper input').change(function () { fieldset_change(this); });

  function fieldset_change(element) {
    var value = $(element).val();
    if (value == 'cashpay') {
      fieldset_hide('directdeposit');
      fieldset_show('cashpay');
    }
    if (value == 'directdeposit') {
      fieldset_hide('cashpay');
      fieldset_show('directdeposit');
    }
    if (value == 'no') {
      fieldset_hide('cashpay');
      fieldset_hide('directdeposit');
    }
  }
}
 
Drupal.behaviors.finalization = function (context) { 
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

  function fieldsets_hide_all() {
    $('fieldset').each(function() {
      if (!$(this).hasClass('collapsed')) {
        $('legend a', this).click();
      }
    });
  }

  $('#edit-finalize').click(function() {
    prepare_finalize();
    return false;
  });
  $('#finalize-cancel').click(function() {
    undo_finalize();
    return false;
  });

  function prepare_finalize() {
    $('#edit-finalize').hide();
    $('#finalize-confirm-box').show();
    lock_fields();
  }
  function undo_finalize() {
    $('#form-throbber').hide();
    $('#finalize-confirm-box').hide();
    $('#edit-finalize').show();
    unlock_fields();
  } 

  function lock_fields() {
    $('.oaportal input[type=text], .oaportal input[type=checkbox]').each(function() {
      $(this).attr("disabled", "disabled");
    });
  }
  function unlock_fields() {
    $('.oaportal input').each(function() {
      $(this).removeAttr("disabled");
    });
  }

  // Unbind click events to fieldset anchors
  $('fieldset.locked > legend a', context).each(function(link) {
    $(link).unbind('click');
  });
  
  Drupal.Ajax.plugins.oaportal = function(hook, args) {
    var errors_exist = false;
    
    if (hook === 'submit') {
      $('#edit-finalize').hide();
      fieldsets_hide_all();
      unlock_fields();
      $('#finalize-confirm-box').hide();
      $('#form-throbber').slideDown();
    }
    if (hook === 'message') {
      if (args.messages_error) {
        $.each(args.messages_error, function (i, msg) {
          errors_exist = true;
          var id = msg.id.replace('[', '-').replace(']', '');
          var legend = $('#' + id).css('border-color', '#00ff00').parentsUntil('legend', 'fieldset');
          $('a', legend).click();
        });
      }
    }

    if (hook == 'afterMessage') {
      if (errors_exist) {
        undo_finalize();
      }
    }
    if (hook === 'redirect') {
      errors_exist = false;
      $('#form-throbber').hide();
      $('#edit-finalize').hide();
      fieldsets_hide_all();
      lock_fields();
      $('.oap-doc-download').show();
      $('#edit-continue').show().click(function() {
        Drupal.Ajax.redirect(args.redirect);
        return false;
      });

      return false;
    }
  }
}