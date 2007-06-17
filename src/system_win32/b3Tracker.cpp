/*
**
**	$Filename:	b3Tracker.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - tracker control
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

#include "b3SystemInclude.h"
#include "blz3/system/b3Tracker.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III b3Tracker                        **
**                                                                      **
*************************************************************************/

CB3Tracker::CB3Tracker(b3Tx *ref_tx)
{
	tx = ref_tx;
}

void CB3Tracker::b3Create(b3_bool mode)
{
	create = mode;
}

void CB3Tracker::AdjustRect(int nHandle,RECT *rect)
{
#if 1
	return;
#else
	long &x1 = rect->left;
	long &y1 = rect->top;
	long &x2 = rect->right;
	long &y2 = rect->bottom;
	long  xDiff,yDiff;
	long  xSize,ySize;

	xSize = tx->xSize;
	ySize = tx->ySize;
	xDiff = x2 - x1;
	yDiff = y2 - y1;

	// Preserve rectangle dimension only when moving
	if (nHandle == 8)
	{
		// Check width
		if (xDiff > xSize)
		{
			xDiff = xSize;
		}

		// Check height
		if (yDiff > ySize)
		{
			yDiff = ySize;
		}
	}

	// Left bound
	if (x1 < 0)
	{
		x1 = 0;
		if (nHandle == 8)
		{
			// To preserve rectangle size
			x2 = xDiff;
		}
	}
	if (create && (x2 < 0))
	{
		x2 = 0;
	}

	// Upper bound
	if (y1 < 0)
	{
		y1 = 0;
		if (nHandle == 8)
		{
			// To preserve rectangle size
			y2 = yDiff;
		}
	}
	if (create && (y2 < 0))
	{
		y2 = 0;
	}

	// Right bound
	if (x2 >= xSize)
	{
		x2 = xSize - 1;
		if (nHandle == 8)
		{
			// To preserve rectangle size
			x1 = x2 - xDiff;
		}
	}

	// Lower bound
	if (y2 >= ySize)
	{
		y2 = ySize - 1;
		if (nHandle == 8)
		{
			// To preserve rectangle size
			y1 = y2 - yDiff;
		}
	}
#endif
}
