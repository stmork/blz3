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

#include "blz3/system/b3Runtime.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

#ifdef HAVE_LIBEXIV2
#include <exiv2/exiv2.hpp>
#endif

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

void b3ExifTest::testSimple()
{
#ifdef HAVE_LIBEXIV2
	const char *    image_filename = "img_exif1_20.jpg";
	Exiv2::ExifData exif_data;

	CPPUNIT_ASSERT_EQUAL(B3_OK, m_Tx.b3SaveJPEG(image_filename));

	exif_data["Exif.Image.Model"] = b3Runtime::b3GetProduct();
	CPPUNIT_ASSERT_THROW(exif_data["Exif.Image.Author"] = b3Runtime::b3GetUserName(), std::exception);
	exif_data["Exif.Image.SamplesPerPixel"] = uint16_t(3);
	exif_data["Exif.Image.XResolution"] = 100;
	exif_data["Exif.Image.YResolution"] = 100;
	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(image_filename);

	CPPUNIT_ASSERT(image.get() != 0);

	image->setExifData(exif_data);
	image->writeMetadata();
#endif
}

void b3ExifTest::testRaytrace()
{
#ifdef HAVE_LIBEXIV2
	b3Path   full_path;
	unsigned i = 0;
	b3ShapeRenderContext context;

	CPPUNIT_ASSERT(m_SearchPath.b3IsValid("Data1.bwd", full_path));
	CPPUNIT_ASSERT(m_World.b3Read(full_path, false));

	for (b3Item * item  = m_World.b3GetFirst();
		item != nullptr;
		item  = item->Succ)
	{
		b3Scene * scene = static_cast<b3Scene *>(item);

		CPPUNIT_ASSERT(scene != nullptr);
		b3Animation *     animation = scene->b3GetAnimation(true);
		b3ModellerInfo *  info      = scene->b3GetModellerInfo();
		b3CameraPart *    camera    = scene->b3GetActualCamera();
		Exiv2::ExifData   exif_data;
		b3Display         display(scene->m_xSize, scene->m_ySize);
		b3Path            image_filename;
		b3_vector         lower, upper;

		CPPUNIT_ASSERT(animation != nullptr);
		CPPUNIT_ASSERT(info    != nullptr);
		CPPUNIT_ASSERT(camera != nullptr);

		const b3_f64 width  = b3Vector::b3Length(&camera->m_Width)  * 2.0;
		const b3_f64 height = b3Vector::b3Length(&camera->m_Height) * 2.0;
		const int    x_dpi  = std::round(width  / info   ->b3Scale(width,  B3_UNIT_IN));
		const int    y_dpi  = std::round(height / info   ->b3Scale(height, B3_UNIT_IN));

		exif_data["Exif.Image.Model"] = b3Runtime::b3GetProduct();
		exif_data["Exif.Image.SamplesPerPixel"] = uint16_t(3);
		exif_data["Exif.Image.XResolution"] = x_dpi;
		exif_data["Exif.Image.YResolution"] = y_dpi;

		scene->b3Reorg();
		scene->b3SetupVertexMemory(&context);
		scene->b3ComputeBounds(&lower, &upper);

		CPPUNIT_ASSERT_NO_THROW(scene->b3Raytrace(&display, true));
		image_filename.b3Format("img_exif2_%02u_20.jpg", i++);

		CPPUNIT_ASSERT(display.b3SaveImage(image_filename));
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(image_filename);

		CPPUNIT_ASSERT(image.get() != 0);

		image->setExifData(exif_data);
		image->writeMetadata();
	}
#endif
}

#endif
