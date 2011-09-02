#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.36])
AC_INIT([needlet_decomp], [1.0], [maurizio.tomasi@unimi.it])

# We use `foreign' because we do not provide a ChangeLog file
# -- it will be created automatically every time you run
# "make dist" to produce a tarball.
AM_INIT_AUTOMAKE([foreign -Wall])
AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADERS([config.h])

AC_ARG_WITH(
	[openmp],
	[AS_HELP_STRING(
		[--with-openmp],
		[link the program with the OpenMP libraries (use this if you built Healpix with OpenMP support) @<:@default=no@:>@])],
	[],
	[use_openmp=no])

AC_ARG_VAR(NOWEAVE)
AC_ARG_VAR(NOTANGLE)
AC_ARG_VAR(CPIF) 

# Checks for programs.
AC_PROG_CXX

AC_PATH_PROG(NOWEAVE, [noweave])
AC_PATH_PROG(NOTANGLE, [notangle])
AC_PATH_PROG(CPIF, [cpif])
AC_PATH_PROG(TEXI2PDF, [texi2pdf])
AC_PATH_PROG(ASY, [asy])

# Checks for libraries.

AC_LIBOBJ(asprintf)

AC_SUBST([GSL_CFLAGS])
AC_SUBST([GSL_LIBS])

AC_CHECK_LIB([m],
             [sin],
	     [],
	     [AC_MSG_ERROR([unable to find the standard C math library])])

AC_CHECK_LIB(gslcblas,main)
AC_CHECK_LIB(gsl,main)

AC_CHECK_LIB([cfitsio],
             [ffpcl],
	     [],
	     [AC_MSG_ERROR([unable to find the CFITSIO library, have you installed it?])])

if [[ "x$HEALPIX" == "x" ]]; then
    AC_MSG_ERROR([environment variable [$HEALPIX] undefined, have you installed Healpix?])
fi

if [[ "x$HEALPIX_TARGET" == "x" ]]; then
    AC_MSG_ERROR([environment variable [$HEALPIX_TARGET] undefined, have you installed Healpix?])
fi

CPPFLAGS="-I$HEALPIX/src/cxx/$HEALPIX_TARGET/include"
LDFLAGS="-L$HEALPIX/src/cxx/$HEALPIX_TARGET/lib"

# All the following libraries are part of the Healpix 2.20a+ bundle

# We must enable OpenMP support before checking for Healpix libraries
if [[ "x$with_openmp" == "xyes" ]]; then
   CFLAGS="$CFLAGS -fopenmp"
   CXXFLAGS="$CXXFLAGS -fopenmp"
   LDFLAGS="$LDFLAGS -fopenmp"
fi

AC_CHECK_LIB(
	[c_utils],
	[wallTime],
        [],
        [AC_MSG_ERROR([unable to link one of Healpix_cxx's dependencies (c_utils)])])
AC_CHECK_LIB(
	[fftpack],
	[cfftf],
        [],
        [AC_MSG_ERROR([unable to link one of Healpix_cxx's dependencies (libfftpack)])])
AC_CHECK_LIB(
	[psht],
	[pshtd_execute_jobs],
        [],
        [AC_MSG_ERROR([unable to link one of Healpix_cxx's dependencies (libpsht)])])

AC_LANG(C++)
LIBS="$LIBS -lhealpix_cxx -lcxxsupport"
AC_TRY_LINK(
	[#include <healpix_base.h>],
	[string2HealpixScheme("RING")],
	,
	[AC_MSG_ERROR([unable to link Healpix_cxx -- have you compiled the C++ library?])])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
