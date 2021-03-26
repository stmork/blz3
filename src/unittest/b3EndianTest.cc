/*
**
**	$Filename:	b3EndianTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Endianess unit tests.
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

#include "b3EndianTest.h"
#include "b3TestMacros.h"

#include <blz3/base/b3Endian.h>

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3EndianTest);

void b3EndianTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3EndianTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3EndianTest::test()
{
	std::array<b3_u08, sizeof(b3_u16)> word = m_Word;
	std::array<b3_u08, sizeof(b3_u32)> quad = m_Quad;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u16,     0x2211, b3Endian::b3Get16(word.data()));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u32, 0x44332211, b3Endian::b3Get32(quad.data()));
}

void b3EndianTest::testLittleEndian()
{
	std::array<b3_u08, sizeof(b3_u16)> word = m_Word;
	std::array<b3_u08, sizeof(b3_u32)> quad = m_Quad;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u16,     0x2211, b3Endian::b3GetIntel16(word.data()));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u32, 0x44332211, b3Endian::b3GetIntel32(quad.data()));
}

void b3EndianTest::testBigEndian()
{
	std::array<b3_u08, sizeof(b3_u16)> word = m_Word;
	std::array<b3_u08, sizeof(b3_u32)> quad = m_Quad;

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u16,     0x1122, b3Endian::b3GetMot16(word.data()));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u32, 0x11223344, b3Endian::b3GetMot32(quad.data()));
}

#endif
