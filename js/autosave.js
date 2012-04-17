Drupal.behaviors.autosave = function (context) {
  var timeout;
  var autosaved = Drupal.settings.autosave;   

  // Fieldset placeholder for Autosave status
  $('form.oaportal fieldset legend:first a').after('<span class="fieldset-autosave-status"></span>');


  // Set form to autosaved values, should they exist
  if (autosaved.serialized) {
    $('#' + autosaved.form_id_id).formHash(autosaved.serialized);
    $('form.oaportal fieldset legend:first .fieldset-autosave-status').text('Autosave recovered');
  }


  // If keypress, copy, paste or cut in any input or textarea, notify that the form has changed but has not been autosaved
  var changesNotSaved = function(event) {
    console.log('changesNotSaved fired with event obj:');
    console.log(event);
    // @TODO change to target parent fieldset to the changed field only?
    $('form.oaportal fieldset legend:first .fieldset-autosave-status').text('Changes not saved');
    // Wait 2 minutes before autosaving
    timeout = setTimeout('Drupal.sendAutosave()', Drupal.settings.autosave.period * 1000);
  }
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal textarea').bind({
    keypress: changesNotSaved,
    copy: changesNotSaved,
    paste: changesNotSaved,
    cut: changesNotSaved
  });


  // If any form element changes, send the Autosave
  $('form.oaportal input[type!="hidden"][type!="submit"], form.oaportal select, form.oaportal textarea').change(function() {
    Drupal.sendAutosave();
  });

  Drupal.sendAutosave = function() {
    console.log('Autosave underway.');
    clearTimeout(timeout);
    var serialized = $('#' + autosaved.form_id_id).formHash();
    serialized['q'] =  Drupal.settings.autosave.q;
    $.ajax({
      url: Drupal.settings.autosave.url,
      type: "POST",
      dataType: "xml/html/script/json",
      data: serialized,
      complete: function(XMLHttpRequest, textStatus) {
        $('form.oaportal fieldset legend:first .fieldset-autosave-status').text('Changes saved');
      }
    });
  }

  Drupal.displaySaved = function() {
    console.log('Autosave sent');
//    $('#autosave-status #status').html('Form autosaved.');
  }

  Drupal.attachAutosave = function() {
    console.log('Autosave queued');
    timeout = setTimeout('Drupal.sendAutosave()', Drupal.settings.autosave.period * 1000);
  }

//  Drupal.attachAutosave();
  $('form.oaportal input, form.oaportal select, form.oaportal textarea').change(function() {
    // @TODO Verify change() is the correct event to bind to.
    console.log('Tick');
    Drupal.sendAutosave();
  });

  Drupal.Ajax.plugins.autosave = function(hook, args) {
    if (hook === 'submit') {
      console.log('Clearing timeout');
      clearTimeout(timeout);
    }
  }

}