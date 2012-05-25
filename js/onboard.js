Drupal.behaviors.onboard = function (context) {

  // If email address field changes, check for dupes
  $('#edit-Email').change(function() {
    var data = {};
    data['email'] = $(this).val();
    $.ajax({
      url: '/apply/dupecheck',
      type: "POST",
      dataType: "json",
      data: data,
      complete: function(XMLHttpRequest, textStatus) {
//console.log(XMLHttpRequest);
      },
      success: function(data) {
        if (data.status == 'dupe') {
          $('#email-address-is-dupe').slideDown();
          $('#edit-submit').hide();
        }
        else {
          $('#email-address-is-dupe').slideUp();
          $('#edit-submit').show();
        }
      }
    });
  });

}
