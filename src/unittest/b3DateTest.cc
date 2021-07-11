/*
**
**	$Filename:	b3DateTest.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3DateTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3DateTest);

const b3Date::b3_day   b3DateTest::TEST_DAY   =   28;
const b3Date::b3_month b3DateTest::TEST_MONTH = b3Date::B3_JUNE;
const b3Date::b3_year  b3DateTest::TEST_YEAR  = 1964;

void b3DateTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3DateTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3DateTest::testNow()
{
	b3Date now;
}

void b3DateTest::testDate()
{
	b3Date date(TEST_DAY, TEST_MONTH, TEST_YEAR);

	CPPUNIT_ASSERT_EQUAL(TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(TEST_YEAR,  date.year);
	CPPUNIT_ASSERT(date.dls);
}

void b3DateTest::testEqual()
{
	b3Date date(TEST_DAY, TEST_MONTH, TEST_YEAR);
	b3Date copy(date);
	b3Date now;

	CPPUNIT_ASSERT_LESSEQUAL(date, copy);
	CPPUNIT_ASSERT_EQUAL(date, copy);
	CPPUNIT_ASSERT_GREATEREQUAL(date, copy);

	CPPUNIT_ASSERT_EQUAL(date.day,      copy.day);
	CPPUNIT_ASSERT_EQUAL(date.month,    copy.month);
	CPPUNIT_ASSERT_EQUAL(date.year,     copy.year);
	CPPUNIT_ASSERT_EQUAL(date.hour,     copy.hour);
	CPPUNIT_ASSERT_EQUAL(date.min,      copy.min);
	CPPUNIT_ASSERT_EQUAL(date.sec,      copy.sec);
	CPPUNIT_ASSERT_EQUAL(date.microsec, copy.microsec);
	CPPUNIT_ASSERT_EQUAL(date.dls,      copy.dls);
	CPPUNIT_ASSERT_EQUAL(date.offset,   copy.offset);

	CPPUNIT_ASSERT_TYPED_EQUAL(std::time_t, date, copy);
	CPPUNIT_ASSERT_EQUAL(date.b3GetMode(), copy.b3GetMode());

	CPPUNIT_ASSERT_LESSEQUAL(now, date);
	CPPUNIT_ASSERT_LESS(now, date);
	CPPUNIT_ASSERT(now > date);
	CPPUNIT_ASSERT(date < now);
	CPPUNIT_ASSERT_GREATER(date, now);
	CPPUNIT_ASSERT_GREATEREQUAL(date, now);
}

void b3DateTest::testUtc()
{
	b3Date date(TEST_DAY, TEST_MONTH, TEST_YEAR);

	CPPUNIT_ASSERT(date.dls);
	CPPUNIT_ASSERT_EQUAL(TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(TEST_YEAR,  date.year);
	CPPUNIT_ASSERT_EQUAL(b3_daytime::B3_DT_LOCAL, date.b3GetMode());

	date.b3SetMode(b3_daytime::B3_DT_GM);
	CPPUNIT_ASSERT(!date.dls);
	CPPUNIT_ASSERT_EQUAL(TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(TEST_YEAR,  date.year);
	CPPUNIT_ASSERT_EQUAL(0,          date.offset);
	CPPUNIT_ASSERT_EQUAL(b3_daytime::B3_DT_GM, date.b3GetMode());
}

#endif
