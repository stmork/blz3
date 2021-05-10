/*
**
**	$Filename:	b3PickTest.h $
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

#pragma once

#ifndef B3_PICK_TEST_H
#define B3_PICK_TEST_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Pick.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3NullPick : public b3Pick
{
public:
	b3NullPick(const b3_coord x = 0, const b3_coord y = 0) :
		b3Pick(x, y)
	{
	}

	virtual void b3Draw(const b3DrawContext * dc B3_UNUSED) override
	{
	}

	virtual b3UndoOperation * b3GetOperation(const b3RenderObject * object B3_UNUSED) override
	{
		return nullptr;
	}
};

class b3PickTest : public CppUnit::TestFixture
{
	b3PickBase m_PickPoints;
	b3NullPick m_Pick;

	static const b3_coord m_x = 10;
	static const b3_coord m_y = 10;

	CPPUNIT_TEST_SUITE(b3PickTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testPick);
	CPPUNIT_TEST(testMoveActive);
	CPPUNIT_TEST(testMoveInActive);
	CPPUNIT_TEST_SUITE_END();

public:
	b3PickTest();

	void setUp() override;
	void tearDown() override;
	void test();
	void testPick();
	void testMoveActive();
	void testMoveInActive();
};

#endif
#endif
