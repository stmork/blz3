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
#include "blz3/base/b3Random.h"
#include "blz3/base/b3Spline.h"

#define B3_OCEAN_MAX_DIM      12
#define B3_OCEAN_MAX          (1 << B3_OCEAN_MAX_DIM)
#define B3_OCEAN_CURVE_DEGREE  2
#define B3_OCEAN_XSKIP         1
#define B3_OCEAN_MAX_MASK     (B3_OCEAN_MAX - B3_OCEAN_XSKIP)

class B3_PLUGIN b3OceanWave : protected b3Mem
{
	const static b3_f64 g;

	b3_f64                  m_t;           // actual time point
	b3_f64                  m_omega_t;     // omega t
	b3_loop                 m_fftMin;      // FFT loop start
	b3_loop                 m_fftMax;      // FFT loop end
	b3_size                 m_fftDiff;     // FFT loop size
	b3_f64                  m_W2;          // Quad wind speed
	b3_f64                  m_GridScale;   // FFT grid scaling
	b3_f64                  m_L;           // windspeed and gravity constant
	b3_f64                  m_L2;          // squard factor
	b3_f64                  m_l2;          // wave length lower limit (squared)
	b3Complex<b3_f64>       m_Cycle;       // e^j*omega*t
	b3SimpleFourier         m_FFT;
	b3PseudoRandom<b3_f64>  m_Random;
	b3Complex<b3_f64>      *m_Phillips;
	b3_bool                 m_Modified;

public:
	// time animation values
	b3_f64   m_T;          //!< Time period.
	b3_count m_Dim;        //!< FFT dimension as poer of two.
	b3_f32   m_Wx;         //!< Direction of the wind (x component).
	b3_f32   m_Wy;         //!< Direction of the wind (y component).
	b3_f32   m_A;          //!< Global factor.
	b3_f32   m_GridSize;   //!< Geometric field size
	b3_f32   m_l;          //!< Lower limit of wave length.

public:
	       b3OceanWave();
		   virtual ~b3OceanWave();

	/**
	 * This method precomputes some calculation invariant values.
	 */
	void   b3PrepareOceanWave(const b3_f64 t);

	b3_f64 b3ComputeOceanWave(const b3_vector *pos);
	void   b3ComputeOceanWaveDeriv(const b3_vector *pos, b3_vector *n);

	/**
	 * This returns the FFT forward buffer as texture.
	 *
	 * @param tx The image to put the buffer in.
	 * @param scale Scales all values by this factor.
	 */
	inline void b3GetBuffer    (b3Tx *tx, b3_f64 scale = 1000.0)
	{
		m_FFT.b3GetBuffer(tx, scale);
	}


	/**
	 * This returns the FFT spectral buffer as texture.
	 *
	 * @param tx The image to put the spectrum in.
	 */
	inline void b3GetSpectrum(b3Tx *tx)
	{
		m_FFT.b3GetSpectrum(tx);
	}

	/**
	 * Returns the value field of the height field.
	 *
	 * @return The height field.
	 */
	inline b3_f64 *b3GetBuffer()
	{
		return m_FFT.b3GetGBuffer();
	}

	inline void b3Modified(b3_bool modified=true)
	{
		m_Modified = modified;
	}
private:
	void              b3ComputePhillipsSpectrum();

	static void       b3FilterPhillipsSpectrum(
		const b3_f64        fx,
		const b3_f64        fy,
		      b3_f64       &re,
		      b3_f64       &im,
		const b3_index      index,
		      b3FilterInfo *filter_info);

	static void       b3SampleHeight(
		const b3_f64        fx,
		const b3_f64        fy,
		      b3_f64       &re,
		      b3_f64       &im,
		const b3_index      index,
		      b3FilterInfo *filter_info);

	void              b3SamplePhillipsSpectrum(b3_f64 fx, b3_f64 fy, b3_index index);
	void              b3TestSpectrum1();
	void              b3TestSpectrum2();
	void              b3TestSpectrum3();
	void              b3TestSpectrum4();
	void              b3DumpImages();
};

#endif
