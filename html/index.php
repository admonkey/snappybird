<?php
	$pageTitle = "Home Page";
	require_once('header.php');
?>

<div class="ct-chart ct-golden-section" id="chart1"></div>
<?php
	$startGame = 245000;
	$query = "SELECT * FROM ScoreChangeCount 
			WHERE GameNumber > " . $startGame . "  -- AND GameNumber < 330000
			AND InstanceID = '40081c52-bb10-4f33-9692-527ffe1b540c'
		  ORDER BY GameNumber
		   LIMIT 30;";
	$result = mysql_query($query);
	if (!$result)
		die('Invalid query: ' . mysql_error());
	$labelCount = $startGame;
	$seriesCount = 0;
	$labels = $startGame;
	$series = "{meta: 'Game# " . $startGame . "', value: 0 }";
	while ($row = mysql_fetch_assoc($result)) {
		while ( $labelCount != $row['GameNumber'] ){
			$labels .= "," . ++$labelCount;
			$series .= ",{meta: 'Game# " . $labelCount . "', value: " . $row['Score'] . "}";
		}
		$labels .= "," . $row['GameNumber'];
		$series .= ",{meta: 'Game# " . $labelCount . "', value: " . $row['Score'] . "}";
	}
?>
<script>

var chart = new Chartist.Line('.ct-chart', {
  labels: [<?php echo $labels;?>],
  series: [
      [<?php echo $series;?>]
  ]
}, {
  fullWidth: true,
  lineSmooth: Chartist.Interpolation.step(),
  plugins: [
    Chartist.plugins.tooltip()
  ]
});

</script>

<?php require_once('footer.php');?>
