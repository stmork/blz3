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

#include "blz3/b3Types.h"
#include "blz3/system/b3Log.h"

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

#ifdef _DEBUG
#include <assert.h>

#define B3_ASSERT(a) assert(a)
#else
#define B3_ASSERT(a)
#endif

#define B3_FRAN(x)      ((float)x        * drand48())
#define B3_IRAN(x)      ((long)((float)x * drand48()))
#define B3_ABS(a)        ((a) < 0 ? (-(a)) : (a))
#define B3_SWAP(a,b)    {(a)^=(b);(b)^=(a);(a)^=(b);}
#define B3_PSWAP(a,b)   b3Runtime::b3PSwap(a,b)
#define B3_MIN(a,b)     ((a) < (b) ? (a) : (b))
#define B3_MAX(a,b)     ((a) < (b) ? (b) : (a))

#define B3_BEEP         b3Runtime::b3Beep()

#define B3_VERSION  3
#define B3_REVISION 0

#define BREAD      "r"
#define BWRITE     "w"
#define BAPPEND    "a"
#define TREAD      "r"
#define TWRITE     "w"  
#define TAPPEND    "a"

typedef enum
{
	B3_BIG_ENDIAN    = 0x4d4d,
	B3_LITTLE_ENDIAN = 0x4949
} b3_cpu_type;

class b3Runtime
{
	b3_cpu_type        cpu_type;
public:
	                   b3Runtime();
	static void        b3PSwap(b3_u32 *uPtr1,b3_u32 *uPtr2);
	static void        b3Beep();
	static b3_bool     b3Hostname(char *hostname,const b3_size buffer_size);
	static b3_s32      b3Execute(const char *command, const b3_bool async);
	static b3_cpu_type b3GetCPUType();
};

#endif
