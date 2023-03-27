<!DOCTYPE html>
<html>
<head>
	<title>Welcome to Apache2 with PHP!</title>
</head>
<body>
	<h1>It works!</h1>
	<p>This is the default web page for Apache2 with PHP. If you see this page, it means that the web server software is installed and running on this server.</p>
	<p>This server is powered by Ubuntu Linux.</p>
	<hr>
	<p><em>Thank you for using Apache2 with PHP.</em></p>
	<?php
		echo "This page was generated using PHP version " . phpversion() . ".";
	?>
</body>
</html>
