/*
**
**	$Filename:	b3FFTTest.h $
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

#ifndef B3_FFT_TEST_H
#define B3_FFT_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3FFT.h"
#include "blz3/base/b3OceanWave.h"

using namespace std;

class b3FFTTest : public CppUnit::TestFixture, protected b3FilterInfo
{
	b3Fourier    fft;
	b3OceanWave  ocean;
	b3_res       dim;

	CPPUNIT_TEST_SUITE(b3FFTTest);
	CPPUNIT_TEST(testFFT);
	CPPUNIT_TEST(testSample);
	CPPUNIT_TEST(testOceanWave);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testFFT();
	void testSample();
	void testOceanWave();

private:
	static void clear(
		const b3_f64        fx,
		const b3_f64        fy,
		const b3_index      index,
		      b3FilterInfo *filter_info);

	static void setbuffer(
		const b3_f64        fx,
		const b3_f64        fy,
		const b3_index      index,
		      b3FilterInfo *filter_info);

	b3_f64 b3Count();
};

#endif
