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

enum b3_fourier_type
{
	B3_FOURIER_UNDEFINED  = -1,
	B3_FOURIER_GREY       =  0,
	B3_FOURIER_PALETTE,
	B3_FOURIER_TRUECOLOR
};

union b3FilterInfo
{
	struct b3FilterInfoBandpass
	{
		b3_f64 start,end;
	} bp;
	struct b3FilterInfoLowpass
	{
		b3_f64 limit;
	} lp;
	struct b3FilterInfoHighpass
	{
		b3_f64 limit;
	} hp;
	struct b3FilterInfoAntiRaster
	{
		b3_f64 limit;
		b3_f64 corr;
	} ar;
	struct b3FilterInfoSinc2
	{
		b3_f64 factor;
	} si2;
	void *ptr;
};

class b3Fourier;

typedef void (*b3FilterFunc)(b3_f64 fx,b3_f64 fy,b3_index re,b3_index im,b3Fourier *fourier,b3FilterInfo *info);

/**
 * This class provides several spectral conversion methods.
 * The most common methods are the FFT/iFFT methods.
 */
class b3Fourier : public b3Mem
{
	b3_f64            *m_rBuffer;
	b3_f64            *m_gBuffer;
	b3_f64            *m_bBuffer;

	b3_f64           **m_rLine;
	b3_f64           **m_gLine;
	b3_f64           **m_bLine;

	b3_f64            *m_TempBuffer;
	b3_index          *m_PermBuffer;
	b3_f64            *m_Values;

	b3_res             m_xSize, m_xOrig, m_xStart;
	b3_res             m_ySize, m_yOrig, m_yStart;
	b3_fourier_type    m_Type;

public:
	        /**
	         * This constructor setup an empty Fourier buffer.
	         */
	        b3Fourier      ();

	        /**
	         * This constructor creates a Fourier buffer from a b3Tx instance.
	         *
	         * @param tx The texture with the values to initialize with.
	         */
	        b3Fourier      (b3Tx *image);

	        /**
	         * This copy constructor copies the values from another Fourier instance.
	         *
	         * @param src The source instance.
	         */
	        b3Fourier      (b3Fourier &src);
	void    b3GetBuffer    (b3Tx *tx);
	void    b3GetSpectrum  (b3Tx *tx);
	
	/**
	 * This method allocates a new buffer with the specified size.
	 *
	 * @param size The new dimension which is adjusted to the next power of 2 value.
	 * @param new_type The buffer type (true color or grey value)
	 * @return True on success.
	 */
	b3_bool b3AllocBuffer  (b3_res size, b3_fourier_type new_type = B3_FOURIER_GREY);

	inline b3_f64 * b3GetRBuffer()
	{
		return m_rBuffer;
	}

	inline b3_f64 * b3GetGBuffer()
	{
		return m_gBuffer;
	}

	inline b3_f64 * b3GetBBuffer()
	{
		return m_bBuffer;
	}


	void    b3Filter       (b3FilterInfo *info, b3FilterFunc func);
	void    b3SetBandpass  (b3_f64 frequency,b3_f64 bandwidth);
	void    b3SetLowpass   (b3_f64 frequency);
	void    b3SetHighpass  (b3_f64 frequency);
	void    b3SetAntiRaster(b3_f64 frequency,b3_f64 corridor);
	void    b3SetSinc2     (b3_f64 frequency);
	void    b3SetSinc2AR   (b3_f64 frequency);
	
	/**
	 * This method executes a forward FFT on the allocated buffers.
	 */
	void    b3FFT2D();

	/**
	 * This method exectues an inverse FFT on the allocated buffers.
	 */
	void    b3IFFT2D();
	
	/**
	 * This method provides a self test which executes a forward FFT and an inverse FFT afterwards.
	 */
	void    b3SelfTest();

protected:
	/**
	 * This method provides a bandpass filer callback method
	 *
	 * @param fx      The x coordinate.
	 * @param fy      The y coordinate.
	 * @param re      The index to the real component of the spectrum.
	 * @param im      The index to the imaginary component of the spectrum.
	 * @param fourier The calling Fourier class instance.
	 * @param info    The information of the bandpass filter.
	 */
	static void b3FilterBandpass  (b3_f64 fx,b3_f64 fy,b3_index re,b3_index im,b3Fourier *fourier,b3FilterInfo *info);
	static void b3FilterHighpass  (b3_f64 fx,b3_f64 fy,b3_index re,b3_index im,b3Fourier *fourier,b3FilterInfo *info);
	static void b3FilterLowpass   (b3_f64 fx,b3_f64 fy,b3_index re,b3_index im,b3Fourier *fourier,b3FilterInfo *info);
	static void b3FilterAntiRaster(b3_f64 fx,b3_f64 fy,b3_index re,b3_index im,b3Fourier *fourier,b3FilterInfo *info);
	static void b3FilterSinc2     (b3_f64 fx,b3_f64 fy,b3_index re,b3_index im,b3Fourier *fourier,b3FilterInfo *info);
	static void b3FilterSinc2AR   (b3_f64 fx,b3_f64 fy,b3_index re,b3_index im,b3Fourier *fourier,b3FilterInfo *info);

private:
	static b3_loop      b3PowOf2(b3_loop value);
	       void     b3Init();
	       b3_f64 **b3Alloc2D();

	// Onedimensional
	static void b3ComplexDFT(b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w);
	static void b3RealDFT   (b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w);
    static void b3DCT       (b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w);
	static void b3DST       (b3_loop n, b3_loop isgn, b3_f64 *a, b3_index *ip, b3_f64 *w);
	static void b3FourierDCT(b3_loop n, b3_f64 *a, b3_f64 *t, b3_index *ip, b3_f64 *w);
	static void b3FourierDST(b3_loop n, b3_f64 *a, b3_f64 *t, b3_index *ip, b3_f64 *w);

	// Twodimensional
	static void b3ComplexDFT2d(b3_loop n1, b3_loop n2, b3_loop isgn, b3_f64 **a, b3_f64 *t,b3_index *ip, b3_f64 *w);
	static void b3RealDFT2d(b3_loop n1, b3_loop n2, b3_loop isgn, b3_f64 **a, b3_f64 *t,b3_index *ip, b3_f64 *w);
	static void b3DCT2d(b3_loop n1, b3_loop n2, b3_loop isgn, b3_f64 **a, b3_f64 *t, b3_index *ip, b3_f64 *w);
	static void b3DST2d(b3_loop n1, b3_loop n2, b3_loop isgn, b3_f64 **a, b3_f64 *t, b3_index *ip, b3_f64 *w);

	static void b3MakeWT(b3_loop nw, b3_index *ip, b3_f64 *w);
	static void b3MakeCT(b3_loop nc, b3_index *ip, b3_f64 *c);
	static void b3BitRV2(b3_loop n, b3_index *ip, b3_f64 *a);
	static void b3BitRV2Conj(b3_loop n, b3_index *ip, b3_f64 *a);

	static void b3ComplexFT1st(b3_loop n, b3_f64 *a, b3_f64 *w);
	static void b3ComplexFTmdl(b3_loop n, b3_loop l, b3_f64 *a, b3_f64 *w);
	static void b3RealFTfsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c);
	static void b3RealFTbsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c);
	static void b3ComplexFTfsub(b3_loop n, b3_f64 *a, b3_f64 *w);
	static void b3ComplexFTbsub(b3_loop n, b3_f64 *a, b3_f64 *w);
	static void b3DCTsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c);
	static void b3DSTsub(b3_loop n, b3_f64 *a, b3_loop nc, b3_f64 *c);
};

#endif
