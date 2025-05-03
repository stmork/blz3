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
	static const b3_pkd_color dithermatrix[4][4];
	static const b3_pkd_color dithermask[8];

protected:
	inline static b3_pkd_color b3Convert(b3_f32 value)
	{
		return value > 1 ? 255 : b3_pkd_color(value * 255);
	}

	inline static b3_bool b3Dither(
		b3_pkd_color Byte,
		b3_count     shift,
		b3_coord     x,
		b3_coord     y)
	{
		b3_u32 mask;

		Byte &= 0xff;
		mask  = dithermask[shift];
		if ((Byte & mask) >= mask)
		{
			return false;
		}
		return (((Byte >> (shift - 4)) & 15) >= (dithermatrix[x & 3][y & 3]));
	}

	inline static b3_bool b3Dither2(b3_pkd_color Byte, b3_coord x, b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK2) >= MASK2)
		{
			return false;
		}
		return (((Byte << 2) & 15) >= (dithermatrix[x & 3][y & 3]));
	}

	inline static b3_bool b3Dither3(b3_pkd_color Byte, b3_coord x, b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK3) >= MASK3)
		{
			return false;
		}
		return (((Byte << 1) & 15) >= (dithermatrix[x & 3][y & 3]));
	}

	inline static b3_bool b3Dither5(b3_pkd_color Byte, b3_coord x, b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK5) >= MASK5)
		{
			return false;
		}
		return (((Byte >> 1) & 15) >= (dithermatrix[x & 3][y & 3]));
	}

	inline static b3_bool b3Dither6(b3_pkd_color Byte, b3_coord x, b3_coord y)
	{
		Byte &= 0xff;
		if ((Byte & MASK6) >= MASK6)
		{
			return false;
		}
		return (((Byte >> 2) & 15) >= (dithermatrix[x & 3][y & 3]));
	}
};

const b3_pkd_color b3DisplayPixelImpl::dithermatrix[4][4] =
{
	{  0,  8,  1,  9 },
	{ 12,  4, 13,  5 },
	{  3, 11,  2, 10 },
	{ 15,  7, 14,  6 }
};

const b3_pkd_color b3DisplayPixelImpl::dithermask[8] =
{
	MASK0, MASK1, MASK2, MASK3, MASK4, MASK5, MASK6, MASK7
};

class b3DisplayPixel08 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel(
		b3_color     ARGB,
		b3_coord     x,
		b3_coord     y) override
	{
		b3_pkd_color r, g, b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		if (b3Dither5(r, x, y))
		{
			r += 0x20;
		}
		if (b3Dither5(g, x, y))
		{
			g += 0x20;
		}
		if (b3Dither6(b, x, y))
		{
			b += 0x40;
		}

		return (r | (g >> 3) | (b >>  6));
	}
};

class b3DisplayPixel15 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel(
		b3_color ARGB,
		b3_coord x,
		b3_coord y) override
	{
		b3_pkd_color r, g, b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		if (b3Dither3(r, x, y))
		{
			r += 0x08;
		}
		if (b3Dither3(g, x, y))
		{
			g += 0x08;
		}
		if (b3Dither3(b, x, y))
		{
			b += 0x08;
		}

		return (r << 7) | (g << 2) | (b >>  3);
	}
};

class b3DisplayPixel16 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel(
		b3_color ARGB,
		b3_coord x,
		b3_coord y) override
	{
		b3_pkd_color r, g, b;

		r = b3Convert(ARGB.r);
		g = b3Convert(ARGB.g);
		b = b3Convert(ARGB.b);

		if (b3Dither3(r, x, y))
		{
			r += 0x08;
		}
		if (b3Dither2(g, x, y))
		{
			g += 0x04;
		}
		if (b3Dither3(b, x, y))
		{
			b += 0x08;
		}

		return ((r << 8) | (g << 3) | (b >>  3));
	}
};

class b3DisplayPixel24 : public b3DisplayPixelImpl
{
public:
	b3_pkd_color b3ARGBtoPixel(
		b3_color ARGB,
		b3_coord x B3_UNUSED,
		b3_coord y B3_UNUSED) override
	{
		b3_pkd_color r, g, b;

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
		b3_coord x B3_UNUSED,
		b3_coord y B3_UNUSED) override
	{
		b3_pkd_color r, g, b;

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

#ifdef HAVE_LIBX11
Colormap b3DisplayView::m_Cmap;
b3_count b3DisplayView::m_CmapCount;
#endif

b3Mutex  b3DisplayView::m_DisplayMutex;
long     b3DisplayView::m_Count = READY;

b3DisplayView::b3DisplayView(const char * title) :
	b3Display(title), m_Title(title)
{
#ifdef HAVE_LIBX11
	b3Open(m_xMax, m_yMax);
#endif
	b3PrintF(B3LOG_FULL, "Opening display \"%s\" of size %u,%u (default)\n",
		m_Title,
		m_xs, m_ys);
}

b3DisplayView::b3DisplayView(
	const b3_res      xSize,
	const b3_res      ySize,
	const char    *   title) : b3Display(xSize, ySize, title), m_Title(title)
{
#ifdef HAVE_LIBX11
	b3Open(xSize, ySize);
#endif
	b3PrintF(B3LOG_FULL, "Opening display \"%s\" of size %u,%u\n",
		m_Title,
		m_xs, m_ys);
}

b3DisplayView::~b3DisplayView()
{
	if (m_Pixel != nullptr)
	{
		delete m_Pixel;
	}
#ifdef HAVE_LIBX11
	b3FreeColormap();
	XFreeGC(m_Display, m_GC);
	XFreePixmap(m_Display, m_Image);
	XDestroyWindow(m_Display, m_Window);
	XCloseDisplay(m_Display);
#endif
}

void b3DisplayView::b3PutPixel(
	const b3_coord   x,
	const b3_coord   y,
	const b3_color & Color)
{
#ifdef HAVE_LIBX11
	b3Display::b3PutPixel(x, y, Color);

	if (m_Opened)
	{
		b3CriticalSection lock(m_Mutex);

		XSetForeground(m_Display, m_GC, m_Pixel->b3ARGBtoPixel(Color, x, y));
		XDrawPoint(m_Display, m_Window, m_GC, x, y);
		XDrawPoint(m_Display, m_Image, m_GC, x, y);
	}
#else
	(void)x;
	(void)y;
	(void)Color;
#endif
}

b3_bool b3DisplayView::b3IsCancelled(
	const b3_coord x B3_UNUSED,
	const b3_coord y B3_UNUSED)
{
#ifdef HAVE_LIBX11
	b3_bool	 loop = true, result = false, really_ask;
	XEvent	 report;

	// CRITICAL SECTION
	{
		b3CriticalSection lock(m_DisplayMutex);

		if (--m_Count > 0)
		{
			really_ask = false;
		}
		else
		{
			m_Count    = READY;
			really_ask = true;
		}
	}

	if (!really_ask)
	{
		return false;
	}

	do	/* check pending events */
	{
		// CRITICAL SECTION
		{
			b3CriticalSection lock(m_Mutex);

			if (XPending(m_Display) <= 0)
			{
				return result;
			}
			XCheckIfEvent(m_Display, &report, &b3SetPredicate, 0);
		}

		switch (report.type)
		{
		case Expose :
			if (report.xexpose.window == m_Window)
			{
				b3FirstDrawing();
				b3RefreshAll();
			}
			break;

		case ButtonPress:
			m_Closed = true;
			break;

		case KeyPress :
			result   = true;
			m_Closed = true;
			B3_FALLTHROUGH;
		default :
			loop   = false;
			break;
		}
	}
	while (loop);

	return result;
#else
	return false;
#endif
}

void b3DisplayView::b3Wait()
{
#ifdef HAVE_LIBX11
	b3_bool	 loop = true;
	XEvent	 report;

	if (m_Closed)
	{
		// If already marked as closed.
		return;
	}

	do
	{
		XNextEvent(m_Display, &report);
		switch (report.type)
		{
		case Expose :
			if (report.xexpose.window == m_Window)
			{
				b3FirstDrawing();
				b3RefreshAll();
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
#endif
}

#ifdef HAVE_LIBX11
void b3DisplayView::b3Open(
	const b3_res xSize,
	const b3_res ySize)
{
#ifdef HAVE_LIBX11
	XGCValues      Values;
	XEvent         report;
	b3_bool        Loop = true;
	XTextProperty  CInfoName;
	b3_res         xScr, yScr;

	if (m_Title == nullptr)
	{
		m_Title = "Raytracen ist gut, Blizzard III ist noch besser...";
	}

	m_Display = XOpenDisplay(NULL);
	if (m_Display == 0)
	{
		B3_THROW(b3DisplayException, B3_DISPLAY_OPEN);
	}
	m_Screen  = DefaultScreen(m_Display);
	m_depth   = DefaultDepth(m_Display, m_Screen);

	m_xMax    = xSize;
	m_yMax    = ySize;
	xScr      = DisplayWidth(m_Display, m_Screen) - 20;
	yScr      = DisplayHeight(m_Display, m_Screen) - 15;

	m_xs      = std::min(m_xMax, xScr);
	m_ys      = std::min(m_yMax, yScr);
	m_Opened  = false;
	m_Closed  = false;

	Values.plane_mask    = AllPlanes;
	Values.function      = GXcopy;

#ifdef _DEBUG
	b3PrintF(B3LOG_NORMAL, "xMax:   %4d\n", m_xMax);
	b3PrintF(B3LOG_NORMAL, "yMax:   %4d\n", m_yMax);
	b3PrintF(B3LOG_NORMAL, "dep:    %4d\n", m_depth);
	b3PrintF(B3LOG_NORMAL, "planes: %4d\n", DisplayPlanes(m_Display, m_Screen));
#endif

	m_Window = XCreateSimpleWindow(
			m_Display, RootWindow(m_Display, m_Screen),
			0, 0, m_xs, m_ys, 0,
			WhitePixel(m_Display, m_Screen),
			BlackPixel(m_Display, m_Screen));

	XStringListToTextProperty(const_cast<char **>(&m_Title), 1, &CInfoName);
	XSetWMName(m_Display, m_Window, &CInfoName);


	m_Image = XCreatePixmap(m_Display, m_Window, m_xs, m_ys, m_depth);
	if (m_Image == 0)
	{
		b3Free(m_Buffer);
		b3PrintF(B3LOG_NORMAL, "Blizzard III ERROR:\n");
		b3PrintF(B3LOG_NORMAL, "no memory for image buffer!\n");
		B3_THROW(b3DisplayException, B3_DISPLAY_MEMORY);
	}

	if (!b3CreateColormap())
	{
		b3Free(m_Buffer);
		b3PrintF(B3LOG_NORMAL, "Blizzard III ERROR:\n");
		b3PrintF(B3LOG_NORMAL, "no colormap available!\n");
		XFreePixmap(m_Display, m_Image);

		B3_THROW(b3DisplayException, B3_DISPLAY_NO_COLORMAP);
	}

	m_GC = XCreateGC(m_Display, m_Window, GCPlaneMask | GCFunction, &Values);

	XMapWindow(m_Display, m_Window);
	XSetWindowColormap(m_Display, m_Window, m_Colormap);
	XSelectInput(m_Display, m_Window,
		ExposureMask |
		ButtonPressMask |
		KeyPressMask);

	// wait for window */
	XSetForeground(m_Display, m_GC, 0);
	XFillRectangle(m_Display, m_Image, m_GC, 0, 0, m_xs, m_ys);

	do
	{
		XNextEvent(m_Display, &report);
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
#endif
}

b3_bool b3DisplayView::b3CreateColormap()
{
#ifdef HAVE_LIBX11
	XVisualInfo * info;
	XVisualInfo   temp;
	b3_bool       result = false;
	int           count  = 0;

	// use existing color map
	m_Colormap = 0;
	m_Pixel    = nullptr;
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
		memset(&temp, 0, sizeof(temp));
		temp.visual   = DefaultVisual(m_Display, m_Screen);
		temp.visualid = XVisualIDFromVisual(temp.visual);
		temp.screen   = m_Screen;
		temp.depth    = m_depth;
		info = XGetVisualInfo(m_Display, VisualIDMask | VisualScreenMask | VisualDepthMask, &temp, &count);
		b3PrintF(B3LOG_FULL, "%d visuals (%p) found - depth: %d.\n",
			count, info, m_depth);
		for (int i = 0; i < count; i++)
		{
			b3PrintF(B3LOG_FULL, "%2d: %08lx %08lx %08lx # %4d %4d # %08x\n", i,
				info[i].red_mask,
				info[i].green_mask,
				info[i].blue_mask,
				info[i].colormap_size,
				info[i].bits_per_rgb,
				info[i].c_class);
		}

		if (info != nullptr)
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
		return m_Pixel != nullptr;

	default:
		b3PrintF(B3LOG_NORMAL, "Unsupported color depth!\n");
		return false;
	}

	b3CriticalSection lock(m_DisplayMutex);

	if (m_Cmap != 0)
	{
		m_CmapCount++;
		m_Colormap = m_Cmap;
		result     = true;
	}
	else
	{
		b3_loop i, iMax;
		XColor  NewColors[256];

		// create color map
		m_Cmap = XCreateColormap(
				m_Display,
				m_Window,
				DefaultVisual(m_Display, m_Screen),
				AllocAll);
		if (m_Cmap != 0)
		{
			// compute colors
			iMax = sizeof(NewColors) / sizeof(XColor);
			for (i = 0; i < iMax; i++)
			{
				NewColors[i].pixel =  i;
				NewColors[i].red   = (i & 0xe0) <<  8;
				NewColors[i].green = (i & 0x1c) << 11;
				NewColors[i].blue  = (i & 0x03) << 14;
				NewColors[i].flags = DoRed | DoGreen | DoBlue;
				NewColors[i].pad   = 0;
			}

			// init color map
			XStoreColors(m_Display, m_Cmap, NewColors, iMax);
			m_CmapCount++;
			m_Colormap = m_Cmap;
			result     = true;
		}
	}
#endif
	return result;
}

void b3DisplayView::b3FreeColormap()
{
	b3CriticalSection lock(m_DisplayMutex);

	if (--m_CmapCount <= 0)
	{
		if (m_depth == 8)
		{
			XFreeColormap(m_Display, m_Colormap);
			m_Cmap = 0;
		}
	}
}

inline void b3DisplayView::b3FirstDrawing()
{
	m_Opened = true;
}

inline void b3DisplayView::b3RefreshAll()
{
	if (m_Opened)
	{
		b3CriticalSection lock(m_Mutex);

		XCopyArea(m_Display,
			m_Image, m_Window,
			m_GC, 0, 0,
			m_xs, m_ys, 0, 0);
	}
}
#endif

void b3DisplayView::b3PutRow(const b3Row * row)
{
#ifdef HAVE_LIBX11
	b3_coord y = row->m_y;

	b3Display::b3PutRow(row);
	if (m_Opened && (y < m_ys))
	{
		b3CriticalSection lock(m_Mutex);

		b3RefreshRow(y);
		XCopyArea(m_Display, m_Image, m_Window, m_GC, 0, y, m_xs, 1, 0, y);
	}
#else
	(void)row;
#endif
}

void b3DisplayView::b3PutTx(b3Tx * tx)
{
#ifdef HAVE_LIBX11
	b3_coord y;

	b3Display::b3PutTx(tx);
	b3CriticalSection lock(m_Mutex);

	for (y = 0; y < m_ys; y++)
	{
		b3RefreshRow(y);
	}
	XCopyArea(m_Display, m_Image, m_Window, m_GC, 0, 0, m_xs, m_ys, 0, 0);
#else
	(void)tx;
#endif
}

#ifdef HAVE_LIBX11
inline void b3DisplayView::b3RefreshRow(const b3_coord y)
{
	b3_color * ptr = &m_Buffer[y * m_xMax];
	b3_coord   x;

	for (x = 0; x < m_xs; x++)
	{
		XSetForeground(m_Display, m_GC, m_Pixel->b3ARGBtoPixel(*ptr++, x, y));
		XDrawPoint(m_Display, m_Image, m_GC, x, y);
	}
}

Bool b3DisplayView::b3SetPredicate(
	Display * display B3_UNUSED,
	XEvent  * event   B3_UNUSED,
	char   *  buffer  B3_UNUSED)
{
	return True;
}
#endif
