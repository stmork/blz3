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

#include <stdexcept>

/*************************************************************************
**                                                                      **
**                        Locale processing class                       **
**                                                                      **
*************************************************************************/

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> b3Locale::m_Convert;

b3Locale::b3Locale() : m_Locale("")
{
}

std::wstring b3Locale::b3FromBytes(const std::string & input)
{
	return m_Convert.from_bytes(input);
}

std::string b3Locale::b3ToBytes(const std::wstring & input)
{
	return m_Convert.to_bytes(input);
}

void b3Locale::b3IsoToLocale(
	const char * src,
	char    *    dst,
	b3_size      len)
{
	if (!b3IsUtf8(src))
	{
		// Convert from ISO to UTF-8
		b3_size i = 0;

		bzero(dst, len);
		while ((*src) && (i < len))
		{
			b3_u08 c = *src;

			if (c < 128)
			{
				dst[i++] = c;
			}
			else if (c == 0xa4)
			{
				dst[i++] = 0xe2;
				if (i < len)
				{
					dst[i++] = 0x82;
					if (i < len)
					{
						dst[i++] = 0xac;
					}
				}
			}
			else
			{
				dst[i++] = 0xc0 + (c >> 6);
				if (i < len)
				{
					dst[i++] = 0x80 + (c & 0x3f);
				}
			}
			src++;
		}
		dst[len - 1] = 0;
	}
	else if (src != dst)
	{
		strncpy(dst, src, len);
		dst[len - 1] = 0;
	}
}

void b3Locale::b3LocaleToIso(
	const char * src,
	char    *    dst,
	b3_size      len)
{
	if (!b3IsUtf8(src))
	{
		throw std::invalid_argument("Input text not UTF-8!");
	}
	if (src != dst)
	{
		strncpy(dst, src, len);
		dst[len - 1] = 0;
	}
}

bool b3Locale::b3IsUtf8(const char * text)
{
	unsigned count = 1;

	for (unsigned i = 0; text[i] != 0; i++)
	{
		unsigned c = text[i] & 0xff;

		if (c >= 0x80)
		{
			if (c < 0xc0)
			{
				if (count-- <= 1)
				{
					return false;
				}
			}
			else if (c >= 0xf5)
			{
				return false;
			}
			else if (c >= 0xf0)
			{
				if (count > 1)
				{
					return false;
				}
				count = 4;
			}
			else if (c >= 0xe0)
			{
				if (count > 1)
				{
					return false;
				}
				count = 3;
			}
			else if (c >= 0xc2)
			{
				if (count > 1)
				{
					return false;
				}
				count = 2;
			}
			else
			{
				return false;
			}
		}
	}
	return count <= 1;
}
