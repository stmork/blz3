/*
**
**	$Filename:	b3Locale.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Locale routines (proto types)
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_LOCALE_H
#define B3_SYSTEM_LOCALE_H

#include "blz3/b3Types.h"

class B3_PLUGIN b3Locale
{
public:
	static void b3IsoToLocale(
		const char   *  src,
		char   *  dst,
		b3_size   len);

	static void b3LocaleToIso(
		const char   *  src,
		char   *  dst,
		b3_size   len);
};

#endif
