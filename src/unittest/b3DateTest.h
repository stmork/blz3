/*
**
**	$Filename:	b3DateTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Skeleton for unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_DATE_TEST_H
#define B3_DATE_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3DateTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3DateTest);
	CPPUNIT_TEST(testNow);
	CPPUNIT_TEST(testDate);
	CPPUNIT_TEST(testEqual);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testNow();
	void testDate();
	void testEqual();
};

#endif
#endif
