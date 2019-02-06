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
	$data = "";

	for($i = 0; $i < $numline; $i++){

		$line = str_replace($originali, $sostituti, $testo[$i]);

		if($line != "<gesture>" && $line != "</gesture>"){

			echo($line . "<br/>\n");
			$data = $data . $line ."\n";
			


		}
		else if($line == "</gesture>"){
			$query = "insert into ".$tabella." values('', '".$data."');";
			$dbMng->execQuery($query, $db);
			$data = "";
		}

	}

	$dbMng->dbClose($db);

?> 
