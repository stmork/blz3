/*
**
**	$Filename:	b3CompareTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Base and system unit tests
**
**      (C) Copyright 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3COMPARETEST_H
#define B3COMPARETEST_H

#include <blz3/b3Config.h>
#include "blz3/base/b3Compare.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class b3CompareTest : public CppUnit::TestFixture
{

	CPPUNIT_TEST_SUITE(b3CompareTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void test();
};

#endif
#endif // B3COMPARETEST_H
