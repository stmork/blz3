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

#include <unordered_map>

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
#endif

#define B3_PLUGIN

#if defined(HAVE_LIBSWSCALE) && defined(HAVE_LIBAVCODEC) && defined(HAVE_LIBAVFORMAT) && defined(HAVE_LIBAVUTIL)
#	define HAVE_VIDEO_ENCODER
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
#define B3_FILESTRINGLEN  (PATH_MAX)

#endif
