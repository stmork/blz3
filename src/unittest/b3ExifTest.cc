/*
**
**	$Filename:	b3ExifTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Tests for EXIF.
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

#include "blz3/system/b3Date.h"
#include "blz3/system/b3Runtime.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"
#include "blz3/raytrace/b3RaytraceExif.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3ExifTest);

void b3ExifTest::setUp()
{
	b3Path WORKSPACE(getenv("WORKSPACE"));
	b3Path BLZ3_HOME(getenv("BLZ3_HOME"));

	m_Tx.b3AllocTx(320, 240, 32);
	m_Tx.b3TestPattern();

	WORKSPACE.b3Append("src/unittest");
	BLZ3_HOME.b3Append("src/unittest");

	m_SearchPath.b3AddPath(WORKSPACE);
	m_SearchPath.b3AddPath(BLZ3_HOME);

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3ExifTest::tearDown()
{
	m_World.b3Free();
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3ExifTest::testSimpleJpeg()
{
	const char   *  image_filename = "img_exif1_20.jpg";
	b3TxExif        exif;

	CPPUNIT_ASSERT_EQUAL(B3_OK, m_Tx.b3SaveJPEG(image_filename));
	CPPUNIT_ASSERT(!exif.b3HasGpsData());
	CPPUNIT_ASSERT_NO_THROW(exif.b3Write(image_filename));
}

void b3ExifTest::testSimpleTiff()
{
	const char   *  image_filename = "img_exif1_20.tif";
	b3TxExif        exif;

	CPPUNIT_ASSERT_EQUAL(B3_OK, m_Tx.b3SaveTIFF(image_filename));
	CPPUNIT_ASSERT(!exif.b3HasGpsData());
	CPPUNIT_ASSERT_NO_THROW(exif.b3Write(image_filename));
}

void b3ExifTest::testRaytrace()
{
	b3ShapeRenderContext context;
	b3Path               full_path;
	unsigned             i = 0;

	CPPUNIT_ASSERT(m_SearchPath.b3IsValid("Data1.bwd", full_path));
	CPPUNIT_ASSERT(m_World.b3Read(full_path, false));

	for (b3Item * item  = m_World.b3GetFirst();
		item != nullptr;
		item  = item->Succ)
	{
		b3Scene * scene = static_cast<b3Scene *>(item);

		CPPUNIT_ASSERT(scene != nullptr);

		b3Display         display(scene->m_xSize, scene->m_ySize);
		b3Path            image_filename;
		b3RaytraceExif    exif;
		b3_vector         lower, upper;

		scene->b3Reorg();
		scene->b3SetupVertexMemory(&context);
		scene->b3ComputeBounds(&lower, &upper);

		CPPUNIT_ASSERT_NO_THROW(scene->b3Raytrace(&display, true));
		image_filename.b3Format("img_exif2_%02u_20.jpg", i++);

		CPPUNIT_ASSERT(display.b3SaveImage(image_filename));
		CPPUNIT_ASSERT_NO_THROW(exif.b3AddValues(scene));
		CPPUNIT_ASSERT_NO_THROW(exif.b3Write(image_filename));
	}
}

void b3ExifTest::testRemoveGps()
{
	b3TxExif exif("IMG_1344.jpeg");

	CPPUNIT_ASSERT(exif.b3HasGpsData());
	CPPUNIT_ASSERT_NO_THROW(exif.b3RemoveGpsData());
	CPPUNIT_ASSERT(!exif.b3HasGpsData());

	b3TxExif copy(exif);

	copy.b3GetResolution(m_Tx.xDPI, m_Tx.yDPI);
	CPPUNIT_ASSERT_NO_THROW(copy.b3Update());
	CPPUNIT_ASSERT_EQUAL(B3_OK, m_Tx.b3SaveJPEG("img_exif3_20.jpg", 85, &copy));
}

void b3ExifTest::testRational()
{
	Exiv2::Rational  a( 7,  2);
	Exiv2::Rational  b(-7,  4);
	Exiv2::Rational  c( 5, -2);
	Exiv2::URational d( 5, -2);
	Exiv2::URational e( 9, -4);
	Exiv2::URational f( 1,  0);

	CPPUNIT_ASSERT_EQUAL( 3.5,  b3TxExif::b3Quotient(a));
	CPPUNIT_ASSERT_EQUAL(-1.75, b3TxExif::b3Quotient(b));
	CPPUNIT_ASSERT_EQUAL(-2.5,  b3TxExif::b3Quotient(c));
	CPPUNIT_ASSERT_EQUAL(-2.5,  b3TxExif::b3Quotient(d));
	CPPUNIT_ASSERT_EQUAL(-2.25, b3TxExif::b3Quotient(e));
	CPPUNIT_ASSERT_EQUAL( 42.0, b3TxExif::b3Quotient(f, 42));

	CPPUNIT_ASSERT_EQUAL( 4,  b3TxExif::b3RoundedQuotient(a));
	CPPUNIT_ASSERT_EQUAL(-2,  b3TxExif::b3RoundedQuotient(b));
	CPPUNIT_ASSERT_EQUAL(-3,  b3TxExif::b3RoundedQuotient(c));
	CPPUNIT_ASSERT_EQUAL(-3,  b3TxExif::b3RoundedQuotient(d));
	CPPUNIT_ASSERT_EQUAL(-2,  b3TxExif::b3RoundedQuotient(e));
	CPPUNIT_ASSERT_EQUAL(19,  b3TxExif::b3RoundedQuotient(f, 19));
}

#endif
