/*
**
**	$Filename:	b3DataSizeTest.h $
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

#ifndef B3_DATASIZE_TEST_H
#define B3_DATASIZE_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/system/b3Memory.h"

using namespace std;

#define MEM_MIN         1000
#define MEM_LOW_MULT       2
#define MEM_HIGH_MULT    200

class b3DataSizeTest : public CppUnit::TestFixture
{
	b3Mem     mem;
	b3_u08    buffer[MEM_MIN];
	void   *  ptr1, *ptr2;

	CPPUNIT_TEST_SUITE(b3DataSizeTest);
	CPPUNIT_TEST(testDataSize);
	CPPUNIT_TEST(testMemory);
	CPPUNIT_TEST(testSwap);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testDataSize();
	void testMemory();
	void testSwap();
};

#endif
#endif
