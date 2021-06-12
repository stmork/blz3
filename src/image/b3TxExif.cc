/*
**
**	$Filename:	b3TxExif.cc $
**	$Release:	Dortmund 20021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving JPEG image
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

#include "blz3/system/b3Date.h"
#include "blz3/system/b3Runtime.h"
#include "blz3/image/b3TxExif.h"

/*************************************************************************
**                                                                      **
**                        EXIF implementation                           **
**                                                                      **
*************************************************************************/

const char * b3TxExif::m_DateTimeFormat = "%04d:%02u:%02u %02u:%02u:%02u";

b3TxExif::b3TxExif(const char * filename)
{
#ifdef HAVE_LIBEXIV2
	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);

	image->readMetadata();
	m_ExifData = image->exifData();
#endif
}

b3TxExif::b3TxExif()
{
#ifdef HAVE_LIBEXIV2
	b3Date now;
	char   hostname[32];
	char   copyright[128];
	char   date_time[64];

	b3Runtime::b3Hostname(hostname, sizeof(hostname));

	snprintf(copyright, sizeof(copyright), "Copyright (C) %s, %u",
		b3Runtime::b3GetUserName(), now.year);
	snprintf(date_time, sizeof(date_time), m_DateTimeFormat,
		now.year, now.month, now.day, now.hour, now.min, now.sec);
	m_ExifData["Exif.Image.Make"]             = b3Runtime::b3GetVendor();
	m_ExifData["Exif.Image.Model"]            = b3Runtime::b3GetProduct();
	m_ExifData["Exif.Image.Software"]         = b3Runtime::b3GetProduct();
	m_ExifData["Exif.Image.Artist"]           = b3Runtime::b3GetUserName();
	m_ExifData["Exif.Image.HostComputer"]     = hostname;
	m_ExifData["Exif.Image.Copyright"]        = copyright;
	m_ExifData["Exif.Image.SamplesPerPixel"]  = uint16_t(3);
	m_ExifData["Exif.Image.Orientation"]      = uint16_t(1);
	m_ExifData["Exif.Image.DateTime"]         = date_time;
	m_ExifData["Exif.Image.DateTimeOriginal"] = date_time;
	m_ExifData["Exif.Image.TimeZoneOffset"]   = short(1 + now.dls);
	m_ExifData["Exif.Photo.DateTimeOriginal"] = date_time;
	m_ExifData["Exif.Photo.ColorSpace"]       = 1;
#endif
}

void b3TxExif::b3Write(const char * filename)
{
#ifdef HAVE_LIBEXIV2
	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);

	image->setExifData(m_ExifData);
	image->writeMetadata();
#endif
}
