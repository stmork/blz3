/*
**
**	$Filename:	b3DisplayView.h   $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision:	1.07
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Display Hardware Routines
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_DISPLAYVIEW_H
#define B3_SYSTEM_DISPLAYVIEW_H

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3Display.h"

/*************************************************************************
**                                                                      **
**                        X11 includes                                  **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBX11
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif

/*************************************************************************
**                                                                      **
**                        typedefs...                                   **
**                                                                      **
*************************************************************************/

/**
 * This abstract class provides a method for converting a pixel of
 * type  <em>b3_pkd_pixel</em> into a system specific color value.
 */
class b3DisplayPixel
{
public:
	virtual ~b3DisplayPixel() = default;

	/**
	 * This method converts a given color value at the given position into
	 * a native color value.
	 *
	 * @param ARGB The color in AARRGGBB nibbles.
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @return The native color value.
	 */
	virtual b3_pkd_color b3ARGBtoPixel(b3_color ARGB, b3_coord x, b3_coord y) = 0;
};

/**
 * This class implements the b3Display class for a X11 display.
 *
 * @see b3Display
 */
class b3DisplayView : public b3Display
{
	b3_res           m_xs, m_ys; // This is the max. visible size
	const char   *   m_Title;
	b3DisplayPixel * m_Pixel;

	// Some X values
	b3_bool         m_Opened;
	b3_bool         m_Closed;
#ifdef HAVE_LIBX11
	Display    *    m_Display;
	int             m_Screen;
	Window          m_Window;
	Colormap        m_Colormap;
	Pixmap          m_Image;
	GC              m_GC;
	static Colormap m_Cmap;
	static b3_count m_CmapCount;
#endif

	static b3Mutex  m_DisplayMutex;
	static long     m_Count;

public:
	/**
	 * This constructor opens a window with the given title in a default resolution.
	 *
	 * @param title The window title.
	 */
	explicit b3DisplayView(const char * title = nullptr);

	/**
	 * This constructor opens a window with the given resolution and the given window
	 * title.
	 *
	 * @param xSize The window width.
	 * @param ySize The window height.
	 * @param title The window title.
	 */
	explicit b3DisplayView(const b3_res xSize, const b3_res ySize, const char * title = nullptr);

	/**
	 * This destructor closes the window.
	 */
	virtual ~b3DisplayView();

	void                b3PutPixel(const b3_coord x, const b3_coord y, const b3_color & pixel) override;
	void                b3PutRow(const b3Row * row) override;
	void                b3PutTx(b3Tx * tx) override;
	b3_bool             b3IsCancelled(const b3_coord x, const b3_coord y) override;
	void                b3Wait() override;

private:
#ifdef HAVE_LIBX11
	void         b3RefreshRow(const b3_coord y);

	void         b3Open(const b3_res xSize, const b3_res ySize);
	b3_bool      b3CreateColormap();
	void         b3FreeColormap();
	static Bool         b3SetPredicate(Display * display, XEvent * event, char * buffer);
	void         b3FirstDrawing();
	void         b3RefreshAll();
#endif
};

#endif

