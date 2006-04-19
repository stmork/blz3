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

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	m_t     = -FLT_MAX;
	m_T     =    10;
	m_Dim   =     8;
	m_Wx    =     1;
	m_Wy    =     0.7f;
	m_A     =  1000;
	m_Size  =    64;
}

void b3OceanWave::b3PrepareOceanWave()
{
	b3PrintF(B3LOG_FULL, "  preparing ocean waves...\n");
	m_W          = b3Complex<b3_f64>(m_Wx, m_Wy);
	m_v          = m_W.b3Length();
	m_W.b3Normalize();

	m_fftMax     =  1 << (m_Dim - 1);
	m_fftMin     = -m_fftMax;
	m_fftDiff    =  m_fftMax - m_fftMin;
	m_Scale      =  1.0 / m_Size;

	m_xWind2     =  m_Wx * m_Wx;
	m_yWind2     =  m_Wy * m_Wy;
	m_Windspeed4 =  m_xWind2 * m_xWind2 + m_yWind2 * m_yWind2;
	m_g2         = -g*g;

	m_FFT.b3AllocBuffer(1 << m_Dim, B3_FOURIER_GREY);
}

b3_f64 b3OceanWave::b3ComputeOceanWave(const b3_vector *pos, const b3_f64 t)
{
	b3_f64   *buffer;
	b3_f64    result;
	b3_f64    fx = pos->x * m_Scale;
	b3_f64    fy = pos->y * m_Scale;
	b3_index  x,y;

	// FIXME: SLOW!!!
	m_Mutex.b3Lock();
	if (m_t != t)
	{
		m_t = t;
#if 0
		b3TestSpectrum3();
#else
		b3ComputePhillipsSpectrum();
#endif

#ifdef VERBOSE
		b3Tx tx;

		m_FFT.b3DumpSpectrum(B3LOG_FULL);
		m_FFT.b3GetSpectrum(&tx);
		tx.b3SaveJPEG("/tmp/spectrum.jpg");
#endif 		
 		m_FFT.b3IFFT2D();
 		
#ifdef VERBOSE
 		m_FFT.b3DumpBuffer(B3LOG_FULL);
		m_FFT.b3GetBuffer(&tx);
		tx.b3SaveJPEG("/tmp/buffer.jpg");
#endif
	}
	m_Mutex.b3Unlock();

	buffer = m_FFT.b3GetGBuffer();
	x = (b3_index)(b3Math::b3FracOne(fx) * m_fftDiff);
	y = (b3_index)(b3Math::b3FracOne(fy) * m_fftDiff);
	result = buffer[y * m_fftDiff + x] * 0.5 + 0.5;

	return result;
}

void b3OceanWave::b3ComputePhillipsSpectrum()
{
	m_FFT.b3Filter((b3FilterInfo *)this, b3FilterPhillipsSpectrum);
}

void b3OceanWave::b3FilterPhillipsSpectrum(
	b3_f64        fx,
	b3_f64        fy,
	b3_index      re,
	b3_index      im,
	b3Fourier    *fourier, 
	b3FilterInfo *filter_info)
{
	b3OceanWave       *ocean  = (b3OceanWave *)filter_info;
	b3Complex<b3_f64>  K(fx * ocean->m_Size, fy * ocean->m_Size);
	b3Complex<b3_f64>  result = ocean->b3Height(K, ocean->m_t);
	b3_f64            *buffer = fourier->b3GetGBuffer();

	buffer[re] = result.b3GetRe();
	buffer[im] = result.b3GetIm();
}

b3Complex<b3_f64> b3OceanWave::b3Height(const b3Complex<b3_f64> &K, const b3_f64 t)
{
	b3_f64            w;
	b3_f64            phillips;
	b3Complex<b3_f64> gauss;
	b3_f64            k2    = K.b3SquareLength();
	b3_f64            k4    = k2 * k2;
	b3_f64            denom = k4 * fabs(K.b3GetRe() * m_Wx + K.b3GetIm() * m_Wy);

	if(denom == 0)
	{
		phillips = 0;
	}
	else
	{
		phillips = m_A * exp( m_g2 / (k2 * k2 * m_Windspeed4) ) / denom;
	}

#ifdef VERBOSE
	b3PrintF(B3LOG_NORMAL,"%f %f # %f %f %f %f %f %f %f",
		K.b3GetRe(), K.b3GetIm(),
		k2,k4,m_xWind2,m_yWind2,m_Windspeed4,m_g2, phillips);
#endif

	do
	{
		gauss.b3SetRe(B3_FRAN(2.0) - 1.0);
		gauss.b3SetIm(B3_FRAN(2.0) - 1.0);
		w  = gauss.b3SquareLength();
	}
	while (w >= 1.0);

	w = sqrt( (-2.0 * log( w ) ) / w );

    b3Complex<b3_f64> result = gauss * w * sqrt(phillips * 0.5);

#ifdef VERBOSE
	b3PrintF(B3LOG_NORMAL, " # %f %f %f # %f %f\n", x1, x2, w, result.b3GetRe(), result.b3GetIm());
#endif
	return result;
}

void b3OceanWave::b3TestSpectrum1()
{
	b3_f64 *buffer = m_FFT.b3GetGBuffer();

	buffer[5] = 0.5;
	buffer[0] = 1;

	buffer[m_fftDiff * m_fftDiff - 5 - m_fftDiff] = 0.5;
	buffer[m_fftDiff * m_fftDiff - 0 - m_fftDiff] = 1;
}

void b3OceanWave::b3TestSpectrum2()
{
	b3_f64            *buffer;
	b3_loop            n,m;

	buffer = m_FFT.b3GetGBuffer();
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

	tx.b3LoadImage("/home/sm/Kram/Bilder/openBC4.jpg");
	m_FFT.b3AllocBuffer(&tx);
	m_FFT.b3DumpBuffer(B3LOG_FULL);
	m_FFT.b3FFT2D();
}

void b3OceanWave::b3TestSpectrum4()
{
	m_FFT.b3SelfTest();
	m_FFT.b3FFT2D();
}
