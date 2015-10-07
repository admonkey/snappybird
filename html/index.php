<?php
	$pageTitle = "Home";
	require_once('header.php');
?>

<?php
	$query = "SELECT * FROM InstanceSettings ORDER BY EndTime DESC";
	$result = mysql_query($query);
	if (!$result)
		die('Invalid query: ' . mysql_error());
	$numfields = mysql_num_fields($result);
	echo "<table class='table table-condensed table-responsive table-bordered table-striped tablesorter'><thead><tr>";
	for ($i=0; $i < $numfields; $i++) // Header
	{ echo '<th>'.mysql_field_name($result, $i).'</th>'; }
	echo "</tr></thead><tbody>";
	while ($row = mysql_fetch_assoc($result)) {
		echo '<tr>';
		for ($i=0; $i < $numfields; $i++) // Header
		{
			if(mysql_field_name($result, $i) == "InstanceID")
				echo '<td><a href="scorechanges.php?instanceid=' . $row["InstanceID"] . '">'.$row["InstanceID"].'</a></td>';
			elseif(mysql_field_name($result, $i) == "PreviousInstanceID")
				echo '<td><a href="scorechanges.php?instanceid=' . $row["PreviousInstanceID"] . '">'.$row["PreviousInstanceID"].'</a></td>';
			else
				echo '<td>'.$row[mysql_field_name($result, $i)].'</td>';
		
		}
		echo '</tr>';
	}
	echo "</tbody></table>";
?>

<script>$(".tablesorter").tablesorter();</script>

<?php require_once('footer.php');?>
