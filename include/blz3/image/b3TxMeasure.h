/*
**
**	$Filename:	b3TxMeasure.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - image measurements.
**
**	(C) Copyright 2005 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_IMAGE_TXMEASURE_H
#define B3_IMAGE_TXMEASURE_H

#include "blz3/b3Config.h"

/**
 * This class handles one image reference point.
 */
class B3_PLUGIN b3TxPoint
{
public:
	b3_coord x,y;

	b3TxPoint(b3_coord new_x = 0,b3_coord new_y = 0)
	{
		x = new_x;
		y = new_y;
	}
};

#define B3_MEASURE_EDGE 3

/**
 * This class is for image measurement.
 */
class B3_PLUGIN b3TxMeasure
{
public:
	b3_bool   valid;
	b3TxPoint left[B3_MEASURE_EDGE];
	b3TxPoint top[B3_MEASURE_EDGE];
	b3TxPoint right[B3_MEASURE_EDGE];
	b3TxPoint bottom[B3_MEASURE_EDGE];

	b3TxPoint points[(B3_MEASURE_EDGE - 1) * 4];
	b3_count  num;
public:
	     b3TxMeasure();
	void b3Init(b3_res xSize,b3_res ySize,b3_res depth);
	void b3Print();
};

#endif
