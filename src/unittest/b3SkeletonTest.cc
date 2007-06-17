/*
**
**	$Filename:	b3SkeletonTest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision: 1475 $
**	$Date: 2007-01-01 14:04:37 +0100 (Mo, 01 Jan 2007) $
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
  
#include "b3SkeletonTest.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3SkeletonTest);

void b3SkeletonTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3SkeletonTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3SkeletonTest::test()
{
}

#endif
