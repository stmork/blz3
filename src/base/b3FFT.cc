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

#include "b3BaseInclude.h"
#include "blz3/base/b3FFT.h"
#include "blz3/base/b3Color.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Random.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.17  2006/05/20 16:32:42  sm
**	- Some complex number optimizations.
**
**	Revision 1.16  2006/05/19 07:02:58  sm
**	- Corrected FFT unit test.
**	
**	Revision 1.15  2006/05/18 19:07:40  sm
**	- DRand48 implementation for all platforms.
**	
**	Revision 1.14  2006/05/17 21:35:37  sm
**	- Minor optimizations.
**	
**	Revision 1.13  2006/05/14 18:05:09  sm
**	- Added return value
**	
**	Revision 1.12  2006/05/14 11:30:39  sm
**	- Added complex number classes to FFT class.
**	
**	Revision 1.11  2006/05/14 08:54:58  sm
**	- Multithreaded 2D FFT.
**	
**	Revision 1.10  2006/05/13 10:01:01  sm
**	- Introduced special complex number computation for FFT handling.
**	
**	Revision 1.9  2006/05/11 15:34:22  sm
**	- Added unit tests
**	- Corrected normal computation for ocean waves
**	- Optimized b3Complex
**	- Added new FFT
**	- Added own assertion include
**	
**	Revision 1.8  2006/05/01 12:32:42  sm
**	- Some value tests.
**	
**	Revision 1.7  2006/05/01 10:03:02  sm
**	- Better Exception handling.
**	- Documentation.
**	
**	Revision 1.6  2006/04/30 08:30:56  sm
**	- Exchanged FFT algorithm.
**	
**	Revision 1.5  2006/04/29 20:29:53  sm
**	- Switched to other FFT 2D algorithm which works correctly.
**	
**	Revision 1.4  2006/04/29 11:25:48  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**	
**	Revision 1.3  2006/04/17 16:36:34  sm
**	- Importing FFT and ocean waves into Windows project.
**	
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
	m_Size   = 0;

	m_Buffer = null;
	m_Lines  = null;
	m_Aux    = null;
	m_CPUs   = B3_MIN(B3_FFT_MAX_THREADS, b3Runtime::b3GetNumCPUs());
}

b3Fourier::~b3Fourier()
{
	b3FreeBuffer();
}

b3_loop b3Fourier::b3PowOf2(b3_loop value)
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

b3_count b3Fourier::b3Log2(b3_u32 value)
{
	b3_count count = -1;

	while(value > 0)
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
	m_Size = 0;
}

b3_bool b3Fourier::b3AllocBuffer(b3_res new_size)
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

b3_bool b3Fourier::b3AllocBuffer  (b3Tx *tx)
{
	b3_loop       x,y,index,max;
	b3_u08       *cPtr;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3AllocBuffer(%dx%d, ...)\n", tx->xSize, tx->ySize);
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
	cPtr    = (b3_u08 *)tx->b3GetData();
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

b3_bool b3Fourier::b3ReallocBuffer()
{
	b3PrintF(B3LOG_FULL, ">b3Fourier::b3ReallocBuffer()\n");

	m_Size   = B3_MAX(m_xSize, m_ySize);
	m_xDim   = b3Log2(m_xSize);
	m_yDim   = b3Log2(m_ySize);

	m_Buffer = new b3Complex64[m_xSize * m_ySize];
	m_Lines  = new b3Complex64 *[m_ySize];
	m_Aux    = new b3Complex64[m_Size * m_CPUs];

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
b3_bool b3Fourier::b3FFT(int dir,b3_res m,b3Complex64 *line)
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
b3_bool b3Fourier::b3FFT2D(int dir)
{
	b3PrintF(B3LOG_FULL, ">b3Fourier::b3FFT2D(<%s>);\n", dir == 1 ? "forward" : "inverse");

	b3Thread    threads[B3_FFT_MAX_THREADS];
	b3_fft_info info[B3_FFT_MAX_THREADS];
	b3_loop     i;

	for(i = 0;i < m_CPUs;i++)
	{
		info[i].m_Lines = m_Lines;
		info[i].m_Aux   = &m_Aux[m_Size * i];
		info[i].m_xDim  = m_xDim;
		info[i].m_yDim  = m_yDim;
		info[i].m_Dir   = dir;
	}	

	if (m_CPUs > 1)
	{
		for(i = 0;i < m_CPUs;i++)
		{
			info[i].m_xMin = 0;
			info[i].m_xMax = m_xSize;
			info[i].m_yMin = m_ySize *  i      / m_CPUs;
			info[i].m_yMax = m_ySize * (i + 1) / m_CPUs;
			threads[i].b3Name("b3FFT - row computation");
			threads[i].b3Start(b3RowFFT, &info[i]);
		}

		for(i = 0;i < m_CPUs;i++)
		{
			threads[i].b3Wait();
		}

		for(i = 0;i < m_CPUs;i++)
		{
			info[i].m_xMin = m_xSize *   i      / m_CPUs;
			info[i].m_xMax = m_xSize * ( i + 1) / m_CPUs;
			info[i].m_yMin = 0;
			info[i].m_yMax = m_ySize;
			threads[i].b3Name("b3FFT - column computation");
			threads[i].b3Start(b3ColumnFFT, &info[i]);
		}

		for(i = 0;i < m_CPUs;i++)
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
	b3Complex64  *aux   = info->m_Aux;
	b3_loop       i,j;

	/* Transform the rows */
	for (j = info->m_yMin; j < info->m_yMax; j++)
	{
		for (i = info->m_xMin; i < info->m_xMax; i++)
		{
			aux[i] = lines[j][i];
		}

		b3FFT(info->m_Dir, info->m_xDim, aux);

		for (i = info->m_xMin; i < info->m_xMax; i++)
		{
			lines[j][i] = aux[i];
		}
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
			aux[j] = lines[j][i];
		}

		b3FFT(info->m_Dir, info->m_yDim, aux);

		for (j = info->m_yMin; j < info->m_yMax; j++)
		{
			lines[j][i] = aux[j];
		}
	}
	return 0;
}

b3_bool b3Fourier::b3GetBuffer(b3Tx *tx, b3_f64 amp)
{
	b3_u08       *cPtr;
	b3_loop       x,y;
	b3_f64        cMin = 0, c, cMax = 0;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3GetBuffer(..., %1.3f)\n", amp);
	if (!tx->b3AllocTx(m_xOrig, m_yOrig, 8))
	{
		B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
	}

	cPtr = (b3_u08 *)tx->b3GetData();
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
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3GetBuffer(...)\n");
	return true;
}

b3_bool b3Fourier::b3GetSpectrum(b3Tx *tx, b3_f64 amp)
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
	cPtr = (b3_u08 *)tx->b3GetData();
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

b3_bool b3Fourier::b3SelfTest()
{
	b3Rand48<b3_f64> random;
    b3_loop          x, y;
    b3_f64           err = 0, e, divisor;

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
	divisor = m_xSize * m_ySize * 0.5;
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
