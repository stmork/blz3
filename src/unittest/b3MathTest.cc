/*
**
**	$Filename:	b3MathTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for math routines.
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

#include "b3MathTest.h"
#include "b3TestMacros.h"
#include <blz3/base/b3FFT.h>

/*************************************************************************
**                                                                      **
**                        Unit test for math routines                   **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MathTest);

void b3MathTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3MathTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3MathTest::test()
{
	b3_f64 base;
	b3_u32 exp;

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Frac(4.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Frac(-4.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(0.0,   b3Math::b3Frac(4.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(0.125, b3Math::b3FracOne(4.125));
	CPPUNIT_ASSERT_EQUAL(0.875, b3Math::b3FracOne(-84.125));
	CPPUNIT_ASSERT_EQUAL(1.25,  b3Math::b3Mix(1.0, 2.0, 0.25));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Min(1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Min(2.0, 1.0));

	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Max(1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Max(2.0, 1.0));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Limit(2.0));
	CPPUNIT_ASSERT_EQUAL(0.5,   b3Math::b3Limit(0.5));
	CPPUNIT_ASSERT_EQUAL(0.0,   b3Math::b3Limit(-1.0));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Limit(0.5, 1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(2.5,   b3Math::b3Limit(2.5, 1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(4.0,   b3Math::b3Limit(4.5, 1.0, 4.0));

	for (base = 0; base < 13; base++)
	{
		for (exp = 0; exp < 16; exp++)
		{
			CPPUNIT_ASSERT_EQUAL(pow(base, static_cast<double>(exp)), b3Math::b3FastPow(base, exp));
		}
	}

	for (base = -81; base <= 81; base += 0.125)
	{
		const double expected = round(cbrt(base) * 10000000);
		const double actual   = round(b3Math::b3Cbrt(base) * 10000000);

		CPPUNIT_ASSERT_EQUAL(expected, actual);
	}
}

void b3MathTest::testLogPow()
{
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 0, b3Math::b3Log2(1));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 1, b3Math::b3Log2(2));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 1, b3Math::b3Log2(3));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(4));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(5));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(6));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 2, b3Math::b3Log2(7));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 3, b3Math::b3Log2(8));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_count, 3, b3Math::b3Log2(9));

	CPPUNIT_ASSERT_EQUAL(  1, b3Math::b3PowOf2(1));
	CPPUNIT_ASSERT_EQUAL(  2, b3Math::b3PowOf2(2));
	CPPUNIT_ASSERT_EQUAL(  4, b3Math::b3PowOf2(3));
	CPPUNIT_ASSERT_EQUAL(  4, b3Math::b3PowOf2(4));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(5));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(6));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(7));
	CPPUNIT_ASSERT_EQUAL(  8, b3Math::b3PowOf2(8));
	CPPUNIT_ASSERT_EQUAL( 16, b3Math::b3PowOf2(9));
}

#endif
