<?php
    session_start();
    
    // doar ca sa nu pun referinta direct catre poza din folder si sa gaseasca astia toate pozele
    
    require_once "includes/user.php";
    
    if (user::isLoggedIn() && isset($_GET["img"]))
    {
        $imgPath = "imgs/".$_GET["img"];
        
        
        header("Content-type: image/png");
        header('Expires: 0');
        header('Content-Length: ' . filesize($imgPath)); // meh...trebuie filtrate
        
        readfile($imgPath);
    }
    else
        die("lel");
    


?>