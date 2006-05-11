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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

	m_Buffer     = null;
	m_Lines      = null;
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
	m_Real = null;
	m_Imag = null;

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
}

b3_bool b3Fourier::b3AllocBuffer(b3_res new_size)
{
	b3_res size = b3PowOf2(new_size);
	
	b3PrintF(B3LOG_FULL, "b3Fourier::b3AllocBuffer(%d)\n", size);
	m_xOrig  =
	m_yOrig  = new_size;
	m_xStart = (size - m_xOrig) >> 1;
	m_yStart = (size - m_yOrig) >> 1;

	if ((m_xSize == size) && (m_ySize == size))
	{
		// New buffer has same size.
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
	b3PrintF(B3LOG_FULL, ">b3Fourier::b3AllocBuffer(%dx%d, ...)\n", tx->xSize, tx->ySize);

	return true;
}

b3_bool b3Fourier::b3ReallocBuffer()
{
	b3_size size = B3_MAX(m_xSize, m_ySize);

	m_xDim   = b3Log2(m_xSize);
	m_yDim   = b3Log2(m_ySize);

	m_Buffer = new b3Complex<b3_f64>[m_xSize * m_ySize];
	m_Lines  = new b3Complex<b3_f64> *[m_ySize];
	if ((m_Buffer == null) || (m_Lines == null))
	{
		return false;
	}

	for (b3_loop y = 0;y < m_ySize;y++)
	{
		m_Lines[y] = &m_Buffer[y * m_xSize];
	}

	m_Real = (b3_f64 *)b3Alloc(size * sizeof(b3_f64));
	m_Imag = (b3_f64 *)b3Alloc(size * sizeof(b3_f64));

	return (m_Real != null) && (m_Imag != null);
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
b3_bool b3Fourier::b3FFT(int dir,b3_res m,b3_f64 *x,b3_f64 *y)
{
	b3_loop nn,i,i1,j,k,i2,l,l1,l2;
	b3_f64  c1,c2,tx,ty,t1,t2,u1,u2,z;

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
			tx = x[i];
			ty = y[i];
			x[i] = x[j];
			y[i] = y[j];
			x[j] = tx;
			y[j] = ty;
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
	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;
	for (l=0;l<m;l++)
	{
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0;
		u2 = 0.0;
		for (j=0;j<l1;j++)
		{
			for (i=j;i<nn;i+=l2)
			{
				i1 = i + l1;
				t1 = u1 * x[i1] - u2 * y[i1];
				t2 = u1 * y[i1] + u2 * x[i1];
				x[i1] = x[i] - t1;
				y[i1] = y[i] - t2;
				x[i] += t1;
				y[i] += t2;
			}
			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}
		c2 = sqrt((1.0 - c1) / 2.0);
		if (dir == 1)
		{
			c2 = -c2;
		}
		c1 = sqrt((1.0 + c1) / 2.0);
	}

	/* Scaling for forward transform */
	if (dir == 1)
	{
		for (i=0;i<nn;i++)
		{
			x[i] /= (b3_f64)nn;
			y[i] /= (b3_f64)nn;
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
	b3_loop   i,j;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3FFT2D(<%s>);\n", dir == 1 ? "forward" : "inverse");

	/* Transform the rows */
	for (j = 0; j < m_ySize; j++)
	{
		for (i = 0; i < m_xSize; i++)
		{
			m_Real[i] = m_Lines[j][i].b3Real();
			m_Imag[i] = m_Lines[j][i].b3Imag();
		}
		b3FFT(dir,m_xDim,m_Real,m_Imag);
		for (i = 0; i < m_xSize; i++)
		{
			m_Lines[j][i].b3Real() = m_Real[i];
			m_Lines[j][i].b3Imag() = m_Imag[i];
		}
	}

	/* Transform the columns */
	for (i = 0; i < m_xSize; i++)
	{
		for (j = 0; j < m_ySize; j++)
		{
			m_Real[j] = m_Lines[j][i].b3Real();
			m_Imag[j] = m_Lines[j][i].b3Imag();
		}
		b3FFT(dir, m_yDim, m_Real, m_Imag);
		for (j = 0; j < m_ySize; j++)
		{
			m_Lines[j][i].b3Real() = m_Real[j];
			m_Lines[j][i].b3Imag() = m_Imag[j];
		}
	}

	return true;
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
    b3_loop  x, y;
    b3_f64   err = 0, e, divisor;

	b3PrintF(B3LOG_FULL, ">b3Fourier::b3SelfTest()\n");
	m_Random.b3SetSeed(0);
	for (y = 0; y < m_ySize; y++)
	{
        for (x = 0; x < m_xSize; x++)
		{
            m_Lines[y][x].b3Real() = m_Random.b3Rand();
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

	m_Random.b3SetSeed(0);
	divisor = m_xSize * m_ySize * 0.5;
    for (y = 0; y < m_ySize; y++)
	{
        for (x = 0; x < m_xSize; x++)
		{
            e   = m_Random.b3Rand() - m_Lines[y][x].b3Real();
            err = B3_MAX(err, fabs(e));
        }
    }
	b3PrintF(B3LOG_NORMAL,"### CLASS: b3Four # error %g\n",err);
	b3PrintF(B3LOG_FULL, "<b3Fourier::b3SelfTest()\n");
	return err < 0.001;
}
