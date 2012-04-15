$(document).ready(function() {

  autosaved = Drupal.settings.autosave;   

  // Set form to autosaved values, should they exist
  if (autosaved.serialized) {
console.log('Autosave recovered');
console.log(autosaved);
console.log($('#' + autosaved.form_id_id));
    $('#' + autosaved.form_id_id).formHash(autosaved.serialized);
  }
  

  Drupal.sendAutosave = function() {
console.log('Autosave underway. Autosaved:');
console.log(autosaved);
    var serialized = $('#' + autosaved.form_id_id).formHash();
    serialized['q'] =  Drupal.settings.autosave.q;
    $.ajax({
      url: Drupal.settings.autosave.url,
      type: "POST",
      dataType: "xml/html/script/json",
      data: serialized,
      complete: function(XMLHttpRequest, textStatus) {
        Drupal.displaySaved();
        Drupal.attachAutosave();
      }
    });
  }

  Drupal.displaySaved = function() {
console.log('Autosave sent');
//    $('#autosave-status #status').html('Form autosaved.');
  }

  Drupal.attachAutosave = function() {
console.log('Autosave queued');
    setTimeout('Drupal.sendAutosave()', Drupal.settings.autosave.period * 1000);
  }

//  Drupal.attachAutosave();

});
