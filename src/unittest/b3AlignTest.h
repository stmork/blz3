/*
**
**	$Filename:	b3ListTest.h $
**	$Release:	Dortmund 2006, 2016 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for doubly linked lists.
**
**      (C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#ifndef B3_ALIGN_TEST_H
#define B3_ALIGN_TEST_H

#include <blz3/b3Config.h>

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3AlignTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3AlignTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

	b3_vector    * ptr_vector_3d = nullptr;
	b3_vector4D  * ptr_vector_4d = nullptr;
	b3_matrix    * ptr_matrix_32 = nullptr;
	b3_matrix64  * ptr_matrix_64 = nullptr;
	b3_color     * ptr_color = nullptr;

public:
	void setUp() override;
	void tearDown() override;
	void test();
};

#endif
#endif // B3_ALIGN_TEST_H
