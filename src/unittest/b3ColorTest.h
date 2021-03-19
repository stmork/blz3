/*
**
**	$Filename:	b3ColorTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Base and system unit tests
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_COLOR_TEST_H
#define B3_COLOR_TEST_H

#include "blz3/base/b3Color.h"
#include "blz3/base/b3Math.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class b3ColorTest : public CppUnit::TestFixture
{
	b3Color color;
	b3Color ac;
	b3Color bc;

	CPPUNIT_TEST_SUITE(b3ColorTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();

	inline b3_f32 round_color(b3_f32 x)
	{
		b3_f32 rnd = 10000.0f;

		return roundf(x * rnd) / rnd;
	}
};

#endif
#endif
