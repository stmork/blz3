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

class b3TxExif
{
protected:
	static const char * m_DateTimeFormat;

#ifdef HAVE_LIBEXIV2
	Exiv2::ExifData     m_ExifData;
#endif

public:
	explicit b3TxExif(const char * filename);
	b3TxExif();

	void b3Write(const char * filename);
};

#endif // B3TXEXIF_H
