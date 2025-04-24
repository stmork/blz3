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

#pragma once

#ifndef B3_BASE_OCEANWAVE_H
#define B3_BASE_OCEANWAVE_H

#include "blz3/b3Config.h"
#include "blz3/base/b3FFT.h"
#include "blz3/base/b3Random.h"
#include "blz3/base/b3Spline.h"

#define B3_OCEAN_MAX_DIM      12
#define B3_OCEAN_MAX          (1 << B3_OCEAN_MAX_DIM)
#define B3_OCEAN_CURVE_DEGREE  2
#define B3_OCEAN_XSKIP         1
#define B3_OCEAN_MAX_MASK     (B3_OCEAN_MAX - B3_OCEAN_XSKIP)

class B3_PLUGIN b3OceanWave : protected b3Mem, protected b3FilterInfo
{
	static constexpr b3_f64 g = 9.81;

	b3_f64                  m_t;           // actual time point
	b3_f64                  m_omega_t;     // omega t
	b3_loop                 m_fftMin;      // FFT loop start
	b3_loop                 m_fftMax;      // FFT loop end
	b3_index                m_fftDiff;     // FFT loop size
	b3_index                m_fftMask;
	b3_f64                  m_W2;          // Quad wind speed
	b3_f64                  m_GridScale;   // FFT grid scaling
	b3_f64                  m_L;           // windspeed and gravity constant
	b3_f64                  m_L2;          // squard factor
	b3_f64                  m_l2;          // wave length lower limit (squared)
	b3_f64                  m_Denom;       // Height field corrector
	b3Complex64             m_Cycle;       // e^j*omega*t
	b3Fourier               m_FFT;
	b3Rand48<b3_f64>        m_Random;
	b3Complex64      *      m_Phillips = nullptr;
	b3_vector       *       m_Normals  = nullptr;
	bool                    m_Modified = true;

public:
	// time animation values
	b3_f64   m_T;          //!< Time period.
	b3_count m_Dim;        //!< FFT dimension as power of two.
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

	/**
	 * This method computes a height value for the given position.
	 *
	 * @param pos The position of the height value to be computed.
	 * @return The computed height value.
	 */
	b3_f64 b3ComputeOceanWave(const b3_vector * pos);

	/**
	 * This method computes the slope values for the given position.
	 *
	 * @param pos The position of the slope values to be computed.
	 * @param n The computed slope values.
	 */
	void   b3ComputeOceanWaveDeriv(const b3_vector * pos, b3_vector * n) const;

	/**
	 * This returns the FFT forward buffer as texture.
	 *
	 * @param tx The image to put the buffer in.
	 * @param scale Scales all values by this factor.
	 */
	inline void b3GetBuffer(b3Tx * tx, b3_f64 scale) const
	{
		m_FFT.b3GetBuffer(tx, scale);
	}

	/**
	 * This returns the FFT spectral buffer as texture.
	 *
	 * @param tx The image to put the spectrum in.
	 * @param scale A global scaling factor.
	 */
	inline void b3GetSpectrum(b3Tx * tx, b3_f64 scale)
	{
		m_FFT.b3GetSpectrum(tx, scale);
	}

	/**
	 * Returns the value field of the height field.
	 *
	 * @return The height field.
	 */
	inline b3Complex64 * b3GetBuffer() const
	{
		return m_FFT.b3GetBuffer();
	}

	/**
	 * This method flags the ocean wave instance whether a Phillips
	 * Spectrum recomputation should be performed.
	 *
	 * @param modified A flag which indicates a modifiaction of computation values.
	 */
	void b3Modified(bool modified = true);

	/**
	 * This method dumps the actual computed Phillips spectrum as height field and
	 * spectrum into the temp directory.
	 */
	void              b3DumpImages();

	/**
	 * This method copies a height field into an image.
	 *
	 * @param tx The retrieving image.
	 */
	void b3CopyHeightField(const b3Tx * tx);

protected:
	void              b3TestSpectrum1();
	void              b3TestSpectrum2();
	void              b3TestSpectrum3();
	void              b3TestSpectrum4();

private:
	void              b3ComputePhillipsSpectrum();

	static void       b3FilterPhillipsSpectrum(
		const b3_f64        fx,
		const b3_f64        fy,
		const b3_index      index,
		b3FilterInfo * filter_info);

	static void       b3SampleHeight(
		const b3_f64        fx,
		const b3_f64        fy,
		const b3_index      index,
		b3FilterInfo * filter_info);

	void              b3SamplePhillipsSpectrum(b3_f64 fx, b3_f64 fy, b3_index index);
	void              b3PrepareNormals();

	inline b3_index  b3GetIndex(const b3_index x, const b3_index y) const
	{
		return ((y & m_fftMask) << m_Dim) | (x & m_fftMask);
	}

	inline b3_f64     b3GetHeight(const b3_index x, const b3_index y)
	{
		b3Complex64 * buffer = b3GetBuffer();

		return buffer[b3GetIndex(x, y)].b3Real();
	}
};

#endif
