/*
**
**	$Filename:	b3MathTest.h $
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

#ifndef B3_MATH_TEST_H
#define B3_MATH_TEST_H

#include "blz3/base/b3Math.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class b3MathTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3MathTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testLogPow);
	CPPUNIT_TEST(testSmoothStep1);
	CPPUNIT_TEST(testSmoothStep2);
	CPPUNIT_TEST_SUITE_END();

	unsigned loops_s;
	b3_f32   epsilon_s;
	b3_f32   recipr_s;

	unsigned loops_d;
	b3_f64   epsilon_d;
	b3_f64   recipr_d;

	unsigned loops_x;
	b3_f96   epsilon_x;
	b3_f96   recipr_x;

public:
	b3MathTest();

	void setUp() override;
	void tearDown() override;
	void test();
	void testLogPow();
	void testSmoothStep1();
	void testSmoothStep2();

private:
	template<typename T> static inline T epsilon(unsigned & loops)
	{
		T        epsilon = 1.0;

		loops = 0;
		while (T(1.0 + 0.5 * epsilon) != 1.0)
		{
			epsilon = epsilon * 0.5;
			loops++;
		}

		return epsilon;
	}
};

#endif
#endif
