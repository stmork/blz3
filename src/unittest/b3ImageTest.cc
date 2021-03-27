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
#include <blz3/base/b3FileList.h>
#include <blz3/image/b3TxPool.h>

#include <array>
#include <regex>

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

b3_pkd_color b3ImageTest::data_u32[]
{
	B3_BLACK, B3_DARK_GREY, B3_GREY, B3_LIGHT_GREY, B3_WHITE,
	B3_YELLOW, B3_GREEN, B3_CYAN, B3_RED, B3_MAGENTA, B3_BLUE
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

void b3ImageTest::setUp()
{
	b3Color        row[TEST_IMG_XMAX];
	b3_pkd_color * tRow;
	b3_u16    *    sRow;
	b3_color   *   rRow;
	b3_res         width   = TEST_IMG_XMAX >> 3;

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
	m_TxPallColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,   8); // 8 bit palette entry
	m_TxHighColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,  16); // High color, 4 bit per color
	m_TxTrueColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,  32); // True color, 8 bit per color
	m_TxRealColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, 128); // Real color, 32 bit per color (floating point)

	for (b3_res x = 0; x < TEST_IMG_XMAX; x++)
	{
		b3_f32 r, g, b, value;
		b3_index idx = x / width;

		value = (b3_f32)(x % width) / width;
		b = idx & 1 ? value : 0.0;
		r = idx & 2 ? value : 0.0;
		g = idx & 4 ? value : 0.0;
		row[x].b3Init(r, g, b);
	}

	sRow = m_TxHighColor.b3GetHighColorData();
	tRow = m_TxTrueColor.b3GetTrueColorData();
	rRow = m_TxRealColor.b3GetHdrData();
	for (b3_res y = 0; y < TEST_IMG_YMAX; y++)
	{
		for (b3_res x = 0; x < TEST_IMG_XMAX; x++)
		{
			b3_u16 high_color = 0;

			for (b3_index i = 0; i < 4; i++)
			{
				high_color = (high_color << 4) | b3_u16(row[x][i] * 15);
			}
			*sRow++ = high_color;
			*tRow++ = row[x];
			*rRow++ = row[x];
		}
	}
}

void b3ImageTest::testRead()
{
	static std::regex  regex(R"(Lenna-([012][01248])\.[a-z]+)");
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
}

void b3ImageTest::testWriteTIFF()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.tiff"));
//	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.tiff"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.tiff"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.tiff"));

	compareImages(m_TxPallColor);
	compareImages(m_TxTrueColor);
}

void b3ImageTest::testWriteJPEG()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.jpg"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.jpg"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.jpg"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.jpg"));

	// No comparison because JPEG is not lossless.
}

void b3ImageTest::testWriteTGA()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.tga"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.tga"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.tga"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.tga"));

	compareImages(m_TxPallColor);
	compareImages(m_TxTrueColor);
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

void b3ImageTest::testWritePS()
{
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxPallColor.b3SaveImage("img_test_08.ps"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxHighColor.b3SaveImage("img_test_10.ps"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxTrueColor.b3SaveImage("img_test_20.ps"));
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_TxRealColor.b3SaveImage("img_test_80.ps"));
}

void b3ImageTest::testWriteOpenEXR()
{
#ifdef BLZ3_USE_OPENEXR
	m_TxRealColor.b3SaveImage("img_test_80.exr");

	compareImages(m_TxRealColor);
#endif
}

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
	CPPUNIT_ASSERT_EQUAL(0.0f,  b3Tx::b3ToGrey(B3_BLACK));
	CPPUNIT_ASSERT_EQUAL(1.0f,  b3Tx::b3ToGrey(B3_WHITE));
	CPPUNIT_ASSERT_EQUAL(0.35f, b3Tx::b3ToGrey(B3_RED));
	CPPUNIT_ASSERT_EQUAL(0.51f, b3Tx::b3ToGrey(B3_GREEN));
	CPPUNIT_ASSERT_EQUAL(0.14f, b3Tx::b3ToGrey(B3_BLUE));
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

	bw.b3AllocTx(4, 1, 1);
	bw.b3SetData(data_bw, TX_BWA(sizeof(data_bw)));
	CPPUNIT_ASSERT( bw.b3IsBW());
	CPPUNIT_ASSERT( bw.b3IsPalette());
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
	CPPUNIT_ASSERT( vga.b3IsPalette());
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
	CPPUNIT_ASSERT( high.b3IsHighColor());
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
	CPPUNIT_ASSERT( color.b3IsTrueColor());
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
	CPPUNIT_ASSERT( hdr.b3IsTrueColor());
	CPPUNIT_ASSERT( hdr.b3IsHdr());
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

	bw.b3AllocTx(4, 1, 1);
	bw.b3SetData(data_bw, TX_BWA(sizeof(data_bw)));
	bw.b3GetRow(row_u32, 0);
	bw.b3GetRow(row_col, 0);

	vga.b3AllocTx(DATA_SIZE(data_vga), 1, 8);
	vga.b3SetData(data_vga, sizeof(data_vga));
	vga.b3SetPalette(data_u32, DATA_SIZE(data_u32));
	vga.b3GetRow(row_u32, 0);
	vga.b3GetRow(row_col, 0);
	for (i = 0; i < hdr.xSize; i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], row_u32[i]);
		CPPUNIT_ASSERT(data_col[i] == row_col[i]);
	}

	high.b3AllocTx(DATA_SIZE(data_u16), 1, 12);
	high.b3SetData(data_u16, sizeof(data_u16));
	high.b3GetRow(row_u32, 0);
	high.b3GetRow(row_col, 0);

	color.b3AllocTx(DATA_SIZE(data_u32), 1, 24);
	color.b3SetData(data_u32, sizeof(data_u32));
	color.b3GetRow(row_u32, 0);
	color.b3GetRow(row_col, 0);
	for (i = 0; i < hdr.xSize; i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], row_u32[i]);
		CPPUNIT_ASSERT(data_col[i] == row_col[i]);
	}

	hdr.b3AllocTx(DATA_SIZE(data_col), 1, 128);
	hdr.b3SetData(data_col, sizeof(data_col));
	hdr.b3GetRow(row_u32, 0);
	hdr.b3GetRow(row_col, 0);
	for (i = 0; i < hdr.xSize; i++)
	{
		CPPUNIT_ASSERT_EQUAL(data_u32[i], row_u32[i]);
		CPPUNIT_ASSERT(data_col[i] == row_col[i]);
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

		src.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth);

		path.b3Format("img_test_scale_us_%03ld.jpg", depth);
		dst_smaller.b3AllocTx(src.xSize * 0.8, src.ySize * 0.8, depth);
		dst_smaller.b3Scale(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_smaller.b3SaveImage(path));

		path.b3Format("img_test_scale_ub_%03ld.jpg", depth);
		dst_bigger.b3AllocTx(src.xSize * 1.4, src.ySize * 1.4, depth);
		dst_bigger.b3Scale(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_bigger.b3SaveImage(path));
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

		src.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth);

		path.b3Format("img_test_scale_fs_%03ld-%03ld.jpg", depth, depth);
		CPPUNIT_ASSERT(dst_smaller.b3AllocTx(src.xSize * 0.8, src.ySize * 0.8, 32));
		dst_smaller.b3ScaleToGrey(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_smaller.b3SaveImage(path));

		path.b3Format("img_test_scale_fb_%03ld-%03ld.jpg", depth, depth);
		CPPUNIT_ASSERT(dst_bigger.b3AllocTx(src.xSize * 1.4, src.ySize * 1.4, 32));
		dst_bigger.b3ScaleToGrey(&src);
		CPPUNIT_ASSERT_EQUAL(B3_OK, dst_bigger.b3SaveImage(path));
	}

	b3Tx   bw;
	b3Tx   vga_smaller;
	b3Tx   vga_bigger;
	b3Path path;

	bw.b3AllocTx(50, 50, 1);

	path.b3Format("img_test_scale_fs_%03ld-%03ld.jpg", bw.depth, vga_smaller.depth);
	CPPUNIT_ASSERT(vga_bigger.b3AllocTx(bw.xSize * 0.8, bw.ySize * 0.8, 32));
	vga_bigger.b3ScaleToGrey(&bw);
	CPPUNIT_ASSERT_EQUAL(B3_OK, vga_bigger.b3SaveImage(path));

	path.b3Format("img_test_scale_fb_%03ld-%03ld.jpg", bw.depth, vga_bigger.depth);
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

		path.b3Format("img_test_grey_%03ld.jpg", depth);
		CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		tx.b3TransToGrey();
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3ImageTest::testMirror()
{
	for (b3_res depth : m_TestDepth)
	{
		if (depth > 1)
		{
			b3Tx   tx;
			b3Path path;

			path.b3Format("img_test_mirror_%03ld.jpg", depth);
			CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
			tx.b3MirrorHorizontal();
			tx.b3MirrorVertical();
			CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
		}
	}
}

void b3ImageTest::testHist()
{
	for (b3_res depth : m_TestDepth)
	{
		b3Tx   tx;
		b3Path path;

		path.b3Format("img_test_hist_%03ld.jpg", depth);
		CPPUNIT_ASSERT(tx.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX, depth));
		CPPUNIT_ASSERT(tx.b3Histogramme());
	}
}

void b3ImageTest::compareImages(const b3Tx & src)
{
	b3Tx tx;

	CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3LoadImage(src.b3GetFilename()));
	CPPUNIT_ASSERT_EQUAL(tx.xSize, src.xSize);
	CPPUNIT_ASSERT_EQUAL(tx.ySize, src.ySize);

	for (b3_res y = 0; y < tx.ySize; y++)
	{
		for (b3_res x = 0; x < tx.xSize; x++)
		{
			CPPUNIT_ASSERT_EQUAL(src.b3GetValue(x, y),    tx.b3GetValue(x, y));
			CPPUNIT_ASSERT_EQUAL(src.b3GetHdrValue(x, y), tx.b3GetHdrValue(x, y));
		}
	}
}

#endif
