<?php
	$user=$_POST['username'];
	$image=$_FILES['myfile'];
	echo "Hello $user <br/>";
	echo "File Name<b>::</b> ".$image['name'];

	move_uploaded_file($image['tmp_name'],"photos/".$image['name']);
?>
