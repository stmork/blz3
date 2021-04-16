/*
**
**	$Filename:	b3ErrorTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Error unit tests.
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

#include "b3ErrorTest.h"
#include "b3TestMacros.h"

#include "blz3/system/b3Error.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ErrorTest);

void b3ErrorTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3ErrorTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3ErrorTest::test()
{
	b3Error error;

	for (int i = 0; i < 30; i++)
	{
		errno = i;

		CPPUNIT_ASSERT(error.b3GetError() == i);
		CPPUNIT_ASSERT(error.b3GetErrorText() != nullptr);
	}
}

#endif
