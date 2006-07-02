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

/*************************************************************************
**                                                                      **
**                        Unit test for math routines                   **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MathTest);

void b3MathTest::setUp()
{
}

void b3MathTest::tearDown()
{
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
	CPPUNIT_ASSERT_EQUAL(1.25,  b3Math::b3Mix(1.0,2.0,0.25));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Min(1.0,2.0));
	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Min(2.0,1.0));

	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Max(1.0,2.0));
	CPPUNIT_ASSERT_EQUAL(2.0,   b3Math::b3Max(2.0,1.0));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Limit(2.0));
	CPPUNIT_ASSERT_EQUAL(0.5,   b3Math::b3Limit(0.5));
	CPPUNIT_ASSERT_EQUAL(0.0,   b3Math::b3Limit(-1.0));

	CPPUNIT_ASSERT_EQUAL(1.0,   b3Math::b3Limit(0.5, 1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(2.5,   b3Math::b3Limit(2.5, 1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(4.0,   b3Math::b3Limit(4.5, 1.0, 4.0));

	for (base = 0;base < 16;base++)
	{
		for(exp = 0;exp < 16;exp++)
		{
			CPPUNIT_ASSERT_EQUAL(pow(base, static_cast<double>(exp)), b3Math::b3FastPow(base, exp));
		}
	} 

	for (base = -27; base <= 27;base += 0.125);
	{
		CPPUNIT_ASSERT_EQUAL(round(pow(base, 1.0 / 3) * 10000000), round(b3Math::b3Cbrt(base) * 10000000));
	}
}

#endif
