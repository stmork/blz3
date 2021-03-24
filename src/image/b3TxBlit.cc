/*
**
**	$Filename:	b3TxHist.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - image blitting
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
**                        b3Tx blitting                                 **
**                                                                      **
*************************************************************************/

void b3Tx::b3Blit(
	b3Tx   *  srcTx,
	b3_coord  xDstOff,
	b3_coord  yDstOff,
	b3_res    xMax,
	b3_res    yMax,
	b3_coord  xSrcOff,
	b3_coord  ySrcOff)
{
	b3_pkd_color * lSrc, *lDst, *pal;
	b3_u16    *    sSrc;
	b3_u08    *    cSrc;
	b3_coord       x, srcMod, SrcOff;
	b3_coord       y, dstMod, DstOff;

	// We support only true color yet
	if (type != B3_TX_RGB8)
	{
		return;
	}

	// Clip against bounds (source)
	if ((xSrcOff + xMax) > srcTx->xSize)
	{
		xMax = srcTx->xSize - xSrcOff;
	}
	if ((ySrcOff + yMax) > srcTx->ySize)
	{
		yMax = srcTx->ySize - ySrcOff;
	}

	// Clip against bounds (destination)
	if ((xDstOff + xMax) > xSize)
	{
		xMax = xSize        - xDstOff;
	}
	if ((yDstOff + yMax) > ySize)
	{
		yMax = ySize        - yDstOff;
	}

	b3PrintF(B3LOG_FULL, "### CLASS: b3Tx   # b3Blit(): size: %ldx%ld (%ld,%ld) -> (%ld,%ld)\n",
		xMax, yMax, xSrcOff, ySrcOff, xDstOff, yDstOff);
	switch (srcTx->type)
	{
	case B3_TX_ILBM:
		if (srcTx->depth != 1)
		{
			return;
		}

		SrcOff = ySrcOff * TX_BWA(srcTx->xSize);
		DstOff = yDstOff * xSize + xDstOff;

		// compute start pointer
		pal    = srcTx->b3GetPalette();
		cSrc   = (b3_u08 *)srcTx->data;
		lDst   = (b3_pkd_color *)data;
		cSrc  += SrcOff;
		lDst  += DstOff;

		// compute line skip value
		srcMod = TX_BWA(srcTx->xSize);
		dstMod = xSize - xMax;
		for (y = 0; y < yMax; y++)
		{
			b3_pkd_color bit;
			b3_index     ind;

			bit = (128 >> xSrcOff) & 7;
			ind = xSrcOff >> 3;
			for (x = 0; x < xMax; x++)
			{

				*lDst++ = pal[cSrc[ind] & bit ? 1 : 0];
				bit     = bit >> 1;
				if (bit == 0)
				{
					ind++;
					bit = 128;
				}
			}
			cSrc += srcMod;
			lDst += dstMod;
		}
		break;

	case B3_TX_VGA:
		SrcOff = ySrcOff * srcTx->xSize + xSrcOff;
		DstOff = yDstOff *        xSize + xDstOff;

		// compute start pointer
		pal    = srcTx->b3GetPalette();
		cSrc   = (b3_u08 *)srcTx->data;
		lDst   = (b3_pkd_color *)data;
		cSrc  += SrcOff;
		lDst  += DstOff;

		// compute line skip value
		srcMod  = srcTx->xSize - xMax;
		dstMod  =        xSize - xMax;
		for (y = 0; y < yMax; y++)
		{
			for (x = 0; x < xMax; x++)
			{
				*lDst++ = pal[*cSrc++];
			}
			cSrc += srcMod;
			lDst += dstMod;
		}
		break;

	case B3_TX_RGB4:
		SrcOff = ySrcOff * srcTx->xSize + xSrcOff;
		DstOff = yDstOff *        xSize + xDstOff;

		// compute start pointer
		sSrc   = (b3_u16 *)srcTx->data;
		lDst   = (b3_pkd_color *)data;
		sSrc  += SrcOff;
		lDst  += DstOff;

		// compute line skip value
		srcMod = srcTx->xSize - xMax;
		dstMod =        xSize - xMax;
		for (y = 0; y < yMax; y++)
		{
			b3_u16 col;

			for (x = 0; x < xMax; x++)
			{
				col     = *sSrc++;
				*lDst++ = TX_RGB4_TO_RGB8(col);
			}
			sSrc += srcMod;
			lDst += dstMod;
		}
		break;

	case B3_TX_RGB8:
		SrcOff = ySrcOff * srcTx->xSize + xSrcOff;
		DstOff = yDstOff *        xSize + xDstOff;

		// compute start pointer
		lSrc   = (b3_pkd_color *)srcTx->data;
		lDst   = (b3_pkd_color *)data;
		lSrc  += SrcOff;
		lDst  += DstOff;

		// compute line skip value
		srcMod = srcTx->xSize - xMax;
		dstMod =        xSize - xMax;
		for (y = 0; y < yMax; y++)
		{
			for (x = 0; x < xMax; x++)
			{
				*lDst++ = *lSrc++;
			}
			lSrc += srcMod;
			lDst += dstMod;
		}
		break;

	default:
		break;
	}
}

void b3Tx::b3GetColorMask(
	b3_u08    *   mask,
	b3_count      BytesPerRow,
	b3_pkd_color  colorMask)
{
	b3_pkd_color * lPtr, bit;
	b3_u08    *   bPtr;
	b3_coord      x, y;
	b3_index      xBytes, pos;
	b3_u08        cache;

	switch (type)
	{
	case B3_TX_ILBM:
		bPtr   = (b3_u08 *)data;
		xBytes = TX_BWA(xSize);
		for (y = 0; y < ySize; y++)
		{
			for (x = 0; x < xBytes; x++)
			{
				mask[x] = bPtr[x];
			}
			mask += BytesPerRow;
			bPtr += xBytes;
		}
		break;

	case B3_TX_VGA:
		bPtr = (b3_u08 *)data;
		for (y = 0; y < ySize; y++)
		{
			pos   =   0;
			bit   = 128;
			cache =   0;
			for (x = 0; x < xSize; x++)
			{
				if (palette[bPtr[x]] == colorMask)
				{
					cache |= bit;
				}
				bit = bit >> 1;
				if (bit == 0)
				{
					mask[pos++] = cache;
					bit   = 128;
					cache = 0;
				}
			}
			mask[pos]  = cache;
			mask      += BytesPerRow;
			bPtr      += xSize;
		}
		break;

	case B3_TX_RGB8:
		lPtr = (b3_pkd_color *)data;
		for (y = 0; y < ySize; y++)
		{
			pos   =   0;
			bit   = 128;
			cache =   0;
			for (x = 0; x < xSize; x++)
			{
				if (lPtr[x] == colorMask)
				{
					cache |= bit;
				}
				bit = bit >> 1;
				if (bit == 0)
				{
					mask[pos++] = cache;
					bit   = 128;
					cache = 0;
				}
			}
			mask[pos]  = cache;
			mask      += BytesPerRow;
			lPtr      += xSize;
		}
		break;

	default:
		break;
	}
}
