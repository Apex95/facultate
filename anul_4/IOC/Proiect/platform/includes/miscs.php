<?php

    require_once "messages.php";

    
    class miscs
    {
        const SITE_ADDRESS = "/";
        
        static function constructResponse($jsonArgs, $jsonValues)
        {
            $len = count($jsonArgs);
            
            if ($len != count($jsonValues))
                die("constructResponse() - jsonArgs & jsonValues must have the same length");

            
            $response = [];
            
            // encodeaza cu UTF-8 tot ce e string (json_encode nu suporta diacritice fara encodare)
            for ($i = 0; $i < $len; $i++) 
                $response[$jsonArgs[$i]] = is_string($jsonValues[$i]) ? utf8_encode($jsonValues[$i]) : $jsonValues[$i];

            
            return json_encode($response);
        }
        
        static function generateImageUrl($imagePath)
        {
            return miscs::SITE_ADDRESS."viewer.php?img=".$imagePath;
        }
    }

?>