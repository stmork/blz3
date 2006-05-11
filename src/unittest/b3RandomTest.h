/*
**
**	$Filename:	b3RandomTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for random number generation.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_RANDOM_TEST_H
#define B3_RANDOM_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/base/b3Random.h"

using namespace std;

#define RANDOM_MAX   1000000
#define RANDOM_SPACE 100
#define RANDOM_ERROR 1.02

template<class R, typename T> class b3RandomTest : public CppUnit::TestFixture
{
	R random;

	CPPUNIT_TEST_SUITE(b3RandomTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		random.b3SetSeed();
	}

	void tearDown()
	{
	}

	void test()
	{
		T       count = 0;
		b3_loop i;
		b3_f64  err;

		for(i = 0;i < RANDOM_MAX;i++)
		{
			count += random.b3Rand(RANDOM_SPACE);
		}

		err = RANDOM_MAX * RANDOM_SPACE * 0.5 / count;

		CPPUNIT_ASSERT(err < RANDOM_ERROR);
		CPPUNIT_ASSERT(err > (1.0 / RANDOM_ERROR));
	}
};

#endif
