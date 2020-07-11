/*
**
**	$Filename:	b3Locale.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III  Locale processing
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemIndInclude.h"
#include "blz3/system/b3Locale.h"
#include <wchar.h>
#include <locale.h>

/*************************************************************************
**                                                                      **
**                        Locale processing class                       **
**                                                                      **
*************************************************************************/

void b3Locale::b3IsoToLocale(const char * src, char * dst, b3_size len)
{
	if (setlocale(LC_CTYPE, "de_DE.ISO8859-1") != null)
	{
		wchar_t result[1024];
		size_t  max;

		max = sizeof(result) / sizeof(wchar_t);
		B3_ASSERT(len < max);

		mbstowcs(result, src, max);

		setlocale(LC_CTYPE, "");
		wcstombs(dst, result, len);
	}
	else
	{
		memcpy(dst, src, len);
	}
}

void b3Locale::b3LocaleToIso(const char * src, char * dst, b3_size len)
{
	wchar_t result[1024];
	size_t  max;

	max = sizeof(result) / sizeof(wchar_t);
	B3_ASSERT(len < max);

	setlocale(LC_CTYPE, "");
	mbstowcs(result, src, max);

	if (setlocale(LC_CTYPE, "de_DE.ISO8859-1") != null)
	{
		wcstombs(dst, result, len);
	}
	else
	{
		memcpy(dst, src, len);
	}
	setlocale(LC_CTYPE, "");
}
