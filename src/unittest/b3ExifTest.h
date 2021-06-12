/*
**
**	$Filename:	b3ExifTest.h $
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

#ifndef B3_EXIF_TEST_H
#define B3_EXIF_TEST_H

#include "blz3/image/b3Tx.h"
#include "blz3/raytrace/b3Scene.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3ExifTest : public CppUnit::TestFixture
{
	b3SearchPath m_SearchPath;
	b3Tx         m_Tx;
	b3World      m_World;

	CPPUNIT_TEST_SUITE(b3ExifTest);
	CPPUNIT_TEST(testSimple);
	CPPUNIT_TEST(testRaytrace);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testSimple();
	void testRaytrace();
};

#endif
#endif
