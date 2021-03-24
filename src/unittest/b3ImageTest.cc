/*
**
**	$Filename:	b3ImageTest.cc $
**	$Release:	Dortmund 2011 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for file access classes.
**
**      (C) Copyright 2011  Steffen A. Mork
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

#include "b3ImageTest.h"
#include "b3TestMacros.h"
#include <blz3/base/b3Endian.h>

/*************************************************************************
**                                                                      **
**                        Unit tests for file access                    **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ImageTest);

#endif

const b3_res b3ImageTest::m_TestDepth[]
{
	1, 8, 12, 16, 24, 32, 128
};

void b3ImageTest::testTxData()
{
	static b3_u08 example[]
	{
		0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
	};

	b3_tx_data     data(example);
	b3_u08    *    cPtr = data;
	b3_u16    *    sPtr = data;
	b3_pkd_color * lPtr = data;
	b3_color   *   fPtr = data;

	CPPUNIT_ASSERT(cPtr != nullptr);
	CPPUNIT_ASSERT(sPtr != nullptr);
	CPPUNIT_ASSERT(lPtr != nullptr);
	CPPUNIT_ASSERT(fPtr != nullptr);

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u08, 0x11, *data);
	for (b3_size i = 0; i < sizeof(example); i++)
	{
		CPPUNIT_ASSERT_EQUAL(example[i], data[i]);
	}
	data++;
	CPPUNIT_ASSERT(&example[1] == data);
	CPPUNIT_ASSERT(0x4433     == b3Endian::b3GetIntel16(&sPtr[1]));
	CPPUNIT_ASSERT(0x88776655 == b3Endian::b3GetIntel32(&lPtr[1]));

	data = example;
	CPPUNIT_ASSERT(example == data);
}

void b3ImageTest::testScaleUnfiltered()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   src;
		b3Tx   dst_smaller;
		b3Tx   dst_bigger;
		b3Path path;

		src.b3AllocTx(50, 50, depth);

		path.b3Format("img_test_scale_us_%03ld.jpg", depth);
		dst_smaller.b3AllocTx(40, 40, depth);
		dst_smaller.b3Scale(&src);
		dst_smaller.b3SaveImage(path);

		path.b3Format("img_test_scale_ub_%03ld.jpg", depth);
		dst_bigger.b3AllocTx(70, 70, depth);
		dst_bigger.b3Scale(&src);
		dst_bigger.b3SaveImage(path);
	}
}

void b3ImageTest::testScaleFiltered()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   src;
		b3Tx   dst_smaller;
		b3Tx   dst_bigger;
		b3Path path;

		src.b3AllocTx(50, 50, depth);

		path.b3Format("img_test_scale_fs_%03ld.jpg", depth);
		dst_smaller.b3AllocTx(40, 40, 32);
		dst_smaller.b3ScaleToGrey(&src);
		dst_smaller.b3SaveImage(path);

		path.b3Format("img_test_scale_fb_%03ld.jpg", depth);
		dst_bigger.b3AllocTx(70, 70, 32);
		dst_bigger.b3ScaleToGrey(&src);
		dst_bigger.b3SaveImage(path);
	}
}

void b3ImageTest::testTransToGrey()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_grey_%03ld.jpg", depth);
		tx.b3AllocTx(50, 50, depth);
		tx.b3TransToGrey();
		tx.b3SaveImage(path);
	}
}
