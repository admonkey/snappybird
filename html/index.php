<?php
	$pageTitle = "Home Page";
	require_once('header.php');
?>

<h1><?php echo $siteTitle;?></h1>

<style>
	/* This selector overrides the points style on line charts. 
	Points on line charts are actually just very short strokes. 
	This allows you to customize even the point size in CSS */
	.ct-series-a .ct-point {
	  /* Colour of your points */
	  stroke: red;
	  /* Size of your points */
	  stroke-width: 6px;
	  /* Make your points appear as squares 
	  stroke-linecap: square;*/
	}
</style>

<script src="js/chartist.min.js"></script>

<div class="ct-chart ct-golden-section" id="chart1"></div>
<!--<div class="ct-chart ct-golden-section" id="chart2"></div>-->
<?php
	$query = "SELECT * FROM ScoreChangeCount 
			WHERE GameNumber > 330000  
			AND InstanceID = '40081c52-bb10-4f33-9692-527ffe1b540c'
		  ORDER BY GameNumber;";
	$result = mysql_query($query);
	if (!$result)
		die('Invalid query: ' . mysql_error());
	$labelCount = 36000;
	$seriesCount = 0;
	$labels = "36000";
	$series = "0";
	while ($row = mysql_fetch_assoc($result)) {
		/*while ( $labelCount != $row['GameNumber'] ){
			$labels .= "," . ++$labelCount;
			$series .= "," . $row['Score'];
		}*/
		$labels .= "," . $row['GameNumber'];
		$series .= "," . $row['Score'];
	}
?>
<script>
  // Initialize a Line chart in the container with the ID chart1
  new Chartist.Line('#chart1', {
    labels: [<?php echo $labels;?>],
    series: [{
    	name: 'series-1',
    	data: [<?php echo $series;?>]
    }]
  },{
    fullWidth: true,
    //showLine: false,
    series: {
    'series-1': {
      lineSmooth: Chartist.Interpolation.step()
    }
  }});

  /*// Initialize a Line chart in the container with the ID chart2
  new Chartist.Bar('#chart2', {
    labels: [1, 2, 3, 4],
    series: [[5, 2, 8, 3]]
  });*/
  
	var $chart = $('.ct-chart');

	var $toolTip = $chart
	  .append('<div class="chartist-tooltip"></div>')
	  .find('.chartist-tooltip')
	  .hide();

	$chart.on('mouseenter', '.ct-point', function() {
	  var $point = $(this),
	    value = $point.attr('ct:value'),
	    seriesName = $point.parent().attr('ct:series-name');
	  $toolTip.html(seriesName + '<br>' + value).show();
	});

	$chart.on('mouseleave', '.ct-point', function() {
	  $toolTip.hide();
	});

	$chart.on('mousemove', function(event) {
	  $toolTip.css({
	    left: (event.offsetX || event.originalEvent.layerX) - $toolTip.width() / 2 - 10,
	    top: (event.offsetY || event.originalEvent.layerY) - $toolTip.height() - 40
	  });
	});

</script>

<?php require_once('footer.php');?>
