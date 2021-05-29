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

#include <unordered_map>

#include "b3ImageSetup.h"

class b3ImageTest : public CppUnit::TestFixture, protected b3ImageSetup
{
	static       b3_pkd_color data_u32[];
	static       b3_u16       data_u16[];
	static       b3_u08       data_bw[];
	static       b3_u08       data_vga[];
	static       b3_color     data_col[];

	static const b3_res m_TestDepth[];
	std::unordered_map<b3_res, const b3Tx *>  m_TxMap;

	CPPUNIT_TEST_SUITE(b3ImageTest);
	CPPUNIT_TEST(testPattern);
	CPPUNIT_TEST(testTxData);
	CPPUNIT_TEST(testColor);
	CPPUNIT_TEST(testPixel);
	CPPUNIT_TEST(testRow);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testCopy);

	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testReadGIF);
	CPPUNIT_TEST(testWriteTIFF);
	CPPUNIT_TEST(testWriteJPEG);
	CPPUNIT_TEST(testWriteTGA);
	CPPUNIT_TEST(testWriteRGB8);
	CPPUNIT_TEST(testWritePS);
#ifdef BLZ3_USE_OPENEXR
	CPPUNIT_TEST(testWriteOpenEXR);
#endif

	CPPUNIT_TEST(testScaleUnfiltered);
	CPPUNIT_TEST(testScaleFiltered);
	CPPUNIT_TEST(testTransToGrey);
	CPPUNIT_TEST(testTransToBlackWhite);
	CPPUNIT_TEST(testMirror);
	CPPUNIT_TEST(testHist);
	CPPUNIT_TEST(testNegate);
	CPPUNIT_TEST(testTurnLeft);
	CPPUNIT_TEST(testTurn);
	CPPUNIT_TEST(testTurnRight);
	CPPUNIT_TEST(testRemoveBlackBorder);
	CPPUNIT_TEST(testShrink);
	CPPUNIT_TEST(testBlitSimple);
	CPPUNIT_TEST(testBlitClipped);
	CPPUNIT_TEST(testBlitOutside);
	CPPUNIT_TEST(testContrast);
	CPPUNIT_TEST(testGauss);
	CPPUNIT_TEST(testFilter);

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

	void testPattern();
	void testTxData();
	void testColor();
	void testPixel();
	void testRow();
	void testEmpty();
	void testCopy();

	void testRead();
	void testReadGIF();
	void testWriteTIFF();
	void testWriteJPEG();
	void testWriteTGA();
	void testWriteRGB8();
	void testWritePS();
#ifdef BLZ3_USE_OPENEXR
	void testWriteOpenEXR();
#endif

	void testScaleUnfiltered();
	void testScaleFiltered();
	void testTransToGrey();
	void testTransToBlackWhite();
	void testMirror();
	void testHist();
	void testNegate();
	void testTurnLeft();
	void testTurn();
	void testTurnRight();
	void testRemoveBlackBorder();
	void testShrink();
	void testBlitSimple();
	void testBlitClipped();
	void testBlitOutside();
	void testContrast();
	void testGauss();
	void testFilter();

private:
	void compareImages(
		const b3Tx & src,
		const bool   compare_meta = true,
		const bool   compare_data = true);
	static void compareImages(
		const b3Tx * src,
		const b3Tx & dst,
		const bool   compare_meta = false,
		const bool   compare_data = true);
};

#endif
#endif
