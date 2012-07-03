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

}

Drupal.behaviors.addAnotherLink = function (context) {
  $(".add-another-link").click(function() {
    $(this).hide();
    $(this).parent().next().slideDown();
  });
}
 
Drupal.behaviors.finalization = function (context) { 

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
    autosave_on = false;
  }
  function undo_finalize() {
    $('#form-throbber').hide();
    $('#finalize-confirm-box').hide();
    $('#edit-finalize').show();
    unlock_fields();
    $('fieldset').each(function() {
      if ($(this).hasClass('collapsed')) {
        $('legend a', this).click();
      }
    });
    autosave_on = true;
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
  var errors_exist = false;
  Drupal.Ajax.plugins.oaportal = function(hook, args) {
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

      // Pull in new doc links with AJAX
      $('.oap-doc-download').hide().parent().show();
      $('.oap-doc-download').html('<div id="docs-throbber"></div>');
      $('.oap-doc-download').parent().slideDown();
      $('.oap-doc-download').slideDown();
      $.ajax({
        url: window.location.pathname + '/docs',
        type: "GET",
        dataType: "json",
        data: {},
        complete: function(XMLHttpRequest, textStatus) {
        },
        success: function(data) {
          $('.oap-doc-download').replaceWith(data.docs_markup);
          $('.form-throbber').slideUp();
          $('.oap-doc-download').parent().show();
        }
      });
      
      
      $('#edit-continue').show().click(function() {
        Drupal.Ajax.redirect(args.redirect);
        return false;
      });

      return false;
    }
  }

  $('.portal-continue').click(function() {
    Drupal.Ajax.redirect('/portal');
    return false;
  });

}

Drupal.behaviors.messages = function (context) {
  $('.oap-message-link a').click(function() {
    $(this).parent().next().slideDown();

    // Mark message as read
    var msg_id = $(this).attr('msg_id');
    $.ajax({
      url: '/portal/message/read/' + msg_id + '/TRUE',
      type: "POST",
      dataType: "json",
      data: {},
      complete: function(XMLHttpRequest, textStatus) {
      },
      success: function(data) {
      }
    });
    return false;
  });

  $('.close-message-link').click(function() {
    $(this).parent().parent().slideUp();
    return false;
  });
}
