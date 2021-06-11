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

void b3ExifTest::testSimple()
{
#ifdef HAVE_LIBEXIV2
	const char   *  image_filename = "img_exif1_20.jpg";

	CPPUNIT_ASSERT_EQUAL(B3_OK, m_Tx.b3SaveJPEG(image_filename));
	Exiv2::Image::AutoPtr image     = Exiv2::ImageFactory::open(image_filename);

	image->readMetadata();
	Exiv2::ExifData   &   exif_data = image->exifData();

	CPPUNIT_ASSERT_THROW(exif_data["Exif.Image.Author"] = b3Runtime::b3GetUserName(), std::exception);

	fill(exif_data);

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

		b3Display         display(scene->m_xSize, scene->m_ySize);
		b3Path            image_filename;
		b3_vector         lower, upper;

		scene->b3Reorg();
		scene->b3SetupVertexMemory(&context);
		scene->b3ComputeBounds(&lower, &upper);

		CPPUNIT_ASSERT_NO_THROW(scene->b3Raytrace(&display, true));
		image_filename.b3Format("img_exif2_%02u_20.jpg", i++);

		CPPUNIT_ASSERT(display.b3SaveImage(image_filename));
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(image_filename);

		image->readMetadata();
		Exiv2::ExifData & exif_data = image->exifData();

		CPPUNIT_ASSERT_NO_THROW(fill(exif_data, scene));
		CPPUNIT_ASSERT(image.get() != 0);

		image->setExifData(exif_data);
		image->writeMetadata();
	}
#endif
}

bool b3ExifTest::fill(Exiv2::ExifData & exif_data, b3Scene * scene)
{
	b3Date now;
	char   hostname[32];
	char   copyright[128];
	char   date_time[64];

	b3Runtime::b3Hostname(hostname, sizeof(hostname));
	CPPUNIT_ASSERT(strlen(hostname) > 0);
	CPPUNIT_ASSERT(strlen(hostname) < sizeof(hostname));

	snprintf(copyright, sizeof(copyright), "Copyright (C) %s, %u", b3Runtime::b3GetUserName(), now.year);
	snprintf(date_time, sizeof(date_time), "%04d:%02u:%02u %02u:%02u:%02u",
		now.year, now.month, now.day, now.hour, now.min, now.sec);
	exif_data["Exif.Image.Make"]             = "MORKNet";
	exif_data["Exif.Image.Model"]            = b3Runtime::b3GetProduct();
	exif_data["Exif.Image.Software"]         = b3Runtime::b3GetProduct();
	exif_data["Exif.Image.Artist"]           = b3Runtime::b3GetUserName();
	exif_data["Exif.Image.HostComputer"]     = hostname;
	exif_data["Exif.Image.Copyright"]        = copyright;
	exif_data["Exif.Image.SamplesPerPixel"]  = uint16_t(3);
	exif_data["Exif.Image.Orientation"]      = uint16_t(1);
	exif_data["Exif.Image.DateTime"]         = date_time;
	exif_data["Exif.Image.DateTimeOriginal"] = date_time;
	exif_data["Exif.Image.TimeZoneOffset"]   = short(1 + now.dls);
	exif_data["Exif.Photo.DateTimeOriginal"] = date_time;
	exif_data["Exif.Photo.ColorSpace"]       = 1;

	if (scene != nullptr)
	{
		b3Animation   *   animation = scene->b3GetAnimation(false);
		b3ModellerInfo  * info      = scene->b3GetModellerInfo();
		b3CameraPart   *  camera    = scene->b3GetActualCamera();

		exif_data["Exif.Image.ImageWidth"]  = scene->m_xSize;
		exif_data["Exif.Image.ImageLength"] = scene->m_ySize;

		CPPUNIT_ASSERT(info != nullptr);
		if (camera != nullptr)
		{
			const b3_f64 focal    = camera->b3GetFocalLength();
			const b3_f64 focal_mm = info->b3Scale(focal, B3_UNIT_MM);
			const b3_f64 width    = b3Vector::b3Length(&camera->m_Width)  * 2.0;
			const b3_f64 height   = b3Vector::b3Length(&camera->m_Height) * 2.0;
			const int    x_dpi    = std::round(width  / info->b3Scale(width,  B3_UNIT_IN));
			const int    y_dpi    = std::round(height / info->b3Scale(height, B3_UNIT_IN));
			const b3_f64 focal_scale = 35.0 / info->b3Scale(width, B3_UNIT_MM);

			exif_data["Exif.Image.XResolution"]     = x_dpi;
			exif_data["Exif.Image.YResolution"]     = y_dpi;
			exif_data["Exif.Image.ResolutionUnit"]  = uint16_t(2);
			exif_data["Exif.Image.FocalLength"]     = Exiv2::Rational(focal_mm, 1);

			exif_data["Exif.Photo.FocalPlaneXResolution"]    = x_dpi;
			exif_data["Exif.Photo.FocalPlaneYResolution"]    = y_dpi;
			exif_data["Exif.Photo.FocalPlaneResolutionUnit"] = uint16_t(2);
			exif_data["Exif.Photo.FocalLength"]              = Exiv2::Rational(focal_mm, 1);
			exif_data["Exif.Photo.FocalLengthIn35mmFilm"]    = Exiv2::Rational(focal_mm * focal_scale, 1);
			exif_data["Exif.Photo.SceneCaptureType"]         = width > height ? 1 : 2;
		}

		if (animation != nullptr)
		{
//			exif_data["Exif.Image.FrameRate"]    = Exiv2::Rational(animation->m_FramesPerSecond, 1);
			exif_data["Exif.Photo.ExposureTime"] = Exiv2::Rational(1, animation->m_FramesPerSecond);
		}
	}
	return true;
}

#endif
