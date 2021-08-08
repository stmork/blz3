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
	const b3Date::b3_day   SUMMER_TEST_DAY   =   28;
	const b3Date::b3_month SUMMER_TEST_MONTH = b3Date::B3_JUNE;
	const b3Date::b3_year  SUMMER_TEST_YEAR  = 2001;

	const b3Date::b3_day   WINTER_TEST_DAY   =    8;
	const b3Date::b3_month WINTER_TEST_MONTH = b3Date::B3_JANUARY;
	const b3Date::b3_year  WINTER_TEST_YEAR  = 2001;

	const unsigned         TEST_HOUR  = 12;
	const unsigned         TEST_MIN   = 34;
	const unsigned         TEST_SEC   = 56;

	// We start at 01.01.1970 at 00:00:00
	const std::time_t DATE_000000_09091999 = (29 * b3Date::TICKS_YEAR + b3Date::TICKS_DAY * 258);
	const std::time_t DATE_000000_10091999 = (DATE_000000_09091999 +    b3Date::TICKS_DAY *   1);
	const std::time_t DATE_000000_31121999 = (30 * b3Date::TICKS_YEAR + b3Date::TICKS_DAY *   6);
	const std::time_t DATE_000000_01012000 = (DATE_000000_31121999 +    b3Date::TICKS_DAY);
	const std::time_t DATE_000000_29022000 = (DATE_000000_31121999 +    b3Date::TICKS_DAY *  60);
	const std::time_t DATE_120000_01042001 = (DATE_000000_01012000 +    b3Date::TICKS_DAY * 456 + 12 * b3Date::TICKS_HOUR);
	const std::time_t DATE_120000_08042001 = (DATE_120000_01042001 +    b3Date::TICKS_DAY *   7);
	const std::time_t DATE_120000_19122018 = (48 * b3Date::TICKS_YEAR + b3Date::TICKS_DAY * (365 - 13 + 13 - 1) + 12 * b3Date::TICKS_HOUR);
	const std::time_t DATE_120000_23032021 = (51 * b3Date::TICKS_YEAR + b3Date::TICKS_DAY * (31 + 28 + 23 + 13 - 1) + 12 * b3Date::TICKS_HOUR);

	CPPUNIT_TEST_SUITE(b3DateTest);
	CPPUNIT_TEST(testNow);
	CPPUNIT_TEST(testWinterDate);
	CPPUNIT_TEST(testSummerDate);
	CPPUNIT_TEST(testEqual);
	CPPUNIT_TEST(testUtc);
	CPPUNIT_TEST(testTimeValSpec);
	CPPUNIT_TEST(testWinterDateTime);
	CPPUNIT_TEST(testSummerDateTime);
	CPPUNIT_TEST(testDates);
	CPPUNIT_TEST(testDst);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;

	void testNow();
	void testWinterDate();
	void testSummerDate();
	void testEqual();
	void testUtc();
	void testTimeValSpec();
	void testWinterDateTime();
	void testSummerDateTime();
	void testDates();
	void testDst();

private:
	void b3Check(const b3Date & date,
		const unsigned h,
		const unsigned m,
		const unsigned s,
		const b3Date::b3_day day,
		const b3Date::b3_month,
		const b3Date::b3_year year,
		const b3Date::b3_week_day w,
		const bool dls,
		const signed offset);
};

#endif
#endif
