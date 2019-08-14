<?php
    session_start();
    
    require_once "includes/user.php";
    require_once "includes/miscs.php";
    require_once "includes/db.php";
    
    if (!user::isLoggedIn())
    {
        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                      [ MESSAGE_TYPE::LOGIN_REQUIRED, MESSAGE_DESC::LOGIN_REQUIRED ]);
        exit;
    }
    
    if (!isset($_GET["action"]))
    {
        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                      [ MESSAGE_TYPE::PROFILE_INVALID_ACTION, MESSAGE_DESC::PROFILE_INVALID_ACTION ]);
        exit;
    }
    
    
    if ($_GET["action"] == "view")
    {
        $username = user::getUsername();
        
        
        $db = new database();
        $db->connect();
        
        if (isset($_GET["username"]))
            $username = strtolower($db->escapeString($_GET["username"]));
        
        $query = "SELECT first_name, last_name, profile_image, birthday, description
                  FROM profiles
                  WHERE username = '".$username."'";
                  
        
        
        $result = $db->query($query);
        
        
        // daca nu exista profilul...
        if (mysqli_num_rows($result) == 0)
        {
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::PROFILE_NOT_FOUND, MESSAGE_DESC::PROFILE_NOT_FOUND ]);
            
            $db->close();   
            exit;
        }
        
        
        $row = mysqli_fetch_assoc($result);
        $birthday = date("d-m-Y", strtotime($row["birthday"]));
        
        echo miscs::constructResponse([ 
                                        JSON_CONSTS::MESSAGE_TYPE, 
                                        JSON_CONSTS::MESSAGE_DESC,

                                        JSON_CONSTS::PROFILE_FIRSTNAME,
                                        JSON_CONSTS::PROFILE_LASTNAME,
                                        JSON_CONSTS::PROFILE_IMAGE,
                                        JSON_CONSTS::PROFILE_BIRTHDAY,
                                        JSON_CONSTS::PROFILE_DESCRIPTION
                                      ],
                                      [ 
                                        MESSAGE_TYPE::PROFILE_VIEW_INFO, 
                                        MESSAGE_DESC::PROFILE_VIEW_INFO,

                                        $row["first_name"],
                                        $row["last_name"],
                                        miscs::generateImageUrl($row["profile_image"]),
                                        $birthday,
                                        $row["description"]
                                      ]);
        
        
        $db->close();
    }
    else
        if ($_GET["action"] == "update")
        {
            // TODO: uploadul de poze...mai tarziu...
            
            // verific daca chiar se actualizeaza ceva ca sa nu fac o conexiune la DB degeaba
            if (!isset($_GET["first_name"]) && !isset($_GET["last_name"])
                && !isset($_GET["birthday"]) && !isset($_GET["description"]) && !isset($_FILES["file"]))
            {
                echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                              [ MESSAGE_TYPE::PROFILE_NO_DATA, MESSAGE_DESC::PROFILE_NO_DATA ]);
                                              
                exit;
            }
            
            $db = new database();
            $db->connect();

            
            $username = user::getUsername();
            
            if (isset($_GET["first_name"]))
            {
                $firstName = $db->escapeString($_GET["first_name"]);
                $query = "UPDATE profiles SET first_name = '".$firstName."'
                                         WHERE username = '".$username."'";
                                         
                $db->query($query);
                
            }
            
            if (isset($_GET["last_name"]))
            {
                $lastName = $db->escapeString($_GET["last_name"]);
                $query = "UPDATE profiles SET last_name = '".$lastName."'
                                         WHERE username = '".$username."'";
                                         
                $db->query($query);
            }
            
            if (isset($_GET["birthday"]))
            {
                $birthday = $db->escapeString($_GET["birthday"]);
                $birthday = date("Y-m-d", strtotime($birthday));
 
                $query = "UPDATE profiles SET birthday = CAST('".$birthday."' AS DATE)
                                         WHERE username = '".$username."'";
                                         
                $db->query($query);
            }
            
            if (isset($_FILES["file"]))
            {
                $isImage = getimagesize($_FILES["file"]["tmp_name"]);
                
                $imgDir = "imgs/";
                
                
                if ($_FILES["file"]["size"] > 5000000) 
                {
                    echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                  [ MESSAGE_TYPE::PROFILE_INVALID_IMAGE, MESSAGE_DESC::PROFILE_INVALID_IMAGE ]);
                                              
                    exit;
                }
                
                if ($isImage !== false) 
                {
                    $timestamp = date_timestamp_get(date_create());
                    
                    
                    $fileName = $db->escapeString($username."-".$timestamp."-".basename($_FILES["file"]["name"]));
                    
                    if (move_uploaded_file($_FILES["file"]["tmp_name"], $imgDir.$fileName))
                    {
                        $query = "UPDATE profiles
                                  SET profile_image = '".$fileName."'
                                  WHERE username = '".$username."'";
                        
                        $db->query($query);
                        
                    }

                } 
                else 
                {
                    echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                  [ MESSAGE_TYPE::PROFILE_INVALID_IMAGE, MESSAGE_DESC::PROFILE_INVALID_IMAGE ]);
                }
            }
            
            if (isset($_GET["description"]))
            {
                $description = $db->escapeString($_GET["description"]);
                
                $query = "UPDATE profiles SET description = '".$description."'
                                         WHERE username = '".$username."'";
                
                $db->query($query);
            }
            
            
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::PROFILE_UPDATED_SUCCESSFULLY, MESSAGE_DESC::PROFILE_UPDATED_SUCCESSFULLY ]);
            
            
            
            $db->close();
            
        }
        else
        {
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::PROFILE_INVALID_ACTION, MESSAGE_DESC::PROFILE_INVALID_ACTION ]);
            exit;
        }
        
    

?>