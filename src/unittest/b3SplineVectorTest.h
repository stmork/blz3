/*
**
**	$Filename:	b3SplineVectorTest.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Spline template parameter unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_SPLINEVECTOR_TEST_H
#define B3_SPLINEVECTOR_TEST_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <blz3/base/b3SplineVector.h>

template<class VECTOR> class b3SplineVectorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3SplineVectorTest<VECTOR>);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

	VECTOR a;
	VECTOR b;
	VECTOR c;
	VECTOR result;

public:
	void setUp() override;
	void tearDown() override
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void test();
};

#endif
#endif
