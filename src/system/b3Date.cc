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

#define TICKS_SEC       (1)
#define TICKS_MIN      (60)
#define TICKS_HOUR   (3600)
#define TICKS_DAY   (86400)
#define TICKS_YEAR  (TICKS_DAY * 365)

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

b3Date::b3Date(const b3Date & other)
{
	*this = other;
}

b3Date::b3Date(
	unsigned short newDay,
	b3_month       newMonth,
	unsigned long  newYear)
{
	b3Now();
	if (!b3SetDate(newDay, newMonth, newYear))
	{
		B3_THROW(b3DateException, B3_DATE_ILLEGAL);
	}
}

void b3Date::b3Now()
{
	// Get time via standard routines...
	time_code = time(&time_code);
	b3LocalTime();
}

void b3Date::b3SetMode(long new_mode)
{
	switch (new_mode)
	{
	case B3_DT_GM :
		b3GMTime();
		break;

	case B3_DT_LOCAL :
		b3LocalTime();
		break;

	case B3_DT_DIFF :
		b3DiffTime();
		break;
	}
}

long b3Date::b3GetMode()
{
	return mode;
}

struct tm * b3Date::b3TM(struct tm * time_tm)
{
	if (time_tm != nullptr)
	{
		time_tm->tm_year  = year  - 1900;
		time_tm->tm_mon   = month - 1;
		time_tm->tm_mday  = day;
		time_tm->tm_wday  = wday;

		time_tm->tm_hour  = hour;
		time_tm->tm_min   = min;
		time_tm->tm_sec   = sec;
		time_tm->tm_isdst = dls;
	}
	return time_tm;
}

void b3Date::b3LocalTime()
{
	const struct tm * now;

	now       = localtime(&time_code);
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
	dls       = now->tm_isdst != 0;
}

void b3Date::b3GMTime()
{
	struct tm * now;
	time_t     mask = -1, bit = 1L << (sizeof(time_code) * 8 - 1);

	do
	{
		now = gmtime(&time_code);
		if (now == nullptr)
		{
			time_code &= (mask ^ bit);
			bit = bit >> 1;
		}
	}
	while (now == nullptr);

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
	dls       = now->tm_isdst != 0;
}

void b3Date::b3DiffTime()
{
	unsigned long rest;

	mode      = B3_DT_DIFF;

	// Setup date...
	year      = 0;
	month     = B3_NONE;
	day       = (unsigned short)(time_code / TICKS_DAY);
	rest      = (unsigned short)(time_code - TICKS_DAY * day);
	wday      = B3_SUNDAY;

	// .. and time
	hour      = (unsigned short)(rest / TICKS_HOUR);
	rest      =                  rest - TICKS_HOUR * hour;

	min       = (unsigned short)(rest / TICKS_MIN);
	sec       = (unsigned short)(rest % TICKS_MIN);
	microsec  = 0;
	dls       = 0;
}

void b3Date::b3Dump(const long code)
{
	b3PrintF(B3LOG_DEBUG, "### Y2K - %02u.%02u.%04u: %c%c%c %c%c%c %c%c (%s)\n",
		day, month, year,
		(code &   1) ? '*' : '-',
		(code &   2) ? '*' : '-',
		(code &   4) ? '*' : '-',
		(code &   8) ? '*' : '-',
		(code &  16) ? '*' : '-',
		(code &  32) ? '*' : '-',
		(code &  64) ? '*' : '-',
		(code & 128) ? '*' : '-',
		(code ==  0) ? "OK" : "ERROR");
}

long b3Date::b3Check(
	unsigned     t_hour,
	unsigned     t_min,
	unsigned     t_sec,
	unsigned     t_day,
	b3_month     t_month,
	signed       t_year,
	b3_week_day  t_wday,
	bool         t_dls)
{
	unsigned code = 0;

	// If we are in summer time we have one hour later.
	// So do it so!
	if (t_dls)
	{
		t_hour++;
	}

	code |= ((unsigned)(hour  != t_hour)  << 0);
	code |= ((unsigned)(min   != t_min)   << 1);
	code |= ((unsigned)(sec   != t_sec)   << 2);
	code |= ((unsigned)(day   != t_day)   << 3);
	code |= ((unsigned)(month != t_month) << 4);
	code |= ((unsigned)(year  != t_year)  << 5);
	code |= ((unsigned)(wday  != t_wday)  << 6);
	code |= ((unsigned)(dls   != t_dls)   << 7);

	return code;
}

bool b3Date::b3Y2K_Selftest()
{
	long   code, diff, aux;
	bool   success = true;
	time_t actual  = time_code;

	b3PrintF(B3LOG_DEBUG, "\n");
	b3PrintF(B3LOG_DEBUG, "### Blizzard III YEAR 2000 Check ###\n");
	b3PrintF(B3LOG_DEBUG, "###                   +--------- hour\n");
	b3PrintF(B3LOG_DEBUG, "###                   |+-------- minute\n");
	b3PrintF(B3LOG_DEBUG, "###                   ||+------- second\n");
	b3PrintF(B3LOG_DEBUG, "###                   ||| +----- day\n");
	b3PrintF(B3LOG_DEBUG, "###                   ||| |+---- month\n");
	b3PrintF(B3LOG_DEBUG, "###                   ||| ||+--- year\n");
	b3PrintF(B3LOG_DEBUG, "###                   ||| ||| +- week day\n");
	b3PrintF(B3LOG_DEBUG, "###                   ||| ||| |+ daylight saving\n");
	b3PrintF(B3LOG_DEBUG, "### Y2K - Check code: hms dmy wl (OK = \"-\" |  Error = \"*\")\n");

	time_code = 12 * TICKS_HOUR;
	b3LocalTime();
	aux = hour;
	b3GMTime();
	diff = (aux - hour) * TICKS_HOUR;

	time_code = DATE_000000_09091999 - diff;
	b3LocalTime();
	code = b3Check(0, 0, 0, 9, B3_SEPTEMBER, 1999, B3_THURSDAY, true);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_000000_10091999 - diff;
	b3LocalTime();
	code = b3Check(0, 0, 0, 10, B3_SEPTEMBER, 1999, B3_FRIDAY, true);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_000000_31121999 - diff;
	b3LocalTime();
	code = b3Check(0, 0, 0, 31, B3_DECEMBER, 1999, B3_FRIDAY, false);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_000000_01012000 - diff;
	b3LocalTime();
	code = b3Check(0, 0, 0, 1, B3_JANUARY, 2000, B3_SATURDAY, false);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_000000_29022000 - diff;
	b3LocalTime();
	code = b3Check(0, 0, 0, 29, B3_FEBRUARY, 2000, B3_TUESDAY, false);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_120000_01042001 - diff;
	b3LocalTime();
	code = b3Check(12, 0, 0, 1, B3_APRIL, 2001, B3_SUNDAY, true);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_120000_08042001 - diff;
	b3LocalTime();
	code = b3Check(12, 0, 0, 8, B3_APRIL, 2001, B3_SUNDAY, true);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_120000_19122018 - diff;
	b3LocalTime();
	code = b3Check(12, 0, 0, 19, B3_DECEMBER, 2018, B3_WEDNESDAY, false);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = DATE_120000_23032021 - diff;
	b3LocalTime();
	code = b3Check(12, 0, 0, 23, B3_MARCH, 2021, B3_TUESDAY, false);
	if (code != 0)
	{
		success = false;
	}
	b3Dump(code);

	time_code = -1 ^ ((time_t)1 << (sizeof(time_t) * 8 - 1));
	b3GMTime();
	b3PrintF(B3LOG_DEBUG, "### Y2K\n");
	b3PrintF(B3LOG_DEBUG, "### Y2K - The last possible time code is at:\n");
	b3PrintF(B3LOG_DEBUG, "### Y2K - %02u:%02u:%02u - %02u.%02u.%d GMT\n",
		hour, min, sec, day, month, year);

	// Set previous mode;
	time_code = actual;
	b3SetMode(mode);

	// Display error message
	if (!success)
	{
		b3PrintF(B3LOG_NORMAL, "### CLASS: b3Date !!! WARNING: This OS is not \"Year 2000\" safe !!!\n\n");
	}
	else
	{
		b3PrintF(B3LOG_DEBUG, "### Blizzard III YEAR 2000 Check complete ###\n\n");
	}

	return success;
}

void b3Date::operator-=(const b3Date & diff)
{
	if (time_code < diff.time_code)
	{
		time_code  = diff.time_code - time_code;
	}
	else
	{
		time_code -= diff.time_code;
	}

	b3SetMode(B3_DT_DIFF);
}

void b3Date::operator+=(const b3Date & diff)
{
	if ((diff.mode != B3_DT_DIFF) && (mode != B3_DT_DIFF))
	{
		// It doen's make sense to add a date to another
		// date. But it make sense to add a difference
		// to a date or another difference. The result
		// is:
		// 1) date  = date + diff
		// 2) diff  = diff + diff
		// 3) ERROR = date + date
		// 4) ERROR = diff + date
		B3_THROW(b3DateException, B3_DATE_ILLEGAL_MODE);
	}
	time_code += diff.time_code;
	b3SetMode(mode);
}

b3Date & b3Date::operator =(const b3Date & eq)
{
	time_code = eq.time_code;
	b3SetMode(eq.mode);

	return *this;
}

b3Date & b3Date::operator=(const time_t & eq)
{
	time_code = eq;
	b3SetMode(B3_DT_LOCAL);

	return *this;
}

bool b3Date::b3SetDate(
	unsigned short newDay,
	b3_month       newMonth,
	unsigned long  newYear)
{
	struct tm date;

	month = newMonth;
	day   = newDay;
	year  = newYear;
	b3TM(&date);

	time_code = mktime(&date);
	return time_code != -1;
}

bool b3Date::b3SetTime(
	unsigned short newHour,
	unsigned short newMin,
	unsigned short newSec)
{
	struct tm date;
	bool      result;

	if ((newHour < 24) && (newMin < 60) && (newSec < 60))
	{
		hour     = newHour;
		min      = newMin;
		sec      = newSec;
		microsec = 0;

		b3TM(&date);
		time_code = mktime(&date);
		result    = (time_code != -1);
	}
	else
	{
		result = false;
	}
	return result;
}

b3Date b3Date::operator+(const b3Date & b)
{
	b3Date a;

	a  = *this;
	a += b;

	return a;
}

b3Date b3Date::operator-(const b3Date & b)
{
	b3Date a;

	a  = *this;
	a -= b;

	return a;
}

// Comparisons
bool b3Date::operator<(const b3Date & comp)
{
	return time_code < comp.time_code;
}

bool b3Date::operator <=(const b3Date & comp)
{
	return time_code <= comp.time_code;
}

bool b3Date::operator==(const b3Date & comp)
{
	return time_code == comp.time_code;
}

bool b3Date::operator !=(const b3Date & comp)
{
	return time_code != comp.time_code;
}

bool b3Date::operator>=(const b3Date & comp)
{
	return time_code >= comp.time_code;
}

bool b3Date::operator >(const b3Date & comp)
{
	return time_code > comp.time_code;
}
