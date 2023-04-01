<?php

if(isset($_GET["temperature"]) && isset($_GET["humidity"]) && isset($_GET["WaterTemp"]) && isset($_GET["ldr"]) && isset($_GET["co2"])) {

   $temperature = $_GET["temperature"]; 
   $humidity = $_GET["humidity"];
   $WaterTemp = $_GET["WaterTemp"]; 
   $ldr = $_GET["ldr"]; 
   $co2 = $_GET["co2"]; 


   $servername = "localhost";
   $username = "esp32";
   $password = "microcontrollerslab@123";
   $database_name = "database_ESP32";

   // Create MySQL connection fom PHP to MySQL server
   $connection = new mysqli($servername, $username, $password, $database_name);
   // Check connection
   if ($connection->connect_error) {
      die("MySQL connection failed: " . $connection->connect_error);
   }

   $sql = "INSERT INTO greenhouse (temp,humidity,co2,water_temp,ligh) VALUES ($temperature,$humidity,$co2,$WaterTemp,$ldr)";

   if ($connection->query($sql) === TRUE) {
      echo "New record created successfully";
   } else {
      echo "Error: " . $sql . " => " . $connection->error;
   }

   $connection->close();
} else {
   echo "temperature is not set in the HTTP request";
}
?>