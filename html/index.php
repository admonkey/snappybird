<?php
	$pageTitle = "Home Page";
	require_once('header.php');
?>

<h1><?php echo $siteTitle;?></h1>

<script src="js/chartist.min.js"></script>

<div class="ct-chart ct-golden-section" id="chart1"></div>
<!--<div class="ct-chart ct-golden-section" id="chart2"></div>-->
<?php
	$query = "SELECT * FROM ScoreChangeCount WHERE GameNumber <> 0 AND InstanceID = '38b39fa6-2084-4754-ba60-89656b5569bd' ORDER BY GameNumber LIMIT 50;";
	$result = mysql_query($query);
	if (!$result)
		die('Invalid query: ' . mysql_error());
	$labels = "0";
	$series = "0";
	while ($row = mysql_fetch_assoc($result)) {
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

  // Initialize a Line chart in the container with the ID chart2
  new Chartist.Line('#chart2', {
    labels: [1, 2, 3, 4],
    series: [[5, 2, 8, 3]]
  });
</script>

<?php require_once('footer.php');?>
