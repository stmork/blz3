/*
**
**	$Filename:	b3TxMirror.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (mirroring)
**
**	(C) Copyright 2004  Steffen A. Mork
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
**	Revision 1.1  2004/08/03 10:46:26  sm
**	- Added simgle frame to DivX/AVI conversion tool
**	- Added image mirror (not completely implemented yet)
**
**
*/

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

void b3Tx::b3MirrorHorizontal()
{
	switch(type)
	{
	default:
		B3_THROW(b3TxException,B3_TX_UNSUPP);
	}
}

void b3Tx::b3MirrorVertical()
{
	b3_coord      x,y,yMax = ySize >> 1;
	b3_pkd_color *ltPtr,*lbPtr,lColor;
	b3_u16       *stPtr,*sbPtr,sColor;
	b3_u08       *ctPtr,*cbPtr,cColor;

	switch(type)
	{
	case B3_TX_RGB8:
		ltPtr = (b3_pkd_color *)data;
		lbPtr = &ltPtr[xSize * ySize];
		for (y = 0;y < yMax;y++)
		{
			lbPtr -= xSize;
			for (x = 0;x < xSize;x++)
			{
				lColor   = ltPtr[x];
				ltPtr[x] = lbPtr[x];
				lbPtr[x] = lColor;
			}
			ltPtr += xSize;
		}
		break;

	case B3_TX_RGB4:
		stPtr = (b3_u16 *)data;
		sbPtr = &stPtr[xSize * ySize];
		for (y = 0;y < yMax;y++)
		{
			sbPtr -= xSize;
			for (x = 0;x < xSize;x++)
			{
				sColor   = stPtr[x];
				stPtr[x] = sbPtr[x];
				sbPtr[x] = sColor;
			}
			stPtr += xSize;
		}
		break;

	case B3_TX_VGA:
		ctPtr = (b3_u08 *)data;
		cbPtr = &ctPtr[xSize * ySize];
		for (y = 0;y < yMax;y++)
		{
			cbPtr -= xSize;
			for (x = 0;x < xSize;x++)
			{
				cColor   = ctPtr[x];
				ctPtr[x] = cbPtr[x];
				cbPtr[x] = cColor;
			}
			ctPtr += xSize;
		}
		break;

	default:
		B3_THROW(b3TxException,B3_TX_UNSUPP);
	}
}
