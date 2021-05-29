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

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3ExifTest : public CppUnit::TestFixture
{
	b3Tx m_Tx;

	CPPUNIT_TEST_SUITE(b3ExifTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void test();
};

#endif
#endif
