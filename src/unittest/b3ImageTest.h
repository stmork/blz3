/*
**
**	$Filename:	b3ImageTest.h $
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

#ifndef B3_IMAGE_TEST_H
#define B3_IMAGE_TEST_H

#include "blz3/image/b3Tx.h"

#ifdef HAVE_LIBCPPUNIT

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

#define TEST_IMG_XMAX 320
#define TEST_IMG_YMAX 240

class b3ImageTest : public CppUnit::TestFixture
{
	b3Tx m_TxGIF;
	b3Tx m_TxPallColor;
	b3Tx m_TxTrueColor;
	b3Tx m_TxRealColor;

	static const b3_res m_TestDepth[];

	CPPUNIT_TEST_SUITE(b3ImageTest);
	CPPUNIT_TEST(testTxData);
	CPPUNIT_TEST(testReadGIF);
	CPPUNIT_TEST(testWriteTIFF);
	CPPUNIT_TEST(testWriteJPEG);
	CPPUNIT_TEST(testWriteTGA);
	CPPUNIT_TEST(testWriteRGB8);
	CPPUNIT_TEST(testWritePS);
	CPPUNIT_TEST(testScaleUnfiltered);
	CPPUNIT_TEST(testScaleFiltered);
	CPPUNIT_TEST(testTransToGrey);

#ifdef BLZ3_USE_OPENEXR
	CPPUNIT_TEST(testWriteOpenEXR);
#endif
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() override
	{
		b3Color        row[TEST_IMG_XMAX];
		b3_pkd_color * tRow;
		b3_color   *   rRow;
		b3_res         width   = TEST_IMG_XMAX >> 3;

		b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
		m_TxPallColor.b3AllocTx(TEST_IMG_XMAX, TEST_IMG_YMAX,   8); // 8 bit palette entry
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

		tRow = m_TxTrueColor.b3GetTrueColorData();
		rRow = m_TxRealColor.b3GetHdrData();
		for (b3_res y = 0; y < TEST_IMG_YMAX; y++)
		{
			for (b3_res x = 0; x < TEST_IMG_XMAX; x++)
			{
				*tRow++ = row[x];
				*rRow++ = row[x];
			}
		}
	}

	void tearDown() override
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void testReadGIF()
	{
		m_TxGIF.b3LoadImage("fft_test.gif", true);
	}

	void testWriteTIFF()
	{
		m_TxPallColor.b3SaveImage("img_test_08.tiff");
		m_TxTrueColor.b3SaveImage("img_test_20.tiff");
		m_TxRealColor.b3SaveImage("img_test_80.tiff");
	}

	void testWriteJPEG()
	{
		m_TxPallColor.b3SaveImage("img_test_08.jpg");
		m_TxTrueColor.b3SaveImage("img_test_20.jpg");
		m_TxRealColor.b3SaveImage("img_test_80.jpg");
	}

	void testWriteTGA()
	{
		m_TxPallColor.b3SaveImage("img_test_08.tga");
		m_TxTrueColor.b3SaveImage("img_test_20.tga");
		m_TxRealColor.b3SaveImage("img_test_80.tga");
	}

	void testWriteRGB8()
	{
		m_TxPallColor.b3SaveImage("img_test_08.rgb8");
		m_TxTrueColor.b3SaveImage("img_test_20.rgb8");
		m_TxRealColor.b3SaveImage("img_test_80.rgb8");
	}

	void testWritePS()
	{
		m_TxPallColor.b3SaveImage("img_test_08.ps");
		m_TxTrueColor.b3SaveImage("img_test_20.ps");
		m_TxRealColor.b3SaveImage("img_test_80.ps");
	}

#ifdef BLZ3_USE_OPENEXR
	void testWriteOpenEXR()
	{
		m_TxRealColor.b3SaveImage("img_test_80.exr");
	}
#endif

	void testTxData();
	void testScaleUnfiltered();
	void testScaleFiltered();
	void testTransToGrey();
};

#endif
#endif
