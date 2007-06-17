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

#include "b3BaseInclude.h"
#include "blz3/base/b3Aux.h"

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
