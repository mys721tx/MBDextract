AC_INIT([MBDextract],[0.1],[mys721tx@gmail.com],[MBDextract],[https://github.com/mys721tx/MBDextract])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC

AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([string.h]) 
AC_CHECK_HEADERS([errno.h])

AC_CHECK_FUNCS([strerror])

AC_FUNC_MALLOC

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT

