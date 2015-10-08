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

function getNewQuery( p, v ){
	var querystring = location.search.replace( '?', '' ).split( '&' );
	var queryObj = {};
	// loop through each name-value pair and populate object
	for ( var i=0; i<querystring.length; i++ ) {
	      // get name and value
	      var name = querystring[i].split('=')[0];
	      var value = querystring[i].split('=')[1];
	      // populate object
	      queryObj[name] = value;
	}
	queryObj[p] = v;
	querystring = jQuery.param( queryObj );
	querystring = querystring.replace( '%5B', '[' );
	querystring = querystring.replace( '%5D', ']' );
	return querystring;
}

function contextMenu() {
	$('.ct-point').bind("contextmenu", function(e) {
	    var val = $( this ).attr( "ct:meta" );
	    $('#menu').css({
		top: e.pageY+'px',
		left: e.pageX+'px'
	    }).html(
	    	"<a href='?" + getNewQuery( "startGame", val ) + "'>Start Point</a><br/>" +
	    	"<a href='?" + getNewQuery( "endGame", val ) + "'>End Point</a>"
	    ).show();

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
