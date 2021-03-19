/*
**
**	$Filename:	b3MatrixTest.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for matrix and vector operations.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_MATRIX_TEST_H
#define B3_MATRIX_TEST_H

#include "blz3/base/b3Matrix.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class b3MatrixTest : public CppUnit::TestFixture
{
	b3_vector    as, bs, cs, ds;
	b3_vector64  ad, bd, cd, dd;
	b3_gl_vector ag, bg, cg, dg;
	b3_line64    ld;
	b3_line      ls;

	b3_matrix    am, bm, cm, dm;

	constexpr static double DELTA = 0.0000001;

	CPPUNIT_TEST_SUITE(b3MatrixTest);
	CPPUNIT_TEST(testVector);
	CPPUNIT_TEST(testMatrix);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testVector();
	void testMatrix();
};

#endif
#endif
