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

#define LOOP_B       4
#define LOOP_INSIDE (1 << LOOP_B)
#define LOOP_MASK   (LOOP_INSIDE - 1)

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/07 21:21:15  sm
**	- OK! Imported some display stuff using the CScrollView. After getting linked today
**	  it should possible to display real things tomorrow.
**
**	
*/

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

void b3LongMemSet(b3_u32 *data,b3_count max,b3_u32 value)
{
	b3_index  i;
	b3_count  long_max,short_max;

	// Compute loop sizes
	long_max  = max >> LOOP_B;
	short_max = max &  LOOP_MASK;

	// Long copy
	for (i = 0;i < long_max;i++)
	{
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
		*data++ = value;
	}

	// Copy rest
	for (i = 0;i < short_max;i++)
	{
		*data++ = value;
	}
}

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

b3_pkd_color b3RGB::operator ()()
{
	return (
		((b3_pkd_color)r << 16) |
		((b3_pkd_color)g <<  8) |
		 (b3_pkd_color)b);
}

b3RGB::operator b3_pkd_color()
{
	return (
		((b3_pkd_color)r << 16) |
		((b3_pkd_color)g <<  8) |
		 (b3_pkd_color)b);
}

void b3RGB::operator=(const b3_pkd_color &color)
{
	r = (b3_u08)((color & 0xff0000) >> 16);
	g = (b3_u08)((color & 0x00ff00) >>  8);
	b = (b3_u08)((color & 0x0000ff));
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
