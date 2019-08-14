
<?php

    require_once "includes/miscs.php";

	class database
	{
		private $host = "localhost";
		private $db = "test";
		private $user = "root";
		private $pass = "";

		public $time = 0;
		public $queries = 0;
        
        private $conn;
        private $testConn;

		function testConnection()
		{
			if ($this->testConn = mysqli_connect($this->host, $this->user, $this->pass))
			{
				mysqli_close($this->testConn);
				return true;
			}
			else 
				return false;
				
			
			return false;
		}

		function connect()
		{
            
			if (!($this->conn = mysqli_connect($this->host, $this->user, $this->pass)))
				die('Database server not available');

			if (!(mysqli_select_db($this->conn, $this->db)))
				die('Database not found');
			
		}
        
        function escapeString($string)
        {
            return mysqli_real_escape_string($this->conn, $string);
        }
        
        
        function getAffectedRows()
        {
            return mysqli_affected_rows($this->conn);
        }

		function close()
		{
			mysqli_close($this->conn);
		}

		function query($q='')
		{
			$startTime = microtime(true);
			$result = mysqli_query($this->conn, $q)  or die(mysqli_error($this->conn));
            
            if (!$result)
            {
                echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                              [ MESSAGE_TYPE::DB_NOT_REACHABLE, MESSAGE_DESC::DB_NOT_REACHABLE ]);
                $db->close();
                exit;
            }
            
			$endTime = microtime(true);	
			
			$this->queries++;
			$this->time = $this->time + ($endTime - $startTime);
			
			return $result;
		}
        
        
	}
?>