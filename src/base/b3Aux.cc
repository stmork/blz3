/*
**
**	$Filename:	b3Aux.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some helpers
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Aux.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.6  2002/08/10 14:36:31  sm
**	- Some shapes had cleared the vertex array whenever the
**	  b3AllocVertices() method were called. Without calling
**	  b3Recomute() the shapes disoccured.
**	- Some methods moved as static methods into the
**	  b3Mem class.
**
**	Revision 1.5  2001/10/31 14:46:35  sm
**	- Filling b3IsCancelled() with sense.
**	- Inlining b3RGB
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
**	Revision 1.1  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**	
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

/*************************************************************************
**                                                                      **
**                    Blizzard III color representation                 **
**                                                                      **
*************************************************************************/

b3RGB::b3RGB()
{
	r = 0xff;
	g = 0x11;
	b = 0x44;
}

/*************************************************************************
**                                                                      **
**                    Blizzard III rect class                           **
**                                                                      **
*************************************************************************/

b3Rect::b3Rect()
{
	x1 = 0;
	y1 = 0;
	x2 = 0;
	y2 = 0;
}

b3_res b3Rect::b3Width()
{
	return x2 - x1;
}

b3_res b3Rect::b3Height()
{
	return y2 - y1;
}

b3_bool b3Rect::b3UpdateBound(b3Rect *rect)
{
	b3_bool changed = false;

	if (rect->x1 < x1)
	{
		x1 = rect->x1;
		changed = true;
	}
	if (rect->y1 < y1)
	{
		y1 = rect->y1;
		changed = true;
	}
	if (rect->x2 > x2)
	{
		x2 = rect->x2;
		changed = true;
	}
	if (rect->y2 > y2)
	{
		y2 = rect->y2;
		changed = true;
	}
	return changed;
}

b3_bool b3Rect::b3CheckBound(b3Rect *rect)
{
	bool changed = false;

	if (x1 < rect->x1)
	{
		x1 = rect->x1;
		changed = true;
	}
	if (y1 < rect->y1)
	{
		y1 = rect->y1;
		changed = true;
	}
	if (x2 > rect->x2)
	{
		x2 = rect->x2;
		changed = true;
	}
	if (y2 > rect->y2)
	{
		y2 = rect->y2;
		changed = true;
	}
	if (x1 > x2)
	{
		x1 = x2;
		changed = true;
	}
	if (y1 > y2)
	{
		y1 = y2;
		changed = true;
	}
	return changed;
}
