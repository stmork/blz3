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

#include "blz3/system/b3Date.h"

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
	b3Date now;
}

void b3DateTest::testDate()
{
	b3Date date(28, b3_month::B3_JUNE, 1964);

	CPPUNIT_ASSERT_TYPED_EQUAL(unsigned, 28, date.day);
	CPPUNIT_ASSERT_EQUAL(b3_month::B3_JUNE,  date.month);
	CPPUNIT_ASSERT_EQUAL(1964,               date.year);
	CPPUNIT_ASSERT(date.dls);
}

void b3DateTest::testEqual()
{
	b3Date date(28, b3_month::B3_JUNE, 1964);
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

#endif
