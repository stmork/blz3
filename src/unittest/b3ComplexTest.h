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
#include "blz3/base/b3Complex64.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3ComplexTest : public CppUnit::TestFixture
{
	b3Complex<b3_f64>   a, b, e;
	b3Complex<b3_f64> * c, *d;
	b3Complex64         ca, cb, cc, cd, ce;

	CPPUNIT_TEST_SUITE(b3ComplexTest);
	CPPUNIT_TEST(testOps);
	CPPUNIT_TEST(testEqual);
	CPPUNIT_TEST(testComplex64);
	CPPUNIT_TEST(testStdComplex);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testOps();
	void testEqual();
	void testComplex64();
	void testStdComplex();
};

#endif
#endif
