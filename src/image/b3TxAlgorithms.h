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
	template<class SRC, class DST> static void b3GetRow(
		DST      *     Line,
		const b3Tx  *  tx,
		const b3_coord y,
		std::function<DST(const SRC)> convert = [] (const SRC data)
	{
		return b3Color(data);
	})
	{
		SRC * ptr = tx->data;

		ptr += (tx->xSize * y);
		for (b3_coord x = 0; x < tx->xSize; x++)
		{
			*Line++ = convert(*ptr++);
		}
	}

	template<class DST> static void b3SetRow(
		const b3Color * row,
		b3Tx      *     tx,
		std::function<DST(const b3Color &)> convert = [] (const b3Color & src)
	{
		return src;
	})
	{
		DST * ptr = tx->data;

		for (b3_res y = 0; y < tx->ySize; y++)
		{
			for (b3_res x = 0; x < tx->xSize; x++)
			{
				*ptr++ = convert(row[x]);
			}
		}
	}

	template<class DST> static void b3TurnLeft(b3Tx * tx)
	{
		const b3_res xNewSize = tx->ySize;
		const b3_res yNewSize = tx->xSize;
		DST     *    old_data;
		DST     *    new_data;
		b3_coord     src_pos;

		try
		{
			new_data = tx->b3TypedAlloc<DST>(xNewSize * yNewSize);
		}
		catch (std::bad_alloc & e)
		{
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}

		// change data pointer
		old_data = tx->data;
		tx->data = new_data;
		tx->xSize = xNewSize;
		tx->ySize = yNewSize;

		src_pos = yNewSize - 1;
		for (b3_res y = 0; y < yNewSize; y++)
		{
			b3_coord src_start = src_pos;

			for (b3_res x = 0; x < xNewSize; x++)
			{
				*new_data++  = old_data[src_start];
				src_start    += yNewSize;
			}
			src_pos--;
		}
		tx->b3Free(old_data);
	}

	template<class DST> static void b3Turn(b3Tx * tx)
	{
		const b3_count size = tx->xSize * tx->ySize;
		const b3_count max  = size >> 1;

		DST * bfPtr = tx->data;
		DST * bbPtr = bfPtr + size;

		for (b3_count i = 0; i < max; i++)
		{
			bbPtr--;
			const DST bBack  = *bbPtr;
			*bbPtr = *bfPtr;
			*bfPtr =  bBack;
			bfPtr++;
		}
	}

	template<class DST> static void b3TurnRight(b3Tx * tx)
	{
		const b3_coord xNewSize  = tx->ySize;
		const b3_coord yNewSize  = tx->xSize;
		DST      *     old_data;
		DST      *     new_data;
		b3_coord       src_pos, src_init;

		try
		{
			new_data = tx->b3TypedAlloc<DST>(xNewSize * yNewSize);
		}
		catch (std::bad_alloc & e)
		{
			B3_THROW(b3TxException, B3_TX_MEMORY);
		}

		// change data pointer
		old_data   = tx->data;
		tx->data  = new_data;
		tx->xSize = xNewSize;
		tx->ySize = yNewSize;

		src_pos    = 0;
		src_init   = yNewSize * (xNewSize - 1);
		for (b3_res y = 0; y < yNewSize; y++)
		{
			b3_coord srcStart = src_init + src_pos;
			for (b3_res x = 0; x < xNewSize; x++)
			{
				*new_data++  = old_data[srcStart];
				srcStart    -= yNewSize;
			}
			src_pos++;
		}
		tx->b3Free(old_data);
	}

	template<class DST> static void b3Transform(
		const b3Tx     *     srcTx,
		b3Tx        *        dstTx,
		const b3_pkd_color * rTable,
		const b3_pkd_color * gTable,
		const b3_pkd_color * bTable)
	{
		DST * dst_ptr = dstTx->data;

		for (b3_res y = 0; y < dstTx->ySize; y++)
		{
			for (b3_res x = 0; x < dstTx->xSize; x++)
			{
				const b3_pkd_color color = srcTx->b3GetValue(x, y);

				// Extract colors
				const b3_pkd_color a =      (color & 0xff000000) >> 24;
				const b3_pkd_color r = rTable[(color & 0xff0000) >> 16];
				const b3_pkd_color g = gTable[(color & 0x00ff00) >>  8];
				const b3_pkd_color b = bTable[(color & 0x0000ff)];

				// Combine colors
				const b3_pkd_color transformed =
					b3Color::b3MakePkdColor(r, g, b, a);

				*dst_ptr++ = b3Color(transformed);
			}
		}
	}

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
				const b3_f64 xDiff  = (b3_f64)(x - xPos) / xHalf;
				const b3_f64 yDiff  = (b3_f64)(y - yPos) / yHalf;
				const b3_f64 radius = xDiff * xDiff + yDiff * yDiff;

				// Computing Gauss value and left to right ramp
				const b3_f32  value = exp(radius / denom) * scale;
				const b3_f32  level = xDiff * slope + niveau;
				const b3Color sub   = level - value;

				*dstPtr++ = color + sub;
			}
		}
	}
};

#endif
