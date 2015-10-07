<?php
	$pageTitle = "Home Page";
	require_once('header.php');
?>

<div class="ct-chart ct-golden-section" id="chart1"></div>
<?php
	if(empty($_GET["instanceid"]))
		die("error: no instance id specified.");
	$instanceid = mysql_real_escape_string($_GET["instanceid"]);

	if($_GET["expanded"] == "true")
		$expanded = true;
	else
		$expanded = false;

	$startGame = 248600;
	$query = "SELECT GameNumber, Score FROM ScoreChangeCount 
			WHERE GameNumber > " . $startGame . "  -- AND GameNumber < 330000
			AND InstanceID = '" . $instanceid . "'
		  ORDER BY GameNumber
		   LIMIT 40;";
	$result = mysql_query($query);
	if (!$result)
		die('Invalid query: ' . mysql_error());
	$labelCount = $startGame;
	$seriesCount = 0;
	$labels = $startGame;
	$series = "{meta: 'Game: " . $startGame . "', value: 0 }";
	while ($row = mysql_fetch_assoc($result)) {
		if($expanded) while ( $labelCount != $row['GameNumber'] ){
			$labels .= "," . ++$labelCount;
			$series .= ",{meta: 'Game: " . $labelCount . "', value: " . $row['Score'] . "}";
		}
		$labels .= "," . $row['GameNumber'];
		$series .= ",{meta: 'Game: " . ($expanded ? $labelCount : $row['GameNumber']) . "', value: " . $row['Score'] . "}";
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
