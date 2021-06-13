/*
**
**	$Filename:	b3OpTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Operation do/undo/redo handling unit tests.
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

#include "b3OpTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3OpTest);

const int b3TestOperation::OP_ID = 1912;

void b3OpTest::setUp()
{
	m_Op = new b3TestOperation();
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3OpTest::tearDown()
{
	m_UndoBuffer.b3Clear();
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3OpTest::test()
{
	CPPUNIT_ASSERT_NO_THROW(m_Op->b3Prepare(nullptr));
	CPPUNIT_ASSERT(m_UndoBuffer.b3Do(m_Op));
	CPPUNIT_ASSERT(m_UndoBuffer.b3HasUndo());
	CPPUNIT_ASSERT(!m_UndoBuffer.b3HasRedo());

	CPPUNIT_ASSERT_NO_THROW(m_UndoBuffer.b3Undo());
	CPPUNIT_ASSERT(!m_UndoBuffer.b3HasUndo());
	CPPUNIT_ASSERT(m_UndoBuffer.b3HasRedo());

	CPPUNIT_ASSERT_NO_THROW(m_UndoBuffer.b3Redo());
	CPPUNIT_ASSERT(m_UndoBuffer.b3HasUndo());
	CPPUNIT_ASSERT(!m_UndoBuffer.b3HasRedo());

	CPPUNIT_ASSERT_EQUAL(b3TestOperation::OP_ID, m_Op->b3GetId());
}

#endif
