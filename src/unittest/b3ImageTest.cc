/*
**
**	$Filename:	b3ImageTest.cc $
**	$Release:	Dortmund 2024 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for file access classes.
**
**      (C) Copyright 2011 - 2024  Steffen A. Mork
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
#include <blz3/base/b3FileList.h>
#include <blz3/base/b3Math.h>
#include <blz3/image/b3TxPool.h>

#include <array>
#include <regex>

/*************************************************************************
**                                                                      **
**                        Static data for unit tests                    **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ImageTest);

#define DATA_SIZE(a) (sizeof(a) / sizeof(a[0]))

const b3_res b3ImageTest::m_TestDepth[]
{
	1, 8, 12, 16, 24, 32, 96, 128
};

b3_pkd_color b3ImageTest::data_u32[]
{
	b3Color::B3_BLACK,
	b3Color::B3_DARK_GREY,
	b3Color::B3_GREY,
	b3Color::B3_LIGHT_GREY,

	b3Color::B3_WHITE,
	b3Color::B3_YELLOW,
	b3Color::B3_GREEN,
	b3Color::B3_CYAN,
	b3Color::B3_RED,
	b3Color::B3_MAGENTA,
	b3Color::B3_BLUE
};

b3_u16       b3ImageTest::data_u16[]
{
	0x000, 0x444, 0x888, 0xccc, 0xfff, 0x900, 0x090, 0x009, 0x123
};

b3_u08       b3ImageTest::data_bw[]
{
	0xa0
};

b3_u08       b3ImageTest::data_vga[DATA_SIZE(data_u32)];
b3_color     b3ImageTest::data_col[DATA_SIZE(data_u32)];

/*************************************************************************
**                                                                      **
**                        Unit test setup                               **
**                                                                      **
*************************************************************************/

void b3ImageTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);

	b3SetupImages();

	m_TxMap.clear();
	m_TxMap.emplace(  1, &m_TxBW);
	m_TxMap.emplace(  8, &m_TxPallColor);
	m_TxMap.emplace( 12, &m_TxHighColor);
	m_TxMap.emplace( 16, &m_TxHighColor);
	m_TxMap.emplace( 24, &m_TxTrueColor);
	m_TxMap.emplace( 32, &m_TxTrueColor);
	m_TxMap.emplace( 48, &m_TxRealColor);
	m_TxMap.emplace( 64, &m_TxRealColor);
	m_TxMap.emplace( 96, &m_TxRealColor);
	m_TxMap.emplace(128, &m_TxRealColor);
}

void b3ImageTest::testPattern()
{
	m_TxBW.b3SaveImage("img_pattern_01.jpg");
	m_TxPallColor.b3SaveImage("img_pattern_08.jpg");
	m_TxHighColor.b3SaveImage("img_pattern_10.jpg");
	m_TxTrueColor.b3SaveImage("img_pattern_20.jpg");
	m_TxRealColor.b3SaveImage("img_pattern_80.jpg");
}

/*************************************************************************
**                                                                      **
**                        Unit tests for basic data type handling       **
**                                                                      **
*************************************************************************/

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

void b3ImageTest::testColor()
{
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  b3Tx::b3ToGrey(b3Color::B3_BLACK), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,  b3Tx::b3ToGrey(b3Color::B3_WHITE), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.35, b3Tx::b3ToGrey(b3Color::B3_RED),   b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.51, b3Tx::b3ToGrey(b3Color::B3_GREEN), b3Math::epsilon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.14, b3Tx::b3ToGrey(b3Color::B3_BLUE),  b3Math::epsilon);

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0x00,  b3Tx::b3ColorToIndex(b3Color::B3_BLACK));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0xff,  b3Tx::b3ColorToIndex(b3Color::B3_WHITE));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0xe0,  b3Tx::b3ColorToIndex(b3Color::B3_RED));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0x1c,  b3Tx::b3ColorToIndex(b3Color::B3_GREEN));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_index, 0x03,  b3Tx::b3ColorToIndex(b3Color::B3_BLUE));

	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, b3Color::B3_BLACK, b3Tx::b3IndexToColor(0x00));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xe0e0c0, b3Tx::b3IndexToColor(0xff));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xe00000, b3Tx::b3IndexToColor(0xe0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x00e000, b3Tx::b3IndexToColor(0x1c));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x0000c0, b3Tx::b3IndexToColor(0x03));

	for (b3_index i = 0; i < 256; i++)
	{
		b3_pkd_color color = b3Tx::b3IndexToColor(i);

		CPPUNIT_ASSERT_EQUAL(i, b3Tx::b3ColorToIndex(color));
	}
}

void b3ImageTest::testPixel()
{
	b3Tx     bw, vga, high, color, hdr;
	b3_size  i = 0;

	for (b3_pkd_color c : data_u32)
	{
		data_vga[i] = i;
		data_col[i] = b3Color(c);
		i++;
	}

	CPPUNIT_ASSERT(bw.b3AllocTx(4, 1, 1));
	bw.b3SetData(data_bw, TX_BWA(sizeof(data_bw)));
	CPPUNIT_ASSERT( bw.b3IsBW());
	CPPUNIT_ASSERT( bw.b3IsPalette());
	CPPUNIT_ASSERT(!bw.b3IsHighColor());
	CPPUNIT_ASSERT(!bw.b3IsTrueColor());
	CPPUNIT_ASSERT(!bw.b3IsHdr());
	CPPUNIT_ASSERT_EQUAL(b3Color::B3_BLACK, bw.b3GetValue(0, 0));
	CPPUNIT_ASSERT_EQUAL(b3Color::B3_WHITE, bw.b3GetValue(1, 0));
	CPPUNIT_ASSERT_EQUAL(b3Color::B3_BLACK, bw.b3GetValue(2, 0));
	CPPUNIT_ASSERT_EQUAL(b3Color::B3_WHITE, bw.b3GetValue(3, 0));

	CPPUNIT_ASSERT(bw.b3GetPalette() != nullptr);
	CPPUNIT_ASSERT_NO_THROW(bw.b3GetIndexData());
	CPPUNIT_ASSERT_THROW(bw.b3GetHighColorData(), b3TxException);
	CPPUNIT_ASSERT_THROW(bw.b3GetTrueColorData(), b3TxException);
	CPPUNIT_ASSERT_THROW(bw.b3GetHdrData(), b3TxException);

	CPPUNIT_ASSERT(vga.b3AllocTx(DATA_SIZE(data_vga), 1, 8));
	vga.b3SetData(data_vga, sizeof(data_vga));
	vga.b3SetPalette(data_u32, DATA_SIZE(data_u32));
	CPPUNIT_ASSERT(!vga.b3IsBW());
	CPPUNIT_ASSERT( vga.b3IsPalette());
	CPPUNIT_ASSERT(!vga.b3IsHighColor());
	CPPUNIT_ASSERT(!vga.b3IsTrueColor());
	CPPUNIT_ASSERT(!vga.b3IsHdr());

	CPPUNIT_ASSERT(vga.b3GetPalette() != nullptr);
	CPPUNIT_ASSERT_NO_THROW(vga.b3GetIndexData());
	CPPUNIT_ASSERT_THROW(vga.b3GetHighColorData(), b3TxException);
	CPPUNIT_ASSERT_THROW(vga.b3GetTrueColorData(), b3TxException);
	CPPUNIT_ASSERT_THROW(vga.b3GetHdrData(), b3TxException);

	for (i = 0; i < DATA_SIZE(data_u32); i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], vga.b3GetValue(i, 0));
	}

	CPPUNIT_ASSERT(high.b3AllocTx(DATA_SIZE(data_u16), 1, 12));
	high.b3SetData(data_u16, sizeof(data_u16));
	CPPUNIT_ASSERT(!high.b3IsBW());
	CPPUNIT_ASSERT(!high.b3IsPalette());
	CPPUNIT_ASSERT( high.b3IsHighColor());
	CPPUNIT_ASSERT(!high.b3IsTrueColor());
	CPPUNIT_ASSERT(!high.b3IsHdr());

	CPPUNIT_ASSERT(high.b3GetPalette() == nullptr);
	CPPUNIT_ASSERT_THROW(high.b3GetIndexData(), b3TxException);
	CPPUNIT_ASSERT_NO_THROW(high.b3GetHighColorData());
	CPPUNIT_ASSERT_THROW(high.b3GetTrueColorData(), b3TxException);
	CPPUNIT_ASSERT_THROW(high.b3GetHdrData(), b3TxException);

	CPPUNIT_ASSERT_EQUAL(b3Color::B3_BLACK, high.b3GetValue(0, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x444444, high.b3GetValue(1, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x888888, high.b3GetValue(2, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0xcccccc, high.b3GetValue(3, 0));
	CPPUNIT_ASSERT_EQUAL(b3Color::B3_WHITE, high.b3GetValue(4, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x990000, high.b3GetValue(5, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x009900, high.b3GetValue(6, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x000099, high.b3GetValue(7, 0));
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_pkd_color, 0x112233, high.b3GetValue(8, 0));

	CPPUNIT_ASSERT(color.b3AllocTx(DATA_SIZE(data_u32), 1, 24));
	color.b3SetData(data_u32, sizeof(data_u32));
	CPPUNIT_ASSERT(!color.b3IsBW());
	CPPUNIT_ASSERT(!color.b3IsPalette());
	CPPUNIT_ASSERT(!color.b3IsHighColor());
	CPPUNIT_ASSERT( color.b3IsTrueColor());
	CPPUNIT_ASSERT(!color.b3IsHdr());

	CPPUNIT_ASSERT(color.b3GetPalette() == nullptr);
	CPPUNIT_ASSERT_THROW(color.b3GetIndexData(), b3TxException);
	CPPUNIT_ASSERT_THROW(color.b3GetHighColorData(), b3TxException);
	CPPUNIT_ASSERT_NO_THROW(color.b3GetTrueColorData());
	CPPUNIT_ASSERT_THROW(color.b3GetHdrData(), b3TxException);
	for (i = 0; i < DATA_SIZE(data_u32); i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], color.b3GetValue(i, 0));
	}

	CPPUNIT_ASSERT(hdr.b3AllocTx(DATA_SIZE(data_col), 1, 128));
	hdr.b3SetData(data_col, sizeof(data_col));
	CPPUNIT_ASSERT(!hdr.b3IsBW());
	CPPUNIT_ASSERT(!hdr.b3IsPalette());
	CPPUNIT_ASSERT(!hdr.b3IsHighColor());
	CPPUNIT_ASSERT( hdr.b3IsTrueColor());
	CPPUNIT_ASSERT( hdr.b3IsHdr());

	CPPUNIT_ASSERT(hdr.b3GetPalette() == nullptr);
	CPPUNIT_ASSERT_THROW(hdr.b3GetIndexData(), b3TxException);
	CPPUNIT_ASSERT_THROW(hdr.b3GetHighColorData(), b3TxException);
	CPPUNIT_ASSERT_NO_THROW(hdr.b3GetTrueColorData());
	CPPUNIT_ASSERT_NO_THROW(hdr.b3GetHdrData());
	for (i = 0; i < DATA_SIZE(data_u32); i++)
	{
		const b3_pkd_color aux = hdr.b3GetValue(i, 0);

		CPPUNIT_ASSERT_EQUAL(data_u32[i], aux);
	}
}

void b3ImageTest::testRow()
{
	b3Tx         bw, vga, high, color, hdr;
	b3_pkd_color row_u32[DATA_SIZE(data_u32)];
	b3_color     row_col[DATA_SIZE(data_u32)];
	b3_res       i = 0;

	for (b3_pkd_color c : data_u32)
	{
		data_vga[i] = i;
		data_col[i] = b3Color(c);
		i++;
	}

	CPPUNIT_ASSERT(bw.b3AllocTx(4, 1, 1));
	bw.b3SetData(data_bw, TX_BWA(sizeof(data_bw)));
	bw.b3GetRow(row_u32, 0);
	bw.b3GetRow(row_col, 0);

	CPPUNIT_ASSERT(vga.b3AllocTx(DATA_SIZE(data_vga), 1, 8));
	vga.b3SetData(data_vga, sizeof(data_vga));
	vga.b3SetPalette(data_u32, DATA_SIZE(data_u32));
	vga.b3GetRow(row_u32, 0);
	vga.b3GetRow(row_col, 0);
	for (i = 0; i < vga.xSize; i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], row_u32[i]);
		CPPUNIT_ASSERT(data_col[i] == row_col[i]);
	}

	CPPUNIT_ASSERT(high.b3AllocTx(DATA_SIZE(data_u16), 1, 12));
	high.b3SetData(data_u16, sizeof(data_u16));
	high.b3GetRow(row_u32, 0);
	high.b3GetRow(row_col, 0);

	CPPUNIT_ASSERT(color.b3AllocTx(DATA_SIZE(data_u32), 1, 24));
	color.b3SetData(data_u32, sizeof(data_u32));
	color.b3GetRow(row_u32, 0);
	color.b3GetRow(row_col, 0);
	for (i = 0; i < color.xSize; i++)
	{
		const b3_pkd_color value = b3Color(row_col[i]);

		CPPUNIT_ASSERT_EQUAL(data_u32[i], row_u32[i]);
		CPPUNIT_ASSERT_EQUAL(data_u32[i], value);
	}

	CPPUNIT_ASSERT(hdr.b3AllocTx(DATA_SIZE(data_col), 1, 128));
	hdr.b3SetData(data_col, sizeof(data_col));
	hdr.b3GetRow(row_u32, 0);
	hdr.b3GetRow(row_col, 0);
	for (i = 0; i < hdr.xSize; i++)
	{
		const b3Color value(row_u32[i]);

		CPPUNIT_ASSERT(data_col[i] == value);
		CPPUNIT_ASSERT(data_col[i] == row_col[i]);
	}
}

void b3ImageTest::testEmpty()
{
	b3Tx empty, dst;

	CPPUNIT_ASSERT(!empty.b3AllocTx(  0,   0, 32));
	CPPUNIT_ASSERT(!empty.b3AllocTx( -1,  -1, -1));
	CPPUNIT_ASSERT(!empty.b3AllocTx(128, 128, 19));

	CPPUNIT_ASSERT(!empty.b3IsLoaded());
	CPPUNIT_ASSERT(!empty.b3IsBW());
	CPPUNIT_ASSERT(!empty.b3IsPalette());
	CPPUNIT_ASSERT(!empty.b3IsHighColor());
	CPPUNIT_ASSERT(!empty.b3IsTrueColor());
	CPPUNIT_ASSERT(!empty.b3IsHdr());

	CPPUNIT_ASSERT(nullptr == empty.b3GetPalette());
	CPPUNIT_ASSERT(nullptr == empty.b3GetIndexData());
	CPPUNIT_ASSERT(nullptr == empty.b3GetHighColorData());
	CPPUNIT_ASSERT(nullptr == empty.b3GetTrueColorData());
	CPPUNIT_ASSERT(nullptr == empty.b3GetHdrData());

	CPPUNIT_ASSERT_THROW(dst.b3Scale(&empty), b3TxException);
	CPPUNIT_ASSERT_THROW(dst.b3ScaleToGrey(&empty), b3TxException);
	CPPUNIT_ASSERT_THROW(dst.b3TransToBW(&empty), b3TxException);
	CPPUNIT_ASSERT(!empty.b3Negate());
	CPPUNIT_ASSERT_THROW(empty.b3MirrorHorizontal(), b3TxException);
	CPPUNIT_ASSERT_THROW(empty.b3MirrorVertical(), b3TxException);
	CPPUNIT_ASSERT_THROW(empty.b3TurnLeft(), b3TxException);
	CPPUNIT_ASSERT_THROW(empty.b3Turn(), b3TxException);
	CPPUNIT_ASSERT_THROW(empty.b3TurnRight(), b3TxException);
	CPPUNIT_ASSERT_THROW(empty.b3RemoveBlackBorder(), b3TxException);
	CPPUNIT_ASSERT_THROW(empty.b3Shrink(), b3TxException);
	CPPUNIT_ASSERT_THROW(empty.b3Histogramme(), b3TxException);

	for (b3_res depth : m_TestDepth)
	{
		b3Tx tx;

		CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));

		if (!tx.b3IsBW())
		{
			CPPUNIT_ASSERT_THROW(empty.b3RemoveBlackBorder(), b3TxException);
			CPPUNIT_ASSERT_THROW(empty.b3Shrink(), b3TxException);
		}
	}
}

void b3ImageTest::testCopy()
{
	for (b3_res depth : m_TestDepth)
	{
		const b3Tx * src = m_TxMap[depth];
		b3Tx         dst;

		CPPUNIT_ASSERT_NO_THROW(dst.b3Copy(src));

		CPPUNIT_ASSERT_EQUAL(src->depth, dst.depth);

		compareImages(src, dst, true);
	}
}

/*************************************************************************
**                                                                      **
**                        Unit tests for file access                    **
**                                                                      **
*************************************************************************/

void b3ImageTest::testRead()
{
	// Use https://regex101.com/
	static std::regex  regex(R"(Lenna-([012][01248c])\.[a-z0-9]+)");
	static std::smatch matcher;
	b3FileList         list;

	list.b3RecCreateList(".");
	list.b3Sort();
	for (b3FileEntry * entry = list.b3First(); entry != nullptr; entry = entry->Succ)
	{
		b3Path filename;

		b3Path::b3SplitFileName(entry->b3Name(), nullptr, filename);
		std::string s(filename);
		if (std::regex_match(s, matcher, regex))
		{
			const b3_res depth = std::stol(matcher[1], 0, 16);
			b3Tx         tx;

			CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3LoadImage(entry->b3Name(), false));
			CPPUNIT_ASSERT_TYPED_EQUAL(b3_res, 512, tx.xSize);
			CPPUNIT_ASSERT_TYPED_EQUAL(b3_res, 512, tx.ySize);

			CPPUNIT_ASSERT_EQUAL(depth, tx.depth);
			CPPUNIT_ASSERT_TYPED_EQUAL(bool, tx.depth <= 8, tx.b3IsPalette());
			if (tx.depth <= 8)
			{
				CPPUNIT_ASSERT(tx.b3GetPalette() != nullptr);
			}
		}
	}
}

void b3ImageTest::testReadGIF()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxGIF.b3LoadImage("fft_test.gif", true));
//	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.gif"));
}

void b3ImageTest::testWriteJPEG()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.jpg"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.jpg"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.jpg"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.jpg"));

	// No data comparison because JPEG is not lossless.
	compareImages(m_TxTrueColor, true, false);
}

void b3ImageTest::testWritePNG()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.png"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.png"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.png"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.png"));

	// No meta data comparison because TGA stores always in true color.
	compareImages(m_TxPallColor, false);
	compareImages(m_TxHighColor, false);
	compareImages(m_TxTrueColor);
	compareImages(m_TxRealColor, false, true);
}

void b3ImageTest::testWritePS()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.ps"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.ps"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.ps"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.ps"));
}

void b3ImageTest::testWriteRGB8()
{
	b3Tx tx;

	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.rgb8"));
//	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.rgb4"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.rgb8"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.rgb8"));

//	compareImages(m_TxPallColor);
//	compareImages(m_TxTrueColor);
}

void b3ImageTest::testWriteTGA()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.tga"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.tga"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.tga"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.tga"));

	// No meta data comparison because TGA stores always in true color.
	compareImages(m_TxPallColor, false);
	compareImages(m_TxHighColor, false);
	compareImages(m_TxTrueColor);
	compareImages(m_TxRealColor, false, true);
}

void b3ImageTest::testWriteTIFF()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK,        m_TxBW.b3SaveImage("img_test_01.tiff"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.tiff"));
//	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.tiff"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.tiff"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.tiff"));

	compareImages(m_TxPallColor);
	compareImages(m_TxTrueColor);
}

#ifdef BLZ3_USE_OPENEXR
void b3ImageTest::testWriteOpenEXR()
{
	m_TxRealColor.b3SaveImage("img_test_80.exr");

	compareImages(m_TxRealColor);
}
#endif

void b3ImageTest::testWriteBomb()
{
	b3Tx   tx;
	b3_res res = 2048;

	while (res <= 32768)
	{
		char filename[B3_FILESTRINGLEN];

		snprintf(filename, sizeof(filename), "image_bomb_%02dk", res / 1024);

		CPPUNIT_ASSERT_NO_THROW(tx.b3AllocTx(res, res, 24));
		CPPUNIT_ASSERT_NO_THROW(tx.b3SaveJPEG((std::string(filename) + ".jpg").c_str(), 1));
		CPPUNIT_ASSERT_NO_THROW(tx.b3SavePNG( (std::string(filename) + ".png").c_str(), nullptr));
		CPPUNIT_ASSERT_NO_THROW(tx.b3SaveTIFF((std::string(filename) + ".tiff").c_str()));
		CPPUNIT_ASSERT_NO_THROW(tx.b3SaveTGA( (std::string(filename) + ".tga").c_str()));

		res *= 2;
	}
}

/*************************************************************************
**                                                                      **
**                        Unit tests for image manipulation             **
**                                                                      **
*************************************************************************/

void b3ImageTest::testScaleUnfiltered()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   src;
		b3Tx   dst_smaller;
		b3Tx   dst_bigger;
		b3Tx   dst_huger;
		b3Path path;

		CPPUNIT_ASSERT(src.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		CPPUNIT_ASSERT_NO_THROW(src.b3Copy(m_TxMap[depth]));

		path.b3Format("img_test_scale_us_%03d.jpg", depth);
		dst_smaller.b3AllocTx(src.xSize * 0.8, src.ySize * 0.8, depth);
		dst_smaller.b3Scale(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_smaller.b3SaveImage(path));

		path.b3Format("img_test_scale_ub_%03d.jpg", depth);
		dst_bigger.b3AllocTx(src.xSize * 1.4, src.ySize * 1.4, depth);
		dst_bigger.b3Scale(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_bigger.b3SaveImage(path));

		path.b3Format("img_test_scale_uh_%03d.jpg", depth);
		CPPUNIT_ASSERT(dst_huger.b3AllocTx(1024, 1024, depth));
		dst_huger.b3Scale(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_huger.b3SaveImage(path));
	}
}

void b3ImageTest::testScaleFiltered()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   src;
		b3Tx   dst_smaller;
		b3Tx   dst_bigger;
		b3Tx   dst_huger;
		b3Path path;

		CPPUNIT_ASSERT(src.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		CPPUNIT_ASSERT_NO_THROW(src.b3Copy(m_TxMap[depth]));

		path.b3Format("img_test_scale_fs_%03d-%03d.jpg", depth, depth);
		CPPUNIT_ASSERT(dst_smaller.b3AllocTx(src.xSize * 0.8, src.ySize * 0.8, 32));
		dst_smaller.b3ScaleToGrey(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_smaller.b3SaveImage(path));

		path.b3Format("img_test_scale_fb_%03d-%03d.jpg", depth, depth);
		CPPUNIT_ASSERT(dst_bigger.b3AllocTx(src.xSize * 1.4, src.ySize * 1.4, 32));
		dst_bigger.b3ScaleToGrey(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_bigger.b3SaveImage(path));

		path.b3Format("img_test_scale_fh_%03d-%03d.jpg", depth, depth);
		CPPUNIT_ASSERT(dst_huger.b3AllocTx(1024, 1024, 32));
		dst_huger.b3ScaleToGrey(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_huger.b3SaveImage(path));
	}

	b3Tx   bw;
	b3Tx   vga_smaller;
	b3Tx   vga_bigger;
	b3Path path;

	bw.b3AllocTx(50, 50, 1);

	path.b3Format("img_test_scale_fs_%03d-%0d.jpg", bw.depth, vga_smaller.depth);
	CPPUNIT_ASSERT(vga_bigger.b3AllocTx(bw.xSize * 0.8, bw.ySize * 0.8, 32));
	vga_bigger.b3ScaleToGrey(&bw);
	CPPUNIT_ASSERT_EQUAL(B3_OK, vga_bigger.b3SaveImage(path));

	path.b3Format("img_test_scale_fb_%03d-%03d.jpg", bw.depth, vga_bigger.depth);
	CPPUNIT_ASSERT(vga_bigger.b3AllocTx(bw.xSize * 1.4, bw.ySize * 1.4, 32));
	vga_bigger.b3ScaleToGrey(&bw);
	CPPUNIT_ASSERT_EQUAL(B3_OK, vga_bigger.b3SaveImage(path));
}

void b3ImageTest::testTransToGrey()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_grey_%03d.jpg", depth);
		CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		CPPUNIT_ASSERT_NO_THROW(tx.b3Copy(m_TxMap[depth]));

		tx.b3TransToGrey();
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3ImageTest::testTransToBlackWhite()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_bw_%03d.tiff", depth);
		CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		CPPUNIT_ASSERT(tx.b3TransToBW(m_TxMap[depth]));
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3ImageTest::testMirror()
{
	for (b3_res depth : m_TestDepth)
	{
		if (depth > 1)
		{
			b3Tx   src;
			b3Path path;

			path.b3Format("img_test_mirror_%03d.jpg", depth);
			CPPUNIT_ASSERT(src.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
			CPPUNIT_ASSERT_NO_THROW(src.b3Copy(m_TxMap[depth]));

			CPPUNIT_ASSERT_NO_THROW(src.b3MirrorHorizontal());
			CPPUNIT_ASSERT_NO_THROW(src.b3MirrorVertical());
			CPPUNIT_ASSERT_EQUAL(B3_OK, src.b3SaveImage(path));
		}
	}
}

void b3ImageTest::testHist()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_hist_%03d.jpg", depth);
		CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		CPPUNIT_ASSERT_NO_THROW(tx.b3Copy(m_TxMap[depth]));

		CPPUNIT_ASSERT_NO_THROW(tx.b3Histogramme());
	}
}

void b3ImageTest::testNegate()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_negate_%03d.jpg", depth);
		CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		CPPUNIT_ASSERT_NO_THROW(tx.b3Copy(m_TxMap[depth]));

		tx.b3Negate();
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3ImageTest::testTurnLeft()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_turn_left_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(tx.b3Copy(m_TxMap[depth]));

		tx.b3TurnLeft();
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3ImageTest::testTurn()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_turn_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(tx.b3Copy(m_TxMap[depth]));

		tx.b3Turn();
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3ImageTest::testTurnRight()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_turn_right_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(tx.b3Copy(m_TxMap[depth]));

		tx.b3TurnRight();
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3ImageTest::testRemoveBlackBorder()
{
	b3Path path;

	path.b3Format("img_test_rbb_%03d.tiff", m_TxBW.depth);
	m_TxBW.b3RemoveBlackBorder();
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxBW.b3SaveImage(path));
}

void b3ImageTest::testShrink()
{
	b3Path path;

	path.b3Format("img_test_shrink_%03d.tiff", m_TxBW.depth);
	m_TxBW.b3Shrink();
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxBW.b3SaveImage(path));
}

void b3ImageTest::testBlitSimple()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   dst;
		b3Path path;

		path.b3Format("img_test_blit_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(dst.b3Copy(&m_TxTrueColor));
		CPPUNIT_ASSERT_NO_THROW(dst.b3Blit(m_TxMap[depth],
				19, 27, 113, 127, 37, 41));
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));
	}
}

void b3ImageTest::testBlitClipped()
{
	for (b3_res depth : m_TestDepth)
	{
		const b3Tx * src = m_TxMap[depth];
		b3Tx         dst;
		b3Path       path;

		path.b3Format("img_test_blit_clipped1_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(dst.b3Copy(&m_TxTrueColor));
		CPPUNIT_ASSERT_NO_THROW(dst.b3Blit(src,
				dst.xSize - 83, dst.ySize - 91, 113, 127, 37, 41));
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));

		path.b3Format("img_test_blit_clipped2_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(dst.b3Copy(&m_TxTrueColor));
		CPPUNIT_ASSERT_NO_THROW(dst.b3Blit(src,
				-83, -91, 113, 127, 37, 41));
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));
	}
}

void b3ImageTest::testBlitOutside()
{
	for (b3_res depth : m_TestDepth)
	{
		const b3Tx * src = m_TxMap[depth];
		b3Tx         dst;
		b3Path       path;

		path.b3Format("img_test_blit_out1_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(dst.b3Copy(&m_TxTrueColor));
		CPPUNIT_ASSERT_NO_THROW(dst.b3Blit(src,
				dst.xSize + 83, dst.ySize + 91, 113, 127, 37, 41));
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));

		path.b3Format("img_test_blit_out2_%03d.jpg", depth);
		CPPUNIT_ASSERT_NO_THROW(dst.b3Copy(&m_TxTrueColor));
		CPPUNIT_ASSERT_NO_THROW(dst.b3Blit(src,
				83, 91, 113, 127, src->xSize + 151, src->ySize + 227));
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));
	}
}

void b3ImageTest::testContrast()
{
	for (b3_f64 gamma = 1.0; gamma <= 2.0; gamma += 0.125)
	{
		const b3Tx * src = &m_TxTrueColor;
		b3Tx         dst;
		b3Path       path;

		path.b3Format("img_test_contrast_%03d-%1.3lf-%03d.jpg", 24, gamma, 32);
		CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, 32));
		CPPUNIT_ASSERT(dst.b3TxContrast(0.75, 0.25, gamma, src));
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));
	}
}

void b3ImageTest::testGauss()
{
	for (b3_res depth : m_TestDepth)
	{
		const b3Tx * src = m_TxMap[depth];
		b3Tx   dst;

		if (src->depth >= 8)
		{
			b3Path path;

			path.b3Format("img_test_gauss_%03d-20.jpg", depth);
			CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, 32));
			CPPUNIT_ASSERT(dst.b3TxGauss(src->xSize >> 1, src->ySize >> 1, 1.0, 0.5, 0.0, 0.0, src));
			CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));

			path.b3Format("img_test_gauss_%03d-80.jpg", depth);
			CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, 128));
			CPPUNIT_ASSERT(dst.b3TxGauss(src->xSize >> 1, src->ySize >> 1, 1.0, 0.5, 0.0, 0.0, src));
			CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));
		}
		else
		{
			CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, 128));
			CPPUNIT_ASSERT_THROW(
				dst.b3TxGauss(src->xSize >> 1, src->ySize >> 1, 0.25, 1.0, 0.0, 0.0, src),
				b3TxException);
		}
	}
}

void b3ImageTest::testFilter()
{
	for (b3_res dst_depth : m_TestDepth)
	{
		for (b3_res depth : m_TestDepth)
		{
			const b3Tx * src = m_TxMap[depth];
			b3Tx         dst;
			b3Path       path;

			if ((dst_depth > 8) || ((dst_depth <= 8) && (depth <= 8) && (dst_depth != 1) && (depth != 1)))
			{
				path.b3Format("img_test_filter_n_%03d_%03d.jpg", depth, dst_depth);
				CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, dst_depth));
				CPPUNIT_ASSERT(dst.b3TxColorFilter(1.0, 1.0, 1.0, src));
				CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));

				path.b3Format("img_test_filter_r_%03d_%03d.jpg", depth, dst_depth);
				CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, dst_depth));
				CPPUNIT_ASSERT(dst.b3TxColorFilter(1.0, 0.25, 0.25, src));
				CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));

				path.b3Format("img_test_filter_g_%03d_%03d.jpg", depth, dst_depth);
				CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, dst_depth));
				CPPUNIT_ASSERT(dst.b3TxColorFilter(0.25, 1.0, 0.25, src));
				CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));

				path.b3Format("img_test_filter_b_%03d_%03d.jpg", depth, dst_depth);
				CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, dst_depth));
				CPPUNIT_ASSERT(dst.b3TxColorFilter(0.25, 0.25, 1.0, src));
				CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));

				path.b3Format("img_test_filter_o_%03d_%03d.jpg", depth, dst_depth);
				CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, dst_depth));
				CPPUNIT_ASSERT(dst.b3TxColorFilter(2.0, 2.0, 2.0, src));
				CPPUNIT_ASSERT_EQUAL(B3_OK, dst.b3SaveImage(path));
			}
			else
			{
				CPPUNIT_ASSERT(dst.b3AllocTx(src->xSize, src->ySize, dst_depth));
				CPPUNIT_ASSERT(!dst.b3TxColorFilter(1.0, 1.0, 1.0, src));
			}
		}
	}
}

void b3ImageTest::compareImages(
	const b3Tx & src,
	const bool   compare_meta,
	const bool   compare_data)
{
	b3Tx tx;

	CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3LoadImage(src.b3GetFilename()));
	compareImages(&src, tx, compare_meta, compare_data);
}

void b3ImageTest::compareImages(
	const b3Tx * src,
	const b3Tx & dst,
	const bool   compare_meta,
	const bool   compare_data)
{
	CPPUNIT_ASSERT_EQUAL(src->xSize, dst.xSize);
	CPPUNIT_ASSERT_EQUAL(src->ySize, dst.ySize);

	if (compare_meta)
	{
		CPPUNIT_ASSERT_EQUAL(
			src->b3GetPalette() != nullptr, dst.b3GetPalette() != nullptr);
		CPPUNIT_ASSERT_EQUAL(src->b3IsLoaded(),      dst.b3IsLoaded());
		CPPUNIT_ASSERT_EQUAL(src->b3IsBW(),          dst.b3IsBW());
		CPPUNIT_ASSERT_EQUAL(src->b3IsGreyPalette(), dst.b3IsGreyPalette());
		CPPUNIT_ASSERT_EQUAL(src->b3IsHighColor(),   dst.b3IsHighColor());
		CPPUNIT_ASSERT_EQUAL(src->b3IsTrueColor(),   dst.b3IsTrueColor());
		CPPUNIT_ASSERT_EQUAL(src->b3IsHdr(),         dst.b3IsHdr());
		CPPUNIT_ASSERT_EQUAL(src->b3IsPalette(),     dst.b3IsPalette());
	}

	if (compare_data)
	{
		for (b3_res y = 0; y < src->ySize; y++)
		{
			for (b3_res x = 0; x < src->xSize; x++)
			{

				CPPUNIT_ASSERT_EQUAL(src->b3GetValue(x, y),    dst.b3GetValue(x, y));

				if (src->b3IsHdr() && dst.b3IsHdr())
				{
					const b3Color & src_color = src->b3GetHdrValue(x, y);
					const b3Color & dst_color = dst.b3GetHdrValue(x, y);

					CPPUNIT_ASSERT(src_color == dst_color);
				}
			}
		}
	}
}

#endif
