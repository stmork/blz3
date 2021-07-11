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

#include "blz3/system/b3Date.h"

class b3DateTest : public CppUnit::TestFixture
{
	const b3Date::b3_day   TEST_DAY   =  28;
	const b3Date::b3_month TEST_MONTH = b3Date::B3_JUNE;
	const b3Date::b3_year  TEST_YEAR  = 1964;

	const unsigned         TEST_HOUR  = 12;
	const unsigned         TEST_MIN   = 34;
	const unsigned         TEST_SEC   = 56;

	CPPUNIT_TEST_SUITE(b3DateTest);
	CPPUNIT_TEST(testNow);
	CPPUNIT_TEST(testDate);
	CPPUNIT_TEST(testEqual);
	CPPUNIT_TEST(testUtc);
	CPPUNIT_TEST(testTimeValSpec);
	CPPUNIT_TEST(testDateTime);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;

	void testNow();
	void testDate();
	void testEqual();
	void testUtc();
	void testTimeValSpec();
	void testDateTime();
};

#endif
#endif
