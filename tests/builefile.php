<?php

for($i = 0; $i <= 1; $i++)
{
	$rs = uitox_make_encrypt_file('/tmp/conn_db/AORD_UAORDWEB.conf', '/home/php-5.5.11/ext/uitox_db/tests/db_file/AORD_UAORDWEB.enc');
	if($rs)
	{
		echo "ok!\n";
	}
}
?>
