$(document).ready(function() {
	$(".oap-tooltip-trigger").click(function(){
		$(".oap-tooltip").css("display","block");
	}, function(){
		$(".oap-tooltip").css("display","none");
	});
	$(".oap-tooltip .close").click(function(){
		$(".oap-tooltip").css("display","none");
	});
	$(".add-another-link").click(function() {
		$(this).hide();
		$(this).parent().next().slideDown();
	});
});
