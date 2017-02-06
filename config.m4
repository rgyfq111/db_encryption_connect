dnl $Id$
dnl config.m4 for extension uitox_db

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(uitox_db, for uitox_db support,
dnl Make sure that the comment is aligned:
dnl [  --with-uitox_db             Include uitox_db support])

dnl Otherwise use enable:

dnl PHP_ADD_LIBRARY_WITH_PATH(z, $PHP_ZLIB_DIR/$PHP_LIBDIR, ZIP_SHARED_LIBADD)

PHP_ARG_ENABLE(uitox_db, whether to enable uitox_db support,
dnl Make sure that the comment is aligned:
[  --enable-uitox_db           Enable uitox_db support])

if test "$PHP_UITOX_DB" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-uitox_db -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/uitox_db.h"  # you most likely want to change this
  dnl if test -r $PHP_UITOX_DB/$SEARCH_FOR; then # path given as parameter
  dnl   UITOX_DB_DIR=$PHP_UITOX_DB
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for uitox_db files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       UITOX_DB_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$UITOX_DB_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the uitox_db distribution])
  dnl fi

  dnl # --with-uitox_db -> add include path
  dnl PHP_ADD_INCLUDE($UITOX_DB_DIR/include)

  dnl # --with-uitox_db -> check for lib and symbol presence
  dnl LIBNAME=uitox_db # you may want to change this
  dnl LIBSYMBOL=uitox_db # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $UITOX_DB_DIR/lib, UITOX_DB_SHARED_LIBADD)
   AC_DEFINE(HAVE_UITOX_DBLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong uitox_db lib version or lib not found])
  dnl ],[
  dnl   -L$UITOX_DB_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(UITOX_DB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(uitox_db, uitox_db.c clib.c,$ext_shared)
fi
