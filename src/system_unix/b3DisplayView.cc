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
#include "blz3/system/b3DisplayView.h"
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
**	Revision 1.13  2005/10/09 12:05:34  sm
**	- Changed to HDR image computation.
**
**	Revision 1.12  2003/11/25 13:31:47  sm
**	- Changed b3_loop to int (best performance)
**	
**	Revision 1.11  2003/09/01 19:03:34  sm
**	- Removed debug output
**	
**	Revision 1.10  2003/09/01 11:18:55  sm
**	- Fixed ticket no. 9: A missing DISPLAY variable doesn't crash
**	  the apps.
**	
**	Revision 1.9  2002/08/29 16:25:38  sm
**	- Added RGB and BGR color support which fixes the red/blue swap
**	  problem on Sun machines.
**	
**	Revision 1.8  2002/08/15 13:56:44  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**	
**	Revision 1.7  2002/08/11 11:03:40  sm
**	- Moved b3Display and b3Row classes from base lib into system
**	  independend lib.
**	- Made b3TimeSpan more system independend;-)
**	
**	Revision 1.6  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.5  2002/08/05 17:42:58  sm
**	- Displaying brt3 options.
**	- Clearing XBuffer which displayed garbage from previous X applications
**	  (This is a security leak inside X!!!)
**	
**	Revision 1.4  2002/03/13 19:01:59  sm
**	- Fixed some GCC warnings.
**	
**	Revision 1.3  2002/01/22 17:11:18  sm
**	- brt3 is now able to save images. The selection of image type
**	  is unsoved yet.
**	- Better b3DisplayView in Un*x port.
**	- Fixed stricmp() in Un*x port.
**	
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
**                        color access routines access                  **
**                                                                      **
*************************************************************************/

#define MASK0 0xff
#define MASK1 0xfe
#define MASK2 0xfc
#define MASK3 0xf8
#define MASK4 0xf0
#define MASK5 0xe0
#define MASK6 0xc0
#define MASK7 0x80

class b3DisplayPixelImpl : public b3DisplayPixel
{
	static b3_pkd_color dithermatrix[4][4];
	static b3_pkd_color dithermask[8];

protected:
	inline static b3_pkd_color b3Convert(b3_f32 value)
	{
		return value > 1 ? 255 : value * 255;
	}

	inline static b3_bool b3Dither (
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

	inline static b3_bool b3Dither2 (b3_pkd_color Byte,b3_coord x,b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK2) >= MASK2) return false;
		return (((Byte << 2) & 15) >= (dithermatrix[x & 3][y & 3]));
	}

	inline static b3_bool b3Dither3(b3_pkd_color Byte,b3_coord x,b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK3) >= MASK3) return false;
		return (((Byte << 1) & 15) >= (dithermatrix[x & 3][y & 3]));
	}

	inline static b3_bool b3Dither5(b3_pkd_color Byte,b3_coord x,b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK5) >= MASK5) return false;
		return (((Byte >> 1) & 15) >= (dithermatrix[x & 3][y & 3]));
	}

	inline static b3_bool b3Dither6(b3_pkd_color Byte,b3_coord x,b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK6) >= MASK6) return false;
		return (((Byte >> 2) & 15) >= (dithermatrix[x & 3][y & 3]));
	}
};

b3_pkd_color b3DisplayPixelImpl::dithermatrix[4][4] =
{
	{  0, 8, 1, 9 },
	{ 12, 4,13, 5 }, 
	{  3,11, 2,10 },
	{ 15, 7,14, 6 }
};

b3_pkd_color b3DisplayPixelImpl::dithermask[8] =
{
	MASK0,MASK1,MASK2,MASK3,MASK4,MASK5,MASK6,MASK7
};

class b3DisplayPixel08 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel (
		b3_color     ARGB,
		b3_coord     x,
		b3_coord     y)
	{
		b3_pkd_color r,g,b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		if (b3Dither5(r, x, y)) r += 0x20;
		if (b3Dither5(g, x, y)) g += 0x20;
		if (b3Dither6(b, x, y)) b += 0x40;

		return (r | (g >> 3) | (b >>  6));
	}
};

class b3DisplayPixel15 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel (
		b3_color ARGB,
		b3_coord x,
		b3_coord y)
	{
		b3_pkd_color r,g,b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		if (b3Dither3(r, x, y)) r += 0x08;
		if (b3Dither3(g, x, y)) g += 0x08;
		if (b3Dither3(b, x, y)) b += 0x08;

		return (r << 7) | (g << 2) | (b >>  3);
	}
};

class b3DisplayPixel16 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel (
		b3_color ARGB,
		b3_coord x,
		b3_coord y)
	{
		b3_pkd_color r,g,b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		if (b3Dither3(r, x, y)) r += 0x08;
		if (b3Dither2(g, x, y)) g += 0x04;
		if (b3Dither3(b, x, y)) b += 0x08;

		return ((r << 8) | (g << 3) | (b >>  3));
	}
};

class b3DisplayPixel24 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel(
		b3_color ARGB,
		b3_coord x,
		b3_coord y)
	{
		b3_pkd_color r,g,b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		return (r << 16) | (g << 8) | b;
	}
};

class b3DisplayPixel24Inv : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel(
		b3_color ARGB,
		b3_coord x,
		b3_coord y)
	{
		b3_pkd_color r,g,b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		return r | (g << 8) | (b << 16);
	}
};

/*************************************************************************
**                                                                      **
**                        color display routines                        **
**                                                                      **
*************************************************************************/

static b3Mutex  display_mutex;
static Colormap cmap;
static b3_count cmap_count;
static long     count = READY;

b3DisplayView::b3DisplayView(const char *title) : b3Display(title)
{
	m_Title = (char *)title;
	b3Open(m_xMax,m_yMax);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu (default)\n",
		m_Title,
		m_xs,m_ys);
}

b3DisplayView::b3DisplayView(
	b3_res      xSize,
	b3_res      ySize,
	const char *title) : b3Display(xSize,ySize,title)
{
	m_Title = (char *)title;
	b3Open(xSize,ySize);
	b3PrintF (B3LOG_FULL,"Opening display \"%s\" of size %lu,%lu\n",
		m_Title,
		m_xs,m_ys);
}

b3DisplayView::~b3DisplayView()
{
	if (m_Pixel != null)
	{
		delete m_Pixel;
	}
	b3FreeColormap();
	XFreeGC       (m_Display,m_GC);
	XFreePixmap   (m_Display,m_Image);
	XDestroyWindow(m_Display,m_Window);
	XCloseDisplay (m_Display);
}

void b3DisplayView::b3Open(
	b3_res xSize,
	b3_res ySize)
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
	if (m_Display == 0)
	{
		B3_THROW(b3DisplayException,B3_DISPLAY_OPEN);
	}
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
		B3_THROW(b3DisplayException,B3_DISPLAY_MEMORY);
	}

	if (!b3CreateColormap())
	{
		b3Free(m_Buffer);
		b3PrintF (B3LOG_NORMAL,"Blizzard III ERROR:\n");
		b3PrintF (B3LOG_NORMAL,"no colormap available!\n");
		XFreePixmap (m_Display,m_Image);

		B3_THROW(b3DisplayException,B3_DISPLAY_NO_COLORMAP);
	}

	m_GC = XCreateGC (m_Display,m_Window,GCPlaneMask|GCFunction,&Values);

	XMapWindow          (m_Display,m_Window);
	XSetWindowColormap  (m_Display,m_Window,m_Colormap);
	XSelectInput        (m_Display,m_Window,
		ExposureMask|
		ButtonPressMask|
		KeyPressMask);

	// wait for window */
	XSetForeground(m_Display,m_GC,0);
	XFillRectangle(m_Display,m_Image, m_GC,0,0,m_xs,m_ys);

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

b3_bool b3DisplayView::b3CreateColormap ()
{
	XVisualInfo *info;
	XVisualInfo  temp;
	b3_bool      result = false;
	int          count,i;

	// use existing color map
	m_Colormap = null;
	m_Pixel    = null;
	switch (m_depth)
	{
	case  8:
		m_Pixel = new b3DisplayPixel08();
		break;
		// No return since we have to create a colormap!

	case 15:
		m_Pixel = new b3DisplayPixel15();
		return true;

	case 16:
		m_Pixel = new b3DisplayPixel16();
		return true;

	case 24:
	case 32:
		memset(&temp,0,sizeof(temp));
		temp.visual   = DefaultVisual(m_Display,m_Screen);
		temp.visualid = XVisualIDFromVisual(temp.visual);
		temp.screen   = m_Screen;
		temp.depth    = m_depth;
		info = XGetVisualInfo(m_Display,VisualIDMask | VisualScreenMask | VisualDepthMask,&temp,&count);
		b3PrintF(B3LOG_FULL,"%d visuals (%p) found - depth: %d.\n",count,info,m_depth);
		for (i = 0;i < count;i++)
		{
			b3PrintF(B3LOG_FULL,"%2d: %08x %08x %08x # %4d %4d # %08x\n",i,
				info[i].red_mask,
				info[i].green_mask,
				info[i].blue_mask,
				info[i].colormap_size,
				info[i].bits_per_rgb,
				info[i].c_class);
		}

		if (info != null)
		{
			if ((info->red_mask  == 0x0000ff) &&
			    (info->blue_mask == 0xff0000))
			{
				// BGR format (e.g. Sun Ultra 10)
				m_Pixel = new b3DisplayPixel24Inv();
			}
			if ((info->red_mask  == 0xff0000) &&
			    (info->blue_mask == 0x0000ff))
			{
				// RGB format (Blizzard III native)
				m_Pixel = new b3DisplayPixel24();
			}
			
			XFree(info);
		}
		return m_Pixel != null;
		
	default:
		b3PrintF(B3LOG_NORMAL,"Unsupported color depth!\n");
		return false;
	}

	display_mutex.b3Lock();
	if (cmap != null)
	{
		cmap_count++;
		m_Colormap = cmap;
		result     = true;
	}
	else
	{
		b3_loop i,iMax;
		XColor  NewColors[256];

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

void b3DisplayView::b3PutRow(b3Row *row)
{
	b3_coord y = row->m_y;

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
	b3_color *ptr = &m_Buffer[y * m_xMax];
	b3_coord  x;
	
	for (x = 0;x < m_xs;x++)
	{
		XSetForeground (m_Display,m_GC,m_Pixel->b3ARGBtoPixel(*ptr++,x,y));
		XDrawPoint     (m_Display,m_Image,m_GC,x,y);
	}
}

void b3DisplayView::b3PutPixel(b3_coord x,b3_coord y,b3_color Color)
{
	b3Display::b3PutPixel(x,y,Color);

	if (m_Opened)
	{
		m_Mutex.b3Lock();
		XSetForeground (m_Display,m_GC,m_Pixel->b3ARGBtoPixel (Color,x,y));
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
