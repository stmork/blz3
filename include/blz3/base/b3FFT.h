/*
**
**	$Filename:	b3FFT.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Fourier transform (1d/2d)
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
#include "blz3/base/b3Complex.h"
#include "blz3/base/b3Random.h"

struct b3FilterInfo
{
};

typedef void (*b3SampleFunc)(const b3_f64 fx,const b3_f64 fy, const b3_index index, b3FilterInfo *info);

class B3_PLUGIN b3Fourier : protected b3Mem
{
	b3_res              m_xSize, m_xOrig, m_xStart, m_xDim;
	b3_res              m_ySize, m_yOrig, m_yStart, m_yDim;

	b3Complex<b3_f64>  *m_Buffer;
	b3Complex<b3_f64> **m_Lines;

	b3_f64             *m_xReal, *m_xImag;
	b3_f64             *m_yReal, *m_yImag;

	b3PseudoRandom<b3_f64> m_Random;

public:
	         b3Fourier();
	virtual ~b3Fourier();
	
	b3_bool b3AllocBuffer(b3_res new_size);
	void    b3AllocBuffer  (b3Tx *tx);
	void    b3FreeBuffer();
	void    b3Sample       (b3FilterInfo *info, b3SampleFunc func);

	inline void    b3FFT2D()
	{
		b3FFT2D(1);
	}

	inline void    b3IFFT2D()
	{
		b3FFT2D(-1);
	}

	void    b3GetBuffer    (b3Tx *tx, b3_f64 amp);
	void    b3GetSpectrum  (b3Tx *tx, b3_f64 amp);

	inline b3Complex<b3_f64> *b3GetBuffer()
	{
		return m_Buffer;
	}
	
	/**
	 * This method provides a self test which executes a forward FFT and an inverse FFT afterwards.
	 */
	void    b3SelfTest();

	static b3_loop  b3PowOf2(b3_loop value);
	static b3_count b3Log2(b3_u32 value);

private:
	static b3_bool b3FFT(int dir,b3_res m,b3_f64 *x,b3_f64 *y);
	       b3_bool b3FFT2D(int dir);
		   void    b3ReallocBuffer();
};

#endif
