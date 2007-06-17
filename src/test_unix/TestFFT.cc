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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/05/01 10:44:46  sm
**	- Unifying ocean wave values.
**
**	Revision 1.2  2006/04/30 08:53:24  sm
**	- Removed some signed/unsigned issues.
**	- Reflect new FFT algorithm.
**	
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

	snprintf(filename, sizeof(filename), "/tmp/%s_%03d.jpg", name, num);
	tx->b3SaveJPEG(name);
}

int main(int argc, char *argv[])
{
	b3Fourier       fft2;
	b3Tx            tx;
	b3OceanWave     ocean;
	char           *HOME = getenv("HOME");
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
