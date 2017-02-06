/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_globals.h"

#include "php_uitox_db.h"
#include <openssl/des.h>
#include <mcrypt.h>
#include <syslog.h>
#include "clib.h"

// void clean_string(char *str);
// void TrimSpaceReturn(char *strStr);

/* If you declare any globals in php_uitox_db.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(uitox_db)
*/
ZEND_DECLARE_MODULE_GLOBALS(uitox_db)
static PHP_GINIT_FUNCTION(uitox_db);
static PHP_GSHUTDOWN_FUNCTION(uitox_db);
/* True global resources - no need for thread safety here */
static int le_uitox_db;

/* {{{ uitox_db_functions[]
 *
 * Every user visible function must have an entry in uitox_db_functions[].
 */
const zend_function_entry uitox_db_functions[] = {
	//PHP_FE(confirm_uitox_db_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(uitox_oracle_connect, NULL)
    PHP_FE(uitox_mysql_connect, NULL)
    PHP_FE(uitox_make_encrypt_file, NULL)
	#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION == 3 && PHP_RELEASE_VERSION >= 7) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4) || (PHP_MAJOR_VERSION > 5)
		PHP_FE_END
	#else
		{NULL,NULL,NULL}
	#endif
};
/* }}} */

/* {{{ uitox_db_module_entry
 */
zend_module_entry uitox_db_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
   // PHP_UITOX_DB_WORLD_EXTNAME,
	"uitox_db",
	uitox_db_functions,
	PHP_MINIT(uitox_db),
	PHP_MSHUTDOWN(uitox_db),
	PHP_RINIT(uitox_db),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(uitox_db),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(uitox_db),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_UITOX_DB_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_UITOX_DB
ZEND_GET_MODULE(uitox_db)
#endif


//取php.INI變數
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("uitox_db.encrytp_key"      , ENCRYPT_KEY,    PHP_INI_SYSTEM, OnUpdateString, encrytp_key,       zend_uitox_db_globals, uitox_db_globals)
    STD_PHP_INI_ENTRY("uitox_db.connect_file_path", ENCRYPT_DB_PATH,PHP_INI_SYSTEM, OnUpdateString, connect_file_path, zend_uitox_db_globals, uitox_db_globals)
PHP_INI_END()


/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("uitox_db.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_uitox_db_globals, uitox_db_globals)
    STD_PHP_INI_ENTRY("uitox_db.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_uitox_db_globals, uitox_db_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_uitox_db_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_uitox_db_init_globals(zend_uitox_db_globals *uitox_db_globals)
{
	uitox_db_globals->global_value = 0;
	uitox_db_globals->global_string = NULL;
}
*/
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(uitox_db)
{
  REGISTER_INI_ENTRIES();
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */

PHP_MSHUTDOWN_FUNCTION(uitox_db)
{
	// uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */


PHP_RINIT_FUNCTION(uitox_db)
{
  //REGISTER_INI_ENTRIES();
	return SUCCESS;

}

/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(uitox_db)
{
  //UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(uitox_db)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "uitox_db support", "enabled");

	#if PHP_MAJOR_VERSION < 7
	php_info_print_table_row(2, "Version", "1.0.4.php5");
	#else
	php_info_print_table_row(2, "Version", "1.0.4.php7");
	#endif

	// php_info_print_table_row(2, "Version", "1.0.4.php7");
	php_info_print_table_row(2,"builed date","2015/12/09 10:00");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_uitox_db_compiled(string arg)
   Return a string to confirm that the module is compiled in */
// PHP_FUNCTION(confirm_uitox_db_compiled)
// {
	// char *arg = NULL;
	// int arg_len, len;
	//
	// //char *strg;
	// zend_string *strg = zend_string_init("Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", sizeof("Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.")-1, 0);
	//
	// if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
	// 	return;
	// }
	//
	// //len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "uitox_db", arg);
	// RETURN_STR(strg);
// }


PHP_FUNCTION(uitox_oracle_connect)
{

#if PHP_MAJOR_VERSION >= 7

    char *tns_name, *user_name;

	size_t tns_name_len, user_name_len;
    char *connect_file_path = UITOX_DB_G(connect_file_path);
    char *encrytp_key       = UITOX_DB_G(encrytp_key);

    zval retval;
    zval function_params[4];
    zval function_name;
    Oracle_TNS  tns;

    ZVAL_STRING(&function_name, "oci_connect");

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ss",&tns_name, &tns_name_len, &user_name, &user_name_len)==FAILURE){
        zend_error(E_ERROR,"function requires args");
    }

    tns = get_ora_tns(tns_name, user_name, encrytp_key, connect_file_path);

    if(tns.user_name == NULL)
    {
    	zend_error(E_ERROR,"connect db file open error [%s]" , tns.file_path);
    }

    ZVAL_STRING(&function_params[0], tns.user_name);
 	ZVAL_STRING(&function_params[1], tns.password);
 	ZVAL_STRING(&function_params[2], tns.hostname);
 	ZVAL_STRING(&function_params[3], tns.char_set);


 	if(call_user_function(CG(function_table),
						NULL,
						&function_name,
						&retval,
						4,
						function_params TSRMLS_CC) == FAILURE )
	{
 		zend_error(E_ERROR,"call function failed");
 	}

    *return_value = retval;

    zval_copy_ctor(return_value);

    zval_ptr_dtor(&retval);
    zval_dtor(&function_name);

	free(tns.user_name);
	free(tns.password);
	free(tns.hostname);
	free(tns.char_set);

#else

	char *tns_name, *user_name;
	char *connect_file_path = UITOX_DB_G(connect_file_path);
	char *encrytp_key       = UITOX_DB_G(encrytp_key);

	zval* input_tns_name;
	zval* input_user_name;

	zval* retval;
	zval* function_params[4];
	zval function_name;
	Oracle_TNS  tns;

	ZVAL_STRING(&function_name, "oci_connect", 1);


	MAKE_STD_ZVAL(retval);

	input_user_name = (zval*)malloc(sizeof(zval));
	input_user_name = (zval*)malloc(sizeof(zval));

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zz",&input_tns_name, &input_user_name)==FAILURE){
	  zend_error(E_ERROR,"function requires args");
	}

	// convert_to_string(args);
	tns_name  = Z_STRVAL_P(input_tns_name);
	user_name = Z_STRVAL_P(input_user_name);

	tns = get_ora_tns(tns_name, user_name, encrytp_key, connect_file_path);

	if(tns.user_name == NULL)
	{
	  zend_error(E_ERROR,"connect db file open error [%s]" , tns.file_path);
	}

	ALLOC_INIT_ZVAL(function_params[0]);
	ZVAL_STRING(function_params[0], tns.user_name, 1);

	ALLOC_INIT_ZVAL(function_params[1])
	ZVAL_STRING(function_params[1], tns.password, 1);

	ALLOC_INIT_ZVAL(function_params[2])
	ZVAL_STRING(function_params[2], tns.hostname, 1);

	ALLOC_INIT_ZVAL(function_params[3])
	ZVAL_STRING(function_params[3], tns.char_set, 1);


	if(call_user_function(CG(function_table), NULL, &function_name, retval, 4, function_params TSRMLS_DC)==FAILURE){
	  zend_error(E_ERROR,"call function failed");
	}

	*return_value=*retval;

	zval_copy_ctor(return_value);

	zval_ptr_dtor(&retval);
	zval_dtor(&function_name);

	zval_ptr_dtor(&function_params[0]);
	zval_ptr_dtor(&function_params[1]);
	zval_ptr_dtor(&function_params[2]);
	zval_ptr_dtor(&function_params[3]);

	free(tns.user_name);
	free(tns.password);
	free(tns.hostname);
	free(tns.char_set);

#endif

}





PHP_FUNCTION(uitox_mysql_connect)
{
#if PHP_MAJOR_VERSION >= 7
    char *database_name, *user_name;
	size_t database_name_len, user_name_len;
    char *connect_file_path = UITOX_DB_G(connect_file_path);
    char *encrytp_key = UITOX_DB_G(encrytp_key);

    zval retval;
    zval function_params[4];
    zval function_name;

    Oracle_TNS  tns;

    ZVAL_STRING(&function_name, "mysqli_connect");

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ss",&database_name, &database_name_len, &user_name, &user_name_len)==FAILURE){
        zend_error(E_ERROR,"function requires args");
    }


    tns = get_ora_tns(database_name, user_name, encrytp_key, connect_file_path);


    if(tns.user_name == NULL)
	{
    	zend_error(E_ERROR,"connect db file open error [%s]" , tns.file_path);
    }

    //host name
    ZVAL_STRING(&function_params[0], tns.hostname);
    //user name
    ZVAL_STRING(&function_params[1], tns.user_name);
    //password
    ZVAL_STRING(&function_params[2], tns.password);
    //database
    ZVAL_STRING(&function_params[3], database_name);

	if(call_user_function(CG(function_table),
					   NULL,
					   &function_name,
					   &retval,
					   4,
					   function_params TSRMLS_CC) == FAILURE )
   {
	   zend_error(E_ERROR,"call function failed");
   }
    *return_value = retval;
    zval_copy_ctor(return_value);

	zval_ptr_dtor(&retval);
    zval_dtor(&function_name);

#else

	char *database_name, *user_name;

	char *connect_file_path = UITOX_DB_G(connect_file_path);
	char *encrytp_key = UITOX_DB_G(encrytp_key);

	zval* input_database_name;
	zval* input_user_name;

	zval* retval;
	zval* function_params[4];
	zval function_name;

	Oracle_TNS  tns;

	ZVAL_STRING(&function_name, "mysqli_connect", 1);

	MAKE_STD_ZVAL(retval);
	input_database_name=(zval*)malloc(sizeof(zval));
	input_user_name=(zval*)malloc(sizeof(zval));

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zz",&input_database_name, &input_user_name)==FAILURE){
		zend_error(E_ERROR,"function requires args");
	}

	// convert_to_string(args);
	database_name  =  Z_STRVAL_P(input_database_name);
	user_name      =  Z_STRVAL_P(input_user_name);

	tns = get_ora_tns(database_name, user_name, encrytp_key, connect_file_path);


	if(tns.user_name == NULL)
	{
		zend_error(E_ERROR,"connect db file open error [%s]" , tns.file_path);
	}

	//host name
	MAKE_STD_ZVAL(function_params[0]);
	ZVAL_STRING(function_params[0], tns.hostname, 1);

	//user name
	MAKE_STD_ZVAL(function_params[1]);
	ZVAL_STRING(function_params[1], tns.user_name, 1);

	//password
	MAKE_STD_ZVAL(function_params[2]);
	ZVAL_STRING(function_params[2], tns.password, 1);

	//database
	MAKE_STD_ZVAL(function_params[3]);
	ZVAL_STRING(function_params[3], database_name, 1);


	if(call_user_function(CG(function_table), NULL, &function_name,retval, 4, function_params TSRMLS_DC)==FAILURE){
		zend_error(E_ERROR,"call function failed");
	}

	*return_value=*retval;
	zval_copy_ctor(return_value);
	zval_ptr_dtor(&retval);
	zval_dtor(&function_name);

#endif

}

PHP_FUNCTION(uitox_make_encrypt_file)
{
#if PHP_MAJOR_VERSION >= 7
    char *connect_file_path = UITOX_DB_G(connect_file_path);
    char *encrytp_key =UITOX_DB_G(encrytp_key);
    char* IV  = "AAAAAAAAAAAAAAAA";
    char z_input_file_path;
    char z_output_file_path;
	size_t z_input_file_path_len, z_output_file_path_len;

    char * input_file_path;
    char * output_file_path;
    //char* user_name;
    // zval* retval;
    zval* function_params[4];
    // zval function_name;
    //ZVAL_STRING(&function_name, "mysqli_connect", 1);
    // ZVAL_STRING(&function_name, "mysqli_connect", 1);


	// char *key = "0123456789abcdef";
    int keysize = 16; /* 128 bits */
    char* buffer;
    int buffer_len = 256;

    // char* buffer2;

    char * plaintext = calloc(256, sizeof(char));
	//
    // z_input_file_path=(zval*)malloc(sizeof(zval));
    // z_output_file_path=(zval*)malloc(sizeof(zval));
    // MAKE_STD_ZVAL(retval);
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ss",&z_input_file_path, &z_input_file_path_len, &z_output_file_path, &z_output_file_path_len)==FAILURE){
        zend_error(E_ERROR,"function requires args");
    }

    input_file_path  = z_input_file_path;
    output_file_path = z_output_file_path;

    plaintext = ReadFile(input_file_path);


    //去空白
    TrimSpaceReturn(plaintext);


    buffer = calloc(1, buffer_len);


    strncpy(buffer, plaintext, buffer_len);

   // printf("plain: %s\n", plaintext);

    //display(buffer, buffer_len);

    encrypt_aes(buffer, buffer_len, IV, encrytp_key, keysize);

    WriteFile(output_file_path, buffer, buffer_len);

    RETURN_TRUE;

#else


    //Oracle_TNS ora_conf[100];
   // char *database_name, *user_name;

    char *connect_file_path = UITOX_DB_G(connect_file_path);
    char *encrytp_key =UITOX_DB_G(encrytp_key);
    char* IV  = "AAAAAAAAAAAAAAAA";
    zval* z_input_file_path;
    zval* z_output_file_path;

    char * input_file_path;
    char * output_file_path;
    //char* user_name;
    // zval* retval;
    zval* function_params[4];
    // zval function_name;
    //ZVAL_STRING(&function_name, "mysqli_connect", 1);
    // ZVAL_STRING(&function_name, "mysqli_connect", 1);


	// char *key = "0123456789abcdef";
    int keysize = 16; /* 128 bits */
    char* buffer;
    int buffer_len = 256;

    // char* buffer2;

    char * plaintext = calloc(256, sizeof(char));

    z_input_file_path=(zval*)malloc(sizeof(zval));
    z_output_file_path=(zval*)malloc(sizeof(zval));
    // MAKE_STD_ZVAL(retval);
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zz",&z_input_file_path, &z_output_file_path)==FAILURE){
        zend_error(E_ERROR,"function requires args");
    }

    input_file_path  = Z_STRVAL_P(z_input_file_path);
    output_file_path = Z_STRVAL_P(z_output_file_path);

    plaintext = ReadFile(input_file_path);


    //去空白
    TrimSpaceReturn(plaintext);


    buffer = calloc(1, buffer_len);


    strncpy(buffer, plaintext, buffer_len);

   // printf("plain: %s\n", plaintext);

    //display(buffer, buffer_len);

    encrypt_aes(buffer, buffer_len, IV, encrytp_key, keysize);

    WriteFile(output_file_path, buffer, buffer_len);

    RETURN_TRUE;

#endif

}



/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
