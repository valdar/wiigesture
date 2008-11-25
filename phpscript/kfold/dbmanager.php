<?php

class dbManager{
	
	var $dbName;				// nome del database
	var $dbHost;				// nome dell'host su cui gira il database (localhost, oppure www.xyz.com)
	var $dbPass;				// password per il database
	var $dbUser;				// nome utente per la connesione al database


	function dbManager(){
	/* costruttore */
		$this->dbHost = "localhost";
		$this->dbName = "wiigesture";
		$this->dbPass = "1spam2pages3";
		$this->dbUser = "root";
	}
	
	function dbConnect(){
	/* funzione di connessione al database */
		$db = mysql_connect($this->dbHost, $this->dbUser, $this->dbPass)
			or die("Errore di connessione al dbms sull'host ".$this->dbHost);
		mysql_select_db($this->dbName, $db)
			or die("Impossibile selezionare il database ".$this->dbName);
		return $db;
	}
	
	function dbClose($db){
	/* chiude la connessione col database */
		mysql_close($db);
	}

	function execQuery($query, $db){
	/* funzione di esecuzione query mySQL */
		$result = mysql_query($query, $db)
			or die("Errore nell'esecuzione della query...<br/>".mysql_error());
		return $result;
	}

}

?>
