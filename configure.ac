#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)
AC_INIT(blz3, 2.99.1, blz3-dev@morknet.de.blz3.tar.gz)
AC_CONFIG_SRCDIR([include/blz3/b3Types.h])
AC_CONFIG_HEADER([include_unix/blz3/autoconf.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_CHECK_TOOL(LD, ld, :)
AC_PROG_MAKE_SET
AC_CHECK_TOOL(AR, ar, :)
AC_PROG_RANLIB
AC_CHECK_TOOL(DOS2UNIX, dos2unix, RemCR)

AC_ARG_VAR(BLZ3_HOME)
AC_ARG_VAR(BLZ3_SYSINC)
AC_ARG_VAR(BLZ3_INCLUDE)
AC_ARG_VAR(BLZ3_LIB)
AC_ARG_VAR(BLZ3_BIN)

BLZ3_HOME="$PWD"
BLZ3_SYSINC="$BLZ3_HOME/include_unix"
BLZ3_INCLUDE="$BLZ3_HOME/include"
BLZ3_LIB="$BLZ3_HOME/lib"
BLZ3_BIN="$BLZ3_HOME/bin"

AC_PATH_XTRA

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC

AC_CHECK_HEADERS([fcntl.h float.h limits.h malloc.h memory.h stddef.h stdlib.h string.h strings.h sys/time.h sys/timeb.h unistd.h])

AC_CHECK_LIB([dl], [main])
AC_CHECK_LIB([pthread], [main])

AC_CHECK_HEADER([GL/glut.h],[AC_DEFINE([BLZ3_USE_OPENGL]) LIBS="$LIBS -lglut -lGLU -lGL"])
AC_CHECK_HEADER([Cg/cg.h],[AC_DEFINE([BLZ3_USE_CG]) LIBS="$LIBS -lCgGL"])
AC_CHECK_HEADER([avilib.h],[AC_DEFINE([BLZ3_USE_AVILIB]) LIBS="$LIBS -lavi"])
AC_CHECK_HEADER([encore2.h],[AC_DEFINE([BLZ3_USE_DIVX4LINUX]) LIBS="$LIBS -ldivxencore"])

CPPFLAGS="$CPPFLAGS -I$BLZ3_SYSINC -I$BLZ3_INCLUDE"
LDFLAGS="$LDFLAGS -L$BLZ3_LIB $X_LIBS"
ARFLAGS="rs"

AC_CHECK_LIB([X11], [main])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_MMAP
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([floor ftime gethostname gettimeofday memset mkdir munmap pow sqrt strchr strerror strstr])

AC_CONFIG_FILES([src/Makefile
	src/system_unix/Makefile
	src/system/Makefile
	src/Btiff/Makefile
	src/Bjpeg/Makefile
	src/base/Makefile
	src/image/Makefile
	src/raytrace/Makefile
	src/test_unix/Makefile
	src/OpenGL/Makefile
	src/brt3/Makefile
	src/tools/Makefile])
AC_OUTPUT
