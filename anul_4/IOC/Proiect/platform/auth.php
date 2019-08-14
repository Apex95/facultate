<?php
    // TODO: de verificat datele de intrare sa fie alfanumerice...



    session_start();

    require_once "includes/db.php";
    require_once "includes/miscs.php";
    require_once "includes/user.php";

    if (!isset($_GET["action"]))
    {
        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                              [ MESSAGE_TYPE::LOGIN_INVALID_ACTION, MESSAGE_DESC::LOGIN_INVALID_ACTION ]);
        exit;
    }
        
        
    if ($_GET["action"] == "login")
    {
        if (user::isLoggedIn())
        {
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::LOGIN_ALREADY_LOGGED_IN, MESSAGE_DESC::LOGIN_ALREADY_LOGGED_IN ]);
            exit;
        }
        
        if (!isset($_GET["username"]) || !isset($_GET["password"]))
        {
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::LOGIN_NO_DATA, MESSAGE_DESC::LOGIN_NO_DATA ]);
            exit;
        }
            
        $db = new database();
        $db->connect();
        
        $username = strtolower($db->escapeString($_GET["username"]));
        $password = $db->escapeString($_GET["password"]);
            
        $query = "SELECT count(*) as N_OF_MATCHES 
                  FROM users 
                  WHERE username='".$username."' AND password='".$password."'";
            
        $result = $db->query($query);
        $row = mysqli_fetch_assoc($result);
            
        if ($row["N_OF_MATCHES"] > 0)
        {
            user::logIn($username);
            user::updateOnlineTimestamp($db);
            
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::LOGIN_SUCCESS, MESSAGE_DESC::LOGIN_SUCCESS ]);
        }
        else
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::LOGIN_FAIL, MESSAGE_DESC::LOGIN_FAIL ]);   

        
        
        $db->close();
        
    }
    else
        if ($_GET["action"] == "logout")
        {
            if (!user::isLoggedIn())
            {
                echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                              [ MESSAGE_TYPE::LOGIN_REQUIRED, MESSAGE_DESC::LOGIN_REQUIRED ]);
                exit;
            }
            else
            {
                user::logOut();
                echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                              [ MESSAGE_TYPE::LOGIN_SUCCESS_LOGOUT, MESSAGE_DESC::LOGIN_SUCCESS_LOGOUT ]);
            }
        }
        else
            if ($_GET["action"] == "register")
            {
                if (user::isLoggedIn())
                {
                    echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                  [ MESSAGE_TYPE::LOGIN_ALREADY_LOGGED_IN, MESSAGE_DESC::LOGIN_ALREADY_LOGGED_IN ]);
                    exit;
                }
                
                if (!isset($_GET["username"]) || !isset($_GET["password"]) || !isset($_GET["email"]))
                {
                    echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                  [ MESSAGE_TYPE::REGISTER_NO_DATA, MESSAGE_DESC::REGISTER_NO_DATA ]);
                    exit;
                }
                

                $db = new database();
                $db->connect();
                
                $username = strtolower($db->escapeString($_GET["username"]));
                $password = $db->escapeString($_GET["password"]);
                $email = strtolower($db->escapeString($_GET["email"]));
                
                
                // -- verific daca nu exista deja un cont cu username-ul asta
                $query = "SELECT count(*) as N_OF_MATCHES 
                          FROM users 
                          WHERE username='".$username."'";
                
                $result = $db->query($query);
                $row = mysqli_fetch_assoc($result);
                
                if ($row["N_OF_MATCHES"] > 0)
                {
                    echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ], 
                                                  [ MESSAGE_TYPE::REGISTER_USERNAME_ALREADY_TAKEN, MESSAGE_DESC::REGISTER_USERNAME_ALREADY_TAKEN ]);
                    $db->close();
                    exit;
                }
                
                // -- inserez datele in DB
                $query = "INSERT INTO users (username, password, email)
                          VALUES ('".$username."', '".$password."', '". $email."')";
                
                $result = $db->query($query);
                
                
                if ($result)
                {
                    $query = "INSERT INTO profiles (username, profile_image)
                              VALUES ('".$username."', 'null.png')";
                
                    $result = $db->query($query);
                    
                    if ($result)
                    {
                        user::logIn($username);
                        user::updateOnlineTimestamp($db);
                        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                      [ MESSAGE_TYPE::REGISTER_SUCCESS_AND_LOGGED_IN, MESSAGE_DESC::REGISTER_SUCCESS_AND_LOGGED_IN ]);
                    }
                    
                    // else ceva problema de consistenta
                }
                
                
                $db->close();
            }
            else
            {
                echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                              [ MESSAGE_TYPE::LOGIN_INVALID_ACTION, MESSAGE_DESC::LOGIN_INVALID_ACTION ]);
                exit;
            }
?>
