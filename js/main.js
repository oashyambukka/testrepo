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

});
