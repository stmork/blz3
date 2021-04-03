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

void b3Locale::b3IsoToLocale(
	const char * src,
	char    *    dst,
	b3_size      len B3_UNUSED)
{
#if 1
	while (*src)
	{
		b3_u08 c = *src;

		if (c < 128)
		{
			*dst++ = c;
		}
		else
		{
			*dst++ = 0xc0 + (c >> 6);
			*dst++ = 0x80 + (c & 0x3f);
		}
		src++;
	}
#else
	if (setlocale(LC_CTYPE, "de_DE.ISO8859-1") != nullptr)
	{
		wchar_t result[1024];
		const size_t  max = sizeof(result) / sizeof(wchar_t);

		B3_ASSERT(len < max);

		mbstowcs(result, src, max);

		setlocale(LC_CTYPE, "");
		wcstombs(dst, result, len);
	}
	else
	{
		memcpy(dst, src, len);
	}
#endif
}

void b3Locale::b3LocaleToIso(const char * src, char * dst, b3_size len)
{
	wchar_t result[1024];
	const size_t  max = sizeof(result) / sizeof(wchar_t);

	B3_ASSERT(len < max);

	setlocale(LC_CTYPE, "");
	mbstowcs(result, src, max);

	if (setlocale(LC_CTYPE, "de_DE.ISO8859-1") != nullptr)
	{
		wcstombs(dst, result, len);
	}
	else
	{
		memcpy(dst, src, len);
	}
	setlocale(LC_CTYPE, "");
}
