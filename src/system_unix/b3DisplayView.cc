/*
**
**      $Filename:      b3DisplayView.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**      $Id$
**
**      Blizzard III - Controlling a color display
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Log.h"
#include "blz3/base/b3Aux.h"

#define no_SYNC
#define no_DEBUG

#define READY 1

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/12/23 10:58:38  sm
**	- Accelerated b3Display.
**	- Fixed YUV conversion.
**	- Accelerated ILBM access to image  pixel/row.
**
**	Revision 1.1  2001/11/04 12:15:15  sm
**	- Renaming some attributes...
**	- Taking account to redesign of b3Display
**	
**	Revision 1.9  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.8  2001/10/31 14:46:35  sm
**	- Filling b3IsCancelled() with sense.
**	- Inlining b3RGB
**	
**	Revision 1.7  2001/10/24 14:59:08  sm
**	- Some GIG bug fixes
**	- An image viewing bug fixed in bimg3
**	
**	Revision 1.6  2001/10/19 14:46:58  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.5  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
**	
**	Revision 1.4  2001/07/08 13:02:19  sm
**	- Merging with Windoze stuff.
**	
**	Revision 1.3  2001/07/08 12:56:43  sm
**	- Now displaying Mandelbrot set. It's like switching all light on together.
**	
**	Revision 1.2  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        colormap routines                             **
**                                                                      **
*************************************************************************/

static b3Mutex  display_mutex;
static Colormap cmap;
static b3_count cmap_count;
static long     count = READY;

b3_bool b3DisplayView::b3CreateColormap ()
{
	b3_bool  result = false;

	// use existing color map
	m_Colormap = null;
	if (m_depth == 15) return true;
	if (m_depth == 16) return true;
	if (m_depth == 24) return true;
	if (m_depth == 32) return true;
	if (m_depth !=  8) return false;

	display_mutex.b3Lock();
	if (cmap)
	{
		cmap_count++;
		m_Colormap = cmap;
		result     = true;
	}
	else
	{
		b3_index i,iMax;
		XColor   NewColors[256];

		// create color map
		cmap = XCreateColormap(
			m_Display,
			m_Window,
			DefaultVisual (m_Display,m_Screen),
			AllocAll);
		if (cmap != null)
		{
			// compute colors
			iMax = sizeof(NewColors) / sizeof(XColor);
			for (i = 0;i < iMax;i++)
			{
				NewColors[i].pixel =  i;
				NewColors[i].red   = (i & 0xe0) <<  8;
				NewColors[i].green = (i & 0x1c) << 11;
				NewColors[i].blue  = (i & 0x03) << 14;
				NewColors[i].flags = DoRed|DoGreen|DoBlue;
				NewColors[i].pad   = 0;
			}

			// init color map
			XStoreColors (m_Display,cmap,NewColors,iMax);
			cmap_count++;
			m_Colormap = cmap;
			result     = true;
		}
	}
	display_mutex.b3Unlock();

	return result;
}

void b3DisplayView::b3FreeColormap()
{
	display_mutex.b3Lock();
	if (--cmap_count <= 0)
	{
		if (m_depth == 8)
		{
			XFreeColormap (m_Display,m_Colormap);
			cmap = null;
		}
	}
	display_mutex.b3Unlock();
}

/*************************************************************************
**                                                                      **
**                        color access routines access                  **
**                                                                      **
*************************************************************************/

static b3_pkd_color dithermatrix[4][4] =
{
	{  0, 8, 1, 9 },
	{ 12, 4,13, 5 }, 
	{  3,11, 2,10 },
	{ 15, 7,14, 6 }
};

#define MASK0 0xff
#define MASK1 0xfe
#define MASK2 0xfc
#define MASK3 0xf8
#define MASK4 0xf0
#define MASK5 0xe0
#define MASK6 0xc0
#define MASK7 0x80

static unsigned long dithermask[8] =
{
	MASK0,MASK1,MASK2,MASK3,MASK4,MASK5,MASK6,MASK7
};

inline b3_bool b3DisplayView::b3Dither (
	b3_pkd_color Byte,
	b3_count     shift,
	b3_coord     x,
	b3_coord     y)
{
	b3_u32 mask;

	Byte &= 0xff;
	mask  = dithermask[shift];
	if ((Byte & mask) >= mask) return false;
	return (((Byte >> (shift - 4)) & 15) >= (dithermatrix[x & 3][y & 3]));
}

inline b3_bool b3DisplayView::b3Dither2 (b3_pkd_color Byte,b3_coord x,b3_coord y)
{
	Byte &= 0xff;
	if ((Byte & MASK2) >= MASK2) return false;
	return (((Byte << 2) & 15) >= (dithermatrix[x & 3][y & 3]));
}

inline b3_bool b3DisplayView::b3Dither3(b3_pkd_color Byte,b3_coord x,b3_coord y)
{
	Byte &= 0xff;
	if ((Byte & MASK3) >= MASK3) return false;
	return (((Byte << 1) & 15) >= (dithermatrix[x & 3][y & 3]));
}

inline b3_bool b3DisplayView::b3Dither5(b3_pkd_color Byte,b3_coord x,b3_coord y)
{
	Byte &= 0xff;
	if ((Byte & MASK5) >= MASK5) return false;
	return (((Byte >> 1) & 15) >= (dithermatrix[x & 3][y & 3]));
}

inline b3_bool b3DisplayView::b3Dither6(b3_pkd_color Byte,b3_coord x,b3_coord y)
{
	Byte &= 0xff;
	if ((Byte & MASK6) >= MASK6) return false;
	return (((Byte >> 2) & 15) >= (dithermatrix[x & 3][y & 3]));
}

inline b3_pkd_color b3DisplayView::b3ARGBtoPIXEL_08 (
	b3_pkd_color ARGB,
	b3_coord     x,
	b3_coord     y)
{
	b3_pkd_color r,g,b;

	r = ARGB & 0xe00000;
	g = ARGB & 0x00e000;
	b = ARGB & 0x0000c0;

	if (b3Dither5(ARGB >> 16,x,y)) r += 0x200000;
	if (b3Dither5(ARGB >>  8,x,y)) g += 0x002000;
	if (b3Dither6(ARGB      ,x,y)) b += 0x000040;

	return ((r >> 16) | (g >> 11) | (b >>  6));
}

inline b3_pkd_color b3DisplayView::b3ARGBtoPIXEL_15 (
	b3_pkd_color ARGB,
	b3_coord     x,
	b3_coord     y)
{
	b3_pkd_color r,g,b;

	r = ARGB & 0xf80000;
	g = ARGB & 0x00f800;
	b = ARGB & 0x0000f8;

	if (b3Dither3(ARGB >> 16,x,y)) r += 0x080000;
	if (b3Dither3(ARGB >>  8,x,y)) g += 0x000800;
	if (b3Dither3(ARGB      ,x,y)) b += 0x000008;

	return ((r >> 9) | (g >> 6) | (b >>  3));
}

inline b3_pkd_color b3DisplayView::b3ARGBtoPIXEL_16 (
	b3_pkd_color ARGB,
	b3_coord     x,
	b3_coord     y)
{
	b3_pkd_color r,g,b;

	r = ARGB & 0xf80000;
	g = ARGB & 0x00fc00;
	b = ARGB & 0x0000f8;

	if (b3Dither3(ARGB >> 16,x,y)) r += 0x080000;
	if (b3Dither2(ARGB >>  8,x,y)) g += 0x000400;
	if (b3Dither3(ARGB      ,x,y)) b += 0x000008;

	return ((r >> 8) | (g >> 5) | (b >>  3));
}

/*************************************************************************
**                                                                      **
**                        color display routines                        **
**                                                                      **
*************************************************************************/

void b3DisplayView::b3Open(b3_res xSize,b3_res ySize)
{
	XGCValues      Values;
	XEvent         report;
	b3_bool        Loop = true;
	XTextProperty  CInfoName;
	b3_res         xScr,yScr;

	if (m_Title == null)
	{
		m_Title = "Raytracen ist gut, Blizzard III ist noch besser...";
	}

	m_Display = XOpenDisplay(NULL);
	m_Screen  = DefaultScreen (m_Display);
	m_depth   = DefaultDepth  (m_Display,m_Screen);
	m_xMax    = xSize;
	m_yMax    = ySize;
	xScr      = DisplayWidth  (m_Display,m_Screen) - 20;
	yScr      = DisplayHeight (m_Display,m_Screen) - 15;

	m_xs      = B3_MIN(m_xMax,xScr);
	m_ys      = B3_MIN(m_yMax,yScr);
	m_Opened  = false;
	m_Closed  = false;

	Values.plane_mask    = AllPlanes;
	Values.function      = GXcopy;

#ifdef _DEBUG
	b3PrintF (B3LOG_NORMAL,"xMax:   %4ld\n",m_xMax);
	b3PrintF (B3LOG_NORMAL,"yMax:   %4ld\n",m_yMax);
	b3PrintF (B3LOG_NORMAL,"dep:    %4ld\n",m_depth);
	b3PrintF (B3LOG_NORMAL,"planes: %4ld\n",DisplayPlanes (m_Display,m_Screen));
#endif

	m_Buffer = (b3_pkd_color *)b3Alloc(sizeof(b3_pkd_color) * m_xMax * m_yMax);
	if (m_Buffer == null)
	{
		b3PrintF (B3LOG_NORMAL,"Blizzard III ERROR:\n");
		b3PrintF (B3LOG_NORMAL,"no memory for pixel buffer!\n");
		throw new b3DisplayException(B3_DISPLAY_MEMORY);
	}

	m_Window = XCreateSimpleWindow (
		m_Display,RootWindow (m_Display,m_Screen),
		0,0,m_xs,m_ys,0,
		WhitePixel (m_Display,m_Screen),
		BlackPixel (m_Display,m_Screen));

	XStringListToTextProperty(&m_Title,1,&CInfoName);
	XSetWMName(m_Display,m_Window,&CInfoName);


	m_Image = XCreatePixmap (m_Display,m_Window,m_xs,m_ys,m_depth);
	if (m_Image == null)
	{
		b3Free (m_Buffer);
		b3PrintF (B3LOG_NORMAL,"Blizzard III ERROR:\n");
		b3PrintF (B3LOG_NORMAL,"no memory for image buffer!\n");
		throw new b3DisplayException(B3_DISPLAY_MEMORY);
	}

	if (!b3CreateColormap())
	{
		b3Free(m_Buffer);
		b3PrintF (B3LOG_NORMAL,"Blizzard III ERROR:\n");
		b3PrintF (B3LOG_NORMAL,"no colormap available!\n");
		XFreePixmap (m_Display,m_Image);

		throw new b3DisplayException(B3_DISPLAY_NO_COLORMAP);
	}

	m_GC = XCreateGC (m_Display,m_Window,GCPlaneMask|GCFunction,&Values);

	XMapWindow          (m_Display,m_Window);
	XSetWindowColormap  (m_Display,m_Window,m_Colormap);
	XSelectInput        (m_Display,m_Window,
		ExposureMask|
		ButtonPressMask|
		KeyPressMask);


	// wait for window */
	do
	{
		XNextEvent (m_Display,&report);
		switch (report.type)
		{
			case Expose :
				if (report.xexpose.window == m_Window)
				{
					m_Opened = true;
					Loop     = false;
				}
				break;
		}
	}
	while (Loop);
}

void b3DisplayView::b3Close()
{
	if (m_Buffer != null) b3Free (m_Buffer);
	b3FreeColormap();
	XFreeGC       (m_Display,m_GC);
	XFreePixmap   (m_Display,m_Image);
	XDestroyWindow(m_Display,m_Window);
	XCloseDisplay (m_Display);
}

inline void b3DisplayView::b3FirstDrawing ()
{
	m_Opened = true;
}

inline void b3DisplayView::b3RefreshAll ()
{
	if (m_Opened)
	{
		m_Mutex.b3Lock();
		XCopyArea (m_Display,
			m_Image,m_Window,
			m_GC,0,0,
			m_xs,m_ys,0,0);
		m_Mutex.b3Unlock();
	}
}

b3DisplayView::b3DisplayView(const char *title)
{
	b3_coord xSize;
	b3_coord ySize;

#ifdef LOW_RES
	xSize = 240;
	ySize = 180;
#else
	xSize = 768;
	ySize = 576;
#endif
	m_Title = (char *)title;
	b3Open(xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu (default)\n",
		m_Title,
		m_xs,m_ys);
}

b3DisplayView::b3DisplayView(b3_res xSize,b3_res ySize,const char *title)
{
	m_Title = (char *)title;
	b3Open(xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu\n",
		m_Title,
		m_xs,m_ys);
}

b3DisplayView::~b3DisplayView()
{
	b3Close();
}

void b3DisplayView::b3PutRow(b3Row *row)
{
	b3_pkd_color *ptr = row->m_buffer;
	b3_coord      y = row->m_y;

	b3Display::b3PutRow(row);
	if (m_Opened && (y < m_ys))
	{
		m_Mutex.b3Lock();
		b3RefreshRow(y);
		XCopyArea (m_Display,m_Image,m_Window,m_GC,0,y,m_xs,1,0,y);
		m_Mutex.b3Unlock();
	}
}

void b3DisplayView::b3PutTx(b3Tx *tx)
{
	b3_coord y;

	b3Display::b3PutTx(tx);
	m_Mutex.b3Lock();
	for (y = 0;y < m_ys;y++)
	{
		b3RefreshRow(y);
	}
	XCopyArea (m_Display,m_Image,m_Window,m_GC,0,0,m_xs,m_ys,0,0);
	m_Mutex.b3Unlock();
}

inline void b3DisplayView::b3RefreshRow(b3_coord y)
{
	b3_pkd_color *ptr = &m_Buffer[y * m_xMax];
	b3_pkd_color  pixel;
	b3_coord      x;
	
	switch (m_depth)
	{
		case  8 :
			for (x = 0;x < m_xs;x++)
			{
				pixel = b3ARGBtoPIXEL_08 (ptr[x],x,y);
				XSetForeground (m_Display,m_GC,pixel);
				XDrawPoint     (m_Display,m_Image,m_GC,x,y);
			}
			break;
		case 15 :
			for (x = 0;x < m_xs;x++)
			{
				pixel = b3ARGBtoPIXEL_15 (ptr[x],x,y);
				XSetForeground (m_Display,m_GC,pixel);
				XDrawPoint     (m_Display,m_Image,m_GC,x,y);
			}
			break;
		case 16 :
			for (x = 0;x < m_xs;x++)
			{
				pixel = b3ARGBtoPIXEL_16 (ptr[x],x,y);
				XSetForeground (m_Display,m_GC,pixel);
				XDrawPoint     (m_Display,m_Image,m_GC,x,y);
			}
			break;
		default :
			for (x = 0;x < m_xs;x++)
			{
				XSetForeground (m_Display,m_GC,ptr[x]);
				XDrawPoint     (m_Display,m_Image,m_GC,x,y);
			}
			break;
	}
}

void b3DisplayView::b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color Color)
{
	b3_pkd_color pixel;

	b3Display::b3PutPixel(x,y,Color);

	if (m_Opened)
	{
		switch (m_depth)
		{
			case  8 :
				pixel = b3ARGBtoPIXEL_08 (Color,x,y);
				break;
			case 15 :
				pixel = b3ARGBtoPIXEL_15 (Color,x,y);
				break;
			case 16 :
				pixel = b3ARGBtoPIXEL_16 (Color,x,y);
				break;
			default :
				pixel = Color;
				break;
		}
		m_Mutex.b3Lock();
		XSetForeground (m_Display,m_GC,pixel);
		XDrawPoint     (m_Display,m_Window,m_GC,x,y);
		XDrawPoint     (m_Display,m_Image, m_GC,x,y);
		m_Mutex.b3Unlock();
	}
}

Bool b3DisplayView::b3SetPredicate (Display *display,XEvent *event,char *buffer)
{
	return True;
}

b3_bool b3DisplayView::b3IsCancelled(b3_coord x,b3_coord y)
{
	b3_bool	 loop = true,result=false,really_ask;
	XEvent	 report;

	display_mutex.b3Lock();
	if (--count > 0)
	{
		really_ask = false;
	}
	else
	{
		count      = READY;
		really_ask = true;
	}
	display_mutex.b3Unlock();

	if (!really_ask)
	{
		return false;
	}

	do	/* check pending events */
	{
		m_Mutex.b3Lock();
		if (XPending  (m_Display) <= 0)
		{
			m_Mutex.b3Unlock();
			return result;
		}
		XCheckIfEvent (m_Display,&report,&b3SetPredicate,0);
		m_Mutex.b3Unlock();
		switch (report.type)
		{
			case Expose :
				if (report.xexpose.window == m_Window)
				{
					b3FirstDrawing ();
					b3RefreshAll   ();
				}
				break;

			case ButtonPress:
				m_Closed = true;
				break;

			case KeyPress :
				result   = true;
				m_Closed = true;
				// Walk through!
			default :
				loop   = false;
				break;
		}
	}
	while (loop);

	return result;
}

void b3DisplayView::b3Wait()
{
	b3_bool	 loop = true;
	XEvent	 report;

	if (m_Closed)
	{
		// If already marked as closed.
		return;
	}

	do
	{
		XNextEvent (m_Display,&report);
		switch (report.type)
		{
			case Expose :
				if (report.xexpose.window == m_Window)
				{
					b3FirstDrawing ();
					b3RefreshAll   ();
				}
				break;

			case ButtonPress :
			case KeyPress :
				loop = false;
				m_Closed = true;
				break;
		}
	}
	while (loop);
}
