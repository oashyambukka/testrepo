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


  $('#edit-delivery-rdo-delivery-option-directdeposit-wrapper input').change(function () {
    var value = $(this).val();
    fieldset_hide('fieldset-directdeposit');
    fieldset_hide('fieldset-cashpay');
    if (value == 'cashpay' || value = 'directdeposit') {
      fieldset_show('fieldset-' + value);
    }
  });
  
  function fieldset_hide($id) {
    if (!$(id).hasClass('collapsed')) {
      $('#fieldset-directdeposit legend a').click();
    }
  }
  function fieldset_show($id) {
    if ($(id).hasClass('collapsed')) {
      $('#fieldset-directdeposit legend a').click();
    }
  }

});
