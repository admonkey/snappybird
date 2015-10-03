<?php
	$siteTitle = "Snappy Bird";

	// declare database variables
	$username = "snappyAgent";
	$password = "TLQPdTfsGqm2utb4";
	$hostname = "localhost";
	$database = "snappy";

	//connection to the database
	$dbhandle = mysql_connect($hostname, $username, $password)
	  or die("Unable to connect to MySQL");

	//select a database to work with
	$selected = mysql_select_db($database,$dbhandle)
	  or die("Could not select database");
?>
