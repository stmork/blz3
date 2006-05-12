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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2006/05/12 14:06:28  smork
**	- Added configurable CPPUNIT tests.
**
**	Revision 1.2  2006/05/11 18:48:24  sm
**	- Small makefile fixes.
**	- Corrected accuracy in unit tests.
**	
**	Revision 1.1  2006/05/11 15:34:23  sm
**	- Added unit tests
**	- Corrected normal computation for ocean waves
**	- Optimized b3Complex
**	- Added new FFT
**	- Added own assertion include
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Unit tests for matrix and vector ops          **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MatrixTest);

void b3MatrixTest::setUp()
{
}

void b3MatrixTest::tearDown()
{
}

void b3MatrixTest::testVector()
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
	CPPUNIT_ASSERT_EQUAL( 2.0, ad.y);
	CPPUNIT_ASSERT_EQUAL(-1.0, ad.z);

	CPPUNIT_ASSERT_EQUAL(&ad, b3Vector::b3Init(&ad, -3.0, 2.0, -1.0));
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3Init(&as, &ad));
	CPPUNIT_ASSERT_EQUAL(-3.0f, as.x);
	CPPUNIT_ASSERT_EQUAL( 2.0f, as.y);
	CPPUNIT_ASSERT_EQUAL(-1.0f, as.z);

	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Init(&bs, &as));
	CPPUNIT_ASSERT(b3Vector::b3IsEqual(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(&bs, b3Vector::b3Negate(&bs));
	CPPUNIT_ASSERT_EQUAL( 3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(-2.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL( 1.0f, bs.z);

	CPPUNIT_ASSERT(!b3Vector::b3IsEqual(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&bd, b3Vector::b3Init(&bd, &ad));
	CPPUNIT_ASSERT(b3Vector::b3IsEqual(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&bd, b3Vector::b3Negate(&bd));
	CPPUNIT_ASSERT_EQUAL( 3.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(-2.0, bd.y);
	CPPUNIT_ASSERT_EQUAL( 1.0, bd.z);

	CPPUNIT_ASSERT_EQUAL(&bs,     b3Vector::b3Init(&bs, 2.0, -1.0, -2.0));
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
	CPPUNIT_ASSERT_EQUAL( 1.0,  b3Vector::b3Length(&bd));
	CPPUNIT_ASSERT_EQUAL( 1.0,  b3Vector::b3Normalize(&bd, 6.0));
	CPPUNIT_ASSERT_EQUAL( 6.0,  b3Vector::b3Length(&bd));

	CPPUNIT_ASSERT_EQUAL(&as,  b3Vector::b3Init(&as, 1.0, -3.0, -4.0));
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Init(&bs, 2.0, -1.0, -2.0));
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Add(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL(  3.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL( -4.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL( -6.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Sub(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL( -1.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL( -2.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL( -2.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Mul(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL(  2.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(  3.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(  8.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3CrossProduct(&as, &bs, &cs));
	CPPUNIT_ASSERT_EQUAL(  2.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL( -6.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(  5.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3CrossProduct(&as, &bs, &cd));
	CPPUNIT_ASSERT_EQUAL(  2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL( -6.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(  5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Add(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(  3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL( -4.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL( -6.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Sub(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(  2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL( -1.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL( -2.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Mul(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(  2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(  3.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(  8.0f, bs.z);

	CPPUNIT_ASSERT_EQUAL(&ad,  b3Vector::b3Init(&ad, 1.0, -3.0, -4.0));
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Init(&bd, 2.0, -1.0, -2.0));
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Add(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(  3.0, cd.x);
	CPPUNIT_ASSERT_EQUAL( -4.0, cd.y);
	CPPUNIT_ASSERT_EQUAL( -6.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Sub(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL( -1.0, cd.x);
	CPPUNIT_ASSERT_EQUAL( -2.0, cd.y);
	CPPUNIT_ASSERT_EQUAL( -2.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Mul(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(  2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(  3.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(  8.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3CrossProduct(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(  2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL( -6.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(  5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3CrossProduct(&ad, &bd, &cd));
	CPPUNIT_ASSERT_EQUAL(  2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL( -6.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(  5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Add(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(  3.0, bd.x);
	CPPUNIT_ASSERT_EQUAL( -4.0, bd.y);
	CPPUNIT_ASSERT_EQUAL( -6.0, bd.z);
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Sub(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(  2.0, bd.x);
	CPPUNIT_ASSERT_EQUAL( -1.0, bd.y);
	CPPUNIT_ASSERT_EQUAL( -2.0, bd.z);
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Mul(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(  2.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(  3.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(  8.0, bd.z);

	CPPUNIT_ASSERT_EQUAL(&as,  b3Vector::b3Init(&as, 1.0, -1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Init(&bs, 2.0,  1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(3.0f, b3Vector::b3Distance(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Scale(&as, &cs, 2.5));
	CPPUNIT_ASSERT_EQUAL(  2.5f, cs.x);
	CPPUNIT_ASSERT_EQUAL( -2.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL(  5.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Scale(&as, &cs, -2.0));
	CPPUNIT_ASSERT_EQUAL( -2.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(  2.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL( -4.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Scale(&cs, -2.0));
	CPPUNIT_ASSERT_EQUAL(  4.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL( -4.0f, cs.y);
	CPPUNIT_ASSERT_EQUAL(  8.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3LinearCombine(&as, &bs, 2.5, &cs));
	CPPUNIT_ASSERT_EQUAL(  6.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(  1.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL( 12.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&cg,  b3Vector::b3LinearCombine(&as, &bs, 2.5, &cg));
	CPPUNIT_ASSERT_EQUAL(  6.0f, cg.x);
	CPPUNIT_ASSERT_EQUAL(  1.5f, cg.y);
	CPPUNIT_ASSERT_EQUAL( 12.0f, cg.z);
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3LinearCombine(&as, &bs, 2.5, -2.0, &cs));
	CPPUNIT_ASSERT_EQUAL( -1.5f, cs.x);
	CPPUNIT_ASSERT_EQUAL( -4.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL( -3.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, &ds));
	CPPUNIT_ASSERT_EQUAL(  9.0f, ds.x);
	CPPUNIT_ASSERT_EQUAL( 10.5f, ds.y);
	CPPUNIT_ASSERT_EQUAL( 18.0f, ds.z);
	CPPUNIT_ASSERT_EQUAL(&dg,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, &dg));
	CPPUNIT_ASSERT_EQUAL(  9.0f, dg.x);
	CPPUNIT_ASSERT_EQUAL( 10.5f, dg.y);
	CPPUNIT_ASSERT_EQUAL( 18.0f, dg.z);
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, 2.0, &ds));
	CPPUNIT_ASSERT_EQUAL( -4.5f, ds.x);
	CPPUNIT_ASSERT_EQUAL(-13.5f, ds.y);
	CPPUNIT_ASSERT_EQUAL( -9.0f, ds.z);

	CPPUNIT_ASSERT_EQUAL(&ad,  b3Vector::b3Init(&ad, 1.0, -1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(&bd,  b3Vector::b3Init(&bd, 2.0,  1.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(3.0,  b3Vector::b3Distance(&ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Scale(&ad, &cd, 2.5));
	CPPUNIT_ASSERT_EQUAL(  2.5, cd.x);
	CPPUNIT_ASSERT_EQUAL( -2.5, cd.y);
	CPPUNIT_ASSERT_EQUAL(  5.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Scale(&ad, &cd, -2.0));
	CPPUNIT_ASSERT_EQUAL( -2.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(  2.0, cd.y);
	CPPUNIT_ASSERT_EQUAL( -4.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3Scale(&cd, -2.0));
	CPPUNIT_ASSERT_EQUAL(  4.0, cd.x);
	CPPUNIT_ASSERT_EQUAL( -4.0, cd.y);
	CPPUNIT_ASSERT_EQUAL(  8.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3LinearCombine(&ad, &bd, 2.5, &cd));
	CPPUNIT_ASSERT_EQUAL(  6.0, cd.x);
	CPPUNIT_ASSERT_EQUAL(  1.5, cd.y);
	CPPUNIT_ASSERT_EQUAL( 12.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3LinearCombine(&ad, &bd, 2.5, -2.0, &cd));
	CPPUNIT_ASSERT_EQUAL( -1.5, cd.x);
	CPPUNIT_ASSERT_EQUAL( -4.5, cd.y);
	CPPUNIT_ASSERT_EQUAL( -3.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&cd,  b3Vector::b3LinearCombine(&as, &bs, 2.5, -2.0, &cd));
	CPPUNIT_ASSERT_EQUAL( -1.5, cd.x);
	CPPUNIT_ASSERT_EQUAL( -4.5, cd.y);
	CPPUNIT_ASSERT_EQUAL( -3.0, cd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&ad, &bd, &cd, 2.5, -2.0, &dd));
	CPPUNIT_ASSERT_EQUAL(  9.0, dd.x);
	CPPUNIT_ASSERT_EQUAL( 10.5, dd.y);
	CPPUNIT_ASSERT_EQUAL( 18.0, dd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, &dd));
	CPPUNIT_ASSERT_EQUAL(  9.0, dd.x);
	CPPUNIT_ASSERT_EQUAL( 10.5, dd.y);
	CPPUNIT_ASSERT_EQUAL( 18.0, dd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&as, &bs, &cs, 2.5, -2.0, 2.0, &dd));
	CPPUNIT_ASSERT_EQUAL( -4.5, dd.x);
	CPPUNIT_ASSERT_EQUAL(-13.5, dd.y);
	CPPUNIT_ASSERT_EQUAL( -9.0, dd.z);

	CPPUNIT_ASSERT_EQUAL(&ld,  b3Vector::b3Init(&ld, &ad, &bd));
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3LinearCombine(&ld, 2.5, &cs, &ds));
	CPPUNIT_ASSERT_EQUAL(  7.5f, ds.x);
	CPPUNIT_ASSERT_EQUAL(  6.0f, ds.y);
	CPPUNIT_ASSERT_EQUAL( 15.0f, ds.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&ld, 2.5, &dd));
	CPPUNIT_ASSERT_EQUAL(  6.0,  dd.x);
	CPPUNIT_ASSERT_EQUAL(  1.5,  dd.y);
	CPPUNIT_ASSERT_EQUAL( 12.0,  dd.z);
	CPPUNIT_ASSERT_EQUAL(&dd,  b3Vector::b3LinearCombine(&ld, 2.5, &dd));

	CPPUNIT_ASSERT_EQUAL(&as,  b3Vector::b3Init(&as, 1.0,  1.5, 4.0));
	CPPUNIT_ASSERT_EQUAL(&bs,  b3Vector::b3Init(&bs, 2.0, -1.0, 2.0));
	CPPUNIT_ASSERT_EQUAL(&cs,  b3Vector::b3Init(&cs, &as));
	CPPUNIT_ASSERT_EQUAL(&ds,  b3Vector::b3Init(&ds, &bs));
	b3Vector::b3Sort(&as, &bs);
	CPPUNIT_ASSERT_EQUAL(  1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL( -1.0f, as.y);
	CPPUNIT_ASSERT_EQUAL(  2.0f, as.z);
	CPPUNIT_ASSERT_EQUAL(  2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(  1.5f, bs.y);
	CPPUNIT_ASSERT_EQUAL(  4.0f, bs.z);	
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3SetMinimum(&as, 0.5));
	CPPUNIT_ASSERT_EQUAL(  1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(  0.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(  2.0f, as.z);
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3CheckLowerBound(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(  1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(  0.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(  2.0f, as.z);
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3SetMaximum(&as, 1.5));
	CPPUNIT_ASSERT_EQUAL(  1.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(  0.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(  1.5f, as.z);
	CPPUNIT_ASSERT_EQUAL(&as, b3Vector::b3CheckUpperBound(&as, &bs));
	CPPUNIT_ASSERT_EQUAL(  2.0f, as.x);
	CPPUNIT_ASSERT_EQUAL(  1.5f, as.y);
	CPPUNIT_ASSERT_EQUAL(  4.0f, as.z);
	b3Vector::b3AdjustBound(&as, &cs, &ds);
	CPPUNIT_ASSERT_EQUAL(  1.0f, cs.x);
	CPPUNIT_ASSERT_EQUAL(  1.5f, cs.y);
	CPPUNIT_ASSERT_EQUAL(  4.0f, cs.z);
	CPPUNIT_ASSERT_EQUAL(  2.0f, ds.x);
	CPPUNIT_ASSERT_EQUAL(  1.5f, ds.y);
	CPPUNIT_ASSERT_EQUAL(  4.0f, ds.z);

	CPPUNIT_ASSERT_EQUAL( &am, b3Matrix::b3Move(null, &am, 1.0, 2.0, 3.0));
	CPPUNIT_ASSERT_EQUAL( &bs, b3Vector::b3Init(&bs, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL( &bs, b3Vector::b3MatrixMul3D(&am, &bs));
	CPPUNIT_ASSERT_EQUAL(  2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL( -2.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(  1.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL( &bs, b3Vector::b3Init(&bs, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL( &bs, b3Vector::b3MatrixMul4D(&am, &bs));
	CPPUNIT_ASSERT_EQUAL(  3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(  0.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(  4.0f, bs.z);
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
	CPPUNIT_ASSERT_EQUAL(static_cast<b3_matrix *>(null), b3Matrix::b3Inverse(&am, &cm));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Transport(&am, &dm));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Move(null, &dm, &as));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Move(null, &dm, 1,2,3));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MoveNegative(null, &dm, &bs));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Scale(null, &dm, &as, &bs));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Scale(null, &dm, &as, 2.0, 3.0, 4.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateX(null, &dm, &as, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateY(null, &dm, &as, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateZ(null, &dm, &as, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3RotateVector(null, &dm, &ls, 0.5 * M_PI));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPoint(null, &dm, &as, 1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPoint(null, &dm, &as, -1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorAxis(null, &dm, &ls, 1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorAxis(null, &dm, &ls, -1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPlane(null, &dm, &as, &bs, &cs, 1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MirrorPlane(null, &dm, &as, &bs, &cs, -1.0));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Align(&dm, &ls));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Dress(&cm, &dm, &as, &bs, &cs, false));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3Dress(&cm, &dm, &as, &bs, &cs, true));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MMul(&am, &bm, &dm));
	CPPUNIT_ASSERT_EQUAL(&dm, b3Matrix::b3MAdd(&am, &bm, &dm));

	CPPUNIT_ASSERT_EQUAL( &am, b3Matrix::b3Move(null, &am, 1.0, 2.0, 3.0));
	CPPUNIT_ASSERT_EQUAL( &as, b3Vector::b3Init(&as, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL( &bs, b3Matrix::b3VMul(&am, &as, &bs, false));
	CPPUNIT_ASSERT_EQUAL(  2.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL( -2.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(  1.0f, bs.z);
	CPPUNIT_ASSERT_EQUAL( &bs, b3Matrix::b3VMul(&am, &as, &bs, true));
	CPPUNIT_ASSERT_EQUAL(  3.0f, bs.x);
	CPPUNIT_ASSERT_EQUAL(  0.0f, bs.y);
	CPPUNIT_ASSERT_EQUAL(  4.0f, bs.z);

	CPPUNIT_ASSERT_EQUAL( &ad, b3Vector::b3Init(&ad, 2.0, -2.0, 1.0));
	CPPUNIT_ASSERT_EQUAL( &bd, b3Matrix::b3VMul(&am, &ad, &bd, false));
	CPPUNIT_ASSERT_EQUAL(  2.0, bd.x);
	CPPUNIT_ASSERT_EQUAL( -2.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(  1.0, bd.z);
	CPPUNIT_ASSERT_EQUAL( &bd, b3Matrix::b3VMul(&am, &ad, &bd, true));
	CPPUNIT_ASSERT_EQUAL(  3.0, bd.x);
	CPPUNIT_ASSERT_EQUAL(  0.0, bd.y);
	CPPUNIT_ASSERT_EQUAL(  4.0, bd.z);
}

#endif
