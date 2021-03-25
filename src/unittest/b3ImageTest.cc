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

#include <array>

/*************************************************************************
**                                                                      **
**                        Unit tests for file access                    **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ImageTest);

#define DATA_SIZE(a) (sizeof(a) / sizeof(a[0]))

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

void b3ImageTest::testPixel()
{
	b3_pkd_color data_u32[]
	{
		B3_BLACK, B3_DARK_GREY, B3_GREY, B3_LIGHT_GREY, B3_WHITE,
		B3_YELLOW, B3_GREEN, B3_CYAN, B3_RED, B3_MAGENTA, B3_BLUE
	};
	b3_u08       data_bw[]  { 0xa0 };
	b3_u16       data_u16[] { 0x000, 0x444, 0x888, 0xccc, 0xfff, 0x900, 0x090, 0x009, 0x123};
	b3_u08       data_vga[DATA_SIZE(data_u32)];
	b3_color     data_col[DATA_SIZE(data_u32)];
	b3_size      i = 0;
	b3Tx         bw, vga, high, color, hdr;

	for (b3_pkd_color c : data_u32)
	{
		data_vga[i] = i;
		data_col[i] = b3Color(c);
		i++;
	}
	bw.b3AllocTx(4, 1, 1);
	bw.b3SetData(data_bw, TX_BWA(sizeof(data_bw)));
	CPPUNIT_ASSERT(bw.b3IsBW());
	CPPUNIT_ASSERT(bw.b3IsPalette());
	CPPUNIT_ASSERT(!bw.b3IsHighColor());
	CPPUNIT_ASSERT(!bw.b3IsTrueColor());
	CPPUNIT_ASSERT(!bw.b3IsHdr());
	CPPUNIT_ASSERT_EQUAL(B3_BLACK, bw.b3GetValue(0, 0));
	CPPUNIT_ASSERT_EQUAL(B3_WHITE, bw.b3GetValue(1, 0));
	CPPUNIT_ASSERT_EQUAL(B3_BLACK, bw.b3GetValue(2, 0));
	CPPUNIT_ASSERT_EQUAL(B3_WHITE, bw.b3GetValue(3, 0));

	vga.b3AllocTx(DATA_SIZE(data_vga), 1, 8);
	vga.b3SetData(data_vga, sizeof(data_vga));
	vga.b3SetPalette(data_u32, DATA_SIZE(data_u32));
	CPPUNIT_ASSERT(!vga.b3IsBW());
	CPPUNIT_ASSERT(vga.b3IsPalette());
	CPPUNIT_ASSERT(!vga.b3IsHighColor());
	CPPUNIT_ASSERT(!vga.b3IsTrueColor());
	CPPUNIT_ASSERT(!vga.b3IsHdr());
	for (i = 0; i < DATA_SIZE(data_u32); i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], vga.b3GetValue(i, 0));
	}

	high.b3AllocTx(DATA_SIZE(data_u16), 1, 12);
	high.b3SetData(data_u16, sizeof(data_u16));
	CPPUNIT_ASSERT(!high.b3IsBW());
	CPPUNIT_ASSERT(!high.b3IsPalette());
	CPPUNIT_ASSERT(high.b3IsHighColor());
	CPPUNIT_ASSERT(!high.b3IsTrueColor());
	CPPUNIT_ASSERT(!high.b3IsHdr());
	CPPUNIT_ASSERT_EQUAL(B3_BLACK, high.b3GetValue(0, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x444444, high.b3GetValue(1, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x888888, high.b3GetValue(2, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xcccccc, high.b3GetValue(3, 0));
	CPPUNIT_ASSERT_EQUAL(B3_WHITE, high.b3GetValue(4, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x990000, high.b3GetValue(5, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x009900, high.b3GetValue(6, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x000099, high.b3GetValue(7, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x112233, high.b3GetValue(8, 0));

	color.b3AllocTx(DATA_SIZE(data_u32), 1, 24);
	color.b3SetData(data_u32, sizeof(data_u32));
	CPPUNIT_ASSERT(!color.b3IsBW());
	CPPUNIT_ASSERT(!color.b3IsPalette());
	CPPUNIT_ASSERT(!color.b3IsHighColor());
	CPPUNIT_ASSERT(color.b3IsTrueColor());
	CPPUNIT_ASSERT(!color.b3IsHdr());
	for (i = 0; i < DATA_SIZE(data_u32); i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], color.b3GetValue(i, 0));
	}

	hdr.b3AllocTx(DATA_SIZE(data_col), 1, 128);
	hdr.b3SetData(data_col, sizeof(data_col));
	CPPUNIT_ASSERT(!hdr.b3IsBW());
	CPPUNIT_ASSERT(!hdr.b3IsPalette());
	CPPUNIT_ASSERT(!hdr.b3IsHighColor());
	CPPUNIT_ASSERT(hdr.b3IsTrueColor());
	CPPUNIT_ASSERT(hdr.b3IsHdr());
	for (i = 0; i < DATA_SIZE(data_u32); i++)
	{
//		CPPUNIT_ASSERT_EQUAL(data_u32[i], hdr.b3GetValue(i, 0));
	}
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

		path.b3Format("img_test_scale_fs_%03ld-%03ld.jpg", depth, depth);
		dst_smaller.b3AllocTx(40, 40, 32);
		dst_smaller.b3ScaleToGrey(&src);
		dst_smaller.b3SaveImage(path);

		path.b3Format("img_test_scale_fb_%03ld-%03ld.jpg", depth, depth);
		dst_bigger.b3AllocTx(70, 70, 32);
		dst_bigger.b3ScaleToGrey(&src);
		dst_bigger.b3SaveImage(path);
	}

	b3Tx   bw;
	b3Tx   vga_smaller;
	b3Tx   vga_bigger;
	b3Path path;

	bw.b3AllocTx(50, 50, 1);

	path.b3Format("img_test_scale_fs_%03ld-%03ld.jpg", bw.depth, vga_smaller.depth);
	vga_bigger.b3AllocTx(40, 40, 32);
	vga_bigger.b3ScaleToGrey(&bw);
	vga_bigger.b3SaveImage(path);

	path.b3Format("img_test_scale_fb_%03ld-%03ld.jpg", bw.depth, vga_bigger.depth);
	vga_bigger.b3AllocTx(70, 70, 32);
	vga_bigger.b3ScaleToGrey(&bw);
	vga_bigger.b3SaveImage(path);
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

#endif
