/*
**
**      $Filename:	b3Display.h   $
**      $Release:	Dortmund 2001 $
**      $Revision:	1.07 
**      $Date$
**      $Developer:	Steffen A. Mork $
**      $Id$
**
**      Blizzard III - Display Hardware Routines
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
*/

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

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

/*************************************************************************
**                                                                      **
**                        typedefs...                                   **
**                                                                      **
*************************************************************************/

class b3DisplayView : public b3Display
{
	b3_res        m_xs,m_ys; // This is the max. visible size
	char         *m_Title;

	// Some X values
	b3_bool       m_Opened;
	b3_bool       m_Closed;
	Display      *m_Display;
	int           m_Screen;
	Window        m_Window;
	Colormap      m_Colormap;
	Pixmap        m_Image;
	GC            m_GC;

public:
	                    b3DisplayView(const char *title = null);
	                    b3DisplayView(b3_res xSize,b3_res ySize,const char *title = null);
	                   ~b3DisplayView();
	void                b3PutPixel(b3_coord x,b3_coord y,b3_pkd_color pixel);
	void                b3PutRow(b3Row *row);
	void                b3PutTx(b3Tx *tx);
	b3_bool             b3IsCancelled(b3_coord x,b3_coord y);
	void                b3Wait();

private:
	       void         b3RefreshRow(b3_coord y);
	static b3_bool      b3Dither( b3_pkd_color Byte,b3_count shift,b3_coord x,b3_coord y);
	static b3_bool      b3Dither2(b3_pkd_color Byte,b3_coord x,b3_coord y);
	static b3_bool      b3Dither3(b3_pkd_color Byte,b3_coord x,b3_coord y);
	static b3_bool      b3Dither5(b3_pkd_color Byte,b3_coord x,b3_coord y);
	static b3_bool      b3Dither6(b3_pkd_color Byte,b3_coord x,b3_coord y);
	static b3_pkd_color b3ARGBtoPIXEL_08(b3_pkd_color color,b3_coord x,b3_coord y);
	static b3_pkd_color b3ARGBtoPIXEL_15(b3_pkd_color color,b3_coord x,b3_coord y);
	static b3_pkd_color b3ARGBtoPIXEL_16(b3_pkd_color color,b3_coord x,b3_coord y);

	       void         b3Open(b3_res xSize,b3_res ySize);
	       b3_bool      b3CreateColormap();
	       void         b3FreeColormap();
	static Bool         b3SetPredicate(Display *display,XEvent *event,char *buffer);
	       void         b3FirstDrawing();
	       void         b3RefreshAll();
};

#endif
