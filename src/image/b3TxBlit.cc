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
**	(C) Copyright 2001 - 2021  Steffen A. Mork
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
#include "b3TxAlgorithms.h"

/*************************************************************************
**                                                                      **
**                        b3Tx blitting                                 **
**                                                                      **
*************************************************************************/

b3_res b3Tx::b3ClipBlit(
	b3_coord  &  src_offset,
	b3_coord  &  dst_offset,
	const b3_res src_size,
	const b3_res dst_size,
	const b3_res blit_size)
{
	b3_res max = blit_size;

	if (dst_offset < 0)
	{
		max += dst_offset;
		dst_offset = 0;
	}
	if (src_offset < 0)
	{
		max += src_offset;
		src_offset = 0;
	}

	if (max > 0)
	{
		if ((src_offset + max) > src_size)
		{
			max = src_size - src_offset;
		}

		if ((dst_offset + max) > dst_size)
		{
			max = dst_size - dst_offset;
		}
	}

	return max;
}

void b3Tx::b3Blit(const b3Tx   *  srcTx,
	b3_coord  xDstOff,
	b3_coord  yDstOff,
	b3_res    xMax,
	b3_res    yMax,
	b3_coord  xSrcOff,
	b3_coord  ySrcOff)
{
	b3_pkd_color * lDst;
	b3_u08    *    cSrc;
	b3_coord       x, srcMod, SrcOff;
	b3_coord       y, dstMod, DstOff;

	// We support only true color yet
	if (type != B3_TX_RGB8)
	{
		B3_THROW(b3TxException, B3_TX_UNSUPP);
	}

	// Clip against bounds
	xMax = b3ClipBlit(xSrcOff, xDstOff, srcTx->xSize, xSize, xMax);
	yMax = b3ClipBlit(ySrcOff, yDstOff, srcTx->ySize, ySize, yMax);
	if ((xMax <= 0) || (yMax <= 0))
	{
		// Nothing to blit: rectangle is outside.
		return;
	}

	b3PrintF(B3LOG_FULL,
		"### CLASS: b3Tx   # b3Blit(): size: %dx%d (%d,%d) -> (%d,%d)\n",
		xMax, yMax, xSrcOff, ySrcOff, xDstOff, yDstOff);
	switch (srcTx->type)
	{
	case B3_TX_ILBM:
		if (srcTx->depth != 1)
		{
			B3_THROW(b3TxException, B3_TX_UNSUPP);
		}

		SrcOff = ySrcOff * TX_BWA(srcTx->xSize);
		DstOff = yDstOff * xSize + xDstOff;

		// compute start pointer
		cSrc    = srcTx->data;
		lDst    = data;
		cSrc   += SrcOff;
		lDst   += DstOff;

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

				*lDst++ = srcTx->palette[(cSrc[ind] & bit) != 0 ? 1 : 0];
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
		b3TxAlgorithms::b3Blit<b3_u08, b3_pkd_color>(
			srcTx,   this,
			xDstOff, yDstOff,
			xMax,    yMax,
			xSrcOff, ySrcOff, [srcTx] (const b3_u08 index)
		{
			return srcTx->palette[index];
		});
		break;

	case B3_TX_RGB4:
		b3TxAlgorithms::b3Blit<b3_u16, b3_pkd_color>(
			srcTx,   this,
			xDstOff, yDstOff,
			xMax,    yMax,
			xSrcOff, ySrcOff, [] (const b3_u16 color)
		{
			return b3Convert(color);
		});
		break;

	case B3_TX_RGB8:
		b3TxAlgorithms::b3Blit<b3_pkd_color, b3_pkd_color>(
			srcTx,   this,
			xDstOff, yDstOff,
			xMax,    yMax,
			xSrcOff, ySrcOff);
		break;

	case B3_TX_FLOAT:
		b3TxAlgorithms::b3Blit<b3_color, b3_pkd_color>(
			srcTx,   this,
			xDstOff, yDstOff,
			xMax,    yMax,
			xSrcOff, ySrcOff, [] (const b3_color & color)
		{
			return b3Color(color);
		});
		break;

	default:
		B3_THROW(b3TxException, B3_TX_ILLEGAL_DATATYPE);
	}
}

void b3Tx::b3GetColorMask(
	b3_u08    *   mask,
	b3_count      BytesPerRow,
	b3_pkd_color  colorMask) const
{
	b3_pkd_color * lPtr, bit;
	b3_u08    *    bPtr;
	b3_coord       x, y;
	b3_index       xBytes, pos;
	b3_u08         cache;

	switch (type)
	{
	case B3_TX_ILBM:
		bPtr   = data;
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
		bPtr = data;
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
		lPtr = data;
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
