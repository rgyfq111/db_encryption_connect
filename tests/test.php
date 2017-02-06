<?php

//if(function_exists('mysqli_get_client_stats'))
//{
//	echo 'is use mysqlnd';
//}
//else
//{
//	echo 'no mysqlnd';
//}


//exit();
$array = array('1','2');

for($i = 0; $i <= 10; $i++)
{
//	sleep(1);
//	echo time() % count($array);
}

//exit();
//for($i = 0; $i <= 1000; $i++)
//{
//	$conn  = uitox_oracle_connect('AORD','UAORDWEB');

	//$conn  = uitox_oracle_connect('FORD','ufuvendor_api', 'uitox_debug_test_key');
	//$conn  = uitox_oracle_connect('GORD','uguvendor_api', 'uitox_debug_test_key');

	//$conn  = uitox_oracle_connect('GORD','ugudownload_api','uitox_debug_test_key');
for($i=0; $i <= 10; $i++)
{
//TEST_AORD_system
//continue;
	$conn  = uitox_oracle_connect('TEST_AORD','system');
//sleep(5);
//echo get_usage_in_kb() . " run.!\n";
//continue;
//sleep(1000);
if (!$conn) {
      $e = oci_error();
              trigger_error(htmlentities($e['message'], ENT_QUOTES), E_USER_ERROR);
               exit('connect fail');
}
$sql = 'SELECT TRUNC (15.79,1) "Truncate" FROM DUAL';
$stid = oci_parse($conn, $sql);
$oe = oci_execute($stid);

while ($row = oci_fetch_array($stid, OCI_ASSOC+OCI_RETURN_NULLS)) {
                   var_dump($row);
}

oci_free_statement($stid);
//oci_free_statement($oe);
oci_close($conn);



echo "\n";


        $conn  = uitox_oracle_connect('TEST_AORD','system');

if (!$conn) {
      $e = oci_error();
              trigger_error(htmlentities($e['message'], ENT_QUOTES), E_USER_ERROR);
               exit('connect fail');
}
$sql = 'SELECT TRUNC (15.79,1) "Truncate" FROM DUAL';
$stid = oci_parse($conn, $sql);
$oe = oci_execute($stid);

while ($row = oci_fetch_array($stid, OCI_ASSOC+OCI_RETURN_NULLS)) {
                   var_dump($row);
}

oci_free_statement($stid);

oci_close($conn);



        $conn  = uitox_oracle_connect('TEST_AORD','system');

if (!$conn) {
      $e = oci_error();
              trigger_error(htmlentities($e['message'], ENT_QUOTES), E_USER_ERROR);
               exit('connect fail');
}
$sql = 'SELECT TRUNC (15.79,1) "Truncate" FROM DUAL';
$stid = oci_parse($conn, $sql);
$oe = oci_execute($stid);

while ($row = oci_fetch_array($stid, OCI_ASSOC+OCI_RETURN_NULLS)) {
                   var_dump($row);
}

oci_free_statement($stid);

oci_close($conn);

//print_r($conn1);
	//$conn  = uitox_oracle_connect('FSTK','ufustock_api');
//	$conn2  = uitox_oracle_connect('AORD','uaustock_api');
}
/*	
        if($conn2 = oci_connect('i','2','xxxxx'))
	{
		echo "Successfully\n";
	}
	else
	{
		echo "testse";
		print_r(OCIError(), true);

	}
*/

echo "testestest";
//exit();
	//$conn2  = uitox_oracle_connect('FRSTK','ufustock_api');
	//$conn2  = uitox_oracle_connect('FSTK','ufustock_api');
//	$conn3  = uitox_oracle_connect('FSTK','ufustock_api');

//}
//sleep(1);
exit();
	//$conn  = uitox_oracle_connect('AORD','uauvendor_api', 'uitox_debug_test_key');
//	$conn  = oci_connect('MARS', 'rgyfqmwx', 'AORD', 'utf8');
//GORD_uguvendor_api.enc

//MARS, password rgyfqmwx, hostname AORD, char_set utf8"
	$stid = oci_parse($conn, "select * from EMP where ROWNUM=1");

	oci_execute($stid);

	//echo "<pre>";
	while( ($row = oci_fetch_array($stid, OCI_ASSOC+OCI_RETURN_NULLS)) != false)
	{
	        print_r($row);
	}
	
	oci_free_statement($stid);
	oci_close($conn);
//}
//exit();





function get_usage_in_kb(){
    echo memory_get_usage()/1024.0 . " kb \n";
}


?>
