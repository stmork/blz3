/*
**
**	$Filename:	b3PixelFormat.h $
**	$Release:	Dortmund 2003 $
**	$Revision: 1046 $
**	$Date: 2005-01-18 12:49:05 +0100 (Di, 18 Jan 2005) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - maintainance of pixel format descriptors
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef PIXELFORMAT_H
#define PIXELFORMAT_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Array.h"

struct CB3PixelFormatDescriptor
{
	 PIXELFORMATDESCRIPTOR desc;
	 b3_index              index;
};

typedef int (*b3PixelFormatSortFunc)(CB3PixelFormatDescriptor *a,CB3PixelFormatDescriptor *b);

class CB3PixelFormat
{
	static const PIXELFORMATDESCRIPTOR m_WindowPixelformat;
	static const PIXELFORMATDESCRIPTOR m_PrintPixelformat;

	b3Array<CB3PixelFormatDescriptor> m_glPixelFormat;

protected:
	// OpenGL window display values
	HDC             m_glDC;
	HGLRC           m_glGC;

protected:
	       void  b3ListPixelFormats(HDC dc,const char *title = "");
	       HGLRC b3CreateContext(HDC dc,b3PixelFormatSortFunc func, b3_bool &double_buffered);
	static void  b3FlagsString(CString &desc,int flags);
	static int   b3ComputePixelFormatMode(const PIXELFORMATDESCRIPTOR *input,const PIXELFORMATDESCRIPTOR *templ);
	static int   b3PixelFormatSorter(PIXELFORMATDESCRIPTOR *a,PIXELFORMATDESCRIPTOR *b,const PIXELFORMATDESCRIPTOR *templFormat);
	static int   b3WindowPixelFormatSorter(CB3PixelFormatDescriptor *a,CB3PixelFormatDescriptor *b);
	static int   b3PrinterPixelFormatSorter(CB3PixelFormatDescriptor *a,CB3PixelFormatDescriptor *b);
};

#endif
