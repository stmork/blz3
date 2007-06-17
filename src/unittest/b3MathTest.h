/*
**
**	$Filename:	b3MathTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision: 1392 $
**	$Date: 2006-05-12 16:06:28 +0200 (Fr, 12 Mai 2006) $
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
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void test();
};

#endif
#endif
