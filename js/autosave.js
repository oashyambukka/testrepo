var autosave_on = true;

Drupal.behaviors.autosave = function (context) {
  var timeout;
  var autosaved = Drupal.settings.autosave;
  var autosave_lock;

  // Fieldset placeholder for Autosave status
  if (!$('form.oaportal fieldset legend .fieldset-autosave-status .locked')) {
    $('form.oaportal fieldset legend').append('<span class="fieldset-autosave-status"></span>');
  }

  // Set form to autosaved values, should they exist
  if (autosaved.serialized) {
    autosaved.serialized.form_build_id = $('input[name=form_build_id]').val();
    autosaved.serialized.form_token = $('input[name=form_token]').val();
    $('#' + autosaved.form_id_id).formHash(autosaved.serialized);
    $('form.oaportal fieldset legend .fieldset-autosave-status').text('Autosave recovered').addClass('autosaved-recovered');
  }

  // If keypress, copy, paste or cut in any input or textarea, notify that the form has changed but has not been autosaved
  var changesNotSaved = function(event) {
    $('.fieldset-autosave-status', $(event.target).parents('fieldset.collapsible')).text('Changes not saved').removeClass('autosaved-recovered').removeClass('changes-saved').addClass('not-saved');
    // Wait 2 minutes before autosaving
    timeout = setTimeout('Drupal.sendAutosave()', 120000);
  }
  var allFields = $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal textarea');
  allFields.bind('keyup', changesNotSaved);
  allFields.bind('copy', changesNotSaved);
  allFields.bind('paste', changesNotSaved);
  allFields.bind('cut', changesNotSaved);


  // If any form element changes, send the Autosave
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal select, form.oaportal textarea').change(function() {
    Drupal.sendAutosave();
  });

  // Autosave fuction
  Drupal.sendAutosave = function() {
    if (!autosave_on) {
      console.log('Autosave not on!');
    }
    if (!autosave_lock && autosave_on) {
      console.log('Sending autosave');
      autosave_lock = true;
      clearTimeout(timeout);
      var serialized = $('#' + autosaved.form_id_id).formHash();
      serialized['q'] =  Drupal.settings.autosave.q;
      $.ajax({
        url: Drupal.settings.autosave.url,
        type: "POST",
        dataType: "xml/html/script/json",
        data: serialized,
        complete: function(XMLHttpRequest, textStatus) {
          autosave_lock = false;
          $('form.oaportal .fieldset-autosave-status').text('Changes saved').removeClass('autosaved-recovered').removeClass('not-saved').addClass('changes-saved');
        }
      });
    }
  }

  Drupal.attachAutosave = function() {
    timeout = setTimeout('Drupal.sendAutosave()', Drupal.settings.autosave.period * 1000);
  }

  Drupal.Ajax.plugins.autosave = function(hook, args) {
    if (hook === 'submit') {
      clearTimeout(timeout);
    }
  }

}
