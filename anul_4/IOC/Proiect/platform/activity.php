<?php
    session_start();
    
    require_once "includes/db.php";
    require_once "includes/user.php";
    require_once "includes/miscs.php";
    
    if (!user::isLoggedIn())
    {
        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                      [ MESSAGE_TYPE::LOGIN_REQUIRED, MESSAGE_DESC::LOGIN_REQUIRED ]);
        exit;
    }
    
    if (!isset($_GET["action"]))
    {
        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                      [ MESSAGE_TYPE::ACTIVITY_INVALID_ACTION, MESSAGE_DESC::ACTIVITY_INVALID_ACTION ]);
        exit;
    }

    if ($_GET["action"] == "list")
    {
        if (!isset($_GET["max_range"]) || !isset($_GET["lat"]) || !isset($_GET["long"]))
        {
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::ACTIVITY_NO_DATA, MESSAGE_DESC::ACTIVITY_NO_DATA ]);
            exit;
        }

        
        
        $tagsList = [];
        
        $db = new database();
        $db->connect();
      

        // black magic; DON'T TOUCH THE QUERY!!
        
        if (isset($_GET["tags"]))
        {
            $tags = $db->escapeString($_GET["tags"]);
            $tagsList = explode(",", $tags);
        }
        
        $nOfTags = count($tagsList);
        
        
        $lat = $db->escapeString($_GET["lat"]);
        $long = $db->escapeString($_GET["long"]);
        $maxRange = $db->escapeString($_GET["max_range"]);
        
        $username = user::getUsername();
        
        // warning: DONT'T TOUCH!
        
        $query = "SELECT distinct(t.activity_id), t2.name, a.max_members, a.crt_members, a.author, a.time, a.location_lat, a.location_long, count(t.name) \"matches\"
                  FROM activities a, tags t, tags t2
                  WHERE a.id = t.activity_id AND t2.activity_id = a.id AND a.author <> '".$username."' 
                  
                        AND 111.045 * DEGREES(ACOS(COS(RADIANS(a.location_lat)) * COS(RADIANS(".$lat.")) *
                            COS(RADIANS(a.location_long) - RADIANS(".$long.")) +
                            SIN(RADIANS(a.location_lat)) * SIN(RADIANS(".$lat.")))) <= a.max_range
                                
                        AND 111.045 * DEGREES(ACOS(COS(RADIANS(a.location_lat)) * COS(RADIANS(".$lat.")) *
                            COS(RADIANS(a.location_long) - RADIANS(".$long.")) +
                            SIN(RADIANS(a.location_lat)) * SIN(RADIANS(".$lat.")))) <= ".$maxRange;
        
        
      

        
        if ($nOfTags > 0)
        {
             $query .= " AND t.name in (";
             
             for ($i = 0; $i < $nOfTags; $i++)
                if ($i < $nOfTags - 1)
                    $query .= "'".$tagsList[$i]."', ";
                else
                    $query .= "'".$tagsList[$i]."')";
        }

        $query .= " GROUP BY activity_id, t2.name
                   HAVING matches >= ".$nOfTags."
                   ORDER by matches DESC, activity_id DESC";
        
        $result = $db->query($query);
        
        $activityId = null;
        
        $tagsArray = [];
        $activitiesArray = [];
        
        $previousAuthor = null;
        $previusTime = null;
        $previousLat = null;
        $previousLong = null;
        $previousMaxMembers = null;
        $previousCrtMembers = null;
        
        while ($row = mysqli_fetch_assoc($result))
        {
            if ($activityId != $row["activity_id"])
            {
                // trece la o activitate noua, cu alte tags
                if ($activityId != null)
                {
                    $activitiesArray[$activityId] = [
                                                     JSON_CONSTS::ACTIVITY_AUTHOR => $previousAuthor, 
                                                     JSON_CONSTS::ACTIVITY_TIME => $previousTime,
                                                     JSON_CONSTS::ACTIVITY_LAT => $previousLat,
                                                     JSON_CONSTS::ACTIVITY_LONG => $previousLong,
                                                     JSON_CONSTS::ACTIVITY_MAX_MEMBERS => $previousMaxMembers,
                                                     JSON_CONSTS::ACTIVITY_TAGS_LIST => $tagsArray[$activityId]
                                                    ];
                }
                
                $activityId = $row["activity_id"];
                $tagsArray[$activityId] = "";
                
                $previousAuthor = $row["author"];
                $previousTime = $row["time"];
                $previousLat = $row["location_lat"];
                $previousLong = $row["location_long"];
                $previousMaxMembers = $row["max_members"];
                $previousCrtMembers = $row["crt_members"];
            }
            
            $tagsArray[$activityId] .= $row["name"].",";
        }
        
        if ($activityId != null)
            $activitiesArray[$activityId] = [
                                             JSON_CONSTS::ACTIVITY_AUTHOR => $previousAuthor, 
                                             JSON_CONSTS::ACTIVITY_TIME => $previousTime,
                                             JSON_CONSTS::ACTIVITY_LAT => $previousLat,
                                             JSON_CONSTS::ACTIVITY_LONG => $previousLong,
                                             JSON_CONSTS::ACTIVITY_CRT_MEMBERS => $previousCrtMembers,
                                             JSON_CONSTS::ACTIVITY_MAX_MEMBERS => $previousMaxMembers,
                                             JSON_CONSTS::ACTIVITY_TAGS_LIST => $tagsArray[$activityId]
                                            ];
        
        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC, JSON_CONSTS::ACTIVITY_TAG ],
                                      [ MESSAGE_TYPE::ACTIVITY_LIST, MESSAGE_DESC::ACTIVITY_LIST, $activitiesArray ]);
        
        
        
        $db->close();
        
    }
    else
        if ($_GET["action"] == "add")
        {
            
        if (!isset($_GET["time"]) || !isset($_GET["lat"]) || !isset($_GET["long"]) 
         || !isset($_GET["max_members"]) || !isset($_GET["tags"]) || !isset($_GET["max_range"]))
            {
                echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                              [ MESSAGE_TYPE::ACTIVITY_NO_DATA, MESSAGE_DESC::ACTIVITY_NO_DATA ]);
                exit;
            }

            $activityId = hexdec(uniqid());
            $author = user::getUserName();
            
            $db = new database();
            $db->connect();
            

            
            $time = $_GET["time"];
            $lat = $db->escapeString($_GET["lat"]);
            $long = $db->escapeString($_GET["long"]);
            
            $maxMembers = $db->escapeString($_GET["max_members"]);
            $maxRange = $db->escapeString($_GET["max_range"]);
            
            $time = date("Y-m-d H:i:s", $time);
            
            $query = "INSERT INTO activities (id, author, time, location_lat, location_long, max_members, max_range, crt_members)
                             VALUES (".$activityId.", '".$author."','".$time."', ".$lat.", ".$long.", ".$maxMembers.", ".$maxRange.", 1)";

            $db->query($query);    
            
            // si acu...tagurile
            $tags = $db->escapeString($_GET["tags"]);
            $tagsList = explode(",", $tags);
            
            $nOfTags = count($tagsList);
            
            $query = "INSERT INTO tags (activity_id, name)
                             VALUES";
            
            for ($i = 0; $i < $nOfTags; $i++)
            {
                if ($i == $nOfTags - 1)
                    $query .= " (".$activityId.", '".$tagsList[$i]."')";
                else
                    $query .= " (".$activityId.", '".$tagsList[$i]."'),";
            }
            
            $db->query($query);
            
            
            // inscriu organizatorul in propriul grup
            $query = "INSERT INTO parties (activity_id, username)
                      VALUES (".$activityId.", '".$author."')";
                      
            $db->query($query);
            
            
            
            
            $db->close();
            
            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                          [ MESSAGE_TYPE::ACTIVITY_SUCCESSFULLY_ADDED, MESSAGE_DESC::ACTIVITY_SUCCESSFULLY_ADDED ]);
        }
        else
            if ($_GET["action"] == "show_tags")
            {
                // TODO: tagurile preluate si sortate dupa nr de aparitii in baza de date.
                die("tot altadata...");
            }
            else
                if ($_GET["action"] == "attend")
                {
                    if (!isset($_GET["activity_id"]) || !isset($_GET["lat"]) || !isset($_GET["long"]))
                    {
                        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                      [ MESSAGE_TYPE::ACTIVITY_NO_DATA, MESSAGE_DESC::ACTIVITY_NO_DATA ]);
                        exit;
                    }
                    
                    $db = new database();
                    $db->connect();
                    
                    $activityId = $db->escapeString($_GET["activity_id"]);
                    $username = user::getUsername();
                    
                    // verific sa nu fie inscris deja
                    $query = "SELECT count(*) as n_joins FROM parties
                              WHERE username = '".$username."' AND activity_id = ".$activityId;
                              
                    $result = $db->query($query);
                    $row = mysqli_fetch_assoc($result);
                    
                    if ($row["n_joins"] > 0)
                    {
                        $db->close();
                        
                        // deja inscris
                        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                      [ MESSAGE_TYPE::ACTIVITY_ALREADY_ATTENDING, MESSAGE_DESC::ACTIVITY_ALREADY_ATTENDING ]);
                        
                        
                        exit;
                    }
                    
                    
                    
                    $lat = $db->escapeString($_GET["lat"]);
                    $long = $db->escapeString($_GET["long"]);
                    
                    $query = "UPDATE activities a
                              SET crt_members = crt_members + 1
                              WHERE id = ".$activityId."
                                AND crt_members < max_members
                                AND 111.045 * DEGREES(ACOS(COS(RADIANS(a.location_lat)) * COS(RADIANS(".$lat.")) *
                                    COS(RADIANS(a.location_long) - RADIANS(".$long.")) +
                                    SIN(RADIANS(a.location_lat)) * SIN(RADIANS(".$lat.")))) <= max_range";
                                
                    $result = $db->query($query);
                    
                    
                    
                    // se pot intampla niste chestii p-aici...sper ca nu totusi.
                    if ($db->getAffectedRows() <= 0)
                    {
                        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                      [ MESSAGE_TYPE::ACTIVITY_INVALID_ATTEND, MESSAGE_DESC::ACTIVITY_INVALID_ATTEND ]);
                        
                        $db->close();
                        exit;
                    }
                    else
                    {
                        // ajunge aici => a ocupat un slot
                        $query = "INSERT INTO parties (activity_id, username)
                                               VALUES (".$activityId.", '".$username."')";
                        
                        $db->query($query);
                        
                        $db->close();
                        
                        echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                      [ MESSAGE_TYPE::ACTIVITY_SUCCESSFULLY_ATTENDED, MESSAGE_DESC::ACTIVITY_SUCCESSFULLY_ATTENDED ]);
                    }
                    
                    
                    
                    
                }   
                else
                    if ($_GET["action"] == "quit")
                    {
                        if (!isset($_GET["activity_id"]))
                        {
                            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                          [ MESSAGE_TYPE::ACTIVITY_NO_DATA, MESSAGE_DESC::ACTIVITY_NO_DATA ]);
                            exit;
                        }
                        
                        $db = new database();
                        $db->connect();
                        
                        $username = user::getUserName();
                        $activityId = $db->escapeString($_GET["activity_id"]);
                        
                        $query = "DELETE FROM parties
                                  WHERE username = '".$username."' AND activity_id = ".$activityId;
                                  
                        $db->query($query);
                        
                        if ($db->getAffectedRows() <= 0)
                        {
                            // nu era inregistrat la acea activitate
                            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                          [ MESSAGE_TYPE::ACTIVITY_INVALID_QUIT, MESSAGE_DESC::ACTIVITY_INVALID_QUIT ]);
                            
                            $db->close();
                            exit;
                        }
                        else
                        {
                            $query = "UPDATE activities
                                      SET crt_members = crt_members - 1
                                      WHERE id = ".$activityId;
                            
                            $db->query($query);
                            
                            // sterg activitatile cu 0 membri - nu le mai modereaza nimeni.
                            $query = "DELETE FROM activities
                                      WHERE id = ".$activityId." AND crt_members = 0";
                                      
                            $db->query($query);
                            
                            
                            $db->close();
                            
                            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                          [ MESSAGE_TYPE::ACTIVITY_SUCCESSFULLY_QUITTED, MESSAGE_DESC::ACTIVITY_SUCCESSFULLY_QUITTED ]);
                            
                            
                            
                            
                        }
                        
                        
                    }
                    else
                        {                    
                            echo miscs::constructResponse([ JSON_CONSTS::MESSAGE_TYPE, JSON_CONSTS::MESSAGE_DESC ],
                                                          [ MESSAGE_TYPE::ACTIVITY_INVALID_ACTION, MESSAGE_DESC::ACTIVITY_INVALID_ACTION ]);
                            exit;
                        }


?>