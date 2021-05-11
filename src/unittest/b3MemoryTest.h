/*
**
**	$Filename:	b3MemoryTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for memory management.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_SKELETON_TEST_H
#define B3_SKELETON_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "blz3/system/b3Memory.h"

class b3MemoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3MemoryTest);
	CPPUNIT_TEST(testSimple);
	CPPUNIT_TEST(testMemory);
	CPPUNIT_TEST(testMultiple);
	CPPUNIT_TEST(testStrCopy);
	CPPUNIT_TEST_SUITE_END();

	static const b3_count ARRAY_COUNT   =  127;
	static const b3_count MEM_MIN       = 1000;
	static const b3_count MEM_LOW_MULT  =    2;
	static const b3_count MEM_HIGH_MULT =  200;

	b3Mem     m_MemPool;
	b3_u08    m_Buffer[MEM_MIN];
	void   *  ptr1, *ptr2;

public:
	void setUp() override;
	void tearDown() override;
	void testSimple();
	void testMemory();
	void testMultiple();
	void testStrCopy();
};

#endif
#endif
