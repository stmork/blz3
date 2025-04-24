/*
**
**	$Filename:	b3FFT.h $
**	$Release:	Dortmund 2006, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - 2D Fast Fourier transform
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_FFT_H
#define B3_BASE_FFT_H

#include "blz3/b3Config.h"
#include "blz3/image/b3Tx.h"
#include "blz3/base/b3Complex64.h"

enum b3_fft_error
{
	B3_FFT_ERROR     = -1,
	B3_FFT_OK        =  0,
	B3_FFT_NO_MEMORY,
	B3_FFT_NO_PALETTE,
	B3_FFT_SRC_TX_EMPTY
};

typedef b3Exception<b3_fft_error, 0x464654> b3FFTException;

class b3Fourier;

struct b3FilterInfo
{
	b3Fourier * m_Fourier = nullptr;
};

typedef void (*b3SampleFunc)(const b3_f64 fx, const b3_f64 fy, const b3_index index, b3FilterInfo * info);

struct b3_fft_info
{
	b3_loop        m_xMin, m_xMax;
	b3_loop        m_yMin, m_yMax;
	b3Complex64 ** m_Lines;
	b3Complex64  * m_Aux;
	b3_res         m_xDim;
	b3_res         m_yDim;
	int            m_Dir;
};

class B3_PLUGIN b3Fourier : protected b3Mem
{
	b3Complex64      *     m_Buffer = nullptr;
	b3Complex64     **     m_Lines  = nullptr;
	b3Complex64      *     m_Aux    = nullptr;
	b3_count               m_CPUs;

	b3_res                 m_xSize = 0, m_xOrig = 0, m_xStart = 0, m_xDim = 0;
	b3_res                 m_ySize = 0, m_yOrig = 0, m_yStart = 0, m_yDim = 0;

public:
	b3Fourier();
	virtual ~b3Fourier();

	/**
	 * This method allocates memory of the needed size for the FFT computation.
	 *
	 * @param new_size The needed new size
	 * @return True on success
	 */
	bool    b3AllocBuffer(const b3_res new_size);

	/**
	 * This method copies the values from a given image into the internal FFT
	 * buffer. It allocates the needed memory.
	 *
	 * @param tx The image with the values to initialize.
	 */
	bool    b3AllocBuffer(b3Tx * tx);

	/**
	 * This method frees all used memory.
	 */
	void    b3FreeBuffer();

	/**
	 * This method samples all values of the internal buffer. The internal buffer
	 * has to be in spatial domain.
	 *
	 * @param info The info structure to pass to the sample method.
	 * @param func The sample callback method which is called for every buffer sample.
	 */
	void    b3Sample(b3FilterInfo * info, b3SampleFunc func);

	/**
	 * This method computes the forward FFT of the internal buffer.
	 */
	inline bool    b3FFT2D()
	{
		return b3FFT2D(1);
	}

	/**
	 * This method computes the inverse FFT of the internal buffer.
	 */
	inline bool     b3IFFT2D()
	{
		return b3FFT2D(-1);
	}

	bool      b3GetBuffer(b3Tx * tx, b3_f64 amp) const;
	bool      b3GetSpectrum(b3Tx * tx, b3_f64 amp) const;

	inline b3Complex64 * b3GetBuffer() const
	{
		return m_Buffer;
	}

	/**
	 * This method provides a self test which executes a forward FFT and an inverse FFT afterwards.
	 */
	bool       b3SelfTest();

private:
	static  bool    b3RowFFT(void * ptr);
	static  bool    b3ColumnFFT(void * ptr);
	static  bool    b3FFT(const int dir, const b3_res m, b3Complex64 * line);

	bool    b3FFT2D(const int dir);
	bool    b3ReallocBuffer();
};

#endif
