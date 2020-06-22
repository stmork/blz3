/*
**
**	$Filename:	b3OceanWave.c $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
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
#include "blz3/system/b3TimeStop.h"
#include "blz3/base/b3OceanWave.h"
#include "blz3/base/b3Procedure.h"

#define not_VERBOSE
#define not_VERBOSE_DUMP

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
	m_A        =     1;
	m_GridSize =   400;
	m_l        =     0.0f;
	m_Denom    = 1.0 / 4096.0;

	m_Phillips = null;
	m_Normals  = null;
	m_Modified = true;
}

b3OceanWave::~b3OceanWave()
{
	if (m_Phillips != null)
	{
		delete [] m_Phillips;
	}
}

void b3OceanWave::b3Modified(b3_bool modified)
{
	m_Modified = modified;
	if (m_Phillips != null)
	{
		delete [] m_Phillips;
		m_Phillips = null;
	}
	if (m_Normals != null)
	{
		b3Free(m_Normals);
		m_Normals = null;
	}
}

void b3OceanWave::b3PrepareOceanWave(const b3_f64 t)
{
	b3PrintF(B3LOG_FULL, "  preparing ocean waves...\n");

	m_Dim = b3Math::b3Limit(m_Dim, 5, 11);

	// Grid values
	m_fftMax         =  1 << (m_Dim - 1);
	m_fftMin         = -m_fftMax;
	m_fftDiff        =  m_fftMax - m_fftMin;
	m_fftMask        =  m_fftDiff - 1;
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

	if (!m_FFT.b3AllocBuffer(1 << m_Dim))
	{
		B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
	}
	m_Random.b3SetSeed(0xcafe);

	b3PrintF(B3LOG_DEBUG, "     T   = %1.3f\n", m_T);
	b3PrintF(B3LOG_DEBUG, "     t   = %1.3f\n", m_t);
	b3PrintF(B3LOG_DEBUG, "     L   = %1.3f\n", m_L);
	b3PrintF(B3LOG_DEBUG, "     L²  = %1.3f\n", m_L2);
	b3PrintF(B3LOG_DEBUG, "    |w|² = %1.3f\n", m_W2);
	b3PrintF(B3LOG_DEBUG, "     l²  = %1.3f\n", m_l2);

#if 0
	b3TestSpectrum3();
#else
	b3ComputePhillipsSpectrum();
#endif
	m_FFT.b3IFFT2D();
	b3PrepareNormals();

#ifdef VERBOSE
	b3DumpImages();
#endif
}

b3_f64 b3OceanWave::b3ComputeOceanWave(const b3_vector *pos)
{
	b3TimeStop             stop("Prepare ocean wave");
	b3Complex64           *buffer = b3GetBuffer();
	b3_f64                 fx     = b3Math::b3FracOne(pos->x * m_GridScale) * m_fftDiff;
	b3_f64                 fy     = b3Math::b3FracOne(pos->y * m_GridScale) * m_fftDiff;
	b3_f64                 dy;
	b3_index               max    = m_fftDiff * m_fftDiff;
	b3_index               index, xs, xe, y;
	b3_f64    B3_ALIGN_16  a[2], b[2], c[2], dx[2];

	xs = (b3_index)fx;
	y  = (b3_index)fy;
	xe = (xs + 1) & m_fftMask;

	dx[0] = dx[1] = fx - xs;
	dy    =         fy - y;

	index = y << m_Dim;
	a[0] = buffer[index + xs].b3Real();
	b[0] = buffer[index + xe].b3Real();

	index += m_fftDiff;
	if (index >= max)
	{
		index -= max;
	}
	a[1] = buffer[index + xs].b3Real();
	b[1] = buffer[index + xe].b3Real();

	for (b3_loop i = 0;i < 2;i++)
	{
		c[i] = a[i] + dx[i] * (b[i] - a[i]);
	}

	return b3Math::b3Mix(c[0], c[1], dy);
}

void b3OceanWave::b3ComputeOceanWaveDeriv(
	const b3_vector *pos, b3_vector *n)
{
	b3_f64    fx = b3Math::b3FracOne(pos->x * m_GridScale) * m_fftDiff, dx;
	b3_f64    fy = b3Math::b3FracOne(pos->y * m_GridScale) * m_fftDiff;
	b3_index  x, y;
	b3_index  ls, le, us, ue;
	b3_vector l,u;

	x  = (b3_index)fx;
	y  = (b3_index)fy;
	dx = fx - x;

	ls = b3GetIndex(x,   y);
	le = b3GetIndex(x+1, y);
	us = b3GetIndex(x,   y+1);
	ue = b3GetIndex(x+1, y+1);

	b3Vector::b3Mix(&m_Normals[ls], &m_Normals[le], dx, &l);
	b3Vector::b3Mix(&m_Normals[us], &m_Normals[ue], dx, &u);
}

void b3OceanWave::b3ComputePhillipsSpectrum()
{
	b3TimeStop stop("Compute Phillips spectrum");

	if (m_Phillips == null)
	{
		b3_size size = m_fftDiff * m_fftDiff;

		m_Phillips = new b3Complex64[size];
		if (m_Phillips == null)
		{
			B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
		}
		m_Normals = (b3_vector *)b3Alloc(size * sizeof(b3_vector));
		if (m_Normals == null)
		{
			B3_THROW(b3FFTException, B3_FFT_NO_MEMORY);
		}
		m_Modified = true;
	}

	if (m_Modified)
	{
		m_FFT.b3Sample(this, b3FilterPhillipsSpectrum);
		m_Modified = false;
	}

	m_FFT.b3Sample(this, b3SampleHeight);
}

void b3OceanWave::b3PrepareNormals()
{
	b3_index   x,y;
	b3_vector *normal = m_Normals;
	b3_f64     grid   = m_GridSize / m_fftDiff;

	for (y = 0;y < m_fftDiff;y++)
	{
		for (x = 0;x < m_fftDiff;x++)
		{
			normal->x = (b3GetHeight(x+1, y)   - b3GetHeight(x-1, y)) * m_Denom;
			normal->y = (b3GetHeight(x,   y+1) - b3GetHeight(x,   y-1)) * m_Denom;
			normal->z = 4 * grid;
			normal++;
		}
	}
}

void b3OceanWave::b3CopyHeightField(b3Tx *tx)
{
	b3Complex64  *buffer = b3GetBuffer();
	b3_pkd_color *ptr    = tx->b3GetTrueColorData();
	b3_res        xSize  = tx->xSize;
	b3_res        ySize  = tx->ySize;

	for (b3_loop y = 0;y < ySize;y++)
	{
		for (b3_loop x = 0;x < xSize;x++)
		{
			b3_f32 c = buffer[b3GetIndex(x,y)].b3Real() * m_Denom + 0.5;

			*ptr++ = b3Color(c, c, c);
		}
	}
}

void b3OceanWave::b3FilterPhillipsSpectrum(
	const b3_f64        fx,
	const b3_f64        fy,
	const b3_index      index,
	b3FilterInfo *filter_info)
{
	b3OceanWave *ocean  = (b3OceanWave *)filter_info;

	ocean->b3SamplePhillipsSpectrum(fx, fy, index);
}

void b3OceanWave::b3SamplePhillipsSpectrum(b3_f64 fx, b3_f64 fy, b3_index index)
{
	b3_f64 factor   = M_PI * 2 * m_fftMax / m_GridSize;
	b3_f64 Kr       = fx * factor;
	b3_f64 Ki       = fy * factor;
	b3_f64 k2       = Kr * Kr + Ki * Ki;
	b3_f64 k4       = k2 * k2;
	b3_f64 quotient = fabs(Kr * m_Wx + Ki * m_Wy);
	b3_f64 hf_scale = exp(-m_l2 * k2);
	b3_f64 gr, gi;
	b3_f64 w;
	b3_f64 phillips;

	if (k4 == 0)
	{
		phillips = 0;
	}
	else
	{
		phillips = m_A * exp( m_L2 / k2 ) * quotient * quotient * quotient * hf_scale / k4;
	}

#ifdef VERBOSE_DUMP
	b3PrintF(B3LOG_NORMAL,"f: %f %f - K: %f %f # k²=%f k^4=%f L²=%f q=%f P=%f",
			 fx, fy,
			 Kr, Ki,
			 k2, k4, m_L2, quotient, phillips);
#endif


	do
	{
		gr = m_Random.b3Rand(2.0) - 1.0;
		gi = m_Random.b3Rand(2.0) - 1.0;

		w  = gr * gr + gi * gi;
	}
	while (w >= 1.0);

	m_Phillips[index] = b3Complex64(gr, gi) * sqrt( -phillips * log(w) / w);

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
	b3OceanWave *ocean  = (b3OceanWave *)filter_info;
	b3Complex64 *buffer = ocean->b3GetBuffer();

	buffer[index] = ocean->m_Phillips[index] * ocean->m_Cycle;
}

void b3OceanWave::b3TestSpectrum1()
{
	b3Complex64 *buffer = b3GetBuffer();

	buffer[5] = 0.5;
	buffer[0] = 1;

	buffer[m_fftDiff * m_fftDiff - 5 - m_fftDiff] = 0.5;
	buffer[m_fftDiff * m_fftDiff - 0 - m_fftDiff] = 1;
}

void b3OceanWave::b3TestSpectrum2()
{
	b3Complex64 *buffer = b3GetBuffer();
	b3_loop      n,m;

	for (n = m_fftMin; n < m_fftMax; n++)
	{
		for (m = m_fftMin; m < m_fftMax; m++)
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
	b3Tx tx;

	m_FFT.b3FFT2D();
	m_FFT.b3GetSpectrum(&tx, 1);
#ifdef HAVE_LIBJPEG
	tx.b3SaveJPEG("ow_spectrum.jpg");
#else
	// TODO: Save TGA image instead.
#endif
	m_FFT.b3IFFT2D();

	m_FFT.b3GetBuffer(&tx, 0.001);
#ifdef HAVE_LIBJPEG
	tx.b3SaveJPEG("ow_buffer.jpg");
#else
	// TODO: Save TGA image instead.
#endif
}
