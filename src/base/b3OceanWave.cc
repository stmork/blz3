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
	m_Dim      =    10;
	m_Wx       =     1;
	m_Wy       =     0.7f;
	m_A        =    50;
	m_GridSize =   400;
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
	m_fftMax     =  1 << (m_Dim - 1);
	m_fftMin     = -m_fftMax;
	m_fftDiff    =  m_fftMax - m_fftMin;
	m_GridScale  =  1.0 / m_GridSize;
	
	// Time values
	m_t          =  t;
	m_omega_t    =  M_PI * 2.0 * m_t / m_T;
	m_Cycle.b3SetRe(cos(m_omega_t));
	m_Cycle.b3SetIm(sin(m_omega_t));

	// Physics
	m_W2         =  m_Wx * m_Wx + m_Wy * m_Wy;;
	m_L          =  m_W2 / g;
	m_L2         = -m_L * m_L;
	m_l2         =  m_l * m_l;

	m_FFT.b3AllocBuffer(1 << m_Dim, B3_FOURIER_GREY);
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
	b3_f64   *buffer = b3GetBuffer();
	b3_f64    fx = b3Math::b3FracOne(pos->x * m_GridScale) * m_fftDiff, dx;
	b3_f64    fy = b3Math::b3FracOne(pos->y * m_GridScale) * m_fftDiff, dy;
	b3_index  x,y;
	b3_size   index, max = m_fftDiff * m_fftDiff;
	b3_f64    a[2], b[2], c[2];

	x = (b3_index)fx & B3_OCEAN_MAX_MASK;
	y = (b3_index)fy;

	dx = (fx - x) / B3_OCEAN_XSKIP;
	dy =  fy - y;

	index = y * m_fftDiff + x;
	a[0] = buffer[index];

	index += B3_OCEAN_XSKIP;
	if (index >= max)
	{
		index -= m_fftDiff;
	}
	b[0] = buffer[index];

	index += m_fftDiff;
	if (index >= max)
	{
		index -= max;
	}
	b[1] = buffer[index];

	index -= B3_OCEAN_XSKIP;
	if (index < 0)
	{
		index += m_fftDiff;
	}
	a[1] = buffer[index];

	for (b3_loop i = 0;i < 2;i++)
	{
		c[i] = a[i] + dx * (b[i] - a[i]);
	}

	return b3Math::b3Mix(c[0], c[1], 0) * 0.5 + 0.5;
}

void b3OceanWave::b3ComputeOceanWaveDeriv(const b3_vector *pos, b3_vector *n)
{
	b3_f64   *buffer = b3GetBuffer();
	b3_f64    fx     = b3Math::b3FracOne(pos->x * m_GridScale) * m_fftDiff, dx;
	b3_f64    fy     = b3Math::b3FracOne(pos->y * m_GridScale) * m_fftDiff, dy;
	b3_index  x,y;
	b3_size   index, max = m_fftDiff * m_fftDiff;
	b3_f64    h, hx, hy;

	x = (b3_index)fx & B3_OCEAN_MAX_MASK;
	y = (b3_index)fy;

	index = y * m_fftDiff + x;
	h = buffer[index];

	index += B3_OCEAN_XSKIP;
	if (index >= max)
	{
		index -= m_fftDiff;
	}

	dx = (fx - x) / B3_OCEAN_XSKIP;
	if (dx == 0)
	{
		dx = 1;
		hx = buffer[index] - h;
	}
	else
	{
		hx = b3Math::b3Mix(h, buffer[index], dx) - h;
	}

	index += m_fftDiff;
	if (index >= max)
	{
		index -= max;
	}

	index-=4;
	if (index < 0)
	{
		index += m_fftDiff;
	}

	dy =  fy - y;
	if (dy == 0)
	{
		dy = 1;
		hy = buffer[index] - h;
	}
	else
	{
		hy = b3Math::b3Mix(h, buffer[index], dy) - h;
	}

	n->x = -hx / dx;
	n->y = -hy / dy;
	n->z = 1.0f;
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
	      b3_f64       &re,
	      b3_f64       &im,
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
	b3_f64             quotient = fabs(K.b3GetRe() * m_Wx + K.b3GetIm() * m_Wy);
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
		K.b3GetRe(), K.b3GetIm(),
		k2, k4, m_L2, quotient, phillips);
#endif

	b3_f64 nx = K.b3GetRe();
	b3_f64 ny = K.b3GetIm();

	do
	{
		gauss.b3SetRe(m_Random.b3Rand(2.0) - 1.0);
		gauss.b3SetIm(m_Random.b3Rand(2.0) - 1.0);
		w  = gauss.b3SquareLength();
	}
	while (w >= 1.0);

//	gauss *= sqrt( (-2.0 * log( w ) ) / w );
//	gauss *= sqrt(phillips * 0.5);
	gauss *= sqrt( -phillips * log(w) / w);

#ifdef VERBOSE_DUMP
	b3PrintF(B3LOG_NORMAL, " # %f %f %f\n",
		gauss.b3GetRe(),  gauss.b3GetIm(), w);
#endif

	m_Phillips[index] = gauss;
}

void b3OceanWave::b3SampleHeight(
	const b3_f64        fx,
	const b3_f64        fy,
	      b3_f64       &re,
	      b3_f64       &im,
	const b3_index      index,
	      b3FilterInfo *filter_info)
{
	b3OceanWave       *ocean  = (b3OceanWave *)filter_info;
	b3_f64            *buffer = ocean->b3GetBuffer();
	b3Complex<b3_f64>  result = ocean->m_Phillips[index] * ocean->m_Cycle;
	
	re = result.b3GetRe();
	im = result.b3GetIm();
}

void b3OceanWave::b3TestSpectrum1()
{
	b3_f64 *buffer = b3GetBuffer();

	buffer[5] = 0.5;
	buffer[0] = 1;

	buffer[m_fftDiff * m_fftDiff - 5 - m_fftDiff] = 0.5;
	buffer[m_fftDiff * m_fftDiff - 0 - m_fftDiff] = 1;
}

void b3OceanWave::b3TestSpectrum2()
{
	b3_f64            *buffer = b3GetBuffer();
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
#ifdef VERBOSE_DUMP
	m_FFT.b3DumpBuffer(B3LOG_FULL);
#endif
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

#ifdef VERBOSE_DUMP
	m_FFT.b3DumpSpectrum(B3LOG_FULL);
#endif
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
#ifdef VERBOSE_DUMP
	m_FFT.b3DumpBuffer(B3LOG_FULL);
#endif
	m_FFT.b3GetBuffer(&tx, 2000);
#ifdef WIN32
	tx.b3SaveJPEG("c:/temp/buffer.jpg");
#else
	tx.b3SaveJPEG("/tmp/buffer.jpg");
#endif
#endif
}
