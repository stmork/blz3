/*
**
**	$Filename:	b3PickTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Tests for pick points.
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

#include "b3PickTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3PickTest);

b3PickTest::b3PickTest()
{
	m_PickPoints.b3Append(&m_Pick);
}

void b3PickTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	m_Pick.b3Moved(m_x, m_y);
}

void b3PickTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3PickTest::test()
{
	const b3_coord x = m_x + 2 * m_PickPoints.m_PickSize;
	const b3_coord y = m_y + 2 * m_PickPoints.m_PickSize;

	CPPUNIT_ASSERT(!m_PickPoints.b3IsEmpty());
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
	CPPUNIT_ASSERT(!m_PickPoints.b3Down(x, y));
	CPPUNIT_ASSERT(!m_PickPoints.b3Move(x, y));
	CPPUNIT_ASSERT(!m_PickPoints.b3Up(x, y));

	// TODO: More tests need more sophisticated implementation.
	CPPUNIT_ASSERT(m_PickPoints.b3GetOperation(nullptr) == nullptr);
	CPPUNIT_ASSERT_NO_THROW(m_PickPoints.b3Draw(nullptr));
}

void b3PickTest::testPick()
{
	const b3_coord   x = 10;
	const b3_coord   y = 10;
	const b3NullPick pick(x, y);

	CPPUNIT_ASSERT(pick.b3Hit(x, y));
	CPPUNIT_ASSERT(pick.b3Hit(x, y + b3PickBase::m_PickSize));
	CPPUNIT_ASSERT(pick.b3Hit(x, y - b3PickBase::m_PickSize));
	CPPUNIT_ASSERT(pick.b3Hit(x + b3PickBase::m_PickSize, y));
	CPPUNIT_ASSERT(pick.b3Hit(x - b3PickBase::m_PickSize, y));
	CPPUNIT_ASSERT(!pick.b3Hit(x, y + b3PickBase::m_PickSize + 1));
	CPPUNIT_ASSERT(!pick.b3Hit(x, y - b3PickBase::m_PickSize - 1));
	CPPUNIT_ASSERT(!pick.b3Hit(x + b3PickBase::m_PickSize + 2, y));
	CPPUNIT_ASSERT(!pick.b3Hit(x - b3PickBase::m_PickSize - 1, y));
}

void b3PickTest::testMoveActive()
{
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
	CPPUNIT_ASSERT(m_PickPoints.b3Down(m_x, m_y));
	CPPUNIT_ASSERT(m_PickPoints.b3IsActive());
	for (b3_coord x = 1; x <= 10; x++)
	{
		CPPUNIT_ASSERT(m_PickPoints.b3IsActive());
		CPPUNIT_ASSERT(m_PickPoints.b3Move(m_x + x, m_y));
	}
	CPPUNIT_ASSERT(m_PickPoints.b3IsActive());
	CPPUNIT_ASSERT(m_PickPoints.b3Up(m_x + 10, m_y));
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
	CPPUNIT_ASSERT(!m_PickPoints.b3Up(m_x + 10, m_y));
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
}

void b3PickTest::testMoveInActive()
{
	const b3_coord y = m_y + 2 * m_PickPoints.m_PickSize;

	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
	CPPUNIT_ASSERT(!m_PickPoints.b3Down(m_x, y));
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
	for (b3_coord x = 1; x <= 10; x++)
	{
		CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
		CPPUNIT_ASSERT(!m_PickPoints.b3Move(m_x + x, y));
	}
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
	CPPUNIT_ASSERT(!m_PickPoints.b3Up(m_x + 10, y));
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
	CPPUNIT_ASSERT(!m_PickPoints.b3Up(m_x + 10, y));
	CPPUNIT_ASSERT(!m_PickPoints.b3IsActive());
}

#endif
