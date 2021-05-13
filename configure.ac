#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT(blz3, 3.1.4, blz3-dev@morknet.de, blz3.tar.gz)
AC_CONFIG_SRCDIR([include/blz3/b3Types.h])
AC_CONFIG_HEADER([include_unix/blz3/autoconf.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_CC
AC_CHECK_TOOL(LD, ld, :)
AC_PROG_MAKE_SET
AC_CHECK_TOOL(AR, ar, :)
AC_PROG_RANLIB
AC_CHECK_TOOL([DOS2UNIX], [dos2unix], [echo >/dev/null])
AC_CHECK_TOOL([QHELPGEN], [qhelpgenerator])

# FIXME!
#if test "$LD" = "ld"
#then
#  LDFLAGS="-Ur -Wl,-E"
#fi

BUILD=${BUILD_NUMBER:-0}

AC_SUBST(MFLAGS)
AC_SUBST(STDFLAGS)
AC_SUBST(BLZ3_IS_LINUX)
AC_SUBST(BLZ3_USE_CG)
AC_SUBST(BLZ3_USE_GLUT)
AC_SUBST(BLZ3_USE_OPENGL)
AC_SUBST(BLZ3_USE_SSE)
AC_SUBST(BLZ3_USE_SSE2)
AC_SUBST(BLZ3_USE_SSE3)
AC_SUBST(BLZ3_USE_X264)
AC_SUBST(HAVE_LIBCPPUNIT)
AC_SUBST(HAVE_SSE)
AC_SUBST(HAVE_SSE2)
AC_SUBST(HAVE_SSE3)
AC_SUBST(HAVE_X264_DELAYED_FRAMES)
AC_SUBST(HAVE_X264_PARAMETER_DEFINITION)
AC_SUBST(SSE_ALIGNED)
AC_SUBST(BLZ3_HOME, "$PWD", [This is the Blizzard III home directory])
AC_SUBST(BLZ3_SYSINC, "$BLZ3_HOME/include_unix", [The Blizzard III system dependend include path])
AC_SUBST(BLZ3_INCLUDE, "$BLZ3_HOME/include", [The Blizzard III system independend include path])
AC_SUBST(BLZ3_LIB, "$BLZ3_HOME/lib", [The Blizzard III library directory])
AC_SUBST(BLZ3_BIN, "$BLZ3_HOME/bin", [The Blizzard III executable directory])
AC_SUBST(BLZ3_DOC, "$BLZ3_DOC", [The Blizzard III documentation directory])
AC_SUBST(GLUTLIBS)
AC_SUBST(X264LIBS)
AC_SUBST(CPPUNITLIBS)
AC_SUBST(BUILD, "$BUILD", [The Jenkins build number])

AC_LANG_PUSH(C++)

AX_CXX_COMPILE_STDCXX_17([ext], [mandatory])
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
AX_CHECK_COMPILE_FLAG([-Wextra], [CXXFLAGS="$CXXFLAGS -Wextra"])
AX_CHECK_COMPILE_FLAG([-Wshadow], [CXXFLAGS="$CXXFLAGS -Wshadow"])
AX_CHECK_COMPILE_FLAG([-Wsuggest-override], [CXXFLAGS="$CXXFLAGS -Wsuggest-override"])
AX_CHECK_COMPILE_FLAG([-march=native], [MFLAGS="$MFLAGS -march=native"])
AX_CHECK_COMPILE_FLAG([-ftree-vectorize], [MFLAGS="$MFLAGS -ftree-vectorize"])
AX_CHECK_COMPILE_FLAG([-std=gnu++1z], [STDFLAGS="$STDFLAGS -std=gnu++1z"])
CFLAGS="$CFLAGS $MFLAGS"
CXXFLAGS="$CXXFLAGS $MFLAGS"
# CPPFLAGS="$CPPFLAGS $STDFLAGS"

AC_CHECK_FUNCS([floor ceil ftime gethostname gettimeofday memset mkdir munmap pow sqrt cbrt strchr strerror strstr])

AC_MSG_CHECKING([wether we compile on Linux])
AC_TRY_LINK([#include <stdio.h>],[
#ifndef __linux__
#error "This is not Linux!"
#endif
],[AC_DEFINE([BLZ3_IS_LINUX], [1], [Is Blizzard running on Linux]) AC_MSG_RESULT(yes)],AC_MSG_RESULT(no))

AC_CHECK_HEADER([xmmintrin.h],[
	AC_DEFINE([HAVE_SSE], [1], [Is SSE available?])
	AC_MSG_CHECKING([for SSE])
	AC_RUN_IFELSE([AC_LANG_PROGRAM([#include <stdio.h>
#include <xmmintrin.h>
__m128 a;
__m128 b,r;],[
char result[[128]];
a = _mm_set_ps1(1);
b = _mm_set_ps1(1);
r = _mm_add_ps(a,b);
// printf ("SSE1 %p %p\n", &a, &b);
sprintf(result,"%f\n",*((float *)&r));
	])],
	[AC_DEFINE([BLZ3_USE_SSE], [1], [Does Blizzard use SSE?]) AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)])
])

AC_CHECK_HEADER([emmintrin.h],[
	AC_DEFINE([HAVE_SSE2], [2], [Is SSE2 available?])
	AC_MSG_CHECKING([for SSE2])
	AC_RUN_IFELSE([AC_LANG_PROGRAM([#include <stdio.h>
#include <emmintrin.h>
__m128d a;
__m128d b,r;],[
char result[[128]];
a = _mm_set1_pd(1);
b = _mm_set1_pd(1);
r = _mm_add_pd(a,b);
// printf ("SSE2 %p %p\n", &a, &b);
sprintf(result,"%lf\n",*((double *)&r));
	])],
	[AC_DEFINE([BLZ3_USE_SSE2], [2], [Does Blizzard use SSE2?]) AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)])
])

AC_CHECK_HEADER([pmmintrin.h],[
	AC_DEFINE([HAVE_SSE3], [3], [Is SSE3 available?])
	AC_MSG_CHECKING([for SSE3])
	AC_RUN_IFELSE([AC_LANG_PROGRAM([#include <stdio.h>
#include <emmintrin.h>
#include <pmmintrin.h>
__m128d a;
__m128d b,r;],[
char result[[128]];
a = _mm_set1_pd(1);
b = _mm_set1_pd(1);
r = _mm_hadd_pd(a,b);
// printf ("SSE3 %p %p\n", &a, &b);
sprintf(result,"%lf\n",*((double *)&r));
	])],
	[AC_DEFINE([BLZ3_USE_SSE3], [3], [Does Blizzard use SSE3?]) AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)])
])

AC_CHECK_HEADER([tmmintrin.h],[
	AC_DEFINE([HAVE_SSSE3], [3], [Is SSSE3 available?])
	AC_MSG_CHECKING([for SSSE3])
	AC_RUN_IFELSE([AC_LANG_PROGRAM([#include <stdio.h>
#include <tmmintrin.h>
__m128i input   = _mm_set1_epi32(0x19121968);
__m128i shuffle = _mm_set_epi8(
					0x80, 0x80, 0x80, 0x00,
					0x80, 0x80, 0x80, 0x01,
					0x80, 0x80, 0x80, 0x02,
					0x80, 0x80, 0x80, 0x03);
__m128i sse = _mm_shuffle_epi8(input, shuffle);
	])],
	[AC_DEFINE([BLZ3_USE_SSSE3], [3], [Does Blizzard use SSSE3?]) AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)])
])

AC_CHECK_HEADER([xmmintrin.h],[
	AC_MSG_CHECKING([for 16 byte alignment])
	AC_RUN_IFELSE([AC_LANG_PROGRAM([#include <stdio.h>
#include <xmmintrin.h>
unsigned short pad;
class alignment_test
{
public:
__m128 a;
unsigned short q;
__m128 b;
unsigned short r;
};
__m128 r;],[
char result[[128]];
alignment_test *str = new alignment_test;
#if 0
printf ("<SSE pointer %p>", str);
printf ("<SSE member %p - %p %p %p>", str, &str->a, &str->b, &str->q);
#endif

str->a = _mm_set_ps1(1);
str->b = _mm_set_ps1(1);
r = _mm_add_ps(str->a, str->b);

sprintf(result,"%f %d\n",*((float *)&r), str->q);
	])],
	[AC_DEFINE([SSE_ALIGNED], [1], [The compilation is SSE memory aligned]) AC_MSG_RESULT(yes)],
	[AC_MSG_RESULT(no)])
])

AC_MSG_CHECKING([for memalign])
AC_TRY_LINK([#include <malloc.h>],[
memalign(16,10240);
],[AC_DEFINE([HAVE_MEMALIGN], [1], [Is memalign() available?]) AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)])

AC_PATH_XTRA

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC

AC_CHECK_HEADERS([fcntl.h float.h limits.h malloc.h memory.h stddef.h stdlib.h string.h strings.h sys/time.h sys/timeb.h unistd.h filesystem])


# Check for OpenGL
AC_CHECK_HEADER([GL/glut.h],
	[
		AC_DEFINE([BLZ3_USE_GLUT], [1], [Do we can use GLUT?])
		GLUTLIBS="$GLUTLIBS -lglut"
		AC_CHECK_LIB([Xmu], [main])
	])
AC_CHECK_HEADER([GL/glu.h],[AC_DEFINE([BLZ3_USE_OPENGL], [1], [Do we can use GLU?]) LIBS="$LIBS -lGLU -lGL"])

# Check for nvidia Cg
AC_CHECK_HEADER([Cg/cg.h],[AC_DEFINE([BLZ3_USE_CG], [1], [Do we have Cg header?])])
AC_CHECK_LIB([CgGL], [main])

# Check for OpenEXR
AC_ARG_WITH(openexr, [AC_HELP_STRING([--with-openexr],[OpenEXR include path])], OPENEXR=$withval)
if test "x$OPENEXR" = "x"; then
  OPENEXR=/usr/include/OpenEXR
fi

save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I$OPENEXR"
AC_CHECK_HEADER(ImfRgbaFile.h, [AC_DEFINE([BLZ3_USE_OPENEXR], [1], [Can we use OpenEXR?])], NOT_FOUND="$NOT_FOUND OpenEXR" CPPFLAGS="$save_CPPFLAGS")
# END OpenEXR

# Check for OpenEXR
AC_CHECK_LIB([Half], [main])
AC_CHECK_LIB([Iex], [main])
AC_CHECK_LIB([Imath], [main])
AC_CHECK_LIB([IlmImf], [main])

# Check for video encoder
AC_CHECK_LIB([x264], [main])
AC_CHECK_LIB([x265], [main])
AC_CHECK_LIB([swscale], [main])
AC_CHECK_LIB([avutil], [main])
AC_CHECK_LIB([avformat], [main])
AC_CHECK_LIB([avcodec], [main])

# Check for image io libraries
AC_CHECK_LIB([tiff], [main])
AC_CHECK_LIB([jpeg], [main])

# Check for system libraries
AC_CHECK_LIB([dl], [main])
AC_CHECK_LIB([pthread], [main])
AC_CHECK_LIB([z], [main])
AC_CHECK_LIB([cppunit], [main], [AC_DEFINE([HAVE_LIBCPPUNIT], [1], [Is CppUnit available?]) CPPUNITLIBS="$CPPUNITLIBS -lcppunit"])

AC_LANG_POP(C++)

CPPFLAGS="$CPPFLAGS -I$BLZ3_SYSINC -I$BLZ3_INCLUDE"
LDFLAGS="$LDFLAGS -L$BLZ3_LIB $X_LIBS"
ARFLAGS="rs"

AC_CHECK_LIB([X11], [main])

AC_CHECK_FUNCS_ONCE([av_muxer_iterate av_codec_iterate])

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_MMAP
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_VPRINTF

AC_CONFIG_FILES([
	control-blz3
	control-libblz3-dev
	Doxyfile
	src/flags.pri
	src/Makefile
	src/icc.make
	src/system_unix/Makefile
	src/system/Makefile
	src/base/Makefile
	src/image/Makefile
	src/raytrace/Makefile
	src/test_unix/Makefile
	src/OpenGL/Makefile
	src/brt3/Makefile
	src/tools/Makefile
	src/cpuperf/Makefile
	src/unittest/Makefile])
AC_OUTPUT
