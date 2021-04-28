/*
**
**	$Filename:	b3Config.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - System configuration
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_CONFIG_H
#define B3_CONFIG_H

#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "blz3/autoconf.h"
#include "blz3/b3Types.h"
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Thread.h"
#include "blz3/system/b3Assert.h"

#ifdef BLZ3_USE_OPENGL
#	include "GL/glu.h"
#	include "blz3_glext.h"

#	ifdef BLZ3_USE_GLUT
#		include <GL/glut.h>
#		define no_GLU_NURBS
#	endif

#	ifdef BLZ3_USE_CG
#		include <Cg/cg.h>
#		include <Cg/cgGL.h>
#	endif
#endif

#ifdef HAVE_SSE
#	include <xmmintrin.h>
#	ifdef HAVE_SSE2
#		include <emmintrin.h>
#		ifdef HAVE_SSE3
#			include <pmmintrin.h>
#			ifdef HAVE_SSSE3
#				include <tmmintrin.h>
#			endif
#		endif
#	endif
#	ifdef SSE_ALIGNED
#		define SSE_PS_LOAD(p)      (p)
#		define SSE_PS_STORE(p, a)  p = (a)
#		define SSE_PD_LOAD(p)      (p)
#		define SSE_PD_STORE(p, a)  p = (a)
#	else
#		define SSE_PS_LOAD(p)      _mm_loadu_ps(p)
#		define SSE_PS_STORE(p, a)  _mm_storeu_ps(p, a)
#		define SSE_PD_LOAD(p)      _mm_loadu_pd(p)
#		define SSE_PD_STORE(p, a)  _mm_storeu_pd(p, a)
#	endif
#endif

#define USE_JPEGLIB_LOAD
#define USE_JPEGLIB_SAVE

#define USE_TIFFLIB_LOAD
#define USE_TIFFLIB_SAVE

#define B3_PLUGIN

#if defined(HAVE_LIBSWSCALE) && defined(HAVE_LIBAVCODEC) && defined(HAVE_LIBAVFORMAT) && defined(HAVE_LIBAVUTIL)
#	define HAVE_VIDEO_ENCODER
#endif

typedef unsigned long b3DrawContext;

#ifndef HAVE_STRLCPY
static inline size_t strlcpy(char * dst, const char * src, size_t size)
{
	if ((size > 0) && (src != dst))
	{
		strncpy(dst, src, size);
		dst[size - 1] = 0;
	}
	return strlen(dst);
}
#endif

#ifndef HAVE_STRICMP
#ifdef HAVE_STRCASECMP
#define stricmp(a,b) strcasecmp(a,b)
#else
static inline int stricmp(const char * a, const char * b)
{
	int diff, i;

	i = 0;
	do
	{
		diff = tolower(a[i]) - tolower(b[i]);
		if ((a[i] == 0) || (b[i] == 0))
		{
			return diff;
		}
		i++;
	}
	while (diff == 0);

	return diff;
}
#endif
#endif

#define B3_FRAN(x)          ((double)(x)        * drand48())
#define B3_IRAN(x)          ((long)((double)(x) * drand48()))
#define B3_SWAP(a,b)        {(a)^=(b);(b)^=(a);(a)^=(b);}

#define B3_BREAD      "r"
#define B3_BWRITE     "w"
#define B3_BAPPEND    "a"
#define B3_TREAD      "r"
#define B3_TWRITE     "w"
#define B3_TAPPEND    "a"

#define B3_MAXHOSTNAMELEN (MAXHOSTNAMELEN)
#define B3_FILESTRINGLEN  (MAXNAMLEN)

/**
 * This class provides some runtime information. See the b3CPU class
 * for some more information.
 */
class B3_PLUGIN b3Runtime : public b3CPU
{
	char               m_Compiler[256];

	b3Runtime();

public:
	/**
	 * This method returns the singleton instance of the runtime description.
	 *
	 * @return The runtime description.
	 */
	static inline b3Runtime & b3Instance()
	{
		static b3Runtime   runtime;

		return runtime;
	}

	/**
	 * This method returns the number of usable CPUs.
	 *
	 * @return Number of usable CPUs.
	 */
	static inline b3_count b3GetNumCPUs()
	{
		return b3Instance().cpu_count;
	}

	/**
	 * This method returns the used endian type of the CPU.
	 *
	 * @return The CPUs endian.
	 */
	static inline b3_cpu_type b3GetCPUType()
	{
		return b3Instance().cpu_type;
	}

	/**
	 * This method returns the CPU's bit count. This is the size of
	 * a pointer in bits.
	 *
	 * @return The used address space.
	 */
	static inline b3_count b3GetCPUBits()
	{
		return b3Instance().cpu_bits;
	}

	/**
	 * This method returns the hostname.
	 *
	 * @param hostname The buffer which receives the hostname.
	 * @param size     The buffer size.
	 * @return True on success.
	 */
	static b3_bool     b3Hostname(char * hostname, const b3_size size);

	/**
	 * This method executes another program.
	 *
	 * @param command The command string to execute via shell.
	 * @param async If the command should start asynchronously.
	 * @return The execution result when executing synchronously.
	 */
	static b3_s32      b3Execute(const char * command, const b3_bool async);

	/**
	 * This method returns information about the used compiler.
	 */
	static const char * b3GetCompiler();

	/**
	 * This method returns an OpenGL function pointer from the given procedure name.
	 *
	 * @param procedure The procedure name to solve.
	 * @return The mapped procedure.
	 */
	static void    *   b3GetOpenGLExtension(const char * procedure);

	/**
	 * This method parses the command line arguments.
	 *
	 * @param argc The number of arguments.
	 * @param argv The arguments itself.
	 * @param index The argument index to start parsing.
	 * @param option The buffer which receives the option argument.
	 * @param size The size of the option buffer.
	 * @return The next index to process.
	 */
	static int         b3ParseOption(int argc, char * argv[], int index, char * option, size_t size);
};

#endif
