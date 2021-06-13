/*
**
**	$Filename:	b3ExceptionTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for exceptions.
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

#include "b3ExceptionTest.h"
#include "b3TestMacros.h"

#include <blz3/base/b3FFT.h>

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ExceptionTest);

void b3ExceptionTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3ExceptionTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3ExceptionTest::test()
{
	try
	{
		B3_THROW(b3FFTException, B3_FFT_NO_PALETTE);
	}
	catch (b3FFTException & exception)
	{
		const b3_errno error = B3_MK_ERRNO(B3_FFT_NO_PALETTE, 0x464654);

		CPPUNIT_ASSERT_EQUAL(error, exception.b3GetError());
		CPPUNIT_ASSERT(exception.b3GetErrorMsg() != nullptr);
		CPPUNIT_ASSERT(exception.what() != nullptr);
	}
}

#endif
