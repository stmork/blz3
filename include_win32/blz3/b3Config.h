/*
**
**	$Filename:	b3Config.h $
**	$Release:	Dortmund 2001 $
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

#define BLIZZARD3_REG_COMPANY  "MORKNet"
#define BLIZZARD3_REG_PRODUCT  "Blizzard III"

#define main(argc,argv) _Blizzard3Main(argc,argv)

// Include MFC stuff
#include "stdafx.h"

// Make memory leaks debuggable
#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

// Include some standard C
#include <ctype.h>
#include <fcntl.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "blz3/b3PluginDef.h"
#include "blz3/system/b3Thread.h"

#ifdef _DEBUG
#include <assert.h>
#define B3_ASSERT(cond) ASSERT(cond)
#else
#define B3_ASSERT(cond)
#endif

// OpenGL is nice...
#ifdef BLZ3_USE_OPENGL
#include <GL/gl.h>
#include <GL/glu.h>
#include "blz3/glext.h"
#endif

// Some defines for getting to know who we are
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

#define IS_NT        (IS_WIN32 && (BOOL)(GetVersion() < 0x80000000))
#define IS_WIN2K     (IS_NT && (LOBYTE(LOWORD(GetVersion())) >= 5))
#define IS_WIN32S    (IS_WIN32 && (BOOL)(!(IS_NT) && (LOBYTE(LOWORD(GetVersion())) < 4)))
#define IS_WIN95     ((BOOL)(!(IS_NT) && !(IS_WIN32S)) && IS_WIN32)

typedef CDC b3DrawContext;

// Ah! Blizzard III
#include "blz3/b3Types.h"
#include "blz3/system/b3Log.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.33  2004/12/27 20:14:18  sm
**	- Added Windows installation project
**
**	Revision 1.32  2004/09/23 20:02:25  sm
**	- Introduced VBOs on Windows - with success!
**	
**	Revision 1.31  2004/09/23 15:47:03  sm
**	- Splitted b3RenderContext into own file.
**	- Added vertex buffer object support which does not
**	  run yet.
**	
**	Revision 1.30  2004/08/28 14:02:14  sm
**	- Including correct FLT_MAX/DBL_MAX file.
**	
**	Revision 1.29  2004/08/28 13:55:33  sm
**	- Added some mirror methods.
**	- Cleanup job.
**	
**	Revision 1.28  2004/01/18 14:29:26  sm
**	- Added Windows security changes
**	
**	Revision 1.27  2003/08/31 08:56:21  sm
**	- Windows support for the snprintf functions
**	
**	Revision 1.26  2003/05/30 14:44:09  sm
**	- Plugin support for Windows added. All exported classes got an
**	  additional keyword: B3_PLUGIN
**	
**	Revision 1.25  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.24  2003/01/03 15:47:09  sm
**	- Changed area light optimization.
**	- Fixed some errors in the light dialog.
**	
**	Revision 1.23  2002/08/09 13:20:18  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.22  2002/07/31 11:57:08  sm
**	- The nVidia OpenGL init bug fixed by using following work
**	  around: The wglMakeCurrent() method is invoked on
**	  every OnPaint(). This is configurable depending on the
**	  hostname.
**	
**	Revision 1.21  2002/04/07 12:59:37  sm
**	- Added support for file dialog with Windows 2000 place bars (Cb3FileDialog)
**	- CB3FileDialog used for CWinApp::OnFileOpen()
**	- Image buttons changed to draw disabled state correctly using
**	  CDC::DrawState()
**	
**	Revision 1.20  2002/03/01 20:26:38  sm
**	- Added CB3FloatSpinButtonCtrl for conveniant input.
**	- Made some minor changes and tests.
**	
**	Revision 1.19  2001/12/04 18:23:25  sm
**	- Drawing LDC correctly
**	- Added pick point support.
**	
**	Revision 1.18  2001/11/11 11:51:19  sm
**	- Added image select feature
**	- Cleaned up scene dialog (Now ready to improve it)
**	- some b3Path fixes
**	
**	Revision 1.17  2001/10/22 14:47:38  sm
**	- Type correction vor b3Base/b3Link. So fixed a bad behaviour
**	  on Windows.
**	- Some minor fixes.
**	
**	Revision 1.16  2001/10/20 16:25:07  sm
**	- Windows compile check an minor fixes
**	
**	Revision 1.15  2001/10/20 16:14:59  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**	
**	Revision 1.14  2001/10/17 21:09:06  sm
**	- Triangle support added for intersections, normal computations. So
**	  Spline shapes can be computed, too. Now only CSG is missing.
**	
**	Revision 1.13  2001/10/15 14:45:07  sm
**	- Materials are accessing textures now.
**	- Created image viewer "bimg3"
**	
**	Revision 1.12  2001/10/13 09:56:44  sm
**	- Minor corrections
**	
**	Revision 1.11  2001/10/07 20:41:32  sm
**	- Updating MSVC project status
**	
**	Revision 1.10  2001/08/16 04:28:29  sm
**	- Solving conflicts
**	
**	Revision 1.9  2001/08/14 13:34:39  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**	
**	Revision 1.8  2001/08/09 15:27:34  sm
**	- Following shapes are newly supported now:
**	  o disk
**	  o cylinder
**	  o cone
**	  o ellipsoid
**	  o torus
**	  o triangles
**	- Done some makefile fixes
**	- Everything is Windozable
**	
**	Revision 1.7  2001/08/05 19:53:43  sm
**	- Removing some nasty CR/LF
**	
**	Revision 1.6  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**	Revision 1.5  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
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

// Define this to prevent different types i JPEG library
#define XMD_H

// Some unessessary warnings in level 4 we have to disable first
#pragma warning(disable : 4100)
#pragma warning(disable : 4711)
#pragma warning(disable : 4244)

#define INTEL    0x4949
#define MOTOROLA 0x4d4d

#define USE_JPEGLIB_LOAD
#define USE_JPEGLIB_SAVE
#define HAVE_BOOLEAN

#define USE_TIFFLIB_LOAD
#define USE_TIFFLIB_SAVE
  
#ifndef __linux__
#	define no_USE_MOTIF
#endif

#define B3_FRAN(x)                 ((x) * (b3_f64)rand() / (RAND_MAX + 1))
#define B3_IRAN(x)          ((long)((x) *         rand() / (RAND_MAX + 1)))
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

enum b3_msg_result
{
	B3_MSG_OK     = 0,
	B3_MSG_YES    = 1,
	B3_MSG_NO     = 2,
	B3_MSG_CANCEL = 3
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

#define B3_MAXHOSTNAMELEN ((MAX_COMPUTERNAME_LENGTH) + 1)
#define B3_FILESTRINGLEN  (_MAX_PATH)

#define  snprintf  _snprintf
#define vsnprintf _vsnprintf

static inline char *strlcpy(char *dst,const char *src,size_t size)
{
	if (size > 0)
	{
		strncpy(dst,src,size);
		dst[size-1] = 0;
	}
	return dst;
}

class B3_PLUGIN b3Runtime : public b3CPU
{
	static b3Runtime     m_Runtime;

	                     b3Runtime();

public:
	static void           b3PSwap(b3_u32 *uPtr1,b3_u32 *uPtr2);
	static void           b3Beep();
	static b3_bool        b3Hostname(char *hostname,const b3_size buffer_size);
	static b3_s32         b3Execute(const char *command, const b3_bool async);
	static b3_msg_result  b3MessageBox(const char *message,   const b3_msgbox_type type=B3_MSGBOX_OK,const char *title=null);
	static b3_msg_result  b3MessageBox(const long  message_id,const b3_msgbox_type type=B3_MSGBOX_OK,const char *title=null);
	static void          *b3GetOpenGLExtension(const char *procedure_name);
};

#endif
