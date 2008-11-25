<?php

	include_once("dbmanager.php");

	$file = $_POST['file'];
	$tabella = $_POST['tabella'];

	$originali = array("\n");
	$sostituti = array("");

	$dbMng = new dbManager();
	$db = $dbMng->dbConnect();

	$testo = @file($file);
	$numline = count($testo);

	for($i = 0; $i < $numline; $i++){

		$line = str_replace($originali, $sostituti, $testo[$i]);

		if($line != "<gesture>" && $line != "</gesture>"){

			echo($line . "<br/>\n");
			$query = "insert into ".$tabella." values('', '".$line."');";
			$dbMng->execQuery($query, $db);


		}

	}

	$dbMng->dbClose($db);

?> 
