/*
**
**	$Filename:	b3Measure.c $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - measuring an image for deskewing
**
**	(C) Copyright 2001  Steffen A. mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                        b3Measure routines                            **
**                                                                      **
*************************************************************************/

b3TxMeasure::b3TxMeasure()
{
	num = 0;
}

void b3TxMeasure::b3Init(
	b3_res xSize,
	b3_res ySize,
	b3_res depth)
{
	b3_index i,oct,mask;

	for (i = 0;i < 3;i++)
	{
		top[i].x    = top[i].y    = 0;
		left[i].x   = left[i].y   = 0;
		right[i].x  = right[i].y  = 0;
		bottom[i].x = bottom[i].y = 0;
	}

	// Is measurement possible?
	valid = false;
	num   = 0;
	if (depth == 1)
	{
		mask = 0xfffffff8;
		if ((xSize < 64) || (ySize < 64))
		{
			return;
		}
	}
	else
	{
		mask = 0xffffffff;
		if ((xSize < 8) || (ySize < 8))
		{
			return;
		}
	}

	// Setting up measure points
	oct = xSize >> 3;
	top[0].x = bottom[0].x = oct & mask;
	top[1].x = bottom[1].x = (xSize >> 1)  & mask;
	top[2].x = bottom[2].x = (xSize - oct) & mask;

	oct = ySize >> 3;
	left[0].y = right[0].y = oct;
	left[1].y = right[1].y = ySize >> 1;
	left[2].y = right[2].y = ySize - oct;

	for (i = 0;i < 3;i++)
	{
		right[i].x  = xSize - 1;
		bottom[i].y = ySize - 1;
	}

	valid = true;
}

void b3TxMeasure::b3Print()
{
	unsigned long i;

	b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # actual measurement:\n");

	b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # top:\n");
	for (i = 0;i < 3;i++)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # %4ld x %4ld\n",
			top[i].x,   top[i].y);
	}

	b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # left:\n");
	for (i = 0;i < 3;i++)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # %4ld x %4ld\n",
			left[i].x,  left[i].y);
	}

	b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # right:\n");
	for (i = 0;i < 3;i++)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # %4ld x %4ld\n",
			right[i].x, right[i].y);
	}

	b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # bottom:\n");
	for (i = 0;i < 3;i++)
	{
		b3PrintF (B3LOG_FULL,"### CLASS: b3Meas # %4ld x %4ld\n",
			bottom[i].x,bottom[i].y);
	}
}
