/*
**
**	$Filename:	TestFFT.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Testing FFT
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3FFT.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2006/04/29 20:29:54  sm
**	- Switched to other FFT 2D algorithm which works correctly.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestOceanWave implementation                **
**                                                                      **
*************************************************************************/

static void save(b3Tx *tx, const char *name, int num)
{
	char filename[B3_FILESTRINGLEN];

	snprintf(filename, sizeof(filename), "/tmp/%s_%03d.jpg");
	tx->b3SaveJPEG(name);
}

int main(int argc, char *argv[])
{
	b3Fourier       fft1;
	b3SimpleFourier fft2;
	b3Tx            tx;

	b3Log::b3SetLevel(B3LOG_FULL);
	fft1.b3SelfTest();
	fft2.b3SelfTest();

	tx.b3LoadImage("/home/sm/Blizzard/Textures/StanOlli.gif");
	fft2.b3AllocBuffer(&tx);
	fft2.b3GetBuffer(&tx, 1.0);
	tx.b3SaveJPEG("/tmp/stan_orig.jpg");

	fft2.b3IFFT2D();
	fft2.b3GetSpectrum(&tx, 1.0 / 256);
	tx.b3SaveJPEG("/tmp/stan_spectrum.jpg");

	fft2.b3FFT2D();
	fft2.b3GetBuffer(&tx, 1.0);
	tx.b3SaveJPEG("/tmp/stan_buffer.jpg");
	
	return EXIT_SUCCESS;
}
