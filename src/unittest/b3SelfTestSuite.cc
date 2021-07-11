/*
**
**	$Filename:	b3SelfTestSuite.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - self test methods.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "b3SelfTestSuite.h"

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3SelfTestSuite);

void b3SelfTestSuite::test()
{
	CPPUNIT_ASSERT(b3SelfTest::b3TestDataSize());
	CPPUNIT_ASSERT(b3SelfTest::b3TestMemory());
	CPPUNIT_ASSERT(b3SelfTest::b3TestIO());
}

#endif
