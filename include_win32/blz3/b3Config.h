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

#define main(argc,argv) _Blizzard3Main(argc,argv)

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

#if defined (WIN32)
#	define IS_WIN32      TRUE
#	define THISPROCESSOR INTEL
#	ifndef _WINDOWS
#		define _WINDOWS
#	endif
#else
#	define IS_WIN32      FALSE
#	error "We need at least 32 Bit! Do you understand?"
#endif // WIN32

#include "blz3/b3Types.h"
#include "blz3/system/b3Log.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**
**	Revision 1.3  2001/07/02 19:28:25  sm
**	- Applying console application on Windows 32
**	- Added further Windows environment
**	
**	Revision 1.2  2001/07/01 19:14:23  sm
**	- Include of JPEG and TIFF libraries
**	
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

// Define this keyword to nothing (for TIFF library)
#define huge

// Some unessessary warnings in level 4 we have to disable first
#pragma warning(disable : 4100)
#pragma warning(disable : 4711)

// Some linker optimazations
#ifndef _DEBUG
/*
#	pragma comment(linker,"/RELEASE")
#	pragma comment(linker,"/merge:.rdata=.data")
#	pragma comment(linker,"/merge:.text=.data")
#	pragma comment(linker,"/merge:.reloc=.data")
*/
#	if _MSC_VER >= 1000
		// Only supported/needed with VC6; VC5 already does 0x200 for release builds.
		// Totally undocumented! And if you set it lower than 512 bytes, the program crashes.
		// Either leave at 0x200 or 0x1000
#		pragma comment(linker,"/FILEALIGN:0x200")
#	endif // _MSC_VER >= 1000
#endif // _DEBUG

#define INTEL    0x4949
#define MOTOROLA 0x4d4d

#define IS_NT             (IS_WIN32 && (BOOL)(GetVersion() < 0x80000000))
#define IS_WIN32S         (IS_WIN32 && (BOOL)(!(IS_NT) && (LOBYTE(LOWORD(GetVersion())) < 4)))
#define IS_WIN95	         ((BOOL)(!(IS_NT) && !(IS_WIN32S)) && IS_WIN32)

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

#define USE_JPEGLIB_LOAD
#define USE_JPEGLIB_SAVE
 
#define USE_TIFFLIB_LOAD
#define USE_TIFFLIB_SAVE
  
#ifndef __linux__
#	define no_USE_MOTIF
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

enum b3_msg_result
{
	B3_MSG_OK      = 0,
	B3_MSG_YES	    = 1,
	B3_MSG_NO		= 2,
	B3_MSG_CANCEL	= 3
};

enum b3_msgbox_type
{
	B3_MSGBOX_OK        = 0,
	B3_MSGBOX_YES_NO    = 1,
	B3_MSGBOX_OK_CANCEL = 2,
	B3_MSGBOX_WARN      = 3,
	B3_MSGBOX_ERROR     = 4,
	B3_MSGBOX_INFO      = B3_MSGBOX_OK
};


class b3Runtime
{
	b3_cpu_type        cpu_type;
public:
	                     b3Runtime();
	static void          b3PSwap(b3_u32 *uPtr1,b3_u32 *uPtr2);
	static void          b3Beep();
	static b3_bool       b3Hostname(char *hostname,const b3_size buffer_size);
	static b3_s32        b3Execute(const char *command, const b3_bool async);
	static b3_cpu_type   b3GetCPUType();
	static b3_msg_result b3MessageBox(const char *message,   const b3_msgbox_type type=B3_MSGBOX_OK,const char *title=null);
	static b3_msg_result b3MessageBox(const long  message_id,const b3_msgbox_type type=B3_MSGBOX_OK,const char *title=null);
};

#endif
