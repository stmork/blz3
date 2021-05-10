/*
**
**	$Filename:	b3OpTest.h $
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

#ifndef B3_OP_TEST_H
#define B3_OP_TEST_H

#include "blz3/b3Config.h"
#include "blz3/base/b3UndoOperation.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3TestOperation : public b3UndoOperation
{
	b3_count m_Count = 0;

public:
	void b3Redo() override
	{
		m_Count++;
	}

	void b3Undo() override
	{
		m_Count--;
	}

	void b3Prepare(b3UndoPrepareInfo * info B3_UNUSED) override
	{
		b3Initialize();
	}

	int b3GetId() const override
	{
		return 0;
	}
};

class b3OpTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3OpTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

	b3UndoBuffer      m_UndoBuffer;
	b3TestOperation * m_Op = nullptr;

public:
	void setUp() override;
	void tearDown() override;
	void test();
};

#endif
#endif
