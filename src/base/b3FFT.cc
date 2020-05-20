/*
**
**	$Filename:	b3FFT.cc $
**	$Release:	Dortmund 2006, 2016 $
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

#include "b3BaseInclude.h"
#include "blz3/system/b3TimeStop.h"
#include "blz3/base/b3FFT.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Random.h"

/*************************************************************************
**                                                                      **
**                        b3SimpleFourier                               **
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
	m_xDim   =
		m_yDim   = 0;

	m_Buffer = null;
	m_Lines  = null;
	m_Aux    = null;
	m_CPUs   = B3_MIN(B3_FFT_MAX_THREADS, b3Runtime::b3GetNumCPUs());
}

b3Fourier::~b3Fourier()
{
	b3FreeBuffer();
}

const b3_loop b3Fourier::b3PowOf2(const b3_loop value)
{
	b3_loop result = 1;

	// Prevent busy loop
	if ((value & 0x80000000) != 0)
	{
		return 0x80000000;
	}

	while (value > result)
	{
		result = result << 1;
	}
	return result;
}

const b3_count b3Fourier::b3Log2(b3_u32 value)
{
	b3_count count = -1;

	while (value > 0)
	{
		value = value >> 1;
		count++;
	}
	return count;
}

void b3Fourier::b3FreeBuffer()
{
	b3Free();
	if (m_Buffer != null)
	{
		delete [] m_Buffer;
		m_Buffer = null;
	}
	if (m_Lines != null)
	{
		delete [] m_Lines;
		m_Lines = null;
	}
	if (m_Aux != null)
	{
		delete [] m_Aux;
		m_Aux = null;
	}
}

const b3_bool b3Fourier::b3AllocBuffer(const b3_res new_size)
{
	b3_res size = b3PowOf2(new_size);

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3AllocBuffer(%d)\n", size);
	m_xOrig  =
		m_yOrig  = new_size;
	m_xStart = (size - m_xOrig) >> 1;
	m_yStart = (size - m_yOrig) >> 1;

	if ((m_xSize == size) && (m_ySize == size))
	{
		// New buffer has same size.
		b3PrintF(B3LOG_FULL, "<b3Fourier::b3AllocBuffer(%d) [unchanged]\n", size);
		return true;
	}
	b3FreeBuffer();

	m_xSize  =
		m_ySize  = size;
	if (!b3ReallocBuffer())
	{
		b3FreeBuffer();
		B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
	}
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3AllocBuffer(%d)\n", size);
	return true;
}

const b3_bool b3Fourier::b3AllocBuffer  (b3Tx *tx)
{
	b3_loop       x,y,index,max;
	b3_u08       *cPtr;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3AllocBuffer(%dx%d, ...)\n", tx->xSize, tx->ySize);
	if (!tx->b3IsLoaded())
	{
		B3_THROW(b3FFTException, B3_FFT_SRC_TX_EMPTY);
	}
	b3FreeBuffer();

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

	if (!tx->b3IsPalette())
	{
		B3_THROW(b3FFTException, B3_FFT_NO_PALETTE);
	}

	b3PrintF(B3LOG_FULL, "  Grey (%dx%d)\n", m_xSize, m_ySize);
	if (!b3ReallocBuffer())
	{
		B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
	}

	cPtr = tx->b3GetIndexData();
	for (y = 0;y < m_yOrig;y++)
	{
		for (x = 0;x < m_xOrig;x++)
		{
			m_Lines[y + m_yStart][x + m_xStart] = (b3_f64)*cPtr++ / 127.5 - 1.0;
		}
		index += m_xSize;
	}
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3AllocBuffer(%dx%d, ...)\n", tx->xSize, tx->ySize);

	return true;
}

const b3_bool b3Fourier::b3ReallocBuffer()
{
	b3PrintF(B3LOG_FULL, ">b3Fourier::b3ReallocBuffer()\n");

	m_xDim   = b3Log2(m_xSize);
	m_yDim   = b3Log2(m_ySize);

	m_Buffer = new b3Complex64[m_xSize * m_ySize];
	m_Lines  = new b3Complex64 *[m_ySize];
	m_Aux    = new b3Complex64[m_ySize * m_CPUs];

	if ((m_Buffer == null) || (m_Lines == null) || (m_Aux == null))
	{
		return false;
	}

	for (b3_loop y = 0;y < m_ySize;y++)
	{
		m_Lines[y] = &m_Buffer[y * m_xSize];
	}

	b3PrintF(B3LOG_FULL, "<b3Fourier::b3ReallocBuffer()\n");
	return true;
}

void b3Fourier::b3Sample(b3FilterInfo *info,b3SampleFunc sample_func)
{
	b3_loop  x,xHalf = m_xSize >> 1, xMask = m_xSize - 1;
	b3_loop  y,yHalf = m_ySize >> 1, yMask = m_ySize - 1;
	b3_f64   fx, fxHalf = 1.0 / xHalf;
	b3_f64   fy, fyHalf = 1.0 / yHalf;
	b3_index index   = 0, pos;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3Sample(...)\n");
	info->m_Fourier = this;
	index = 0;
	for (y = -yHalf;y < yHalf;y++)
	{
		fy = fyHalf * y;
		index = (y & yMask) << m_xDim;
		for (x = -xHalf;x < xHalf;x++)
		{
			fx = fxHalf * x;
			pos = index + (x & xMask);
			sample_func(fx, fy, pos, info);
		}
	}
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3Sample(...)\n");
}

/*-------------------------------------------------------------------------
   This computes an in-place complex-to-complex FFT
   x and y are the real and imaginary arrays of 2^m points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform

     Formula: forward
                  N-1
                  ---
              1   \          - j k 2 pi n / N
      X(n) = ---   >   x(k) e                    = forward transform
              N   /                                n=0..N-1
                  ---
                  k=0

      Formula: reverse
                  N-1
                  ---
                  \          j k 2 pi n / N
      X(n) =       >   x(k) e                    = forward transform
                  /                                n=0..N-1
                  ---
                  k=0
*/
const b3_bool b3Fourier::b3FFT(const int dir, const b3_res m, b3Complex64 *line)
{
	b3Complex64 one   = b3Complex64(1.0, 1.0);
	b3Complex64 half  = b3Complex64(0.5, 0.5);
	b3Complex64 dMult = b3Complex64(1.0, dir == 1 ? -1.0 : 1.0);
	b3Complex64 c,u;
	b3_loop     nn,i,i1,j,k,i2,l,l1,l2;

	/* Calculate the number of points */
	nn = 1;
	for (i=0;i<m;i++)
	{
		nn *= 2;
	}

	/* Do the bit reversal */
	i2 = nn >> 1;
	j = 0;
	for (i=0;i<nn-1;i++)
	{
		if (i < j)
		{
			b3Complex64::b3Swap(line[i], line[j]);
		}
		k = i2;
		while (k <= j)
		{
			j -= k;
			k >>= 1;
		}
		j += k;
	}

	/* Compute the FFT */
	c  = -1.0;
	l2 = 1;
	for (l=0;l<m;l++)
	{
		u    = 1;
		l1   = l2;
		l2 <<= 1;
		for (j=0;j<l1;j++)
		{
			for (i=j;i<nn;i+=l2)
			{
				i1 = i + l1;

				b3Complex64 t = u * line[i1];

				line[i1]  = line[i] - t;
				line[i]  += t;
			}
			u *= c;
		}

		b3_f64 cr = c.b3Real();
		b3Complex64 q = one + b3Complex64(cr, -cr);

		q.b3Scale(half);
		c = b3Complex64::b3Sqrt(q);
		c.b3Scale(dMult);
	}

	/* Scaling for forward transform */
	if (dir == 1)
	{
		b3Complex64 denom(1.0 / nn, 1.0 / nn);

		for (i = 0; i < nn; i++)
		{
			line[i].b3Scale(denom);
		}
	}

	return true;
}

/**
 * Perform a 2D FFT inplace given a complex 2D array
 * The direction dir, 1 for forward, -1 for reverse
 * The size of the array (nx,ny)
 * Return false if there are memory problems or
 *    the dimensions are not powers of 2
 *
 * @param dir 1 for forward FFT, -1 for inverse FFT.
 * @return True on success.
 */
const b3_bool b3Fourier::b3FFT2D(const int dir)
{
	b3TimeStop stop("2D FFT");

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3FFT2D(<%s>);\n", dir == 1 ? "forward" : "inverse");

	b3Thread    threads[B3_FFT_MAX_THREADS];
	b3_fft_info info[B3_FFT_MAX_THREADS];
	b3_loop     i;

	for (i = 0;i < m_CPUs;i++)
	{
		info[i].m_Lines = m_Lines;
		info[i].m_Aux   = &m_Aux[m_ySize * i];
		info[i].m_xDim  = m_xDim;
		info[i].m_yDim  = m_yDim;
		info[i].m_Dir   = dir;
	}

	if (m_CPUs > 1)
	{
		for (i = 0;i < m_CPUs;i++)
		{
			info[i].m_xMin = 0;
			info[i].m_xMax = m_xSize;
			info[i].m_yMin = m_ySize *  i      / m_CPUs;
			info[i].m_yMax = m_ySize * (i + 1) / m_CPUs;
			threads[i].b3Name("b3FFT - row computation");
			threads[i].b3Start(b3RowFFT, &info[i]);
		}

		for (i = 0;i < m_CPUs;i++)
		{
			threads[i].b3Wait();
		}

		stop.b3TimePos();

		for (i = 0;i < m_CPUs;i++)
		{
			info[i].m_xMin = m_xSize *   i      / m_CPUs;
			info[i].m_xMax = m_xSize * ( i + 1) / m_CPUs;
			info[i].m_yMin = 0;
			info[i].m_yMax = m_ySize;
			threads[i].b3Name("b3FFT - column computation");
			threads[i].b3Start(b3ColumnFFT, &info[i]);
		}

		for (i = 0;i < m_CPUs;i++)
		{
			threads[i].b3Wait();
		}
	}
	else
	{
		info[0].m_xMin = 0;
		info[0].m_xMax = m_xSize;
		info[0].m_yMin = 0;
		info[0].m_yMax = m_ySize;
		b3RowFFT(&info);
		b3ColumnFFT(&info);
	}

	return true;
}

b3_u32 b3Fourier::b3RowFFT(void *ptr)
{
	b3_fft_info  *info  = static_cast<b3_fft_info *>(ptr);
	b3Complex64 **lines = info->m_Lines;
	b3_loop       j;

	/* Transform the rows */
	for (j = info->m_yMin; j < info->m_yMax; j++)
	{
		b3FFT(info->m_Dir, info->m_xDim, lines[j]);
	}
	return 0;
}

b3_u32 b3Fourier::b3ColumnFFT(void *ptr)
{
	b3_fft_info  *info  = static_cast<b3_fft_info *>(ptr);
	b3Complex64 **lines = info->m_Lines;
	b3Complex64  *aux   = info->m_Aux;
	b3_loop       i,j;

	/* Transform the columns */
	for (i = info->m_xMin; i < info->m_xMax; i++)
	{
		for (j = info->m_yMin; j < info->m_yMax; j++)
		{
#ifdef BLZ3_USE_SSE
			_mm_prefetch(reinterpret_cast<const char *>(&lines[j+2][i]), _MM_HINT_NTA);
#endif
			aux[j] = lines[j][i];
		}

		b3FFT(info->m_Dir, info->m_yDim, aux);

		for (j = info->m_yMin; j < info->m_yMax; j++)
		{
#ifdef BLZ3_USE_SSE2
			b3Complex64::b3CopyUncached(lines[j][i], aux[j]);
#else
			lines[j][i] = aux[j];
#endif
		}
	}

	return 0;
}

const b3_bool b3Fourier::b3GetBuffer(b3Tx *tx, b3_f64 amp)
{
	b3_u08       *cPtr;
	b3_loop       x,y;
	b3_f64        cMin = 0, c, cMax = 0;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3GetBuffer(..., %1.3f)\n", amp);
	if (!tx->b3AllocTx(m_xOrig, m_yOrig, 8))
	{
		B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
	}

	cPtr = tx->b3GetIndexData();
	for (y = 0;y < m_yOrig;y++)
	{
		for (x = 0;x < m_xOrig;x++)
		{
			c = m_Lines[y+m_yStart][x+m_xStart].b3Real() * amp;
			if (c < cMin) cMin = c;
			if (c > cMax) cMax = c;
			*cPtr++ = (b3_u08)floor(b3Math::b3Limit(c * 0.5 + 0.5) * 255);
		}
	}
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3GetBuffer(...) [%1.4f - %1.4f]\n",
			 cMin, cMax);
	return true;
}

const b3_bool b3Fourier::b3GetSpectrum(b3Tx *tx, b3_f64 amp)
{
	b3_u08        *cPtr;
	b3_f64         result;
	b3_f64         cMax = 0;
	b3_loop        x,xHalf = m_xSize >> 1, xMask = m_xSize - 1;
	b3_loop        y,yHalf = m_ySize >> 1, yMask = m_ySize - 1;
	b3_index       index;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3GetSpectrum(..., %1.3f)\n", amp);
	if (!tx->b3AllocTx(m_xSize, m_ySize, 8))
	{
		B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
	}
	cPtr = tx->b3GetIndexData();
	for (y = -yHalf;y < yHalf;y++)
	{
		index = (y & yMask) << m_xDim;
		for (x = -xHalf;x < xHalf;x++)
		{
			result  = m_Buffer[index + (x & xMask)].b3Length() * amp * 127;
			if (result > cMax) cMax = result;

			if (result > 255) result = 255;
			*cPtr++ = (b3_u08)result;
		}
	}
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3GetSpectrum(...)\n");
	return true;
}

const b3_bool b3Fourier::b3SelfTest()
{
	b3Rand48<b3_f64> random;
	b3_loop          x, y;
	b3_f64           err = 0, e;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3SelfTest() %dx%d\n", m_xSize, m_ySize);

	random.b3SetSeed();
	for (y = 0; y < m_ySize; y++)
	{
		for (x = 0; x < m_xSize; x++)
		{
			m_Lines[y][x].b3Real() = random.b3Rand();
		}
	}

	if (!b3FFT2D())
	{
		return false;
	}
	if (!b3IFFT2D())
	{
		return false;
	}

	random.b3SetSeed();
	for (y = 0; y < m_ySize; y++)
	{
		for (x = 0; x < m_xSize; x++)
		{
			e   = fabs(random.b3Rand() - m_Lines[y][x].b3Real());
			err = B3_MAX(err, fabs(e));
		}
	}
	b3PrintF(B3LOG_NORMAL,"### CLASS: b3Four # error diff %g\n",err);
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3SelfTest()\n");
	return err < 0.001;
}
