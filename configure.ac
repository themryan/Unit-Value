#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([uvalue], [0.1.0], [themryan@pacbell.net])
AM_INIT_AUTOMAKE([uvalue], [0.1.0])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_PROG_RANLIB()

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_STRTOD
AC_CHECK_FUNCS([pow strchr])

AC_CONFIG_FILES([Makefile src/Makefile examples/Makefile tests/Makefile])
AC_OUTPUT

#LT_PREREQ([2.4.2])
#LT_INIT()
#AC_PROG_LIBTOOL
#AM_PROG_LIBTOOL
#LT_OUTPUT
