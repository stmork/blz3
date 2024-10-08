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
	CPPUNIT_TEST(testVectorInit);
	CPPUNIT_TEST(testVectorSMul);
	CPPUNIT_TEST(testVectorLinearCombine);
	CPPUNIT_TEST(testVectorBounds);
	CPPUNIT_TEST(testVectorMix);
	CPPUNIT_TEST(testMatrix);
	CPPUNIT_TEST(testMatrixTranspose);
	CPPUNIT_TEST(testMatrixMul);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testVectorInit();
	void testVectorSMul();
	void testVectorLinearCombine();
	void testVectorBounds();
	void testVectorMix();
	void testMatrix();
	void testMatrixTranspose();
	void testMatrixMul();

private:
	void init(b3_matrix & m, const b3_f32 factor = 1.0f);
};

#endif
#endif
