/*
**
**	$Filename:	b3DataSizeTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit test for basic data types.
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

#include "b3DataSizeTest.h"
#include "b3TestMacros.h"

#include "blz3/base/b3Color.h"
#include "blz3/base/b3Vector.h"

#include <cstddef>

/*************************************************************************
**                                                                      **
**                        Unit test for basic data types                **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3DataSizeTest);

void b3DataSizeTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3DataSizeTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

#pragma pack(push, 1)
struct align_1
{
	alignas(1) b3_u08   a;
	alignas(1) b3_u32   b;
	alignas(1) b3_u16   c;
	alignas(1) b3_u32   d;
};

#pragma pack(2)
struct align_2
{
	alignas(2) b3_u08   a;
	alignas(2) b3_u32   b;
	alignas(2) b3_u16   c;
	alignas(2) b3_u32   d;
};
#pragma pack(pop)

struct align_4
{
	b3_u08   a;
	b3_u32   b;
	b3_u16   c;
	b3_u32   d;
};

void b3DataSizeTest::testAlignment()
{
	CPPUNIT_ASSERT_SIZET_EQUAL(11, sizeof(align_1));
	CPPUNIT_ASSERT_SIZET_EQUAL(12, sizeof(align_2));
	CPPUNIT_ASSERT_SIZET_EQUAL(16, sizeof(align_4));

	CPPUNIT_ASSERT_TYPED_EQUAL(off_t,  1, offsetof(align_1, b));
	CPPUNIT_ASSERT_TYPED_EQUAL(off_t,  7, offsetof(align_1, d));
	CPPUNIT_ASSERT_TYPED_EQUAL(off_t,  2, offsetof(align_2, b));
	CPPUNIT_ASSERT_TYPED_EQUAL(off_t,  8, offsetof(align_2, d));
	CPPUNIT_ASSERT_TYPED_EQUAL(off_t,  4, offsetof(align_4, b));
	CPPUNIT_ASSERT_TYPED_EQUAL(off_t, 12, offsetof(align_4, d));
}

void b3DataSizeTest::testDataSize()
{
	void * ptr = nullptr;

	CPPUNIT_ASSERT_SIZET_EQUAL(1, sizeof(b3_u08));
	CPPUNIT_ASSERT_SIZET_EQUAL(2, sizeof(b3_u16));
	CPPUNIT_ASSERT_SIZET_EQUAL(4, sizeof(b3_u32));
	CPPUNIT_ASSERT_SIZET_EQUAL(8, sizeof(b3_u64));

	CPPUNIT_ASSERT_SIZET_EQUAL(1, sizeof(b3_s08));
	CPPUNIT_ASSERT_SIZET_EQUAL(2, sizeof(b3_s16));
	CPPUNIT_ASSERT_SIZET_EQUAL(4, sizeof(b3_s32));
	CPPUNIT_ASSERT_SIZET_EQUAL(8, sizeof(b3_s64));

	CPPUNIT_ASSERT_SIZET_EQUAL(4, sizeof(b3_bool));
	CPPUNIT_ASSERT_SIZET_EQUAL(4, sizeof(b3_f32));
	CPPUNIT_ASSERT_SIZET_EQUAL(8, sizeof(b3_f64));
	CPPUNIT_ASSERT(sizeof(b3_f96) >= 10);

	CPPUNIT_ASSERT_SIZET_EQUAL(16, sizeof(b3Color));
	CPPUNIT_ASSERT_SIZET_EQUAL(16, sizeof(b3_color));
	CPPUNIT_ASSERT_SIZET_EQUAL(16, sizeof(b3_vector32_3D));
	CPPUNIT_ASSERT_SIZET_EQUAL(16, sizeof(b3_vector32_4D));
	CPPUNIT_ASSERT_SIZET_EQUAL(32, sizeof(b3_vector64_3D));
	CPPUNIT_ASSERT_SIZET_EQUAL(32, sizeof(b3_vector64_4D));
	CPPUNIT_ASSERT_SIZET_EQUAL(16, sizeof(b3Vector32));
	CPPUNIT_ASSERT_SIZET_EQUAL(32, sizeof(b3Vector64));

	CPPUNIT_ASSERT_EQUAL(sizeof(b3_ptr), sizeof(ptr));
}

void b3DataSizeTest::testSwap()
{
	b3_u32 v1, v2;

	v1 = 1;
	v2 = 2;
	std::swap(v1, v2);
	CPPUNIT_ASSERT_U32_EQUAL(2, v1);
	CPPUNIT_ASSERT_U32_EQUAL(1, v2);

	v1 = 1;
	v2 = 2;
	std::swap(v2, v1);
	CPPUNIT_ASSERT_U32_EQUAL(2, v1);
	CPPUNIT_ASSERT_U32_EQUAL(1, v2);
}

#endif
