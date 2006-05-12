/*
**
**	$Filename:	b3ComplexTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Complex number unit test.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_COMPLEX_TEST_H
#define B3_COMPLEX_TEST_H

#include "blz3/base/b3Complex.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class b3ComplexTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3ComplexTest);
	CPPUNIT_TEST(testOps);
	CPPUNIT_TEST(testEqual);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testOps();
	void testEqual();

private:
	b3Complex<b3_f64> a, b, e;
	b3Complex<b3_f64> *c , *d;
};

#endif
#endif
