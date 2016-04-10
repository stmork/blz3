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

typedef b3Exception<b3_fft_error,0x464654> b3FFTException;

class b3Fourier;

struct b3FilterInfo
{
	b3Fourier *m_Fourier;
};

typedef void (*b3SampleFunc)(const b3_f64 fx,const b3_f64 fy, const b3_index index, b3FilterInfo *info);

#define B3_FFT_MAX_THREADS 4

struct b3_fft_info
{
	b3_loop       m_xMin, m_xMax;
	b3_loop       m_yMin, m_yMax;
	b3Complex64 **m_Lines;
	b3Complex64  *m_Aux;
	b3_res        m_xDim;
	b3_res        m_yDim;
	int           m_Dir;
};

class B3_PLUGIN b3Fourier : protected b3Mem
{
	b3_res                 m_xSize, m_xOrig, m_xStart, m_xDim;
	b3_res                 m_ySize, m_yOrig, m_yStart, m_yDim;
//	b3_count               m_Size;

	b3Complex64           *m_Buffer;
	b3Complex64          **m_Lines;
	b3Complex64           *m_Aux;

	b3_count               m_CPUs;

public:
	b3Fourier();
	virtual ~b3Fourier();

	/**
	 * This method allocates memory of the needed size for the FFT computation.
	 *
	 * @param new_size The needed new size
	 * @return True on success
	 */
	const b3_bool b3AllocBuffer(const b3_res new_size);

	/**
	 * This method copies the values from a given image into the internal FFT
	 * buffer. It allocates the needed memory.
	 *
	 * @param tx The image with the values to initialize.
	 */
	const b3_bool b3AllocBuffer  (b3Tx *tx);

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
	void    b3Sample       (b3FilterInfo *info, b3SampleFunc func);

	/**
	 * This method computes the forward FFT of the internal buffer.
	 */
	inline const b3_bool b3FFT2D()
	{
		return b3FFT2D(1);
	}

	/**
	 * This method computes the inverse FFT of the internal buffer.
	 */
	inline const b3_bool  b3IFFT2D()
	{
		return b3FFT2D(-1);
	}

	const b3_bool   b3GetBuffer    (b3Tx *tx, b3_f64 amp) throw(b3FFTException);
	const b3_bool   b3GetSpectrum  (b3Tx *tx, b3_f64 amp);

	inline b3Complex64 *b3GetBuffer() const
	{
		return m_Buffer;
	}

	/**
	 * This method provides a self test which executes a forward FFT and an inverse FFT afterwards.
	 */
	const b3_bool    b3SelfTest();

	/**
	 * This method returns the number which is a power of 2 number
	 * and greater or equal to the given number.
	 *
	 * @param value The value to compute the power of 2 number from.
	 * @return The value which is a power of two number and greater or equal to the given number.
	 */
	static const b3_loop  b3PowOf2(const b3_loop value);

	/**
	 * This method returns the log of the given value.
	 *
	 * @param value The value to compute the log2 from.
	 * @return The resulting log2 value.
	 */
	static const b3_count b3Log2(b3_u32 value);

private:
	static       b3_u32  b3RowFFT(void *ptr);
	static       b3_u32  b3ColumnFFT(void *ptr);
	static const b3_bool b3FFT(const int dir, const b3_res m, b3Complex64 *line);

	const b3_bool b3FFT2D(const int dir);
	const b3_bool b3ReallocBuffer();
};

#endif
