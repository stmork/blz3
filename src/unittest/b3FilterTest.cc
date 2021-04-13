/*
**
**	$Filename:	b3FilterTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for filtering.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
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

#include "b3FilterTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3FilterTest);

void b3FilterTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3FilterTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3FilterTest::testBoxFilter()
{
	b3BoxFilter filter;

	const b3_f64 func     = testFunc(filter);
	const b3_f64 integral = testIntegral(filter);

	CPPUNIT_ASSERT_GREATER(0.0, func);
	CPPUNIT_ASSERT_GREATER(0.0, integral);
}

void b3FilterTest::testShutterFilter()
{
	b3ShutterFilter filter;

	const b3_f64 func     = testFunc(filter);
	const b3_f64 integral = testIntegral(filter);

	CPPUNIT_ASSERT_GREATER(0.0, func);
	CPPUNIT_ASSERT_GREATER(0.0, integral);
}

void b3FilterTest::testGaussFilter()
{
	b3GaussFilter filter;

	const b3_f64 func     = testFunc(filter);
	const b3_f64 integral = testIntegral(filter);

	CPPUNIT_ASSERT_GREATER(0.0, func);
	CPPUNIT_ASSERT_GREATER(0.0, integral);
}

b3_f64 b3FilterTest::testFunc(const b3Filter & filter)
{
	b3_f64 value = 0.0;

	for (double x = -10; x <= 10; x += step)
	{
		const b3_f64 left   = filter.b3Func(x - step * 0.5);
		const b3_f64 right  = filter.b3Func(x + step * 0.5);
		const b3_f64 sample = (left + right) * 0.5 * step;

		value += sample;
	}
	return value;
}

b3_f64 b3FilterTest::testIntegral(const b3Filter & filter)
{
	b3_f64 value = 0.0;

	for (double x = -10; x <= 10; x += step)
	{
		const b3_f64 left   = filter.b3Integral(x - step * 0.5);
		const b3_f64 right  = filter.b3Integral(x + step * 0.5);
		const b3_f64 sample = (left + right) * 0.5 * step;

		value += sample;
	}
	return value;
}

#endif
