<?php include_once("dbmanager.php"); ?>

<html>
<head><title>3-fold generator</title></head>

<body>

<form action="index.php?action=gen" method="post">
	<input type="text" name="tabella"/>&nbsp;Tabella sorgente<br/>
	<input type="text" name="file"/>&nbsp;Nome file destinazione<br/>
	<input type="submit" value="Invia"/>
	<input type="reset" value="Cancella"/>
</form>

<p>Tabelle presenti nel database:</p>

<?php

$dbmng = new dbManager();
$db = $dbmng->dbConnect();

// elenca le tabelle nel database wiigesture
$q1 = "show tables from ".$dbmng->dbName.";";
$result1 = $dbmng->execQuery($q1, $db);

while($row = mysql_fetch_row($result1)){
	echo($row[0]."<br/>\n");
}

// cartella destinazione dei file
$filepath = "/home/lynwis/Projects/git_repo/wiigesture/dataset/kfold-php/";

if(isset($_GET['action']) && $_GET['action'] == "gen"){
	
	$tab = $_POST['tabella'];
	
	// genera nomi dei file
	$filename = $filepath . $_POST['file'];
	
	$train1 = $filename . "_train1.txt";
	$train2 = $filename . "_train2.txt";
	$train3 = $filename . "_train3.txt";
	$valid1 = $filename . "_valid1.txt";
	$valid2 = $filename . "_valid2.txt";
	$valid3 = $filename . "_valid3.txt";

	// recupera le gesture dal db
	$q2 = "select gesture from ".$tab.";";
	$result2 = $dbmng->execQuery($q2, $db);

	$data = array();
	
	while($row = mysql_fetch_row($result2)){
		
		$data[] = $row[0];
	}
	
	// randomizza le gesture
	shuffle($data);
	
	// n° gesture e dimensione delle 3 fold da creare
	$size = count($data);
	$split = $size / 3;
	$part1 = $part2 = $part3 = $split;
	
	if($size % 3 == 1){
			
		$part1++;
		
	}
	else if($size % 3 == 2){
		
		$part1++;
		$part2++;
		
	}
	else{
		
	}
	
	// generazione file
	for($i=0; $i<$size; $i++){
		
		if($i<$part1){
			
			$t1 = fopen($train1, "a");
			fwrite($t1, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
			$t2 = fopen($train2, "a");
			fwrite($t2, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
			$v3 = fopen($valid3, "a");
			fwrite($v3, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
		}
		else if($part1 <= $i && $i < ($part1 + $part2)){
			
			$t1 = fopen($train1, "a");
			fwrite($t1, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
			$t3 = fopen($train3, "a");
			fwrite($t3, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
			$v2 = fopen($valid2, "a");
			fwrite($v2, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
		}
		else{
			
			$t2 = fopen($train2, "a");
			fwrite($t2, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
			$t3 = fopen($train3, "a");
			fwrite($t3, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
			$v1 = fopen($valid1, "a");
			fwrite($v1, "<gesture>\n".$data[$i]."\n</gesture>\n");
			
		}
	}
	
}

?>

</body>

</html>