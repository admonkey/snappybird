function toggleGrid(){
	var sw = $('.ct-grid').css( "stroke-width");
	if(sw == "1px") {
		$('.ct-grid').css( "stroke-width", "0px" );
		$('#gridtoggle').removeClass('active');
	} else {
		$('.ct-grid').css( "stroke-width", "1px" );
		$('#gridtoggle').addClass('active');
	}
}
function toggleLine(){
	var sw = $('.ct-line').css( "stroke-width");
	if(sw == "1px") {
		$('.ct-line').css( "stroke-width", "0px" );
		$('#linetoggle').removeClass('active');
	} else {
		$('.ct-line').css( "stroke-width", "1px" );
		$('#linetoggle').addClass('active');
	}
}
function contextMenu() {
	$('.ct-point').bind("contextmenu", function(e) {
	    $('#menu').css({
		top: e.pageY+'px',
		left: e.pageX+'px'
	    }).show();

	    return false;
	});
	$('#contextM').addClass('active');
}
$(document).ready(function() {

    $('#menu').click(function() {
        $('#menu').hide();
    });
    $(document).click(function() {
        $('#menu').hide();
    });

});
