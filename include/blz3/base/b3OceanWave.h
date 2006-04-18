/*
**
**	$Filename:	b3OceanWave.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Ocean wave bump map
**	               functions.
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_OCEANWAVE_H
#define B3_BASE_OCEANWAVE_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Complex.h"
#include "blz3/base/b3FFT.h"

class B3_PLUGIN b3OceanWave
{
	const static b3_f64 g;

	b3_f64            m_t;           // actual time point
	b3_loop           m_fftMin;      // FFT loop start
	b3_loop           m_fftMax;      // FFT loop end
	b3_size           m_fftDiff;     // FFT loop size
	b3Complex<b3_f64> m_W;           // Normalized wind direction
	b3_f64            m_v;           // Wind speed
	b3_f64            m_xWind2;      // squared x component of wind direction
	b3_f64            m_yWind2;      // squared y component of wind direction
	b3_f64            m_Windspeed4;  // Quad wind speed
	b3_f64            m_g2;          // Squared gravity constant
	b3_f64            m_Scale;       // FFT grid scaling

	b3Fourier         m_FFT;
	b3Mutex           m_Mutex;

public:
	// time animation values
	b3_f64   m_T;      //!< Time period.
	b3_count m_Dim;    //!< FFT dimension as poer of two.

	b3_f32   m_Wx;     //!< Direction of the wind (x component).
	b3_f32   m_Wy;     //!< Direction of the wind (y component).
	b3_f32   m_A;      //!< Global factor.
	b3_f32   m_Size;   //!< Geometric field size

public:
	       b3OceanWave();

	/**
	 * This method precomputes some calculation invariant values.
	 */
	void   b3PrepareOceanWave();

	b3_f64 b3ComputeOceanWave(const b3_vector *pos, const b3_f64 t);

private:
	void              b3TestSpectrum1();
	void              b3TestSpectrum2();
	void              b3TestSpectrum3();
	void              b3TestSpectrum4();
	void              b3ComputePhillipsSpectrum();
	b3Complex<b3_f64> b3Height(const b3Complex<b3_f64> &k, const b3_f64 t);

	static void       b3FilterPhillipsSpectrum(
		b3_f64        fx,b3_f64        fy,
		b3_index      re,b3_index      im,
		b3Fourier    *fourier, 
		b3FilterInfo *filter_info);
};

#endif
