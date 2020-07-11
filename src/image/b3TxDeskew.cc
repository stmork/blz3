/*
**
**	$Filename:	b3TxDeskew.c $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - image deskewing
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

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                        b3Tx deskew                                   **
**                                                                      **
*************************************************************************/

void b3Tx::b3Deskew()
{
	switch (type)
	{
	case B3_TX_ILBM:
		b3DeskewILBM();
		break;
	case B3_TX_RGB4:
		b3DeskewRGB4();
		break;
	case B3_TX_RGB8:
		b3DeskewRGB8();
		break;
	case B3_TX_VGA:
		b3DeskewVGA();
		break;

	default:
		B3_THROW(b3TxException, B3_TX_UNKNOWN_DATATYPE);
	}
}

void b3Tx::b3DeskewILBM()
{
	b3TxMeasure  measure;
	b3_index     xBytes, i;
	b3_coord     x, y;
	b3_u08   *   bPtr;
	b3_bool      isBlack;

	if (!b3IsBW())
	{
		return;
	}

	xBytes = TX_BWA(xSize);
	for (i = 0; i < B3_MEASURE_EDGE; i++)
	{
		unsigned long index;

		// Search for first 0-Bit from left to right
		isBlack = true;
		index   = measure.left[i].y * xBytes;
		bPtr    = &data[index];
		isBlack = true;
		for (x = measure.left[i].x; (x < xSize) && (isBlack); x += 8)
		{
			if (bPtr[x >> 3] == 255)
			{
				isBlack = true;
			}
			else
			{
				b3_u08 byte;
				unsigned long bit = 128;

				for (byte = bPtr[x >> 3]; byte & bit; bit = bit >> 1)
				{
					x++;
				}
				isBlack = false;
				x -= 8;
			}
		}
		measure.left[i].x = x;

		// Search for first 0-Bit from right to left
		isBlack = true;
		index   = measure.right[i].y * xBytes;
		bPtr    = &data[index];
		for (x = xSize; x & 7; x++)
		{
			bPtr[x >> 3] |= (128 >> (x & 7));
		}
		while ((x >= 0) && (isBlack))
		{
			x -= 8;
			if (bPtr[x >> 3] == 255)
			{
				isBlack = true;
			}
			else
			{
				unsigned long bit = 1, byte;

				x += 7;
				for (byte = bPtr[x >> 3]; byte & bit; bit = bit << 1)
				{
					x--;
				}
				isBlack = false;
			}
		}
		measure.right[i].x = x;

		// Search for first 0-Bit from top to bottom
		index = measure.top[i].y * xBytes + (measure.top[i].x >> 3);
		bPtr  = &data[index];
		for (y = measure.top[i].y; (bPtr[0] & 128) && (y < ySize); y++)
		{
			y++;
			bPtr += xBytes;
		}
		measure.top[i].y = y;

		// Search for first 0-Bit from bottom to top
		index = measure.bottom[i].y * xBytes + (measure.bottom[i].x >> 3);
		bPtr  = &data[index];
		for (y = measure.bottom[i].y; (bPtr[0] & 128) && (y >= 0); y--)
		{
			bPtr -= xBytes;
		}
		measure.bottom[i].y = y;
	}
	measure.b3Print();
}

void b3Tx::b3DeskewRGB4()
{
}

void b3Tx::b3DeskewRGB8()
{
}

void b3Tx::b3DeskewVGA()
{
}
