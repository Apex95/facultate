<?php

    class user
    {
        static function isLoggedIn()
        {
            if (isset($_SESSION["username"]))
                return true;
            
            return false;
        }
        
        static function logOut()
        {
            session_unset($_SESSION["username"]);       
        }
        
        static function logIn($username)
        {
            $_SESSION["username"] = $username;
        }
        
        static function getUsername()
        {
            return $_SESSION["username"];
        }
        
        static function updateOnlineTimestamp($db)
        {
            if (user::isLoggedIn())
            {
                $currentDate = date("Y-m-d H:i:s");
                $query = "UPDATE users SET last_online = '".$currentDate."' WHERE username = '".$_SESSION["username"]."'";
                $db->query($query);
            }
        }

        
    }


?>