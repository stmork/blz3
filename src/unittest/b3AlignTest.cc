/*
**
**	$Filename:	b3SkeletonTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Skeleton for unit tests.
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

#include "b3AlignTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

#include <cstdalign>

CPPUNIT_TEST_SUITE_REGISTRATION(b3AlignTest);

void b3AlignTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	ptr_color      = new b3_color;
	ptr_complex_64 = new b3Complex64;
	ptr_vector_3d  = new b3_vector;
	ptr_vector_4d  = new b3_vector4D;
	ptr_matrix_32  = new b3_matrix;
	ptr_matrix_64  = new b3_matrix64;
}

void b3AlignTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	delete ptr_color;
	delete ptr_complex_64;
	delete ptr_vector_3d;
	delete ptr_vector_4d;
	delete ptr_matrix_32;
	delete ptr_matrix_64;
}

void b3AlignTest::test()
{
	char         gap1 B3_UNUSED;
	b3_vector    local_vector_3d;
	b3_vector4D  local_vector_4d;
	char         gap2 B3_UNUSED;
	b3_matrix    local_matrix_32;
	b3_matrix64  local_matrix_64;
	char         gap3 B3_UNUSED;
	b3_color     local_color;
	b3Complex64  local_complex_64;

	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(local_vector_3d));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(local_vector_4d));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(local_matrix_32));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(local_matrix_64));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(local_color));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(local_complex_64));

	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(*ptr_vector_3d));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(*ptr_vector_4d));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(*ptr_matrix_32));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(*ptr_matrix_64));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(*ptr_color));
	CPPUNIT_ASSERT_TYPED_EQUAL(size_t, 16, alignof(*ptr_complex_64));
}

#endif
