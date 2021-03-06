/*
**
**	$Filename:	b3FFTTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - FFT unit test.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3FFTTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        FFT unit test                                 **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3FFTTest);

#define TEST_RE 1.5
#define TEST_IM 2.5

void b3FFTTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	dim = 1 << 4;
}

void b3FFTTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3FFTTest::testFFT()
{
	b3Tx tx;

	for (b3_loop size = 1; size <= 2048; size += size)
	{
		CPPUNIT_ASSERT_NO_THROW(fft.b3AllocBuffer(size));
		CPPUNIT_ASSERT_NO_THROW(fft.b3SelfTest());
	}

	tx.b3LoadImage("fft_test.gif");
	CPPUNIT_ASSERT_NO_THROW(fft.b3AllocBuffer(&tx));
	CPPUNIT_ASSERT(fft.b3GetBuffer(&tx, 1.0));
	tx.b3SaveJPEG("fft_orig.jpg");

	CPPUNIT_ASSERT(fft.b3IFFT2D());
	CPPUNIT_ASSERT(fft.b3GetSpectrum(&tx, 1.0 / 256));
	tx.b3SaveJPEG("fft_spectrum.jpg");

	CPPUNIT_ASSERT(fft.b3FFT2D());
	CPPUNIT_ASSERT(fft.b3GetBuffer(&tx, 1.0));
	tx.b3SaveJPEG("fft_buffer.jpg");
}

void b3FFTTest::testSample()
{
	CPPUNIT_ASSERT_NO_THROW(fft.b3AllocBuffer(dim));

	CPPUNIT_ASSERT_NO_THROW(fft.b3Sample(this, setbuffer));
	CPPUNIT_ASSERT_EQUAL((TEST_RE + TEST_IM) * dim * dim, b3Count());

	CPPUNIT_ASSERT_NO_THROW(fft.b3Sample(this, clear));
	CPPUNIT_ASSERT_EQUAL(0.0, b3Count());
}

b3_f64 b3FFTTest::b3Count()
{
	b3_f64        count = 0;
	b3_loop       i, max = dim * dim;
	b3Complex64 * buffer = fft.b3GetBuffer();

	CPPUNIT_ASSERT(buffer != nullptr);
	for (i = 0; i < max; i++)
	{
		count += fabs(buffer[i].b3Real());
		count += fabs(buffer[i].b3Imag());
	}
	return count;
}

void b3FFTTest::clear(
	const b3_f64     fx B3_UNUSED,
	const b3_f64     fy B3_UNUSED,
	const b3_index   index,
	b3FilterInfo  *  filter_info)
{
	b3Complex64 * buffer = filter_info->m_Fourier->b3GetBuffer();

	buffer[index] = 0;
}

void b3FFTTest::setbuffer(
	const b3_f64     fx B3_UNUSED,
	const b3_f64     fy B3_UNUSED,
	const b3_index   index,
	b3FilterInfo  *  filter_info)
{
	b3Complex64 * buffer = filter_info->m_Fourier->b3GetBuffer();

	buffer[index] = b3Complex64(TEST_RE, TEST_IM);
}

void b3FFTTest::testOceanWave()
{
	CPPUNIT_ASSERT_NO_THROW(ocean.b3PrepareOceanWave(0));
	CPPUNIT_ASSERT_NO_THROW(ocean.b3DumpImages());
}

#endif
