/*
**
**	$Filename:	b3FFT.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Fourier transform 
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3FFT.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3Math.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/04/17 14:42:46  sm
**	- Completed ocean waves. I see ocean waves. They are not nice but
**	  I can see them!
**
**	Revision 1.1  2006/04/16 21:05:03  sm
**	- Added FFT module.
**	- Changed ocean waves to FFT creation. Not working yet!
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Fourier                                     **
**                                                                      **
*************************************************************************/

b3Fourier::b3Fourier()
{
	// Dimension
	m_xOrig  =
	m_yOrig  = 0;
	m_xSize  =
	m_ySize  = 0;
	m_xStart =
	m_yStart = 0;

	// Aux buffers
	m_TempBuffer = null;
	m_PermBuffer = null;
	m_Values     = null;

	// Value buffers
	m_Type    = B3_FOURIER_UNDEFINED;
	m_rLine   = null;
	m_gLine   = null;
	m_bLine   = null;
	m_rBuffer = null;
	m_gBuffer = null;
	m_bBuffer = null;
}

b3Fourier::b3Fourier(b3Tx *tx)
{
	b3AllocBuffer(tx);
}

b3Fourier::b3Fourier(b3Fourier &src)
{
	b3_size size;

	m_xSize  = src.m_xSize;
	m_ySize  = src.m_ySize;
	m_xOrig  = src.m_xOrig;
	m_yOrig  = src.m_yOrig;
	m_xStart = src.m_xStart;
	m_yStart = src.m_yStart;
	size   = m_xSize * m_ySize * sizeof(b3_f64);

	// Aux buffers
	m_TempBuffer = null;
	m_PermBuffer = null;
	m_Values     = null;

	// Clone red
	if (src.m_rBuffer != null)
	{
		m_rLine = b3Alloc2D();
		if (m_rLine == null)
		{
			throw;
		}
		m_rBuffer = m_rLine[0];
		memcpy(m_rBuffer,src.m_rBuffer,size);
	}
	else
	{
		m_rLine   = null;
		m_rBuffer = null;
	}

	// Clone green
	if (src.m_gBuffer != null)
	{
		m_gLine = b3Alloc2D();
		if (m_gLine == null)
		{
			throw;
		}
		m_gBuffer = m_gLine[0];
		memcpy(m_gBuffer,src.m_gBuffer,size);
	}
	else
	{
		m_gLine   = null;
		m_gBuffer = null;
	}

	// Clone blue
	if (src.m_bBuffer != null)
	{
		m_bLine = b3Alloc2D();
		if (m_bLine == null)
		{
			throw;
		}
		m_bBuffer = m_bLine[0];
		memcpy(m_bBuffer,src.m_bBuffer,size);
	}
	else
	{
		m_bLine   = null;
		m_bBuffer = null;
	}

	m_Type   = src.m_Type;
}

void b3Fourier::b3AllocBuffer(b3Tx *tx)
{
	b3_loop       x,y,index,max;
	b3_pkd_color *palette,color;
	b3_u08       *cPtr;
	b3_u32       *lPtr;

	b3PrintF(B3LOG_FULL, "b3Fourier::b3AllocBuffer(%dx%d, ...)\n", tx->xSize, tx->ySize);
	b3Free();

	m_xOrig  = tx->xSize;
	m_yOrig  = tx->ySize;
	m_xSize  = b3PowOf2(m_xOrig);
	m_ySize  = b3PowOf2(m_yOrig);
	max      = B3_MAX(m_xSize, m_ySize);
	m_xSize  = max;
	m_ySize  = max;
	m_xStart = (m_xSize - m_xOrig) >> 1;
	m_yStart = (m_ySize - m_yOrig) >> 1;
	index    = m_yStart * m_xSize + m_xStart;

	// Aux buffers
	m_TempBuffer = null;
	m_PermBuffer = null;
	m_Values     = null;

	m_Type = B3_FOURIER_UNDEFINED;
	if (tx->b3IsGreyPalette())
	{
		b3PrintF(B3LOG_FULL, "  Grey (%dx%d)\n", m_xSize, m_ySize);
		m_rLine   = null;
		m_gLine   = b3Alloc2D();
		m_bLine   = null;
		m_rBuffer = null;
		m_gBuffer = m_gLine[0];
		m_bBuffer = null;
		cPtr    = (b3_u08 *)tx->b3GetData();
		for (y = 0;y < m_yOrig;y++)
		{
			for (x = 0;x < m_xOrig;x++)
			{
				m_gBuffer[index + x] = (b3_f64)*cPtr++ / 255.0;
			}
			index += m_xSize;
		}

#if 0
		index = 0;
		long xHalf = m_xSize >> 1,yHalf = m_ySize >> 1;
		for (y = 0;y < m_ySize;y++)
		{
			for (x = 0;x < m_xSize;x++)
			{
				b3_f64 rx = x - xHalf,ry = (y - yHalf) * 1;

//				m_gBuffer[index + x] = cos((rx + ry) / (b3_f64)xSize * M_PI * 2.0 *  4.0) * 0.5 + 0.5;
//				m_gBuffer[index + x] = cos(rx / (b3_f64)m_xSize * M_PI * 2.0 * 32.0) > -0.2 ? 1.0 : 0.0;
//				m_gBuffer[index + x] = cos(sqrt(rx * rx + ry * ry) / (b3_f64)m_xSize * M_PI * 2.0 * 32.0) > -0.2 ? 1.0 : 0.0;
//				m_gBuffer[index + x] = cos(sqrt(rx * rx + ry * ry) / (b3_f64)m_xSize * M_PI * 2.0 *  4.0) * 0.5 + 0.5;
//				m_gBuffer[index + x] = cos((rx + ry) / (b3_f64)m_xSize * M_PI * 2.0 * 32.0) * 0.5 + 0.5;
			}
			index += m_xSize;
		}
#endif
		m_Type = B3_FOURIER_GREY;
	}
	else
	{
		if(tx->b3IsPalette())
		{
			b3PrintF(B3LOG_FULL, "  Palette (%dx%d)\n", m_xSize, m_ySize);
			m_rLine   = b3Alloc2D();
			m_gLine   = b3Alloc2D();
			m_bLine   = b3Alloc2D();
			m_rBuffer = m_rLine[0];
			m_gBuffer = m_gLine[0];
			m_bBuffer = m_bLine[0];
			palette = tx->b3GetPalette();
			cPtr    = (b3_u08 *)tx->b3GetData();
			for (y = 0;y < m_yOrig;y++)
			{
				for (x = 0;x < m_xOrig;x++)
				{
					color = palette[*cPtr++];
					m_rBuffer[index + x] = (b3_f64)((color & 0xff0000) >> 16) / 255.0;
					m_gBuffer[index + x] = (b3_f64)((color & 0x00ff00) >>  8) / 255.0;
					m_bBuffer[index + x] = (b3_f64)((color & 0x0000ff))       / 255.0;
				}
				index += m_xSize;
			}
			m_Type = B3_FOURIER_PALETTE;
		}

		if (tx->b3IsTrueColor())
		{
			b3PrintF(B3LOG_FULL, "  True color (%dx%d)\n", m_xSize, m_ySize);
			m_rLine   = b3Alloc2D();
			m_gLine   = b3Alloc2D();
			m_bLine   = b3Alloc2D();
			m_rBuffer = m_rLine[0];
			m_gBuffer = m_gLine[0];
			m_bBuffer = m_bLine[0];
			lPtr    = (b3_pkd_color *)tx->b3GetData();
			for (y = 0;y < m_yOrig;y++)
			{
				for (x = 0;x < m_xOrig;x++)
				{
					color = *lPtr++;
					m_rBuffer[index + x] = (b3_f64)((color & 0xff0000) >> 16) / 255.0;
					m_gBuffer[index + x] = (b3_f64)((color & 0x00ff00) >>  8) / 255.0;
					m_bBuffer[index + x] = (b3_f64)((color & 0x0000ff))       / 255.0;
				}
				index += m_xSize;
			}
			m_Type = B3_FOURIER_TRUECOLOR;
		}
	}

	if (m_Type == B3_FOURIER_UNDEFINED)
	{
		throw;
	}
}

b3_bool b3Fourier::b3AllocBuffer(b3_res size, b3_fourier_type new_type)
{
	b3_res max;

	b3PrintF(B3LOG_FULL, "b3Fourier::b3AllocBuffer(%d, ...)\n", size);
	b3Free();
	m_xOrig  =
	m_yOrig  = size;
	m_xSize  =
	m_ySize  = b3PowOf2(size);
	m_xStart = (m_xSize - m_xOrig) >> 1;
	m_yStart = (m_ySize - m_yOrig) >> 1;

	// Aux buffers
	m_TempBuffer = null;
	m_PermBuffer = null;
	m_Values     = null;

	m_Type = B3_FOURIER_UNDEFINED;
	switch(new_type)
	{
	case B3_FOURIER_GREY:
		m_Type    = new_type;
		m_rLine   = null;
		m_gLine   = b3Alloc2D();
		m_bLine   = null;
		m_rBuffer = null;
		m_gBuffer = m_gLine[0];
		m_bBuffer = null;
		break;

	case B3_FOURIER_TRUECOLOR:
		m_Type    = new_type;
		m_rLine   = b3Alloc2D();
		m_gLine   = b3Alloc2D();
		m_bLine   = b3Alloc2D();
		m_rBuffer = m_rLine[0];
		m_gBuffer = m_gLine[0];
		m_bBuffer = m_bLine[0];
		break;

	default :
		m_Type    = B3_FOURIER_UNDEFINED;
		m_rLine   = null;
		m_gLine   = null;
		m_bLine   = null;
		m_rBuffer = null;
		m_gBuffer = null;
		m_bBuffer = null;
		break;
	}
	return m_Type != B3_FOURIER_UNDEFINED;
}

void b3Fourier::b3SelfTest()
{
    b3_loop  x, y;
    b3_s32   seed;
    b3_f64   err = 0, e, divisor;

	seed = 0;
    for (y = 0; y < m_ySize; y++)
	{
        for (x = 0; x < m_xSize; x++)
		{
            m_gLine[y][x] = b3Rnd(seed);
        }
    }

	b3DumpBuffer(B3LOG_FULL);

	b3FFT2D();
	b3IFFT2D();

	seed = 0;
	divisor = m_xSize * m_ySize * 0.5;
    for (y = 0; y < m_ySize; y++)
	{
        for (x = 0; x < m_xSize; x++)
		{
            e   = b3Rnd(seed) - m_gLine[y][x];
            err = B3_MAX(err, fabs(e));
        }
    }
	b3PrintF(B3LOG_NORMAL,"### CLASS: b3Four # error %g\n",err);
}

void b3Fourier::b3GetBuffer(b3Tx *tx)
{
	b3_pkd_color *lPtr,color;
	b3_u08       *cPtr;
	b3_f64        r;
	b3_loop       x,y,index = 0;

	b3PrintF(B3LOG_FULL, "b3Fourier::b3GetBuffer(...)\n");
	index  = m_yStart * m_xSize + m_xStart;
	switch(m_Type)
	{
	case B3_FOURIER_GREY:
		tx->b3AllocTx(m_xOrig, m_yOrig, 8);
		cPtr = (b3_u08 *)tx->b3GetData();
		for (y = 0;y < m_yOrig;y++)
		{
			for (x = 0;x < m_xOrig;x++)
			{
				*cPtr++ = (b3_u08)floor(b3Math::b3Limit(m_gBuffer[index + x]) * 255);
			}
			index += m_xSize;
		}
		break;

	case B3_FOURIER_PALETTE:
	case B3_FOURIER_TRUECOLOR:
		tx->b3AllocTx(m_xOrig, m_yOrig, 24);
		lPtr = (b3_pkd_color *)tx->b3GetData();
		for (y = 0;y < m_yOrig;y++)
		{
			for (x = 0;x < m_xOrig;x++)
			{
				*lPtr++ = b3Color(m_rBuffer[index + x],m_gBuffer[index + x],m_bBuffer[index + x]);
			}
			index += m_xSize;
		}
		break;

	default:
		break;
	}
}

void b3Fourier::b3GetSpectrum(b3Tx *tx)
{
	b3_pkd_color  *lPtr,color;
	b3_u08        *cPtr;
	b3_f64         r,i = 0,R,G,B;
	b3_f64         result;
	b3_f64         denom = 4.0 / (m_xSize + m_ySize);
	b3_loop        x,y,index = 0,xHalf = m_xSize >> 1,yHalf = m_ySize >> 1;
	b3_loop        xMask = m_xSize - 1,yMask = m_ySize - 1;
	b3_index       dst,off;

	switch(m_Type)
	{
	case B3_FOURIER_GREY:
		tx->b3AllocTx(m_xSize, m_ySize, 8);
		cPtr = (b3_u08 *)tx->b3GetData();
		for (y = 0;y < m_ySize;y++)
		{
			dst = ((y + yHalf) & yMask) * m_xSize;
			off = xHalf;
			for (x = 0;x < xHalf;x++)
			{
				r = m_gBuffer[index + x];
				i = m_gBuffer[index + x + xHalf];
				result  = floor(sqrt(r * r + i * i) * 0.5);
				cPtr[dst + off]     = (b3_u08)result;
				cPtr[dst + off + 1] = (b3_u08)result;

				off = (off + 2) & xMask;
			}
			index += m_xSize;
		}
		break;

	case B3_FOURIER_PALETTE:
	case B3_FOURIER_TRUECOLOR:
		tx->b3AllocTx(m_xSize, m_ySize, 24);
		lPtr = (b3_pkd_color *)tx->b3GetData();
		for (y = 0;y < m_ySize;y++)
		{
			dst = ((y + yHalf) & yMask) * m_xSize;
			off = xHalf;
			for (x = 0;x < xHalf;x++)
			{
				r = m_rBuffer[index + x];
				i = m_rBuffer[index + x + xHalf];
				R = sqrt(r * r + i * i);

				r = m_gBuffer[index + x];
				i = m_gBuffer[index + x + xHalf];
				G = sqrt(r * r + i * i);

				r = m_bBuffer[index + x];
				i = m_bBuffer[index + x + xHalf];
				B = sqrt(r * r + i * i);

				lPtr[dst + off]     =
				lPtr[dst + off + 1] = b3Color(
					R * denom,
					G * denom,
					B * denom);

				off = (off + 2) & xMask;
			}
			index += m_xSize;
		}
		break;

	default:
		break;
	}
}

void b3Fourier::b3DumpBuffer(b3_log_level level)
{
	b3_loop  x,y;
	b3_index index = 0;

	b3PrintF(level,"### Fourier buffer dump # %dx%d\n", m_xSize, m_ySize);
	for (y = 0;y < m_ySize;y++)
	{
		for (x = 0;x < m_xSize;x++)
		{
			b3PrintF(level, "%1.3f ", m_gBuffer[index++]);
		}
		b3PrintF(level,"\n");
	}
}

void b3Fourier::b3DumpSpectrum(b3_log_level level)
{
	b3_loop  x,y,xHalf = m_xSize >> 1;
	b3_index index = 0;

	b3PrintF(level,"### Fourier spectrum dump # %dx%d\n", m_xSize, m_ySize);
	for (y = 0;y < m_ySize;y++)
	{
		for (x = 0;x < xHalf;x++)
		{
			b3PrintF(level, "%1.3f/%1.3f ", m_gBuffer[index], m_gBuffer[index + xHalf]);
			index++;
		}
		b3PrintF(level,"\n");
	}
}

void b3Fourier::b3FilterBandpass(
	b3_f64          fx,
	b3_f64          fy,
	b3_index        re,
	b3_index        im,
	b3Fourier    *fourier,
	b3FilterInfo *filter_info)
{
	b3_f64 rad;
	b3_f64 start    = filter_info->bp.start;
	b3_f64 end      = filter_info->bp.end;
	b3_f64 *rBuffer = fourier->m_rBuffer;
	b3_f64 *gBuffer = fourier->m_gBuffer;
	b3_f64 *bBuffer = fourier->m_bBuffer;

	rad = fx * fx + fy * fy;
	if ((end <= rad) && (rad <= start))
	{
		if (rBuffer != null)
		{
			rBuffer[re] = 0;
			rBuffer[im] = 0;
		}
		if (gBuffer != null)
		{
			gBuffer[re] = 0;
			gBuffer[im] = 0;
		}
		if (bBuffer != null)
		{
			bBuffer[re] = 0;
			bBuffer[im] = 0;
		}
	}
}

void b3Fourier::b3FilterLowpass(
	b3_f64          fx,
	b3_f64          fy,
	b3_index            re,
	b3_index            im,
	b3Fourier    *fourier,
	b3FilterInfo *filter_info)
{
	b3_f64 *rBuffer = fourier->m_rBuffer;
	b3_f64 *gBuffer = fourier->m_gBuffer;
	b3_f64 *bBuffer = fourier->m_bBuffer;

	if ((fx * fx + fy * fy) >= filter_info->lp.limit)
	{
		if (rBuffer != null)
		{
			rBuffer[re] = 0;
			rBuffer[im] = 0;
		}
		if (gBuffer != null)
		{
			gBuffer[re] = 0;
			gBuffer[im] = 0;
		}
		if (bBuffer != null)
		{
			bBuffer[re] = 0;
			bBuffer[im] = 0;
		}
	}
}

void b3Fourier::b3FilterHighpass(
	b3_f64          fx,
	b3_f64          fy,
	b3_index            re,
	b3_index            im,
	b3Fourier    *fourier,
	b3FilterInfo *filter_info)
{
	b3_f64 *rBuffer = fourier->m_rBuffer;
	b3_f64 *gBuffer = fourier->m_gBuffer;
	b3_f64 *bBuffer = fourier->m_bBuffer;

	if ((fx * fx + fy * fy) <= filter_info->hp.limit)
	{
		if (rBuffer != null)
		{
			rBuffer[re] = 0;
			rBuffer[im] = 0;
		}
		if (gBuffer != null)
		{
			gBuffer[re] = 0;
			gBuffer[im] = 0;
		}
		if (bBuffer != null)
		{
			bBuffer[re] = 0;
			bBuffer[im] = 0;
		}
	}
}

void b3Fourier::b3FilterAntiRaster(
	b3_f64          fx,
	b3_f64          fy,
	b3_index            re,
	b3_index            im,
	b3Fourier    *fourier,
	b3FilterInfo *filter_info)
{
	b3_f64 *rBuffer = fourier->m_rBuffer;
	b3_f64 *gBuffer = fourier->m_gBuffer;
	b3_f64 *bBuffer = fourier->m_bBuffer;
	b3_f64  corr    = filter_info->ar.corr;

	if (((fx * fx + fy * fy) >= filter_info->ar.limit) &&
		(fabs(fx) >= corr) &&
		(fabs(fy) >= corr))
	{
		if (rBuffer != null)
		{
			rBuffer[re] = 0;
			rBuffer[im] = 0;
		}
		if (gBuffer != null)
		{
			gBuffer[re] = 0;
			gBuffer[im] = 0;
		}
		if (bBuffer != null)
		{
			bBuffer[re] = 0;
			bBuffer[im] = 0;
		}
	}
}

void b3Fourier::b3FilterSinc2(
	b3_f64          fx,
	b3_f64          fy,
	b3_index            re,
	b3_index            im,
	b3Fourier    *fourier,
	b3FilterInfo *filter_info)
{
	b3_f64 *rBuffer = fourier->m_rBuffer;
	b3_f64 *gBuffer = fourier->m_gBuffer;
	b3_f64 *bBuffer = fourier->m_bBuffer;
	b3_f64  factor  = filter_info->si2.factor;
	b3_f64  result  = 1.0,x;

	x       = sqrt(fx * fx + fy * fy) * factor;
	if (x == 0.0)
	{
		result = 1.0;
	}
	else
	{
		result  = sin(x) / x;
		result *= result;
	}
	if (rBuffer != null)
	{
		rBuffer[re] *= result;
		rBuffer[im] *= result;
	}
	if (gBuffer != null)
	{
		gBuffer[re] *= result;
		gBuffer[im] *= result;
	}
	if (bBuffer != null)
	{
		bBuffer[re] *= result;
		bBuffer[im] *= result;
	}
}

void b3Fourier::b3FilterSinc2AR(
	b3_f64          fx,
	b3_f64          fy,
	b3_index            re,
	b3_index            im,
	b3Fourier    *fourier,
	b3FilterInfo *filter_info)
{
	b3_f64 *rBuffer = fourier->m_rBuffer;
	b3_f64 *gBuffer = fourier->m_gBuffer;
	b3_f64 *bBuffer = fourier->m_bBuffer;
	b3_f64  factor  = filter_info->si2.factor;
	b3_f64  result  = 1.0,x,y;

	x = fx * factor;
	y = fy * factor;
	if (x != 0.0)
	{
		result *= (sin(x) / x);
		result *= result;
	}
	if (y != 0.0)
	{
		result *= (sin(y) / y);
		result *= result;
	}

	if (rBuffer != null)
	{
		rBuffer[re] *= result;
		rBuffer[im] *= result;
	}
	if (gBuffer != null)
	{
		gBuffer[re] *= result;
		gBuffer[im] *= result;
	}
	if (bBuffer != null)
	{
		bBuffer[re] *= result;
		bBuffer[im] *= result;
	}
}

void b3Fourier::b3Filter(b3FilterInfo *info,b3FilterFunc filter_func)
{
	b3_f64   fx,fy;
	b3_loop  x,xHalf = m_xSize >> 1,xMask = xHalf - 1,xQuart = m_xSize >> 2;
	b3_loop  y,yHalf = m_ySize >> 1,yMask = m_ySize - 1;
	b3_index index   = 0;

	index = 0;
	for (y = 0;y < m_ySize;y++)
	{
		fy = (b3_f64)((y + yHalf) & yMask) / (b3_f64)yHalf - 1.0;
		for (x = 0;x < xHalf;x++)
		{
			fx = (b3_f64)((x + xQuart) & xMask) / (b3_f64)xQuart - 1.0;
			filter_func(fx,fy,index + x,index + x + xHalf,this,info);
		}
		index += m_xSize;
	}
}

void b3Fourier::b3SetBandpass(b3_f64 frequency,b3_f64 bandwidth)
{
	b3FilterInfo info;

	info.bp.start  = frequency + bandwidth * 0.5;
	info.bp.start *= info.bp.start;
	info.bp.end    = frequency - bandwidth * 0.5;
	if (info.bp.end < 0)
	{
		info.bp.end = 0;
	}
	else
	{
		info.bp.end *= info.bp.end;
	}
	b3Filter(&info,b3FilterBandpass);
}

void b3Fourier::b3SetLowpass(b3_f64 frequency)
{
	b3FilterInfo info;

	info.lp.limit = frequency * frequency;
	b3Filter(&info,b3FilterLowpass);
}

void b3Fourier::b3SetHighpass(b3_f64 frequency)
{
	b3FilterInfo info;

	info.hp.limit = frequency * frequency;
	b3Filter(&info,b3FilterHighpass);
}

void b3Fourier::b3SetAntiRaster(b3_f64 frequency,b3_f64 corridor)
{
	b3FilterInfo info;

	info.ar.limit = frequency * frequency;
	info.ar.corr  = corridor * 0.5;
	b3Filter(&info,b3FilterAntiRaster);
}

void b3Fourier::b3SetSinc2(b3_f64 frequency)
{
	b3FilterInfo info;

	info.si2.factor = M_PI / frequency;
	b3Filter(&info,b3FilterSinc2);
}

void b3Fourier::b3SetSinc2AR(b3_f64 frequency)
{
	b3FilterInfo info;

	info.si2.factor = M_PI / frequency;
	b3Filter(&info,b3FilterSinc2AR);
}

b3_loop b3Fourier::b3PowOf2(b3_loop value)
{
	b3_loop result = 1;

	// Prevent busy loop
	if (value >= 0x80000000)
	{
		return 0x80000000;
	}

	while (value > result)
	{
		result = result << 1;
	}
	return result;
}

void b3Fourier::b3Init()
{
	b3_size n;

	if (m_TempBuffer == null)
	{
		m_TempBuffer = (b3_f64 *)b3Alloc(2 * m_xSize * sizeof(b3_f64));
	}

	if (m_PermBuffer == null)
	{
	    n            = B3_MAX(m_xSize, m_ySize / 2);
		m_PermBuffer = (b3_index *)b3Alloc((2 + (b3_size)sqrt(n + 0.5)) * sizeof(b3_index));
	}
    m_PermBuffer[0] = 0;

	if (m_Values == null)
	{
		n        = B3_MAX(m_xSize * 5 / 4, m_ySize * 5 / 4) + m_ySize / 4;
		m_Values = (b3_f64 *)b3Alloc(n * sizeof(b3_f64));
	}
}

b3_f64 ** b3Fourier::b3Alloc2D()
{
	b3_f64 **linePtr;
	b3_f64  *buffer;
	long     y;

	buffer = (b3_f64 *)b3Alloc(m_xSize * m_ySize * sizeof(b3_f64));
	if (buffer == null)
	{
		throw;
	}

	linePtr = (b3_f64 **)b3Alloc(m_ySize * sizeof(b3_f64 *));
	if (linePtr == null)
	{
		throw;
	}

	for (y = 0;y < m_ySize;y++)
	{
		linePtr[y] = &buffer[y * m_xSize];
	}

	return linePtr;
}

void b3Fourier::b3FFT2D()
{
	b3PrintF(B3LOG_FULL, "b3Fourier::b3FFT2D()\n");

	b3Init();
	if (m_rLine != null)
	{
	    b3ComplexDFT2d(m_xSize, m_ySize,  1, m_rLine, m_TempBuffer, m_PermBuffer, m_Values);
	}
	if (m_gLine != null)
	{
	    b3ComplexDFT2d(m_xSize, m_ySize,  1, m_gLine, m_TempBuffer, m_PermBuffer, m_Values);
	}
	if (m_bLine != null)
	{
	    b3ComplexDFT2d(m_xSize, m_ySize,  1, m_bLine, m_TempBuffer, m_PermBuffer, m_Values);
	}
}

void b3Fourier::b3IFFT2D()
{
	b3_loop i,max = m_xSize * m_ySize;
	b3_f64 divisor = 2.0 / (b3_f64)(m_xSize * m_ySize);

	b3PrintF(B3LOG_FULL, "b3Fourier::b3IFFT2D()\n");
	b3Init();
	if (m_rLine != null)
	{
	    b3ComplexDFT2d(m_xSize, m_ySize, -1, m_rLine, m_TempBuffer, m_PermBuffer, m_Values);
		for (i = 0;i < max;i++)
		{
			m_rBuffer[i] *= divisor;
		}
	}
	if (m_gLine != null)
	{
	    b3ComplexDFT2d(m_xSize, m_ySize, -1, m_gLine, m_TempBuffer, m_PermBuffer, m_Values);
		for (i = 0;i < max;i++)
		{
			m_gBuffer[i] *= divisor;
		}
	}
	if (m_bLine != null)
	{
	    b3ComplexDFT2d(m_xSize, m_ySize, -1, m_bLine, m_TempBuffer, m_PermBuffer, m_Values);
		for (i = 0;i < max;i++)
		{
			m_bBuffer[i] *= divisor;
		}
	}
}

/*
Fast Fourier/Cosine/Sine Transform
    dimension   :one
    data length :power of 2
    decimation  :frequency
    radix       :4, 2
    data        :inplace
    table       :use
functions
    cdft: Complex Discrete Fourier Transform
    rdft: Real Discrete Fourier Transform
    ddct: Discrete Cosine Transform
    ddst: Discrete Sine Transform
    dfct: Cosine Transform of RDFT (Real Symmetric DFT)
    dfst: Sine Transform of RDFT (Real Anti-symmetric DFT)
function prototypes
    void cdft(int, int, b3_f64 *, b3_loop *, b3_f64 *);
    void rdft(int, int, b3_f64 *, b3_loop *, b3_f64 *);
    void ddct(int, int, b3_f64 *, b3_loop *, b3_f64 *);
    void ddst(int, int, b3_f64 *, b3_loop *, b3_f64 *);
    void dfct(int, b3_f64 *, b3_f64 *, b3_loop *, b3_f64 *);
    void dfst(int, b3_f64 *, b3_f64 *, b3_loop *, b3_f64 *);


-------- Complex DFT (Discrete Fourier Transform) --------
    [definition]
        <case1>
            X[k] = sum_j=0^n-1 x[j]*exp(2*pi*i*j*k/n), 0<=k<n
        <case2>
            X[k] = sum_j=0^n-1 x[j]*exp(-2*pi*i*j*k/n), 0<=k<n
        (notes: sum_j=0^n-1 is a summation from j=0 to n-1)
    [usage]
        <case1>
            ip[0] = 0; // first time only
            cdft(2*n, 1, a, ip, m_Values);
        <case2>
            ip[0] = 0; // first time only
            cdft(2*n, -1, a, ip, m_Values);
    [parameters]
        2*n            :data length (int)
                        n >= 1, n = power of 2
        a[0...2*n-1]   :input/output data (b3_f64 *)
                        input data
                            a[2*j] = Re(x[j]), 
                            a[2*j+1] = Im(x[j]), 0<=j<n
                        output data
                            a[2*k] = Re(X[k]), 
                            a[2*k+1] = Im(X[k]), 0<=k<n
        ip[0...*]      :work area for bit reversal (b3_loop *)
                        length of ip >= 2+sqrt(n)
                        strictly, 
                        length of ip >= 
                            2+(1<<(int)(log(n+0.5)/log(2))/2).
                        ip[0],ip[1] are pointers of the cos/sin table.
        w[0...n/2-1]   :cos/sin table (b3_f64 *)
                        w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            cdft(2*n, -1, a, ip, w);
        is 
            cdft(2*n, 1, a, ip, w);
            for (j = 0; j <= 2 * n - 1; j++) {
                a[j] *= 1.0 / n;
            }
        .


-------- Real DFT / Inverse of Real DFT --------
    [definition]
        <case1> RDFT
            R[k] = sum_j=0^n-1 a[j]*cos(2*pi*j*k/n), 0<=k<=n/2
            I[k] = sum_j=0^n-1 a[j]*sin(2*pi*j*k/n), 0<k<n/2
        <case2> IRDFT (excluding scale)
            a[k] = (R[0] + R[n/2]*cos(pi*k))/2 + 
                   sum_j=1^n/2-1 R[j]*cos(2*pi*j*k/n) + 
                   sum_j=1^n/2-1 I[j]*sin(2*pi*j*k/n), 0<=k<n
    [usage]
        <case1>
            ip[0] = 0; // first time only
            rdft(n, 1, a, ip, w);
        <case2>
            ip[0] = 0; // first time only
            rdft(n, -1, a, ip, w);
    [parameters]
        n              :data length (int)
                        n >= 2, n = power of 2
        a[0...n-1]     :input/output data (b3_f64 *)
                        <case1>
                            output data
                                a[2*k] = R[k], 0<=k<n/2
                                a[2*k+1] = I[k], 0<k<n/2
                                a[1] = R[n/2]
                        <case2>
                            input data
                                a[2*j] = R[j], 0<=j<n/2
                                a[2*j+1] = I[j], 0<j<n/2
                                a[1] = R[n/2]
        ip[0...*]      :work area for bit reversal (b3_loop *)
                        length of ip >= 2+sqrt(n/2)
                        strictly, 
                        length of ip >= 
                            2+(1<<(int)(log(n/2+0.5)/log(2))/2).
                        ip[0],ip[1] are pointers of the cos/sin table.
        w[0...n/2-1]   :cos/sin table (b3_f64 *)
                        w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            rdft(n, 1, a, ip, w);
        is 
            rdft(n, -1, a, ip, w);
            for (j = 0; j <= n - 1; j++) {
                a[j] *= 2.0 / n;
            }
        .


-------- DCT (Discrete Cosine Transform) / Inverse of DCT --------
    [definition]
        <case1> IDCT (excluding scale)
            C[k] = sum_j=0^n-1 a[j]*cos(pi*j*(k+1/2)/n), 0<=k<n
        <case2> DCT
            C[k] = sum_j=0^n-1 a[j]*cos(pi*(j+1/2)*k/n), 0<=k<n
    [usage]
        <case1>
            ip[0] = 0; // first time only
            ddct(n, 1, a, ip, w);
        <case2>
            ip[0] = 0; // first time only
            ddct(n, -1, a, ip, w);
    [parameters]
        n              :data length (int)
                        n >= 2, n = power of 2
        a[0...n-1]     :input/output data (b3_f64 *)
                        output data
                            a[k] = C[k], 0<=k<n
        ip[0...*]      :work area for bit reversal (b3_loop *)
                        length of ip >= 2+sqrt(n/2)
                        strictly, 
                        length of ip >= 
                            2+(1<<(int)(log(n/2+0.5)/log(2))/2).
                        ip[0],ip[1] are pointers of the cos/sin table.
        w[0...n*5/4-1] :cos/sin table (b3_f64 *)
                        w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            ddct(n, -1, a, ip, w);
        is 
            a[0] *= 0.5;
            ddct(n, 1, a, ip, w);
            for (j = 0; j <= n - 1; j++) {
                a[j] *= 2.0 / n;
            }
        .


-------- DST (Discrete Sine Transform) / Inverse of DST --------
    [definition]
        <case1> IDST (excluding scale)
            S[k] = sum_j=1^n A[j]*sin(pi*j*(k+1/2)/n), 0<=k<n
        <case2> DST
            S[k] = sum_j=0^n-1 a[j]*sin(pi*(j+1/2)*k/n), 0<k<=n
    [usage]
        <case1>
            ip[0] = 0; // first time only
            ddst(n, 1, a, ip, w);
        <case2>
            ip[0] = 0; // first time only
            ddst(n, -1, a, ip, w);
    [parameters]
        n              :data length (int)
                        n >= 2, n = power of 2
        a[0...n-1]     :input/output data (b3_f64 *)
                        <case1>
                            input data
                                a[j] = A[j], 0<j<n
                                a[0] = A[n]
                            output data
                                a[k] = S[k], 0<=k<n
                        <case2>
                            output data
                                a[k] = S[k], 0<k<n
                                a[0] = S[n]
        ip[0...*]      :work area for bit reversal (b3_loop *)
                        length of ip >= 2+sqrt(n/2)
                        strictly, 
                        length of ip >= 
                            2+(1<<(int)(log(n/2+0.5)/log(2))/2).
                        ip[0],ip[1] are pointers of the cos/sin table.
        w[0...n*5/4-1] :cos/sin table (b3_f64 *)
                        w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            ddst(n, -1, a, ip, w);
        is 
            a[0] *= 0.5;
            ddst(n, 1, a, ip, w);
            for (j = 0; j <= n - 1; j++) {
                a[j] *= 2.0 / n;
            }
        .


-------- Cosine Transform of RDFT (Real Symmetric DFT) --------
    [definition]
        C[k] = sum_j=0^n a[j]*cos(pi*j*k/n), 0<=k<=n
    [usage]
        ip[0] = 0; // first time only
        dfct(n, a, t, ip, w);
    [parameters]
        n              :data length - 1 (int)
                        n >= 2, n = power of 2
        a[0...n]       :input/output data (b3_f64 *)
                        output data
                            a[k] = C[k], 0<=k<=n
        t[0...n/2]     :work area (b3_f64 *)
        ip[0...*]      :work area for bit reversal (b3_loop *)
                        length of ip >= 2+sqrt(n/4)
                        strictly, 
                        length of ip >= 
                            2+(1<<(int)(log(n/4+0.5)/log(2))/2).
                        ip[0],ip[1] are pointers of the cos/sin table.
        w[0...n*5/8-1] :cos/sin table (b3_f64 *)
                        w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            a[0] *= 0.5;
            a[n] *= 0.5;
            dfct(n, a, t, ip, w);
        is 
            a[0] *= 0.5;
            a[n] *= 0.5;
            dfct(n, a, t, ip, w);
            for (j = 0; j <= n; j++) {
                a[j] *= 2.0 / n;
            }
        .


-------- Sine Transform of RDFT (Real Anti-symmetric DFT) --------
    [definition]
        S[k] = sum_j=1^n-1 a[j]*sin(pi*j*k/n), 0<k<n
    [usage]
        ip[0] = 0; // first time only
        dfst(n, a, t, ip, w);
    [parameters]
        n              :data length + 1 (int)
                        n >= 2, n = power of 2
        a[0...n-1]     :input/output data (b3_f64 *)
                        output data
                            a[k] = S[k], 0<k<n
                        (a[0] is used for work area)
        t[0...n/2-1]   :work area (b3_f64 *)
        ip[0...*]      :work area for bit reversal (b3_loop *)
                        length of ip >= 2+sqrt(n/4)
                        strictly, 
                        length of ip >= 
                            2+(1<<(int)(log(n/4+0.5)/log(2))/2).
                        ip[0],ip[1] are pointers of the cos/sin table.
        w[0...n*5/8-1] :cos/sin table (b3_f64 *)
                        w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            dfst(n, a, t, ip, w);
        is 
            dfst(n, a, t, ip, w);
            for (j = 1; j <= n - 1; j++) {
                a[j] *= 2.0 / n;
            }
        .


Appendix :
    The cos/sin table is recalculated when the larger table required.
    w[] and ip[] are compatible with all routines.
*/


void b3Fourier::b3ComplexDFT(b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w)
{
	if (n > (ip[0] << 2))
	{
		b3MakeWT(n >> 2, ip, w);
	}
	if (n > 4)
	{
		if (isgn >= 0)
		{
			b3BitRV2(n, ip + 2, a);
			b3ComplexFTfsub(n, a, w);
		}
		else
		{
			b3BitRV2Conj(n, ip + 2, a);
			b3ComplexFTbsub(n, a, w);
		}
	}
	else if (n == 4)
	{
		b3ComplexFTfsub(n, a, w);
	}
}


void b3Fourier::b3RealDFT(b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w)
{
	b3_loop nw, nc;
	b3_f64 xi;

	nw = ip[0];
	if (n > (nw << 2))
	{
		nw = n >> 2;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n > (nc << 2)) {
		nc = n >> 2;
		b3MakeCT(nc, ip, w + nw);
	}
	if (isgn >= 0)
	{
		if (n > 4)
		{
			b3BitRV2(n, ip + 2, a);
			b3ComplexFTfsub(n, a, w);
			b3RealFTfsub(n, a, nc, w + nw);
		}
		else if (n == 4)
		{
			b3ComplexFTfsub(n, a, w);
		}
		xi    = a[0] - a[1];
		a[0] += a[1];
		a[1]  = xi;
	}
	else
	{
		a[1] = 0.5 * (a[0] - a[1]);
		a[0] -= a[1];
		if (n > 4)
		{
			b3RealFTbsub(n, a, nc, w + nw);
			b3BitRV2(n, ip + 2, a);
			b3ComplexFTbsub(n, a, w);
		}
		else if (n == 4)
		{
			b3ComplexFTfsub(n, a, w);
		}
	}
}


void b3Fourier::b3DCT(b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w)
{
	b3_loop j, nw, nc;
	b3_f64 xr;

	nw = ip[0];
	if (n > (nw << 2))
	{
		nw = n >> 2;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n > nc)
	{
		nc = n;
		b3MakeCT(nc, ip, w + nw);
	}
	if (isgn < 0)
	{
		xr = a[n - 1];
		for (j = n - 2; j >= 2; j -= 2)
		{
			a[j + 1] = a[j] - a[j - 1];
			a[j] += a[j - 1];
		}
		a[1] = a[0] - xr;
		a[0] += xr;
		if (n > 4)
		{
			b3RealFTbsub(n, a, nc, w + nw);
			b3BitRV2(n, ip + 2, a);
			b3ComplexFTbsub(n, a, w);
		}
		else if (n == 4)
		{
			b3ComplexFTfsub(n, a, w);
		}
	}
	b3DCTsub(n, a, nc, w + nw);
	if (isgn >= 0)
	{
		if (n > 4)
		{
			b3BitRV2(n, ip + 2, a);
			b3ComplexFTfsub(n, a, w);
			b3RealFTfsub(n, a, nc, w + nw);
		}
		else if (n == 4)
		{
			b3ComplexFTfsub(n, a, w);
		}
		xr = a[0] - a[1];
		a[0] += a[1];
		for (j = 2; j < n; j += 2)
		{
			a[j - 1] = a[j] - a[j + 1];
			a[j] += a[j + 1];
		}
		a[n - 1] = xr;
	}
}


void b3Fourier::b3DST(b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w)
{
	b3_loop j, nw, nc;
	b3_f64 xr;

	nw = ip[0];
	if (n > (nw << 2))
	{
		nw = n >> 2;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n > nc)
	{
		nc = n;
		b3MakeCT(nc, ip, w + nw);
	}
	if (isgn < 0)
	{
		xr = a[n - 1];
		for (j = n - 2; j >= 2; j -= 2)
		{
			a[j + 1] = -a[j] - a[j - 1];
			a[j] -= a[j - 1];
		}
		a[1] = a[0] + xr;
		a[0] -= xr;
		if (n > 4)
		{
			b3RealFTbsub(n, a, nc, w + nw);
			b3BitRV2(n, ip + 2, a);
			b3ComplexFTbsub(n, a, w);
		}
		else if (n == 4)
		{
			b3ComplexFTfsub(n, a, w);
		}
	}
	b3DSTsub(n, a, nc, w + nw);
	if (isgn >= 0)
	{
		if (n > 4)
		{
			b3BitRV2(n, ip + 2, a);
			b3ComplexFTfsub(n, a, w);
			b3RealFTfsub(n, a, nc, w + nw);
		}
		else if (n == 4)
		{
			b3ComplexFTfsub(n, a, w);
		}
		xr = a[0] - a[1];
		a[0] += a[1];
		for (j = 2; j < n; j += 2)
		{
			a[j - 1] = -a[j] - a[j + 1];
			a[j] -= a[j + 1];
		}
		a[n - 1] = -xr;
	}
}


void b3Fourier::b3FourierDCT(b3_loop n, b3_f64 *a, b3_f64 *t, b3_index *ip, b3_f64 *w)
{
	b3_loop j, k, l, m, mh, nw, nc;
	b3_f64 xr, xi, yr, yi;

	nw = ip[0];
	if (n > (nw << 3))
	{
		nw = n >> 3;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n > (nc << 1))
	{
		nc = n >> 1;
		b3MakeCT(nc, ip, w + nw);
	}
	m = n >> 1;
	yi = a[m];
	xi = a[0] + a[n];
	a[0] -= a[n];
	t[0] = xi - yi;
	t[m] = xi + yi;
	if (n > 2)
	{
		mh = m >> 1;
		for (j = 1; j < mh; j++)
		{
			k = m - j;
			xr = a[j] - a[n - j];
			xi = a[j] + a[n - j];
			yr = a[k] - a[n - k];
			yi = a[k] + a[n - k];
			a[j] = xr;
			a[k] = yr;
			t[j] = xi - yi;
			t[k] = xi + yi;
		}
		t[mh] = a[mh] + a[n - mh];
		a[mh] -= a[n - mh];
		b3DCTsub(m, a, nc, w + nw);
		if (m > 4)
		{
			b3BitRV2(m, ip + 2, a);
			b3ComplexFTfsub(m, a, w);
			b3RealFTfsub(m, a, nc, w + nw);
		}
		else if (m == 4)
		{
			b3ComplexFTfsub(m, a, w);
		}
		a[n - 1] = a[0] - a[1];
		a[1] = a[0] + a[1];
		for (j = m - 2; j >= 2; j -= 2)
		{
			a[2 * j + 1] = a[j] + a[j + 1];
			a[2 * j - 1] = a[j] - a[j + 1];
		}
		l = 2;
		m = mh;
		while (m >= 2)
		{
			b3DCTsub(m, t, nc, w + nw);
			if (m > 4)
			{
				b3BitRV2(m, ip + 2, t);
				b3ComplexFTfsub(m, t, w);
				b3RealFTfsub(m, t, nc, w + nw);
			}
			else if (m == 4)
			{
				b3ComplexFTfsub(m, t, w);
			}
			a[n - l] = t[0] - t[1];
			a[l] = t[0] + t[1];
			k = 0;
			for (j = 2; j < m; j += 2)
			{
				k += l << 2;
				a[k - l] = t[j] - t[j + 1];
				a[k + l] = t[j] + t[j + 1];
			}
			l <<= 1;
			mh = m >> 1;
			for (j = 0; j < mh; j++)
			{
				k = m - j;
				t[j] = t[m + k] - t[m + j];
				t[k] = t[m + k] + t[m + j];
			}
			t[mh] = t[m + mh];
			m = mh;
		}
		a[l] = t[0];
		a[n] = t[2] - t[1];
		a[0] = t[2] + t[1];
		} else {
		a[1] = a[0];
		a[2] = t[0];
		a[0] = t[1];
	}
}


void b3Fourier::b3FourierDST(b3_loop n, b3_f64 *a, b3_f64 *t, b3_index *ip, b3_f64 *w)
{
	b3_loop j, k, l, m, mh, nw, nc;
	b3_f64 xr, xi, yr, yi;

	nw = ip[0];
	if (n > (nw << 3))
	{
		nw = n >> 3;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n > (nc << 1))
	{
		nc = n >> 1;
		b3MakeCT(nc, ip, w + nw);
	}
	if (n > 2)
	{
		m = n >> 1;
		mh = m >> 1;
		for (j = 1; j < mh; j++)
		{
			k = m - j;
			xr = a[j] + a[n - j];
			xi = a[j] - a[n - j];
			yr = a[k] + a[n - k];
			yi = a[k] - a[n - k];
			a[j] = xr;
			a[k] = yr;
			t[j] = xi + yi;
			t[k] = xi - yi;
		}
		t[0] = a[mh] - a[n - mh];
		a[mh] += a[n - mh];
		a[0] = a[m];
		b3DSTsub(m, a, nc, w + nw);
		if (m > 4)
		{
			b3BitRV2(m, ip + 2, a);
			b3ComplexFTfsub(m, a, w);
			b3RealFTfsub(m, a, nc, w + nw);
		}
		else if (m == 4)
		{
			b3ComplexFTfsub(m, a, w);
		}
		a[n - 1] = a[1] - a[0];
		a[1] = a[0] + a[1];
		for (j = m - 2; j >= 2; j -= 2)
		{
			a[2 * j + 1] = a[j] - a[j + 1];
			a[2 * j - 1] = -a[j] - a[j + 1];
		}
		l = 2;
		m = mh;
		while (m >= 2)
		{
			b3DSTsub(m, t, nc, w + nw);
			if (m > 4)
			{
				b3BitRV2(m, ip + 2, t);
				b3ComplexFTfsub(m, t, w);
				b3RealFTfsub(m, t, nc, w + nw);
			}
			else if (m == 4)
			{
				b3ComplexFTfsub(m, t, w);
			}
			a[n - l] = t[1] - t[0];
			a[l] = t[0] + t[1];
			k = 0;
			for (j = 2; j < m; j += 2)
			{
				k += l << 2;
				a[k - l] = -t[j] - t[j + 1];
				a[k + l] = t[j] - t[j + 1];
			}
			l <<= 1;
			mh = m >> 1;
			for (j = 1; j < mh; j++)
			{
				k = m - j;
				t[j] = t[m + k] + t[m + j];
				t[k] = t[m + k] - t[m + j];
			}
			t[0] = t[m + mh];
			m = mh;
		}
		a[l] = t[0];
	}
	a[0] = 0;
}


/* -------- initializing routines -------- */


void b3Fourier::b3MakeWT(b3_loop nw, b3_index *ip, b3_f64 *w)
{
	b3_loop    j, nwh;
	b3_f64 delta, x, y;

	ip[0] = nw;
	ip[1] = 1;
	if (nw > 2)
	{
		nwh        = nw >> 1;
		delta      = atan(1.0) / nwh;
		w[0]       = 1;
		w[1]       = 0;
		w[nwh]     = cos(delta * nwh);
		w[nwh + 1] = w[nwh];
		if (nwh > 2)
		{
			for (j = 2; j < nwh; j += 2)
			{
				x             = cos(delta * j);
				y             = sin(delta * j);
				w[j]          = x;
				w[j + 1]      = y;
				w[nw - j]     = y;
				w[nw - j + 1] = x;
			}
			b3BitRV2(nw, ip + 2, w);
		}
	}
}


void b3Fourier::b3MakeCT(b3_loop nc, b3_index *ip, b3_f64 *c)
{
	b3_loop    j, nch;
	b3_f64 delta;

	ip[1] = nc;
	if (nc > 1)
	{
		nch    = nc >> 1;
		delta  = atan(1.0) / nch;
		c[0]   = cos(delta * nch);
		c[nch] = 0.5 * c[0];
		for (j = 1; j < nch; j++)
		{
			c[j]      = 0.5 * cos(delta * j);
			c[nc - j] = 0.5 * sin(delta * j);
		}
	}
}


/* -------- child routines -------- */


void b3Fourier::b3BitRV2(b3_loop n, b3_index *ip, b3_f64 *a)
{
	b3_loop j, j1, k, k1, l, m, m2;
	b3_f64 xr, xi, yr, yi;

	ip[0] = 0;
	l = n;
	m = 1;
	while ((m << 3) < l)
	{
		l >>= 1;
		for (j = 0; j < m; j++)
		{
			ip[m + j] = ip[j] + l;
		}
		m <<= 1;
	}
	m2 = 2 * m;
	if ((m << 3) == l)
	{
		for (k = 0; k < m; k++)
		{
			for (j = 0; j < k; j++)
			{
				j1 = 2 * j + ip[k];
				k1 = 2 * k + ip[j];
				xr = a[j1];
				xi = a[j1 + 1];
				yr = a[k1];
				yi = a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 += 2 * m2;
				xr = a[j1];
				xi = a[j1 + 1];
				yr = a[k1];
				yi = a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 -= m2;
				xr = a[j1];
				xi = a[j1 + 1];
				yr = a[k1];
				yi = a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 += 2 * m2;
				xr = a[j1];
				xi = a[j1 + 1];
				yr = a[k1];
				yi = a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
			}
			j1 = 2 * k + m2 + ip[k];
			k1 = j1 + m2;
			xr = a[j1];
			xi = a[j1 + 1];
			yr = a[k1];
			yi = a[k1 + 1];
			a[j1] = yr;
			a[j1 + 1] = yi;
			a[k1] = xr;
			a[k1 + 1] = xi;
		}
	}
	else
	{
		for (k = 1; k < m; k++)
		{
			for (j = 0; j < k; j++)
			{
				j1 = 2 * j + ip[k];
				k1 = 2 * k + ip[j];
				xr = a[j1];
				xi = a[j1 + 1];
				yr = a[k1];
				yi = a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 += m2;
				xr = a[j1];
				xi = a[j1 + 1];
				yr = a[k1];
				yi = a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
			}
		}
	}
}


void b3Fourier::b3BitRV2Conj(b3_loop n, b3_index *ip, b3_f64 *a)
{
	b3_loop j, j1, k, k1, l, m, m2;
	b3_f64 xr, xi, yr, yi;

	ip[0] = 0;
	l = n;
	m = 1;
	while ((m << 3) < l)
	{
		l >>= 1;
		for (j = 0; j < m; j++)
		{
			ip[m + j] = ip[j] + l;
		}
		m <<= 1;
	}
	m2 = 2 * m;
	if ((m << 3) == l)
	{
		for (k = 0; k < m; k++)
		{
			for (j = 0; j < k; j++)
			{
				j1 = 2 * j + ip[k];
				k1 = 2 * k + ip[j];
				xr = a[j1];
				xi = -a[j1 + 1];
				yr = a[k1];
				yi = -a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 += 2 * m2;
				xr = a[j1];
				xi = -a[j1 + 1];
				yr = a[k1];
				yi = -a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 -= m2;
				xr = a[j1];
				xi = -a[j1 + 1];
				yr = a[k1];
				yi = -a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 += 2 * m2;
				xr = a[j1];
				xi = -a[j1 + 1];
				yr = a[k1];
				yi = -a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
			}
			k1 = 2 * k + ip[k];
			a[k1 + 1] = -a[k1 + 1];
			j1 = k1 + m2;
			k1 = j1 + m2;
			xr = a[j1];
			xi = -a[j1 + 1];
			yr = a[k1];
			yi = -a[k1 + 1];
			a[j1] = yr;
			a[j1 + 1] = yi;
			a[k1] = xr;
			a[k1 + 1] = xi;
			k1 += m2;
			a[k1 + 1] = -a[k1 + 1];
		}
	}
	else
	{
		a[1] = -a[1];
		a[m2 + 1] = -a[m2 + 1];
		for (k = 1; k < m; k++)
		{
			for (j = 0; j < k; j++)
			{
				j1 = 2 * j + ip[k];
				k1 = 2 * k + ip[j];
				xr = a[j1];
				xi = -a[j1 + 1];
				yr = a[k1];
				yi = -a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
				j1 += m2;
				k1 += m2;
				xr = a[j1];
				xi = -a[j1 + 1];
				yr = a[k1];
				yi = -a[k1 + 1];
				a[j1] = yr;
				a[j1 + 1] = yi;
				a[k1] = xr;
				a[k1 + 1] = xi;
			}
			k1 = 2 * k + ip[k];
			a[k1 + 1] = -a[k1 + 1];
			a[k1 + m2 + 1] = -a[k1 + m2 + 1];
		}
	}
}


void b3Fourier::b3ComplexFTfsub(b3_loop n, b3_f64 *a, b3_f64 *w)
{
	b3_loop j, j1, j2, j3, l;
	b3_f64 x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

	l = 2;
	if (n > 8)
	{
		b3ComplexFT1st(n, a, w);
		l = 8;
		while ((l << 2) < n)
		{
			b3ComplexFTmdl(n, l, a, w);
			l <<= 2;
		}
	}
	if ((l << 2) == n)
	{
		for (j = 0; j < l; j += 2)
		{
			j1 = j + l;
			j2 = j1 + l;
			j3 = j2 + l;
			x0r = a[j] + a[j1];
			x0i = a[j + 1] + a[j1 + 1];
			x1r = a[j] - a[j1];
			x1i = a[j + 1] - a[j1 + 1];
			x2r = a[j2] + a[j3];
			x2i = a[j2 + 1] + a[j3 + 1];
			x3r = a[j2] - a[j3];
			x3i = a[j2 + 1] - a[j3 + 1];
			a[j] = x0r + x2r;
			a[j + 1] = x0i + x2i;
			a[j2] = x0r - x2r;
			a[j2 + 1] = x0i - x2i;
			a[j1] = x1r - x3i;
			a[j1 + 1] = x1i + x3r;
			a[j3] = x1r + x3i;
			a[j3 + 1] = x1i - x3r;
		}
	}
	else
	{
		for (j = 0; j < l; j += 2)
		{
			j1 = j + l;
			x0r = a[j] - a[j1];
			x0i = a[j + 1] - a[j1 + 1];
			a[j] += a[j1];
			a[j + 1] += a[j1 + 1];
			a[j1] = x0r;
			a[j1 + 1] = x0i;
		}
	}
}


void b3Fourier::b3ComplexFTbsub(b3_loop n, b3_f64 *a, b3_f64 *w)
{
	b3_loop j, j1, j2, j3, l;
	b3_f64 x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

	l = 2;
	if (n > 8)
	{
		b3ComplexFT1st(n, a, w);
		l = 8;
		while ((l << 2) < n)
		{
			b3ComplexFTmdl(n, l, a, w);
			l <<= 2;
		}
	}
	if ((l << 2) == n)
	{
		for (j = 0; j < l; j += 2)
		{
			j1 = j + l;
			j2 = j1 + l;
			j3 = j2 + l;
			x0r = a[j] + a[j1];
			x0i = -a[j + 1] - a[j1 + 1];
			x1r = a[j] - a[j1];
			x1i = -a[j + 1] + a[j1 + 1];
			x2r = a[j2] + a[j3];
			x2i = a[j2 + 1] + a[j3 + 1];
			x3r = a[j2] - a[j3];
			x3i = a[j2 + 1] - a[j3 + 1];
			a[j] = x0r + x2r;
			a[j + 1] = x0i - x2i;
			a[j2] = x0r - x2r;
			a[j2 + 1] = x0i + x2i;
			a[j1] = x1r - x3i;
			a[j1 + 1] = x1i - x3r;
			a[j3] = x1r + x3i;
			a[j3 + 1] = x1i + x3r;
		}
	}
	else
	{
		for (j = 0; j < l; j += 2)
		{
			j1 = j + l;
			x0r = a[j] - a[j1];
			x0i = -a[j + 1] + a[j1 + 1];
			a[j] += a[j1];
			a[j + 1] = -a[j + 1] - a[j1 + 1];
			a[j1] = x0r;
			a[j1 + 1] = x0i;
		}
	}
}


void b3Fourier::b3ComplexFT1st(b3_loop n, b3_f64 *a, b3_f64 *w)
{
	b3_loop    j, k1, k2;
	b3_f64 wk1r, wk1i, wk2r, wk2i, wk3r, wk3i;
	b3_f64 x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

	x0r = a[0] + a[2];
	x0i = a[1] + a[3];
	x1r = a[0] - a[2];
	x1i = a[1] - a[3];
	x2r = a[4] + a[6];
	x2i = a[5] + a[7];
	x3r = a[4] - a[6];
	x3i = a[5] - a[7];
	a[0] = x0r + x2r;
	a[1] = x0i + x2i;
	a[4] = x0r - x2r;
	a[5] = x0i - x2i;
	a[2] = x1r - x3i;
	a[3] = x1i + x3r;
	a[6] = x1r + x3i;
	a[7] = x1i - x3r;
	wk1r = w[2];
	x0r = a[8] + a[10];
	x0i = a[9] + a[11];
	x1r = a[8] - a[10];
	x1i = a[9] - a[11];
	x2r = a[12] + a[14];
	x2i = a[13] + a[15];
	x3r = a[12] - a[14];
	x3i = a[13] - a[15];
	a[8] = x0r + x2r;
	a[9] = x0i + x2i;
	a[12] = x2i - x0i;
	a[13] = x0r - x2r;
	x0r = x1r - x3i;
	x0i = x1i + x3r;
	a[10] = wk1r * (x0r - x0i);
	a[11] = wk1r * (x0r + x0i);
	x0r = x3i + x1r;
	x0i = x3r - x1i;
	a[14] = wk1r * (x0i - x0r);
	a[15] = wk1r * (x0i + x0r);
	k1 = 0;
	for (j = 16; j < n; j += 16)
	{
		k1 += 2;
		k2 = 2 * k1;
		wk2r = w[k1];
		wk2i = w[k1 + 1];
		wk1r = w[k2];
		wk1i = w[k2 + 1];
		wk3r = wk1r - 2 * wk2i * wk1i;
		wk3i = 2 * wk2i * wk1r - wk1i;
		x0r = a[j] + a[j + 2];
		x0i = a[j + 1] + a[j + 3];
		x1r = a[j] - a[j + 2];
		x1i = a[j + 1] - a[j + 3];
		x2r = a[j + 4] + a[j + 6];
		x2i = a[j + 5] + a[j + 7];
		x3r = a[j + 4] - a[j + 6];
		x3i = a[j + 5] - a[j + 7];
		a[j] = x0r + x2r;
		a[j + 1] = x0i + x2i;
		x0r -= x2r;
		x0i -= x2i;
		a[j + 4] = wk2r * x0r - wk2i * x0i;
		a[j + 5] = wk2r * x0i + wk2i * x0r;
		x0r = x1r - x3i;
		x0i = x1i + x3r;
		a[j + 2] = wk1r * x0r - wk1i * x0i;
		a[j + 3] = wk1r * x0i + wk1i * x0r;
		x0r = x1r + x3i;
		x0i = x1i - x3r;
		a[j + 6] = wk3r * x0r - wk3i * x0i;
		a[j + 7] = wk3r * x0i + wk3i * x0r;
		wk1r = w[k2 + 2];
		wk1i = w[k2 + 3];
		wk3r = wk1r - 2 * wk2r * wk1i;
		wk3i = 2 * wk2r * wk1r - wk1i;
		x0r = a[j + 8] + a[j + 10];
		x0i = a[j + 9] + a[j + 11];
		x1r = a[j + 8] - a[j + 10];
		x1i = a[j + 9] - a[j + 11];
		x2r = a[j + 12] + a[j + 14];
		x2i = a[j + 13] + a[j + 15];
		x3r = a[j + 12] - a[j + 14];
		x3i = a[j + 13] - a[j + 15];
		a[j + 8]   = x0r + x2r;
		a[j + 9]   = x0i + x2i;
		x0r       -= x2r;
		x0i       -= x2i;
		a[j + 12]  = -wk2i * x0r - wk2r * x0i;
		a[j + 13]  = -wk2i * x0i + wk2r * x0r;
		x0r        = x1r - x3i;
		x0i        = x1i + x3r;
		a[j + 10]  = wk1r * x0r - wk1i * x0i;
		a[j + 11]  = wk1r * x0i + wk1i * x0r;
		x0r        = x1r + x3i;
		x0i        = x1i - x3r;
		a[j + 14]  = wk3r * x0r - wk3i * x0i;
		a[j + 15]  = wk3r * x0i + wk3i * x0r;
	}
}


void b3Fourier::b3ComplexFTmdl(b3_loop n, b3_loop l, b3_f64 *a, b3_f64 *w)
{
	b3_loop j, j1, j2, j3, k, k1, k2, m, m2;
	b3_f64 wk1r, wk1i, wk2r, wk2i, wk3r, wk3i;
	b3_f64 x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i;

	m = l << 2;
	for (j = 0; j < l; j += 2)
	{
		j1        = j + l;
		j2        = j1 + l;
		j3        = j2 + l;
		x0r       = a[j] + a[j1];
		x0i       = a[j + 1] + a[j1 + 1];
		x1r       = a[j] - a[j1];
		x1i       = a[j + 1] - a[j1 + 1];
		x2r       = a[j2] + a[j3];
		x2i       = a[j2 + 1] + a[j3 + 1];
		x3r       = a[j2] - a[j3];
		x3i       = a[j2 + 1] - a[j3 + 1];
		a[j]      = x0r + x2r;
		a[j + 1]  = x0i + x2i;
		a[j2]     = x0r - x2r;
		a[j2 + 1] = x0i - x2i;
		a[j1]     = x1r - x3i;
		a[j1 + 1] = x1i + x3r;
		a[j3]     = x1r + x3i;
		a[j3 + 1] = x1i - x3r;
	}
	wk1r = w[2];
	for (j = m; j < l + m; j += 2)
	{
		j1        = j + l;
		j2        = j1 + l;
		j3        = j2 + l;
		x0r       = a[j] + a[j1];
		x0i       = a[j + 1] + a[j1 + 1];
		x1r       = a[j] - a[j1];
		x1i       = a[j + 1] - a[j1 + 1];
		x2r       = a[j2] + a[j3];
		x2i       = a[j2 + 1] + a[j3 + 1];
		x3r       = a[j2] - a[j3];
		x3i       = a[j2 + 1] - a[j3 + 1];
		a[j]      = x0r + x2r;
		a[j + 1]  = x0i + x2i;
		a[j2]     = x2i - x0i;
		a[j2 + 1] = x0r - x2r;
		x0r       = x1r - x3i;
		x0i       = x1i + x3r;
		a[j1]     = wk1r * (x0r - x0i);
		a[j1 + 1] = wk1r * (x0r + x0i);
		x0r       = x3i + x1r;
		x0i       = x3r - x1i;
		a[j3]     = wk1r * (x0i - x0r);
		a[j3 + 1] = wk1r * (x0i + x0r);
	}
	k1 = 0;
	m2 = 2 * m;
	for (k = m2; k < n; k += m2)
	{
		k1   += 2;
		k2    = 2 * k1;
		wk2r  = w[k1];
		wk2i  = w[k1 + 1];
		wk1r  = w[k2];
		wk1i  = w[k2 + 1];
		wk3r  = wk1r - 2 * wk2i * wk1i;
		wk3i  = 2 * wk2i * wk1r - wk1i;
		for (j = k; j < l + k; j += 2)
		{
			j1  = j + l;
			j2  = j1 + l;
			j3  = j2 + l;
			x0r = a[j] + a[j1];
			x0i = a[j + 1] + a[j1 + 1];
			x1r = a[j] - a[j1];
			x1i = a[j + 1] - a[j1 + 1];
			x2r = a[j2] + a[j3];
			x2i = a[j2 + 1] + a[j3 + 1];
			x3r = a[j2] - a[j3];
			x3i = a[j2 + 1] - a[j3 + 1];
			a[j] = x0r + x2r;
			a[j + 1] = x0i + x2i;
			x0r -= x2r;
			x0i -= x2i;
			a[j2] = wk2r * x0r - wk2i * x0i;
			a[j2 + 1] = wk2r * x0i + wk2i * x0r;
			x0r = x1r - x3i;
			x0i = x1i + x3r;
			a[j1] = wk1r * x0r - wk1i * x0i;
			a[j1 + 1] = wk1r * x0i + wk1i * x0r;
			x0r = x1r + x3i;
			x0i = x1i - x3r;
			a[j3] = wk3r * x0r - wk3i * x0i;
			a[j3 + 1] = wk3r * x0i + wk3i * x0r;
		}
		wk1r = w[k2 + 2];
		wk1i = w[k2 + 3];
		wk3r = wk1r - 2 * wk2r * wk1i;
		wk3i = 2 * wk2r * wk1r - wk1i;
		for (j = k + m; j < l + (k + m); j += 2)
		{
			j1 = j + l;
			j2 = j1 + l;
			j3 = j2 + l;
			x0r = a[j] + a[j1];
			x0i = a[j + 1] + a[j1 + 1];
			x1r = a[j] - a[j1];
			x1i = a[j + 1] - a[j1 + 1];
			x2r = a[j2] + a[j3];
			x2i = a[j2 + 1] + a[j3 + 1];
			x3r = a[j2] - a[j3];
			x3i = a[j2 + 1] - a[j3 + 1];
			a[j] = x0r + x2r;
			a[j + 1] = x0i + x2i;
			x0r -= x2r;
			x0i -= x2i;
			a[j2] = -wk2i * x0r - wk2r * x0i;
			a[j2 + 1] = -wk2i * x0i + wk2r * x0r;
			x0r = x1r - x3i;
			x0i = x1i + x3r;
			a[j1] = wk1r * x0r - wk1i * x0i;
			a[j1 + 1] = wk1r * x0i + wk1i * x0r;
			x0r = x1r + x3i;
			x0i = x1i - x3r;
			a[j3] = wk3r * x0r - wk3i * x0i;
			a[j3 + 1] = wk3r * x0i + wk3i * x0r;
		}
	}
}


void b3Fourier::b3RealFTfsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c)
{
	b3_loop j, k, kk, ks, m;
	b3_f64 wkr, wki, xr, xi, yr, yi;

	m = n >> 1;
	ks = 2 * nc / m;
	kk = 0;
	for (j = 2; j < m; j += 2)
	{
		k         = n - j;
		kk       += ks;
		wkr       = 0.5 - c[nc - kk];
		wki       = c[kk];
		xr        = a[j] - a[k];
		xi        = a[j + 1] + a[k + 1];
		yr        = wkr * xr - wki * xi;
		yi        = wkr * xi + wki * xr;
		a[j]     -= yr;
		a[j + 1] -= yi;
		a[k]     += yr;
		a[k + 1] -= yi;
	}
}


void b3Fourier::b3RealFTbsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c)
{
    b3_loop j, k, kk, ks, m;
    b3_f64 wkr, wki, xr, xi, yr, yi;
    
    a[1] = -a[1];
    m = n >> 1;
    ks = 2 * nc / m;
    kk = 0;
    for (j = 2; j < m; j += 2) {
        k = n - j;
        kk += ks;
        wkr = 0.5 - c[nc - kk];
        wki = c[kk];
        xr = a[j] - a[k];
        xi = a[j + 1] + a[k + 1];
        yr = wkr * xr + wki * xi;
        yi = wkr * xi - wki * xr;
        a[j] -= yr;
        a[j + 1] = yi - a[j + 1];
        a[k] += yr;
        a[k + 1] = yi - a[k + 1];
    }
    a[m + 1] = -a[m + 1];
}


void b3Fourier::b3DCTsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c)
{
	b3_loop j, k, kk, ks, m;
	b3_f64 wkr, wki, xr;

	m  = n >> 1;
	ks = nc / n;
	kk = 0;
	for (j = 1; j < m; j++)
	{
		k     = n - j;
		kk   += ks;
		wkr   = c[kk] - c[nc - kk];
		wki   = c[kk] + c[nc - kk];
		xr    = wki * a[j] - wkr * a[k];
		a[j]  = wkr * a[j] + wki * a[k];
		a[k]  = xr;
	}
	a[m] *= c[0];
}


void b3Fourier::b3DSTsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c)
{
	b3_loop j, k, kk, ks, m;
	b3_f64 wkr, wki, xr;

	m  = n >> 1;
	ks = nc / n;
	kk = 0;
	for (j = 1; j < m; j++)
	{
		k     = n - j;
		kk   += ks;
		wkr   = c[kk] - c[nc - kk];
		wki   = c[kk] + c[nc - kk];
		xr    = wki * a[k] - wkr * a[j];
		a[k]  = wkr * a[k] + wki * a[j];
		a[j]  = xr;
	}
	a[m] *= c[0];
}

/*
Fast Fourier/Cosine/Sine Transform
    dimension   :two
    data length :power of 2
    decimation  :frequency
    radix       :*, row-column
    data        :inplace
    table       :use
functions
    cdft2d: Complex Discrete Fourier Transform
    rdft2d: Real Discrete Fourier Transform
    ddct2d: Discrete Cosine Transform
    ddst2d: Discrete Sine Transform
function prototypes
    void cdft2d(int, int, int, b3_f64 **, b3_f64 *, b3_loop *, b3_f64 *);
    void rdft2d(int, int, int, b3_f64 **, b3_f64 *, b3_loop *, b3_f64 *);
    void ddct2d(int, int, int, b3_f64 **, b3_f64 *, b3_loop *, b3_f64 *);
    void ddst2d(int, int, int, b3_f64 **, b3_f64 *, b3_loop *, b3_f64 *);
necessary package
    fft*g.c  : 1D-FFT package


-------- Complex DFT (Discrete Fourier Transform) --------
    [definition]
        <case1>
            X[k1][k2] = sum_j1=0^n1-1 sum_j2=0^n2-1 x[j1][j2] * 
                            exp(2*pi*i*j1*k1/n1) * 
                            exp(2*pi*i*j2*k2/n2), 0<=k1<n1, 0<=k2<n2
        <case2>
            X[k1][k2] = sum_j1=0^n1-1 sum_j2=0^n2-1 x[j1][j2] * 
                            exp(-2*pi*i*j1*k1/n1) * 
                            exp(-2*pi*i*j2*k2/n2), 0<=k1<n1, 0<=k2<n2
        (notes: sum_j=0^n-1 is a summation from j=0 to n-1)
    [usage]
        <case1>
            ip[0] = 0; // first time only
            cdft2d(n1, 2*n2, 1, a, t, ip, w);
        <case2>
            ip[0] = 0; // first time only
            cdft2d(n1, 2*n2, -1, a, t, ip, w);
    [parameters]
        n1     :data length (int)
                n1 >= 1, n1 = power of 2
        2*n2   :data length (int)
                n2 >= 1, n2 = power of 2
        a[0...n1-1][0...2*n2-1]
               :input/output data (b3_f64 **)
                input data
                    a[j1][2*j2] = Re(x[j1][j2]), 
                    a[j1][2*j2+1] = Im(x[j1][j2]), 
                    0<=j1<n1, 0<=j2<n2
                output data
                    a[k1][2*k2] = Re(X[k1][k2]), 
                    a[k1][2*k2+1] = Im(X[k1][k2]), 
                    0<=k1<n1, 0<=k2<n2
        t[0...2*n1-1]
               :work area (b3_f64 *)
        ip[0...*]
               :work area for bit reversal (b3_loop *)
                length of ip >= 2+sqrt(n)
                (n = max(n1, n2))
                ip[0],ip[1] are pointers of the cos/sin table.
        w[0...*]
               :cos/sin table (b3_f64 *)
                length of w >= max(n1/2, n2/2)
                w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            cdft2d(n1, 2*n2, -1, a, t, ip, w);
        is 
            cdft2d(n1, 2*n2, 1, a, t, ip, w);
            for (j1 = 0; j1 <= n1 - 1; j1++) {
                for (j2 = 0; j2 <= 2 * n2 - 1; j2++) {
                    a[j1][j2] *= 1.0 / (n1 * n2);
                }
            }
        .


-------- Real DFT / Inverse of Real DFT --------
    [definition]
        <case1> RDFT
            R[k1][k2] = sum_j1=0^n1-1 sum_j2=0^n2-1 a[j1][j2] * 
                            cos(2*pi*j1*k1/n1 + 2*pi*j2*k2/n2), 
                            0<=k1<n1, 0<=k2<n2
            I[k1][k2] = sum_j1=0^n1-1 sum_j2=0^n2-1 a[j1][j2] * 
                            sin(2*pi*j1*k1/n1 + 2*pi*j2*k2/n2), 
                            0<=k1<n1, 0<=k2<n2
        <case2> IRDFT (excluding scale)
            a[k1][k2] = (1/2) * sum_j1=0^n1-1 sum_j2=0^n2-1
                            (R[j1][j2] * 
                            cos(2*pi*j1*k1/n1 + 2*pi*j2*k2/n2) + 
                            I[j1][j2] * 
                            sin(2*pi*j1*k1/n1 + 2*pi*j2*k2/n2)), 
                            0<=k1<n1, 0<=k2<n2
        (notes: R[n1-k1][n2-k2] = R[k1][k2], 
                I[n1-k1][n2-k2] = -I[k1][k2], 
                R[n1-k1][0] = R[k1][0], 
                I[n1-k1][0] = -I[k1][0], 
                R[0][n2-k2] = R[0][k2], 
                I[0][n2-k2] = -I[0][k2], 
                0<k1<n1, 0<k2<n2)
    [usage]
        <case1>
            ip[0] = 0; // first time only
            rdft2d(n1, n2, 1, a, t, ip, w);
        <case2>
            ip[0] = 0; // first time only
            rdft2d(n1, n2, -1, a, t, ip, w);
    [parameters]
        n1     :data length (int)
                n1 >= 2, n1 = power of 2
        n2     :data length (int)
                n2 >= 2, n2 = power of 2
        a[0...n1-1][0...n2-1]
               :input/output data (b3_f64 **)
                <case1>
                    output data
                        a[k1][2*k2] = R[k1][k2] = R[n1-k1][n2-k2], 
                        a[k1][2*k2+1] = I[k1][k2] = -I[n1-k1][n2-k2], 
                            0<k1<n1, 0<k2<n2/2, 
                        a[0][2*k2] = R[0][k2] = R[0][n2-k2], 
                        a[0][2*k2+1] = I[0][k2] = -I[0][n2-k2], 
                            0<k2<n2/2, 
                        a[k1][0] = R[k1][0] = R[n1-k1][0], 
                        a[k1][1] = I[k1][0] = -I[n1-k1][0], 
                        a[n1-k1][1] = R[k1][n2/2] = R[n1-k1][n2/2], 
                        a[n1-k1][0] = -I[k1][n2/2] = I[n1-k1][n2/2], 
                            0<k1<n1/2, 
                        a[0][0] = R[0][0], 
                        a[0][1] = R[0][n2/2], 
                        a[n1/2][0] = R[n1/2][0], 
                        a[n1/2][1] = R[n1/2][n2/2]
                <case2>
                    input data
                        a[j1][2*j2] = R[j1][j2] = R[n1-j1][n2-j2], 
                        a[j1][2*j2+1] = I[j1][j2] = -I[n1-j1][n2-j2], 
                            0<j1<n1, 0<j2<n2/2, 
                        a[0][2*j2] = R[0][j2] = R[0][n2-j2], 
                        a[0][2*j2+1] = I[0][j2] = -I[0][n2-j2], 
                            0<j2<n2/2, 
                        a[j1][0] = R[j1][0] = R[n1-j1][0], 
                        a[j1][1] = I[j1][0] = -I[n1-j1][0], 
                        a[n1-j1][1] = R[j1][n2/2] = R[n1-j1][n2/2], 
                        a[n1-j1][0] = -I[j1][n2/2] = I[n1-j1][n2/2], 
                            0<j1<n1/2, 
                        a[0][0] = R[0][0], 
                        a[0][1] = R[0][n2/2], 
                        a[n1/2][0] = R[n1/2][0], 
                        a[n1/2][1] = R[n1/2][n2/2]
        t[0...2*n1-1]
               :work area (b3_f64 *)
        ip[0...*]
               :work area for bit reversal (b3_loop *)
                length of ip >= 2+sqrt(n)
                (n = max(n1, n2/2))
                ip[0],ip[1] are pointers of the cos/sin table.
        w[0...*]
               :cos/sin table (b3_f64 *)
                length of w >= max(n1/2, n2/4) + n2/4
                w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            rdft2d(n1, n2, 1, a, t, ip, w);
        is 
            rdft2d(n1, n2, -1, a, t, ip, w);
            for (j1 = 0; j1 <= n1 - 1; j1++) {
                for (j2 = 0; j2 <= n2 - 1; j2++) {
                    a[j1][j2] *= 2.0 / (n1 * n2);
                }
            }
        .


-------- DCT (Discrete Cosine Transform) / Inverse of DCT --------
    [definition]
        <case1> IDCT (excluding scale)
            C[k1][k2] = sum_j1=0^n1-1 sum_j2=0^n2-1 a[j1][j2] * 
                            cos(pi*j1*(k1+1/2)/n1) * 
                            cos(pi*j2*(k2+1/2)/n2), 
                            0<=k1<n1, 0<=k2<n2
        <case2> DCT
            C[k1][k2] = sum_j1=0^n1-1 sum_j2=0^n2-1 a[j1][j2] * 
                            cos(pi*(j1+1/2)*k1/n1) * 
                            cos(pi*(j2+1/2)*k2/n2), 
                            0<=k1<n1, 0<=k2<n2
    [usage]
        <case1>
            ip[0] = 0; // first time only
            ddct2d(n1, n2, 1, a, t, ip, w);
        <case2>
            ip[0] = 0; // first time only
            ddct2d(n1, n2, -1, a, t, ip, w);
    [parameters]
        n1     :data length (int)
                n1 >= 2, n1 = power of 2
        n2     :data length (int)
                n2 >= 2, n2 = power of 2
        a[0...n1-1][0...n2-1]
               :input/output data (b3_f64 **)
                output data
                    a[k1][k2] = C[k1][k2], 0<=k1<n1, 0<=k2<n2
        t[0...n1-1]
               :work area (b3_f64 *)
        ip[0...*]
               :work area for bit reversal (b3_loop *)
                length of ip >= 2+sqrt(n)
                (n = max(n1/2, n2/2))
                ip[0],ip[1] are pointers of the cos/sin table.
        w[0...*]
               :cos/sin table (b3_f64 *)
                length of w >= max(n1*5/4, n2*5/4)
                w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            ddct2d(n1, n2, -1, a, t, ip, w);
        is 
            for (j1 = 0; j1 <= n1 - 1; j1++) {
                a[j1][0] *= 0.5;
            }
            for (j2 = 0; j2 <= n2 - 1; j2++) {
                a[0][j2] *= 0.5;
            }
            ddct2d(n1, n2, 1, a, t, ip, w);
            for (j1 = 0; j1 <= n1 - 1; j1++) {
                for (j2 = 0; j2 <= n2 - 1; j2++) {
                    a[j1][j2] *= 4.0 / (n1 * n2);
                }
            }
        .


-------- DST (Discrete Sine Transform) / Inverse of DST --------
    [definition]
        <case1> IDST (excluding scale)
            S[k1][k2] = sum_j1=1^n1 sum_j2=1^n2 A[j1][j2] * 
                            sin(pi*j1*(k1+1/2)/n1) * 
                            sin(pi*j2*(k2+1/2)/n2), 
                            0<=k1<n1, 0<=k2<n2
        <case2> DST
            S[k1][k2] = sum_j1=0^n1-1 sum_j2=0^n2-1 a[j1][j2] * 
                            sin(pi*(j1+1/2)*k1/n1) * 
                            sin(pi*(j2+1/2)*k2/n2), 
                            0<k1<=n1, 0<k2<=n2
    [usage]
        <case1>
            ip[0] = 0; // first time only
            ddst2d(n1, n2, 1, a, t, ip, w);
        <case2>
            ip[0] = 0; // first time only
            ddst2d(n1, n2, -1, a, t, ip, w);
    [parameters]
        n1     :data length (int)
                n1 >= 2, n1 = power of 2
        n2     :data length (int)
                n2 >= 2, n2 = power of 2
        a[0...n1-1][0...n2-1]
               :input/output data (b3_f64 **)
                <case1>
                    input data
                        a[j1][j2] = A[j1][j2], 0<j1<n1, 0<j2<n2, 
                        a[j1][0] = A[j1][n2], 0<j1<n1, 
                        a[0][j2] = A[n1][j2], 0<j2<n2, 
                        a[0][0] = A[n1][n2]
                        (i.e. A[j1][j2] = a[j1 % n1][j2 % n2])
                    output data
                        a[k1][k2] = S[k1][k2], 0<=k1<n1, 0<=k2<n2
                <case2>
                    output data
                        a[k1][k2] = S[k1][k2], 0<k1<n1, 0<k2<n2, 
                        a[k1][0] = S[k1][n2], 0<k1<n1, 
                        a[0][k2] = S[n1][k2], 0<k2<n2, 
                        a[0][0] = S[n1][n2]
                        (i.e. S[k1][k2] = a[k1 % n1][k2 % n2])
        t[0...n1-1]
               :work area (b3_f64 *)
        ip[0...*]
               :work area for bit reversal (b3_loop *)
                length of ip >= 2+sqrt(n)
                (n = max(n1/2, n2/2))
                ip[0],ip[1] are pointers of the cos/sin table.
        w[0...*]
               :cos/sin table (b3_f64 *)
                length of w >= max(n1*5/4, n2*5/4)
                w[],ip[] are initialized if ip[0] == 0.
    [remark]
        Inverse of 
            ddst2d(n1, n2, -1, a, t, ip, w);
        is 
            for (j1 = 0; j1 <= n1 - 1; j1++) {
                a[j1][0] *= 0.5;
            }
            for (j2 = 0; j2 <= n2 - 1; j2++) {
                a[0][j2] *= 0.5;
            }
            ddst2d(n1, n2, 1, a, t, ip, w);
            for (j1 = 0; j1 <= n1 - 1; j1++) {
                for (j2 = 0; j2 <= n2 - 1; j2++) {
                    a[j1][j2] *= 4.0 / (n1 * n2);
                }
            }
        .
*/

void b3Fourier::b3ComplexDFT2d(
	b3_loop    n1,
	b3_loop    n2,
	b3_loop    isgn,
	b3_f64   **a,
	b3_f64    *t, 
    b3_index  *ip,
	b3_f64    *w)
{
	b3_loop n, i, j, i2;

	n = n1 << 1;
	if (n < n2)
	{
		n = n2;
	}
	if (n > (ip[0] << 2))
	{
		b3MakeWT(n >> 2, ip, w);
	}
	for (i = 0; i < n1; i++)
	{
		b3ComplexDFT(n2, isgn, a[i], ip, w);
	}
	for (j = 0; j <= n2 - 2; j += 2)
	{
		for (i = 0; i < n1; i++)
		{
			i2        = i << 1;
			t[i2]     = a[i][j];
			t[i2 + 1] = a[i][j + 1];
		}
		b3ComplexDFT(n1 << 1, isgn, t, ip, w);
		for (i = 0; i < n1; i++)
		{
			i2          = i << 1;
			a[i][j]     = t[i2];
			a[i][j + 1] = t[i2 + 1];
		}
	}
}


void b3Fourier::b3RealDFT2d(
	b3_loop   n1,
	b3_loop   n2,
	b3_loop   isgn,
	b3_f64  **a,
	b3_f64   *t, 
    b3_index *ip,
	b3_f64   *w)
{
	b3_loop n, nw, nc, n1h, i, j, i2;
	b3_f64 xi;

	n = n1 << 1;
	if (n < n2)
	{
		n = n2;
	}
	nw = ip[0];
	if (n > (nw << 2))
	{
		nw = n >> 2;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n2 > (nc << 2))
	{
		nc = n2 >> 2;
		b3MakeCT(nc, ip, w + nw);
	}
	n1h = n1 >> 1;
	if (isgn < 0)
	{
		for (i = 1; i <= n1h - 1; i++)
		{
			j        = n1 - i;
			xi       = a[i][0] - a[j][0];
			a[i][0] += a[j][0];
			a[j][0]  = xi;
			xi       = a[j][1] - a[i][1];
			a[i][1] += a[j][1];
			a[j][1]  = xi;
		}
		for (j = 0; j <= n2 - 2; j += 2)
		{
			for (i = 0; i < n1; i++)
			{
				i2        = i << 1;
				t[i2]     = a[i][j];
				t[i2 + 1] = a[i][j + 1];
			}
			b3ComplexDFT(n1 << 1, isgn, t, ip, w);
			for (i = 0; i <= n1 - 1; i++)
			{
				i2          = i << 1;
				a[i][j]     = t[i2];
				a[i][j + 1] = t[i2 + 1];
			}
		}
		for (i = 0; i <= n1 - 1; i++)
		{
			b3RealDFT(n2, isgn, a[i], ip, w);
		}
	}
	else
	{
		for (i = 0; i <= n1 - 1; i++)
		{
			b3RealDFT(n2, isgn, a[i], ip, w);
		}
		for (j = 0; j <= n2 - 2; j += 2)
		{
			for (i = 0; i <= n1 - 1; i++)
			{
				i2        = i << 1;
				t[i2]     = a[i][j];
				t[i2 + 1] = a[i][j + 1];
			}
			b3ComplexDFT(n1 << 1, isgn, t, ip, w);
			for (i = 0; i <= n1 - 1; i++)
			{
				i2          = i << 1;
				a[i][j]     = t[i2];
				a[i][j + 1] = t[i2 + 1];
			}
		}
		for (i = 1; i <= n1h - 1; i++)
		{
			j        = n1 - i;
			a[j][0]  = 0.5 * (a[i][0] - a[j][0]);
			a[i][0] -= a[j][0];
			a[j][1]  = 0.5 * (a[i][1] + a[j][1]);
			a[i][1] -= a[j][1];
		}
	}
}


void b3Fourier::b3DCT2d(
	b3_loop    n1,
	b3_loop    n2,
	b3_loop    isgn,
	b3_f64   **a,
	b3_f64    *t, 
    b3_index  *ip,
    b3_f64    *w)
{
	b3_loop n, nw, nc, i, j;

	n = n1;
	if (n < n2)
	{
		n = n2;
	}
	nw = ip[0];
	if (n > (nw << 2))
	{
		nw = n >> 2;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n > nc)
	{
		nc = n;
		b3MakeCT(nc, ip, w + nw);
	}
	for (i = 0; i <= n1 - 1; i++)
	{
		b3DCT(n2, isgn, a[i], ip, w);
	}
	for (j = 0; j <= n2 - 1; j++)
	{
		for (i = 0; i <= n1 - 1; i++)
		{
			t[i] = a[i][j];
		}
		b3DCT(n1, isgn, t, ip, w);
		for (i = 0; i <= n1 - 1; i++)
		{
			a[i][j] = t[i];
		}
	}
}


void b3Fourier::b3DST2d(
	b3_loop   n1,
	b3_loop   n2,
	b3_loop   isgn,
	b3_f64  **a,
	b3_f64   *t, 
    b3_index *ip,
    b3_f64   *w)
{
	b3_loop n, nw, nc, i, j;

	n = n1;
	if (n < n2)
	{
		n = n2;
	}
	nw = ip[0];
	if (n > (nw << 2))
	{
		nw = n >> 2;
		b3MakeWT(nw, ip, w);
	}
	nc = ip[1];
	if (n > nc)
	{
		nc = n;
		b3MakeCT(nc, ip, w + nw);
	}
	for (i = 0; i <= n1 - 1; i++)
	{
		b3DST(n2, isgn, a[i], ip, w);
	}
	for (j = 0; j <= n2 - 1; j++)
	{
		for (i = 0; i <= n1 - 1; i++)
		{
			t[i] = a[i][j];
		}
		b3DST(n1, isgn, t, ip, w);
		for (i = 0; i <= n1 - 1; i++)
		{
			a[i][j] = t[i];
		}
	}
}

