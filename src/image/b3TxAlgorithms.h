/*
**
**	$Filename:	b3TxAlgorithms.h $
**	$Release:	Dortmund 2011, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging algorithm templates
**
**	(C) Copyright 2021 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_IMAGE_TX_ALGORITHMS_H
#define B3_IMAGE_TX_ALGORITHMS_H

#include "blz3/image/b3Tx.h"

struct B3_PLUGIN b3TxAlgorithms
{
	template<class SRC, class DST> static void b3Blit(
		const b3Tx   *  srcTx,
		const b3Tx   *  dstTx,
		const b3_coord  xDstOff,
		const b3_coord  yDstOff,
		const b3_res    xSrcSize,
		const b3_res    ySrcSize,
		const b3_coord  xSrcOff,
		const b3_coord  ySrcOff,
		std::function<DST(const SRC)> convert = [] (const SRC data)
	{
		return data;
	})
	{
		const b3_coord src_offset = ySrcOff * srcTx->xSize + xSrcOff;
		const b3_coord dst_offset = yDstOff * dstTx->xSize + xDstOff;
		const b3_coord src_modulo = srcTx->xSize - xSrcSize;
		const b3_coord dst_modulo = dstTx->xSize - xSrcSize;

		// compute start pointer
		SRC * src_ptr = srcTx->data;
		DST * dst_ptr = dstTx->data;

		src_ptr  += src_offset;
		dst_ptr  += dst_offset;

		// compute line skip value
		for (b3_coord y = 0; y < ySrcSize; y++)
		{
			for (b3_coord x = 0; x < xSrcSize; x++)
			{
				*dst_ptr++ = convert(*src_ptr++);
			}
			src_ptr += src_modulo;
			dst_ptr += dst_modulo;
		}
	}

	template<class SRC, class DST> static void b3Gauss(
		const b3Tx  *  src,
		b3Tx     *     dst,
		const b3_coord xPos,
		const b3_coord yPos,
		const b3_f64   scale,
		const b3_f64   sigma,
		const b3_f64   niveau,
		const b3_f64   slope,
		std::function<b3Color(const SRC)> convert = [] (const SRC data)
	{
		return data;
	})
	{
		const b3_f64   denom = -2.0 * sigma * sigma;
		const b3_f64   xHalf = dst->xSize >> 1;
		const b3_f64   yHalf = dst->ySize >> 1;

		SRC * srcPtr = src->data;
		DST * dstPtr = dst->data;

		for (b3_res y = 0; y < dst->ySize; y++)
		{
			for (b3_res x = 0; x < dst->xSize; x++)
			{
				const b3Color & color(convert(*srcPtr++));

				// Computing distance to center
				b3_f64 xDiff  = (b3_f64)(x - xPos) / (b3_f64)xHalf;
				b3_f64 yDiff  = (b3_f64)(y - yPos) / (b3_f64)yHalf;
				b3_f64 radius = xDiff * xDiff + yDiff * yDiff;

				// Computing Gauss value and left to right ramp
				const b3_f32 value  = exp(radius / denom) * scale;
				const b3_f32 level  = xDiff * slope + niveau;

				// Subtract color values
				b3f_32 sub = level - value;

				*dstPtr++ = color + sub;
			}
		}
	}
};

#endif
