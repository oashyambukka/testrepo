$(document).ready(function() {

  $(".oap-tooltip-trigger").click(
    function() {
      $(".oap-tooltip").css("display","block");
    },
    function(){
      $(".oap-tooltip").css("display","none");
  });

  $(".oap-tooltip .close").click(function() {
    $(".oap-tooltip").css("display","none");
  });

  $(".add-another-link").click(function() {
    $(this).hide();
    $(this).parent().next().slideDown();
  });


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
    $('#finalize-confirm-box').slideDown();
    lock_fields();
  }
  function undo_finalize() {
    $('#finalize-confirm-box').hide();
    $('#edit-finalize').slideDown();
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


  Drupal.Ajax.plugins.oaportal = function(hook, args) {
    console.log(args);
    if (hook === 'submit') {
      fieldsets_hide_all();
      unlock_fields();
      $('#finalize-confirm-box').hide();
      // @TODO show throbber icon? 
    }
    if (hook === 'message') {
      // @TODO Find error fields, expand their fieldsets
      undo_finalize();
    }
    if (hook === 'redirect') {
      // @TODO Expose Continue button
      fieldsets_hide_all();
      lock_fields();
      $('#edit-continue').show().click(function() {
        Drupal.Ajax.redirect(args.redirect);
        return false;
      });

      return false;
    }
  }

});
