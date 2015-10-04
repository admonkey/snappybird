function toggleGrid(){
	var sw = $('.ct-grid').css( "stroke-width");
	if(sw == "1px")
		$('.ct-grid').css( "stroke-width", "0px" );
	else
		$('.ct-grid').css( "stroke-width", "1px" );
}
