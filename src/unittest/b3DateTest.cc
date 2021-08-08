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
	CPPUNIT_ASSERT_NO_THROW(b3Date());
}

void b3DateTest::testWinterDate()
{
	b3Date date(WINTER_TEST_DAY, WINTER_TEST_MONTH, WINTER_TEST_YEAR);

	CPPUNIT_ASSERT_EQUAL(WINTER_TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(WINTER_TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(WINTER_TEST_YEAR,  date.year);
	CPPUNIT_ASSERT(!date.dst);
}

void b3DateTest::testSummerDate()
{
	b3Date date(SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR);

	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_YEAR,  date.year);
	CPPUNIT_ASSERT(date.dst);
}

void b3DateTest::testEqual()
{
	b3Date date(SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR);
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
	CPPUNIT_ASSERT_EQUAL(date.dst,      copy.dst);
	CPPUNIT_ASSERT_EQUAL(date.offset,   copy.offset);

	CPPUNIT_ASSERT_TYPED_EQUAL(std::time_t, date, copy);
	CPPUNIT_ASSERT_EQUAL(date.b3GetMode(), copy.b3GetMode());

	CPPUNIT_ASSERT_LESSEQUAL(now, date);
	CPPUNIT_ASSERT_LESS(now, date);
	CPPUNIT_ASSERT(now > date);
	CPPUNIT_ASSERT(now != date);
	CPPUNIT_ASSERT(date != now);
	CPPUNIT_ASSERT(date < now);
	CPPUNIT_ASSERT_GREATER(date, now);
	CPPUNIT_ASSERT_GREATEREQUAL(date, now);
}

void b3DateTest::testUtc()
{
	b3Date date(SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR);
	const std::time_t before = date;

	CPPUNIT_ASSERT(date.dst);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_YEAR,  date.year);
	CPPUNIT_ASSERT_EQUAL(b3_daytime::B3_DT_LOCAL, date.b3GetMode());

	date.b3SetMode(b3_daytime::B3_DT_GM);
	const std::time_t after = date;

	CPPUNIT_ASSERT(!date.dst);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_YEAR,  date.year);
	CPPUNIT_ASSERT_EQUAL(0,          date.offset);
	CPPUNIT_ASSERT_EQUAL(b3_daytime::B3_DT_GM, date.b3GetMode());
	CPPUNIT_ASSERT_EQUAL(before,     after);
}

void b3DateTest::testTimeValSpec()
{
	const b3Date   now;
	const timeval  tv = now;
	const timespec ts = now;

	CPPUNIT_ASSERT_TYPED_EQUAL(std::time_t, now,          tv.tv_sec);
	CPPUNIT_ASSERT_TYPED_EQUAL(unsigned,    now.microsec, tv.tv_usec);

	CPPUNIT_ASSERT_TYPED_EQUAL(std::time_t, now,                 ts.tv_sec);
	CPPUNIT_ASSERT_TYPED_EQUAL(unsigned,    now.microsec * 1000, ts.tv_nsec);
}

void b3DateTest::testWinterDateTime()
{
	b3Date date;
	b3Date time;
	b3Date date_time;

	CPPUNIT_ASSERT_NO_THROW(date.b3SetDate(SUMMER_TEST_DAY, WINTER_TEST_MONTH, SUMMER_TEST_YEAR));
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(WINTER_TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_YEAR,  date.year);
	CPPUNIT_ASSERT(!date.dst);

	CPPUNIT_ASSERT_NO_THROW(!time.b3SetTime(24, TEST_MIN, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(time.b3SetTime(TEST_HOUR, 60, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(time.b3SetTime(TEST_HOUR, TEST_MIN, 60));
	CPPUNIT_ASSERT_NO_THROW(time.b3SetTime(TEST_HOUR, TEST_MIN, TEST_SEC));

	CPPUNIT_ASSERT_EQUAL(TEST_HOUR, time.hour);
	CPPUNIT_ASSERT_EQUAL(TEST_MIN,  time.min);
	CPPUNIT_ASSERT_EQUAL(TEST_SEC,  time.sec);
	CPPUNIT_ASSERT_EQUAL(0u,        time.microsec);

	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, WINTER_TEST_MONTH, SUMMER_TEST_YEAR,
			24, TEST_MIN, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, WINTER_TEST_MONTH, SUMMER_TEST_YEAR,
			TEST_HOUR, 60, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, WINTER_TEST_MONTH, SUMMER_TEST_YEAR,
			TEST_HOUR, TEST_MIN, 60));
	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, WINTER_TEST_MONTH, SUMMER_TEST_YEAR,
			TEST_HOUR, TEST_MIN,   TEST_SEC));
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_DAY,   date_time.day);
	CPPUNIT_ASSERT_EQUAL(WINTER_TEST_MONTH, date_time.month);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_YEAR,  date_time.year);
	CPPUNIT_ASSERT_EQUAL(TEST_HOUR,  date_time.hour);
	CPPUNIT_ASSERT_EQUAL(TEST_MIN,   date_time.min);
	CPPUNIT_ASSERT_EQUAL(TEST_SEC,   date_time.sec);
	CPPUNIT_ASSERT_EQUAL(0u,         date_time.microsec);
	CPPUNIT_ASSERT(!date_time.dst);
}

void b3DateTest::testSummerDateTime()
{
	b3Date date;
	b3Date time;
	b3Date date_time;

	CPPUNIT_ASSERT_NO_THROW(date.b3SetDate(SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR));
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_DAY,   date.day);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_MONTH, date.month);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_YEAR,  date.year);
	CPPUNIT_ASSERT(date.dst);

	CPPUNIT_ASSERT_NO_THROW(!time.b3SetTime(24, TEST_MIN, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(time.b3SetTime(TEST_HOUR, 60, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(time.b3SetTime(TEST_HOUR, TEST_MIN, 60));
	CPPUNIT_ASSERT_NO_THROW(time.b3SetTime(TEST_HOUR, TEST_MIN, TEST_SEC));

	CPPUNIT_ASSERT_EQUAL(TEST_HOUR, time.hour);
	CPPUNIT_ASSERT_EQUAL(TEST_MIN,  time.min);
	CPPUNIT_ASSERT_EQUAL(TEST_SEC,  time.sec);
	CPPUNIT_ASSERT_EQUAL(0u,        time.microsec);

	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR,
			24, TEST_MIN, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR,
			TEST_HOUR, 60, TEST_SEC));
	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR,
			TEST_HOUR, TEST_MIN, 60));
	CPPUNIT_ASSERT_NO_THROW(date_time.b3SetDateTime(
			SUMMER_TEST_DAY, SUMMER_TEST_MONTH, SUMMER_TEST_YEAR,
			TEST_HOUR, TEST_MIN,   TEST_SEC));
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_DAY,   date_time.day);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_MONTH, date_time.month);
	CPPUNIT_ASSERT_EQUAL(SUMMER_TEST_YEAR,  date_time.year);
	CPPUNIT_ASSERT_EQUAL(TEST_HOUR,  date_time.hour);
	CPPUNIT_ASSERT_EQUAL(TEST_MIN,   date_time.min);
	CPPUNIT_ASSERT_EQUAL(TEST_SEC,   date_time.sec);
	CPPUNIT_ASSERT_EQUAL(0u,         date_time.microsec);
	CPPUNIT_ASSERT(date_time.dst);
}

void b3DateTest::testDates()
{
	b3Date      date;
	std::time_t diff = 0;

	date = 12 * b3Date::TICKS_HOUR;
	diff = date.offset * b3Date::TICKS_MIN;

	date = DATE_000000_09091999 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 0, 0, 0, 9, b3Date::B3_SEPTEMBER, 1999, b3Date::B3_THURSDAY, true, 120);

	date = DATE_000000_10091999 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 0, 0, 0, 10, b3Date::B3_SEPTEMBER, 1999, b3Date::B3_FRIDAY, true, 120);

	date = DATE_000000_31121999 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 0, 0, 0, 31, b3Date::B3_DECEMBER, 1999, b3Date::B3_FRIDAY, false,  60);

	date = DATE_000000_01012000 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 0, 0, 0, 1, b3Date::B3_JANUARY, 2000, b3Date::B3_SATURDAY, false,  60);

	date = DATE_000000_29022000 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 0, 0, 0, 29, b3Date::B3_FEBRUARY, 2000, b3Date::B3_TUESDAY, false,  60);

	date = DATE_120000_01042001 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 12, 0, 0, 1, b3Date::B3_APRIL, 2001, b3Date::B3_SUNDAY, true, 120);

	date = DATE_120000_08042001 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 12, 0, 0, 8, b3Date::B3_APRIL, 2001, b3Date::B3_SUNDAY, true, 120);

	date = DATE_120000_19122018 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 12, 0, 0, 19, b3Date::B3_DECEMBER, 2018, b3Date::B3_WEDNESDAY, false,  60);

	date = DATE_120000_23032021 - diff;
	date.b3SetMode(B3_DT_LOCAL);
	b3Check(date, 12, 0, 0, 23, b3Date::B3_MARCH, 2021, b3Date::B3_TUESDAY, false, 60);
}

void b3DateTest::testDst()
{
	b3Date date;

	CPPUNIT_ASSERT(date.b3SetDateTime(17, b3Date::B3_JULY, 2020, 10, 41, 39));

	date.b3SetMode(B3_DT_LOCAL);
	CPPUNIT_ASSERT(date.dst);
}

void b3DateTest::b3Check(
	const b3Date       &      date,
	const unsigned            h,
	const unsigned            m,
	const unsigned            s,
	const b3Date::b3_day      day,
	const b3Date::b3_month    month,
	const b3Date::b3_year     year,
	const b3Date::b3_week_day w,
	const bool                dls,
	const signed              offset)
{
	CPPUNIT_ASSERT_EQUAL(0u,                date.microsec);
	CPPUNIT_ASSERT_EQUAL(s,                 date.sec);
	CPPUNIT_ASSERT_EQUAL(m,                 date.min);
	CPPUNIT_ASSERT_EQUAL(h + (dls ? 1 : 0), date.hour);
	CPPUNIT_ASSERT_EQUAL(day,               date.day);
	CPPUNIT_ASSERT_EQUAL(month,             date.month);
	CPPUNIT_ASSERT_EQUAL(year,              date.year);
	CPPUNIT_ASSERT_EQUAL(w,                 date.wday);
	CPPUNIT_ASSERT_EQUAL(dls,               date.dst);
	CPPUNIT_ASSERT_EQUAL(offset,            date.offset);
}

#endif
