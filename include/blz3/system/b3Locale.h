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

#include <locale>
#include <codecvt>

/**
 * This class contains static methods for converting character encodings
 * from ISO-8859 into UTF-8.
 */
class B3_PLUGIN b3Locale
{
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> m_Convert;

	std::locale m_Locale;

public:
	b3Locale();

	/**
	 * This method returns the std::locale instance as cast operator.
	 */
	inline operator std::locale & ()
	{
		return m_Locale;
	}

	/**
	 * This method converts an UTF-8 qc char typed string into an @c wchar_t
	 * based string using UTF-16.
	 *
	 * @param input The UTF-8 input string.
	 * @return The resulting UTF-16 string-.
	 */
	std::wstring b3FromBytes(const std::string & input);

	/**
	 * This method converts an UTF-16 @c wchar_t typed string into an @c char
	 * based string using UTF-16.
	 *
	 * @param input The UTF-8 input string.
	 * @return The resulting UTF-16 string-.
	 */
	std::string b3ToBytes(const std::wstring & input);

	/**
	 * This method converts a given ISO-8859-15 encoded input text into UTf-8.
	 * If the input text is already UTF-8 using the b3IsUtf8() method the
	 * buffer is simply copied.
	 *
	 * @note The conversion takes place to convert only german umlauts.
	 * @see b3IsUtf8()
	 * @see https://stackoverflow.com/questions/4059775/convert-iso-8859-1-strings-to-utf-8-in-c-c
	 *
	 * @param src The ISO-8859 encoded input text.
	 * @param dst The buffer to copy into the UTF-8 encoded text.
	 * @param len The destination buffer size.
	 */
	static void b3IsoToLocale(
		const char * src,
		char    *    dst,
		b3_size      len);

	/**
	 * This method copies a text from the source buffer into the destination
	 * buffer. If the input text is not UTF-8 an std::invalid_argument
	 * exception is thrown.
	 *
	 * @param src The UTF-8 input text to copy from.
	 * @param dst The buffer to copy into the UTF-8 encoded text.
	 * @param len The destination buffer size.
	 */
	static void b3LocaleToIso(
		const char * src,
		char    *    dst,
		b3_size      len);

	/**
	 * This method checks whether the given text conforms to the UTF-8
	 * standard. Since ASCII is a subset of UTF-8 this method will return
	 * true on pure ASCII input but does not ensure this.
	 *
	 * @see https://de.wikipedia.org/wiki/UTF-8#Zul%C3%A4ssige_Bytes_und_ihre_Bedeutung
	 *
	 * @param text The input text.
	 * @return True if the given text is UTF-8 conform.
	 */
	static bool b3IsUtf8(const char * text);
};

#endif
