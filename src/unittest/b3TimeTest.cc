/*
**
**	$Filename:	b3SkeletonTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Skeleton for unit tests.
**
**      (C) Copyright 2006  Steffen A. Mork
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

#include "b3TimeTest.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3TimeTest);

void b3TimeTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3TimeTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3TimeTest::test()
{
	b3Time now;

	b3_f64                    tp = now;
	time_t                    ti = now;
	b3_u32                    tr = std::fmod(tp * 1000000, 1000000.0);
	std::chrono::microseconds ms = now;
	std::chrono::seconds      s(now);
	std::chrono::microseconds r  = ms - s;

	CPPUNIT_ASSERT(ti == s.count());
	CPPUNIT_ASSERT(tr == r.count());
}

#endif
