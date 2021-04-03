/*
**
**	$Filename:	b3SamplerTest.h $
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

#ifndef B3_SAMPLER_TEST_H
#define B3_SAMPLER_TEST_H

#include "blz3/b3Config.h"

#include "blz3/image/b3Sampler.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class b3SamplerTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(b3SamplerTest);
	CPPUNIT_TEST(testSampler);
	CPPUNIT_TEST(testOceanWave);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override;
	void tearDown() override;
	void testSampler();
	void testOceanWave();

private:
	void sample(b3Sampler & sampler, b3Tx & tx, const char * image_name);
};

#endif
#endif
