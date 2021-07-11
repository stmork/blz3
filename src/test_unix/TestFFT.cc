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
#include "blz3/base/b3OceanWave.h"

/*************************************************************************
**                                                                      **
**                        b3TestOceanWave implementation                **
**                                                                      **
*************************************************************************/

int main()
{
	b3Fourier       fft2;
	b3Tx            tx;
	b3OceanWave     ocean;
	char      *     HOME = getenv("HOME");
	char            StanOlliImage[B3_FILESTRINGLEN];

	snprintf(StanOlliImage, sizeof(StanOlliImage), "%s/Blizzard/Textures/StanOlli.gif", HOME);

	b3Log::b3SetLevel(B3LOG_FULL);
	fft2.b3SelfTest();

	tx.b3LoadImage(StanOlliImage);
	fft2.b3AllocBuffer(&tx);
	fft2.b3GetBuffer(&tx, 1.0);
	tx.b3SaveJPEG("/tmp/stan_orig.jpg");

	fft2.b3IFFT2D();
	fft2.b3GetSpectrum(&tx, 1.0 / 256);
	tx.b3SaveJPEG("/tmp/stan_spectrum.jpg");

	fft2.b3FFT2D();
	fft2.b3GetBuffer(&tx, 1.0);
	tx.b3SaveJPEG("/tmp/stan_buffer.jpg");

	ocean.b3PrepareOceanWave(0);
	ocean.b3DumpImages();

	return EXIT_SUCCESS;
}
