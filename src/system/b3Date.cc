/*
**
**	$Filename:	b3Date.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - date routines
**
**	(C) Copyright 2001  Steffen A. Mork
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
#include "blz3/system/b3Date.h"
#include "blz3/system/b3Log.h"

// We start at 01.01.1970 at 00:00:00
#define DATE_000000_09091999 (29 * TICKS_YEAR      + TICKS_DAY * 258)
#define DATE_000000_10091999 (DATE_000000_09091999 + TICKS_DAY *   1)
#define DATE_000000_31121999 (30 * TICKS_YEAR      + TICKS_DAY *   6)
#define DATE_000000_01012000 (DATE_000000_31121999 + TICKS_DAY)
#define DATE_000000_29022000 (DATE_000000_31121999 + TICKS_DAY *  60)
#define DATE_120000_01042001 (DATE_000000_01012000 + TICKS_DAY * 456 + 12 * TICKS_HOUR)
#define DATE_120000_08042001 (DATE_120000_01042001 + TICKS_DAY *   7)
#define DATE_120000_19122018 (48 * TICKS_YEAR + TICKS_DAY * (365 - 13 + 13 - 1) + 12 * TICKS_HOUR)
#define DATE_120000_23032021 (51 * TICKS_YEAR + TICKS_DAY * (31 +28 + 23 + 13 - 1) + 12 * TICKS_HOUR)

/*************************************************************************
**                                                                      **
**                        b3Date routines                               **
**                                                                      **
*************************************************************************/

b3Date::b3Date()
{
	b3Now();
}

b3Date::b3Date(const b3Date & other) :
	time_code(other.time_code)
{
	microsec  = other.microsec;
	sec       = other.sec;
	min       = other.min;
	hour      = other.hour;
	day       = other.day;
	month     = other.month;
	year      = other.year;
	mode      = other.mode;
	offset    = other.offset;
	dst       = other.dst;
	wday      = other.wday;

	if (!b3Update())
	{
		B3_THROW(b3DateException, B3_DATE_ILLEGAL);
	}
}

b3Date::b3Date(
	b3_day    new_day,
	b3_month  new_month,
	b3_year   new_year)
{
	b3Now();
	if (b3SetDate(new_day, new_month, new_year))
	{
		b3SetMode(mode);
	}
	else
	{
		B3_THROW(b3DateException, B3_DATE_ILLEGAL);
	}
}

void b3Date::b3Now()
{
	// Get time via standard routines...
	time_code = std::time(&time_code);
	b3LocalTime();
}

void b3Date::b3SetMode(const b3_daytime new_mode)
{
	switch (new_mode)
	{
	case B3_DT_GM :
		b3GMTime();
		break;

	case B3_DT_LOCAL :
		b3LocalTime();
		break;
	}
}

b3_daytime b3Date::b3GetMode() const
{
	return mode;
}

struct std::tm * b3Date::b3TM(struct std::tm * time_tm) const
{
	if (time_tm != nullptr)
	{
		time_tm->tm_year   = year  - 1900;
		time_tm->tm_mon    = month - 1;
		time_tm->tm_mday   = day;
		time_tm->tm_wday   = wday;

		time_tm->tm_hour   = hour;
		time_tm->tm_min    = min;
		time_tm->tm_sec    = sec;
		time_tm->tm_isdst  = dst;
		time_tm->tm_gmtoff = offset * 60;
	}
	return time_tm;
}

void b3Date::b3LocalTime()
{
	struct std::tm         buffer;
	const struct std::tm * now = localtime_r(&time_code, &buffer);

	mode      = B3_DT_LOCAL;

	// ... copy date...
	year      =                  now->tm_year + 1900;
	month     = (b3_month)      (now->tm_mon  +    1);
	day       = (unsigned short) now->tm_mday;
	wday      = (b3_week_day)   (now->tm_wday % B3_WEEKDAY_COUNT);

	// ... and time
	hour      = (unsigned short) now->tm_hour;
	min       = (unsigned short) now->tm_min;
	sec       = (unsigned short) now->tm_sec;
	microsec  = 0;
	dst       = now->tm_isdst > 0;
	offset    = now->tm_gmtoff / 60;
}

void b3Date::b3GMTime()
{
	struct std::tm         buffer;
	const struct std::tm * now = gmtime_r(&time_code, &buffer);

	mode      = B3_DT_GM;

	// ... copy date...
	year      =                  now->tm_year + 1900;
	month     = (b3_month)      (now->tm_mon  +    1);
	day       = (unsigned short) now->tm_mday;
	wday      = (b3_week_day)   (now->tm_wday % B3_WEEKDAY_COUNT);

	// ... and time
	hour      = (unsigned short)now->tm_hour;
	min       = (unsigned short)now->tm_min;
	sec       = (unsigned short)now->tm_sec;
	microsec  = 0;
	dst       = now->tm_isdst > 0;
	offset    = now->tm_gmtoff / 60;
}

b3Date & b3Date::operator=(const b3Date & eq)
{
	time_code = eq.time_code;
	b3SetMode(eq.mode);

	return *this;
}

b3Date & b3Date::operator=(const std::time_t & eq)
{
	time_code = eq;
	b3SetMode(mode);

	return *this;
}

bool b3Date::b3SetDate(
	const b3_day   new_day,
	const b3_month new_month,
	const b3_year  new_year)
{
	month = new_month;
	day   = new_day;
	year  = new_year;

	return b3Update();
}

bool b3Date::b3SetTime(
	const unsigned new_hour,
	const unsigned new_min,
	const unsigned new_sec)
{
	bool result;

	if ((new_hour < 24) && (new_min < 60) && (new_sec < 60))
	{
		hour     = new_hour;
		min      = new_min;
		sec      = new_sec;
		microsec = 0;

		result   = b3Update();
	}
	else
	{
		result = false;
	}
	return result;
}

bool b3Date::b3SetDateTime(
	const b3_day   new_day,
	const b3_month new_month,
	const b3_year  new_year,
	const unsigned new_hour,
	const unsigned new_min,
	const unsigned new_sec)
{
	bool      result;

	if ((new_hour < 24) && (new_min < 60) && (new_sec < 60))
	{
		month    = new_month;
		day      = new_day;
		year     = new_year;
		hour     = new_hour;
		min      = new_min;
		sec      = new_sec;
		microsec = 0;
		dst      = false;
		offset   = 0;

		result   = b3Update();
	}
	else
	{
		result = false;
	}
	return result;
}

bool b3Date::b3Update()
{
	struct tm date {};

	b3TM(&date);

	// Repair DST
	date.tm_isdst = -1; // Unknown.
	std::mktime(&date);

	// Copy corrected DST
	dst       = date.tm_isdst > 0;

	// Get real time_t
	time_code = std::mktime(&date);

	return time_code != -1;
}

#ifndef __cpp_impl_three_way_comparison

// Comparisons
bool b3Date::operator<(const b3Date & other) const
{
	return time_code < other.time_code;
}

bool b3Date::operator<=(const b3Date & comp) const
{
	return time_code <= comp.time_code;
}

bool b3Date::operator==(const b3Date & comp) const
{
	return time_code == comp.time_code;
}

bool b3Date::operator!=(const b3Date & comp) const
{
	return !operator==(comp);
}

bool b3Date::operator>=(const b3Date & comp) const
{
	return comp.operator<=(*this);
}

bool b3Date::operator>(const b3Date & comp) const
{
	return comp.operator<(*this);
}

#endif
