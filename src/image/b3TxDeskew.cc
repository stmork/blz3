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

#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/08/15 13:56:43  sm
**	- Introduced B3_THROW macro which supplies filename
**	  and line number of source code.
**	- Fixed b3AllocTx when allocating a zero sized image.
**	  This case is definitely an error!
**	- Added row refresh count into Lines
**
**	Revision 1.2  2002/03/13 19:01:58  sm
**	- Fixed some GCC warnings.
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
**	Revision 1.3  2000/09/21 10:22:27  smork
**	- Setting Blizzard III projects to warning: Found
**	  some uninitialized variables.
**	- changed b3Mutex from CMutex to CRITICAL_SECTION (thread
**	  synchronization only)
**	- introduced b3IPCMutex for process synchronization
**	
**	Revision 1.2  2000/08/14 11:11:23  smork
**	- Inserted change log
**	- Inserted author tag
**	- Cleaned up file header
**	
*/

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
		B3_THROW(b3TxException,B3_TX_UNKNOWN_DATATYPE);
	}
}

void b3Tx::b3DeskewILBM()
{
	b3_index  xBytes,i;
	b3_coord  x,y;
	b3_u08   *cPtr;
	b3_bool   isBlack;

	if (!b3IsBW())
	{
		return;
	}

	xBytes = TX_BWA(xSize);
	for (i = 0;i < B3_MEASURE_EDGE;i++)
	{
		unsigned long index;

		// Search for first 0-Bit from left to right
		isBlack = true;
		index   = measure.left[i].y * xBytes;
		cPtr    = &data[index];
		isBlack = true;
		for (x = measure.left[i].x;(x < xSize) && (isBlack);x += 8)
		{
			if (cPtr[x >> 3] == 255)
			{
				isBlack = true;
			}
			else
			{
				b3_u08 byte;
				unsigned long bit = 128;

				for (byte = cPtr[x >> 3];byte & bit;bit = bit >> 1)
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
		cPtr    = &data[index];
		for (x = xSize;x & 7;x++)
		{
			cPtr[x >> 3] |= (128 >> (x & 7));
		}
		while ((x >= 0) && (isBlack))
		{
			x -= 8;
			if (cPtr[x >> 3] == 255)
			{
				isBlack = true;
			}
			else
			{
				unsigned long bit = 1,byte;

				x += 7;
				for (byte = cPtr[x >> 3];byte & bit;bit = bit << 1)
				{
					x--;
				}
				isBlack = false;
			}
		}
		measure.right[i].x = x;

		// Search for first 0-Bit from top to bottom
		index = measure.top[i].y * xBytes + (measure.top[i].x >> 3);
		cPtr  = &data[index];
		for (y = measure.top[i].y;(cPtr[0] & 128) && (y < ySize);y++)
		{
			y++;
			cPtr += xBytes;
		}
		measure.top[i].y = y;

		// Search for first 0-Bit from bottom to top
		index = measure.bottom[i].y * xBytes + (measure.bottom[i].x >> 3);
		cPtr  = &data[index];
		for (y = measure.bottom[i].y;(cPtr[0] & 128) && (y >= 0);y--)
		{
			cPtr -= xBytes;
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
