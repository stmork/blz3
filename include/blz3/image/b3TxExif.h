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
#include "blz3/system/b3Date.h"

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
	char                m_Hostname[32];
	b3_res              m_xDPI      = 72;
	b3_res              m_yDPI      = 72;
	signed              m_UtcOffset =  0;

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

	b3TxExif & operator=(const b3TxExif & other);

	/**
	 * This method updates the processing date and time.
	 */
	void b3Update();

	/**
	 * This method returns the pixel density in pixel per inch in horizontal
	 * and vertical direction.
	 *
	 * @param xDPI The horizontal pixel per inch.
	 * @param yDPI The vertical pixel per inch.
	 */
	void b3GetResolution(b3_res & xDPI, b3_res & yDPI) const;

	/**
	 * This method sets the horizontal and vertical pixel density in pixel
	 * per inch.
	 *
	 * @param xDPI The horizontal pixel per inch to set.
	 * @param yDPI The vertical pixel per inch to set.
	 */
	void b3SetResolution(const b3_res xDPI, const b3_res yDPI);

	/**
	 * This sets the image compression quality for JPEG images. @note This
	 * value has only informational character since the real image quality is
	 * set during the b3Tx::b3SaveJPEG() method.
	 *
	 * @param quality The compression quality to set.
	 */
	void b3SetQuality(const unsigned quality);

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

	/**
	 * This method computes the quotient of the given rational. In case the
	 * denominator is @c 0 the default value is returned to prevent an
	 * division by zero error.
	 *
	 * @param rational The rational value.
	 * @param default_value The value in case of a prevented division by zero.
	 */
	static b3_f64 b3Quotient(
		const Exiv2::Rational & rational,
		const b3_f64            default_value = 1);

	/**
	 * This method computes the rounded quotient of the given rational. In case
	 * the denominator is @c 0 the default value is returned to prevent an
	 * division by zero error. The resulting quotient is rounded nearest.
	 *
	 * @param rational The rounded rational integer value.
	 * @param default_value The value in case of a prevented division by zero.
	 */
	static signed b3RoundedQuotient(
		const Exiv2::Rational & rational,
		const b3_f64            default_value = 1);

	operator b3Date ();

private:
	const char * b3PrepareDate(const b3Date & date, char * date_time, const size_t size);

	static signed b3RoundedQuotient(
		const Exiv2::Exifdatum & datum,
		const b3_f64             default_value);
};

#endif // B3TXEXIF_H
