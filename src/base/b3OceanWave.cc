/*
**
**	$Filename:	b3OceanWave.c $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Ocean wave bump map
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3OceanWave.h"
#include "blz3/base/b3Procedure.h"

#define not_VERBOSE
#define not_VERBOSE_DUMP

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.17  2006/04/30 11:48:12  sm
**	- New ocean wave deriv try.
**
**	Revision 1.16  2006/04/30 10:52:54  sm
**	- Done some height field corrections.
**	
**	Revision 1.15  2006/04/30 09:05:25  sm
**	- No verbose!
**	
**	Revision 1.14  2006/04/30 08:55:52  sm
**	- Further signed/unsigned issues.
**	
**	Revision 1.13  2006/04/30 08:53:24  sm
**	- Removed some signed/unsigned issues.
**	- Reflect new FFT algorithm.
**	
**	Revision 1.12  2006/04/30 08:30:56  sm
**	- Exchanged FFT algorithm.
**	
**	Revision 1.11  2006/04/29 20:45:57  sm
**	- New scaling.
**	
**	Revision 1.10  2006/04/29 20:29:54  sm
**	- Switched to other FFT 2D algorithm which works correctly.
**	
**	Revision 1.9  2006/04/29 17:58:27  sm
**	- Minor value changes.
**	
**	Revision 1.8  2006/04/29 11:25:49  sm
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
**	Revision 1.7  2006/04/19 18:15:09  sm
**	- Fine tuning ocean waves.
**	
**	Revision 1.6  2006/04/19 15:19:58  sm
**	- The break through! Ocean waves...
**	
**	Revision 1.5  2006/04/19 10:55:51  sm
**	- Fine tuning on ocean waves
**	
**	Revision 1.4  2006/04/19 10:20:30  sm
**	- Adjusted splitted includes.
**	- Adjusted ocean waves values.
**	
**	Revision 1.3  2006/04/19 08:48:19  sm
**	- Fine tuning on ocean waves
**	
**	Revision 1.2  2006/04/18 20:38:25  sm
**	- Optimized compilation
**	
**	Revision 1.1  2006/04/18 15:48:59  sm
**	- Extracted from procedure module:
**	  o clouds
**	  o ocean waves
**	  o water
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Cloud default values                          **
**                                                                      **
*************************************************************************/

const b3_f64 b3OceanWave::g = 9.81;

b3OceanWave::b3OceanWave()
{
	m_t        = -FLT_MAX;
	m_T        =     3;
	m_Dim      =     9;
	m_Wx       =     1;
	m_Wy       =     0.7f;
	m_A        =     10;
	m_GridSize =   800;
	m_l        =     0.1f;
	
	m_Phillips = null;
	m_Modified = true;
}

b3OceanWave::~b3OceanWave()
{
	if (m_Phillips != null)
	{
		delete [] m_Phillips;
	}
}

void b3OceanWave::b3PrepareOceanWave(const b3_f64 t)
{
	b3PrintF(B3LOG_FULL, "  preparing ocean waves...\n");

	// Grid values
	m_fftMax         =  1 << (m_Dim - 1);
	m_fftMin         = -m_fftMax;
	m_fftDiff        =  m_fftMax - m_fftMin;
	m_GridScale      =  1.0 / m_GridSize;
	
	// Time values
	m_t              =  t;
	m_omega_t        =  M_PI * 2.0 * m_t / m_T;
	m_Cycle.b3Real() = cos(m_omega_t);
	m_Cycle.b3Imag() = sin(m_omega_t);

	// Physics
	m_W2             =  m_Wx * m_Wx + m_Wy * m_Wy;;
	m_L              =  m_W2 / g;
	m_L2             = -m_L * m_L;
	m_l2             =  m_l * m_l;

	m_FFT.b3AllocBuffer(1 << m_Dim);
	m_Random.b3SetSeed(0);

	b3PrintF(B3LOG_DEBUG, "    T    = %1.3f\n", m_T);
	b3PrintF(B3LOG_DEBUG, "    t    = %1.3f\n", m_t);
	b3PrintF(B3LOG_DEBUG, "    L    = %1.3f\n", m_L);
	b3PrintF(B3LOG_DEBUG, "    L²   = %1.3f\n", m_L2);
	b3PrintF(B3LOG_DEBUG, "    |w|² = %1.3f\n", m_W2);

#if 0
	b3TestSpectrum3();
#else
	b3ComputePhillipsSpectrum();
#endif

#ifdef VERBOSE
	b3DumpImages();
#else
	m_FFT.b3IFFT2D();
#endif
}

b3_f64 b3OceanWave::b3ComputeOceanWave(const b3_vector *pos)
{
	b3Complex<b3_f64> *buffer = b3GetBuffer();
	b3_f64             fx     = b3Math::b3FracOne(pos->x * m_GridScale) * m_fftDiff, dx[2];
	b3_f64             fy     = b3Math::b3FracOne(pos->y * m_GridScale) * m_fftDiff, dy;
	b3_index           max    = m_fftDiff * m_fftDiff;
	b3_index           index, x, y;
	b3_f64             a[2], b[2], c[2];

	x = (b3_index)fx & B3_OCEAN_MAX_MASK;
	y = (b3_index)fy;

	dx[0] = dx[1] = (fx - x) / B3_OCEAN_XSKIP;
	dy =  fy - y;

	index = y * m_fftDiff + x;
	a[0] = buffer[index].b3Real();

	index += B3_OCEAN_XSKIP;
	if (index >= max)
	{
		index -= m_fftDiff;
	}
	b[0] = buffer[index].b3Real();

	index += m_fftDiff;
	if (index >= max)
	{
		index -= max;
	}
	b[1] = buffer[index].b3Real();

	index -= B3_OCEAN_XSKIP;
	if (index < 0)
	{
		index += m_fftDiff;
	}
	a[1] = buffer[index].b3Real();

	for (b3_loop i = 0;i < 2;i++)
	{
		c[i] = a[i] + dx[i] * (b[i] - a[i]);
	}

#ifdef VERBOSE_DUMP
	b3PrintF(B3LOG_FULL, "%4d %4d %f %f # %f %f  %f %f # %f %f # %f %f\n",x, y, pos->x, pos->y, a[0], b[0], a[1], b[1], c[0], c[1], dx,dy);
#endif

	return b3Math::b3Mix(c[0], c[1], dy);
}

/**
 *              hyl
 *               *
 *               |
 *               |
 *     hxl *-----*-----* hxh
 *               |h
 *               |
 *               *
 *              hyl
 */
void b3OceanWave::b3ComputeOceanWaveDeriv(const b3_vector *pos, b3_vector *n)
{
#if 1
	b3Complex<b3_f64> *buffer = b3GetBuffer();
	b3_f64             fx     = b3Math::b3FracOne(pos->x * m_GridScale) * m_fftDiff, dx;
	b3_f64             fy     = b3Math::b3FracOne(pos->y * m_GridScale) * m_fftDiff, dy;
	b3_index           max    = m_fftDiff * m_fftDiff;
	b3_index           index, x, y;
	b3_f64             h, hxl, hxh, hyl, hyh;

	x = (b3_index)fx & B3_OCEAN_MAX_MASK;
	y = (b3_index)fy;

	index = y * m_fftDiff + x;
	h = buffer[index].b3Real();

	index += B3_OCEAN_XSKIP;
	if (index >= max)
	{
		index -= m_fftDiff;
	}
	hxh = buffer[index].b3Real() - h;

	index -= B3_OCEAN_XSKIP;
	index -= B3_OCEAN_XSKIP;
	if (index < 0)
	{
		index += m_fftDiff;
	}
	hxl = buffer[index].b3Real() - h;

	index += B3_OCEAN_XSKIP;
	if (index >= max)
	{
		index -= m_fftDiff;
	}
	index += m_fftDiff;
	if (index >= max)
	{
		index -= max;
	}
	hyh = buffer[index].b3Real() - h;

	index -= m_fftDiff;
	index -= m_fftDiff;
	if (index < 0)
	{
		index += max;
	}
	hyl = buffer[index].b3Real() - h;

	dx = (fx - x) / B3_OCEAN_XSKIP;
	dy =  fy - y;

	n->x = b3Math::b3Mix(hxl, hxh, dx) * 0.01;
	n->y = b3Math::b3Mix(hyl, hyh, dy) * 0.01;
	n->z = 1;

	b3PrintF(B3LOG_FULL, "%4d %4d %f %f # %f %f %f - %f %f %f # %f %f\n",
		x, y, pos->x, pos->y, hxl, dx, hxh, hyl, dy, hyh, n->x, n->y);
#else
	b3Complex<b3_f64> *buffer = b3GetBuffer();
	b3_f64             fx     = b3Math::b3FracOne(pos->x * m_GridScale) * m_fftDiff, dx;
	b3_f64             fy     = b3Math::b3FracOne(pos->y * m_GridScale) * m_fftDiff, dy;
	b3_index           max    = m_fftDiff * m_fftDiff;
	b3_index           index, x, y;
	b3_f64             h, hx, hy;

	x = (b3_index)fx & B3_OCEAN_MAX_MASK;
	y = (b3_index)fy;

	index = y * m_fftDiff + x;
	h = buffer[index].b3Real();

	index += B3_OCEAN_XSKIP;
	if (index >= max)
	{
		index -= m_fftDiff;
	}

	dx = (fx - x) / B3_OCEAN_XSKIP;
	if (dx == 0)
	{
		dx = 1;
		hx = buffer[index].b3Real() - h;
	}
	else
	{
		hx = b3Math::b3Mix(h, buffer[index].b3Real(), dx) - h;
	}

	index += m_fftDiff;
	if (index >= max)
	{
		index -= max;
	}

	index -= B3_OCEAN_XSKIP;
	if (index < 0)
	{
		index += m_fftDiff;
	}

	dy =  fy - y;
	if (dy == 0)
	{
		dy = 1;
		hy = buffer[index].b3Real() - h;
	}
	else
	{
		hy = b3Math::b3Mix(h, buffer[index].b3Real(), dy) - h;
	}

	n->x = -hx / dx;
	n->y = -hy / dy;
	n->z = 1.0f;

#ifdef VERBOSE_DUMP
	b3PrintF(B3LOG_FULL, "%4d %4d %f %f # %f %f  %f %f # %f %f\n",x, y, pos->x, pos->y, hx, dx, hy, dy, n->x, n->y);
#endif
#endif
}

void b3OceanWave::b3ComputePhillipsSpectrum()
{
	if (m_Phillips == null)
	{
		m_Phillips = new b3Complex<b3_f64>[m_fftDiff * m_fftDiff];
		if (m_Phillips == null)
		{
			throw;
		}

	}

	if (m_Modified)
	{
		m_FFT.b3Sample((b3FilterInfo *)this, b3FilterPhillipsSpectrum);
		m_Modified = false;
	}

	m_FFT.b3Sample((b3FilterInfo *)this, b3SampleHeight);
}

void b3OceanWave::b3FilterPhillipsSpectrum(
	const b3_f64        fx,
	const b3_f64        fy,
	const b3_index      index,
	      b3FilterInfo *filter_info)
{
	b3OceanWave       *ocean  = (b3OceanWave *)filter_info;

	ocean->b3SamplePhillipsSpectrum(fx, fy, index);
}

void b3OceanWave::b3SamplePhillipsSpectrum(b3_f64 fx, b3_f64 fy, b3_index index)
{
	b3_f64             factor = M_PI * 2 * m_fftMax / m_GridSize;
	b3_f64             w;
	b3_f64             phillips;
	b3Complex<b3_f64>  gauss;
	b3Complex<b3_f64>  result;
	b3Complex<b3_f64>  K(fx * factor, fy * factor);
	b3_f64             k2       = K.b3SquareLength();
	b3_f64             k4       = k2 * k2;
	b3_f64             quotient = fabs(K.b3Real() * m_Wx + K.b3Imag() * m_Wy);
	b3_f64             hf_scale = exp(-m_l2 * k2);

	if(k4 == 0)
	{
		phillips = 0;
	}
	else
	{
		phillips = m_A * exp( m_L2 / k2 ) * quotient * quotient * quotient / k4;
	}

#ifdef VERBOSE_DUMP
	b3PrintF(B3LOG_NORMAL,"f: %f %f - K: %f %f # k²=%f k^4=%f L²=%f q=%f P=%f",
		fx, fy,
		K.b3Real(), K.b3Imag(),
		k2, k4, m_L2, quotient, phillips);
#endif


	do
	{
		gauss.b3Real() = m_Random.b3Rand(2.0) - 1.0;
		gauss.b3Imag() = m_Random.b3Rand(2.0) - 1.0;
		w  = gauss.b3SquareLength();
	}
	while (w >= 1.0);

	m_Phillips[index] = gauss * sqrt( -phillips * log(w) / w);

#ifdef VERBOSE_DUMP
	b3PrintF(B3LOG_NORMAL, " # %f %f %f\n",
		m_Phillips[index].b3Real(),  m_Phillips[index].b3Imag(), w);
#endif
}

void b3OceanWave::b3SampleHeight(
	const b3_f64        fx,
	const b3_f64        fy,
	const b3_index      index,
	      b3FilterInfo *filter_info)
{
	b3OceanWave       *ocean  = (b3OceanWave *)filter_info;
	b3Complex<b3_f64> *buffer = ocean->b3GetBuffer();
	
	buffer[index] = ocean->m_Phillips[index] * ocean->m_Cycle;
}

void b3OceanWave::b3TestSpectrum1()
{
	b3Complex<b3_f64> *buffer = b3GetBuffer();

	buffer[5] = 0.5;
	buffer[0] = 1;

	buffer[m_fftDiff * m_fftDiff - 5 - m_fftDiff] = 0.5;
	buffer[m_fftDiff * m_fftDiff - 0 - m_fftDiff] = 1;
}

void b3OceanWave::b3TestSpectrum2()
{
	b3Complex<b3_f64> *buffer = b3GetBuffer();
	b3_loop            n,m;

	for (n = m_fftMin; n < m_fftMax; n++)
	{
		for(m = m_fftMin; m < m_fftMax; m++)
		{
			*buffer++ = sin(M_PI * 4.0 * m / m_fftDiff);
		}
	}
	m_FFT.b3FFT2D();
}

void b3OceanWave::b3TestSpectrum3()
{
	b3Tx tx;

//	tx.b3LoadImage("/home/sm/Kram/Bilder/openBC4.jpg");
	tx.b3LoadImage("s:/blz3/WWW/lines/normal/window_raytrace.jpg");
	m_FFT.b3AllocBuffer(&tx);
	m_FFT.b3FFT2D();
}

void b3OceanWave::b3TestSpectrum4()
{
	m_FFT.b3SelfTest();
	m_FFT.b3FFT2D();
}

void b3OceanWave::b3DumpImages()
{
#ifdef VERBOSE
	b3Tx tx;

//	m_FFT.b3SetAntiRaster(0.5, 0.25);
	m_FFT.b3GetSpectrum(&tx, 0.2);
#ifdef WIN32
	tx.b3SaveJPEG("c:/temp/spectrum.jpg");
#else
	tx.b3SaveJPEG("/tmp/spectrum.jpg");
#endif
#endif 		
 	m_FFT.b3IFFT2D();

#ifdef VERBOSE
	m_FFT.b3GetBuffer(&tx, 2000);
#ifdef WIN32
	tx.b3SaveJPEG("c:/temp/buffer.jpg");
#else
	tx.b3SaveJPEG("/tmp/buffer.jpg");
#endif
#endif
}
