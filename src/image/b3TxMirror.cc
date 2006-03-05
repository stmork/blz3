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

#include "b3TxInclude.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2006/03/05 21:22:34  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.3  2005/10/09 15:06:47  sm
**	- Added HDR image processing
**	
**	Revision 1.2  2004/08/28 13:55:33  sm
**	- Added some mirror methods.
**	- Cleanup job.
**	
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
	b3_coord      x,y,xMax = xSize >> 1;
	b3_color     *ctPtr,*cbPtr,cColor;
	b3_pkd_color *ltPtr,*lbPtr,lColor;
	b3_u16       *stPtr,*sbPtr,sColor;
	b3_u08       *btPtr,*bbPtr,bColor;

	// Untested, yet.
	switch(type)
	{
	case B3_TX_FLOAT:
		cbPtr = (b3_color *)data;
		ctPtr = &cbPtr[xSize];
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xMax;x++)
			{
				ctPtr--;
				 cColor   = *ctPtr;
				*ctPtr    =  cbPtr[x];
				 cbPtr[x] =  cColor;
			}
			cbPtr += xSize;
		}
		break;

	case B3_TX_RGB8:
		lbPtr = (b3_pkd_color *)data;
		ltPtr = &lbPtr[xSize];
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xMax;x++)
			{
				ltPtr--;
				 lColor   = *ltPtr;
				*ltPtr    =  lbPtr[x];
				 lbPtr[x] =  lColor;
			}
			lbPtr += xSize;
		}
		break;

	case B3_TX_RGB4:
		sbPtr = (b3_u16 *)data;
		stPtr = &sbPtr[xSize];
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xMax;x++)
			{
				stPtr--;
				 sColor   = *stPtr;
				*stPtr    =  sbPtr[x];
				 sbPtr[x] =  sColor;
			}
			sbPtr += xSize;
		}
		break;

	case B3_TX_VGA:
		bbPtr = (b3_u08 *)data;
		btPtr = &bbPtr[xSize];
		for (y = 0;y < ySize;y++)
		{
			for (x = 0;x < xMax;x++)
			{
				btPtr--;
				 bColor   = *btPtr;
				*btPtr    =  bbPtr[x];
				 bbPtr[x] =  bColor;
			}
			bbPtr += xSize;
		}
		break;

	default:
		B3_THROW(b3TxException,B3_TX_UNSUPP);
	}
}

void b3Tx::b3MirrorVertical()
{
	b3_coord      x,y,yMax = ySize >> 1;
	b3_color     *ctPtr,*cbPtr,cColor;
	b3_pkd_color *ltPtr,*lbPtr,lColor;
	b3_u16       *stPtr,*sbPtr,sColor;
	b3_u08       *btPtr,*bbPtr,bColor;

	switch(type)
	{
	case B3_TX_FLOAT:
		ctPtr = (b3_color *)data;
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
		btPtr = (b3_u08 *)data;
		bbPtr = &btPtr[xSize * ySize];
		for (y = 0;y < yMax;y++)
		{
			bbPtr -= xSize;
			for (x = 0;x < xSize;x++)
			{
				bColor   = btPtr[x];
				btPtr[x] = bbPtr[x];
				bbPtr[x] = bColor;
			}
			btPtr += xSize;
		}
		break;

	default:
		B3_THROW(b3TxException,B3_TX_UNSUPP);
	}
}
