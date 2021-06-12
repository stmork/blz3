/*
**
**	$Filename:	b3TxExif.h $
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

#pragma once

#ifndef B3_TXEXIF_H
#define B3_TXEXIF_H

#include "blz3/b3Config.h"

#ifdef HAVE_LIBEXIV2
#include <exiv2/exiv2.hpp>
#endif

/**
 * This class maintains EXIF related data.
 */
class b3TxExif
{
protected:
	static const char * m_DateTimeFormat;

#ifdef HAVE_LIBEXIV2
	Exiv2::ExifData     m_ExifData;
#endif

public:
	/**
	 * This method reads EXIF data from the existing image filename.
	 *
	 * @param filename The image filename to read EXIF data from.
	 */
	explicit b3TxExif(const char * filename);

	/**
	 * The copy contructor copies EXIF data from another instance.
	 *
	 * @param other The other b3TxExif instance to copy EXIF data from.
	 */
	explicit b3TxExif(const b3TxExif & other);

	/**
	 * The default constructor fills in some default values into the EXIF
	 * data.
	 */
	b3TxExif();

	/**
	 * This method updates the processing date and time.
	 */
	void b3Update();

	/**
	 * This method removes all geotagging related information from the EXIF
	 * data.
	 */
	void b3RemoveGpsData();

	/**
	 * This method checks whether the EXIF data contains and geo tagging
	 * related information.
	 *
	 * @return True if EXIF data contains geo tagging related information.
	 */
	bool b3HasGpsData() const;

	/**
	 * This method writes the EXIF data into the specified file. The image
	 * file must exist.
	 *
	 * @param filename The image filename to insert the EXIF data.
	 */
	void b3Write(const char * filename);

private:
	const char * b3PrepareDate(char * date_time, const size_t size);
};

#endif // B3TXEXIF_H
