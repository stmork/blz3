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
	m_xDPI     = m_ExifData["Exif.Image.XResolution"].toRational().first;
	m_yDPI     = m_ExifData["Exif.Image.YResolution"].toRational().first;
#endif
}

b3TxExif::b3TxExif(const b3TxExif & other)
{
	m_ExifData = other.m_ExifData;
	m_xDPI     = m_ExifData["Exif.Image.XResolution"].toRational().first;
	m_yDPI     = m_ExifData["Exif.Image.YResolution"].toRational().first;
}

b3TxExif::b3TxExif()
{
#ifdef HAVE_LIBEXIV2
	b3Date now;
	char   copyright[128];
	char   date_time[64];

	b3PrepareDate(date_time, sizeof(date_time));
	snprintf(copyright, sizeof(copyright), "Copyright (C) %s, %d",
		b3Runtime::b3GetUserName(), now.year);

	m_ExifData["Exif.Image.Make"]             = b3Runtime::b3GetVendor();
	m_ExifData["Exif.Image.Model"]            = b3Runtime::b3GetProduct();
	m_ExifData["Exif.Image.Software"]         = b3Runtime::b3GetProduct();
	m_ExifData["Exif.Image.Copyright"]        = copyright;
	m_ExifData["Exif.Image.Orientation"]      = uint16_t(1);
	m_ExifData["Exif.Image.DateTimeOriginal"] = date_time;
	m_ExifData["Exif.Image.TimeZoneOffset"]   = short(m_UtcOffset / 60);

	m_ExifData["Exif.Photo.DateTimeOriginal"] = date_time;
	m_ExifData["Exif.Photo.ColorSpace"]       = uint16_t(1);
#endif
	b3SetResolution(72, 72);
	b3Update();
}

void b3TxExif::b3Update()
{
#ifdef HAVE_LIBEXIV2
	char date_time[64];

	b3PrepareDate(date_time, sizeof(date_time));
	b3Runtime::b3Hostname(m_Hostname, sizeof(m_Hostname));

	m_ExifData["Exif.Image.HostComputer"] = m_Hostname;
	m_ExifData["Exif.Image.Software"]     = b3Runtime::b3GetProduct();
	m_ExifData["Exif.Image.Artist"]       = b3Runtime::b3GetUserName();
	m_ExifData["Exif.Image.DateTime"]     = date_time;
#endif
}

void b3TxExif::b3GetResolution(b3_res & xDPI, b3_res & yDPI) const
{
	xDPI = m_xDPI;
	yDPI = m_yDPI;
}

void b3TxExif::b3SetResolution(const b3_res xDPI, const b3_res yDPI)
{
	m_xDPI = xDPI;
	m_yDPI = yDPI;

	m_ExifData["Exif.Image.XResolution"]    = Exiv2::URational(m_xDPI, 1);
	m_ExifData["Exif.Image.YResolution"]    = Exiv2::URational(m_yDPI, 1);
	m_ExifData["Exif.Image.ResolutionUnit"] = uint16_t(2);

	m_ExifData["Exif.Photo.FocalPlaneXResolution"]    = Exiv2::URational(m_xDPI, 1);;
	m_ExifData["Exif.Photo.FocalPlaneYResolution"]    = Exiv2::URational(m_yDPI, 1);;
	m_ExifData["Exif.Photo.FocalPlaneResolutionUnit"] = uint16_t(2);
}

void b3TxExif::b3SetQuality(const unsigned quality)
{
	m_ExifData["Exif.Image.BestQualityScale"] = Exiv2::URational(quality, 100);
}

void b3TxExif::b3RemoveGpsData()
{
#ifdef HAVE_LIBEXIV2
	Exiv2::ExifData::iterator it = m_ExifData.begin();

	while (it != m_ExifData.end())
	{
		const Exiv2::Exifdatum & datum = *it;
		const std::string & group = datum.groupName();

		if (group == "GPSInfo")
		{
			it = m_ExifData.erase(it);
		}
		else
		{
			++it;
		}
	}
#endif
}

bool b3TxExif::b3HasGpsData() const
{
#ifdef HAVE_LIBEXIV2
	b3_count gps_count = 0;

	for (const Exiv2::Exifdatum & datum : m_ExifData)
	{
		if (datum.groupName() == "GPSInfo")
		{
			gps_count++;
		}
	}
	return gps_count > 0;
#else
	return false;
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

const char * b3TxExif::b3PrepareDate(char * date_time, const size_t size)
{
	const std::time_t      current_time = std::time(nullptr);
	const struct std::tm * timeinfo     = std::localtime(&current_time);
	const b3Date           now;

	m_UtcOffset = timeinfo->tm_gmtoff / 60;

	snprintf(date_time, size, m_DateTimeFormat,
		now.year, now.month, now.day, now.hour, now.min, now.sec);

	return date_time;
}
