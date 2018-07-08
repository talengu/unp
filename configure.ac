#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([unp], [0.1], [niexiaowen@uestc.edu.cn])
AC_CONFIG_SRCDIR([])
AM_INIT_AUTOMAKE(unp, 0.1)
AC_CONFIG_HEADERS([config.h])
# AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
LT_INIT
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_ARG_ENABLE(debug, [  --enable-debug	Enable DEBUG output. ],
	[ CXXFLAGS="-g2 -O0 -DDEBUG -Wall -Werror" ],
	[ CXXFLAGS="-O3 -Wall -Werror" ])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_OUTPUT
