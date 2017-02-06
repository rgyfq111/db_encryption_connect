<?php

$db  = uitox_mysql_connect('mysql','root');

$sql = <<<SQL
    SELECT *
    FROM `user`
    WHERE 1
SQL;

if(!$result = $db->query($sql)){
    die('There was an error running the query [' . $db->error . ']');
}

while($row = $result->fetch_assoc()){
    print_r($row);;
}

//print_r($db);

?>
