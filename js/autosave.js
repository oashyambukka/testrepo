Drupal.behaviors.autosave = function (context) {
  var timeout;
  var autosaved = Drupal.settings.autosave;   

  // Fieldset placeholder for Autosave status
  $('form.oaportal fieldset legend').append('<span class="fieldset-autosave-status">TESTRP</span>');

  // Set form to autosaved values, should they exist
  if (autosaved.serialized) {
    $('#' + autosaved.form_id_id).formHash(autosaved.serialized);
    $('form.oaportal fieldset legend .fieldset-autosave-status').text('Autosave recovered');
  }

  // If keypress, copy, paste or cut in any input or textarea, notify that the form has changed but has not been autosaved
  var changesNotSaved = function(event) {
    $('.fieldset-autosave-status', $(event.target).parents('fieldset.collapsible')).text('Changes not saved');
    // Wait 2 minutes before autosaving
    timeout = setTimeout('Drupal.sendAutosave()', 120000);
  }
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal textarea').bind('keyup', changesNotSaved);
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal textarea').bind('copy', changesNotSaved);
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal textarea').bind('paste', changesNotSaved);
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal textarea').bind('cut', changesNotSaved);


  // If any form element changes, send the Autosave
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal select, form.oaportal textarea').change(function() {
    Drupal.sendAutosave();
  });

  // Autosave fuction
  Drupal.sendAutosave = function() {
    clearTimeout(timeout);
    var serialized = $('#' + autosaved.form_id_id).formHash();
    serialized['q'] =  Drupal.settings.autosave.q;
    $.ajax({
      url: Drupal.settings.autosave.url,
      type: "POST",
      dataType: "xml/html/script/json",
      data: serialized,
      complete: function(XMLHttpRequest, textStatus) {
        $('form.oaportal .fieldset-autosave-status').text('Changes saved');
      }
    });
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
