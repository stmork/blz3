/*
**
**	$Filename:	b3PixelFormat.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - maintainance of pixel format descriptors
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3PixelFormat.h"
#include "blz3/system/b3App.h"
#include "blz3/base/b3Render.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2005/01/13 20:05:16  sm
**	- Some Lines bugfixes
**
**	Revision 1.4  2004/12/30 16:27:39  sm
**	- Removed assertion problem when starting Lines III: The
**	  image list were initialized twice due to double calling
**	  OnInitDialog() of CDialogBar. The CDialogBar::Create()
**	  calls OnInitDialog() automatically sinde MFC 7
**	- Removed many global references from raytrace and base lib
**	- Fixed ticket no. 29: The b3RenderObject::b3Recompute
**	  method checks the vertex maintainer against a null pointer.
**	
**	Revision 1.3  2004/10/12 19:54:19  sm
**	- Some camera/light resort. We have to draw the
**	  light just after the camera to ensure a proper
**	  view matrix as part of the model view matrix.
**	
**	Revision 1.2  2003/02/25 17:07:43  sm
**	- Fixed wrong include
**	
**	Revision 1.1  2003/02/25 15:56:21  sm
**	- Added SplineRot to control grid drawing.
**	- Added support for pixel format selection in dialog items
**	- Restructured b3PickInfo
**	
*/

/*************************************************************************
**                                                                      **
**                        OpenGL pixel format computation               **
**                                                                      **
*************************************************************************/

const PIXELFORMATDESCRIPTOR CB3PixelFormat::m_WindowPixelformat =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,          // color bits
	0,0,0,0,0,0, // rgb bits/shift
	0,0,         // alpha bits/shift
	0,0,0,0,0,   // accum rgba bits
	24,          // depth bits
	0,           // stencil bits
	0,           // aux buffers
	0,           // layer type
	0,           // reserverd
	0,0,0        // layer/visible/damage mask
};

const PIXELFORMATDESCRIPTOR CB3PixelFormat::m_PrintPixelformat =
{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_GENERIC_FORMAT,
	PFD_TYPE_RGBA,
	24,          // color bits
	0,0,0,0,0,0, // rgb bits/shift
	0,0,         // alpha bits/shift
	0,0,0,0,0,   // accum rgba bits
	32,          // depth bits
	0,           // stencil bits
	0,           // aux buffers
	0,           // layer type
	0,           // reserverd
	0,0,0        // layer/visible/damage mask
};

#define B3_PF_PIXELTYPE    16
#define B3_PF_ACCELERATE    8
#define B3_PF_DOUBLEBUFFER  4
#define B3_PF_SWAP          2
#define B3_PF_DESTINATION   1

int CB3PixelFormat::b3ComputePixelFormatMode(
	const PIXELFORMATDESCRIPTOR *input,
	const PIXELFORMATDESCRIPTOR *templ)
{
	int inputFlags = input->dwFlags;
	int templFlags = templ->dwFlags;
	int mode       = 0;

	if (input->iPixelType == templ->iPixelType)
	{
		mode |= B3_PF_PIXELTYPE;
	}

	if ((inputFlags & PFD_GENERIC_FORMAT) == (templFlags & PFD_GENERIC_FORMAT))
	{
		mode |= B3_PF_ACCELERATE;
	}
	
	if ((inputFlags & PFD_DOUBLEBUFFER) == (templFlags & PFD_DOUBLEBUFFER))
	{
		mode |= B3_PF_DOUBLEBUFFER;
		if (inputFlags & PFD_SWAP_EXCHANGE)
		{
			mode |= B3_PF_SWAP;
		}
	}

	if ((inputFlags & (PFD_DRAW_TO_BITMAP | PFD_DRAW_TO_WINDOW)) == (templFlags & (PFD_DRAW_TO_BITMAP | PFD_DRAW_TO_WINDOW)))
	{
		mode |= B3_PF_DESTINATION;
	}
	
	return mode;
}

int CB3PixelFormat::b3PixelFormatSorter(
	      PIXELFORMATDESCRIPTOR *a,
	      PIXELFORMATDESCRIPTOR *b,
	const PIXELFORMATDESCRIPTOR *templFormat)
{
	int result;

	int aMode = b3ComputePixelFormatMode(a,templFormat);
	int bMode = b3ComputePixelFormatMode(b,templFormat);

	result = bMode - aMode;
	if (result == 0)
	{
		result = b->cColorBits - a->cColorBits;
		if (result == 0)
		{
			result = b->cDepthBits - a->cDepthBits;
		}
	}
	return result;
}

int CB3PixelFormat::b3WindowPixelFormatSorter(CB3PixelFormatDescriptor *a,CB3PixelFormatDescriptor *b)
{
	return b3PixelFormatSorter(&a->desc,&b->desc,&m_WindowPixelformat);
}

int CB3PixelFormat::b3PrinterPixelFormatSorter(CB3PixelFormatDescriptor *a,CB3PixelFormatDescriptor *b)
{
	return b3PixelFormatSorter(&a->desc,&b->desc,&m_PrintPixelformat);
}

void CB3PixelFormat::b3FlagsString(CString &desc,int flags)
{
	desc = "";

	if (flags & PFD_GENERIC_FORMAT)      desc += " PFD_GENERIC_FORMAT";
	if (flags & PFD_DRAW_TO_WINDOW)      desc += " PFD_DRAW_TO_WINDOW";
	if (flags & PFD_DRAW_TO_BITMAP)      desc += " PFD_DRAW_TO_BITMAP";
	if (flags & PFD_SUPPORT_GDI)         desc += " PFD_SUPPORT_GDI";
	if (flags & PFD_SUPPORT_OPENGL)      desc += " PFD_SUPPORT_OPENGL";
	if (flags & PFD_DOUBLEBUFFER)        desc += " PFD_DOUBLEBUFFER";
	if (flags & PFD_SWAP_EXCHANGE)       desc += " PFD_SWAP_EXCHANGE";
	if (flags & PFD_SWAP_COPY)           desc += " PFD_SWAP_COPY";
	if (flags & PFD_STEREO)              desc += " PFD_STEREO";
	if (flags & PFD_NEED_PALETTE)        desc += " PFD_NEED_PALETTE";
	if (flags & PFD_NEED_SYSTEM_PALETTE) desc += " PFD_NEED_SYSTEM_PALETTE";
	if (flags & PFD_SWAP_LAYER_BUFFERS)  desc += " PFD_SWAP_LAYER_BUFFERS";
	if (flags & PFD_GENERIC_ACCELERATED) desc += " PFD_GENERIC_ACCELERATED";
	if (flags & PFD_SUPPORT_DIRECTDRAW)  desc += " PFD_SUPPORT_DIRECTDRAW";
}

HGLRC CB3PixelFormat::b3CreateContext(HDC dc,b3PixelFormatSortFunc func)
{
	CB3PixelFormatDescriptor format;
	CString                  flags;
	HGLRC                    gc;
	int                      PixelFormatIndex,max,i,value;

	// Retrieve all pixel formats
	max = DescribePixelFormat(dc,1,0,NULL);
	format.desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	m_glPixelFormat.b3Clear();
	for (i = 1;i <= max;i++)
	{
		format.index = i;
		if (DescribePixelFormat(dc,i,format.desc.nSize,&format.desc) != 0)
		{
			m_glPixelFormat.b3Add(format);
		}
	}

	// List and sort pixel formats depending on our requirements.
	b3ListPixelFormats(dc,"Known OpenGL pixel formats:");
	m_glPixelFormat.b3Sort(func);
	b3ListPixelFormats(dc,"Sorted OpenGL pixel formats:");

	// What we found...
	PixelFormatIndex = m_glPixelFormat[0].index;
	SetPixelFormat(dc,PixelFormatIndex,&m_glPixelFormat[0].desc);
	gc = wglCreateContext(dc);

	if (gc != null)
	{
		CB3GetApp()->b3SelectRenderContext(dc,gc);

		b3PrintF(B3LOG_DEBUG,"Pixel values of chosen pixel format index: %d:\n",
			PixelFormatIndex);
		
		b3RenderContext::b3Init();
		glGetIntegerv(GL_RED_BITS,  &value); b3PrintF(B3LOG_DEBUG,"R: %2d\n",value);
		glGetIntegerv(GL_GREEN_BITS,&value); b3PrintF(B3LOG_DEBUG,"G: %2d\n",value);
		glGetIntegerv(GL_BLUE_BITS, &value); b3PrintF(B3LOG_DEBUG,"B: %2d\n",value);
		glGetIntegerv(GL_ALPHA_BITS,&value); b3PrintF(B3LOG_DEBUG,"A: %2d\n",value);
		glGetIntegerv(GL_DEPTH_BITS,&value); b3PrintF(B3LOG_DEBUG,"Z: %2d\n",value);
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"Cannot allocate GC for píxel format index %d.\n",
			PixelFormatIndex);
	}

	return gc;
}

void CB3PixelFormat::b3ListPixelFormats(HDC dc,const char *title)
{
#ifdef _DEBUG
	int                   max,i;
	PIXELFORMATDESCRIPTOR format;
	CString               flags;

	format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	max = DescribePixelFormat(dc,1,0,NULL);

	b3PrintF(B3LOG_DEBUG,"========================================= %s\n",title);
	b3PrintF(B3LOG_DEBUG,"(index) (flags) (pixeltype) (color bits) (depth) (accum bits):\n");
	for (i = 0;i < m_glPixelFormat.b3GetCount();i++)
	{
		b3FlagsString(flags,m_glPixelFormat[i].desc.dwFlags);
		b3PrintF(B3LOG_DEBUG,
			"%3d: %8lx %8lx %2ld %2ld %2ld%s\n",
			m_glPixelFormat[i].index,
			m_glPixelFormat[i].desc.dwFlags,
			m_glPixelFormat[i].desc.iPixelType,
			m_glPixelFormat[i].desc.cColorBits,
			m_glPixelFormat[i].desc.cDepthBits,
			m_glPixelFormat[i].desc.cAccumBits,flags);
	}
	b3PrintF(B3LOG_DEBUG,"=========================================\n");
#endif
}
