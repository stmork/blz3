/*
**
**	$Filename:	b3EndianTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - endianess unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_ENDIAN_TEST_H
#define B3_ENDIAN_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <array>

class b3EndianTest : public CppUnit::TestFixture
{
	static constexpr std::array<b3_u08, sizeof(b3_u16)> m_Word
	{
		0x11, 0x22
	};

	static constexpr std::array<b3_u08, sizeof(b3_u32)> m_Quad
	{
		0x11, 0x22, 0x33, 0x44
	};

	static constexpr std::array<b3_u08, sizeof(b3_u64)> m_Octa
	{
		0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
	};

	CPPUNIT_TEST_SUITE(b3EndianTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testLittleEndian);
	CPPUNIT_TEST(testBigEndian);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();
	void testLittleEndian(); // Intel
	void testBigEndian(); // Motorola
};

#endif
#endif
