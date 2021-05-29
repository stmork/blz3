/*
**
**	$Filename:	b3ExifTest.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3ExifTest.h"
#include "b3TestMacros.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ExifTest);

void b3ExifTest::setUp()
{
	m_Tx.b3AllocTx(320, 240, 32);
	m_Tx.b3TestPattern();

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3ExifTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3ExifTest::test()
{
#ifdef HAVE_LIBEXIF
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_Tx.b3SaveJPEG("img_exif_32.jpg"));
#endif
}

#endif
