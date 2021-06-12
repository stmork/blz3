/*
**
**	$Filename:	b3RaytraceExif.h $
**	$Release:	Dortmund 20021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Handling EXIF meta data from raytracing geometry.
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3RaytraceExif.h"
#include "blz3/raytrace/b3Scene.h"

b3RaytraceExif::b3RaytraceExif(const char * filename) : b3TxExif(filename)
{
}

b3RaytraceExif::b3RaytraceExif() : b3TxExif()
{
}

void b3RaytraceExif::b3AddValues(b3Scene * scene)
{
#ifdef HAVE_LIBEXIV2
	if (scene != nullptr)
	{
		b3Animation   *   animation = scene->b3GetAnimation(false);
		b3ModellerInfo  * info      = scene->b3GetModellerInfo();
		b3CameraPart   *  camera    = scene->b3GetActualCamera();

		B3_ASSERT(info != nullptr);
		if (camera != nullptr)
		{
			const b3_f64 focal    = camera->b3GetFocalLength();
			const b3_f64 focal_mm = info->b3Scale(focal, B3_UNIT_MM);
			const b3_f64 width    = b3Vector::b3Length(&camera->m_Width)  * 2.0;
			const b3_f64 height   = b3Vector::b3Length(&camera->m_Height) * 2.0;
			const b3_f64 focal_scale = 35.0 / info->b3Scale(width, B3_UNIT_MM);

			m_xDPI = std::round(width  / info->b3Scale(width,  B3_UNIT_IN));
			m_yDPI = std::round(height / info->b3Scale(height, B3_UNIT_IN));

			m_ExifData["Exif.Image.XResolution"]     = m_xDPI;
			m_ExifData["Exif.Image.YResolution"]     = m_yDPI;
			m_ExifData["Exif.Image.ResolutionUnit"]  = uint16_t(2);
			m_ExifData["Exif.Image.FocalLength"]     = Exiv2::Rational(focal_mm, 1);

			m_ExifData["Exif.Photo.FocalPlaneXResolution"]    = m_xDPI;
			m_ExifData["Exif.Photo.FocalPlaneYResolution"]    = m_yDPI;
			m_ExifData["Exif.Photo.FocalPlaneResolutionUnit"] = uint16_t(2);
			m_ExifData["Exif.Photo.FocalLength"]              = Exiv2::Rational(focal_mm, 1);
			m_ExifData["Exif.Photo.FocalLengthIn35mmFilm"]    = Exiv2::Rational(focal_mm * focal_scale, 1);
			m_ExifData["Exif.Photo.SceneCaptureType"]         = width > height ? 1 : 2;
		}

		if (animation != nullptr)
		{
//			exif_data["Exif.Image.FrameRate"]    = Exiv2::Rational(animation->m_FramesPerSecond, 1);
			m_ExifData["Exif.Photo.ExposureTime"] = Exiv2::Rational(1, animation->m_FramesPerSecond);
		}
	}
#endif
}

