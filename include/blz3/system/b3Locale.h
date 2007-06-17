/*
**
**	$Filename:	b3Locale.h $
**	$Release:	Dortmund 2006 $
**	$Revision: 1322 $
**	$Date: 2006-02-05 19:57:14 +0100 (So, 05 Feb 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Locale routines (proto types)
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_LOCALE_H
#define B3_SYSTEM_LOCALE_H

#include "blz3/b3Types.h"

class B3_PLUGIN b3Locale
{
public:
	static void b3IsoToLocale(
		const char     *src,
		      char     *dst,
		      b3_size   len);

	static void b3LocaleToIso(
		const char     *src,
		      char     *dst,
		      b3_size   len);
};

#endif
