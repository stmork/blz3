/*
**
**      $Filename:      b3Config.h $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - basic configuration
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

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

#include "blz3/b3Types.h"
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Thread.h"

#ifdef BLZ3_USE_OPENGL
#include <GL/glut.h>

#define no_GLU_NURBS
#endif

#define USE_JPEGLIB_LOAD
#define USE_JPEGLIB_SAVE
 
#define USE_TIFFLIB_LOAD
#define USE_TIFFLIB_SAVE
  
#ifndef __linux__
#define no_USE_MOTIF
#endif

#ifndef __GNUC_PATCHLEVEL__
#define __GNUC_PATCHLEVEL__ 0
#endif

#ifdef _DEBUG
#include <assert.h>

#define B3_ASSERT(a) assert(a)
#else
#define B3_ASSERT(a)
#endif

typedef unsigned long b3DrawContext;

static inline int stricmp(const char *a,const char *b)
{
	int diff = 0,i;

	for (i = 0;diff == 0;i++)
	{
		diff = tolower(a[i]) - tolower(b[i]);
		if ((a[i] == 0) || (b[i] == 0))
		{
			return diff;
		}
	}
	return diff;
}

#define B3_FRAN(x)          ((float)x        * drand48())
#define B3_IRAN(x)          ((long)((float)x * drand48()))
#define B3_SWAP(a,b)        {(a)^=(b);(b)^=(a);(a)^=(b);}
#define B3_PSWAP(a,b)       b3Runtime::b3PSwap(a,b)
#define B3_BEEP             b3Runtime::b3Beep()

#define B3_VERSION  3
#define B3_REVISION 0

#define B3_BREAD      "r"
#define B3_BWRITE     "w"
#define B3_BAPPEND    "a"
#define B3_TREAD      "r"
#define B3_TWRITE     "w"  
#define B3_TAPPEND    "a"

typedef enum
{
	B3_BIG_ENDIAN    = 0x4d4d,
	B3_LITTLE_ENDIAN = 0x4949
} b3_cpu_type;

#define B3_MAXHOSTNAMELEN (MAXHOSTNAMELEN)
#define B3_FILESTRINGLEN  (MAXNAMLEN)

class b3Runtime : public b3CPU
{
	b3_cpu_type        cpu_type;
public:
	                   b3Runtime();
	static void        b3PSwap(b3_u32 *uPtr1,b3_u32 *uPtr2);
	static void        b3Beep();
	static b3_bool     b3Hostname(char *hostname,const b3_size buffer_size);
	static b3_s32      b3Execute(const char *command, const b3_bool async);
	static b3_cpu_type b3GetCPUType();
	static b3_count    b3GetNumThreads();
	static b3_count    b3GetNumCPUs();
};

#endif
