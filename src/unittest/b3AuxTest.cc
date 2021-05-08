/*
**
**	$Filename:	b3AuxTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Auxiliary for unit tests.
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

#include "b3AuxTest.h"
#include "b3TestMacros.h"

#include <blz3/base/b3Aux.h>
#include <blz3/base/b3Color.h>

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3AuxTest);

void b3AuxTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3AuxTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3AuxTest::testAux()
{
	b3RGB rgb;

	rgb.r = 0x33;
	rgb.g = 0x55;
	rgb.b = 0x77;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x335577, rgb);
	rgb = 0xffffff;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xffffff, rgb);

	b3_color color = b3Color(1.0, 1.0, 1.0);
	rgb = color;
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xffffff, rgb);
}

void b3AuxTest::testRect()
{
	b3Rect rect;
	b3Rect r1(-1, -2, 3, 4);
	b3Rect r2( 1,  2, 3, 4);

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, 0, rect.b3Height());

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, -1, r1.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord, -2, r1.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  3, r1.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  4, r1.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  4, r1.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  6, r1.b3Height());

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  1, r2.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r2.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  3, r2.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  4, r2.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r2.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r2.b3Height());

	CPPUNIT_ASSERT(!rect.b3CheckBound(&r1));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  0, rect.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  0, rect.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  0, rect.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  0, rect.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  0, rect.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  0, rect.b3Height());

	CPPUNIT_ASSERT(r1.b3CheckBound(&r2));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  1, r1.m_x1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r1.m_y1);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  3, r1.m_x2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  4, r1.m_y2);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r1.b3Width());
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_coord,  2, r1.b3Height());
}

void b3AuxTest::testPoint()
{
	b3Point zero, point(-1.2, 2.5);

	CPPUNIT_ASSERT_EQUAL(0.0, zero.x);
	CPPUNIT_ASSERT_EQUAL(0.0, zero.y);

	CPPUNIT_ASSERT_EQUAL(-1.2, point.x);
	CPPUNIT_ASSERT_EQUAL( 2.5, point.y);
}

void b3AuxTest::testStrCase()
{
	const std::string test1 = "AA12BB-Öß-Αθήνα";
	const std::string test2 = "Aa12Bb-öß-Αθήνα";
	const std::string test3 = "Aa12bb-Öß-Αθήνα";
	const std::string lower = "aa12bb-öß-αθήνα";
	const std::string upper = "AA12BB-Öß-ΑΘΉΝΑ";

	CPPUNIT_ASSERT_EQUAL(lower, b3StringTool::b3ToLower(test1));
	CPPUNIT_ASSERT_EQUAL(lower, b3StringTool::b3ToLower(test2));
	CPPUNIT_ASSERT_EQUAL(lower, b3StringTool::b3ToLower(test3));

	CPPUNIT_ASSERT_EQUAL(upper, b3StringTool::b3ToUpper(test1));
	CPPUNIT_ASSERT_EQUAL(upper, b3StringTool::b3ToUpper(test2));
	CPPUNIT_ASSERT_EQUAL(upper, b3StringTool::b3ToUpper(test3));
}

void b3AuxTest::testStrCaseCmp()
{
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("", "") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("", "a") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "") > 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "aa") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "AA") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "aA") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "AA") < 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "b") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "B") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "b") < 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "B") < 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "a") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("a", "A") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "a") == 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("A", "A") == 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("b", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("b", "A") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("B", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("B", "A") > 0);

	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("aa", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("aa", "A") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("AA", "a") > 0);
	CPPUNIT_ASSERT(b3StringTool::b3CaseCompare("AA", "A") > 0);
}

#endif
