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

#include "blz3/base/b3Render.h"

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

CPPUNIT_TEST_SUITE_REGISTRATION(b3VectorOpenGlTest);

#ifdef BLZ3_USE_OPENGL

void b3VectorOpenGlTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3VectorOpenGlTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3VectorOpenGlTest::test()
{
	b3_vector32_3D src_3d;
	b3_vector32_4D src_4d;
	GLfloat        dst_float[4];

	src_3d.x =  0.25;
	src_3d.y =  2.0;
	src_3d.z = -3.0;
	b3RenderContext::b3VectorToGL(&src_3d, dst_float);
	CPPUNIT_ASSERT_EQUAL( 0.25f, dst_float[0]);
	CPPUNIT_ASSERT_EQUAL( 2.0f,  dst_float[1]);
	CPPUNIT_ASSERT_EQUAL(-3.0f,  dst_float[2]);
	CPPUNIT_ASSERT_EQUAL( 1.0f,  dst_float[3]);

	src_4d.x =  0.25;
	src_4d.y =  2.0;
	src_4d.z = -3.0;
	src_4d.w = -0.75;
	b3RenderContext::b3VectorToGL(&src_4d, dst_float);
	CPPUNIT_ASSERT_EQUAL( 0.25f, dst_float[0]);
	CPPUNIT_ASSERT_EQUAL( 2.0f,  dst_float[1]);
	CPPUNIT_ASSERT_EQUAL(-3.0f,  dst_float[2]);
	CPPUNIT_ASSERT_EQUAL(-0.75f, dst_float[3]);

	const b3_f32 len = sqrt(
			src_3d.x * src_3d.x +
			src_3d.y * src_3d.y +
			src_3d.z * src_3d.z);
	b3RenderContext::b3VectorToDirectionalGL(&src_3d, dst_float);
	CPPUNIT_ASSERT_EQUAL( 0.25f / len, dst_float[0]);
	CPPUNIT_ASSERT_EQUAL( 2.0f  / len, dst_float[1]);
	CPPUNIT_ASSERT_EQUAL(-3.0f  / len, dst_float[2]);
	CPPUNIT_ASSERT_EQUAL( 0.0f,        dst_float[3]);
}

#endif

#endif
