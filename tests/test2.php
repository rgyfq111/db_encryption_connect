<?php

$tns= "TEST_AORD2=(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=dbaord01.dev1.ux)(PORT = 1521)))(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME=AORD)))";
$conn2 = oci_connect('system','rgyfqmwx','TEST_AORD','utf8');

print_r($conn2);
//$conn2 = oci_connect('system','rgyfqmwx',$tns,'utf8');
//$conn2 = oci_connect('system','rgyfqmwx',$tns,'utf8');
//$conn2 = oci_connect('system','rgyfqmwx',$tns,'utf8');


/*$conn2 = oci_connect('i','2','xxxxx');
$conn2 = oci_connect('i','2','xxxxx');
$conn2 = oci_connect('i','2','xxxxx');
$conn2 = oci_connect('i','2','xxxxx');
$conn2 = oci_connect('i','2','xxxxx');
$conn2 = oci_connect('i','2','xxxxx');
$conn2 = oci_connect('i','2','xxxxx');
*/

?>
