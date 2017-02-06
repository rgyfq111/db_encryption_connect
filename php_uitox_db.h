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

#ifndef PHP_UITOX_DB_H
#define PHP_UITOX_DB_H

extern zend_module_entry uitox_db_module_entry;
#define phpext_uitox_db_ptr &uitox_db_module_entry

#define PHP_UITOX_DB_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_UITOX_DB_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_UITOX_DB_API __attribute__ ((visibility("default")))
#else
#	define PHP_UITOX_DB_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "php_globals.h"

PHP_MINIT_FUNCTION(uitox_db);
PHP_MSHUTDOWN_FUNCTION(uitox_db);
PHP_RINIT_FUNCTION(uitox_db);
PHP_RSHUTDOWN_FUNCTION(uitox_db);
PHP_MINFO_FUNCTION(uitox_db);

PHP_FUNCTION(confirm_uitox_db_compiled);	/* For testing, remove later. */


//連線oracle
PHP_FUNCTION(uitox_oracle_connect);

//連線mysql
PHP_FUNCTION(uitox_mysql_connect);

//產生加密連線檔案
PHP_FUNCTION(uitox_make_encrypt_file);

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:
*/

ZEND_BEGIN_MODULE_GLOBALS(uitox_db)
  char * encrytp_key;
  char * connect_file_path;
ZEND_END_MODULE_GLOBALS(uitox_db)


/* In every utility function you add that needs to use variables
   in php_uitox_db_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as UITOX_DB_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define UITOX_DB_G(v) TSRMG(uitox_db_globals_id, zend_uitox_db_globals *, v)
#else
#define UITOX_DB_G(v) (uitox_db_globals.v)
#endif

#endif	/* PHP_UITOX_DB_H */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/des.h>
#include <regex.h>
#include <assert.h>

#include "lib/encrytp.h"
#include "lib/rwfile.h"

#define ENCRYPT_KEY "uitox123456"

#define ENCRYPT_DB_PATH "/tmp"

// typedef struct Oracle_TNS
// {
  // char *user_name;
  // char *password;
  // char *hostname;
  // char *char_set;
// }Oracle_TNS;


// char * get_server_name();
// char * get_host_name();
// char * get_server_environment();
// char ** split_string(char * string, char * seps);

// char** str_split(char* a_str, const char a_delim);
// char** str_split( char* str, char delim, int* numSplits );

// Oracle_TNS  get_ora_tns(char* tns_name, char* user_name);

// char * replace(
   // char const * const original,
    // char const * const pattern,
   // char const * const replacement
// );


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
