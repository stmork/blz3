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

class b3DataSizeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3DataSizeTest);
	CPPUNIT_TEST(testAlignment);
	CPPUNIT_TEST(testDataSize);
	CPPUNIT_TEST(testSwap);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;

	void testAlignment();
	void testDataSize();
	void testSwap();
};

#endif
#endif
