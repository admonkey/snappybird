<?php
	$pageTitle = "Score Changes";
	$graphcontrols = true;
	require_once('header.php');
?>
<script>$("#scorechangesphp").addClass("active");</script>
<?php
	if(empty($_GET["instanceid"]))
		die("error: no instance id specified.");
	$instanceid = mysql_real_escape_string($_GET["instanceid"]);

	if($_GET["expanded"] == "true")
		$expanded = true;
	else
		$expanded = false;

	if(is_numeric($_GET["startGame"])) {
		$startGame = $_GET["startGame"];
	} else {
		$query = " SELECT MIN(GameNumber) AS sg FROM ScoreChangeCount WHERE InstanceID = '" . $instanceid . "' 
				AND GameNumber <> 0";
		$result = mysql_query($query);
		if (!$result)
			die('Invalid query: ' . mysql_error());
		$row = mysql_fetch_assoc($result);
		if(!empty($row["sg"]))
			$startGame = $row["sg"];
		else	$startGame = 0;
	}

	if(is_numeric($_GET["endGame"]))
		$endGame = $_GET["endGame"];

	$query = " SELECT GameNumber, Score FROM ScoreChangeCount 
			WHERE GameNumber > " . $startGame;
	if(!empty($endGame))
		$query .= " AND GameNumber < " . $endGame;
	$query .= " AND InstanceID = '" . $instanceid . "' ";
	$query .= " ORDER BY GameNumber;
		--   LIMIT 40;";
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
<div class="ct-chart ct-golden-section" id="chart1"></div>
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

<div id="menu">
	<a href="http://google.com">google</a>
</div>

<?php require_once('footer.php');?>
