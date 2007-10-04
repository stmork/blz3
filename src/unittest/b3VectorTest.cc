/*
**
**	$Filename:	b3VectorTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for vector class templates.
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

#include "b3VectorTest.h"

/*************************************************************************
**                                                                      **
**                        Unit test for vector class templates          **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

typedef b3VectorTest<b3_f32, 3> vector_test_f32_3;
typedef b3VectorTest<b3_f32, 4> vector_test_f32_4;
typedef b3VectorTest<b3_f64, 3> vector_test_f64_3;
typedef b3VectorTest<b3_f64, 4> vector_test_f64_4;

CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f32_3);
CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f32_4);
CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f64_3);
CPPUNIT_TEST_SUITE_REGISTRATION(vector_test_f64_4);

#endif
