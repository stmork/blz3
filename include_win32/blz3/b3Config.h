/*
**
**	$Filename:	b3Config.h $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III base defines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_CONFIG_H
#define B3_CONFIG_H

#define BLIZZARD3_REG_COMPANY  "Steffen A. Mork"
#define BLIZZARD3_REG_PRODUCT  "Blizzard 3"

#include "stdafx.h"

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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/01 16:31:51  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**
**	
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III Windoze specific defines         **
**                                                                      **
*************************************************************************/

#ifdef _WINDOWS

// Some unessessary warnings in level 4 we have to disable first
#	pragma warning(disable : 4100)
#	pragma warning(disable : 4711)

// Some linker optimazations
#	ifndef _DEBUG
/*
#		pragma comment(linker,"/RELEASE")
#		pragma comment(linker,"/merge:.rdata=.data")
#		pragma comment(linker,"/merge:.text=.data")
#		pragma comment(linker,"/merge:.reloc=.data")
*/
#		if _MSC_VER >= 1000
			// Only supported/needed with VC6; VC5 already does 0x200 for release builds.
			// Totally undocumented! And if you set it lower than 512 bytes, the program crashes.
			// Either leave at 0x200 or 0x1000
#			pragma comment(linker,"/FILEALIGN:0x200")
#		endif // _MSC_VER >= 1000
#	endif

#	include "StdAfx.h"

#	define FILESTRINGLEN		_MAX_PATH
#	define MAXHOSTNAMELEN		 64

#	define INTEL    0x4949
#	define MOTOROLA 0x4d4d

#	if defined (WIN32)
#		define IS_WIN32      TRUE
#		define THISPROCESSOR INTEL
#	else
#		define IS_WIN32      FALSE
#	endif
#	define IS_NT             (IS_WIN32 && (BOOL)(GetVersion() < 0x80000000))
#	define IS_WIN32S         (IS_WIN32 && (BOOL)(!(IS_NT) && (LOBYTE(LOWORD(GetVersion())) < 4)))
#	define IS_WIN95	         ((BOOL)(!(IS_NT) && !(IS_WIN32S)) && IS_WIN32)

#	ifdef _DEBUG
#		define new DEBUG_NEW
#	endif

#	define WM_MAUI_BARCODE         (WM_USER + 10)
#	define WM_MAUI_PAGECOUNT       (WM_USER + 11)
#	define WM_MAUI_SETTITLE        (WM_USER + 12)
#	define WM_MAUI_UPDATE_CONTROLS (WM_USER + 13)
#	define WM_MAUI_SCAN_READY      (WM_USER + 14)
#	define WM_MAUI_SCAN_RATE       (WM_USER + 15)
#endif	// _WINDOWS

#define USE_JPEGLIB_LOAD
#define USE_JPEGLIB_SAVE
 
#define USE_TIFFLIB_LOAD
#define USE_TIFFLIB_SAVE
  
#ifndef __linux__
#define no_USE_MOTIF
#endif

#define B3_FRAN(x)      ((float)x        * drand48())
#define B3_IRAN(x)      ((long)((float)x * drand48()))
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
