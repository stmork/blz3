/*
**
**	$Filename:	b3LocaleTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Locale unit tests.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3LocaleTest.h"
#include "b3TestMacros.h"

#include "blz3/system/b3Locale.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3LocaleTest);

void b3LocaleTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3LocaleTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

const b3_u08 seq1[]
{
	0x80, 0
};

const b3_u08 seq2[]
{
	0xe0, 0
};

const b3_u08 seq3[]
{
	0xe0, 0x45, 0
};

const b3_u08 seq4[]
{
	0xe0, 0x88, 0
};

const b3_u08 seq5[]
{
	0xe0, 0x88, 0x45, 0
};

const b3_u08 seq6[]
{
	0xc4, 0xd6, 0xdc,
	0xdf,
	0xe4, // 0xf6, 0xfc, Illegal UTF-8 characters anyway.
	0
};

const b3_u08 seq6a[]
{
	0xc4, 'u', 0xdf, 'e', 0
};

const b3_u08 seq6b[]
{
	0xd6, 'f', 'f', 0
};

const b3_u08 seq6c[]
{
	0xdc, 'b', 'e', 'r', 0
};

const b3_u08 seq7[]
{
	0xf6, 0xfc, 0
};

const b3_u08 seq8[]
{
	0xa4, ' ', 'E', 'u', 'r', 'o', 0
};

void b3LocaleTest::test()
{
	testIsoSequence(seq1);
	testIsoSequence(seq2);
	testIsoSequence(seq3);
	testIsoSequence(seq4);
	testIsoSequence(seq5);
	testIsoSequence(seq6);
	testIsoSequence(seq6a, "Äuße");
	testIsoSequence(seq6b, "Öff");
	testIsoSequence(seq6c, "Über");
	testIsoSequence(seq7);
	testIsoSequence(seq8, "€ Euro");

	testUtfSequence("");
	testUtfSequence("Example");
	testUtfSequence("Example with Umlauts: äöü");
	testUtfSequence("Example with Umlauts:  ß");
	testUtfSequence("Example with Umlauts: ÄÖÜ");
	testUtfSequence("ÄaÄ");
	testUtfSequence("ß");
	testUtfSequence("ÖöÖ");
	testUtfSequence("Ä");
}

void b3LocaleTest::testIsoSequence(const b3_u08 * iso, const char * expected)
{
	const char * input = (const char *)iso;
	char         buffer[1024];

	CPPUNIT_ASSERT(!b3Locale::b3IsUtf8(input));
	CPPUNIT_ASSERT_THROW(b3Locale::b3LocaleToIso(input, buffer, sizeof(buffer)), std::invalid_argument);

	b3Locale::b3IsoToLocale(input, buffer, sizeof(buffer));
	CPPUNIT_ASSERT(strlen(buffer) < sizeof(buffer));
	CPPUNIT_ASSERT(b3Locale::b3IsUtf8(buffer));
	CPPUNIT_ASSERT_NO_THROW(b3Locale::b3LocaleToIso(buffer, buffer, sizeof(buffer)));

	if (expected != nullptr)
	{
		CPPUNIT_ASSERT(strcmp(expected, buffer) == 0);
	}
}

void b3LocaleTest::testUtfSequence(const char * utf8)
{
	char         buffer[1024];

	CPPUNIT_ASSERT(b3Locale::b3IsUtf8(utf8));
	b3Locale::b3IsoToLocale(utf8, buffer, sizeof(buffer));
	CPPUNIT_ASSERT(strcmp(utf8, buffer) == 0);
}

#endif
