/*
**
**	$Filename:	b3MatrixTest.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3MatrixTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit tests for matrix and vector ops          **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MatrixTest);

void b3MatrixTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3MatrixTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3MatrixTest::testVectorInit()
{
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as));
	CPPUNIT_ASSERT_EQUAL(0.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(0.0f, as.y);
	CPPUNIT_ASSERT_EQUAL(0.0f, as.z);

	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as, 1.0, 2.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(2.0f, as.y);
	CPPUNIT_ASSERT_EQUAL(3.0f, as.z);

	CPPUNIT_ASSERT_EQUAL(&ad, b3Vector::b3Init(&ad));
	CPPUNIT_ASSERT_EQUAL(0.0, ad.x);
	CPPUNIT_ASSERT_EQUAL(0.0, ad.y);
	CPPUNIT_ASSERT_EQUAL(0.0, ad.z);

	CPPUNIT_ASSERT_EQUAL(&ad, b3Vector::b3Init(&ad, 1.0, 2.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(1.0, ad.x);
	CPPUNIT_ASSERT_EQUAL(2.0, ad.y);
	CPPUNIT_ASSERT_EQUAL(3.0, ad.z);

	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Init(&bs, &as));
	CPPUNIT_ASSERT_EQUAL(1.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(2.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(3.0f, bs.z);

	CPPUNIT_ASSERT_EQUAL(&bd, b3Vector::b3Init(&bd, &ad));
	CPPUNIT_ASSERT_EQUAL(1.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(2.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(3.0, bd.z);

	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as, -3.0, 2.0, -1.0));
	CPPUNIT_ASSERT_EQUAL(&ad, b3Vector::b3Init(&ad, &as));
	CPPUNIT_ASSERT_EQUAL(-3.0, ad.x);
	CPPUNIT_ASSERT_EQUAL(2.0, ad.y);
	CPPUNIT_ASSERT_EQUAL(-1.0, ad.z);

	CPPUNIT_ASSERT_EQUAL(&ad, b3Vector::b3Init(&ad, -3.0, 2.0, -1.0));
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as, &ad));
	CPPUNIT_ASSERT_EQUAL(-3.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(2.0f, as.y);
	CPPUNIT_ASSERT_EQUAL(-1.0f, as.z);

	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Init(&bs, &as));
	CPPUNIT_ASSERT(b3Vector::b3IsEqual(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Negate(&bs));
	CPPUNIT_ASSERT_EQUAL(3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(1.0f, bs.z);

	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&bd, b3Vector::b3Init(&bd, &ad));
	CPPUNIT_ASSERT(b3Vector::b3IsEqual(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&bd, b3Vector::b3Negate(&bd));
	CPPUNIT_ASSERT_EQUAL(3.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(-2.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(1.0, bd.z);
}

void b3MatrixTest::testVectorSMul()
{
	CPPUNIT_ASSERT_EQUAL(&as,     b3Vector::b3Init(&as, -3.0,  2.0, -1.0));
	CPPUNIT_ASSERT_EQUAL(&ad,     b3Vector::b3Init(&ad, &as));
	CPPUNIT_ASSERT_EQUAL(&bs,     b3Vector::b3Init(&bs,  2.0, -1.0, -2.0));
	CPPUNIT_ASSERT_EQUAL(-6.0f,   b3Vector::b3SMul(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(-6.0,    b3Vector::b3SMul(&ad, &bs));
	CPPUNIT_ASSERT_EQUAL( 3.0f,   b3Vector::b3Length(&bs));
	CPPUNIT_ASSERT_EQUAL( 9.0f,   b3Vector::b3QuadLength(&bs));
	CPPUNIT_ASSERT_EQUAL( 3.0f,   b3Vector::b3Normalize(&bs));
	CPPUNIT_ASSERT_EQUAL( 1.0f,   b3Vector::b3Length(&bs));
	CPPUNIT_ASSERT_EQUAL( 1.0f,   b3Vector::b3Normalize(&bs, 6.0));
	CPPUNIT_ASSERT_EQUAL( 6.0f,   b3Vector::b3Length(&bs));

	CPPUNIT_ASSERT_EQUAL(&bd,   b3Vector::b3Init(&bd, 2.0, -1.0, -2.0));
	CPPUNIT_ASSERT_EQUAL(-6.0,  b3Vector::b3SMul(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(-6.0,  b3Vector::b3SMul(&as, &bd));
	CPPUNIT_ASSERT_EQUAL( 3.0,  b3Vector::b3Length(&bd));
	CPPUNIT_ASSERT_EQUAL( 9.0,  b3Vector::b3QuadLength(&bd));
	CPPUNIT_ASSERT_EQUAL( 3.0,  b3Vector::b3Normalize(&bd));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,  b3Vector::b3Length(&bd), DELTA);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,  b3Vector::b3Normalize(&bd, 6.0), DELTA);
	CPPUNIT_ASSERT_EQUAL(6.0,  b3Vector::b3Length(&bd));

	CPPUNIT_ASSERT_EQUAL(&as,  b3Vector::b3Init(&as, 1.0, -3.0, -4.0));
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Init(&bs, 2.0, -1.0, -2.0));
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Add(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL(3.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(-4.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(-6.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Sub(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL(-1.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(-2.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Mul(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL(2.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(3.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(8.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3CrossProduct(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL(2.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(-6.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(5.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3CrossProduct(&as, &bs, &cd));
	CPPUNIT_ASSERT_EQUAL(2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(-6.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Add(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(-4.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(-6.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Sub(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(-1.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(-2.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Mul(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(3.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(8.0f, bs.z);

	CPPUNIT_ASSERT_EQUAL(&ad,  b3Vector::b3Init(&ad, 1.0, -3.0, -4.0));
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Init(&bd, 2.0, -1.0, -2.0));
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Add(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(3.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(-4.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(-6.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Sub(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(-1.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(-2.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(-2.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Mul(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(3.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(8.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3CrossProduct(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(-6.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3CrossProduct(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(-6.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Add(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(3.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(-4.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(-6.0, bd.z);
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Sub(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(2.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(-1.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(-2.0, bd.z);
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Mul(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(2.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(3.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(8.0, bd.z);

	CPPUNIT_ASSERT_EQUAL(&as,  b3Vector::b3Init(&as, 1.0, -1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Init(&bs, 2.0,  1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(3.0f, b3Vector::b3Distance(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Scale(&as, &cs, 2.5));
	CPPUNIT_ASSERT_EQUAL(2.5f, cs.x);
	CPPUNIT_ASSERT_EQUAL(-2.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL(5.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Scale(&as, &cs, -2.0));
	CPPUNIT_ASSERT_EQUAL(-2.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(2.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(-4.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Scale(&cs, -2.0));
	CPPUNIT_ASSERT_EQUAL(4.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(-4.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(8.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3LinearCombine(&as, &bs, 2.5, &cs));
	CPPUNIT_ASSERT_EQUAL(6.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(1.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL(12.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cg,  b3Vector::b3LinearCombine(&as, &bs, 2.5, &cg));
	CPPUNIT_ASSERT_EQUAL(6.0f, cg.x);
	CPPUNIT_ASSERT_EQUAL(1.5f, cg.y);
	CPPUNIT_ASSERT_EQUAL(12.0f, cg.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3LinearCombine(&as, &bs, 2.5, -2.0, &cs));
	CPPUNIT_ASSERT_EQUAL(-1.5f, cs.x);
	CPPUNIT_ASSERT_EQUAL(-4.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL(-3.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, &ds));
	CPPUNIT_ASSERT_EQUAL(9.0f, ds.x);
	CPPUNIT_ASSERT_EQUAL(10.5f, ds.y);
	CPPUNIT_ASSERT_EQUAL(18.0f, ds.z);
	CPPUNIT_ASSERT_EQUAL(&dg,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, &dg));
	CPPUNIT_ASSERT_EQUAL(9.0f, dg.x);
	CPPUNIT_ASSERT_EQUAL(10.5f, dg.y);
	CPPUNIT_ASSERT_EQUAL(18.0f, dg.z);
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, 2.0, &ds));
	CPPUNIT_ASSERT_EQUAL(-4.5f, ds.x);
	CPPUNIT_ASSERT_EQUAL(-13.5f, ds.y);
	CPPUNIT_ASSERT_EQUAL(-9.0f, ds.z);
}

void b3MatrixTest::testVectorLinearCombine()
{
	CPPUNIT_ASSERT_EQUAL(&ad,  b3Vector::b3Init(&ad, 1.0, -1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Init(&bd, 2.0,  1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(3.0,  b3Vector::b3Distance(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Scale(&ad, &cd, 2.5));
	CPPUNIT_ASSERT_EQUAL(2.5, cd.x);
	CPPUNIT_ASSERT_EQUAL(-2.5, cd.y);
	CPPUNIT_ASSERT_EQUAL(5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Scale(&ad, &cd, -2.0));
	CPPUNIT_ASSERT_EQUAL(-2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(2.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(-4.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Scale(&cd, -2.0));
	CPPUNIT_ASSERT_EQUAL(4.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(-4.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(8.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3LinearCombine(&ad, &bd, 2.5, &cd));
	CPPUNIT_ASSERT_EQUAL(6.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(1.5, cd.y);
	CPPUNIT_ASSERT_EQUAL(12.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3LinearCombine(&ad, &bd, 2.5, -2.0, &cd));
	CPPUNIT_ASSERT_EQUAL(-1.5, cd.x);
	CPPUNIT_ASSERT_EQUAL(-4.5, cd.y);
	CPPUNIT_ASSERT_EQUAL(-3.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&as,  b3Vector::b3Init(&as, &ad));
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Init(&bs, &bd));
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Init(&cs, &cd));
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3LinearCombine(&as, &bs, 2.5, -2.0, &cd));
	CPPUNIT_ASSERT_EQUAL(-1.5, cd.x);
	CPPUNIT_ASSERT_EQUAL(-4.5, cd.y);
	CPPUNIT_ASSERT_EQUAL(-3.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&ad, &bd, &cd, 2.5, -2.0, &dd));
	CPPUNIT_ASSERT_EQUAL(9.0, dd.x);
	CPPUNIT_ASSERT_EQUAL(10.5, dd.y);
	CPPUNIT_ASSERT_EQUAL(18.0, dd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, &dd));
	CPPUNIT_ASSERT_EQUAL(9.0, dd.x);
	CPPUNIT_ASSERT_EQUAL(10.5, dd.y);
	CPPUNIT_ASSERT_EQUAL(18.0, dd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, 2.0, &dd));
	CPPUNIT_ASSERT_EQUAL(-4.5, dd.x);
	CPPUNIT_ASSERT_EQUAL(-13.5, dd.y);
	CPPUNIT_ASSERT_EQUAL(-9.0, dd.z);

	CPPUNIT_ASSERT_EQUAL(&ld,  b3Vector::b3Init(&ld, &ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3LinearCombine(&ld, 2.5, &cs, &ds));
	CPPUNIT_ASSERT_EQUAL(7.5f, ds.x);
	CPPUNIT_ASSERT_EQUAL(6.0f, ds.y);
	CPPUNIT_ASSERT_EQUAL(15.0f, ds.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&ld, 2.5, &dd));
	CPPUNIT_ASSERT_EQUAL(6.0,  dd.x);
	CPPUNIT_ASSERT_EQUAL(1.5,  dd.y);
	CPPUNIT_ASSERT_EQUAL(12.0,  dd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&ld, 2.5, &dd));
}

void b3MatrixTest::testVectorBounds()
{
	CPPUNIT_ASSERT_EQUAL(&as,  b3Vector::b3Init(&as, 1.0,  1.5, 4.0));
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Init(&bs, 2.0, -1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Init(&cs, &as));
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3Init(&ds, &bs));
	b3Vector::b3Sort(&as, &bs);
	CPPUNIT_ASSERT_EQUAL(1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(-1.0f, as.y);
	CPPUNIT_ASSERT_EQUAL(2.0f, as.z);
	CPPUNIT_ASSERT_EQUAL(2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(1.5f, bs.y);
	CPPUNIT_ASSERT_EQUAL(4.0f, bs.z);

	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3SetMinimum(&as, 0.5));
	CPPUNIT_ASSERT_EQUAL(1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(0.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(2.0f, as.z);

	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3CheckLowerBound(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(0.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(2.0f, as.z);

	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3SetMaximum(&as, 1.5));
	CPPUNIT_ASSERT_EQUAL(1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(0.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(1.5f, as.z);

	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3CheckUpperBound(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(2.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(1.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(4.0f, as.z);

	b3Vector::b3AdjustBound(&as, &cs, &ds);
	CPPUNIT_ASSERT_EQUAL(1.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(1.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL(4.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(2.0f, ds.x);
	CPPUNIT_ASSERT_EQUAL(1.5f, ds.y);
	CPPUNIT_ASSERT_EQUAL(4.0f, ds.z);

	CPPUNIT_ASSERT_EQUAL(&am, b3Matrix::b3Move(nullptr, &am, 1.0, 2.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Init(&bs, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3MatrixMul3D(&am, &bs));
	CPPUNIT_ASSERT_EQUAL(2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(1.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Init(&bs, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3MatrixMul4D(&am, &bs));
	CPPUNIT_ASSERT_EQUAL(3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(0.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(4.0f, bs.z);
}

void b3MatrixTest::testVectorMix()
{
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as,  1.0,  2.0, -4.0));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Init(&bs, -1.0, -4.0,  2.0));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&as, &bs));

	CPPUNIT_ASSERT_EQUAL(&cs, b3Vector::b3Mix(&as, &bs, 0.0, &cs));
	CPPUNIT_ASSERT( b3Vector::b3IsEqual(&as, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&bs, &cs));

	CPPUNIT_ASSERT_EQUAL(&cs, b3Vector::b3Mix(&as, &bs, 1.0, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&as, &cs));
	CPPUNIT_ASSERT( b3Vector::b3IsEqual(&bs, &cs));

	CPPUNIT_ASSERT_EQUAL(&cs, b3Vector::b3Mix(&as, &bs, 0.2, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&as, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&bs, &cs));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.6, cs.x, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.8, cs.y, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.8, cs.z, b3Math::epsilon);

	CPPUNIT_ASSERT_EQUAL(&cs, b3Vector::b3Mix(&as, &bs, 0.5, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&as, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&bs, &cs));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, cs.x, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, cs.y, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, cs.z, b3Math::epsilon);

	CPPUNIT_ASSERT_EQUAL(&cs, b3Vector::b3Mix(&as, &bs, 0.8, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&as, &cs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&bs, &cs));
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.6, cs.x, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.8, cs.y, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.8, cs.z, b3Math::epsilon);

	CPPUNIT_ASSERT_EQUAL(&ad, b3Vector::b3Init(&ad, &as));
	CPPUNIT_ASSERT_EQUAL(&bd, b3Vector::b3Init(&bd, &bs));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&ad, &bd));

	CPPUNIT_ASSERT_EQUAL(&cd, b3Vector::b3Mix(&ad, &bd, 0.0, &cd));
	CPPUNIT_ASSERT( b3Vector::b3IsEqual(&ad, &cd));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&bd, &cd));

	CPPUNIT_ASSERT_EQUAL(&cd, b3Vector::b3Mix(&ad, &bd, 1.0, &cd));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&ad, &cd));
	CPPUNIT_ASSERT( b3Vector::b3IsEqual(&bd, &cd));

	CPPUNIT_ASSERT_EQUAL(&cd, b3Vector::b3Mix(&ad, &bd, 0.5, &cd));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&ad, &cd));
	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&bd, &cd));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, cd.x, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, cd.y, b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, cd.z, b3Math::epsilon);
}

void b3MatrixTest::testMatrix()
{
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as, 1.0, -1.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Init(&bs, 1.0,  0.0, 0.0));
	CPPUNIT_ASSERT_EQUAL(&cs, b3Vector::b3Init(&cs, 0.0,  1.0, 0.0));
	CPPUNIT_ASSERT_EQUAL(&ds, b3Vector::b3Init(&ds, 0.0,  0.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(&ls, b3Vector::b3Init(&ls, &cs, &ds));

	CPPUNIT_ASSERT_EQUAL(&am, b3Matrix::b3Unit(&am));
	CPPUNIT_ASSERT(b3Matrix::b3IsUnitMatrix(&am));
	CPPUNIT_ASSERT_EQUAL(&bm, b3Matrix::b3Inverse(&am, &bm));
	am.m11 = 0;
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_matrix *, nullptr, b3Matrix::b3Inverse(&am, &cm));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Copy(&am, &dm));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Move(nullptr, &dm, &as));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Move(nullptr, &dm, 1, 2, 3));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MoveNegative(nullptr, &dm, &bs));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Scale(nullptr, &dm, &as, &bs));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Scale(nullptr, &dm, &as, 2.0, 3.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateX(nullptr, &dm, &as, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateY(nullptr, &dm, &as, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateZ(nullptr, &dm, &as, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateVector(nullptr, &dm, &ls, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPoint(nullptr, &dm, &as, 1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPoint(nullptr, &dm, &as, -1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorAxis(nullptr, &dm, &ls, 1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorAxis(nullptr, &dm, &ls, -1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPlane(nullptr, &dm, &as, &bs, &cs, 1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPlane(nullptr, &dm, &as, &bs, &cs, -1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Align(&dm, &ls));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Dress(&cm, &dm, &as, &bs, &cs, false));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Dress(&cm, &dm, &as, &bs, &cs, true));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MMul(&am, &bm, &dm));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MAdd(&am, &bm, &dm));

	CPPUNIT_ASSERT_EQUAL(&am, b3Matrix::b3Move(nullptr, &am, 1.0, 2.0, 3.0));
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Matrix::b3VMul(&am, &as, &bs, false));
	CPPUNIT_ASSERT_EQUAL(2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(1.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL(&bs, b3Matrix::b3VMul(&am, &as, &bs, true));
	CPPUNIT_ASSERT_EQUAL(3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(0.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(4.0f, bs.z);

	CPPUNIT_ASSERT_EQUAL(&ad, b3Vector::b3Init(&ad, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(&bd, b3Matrix::b3VMul(&am, &ad, &bd, false));
	CPPUNIT_ASSERT_EQUAL( 2.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(-2.0, bd.y);
	CPPUNIT_ASSERT_EQUAL( 1.0, bd.z);
	CPPUNIT_ASSERT_EQUAL(&bd, b3Matrix::b3VMul(&am, &ad, &bd, true));
	CPPUNIT_ASSERT_EQUAL(3.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(0.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(4.0, bd.z);

	CPPUNIT_ASSERT_TYPED_EQUAL(const b3_matrix *, &am, b3Matrix::b3Dump(&am));
	CPPUNIT_ASSERT_TYPED_EQUAL(const b3_matrix *, &bm,
		b3Matrix::b3Dump(&bm, "Matrix unit test"));
}

void b3MatrixTest::testMatrixTranspose()
{
	b3_matrix a, result;

	init(a);

	CPPUNIT_ASSERT_EQUAL( 1.0f, a.m11);
	CPPUNIT_ASSERT_EQUAL( 4.0f, a.m14);
	CPPUNIT_ASSERT_EQUAL( 5.0f, a.m21);
	CPPUNIT_ASSERT_EQUAL( 6.0f, a.m22);
	CPPUNIT_ASSERT_EQUAL(13.0f, a.m41);
	CPPUNIT_ASSERT_EQUAL(16.0f, a.m44);

	CPPUNIT_ASSERT_EQUAL(&result, b3Matrix::b3Transpose(&a, &result));

	const b3_f32 * rPtr = &result.m11;

	for (b3_loop y = 0; y < 4; y++)
	{
		for (b3_loop x = 0; x < 4; x++)
		{
			const b3_f32 expected = x * 4 + y + 1;

			CPPUNIT_ASSERT_EQUAL(expected, *rPtr++);
		}
	}

	CPPUNIT_ASSERT_EQUAL( 1.0f, result.m11);
	CPPUNIT_ASSERT_EQUAL( 5.0f, result.m12);
	CPPUNIT_ASSERT_EQUAL(13.0f, result.m14);
	CPPUNIT_ASSERT_EQUAL( 6.0f, result.m22);
	CPPUNIT_ASSERT_EQUAL( 4.0f, result.m41);
	CPPUNIT_ASSERT_EQUAL(16.0f, result.m44);
}

void b3MatrixTest::testMatrixMul()
{
	b3_matrix a, b, result;

	b3Matrix::b3Unit(&a);
	b3Matrix::b3Unit(&b);
	b3Matrix::b3MMul(&a, &b, &result);
	CPPUNIT_ASSERT(b3Matrix::b3IsUnitMatrix(&result));

	init(a);
	init(b, 2.0f);
	CPPUNIT_ASSERT(!b3Matrix::b3IsEqual(&a, &b));

	b3Matrix::b3MMul(&a, &b, &result);
	CPPUNIT_ASSERT_EQUAL( 240.0f, result.m14);
	CPPUNIT_ASSERT_EQUAL( 560.0f, result.m24);
	CPPUNIT_ASSERT_EQUAL( 880.0f, result.m34);
	CPPUNIT_ASSERT_EQUAL(1200.0f, result.m44);

	init(a);
	init(b, 2.0f);
	b3Matrix::b3MMul(&a, &b, &a);
	CPPUNIT_ASSERT(b3Matrix::b3IsEqual(&a, &result));

	init(a);
	init(b, 2.0f);
	b3Matrix::b3MMul(&a, &b, &b);
	CPPUNIT_ASSERT(b3Matrix::b3IsEqual(&b, &result));
}

void b3MatrixTest::init(b3_matrix & m, const b3_f32 factor)
{
	b3_f32 * ptr = &m.m11;
	b3_f32   val = 1;

	for (b3_loop y = 0; y < 4; y++)
	{
		for (b3_loop x = 0; x < 4; x++)
		{
			const b3_f32 expected = y * 4 + x + 1;

			CPPUNIT_ASSERT_EQUAL(expected, val);
			*ptr++ = factor * val++;
		}
	}
}

#endif
