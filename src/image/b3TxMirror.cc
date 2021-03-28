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
**	(C) Copyright 2004 - 2021  Steffen A. Mork
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
**                        implementation                                **
**                                                                      **
*************************************************************************/

void b3Tx::b3MirrorHorizontal()
{
	b3_tx_data     ptr(data);
	b3_coord       x, y, xHalf = xSize >> 1;
	b3_color   *   ctPtr, *cbPtr, cColor;
	b3_pkd_color * ltPtr, *lbPtr, lColor;
	b3_u16    *    stPtr, *sbPtr, sColor;
	b3_u08    *    btPtr, *bbPtr, bColor;

	// Untested, yet.
	switch (type)
	{
	case B3_TX_FLOAT:
		cbPtr = ptr;
		for (y = 0; y < ySize; y++)
		{
			ctPtr = &cbPtr[xSize];
			for (x = 0; x < xHalf; x++)
			{
				ctPtr--;
				cColor   = *ctPtr;
				*ctPtr   =  cbPtr[x];
				cbPtr[x] =  cColor;
			}
			cbPtr += xSize;
		}
		break;

	case B3_TX_RGB8:
		lbPtr = ptr;
		for (y = 0; y < ySize; y++)
		{
			ltPtr = &lbPtr[xSize];
			for (x = 0; x < xHalf; x++)
			{
				ltPtr--;
				lColor   = *ltPtr;
				*ltPtr   =  lbPtr[x];
				lbPtr[x] =  lColor;
			}
			lbPtr += xSize;
		}
		break;

	case B3_TX_RGB4:
		sbPtr = ptr;
		for (y = 0; y < ySize; y++)
		{
			stPtr = &sbPtr[xSize];
			for (x = 0; x < xHalf; x++)
			{
				stPtr--;
				sColor   = *stPtr;
				*stPtr   =  sbPtr[x];
				sbPtr[x] =  sColor;
			}
			sbPtr += xSize;
		}
		break;

	case B3_TX_VGA:
		bbPtr = data;
		for (y = 0; y < ySize; y++)
		{
			btPtr = &bbPtr[xSize];
			for (x = 0; x < xHalf; x++)
			{
				btPtr--;
				bColor   = *btPtr;
				*btPtr   =  bbPtr[x];
				bbPtr[x] =  bColor;
			}
			bbPtr += xSize;
		}
		break;

	default:
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}
}

void b3Tx::b3MirrorVertical()
{
	b3_coord       x, y, yMax = ySize >> 1;
	b3_color   *   ctPtr, *cbPtr, cColor;
	b3_pkd_color * ltPtr, *lbPtr, lColor;
	b3_u16    *    stPtr, *sbPtr, sColor;
	b3_u08    *    btPtr, *bbPtr, bColor;

	switch (type)
	{
	case B3_TX_FLOAT:
		ctPtr = data;
		cbPtr = &ctPtr[xSize * ySize];
		for (y = 0; y < yMax; y++)
		{
			cbPtr -= xSize;
			for (x = 0; x < xSize; x++)
			{
				cColor   = ctPtr[x];
				ctPtr[x] = cbPtr[x];
				cbPtr[x] = cColor;
			}
			ctPtr += xSize;
		}
		break;

	case B3_TX_RGB8:
		ltPtr = data;
		lbPtr = &ltPtr[xSize * ySize];
		for (y = 0; y < yMax; y++)
		{
			lbPtr -= xSize;
			for (x = 0; x < xSize; x++)
			{
				lColor   = ltPtr[x];
				ltPtr[x] = lbPtr[x];
				lbPtr[x] = lColor;
			}
			ltPtr += xSize;
		}
		break;

	case B3_TX_RGB4:
		stPtr = data;
		sbPtr = &stPtr[xSize * ySize];
		for (y = 0; y < yMax; y++)
		{
			sbPtr -= xSize;
			for (x = 0; x < xSize; x++)
			{
				sColor   = stPtr[x];
				stPtr[x] = sbPtr[x];
				sbPtr[x] = sColor;
			}
			stPtr += xSize;
		}
		break;

	case B3_TX_VGA:
		btPtr = data;
		bbPtr = &btPtr[xSize * ySize];
		for (y = 0; y < yMax; y++)
		{
			bbPtr -= xSize;
			for (x = 0; x < xSize; x++)
			{
				bColor   = btPtr[x];
				btPtr[x] = bbPtr[x];
				bbPtr[x] = bColor;
			}
			btPtr += xSize;
		}
		break;

	default:
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}
}
