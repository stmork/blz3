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

	static       b3_pkd_color data_u32[];
	static       b3_u16       data_u16[];
	static       b3_u08       data_bw[];
	static       b3_u08       data_vga[];
	static       b3_color     data_col[];

	static const b3_res m_TestDepth[];

	CPPUNIT_TEST_SUITE(b3ImageTest);
	CPPUNIT_TEST(testTxData);
	CPPUNIT_TEST(testPixel);
	CPPUNIT_TEST(testRow);
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
	void setUp() override;

	void tearDown() override
	{
		b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
	}

	void testReadGIF();
	void testWriteTIFF();
	void testWriteJPEG();
	void testWriteTGA();
	void testWriteRGB8();
	void testWritePS();
	void testWriteOpenEXR();

	void testTxData();
	void testPixel();
	void testRow();
	void testScaleUnfiltered();
	void testScaleFiltered();
	void testTransToGrey();

private:
	void compareImages(const b3Tx & src);
};

#endif
#endif
