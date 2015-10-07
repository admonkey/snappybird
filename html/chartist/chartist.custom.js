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
