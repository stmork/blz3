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
#define DATE_LAST             0x80000000

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2002/08/11 08:17:41  sm
**	- Renamed system_independend to system like include
**
**	Revision 1.3  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.2  2001/07/02 19:52:03  sm
**	- Cleaning up comments
**	
**	Revision 1.1  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Date routines                               **
**                                                                      **
*************************************************************************/

b3Date::b3Date()
{
	b3Now();
}

b3Date::b3Date(
	unsigned short newDay,
	unsigned short newMonth,
	unsigned long  newYear)
{
	if (!b3SetDate(newDay,newMonth,newYear))
	{
		throw b3DateException(B3_DATE_ILLEGAL);
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

struct tm *b3Date::b3TM(struct tm *time_tm)
{
	if (time_tm != null)
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
	struct tm *now;

	now       = localtime(&time_code);
	mode      = B3_DT_LOCAL;

	// ... copy date...
	year      =                  now->tm_year + 1900;
	month     = (unsigned short)(now->tm_mon  +    1);
	day       = (unsigned short) now->tm_mday;
	wday      = (unsigned short) now->tm_wday;

	// ... and time
	hour      = (unsigned short) now->tm_hour;
	min       = (unsigned short) now->tm_min;
	sec       = (unsigned short) now->tm_sec;
	microsec  = 0;
	dls       = now->tm_isdst != 0;
}

void b3Date::b3GMTime()
{
	struct tm *now;

	do
	{
		now = gmtime(&time_code);
		if (now == null)
		{
			time_code--;
		}
	}
	while (now == null);

	mode      = B3_DT_GM;

	// ... copy date...
	year      =                  now->tm_year + 1900;
	month     = (unsigned short)(now->tm_mon  +    1);
	day       = (unsigned short) now->tm_mday;
	wday      = (unsigned short) now->tm_wday;

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
	month     = 0;
	day       = (unsigned short)(time_code / TICKS_DAY);
	rest      = (unsigned short)(time_code - TICKS_DAY * day);
	wday      = 0;

	// .. and time
	hour      = (unsigned short)(rest / TICKS_HOUR);
	rest      =                  rest - TICKS_HOUR * hour;

	min       = (unsigned short)(rest / TICKS_MIN);
	sec       = (unsigned short)(rest % TICKS_MIN);
	microsec  = 0;
	dls       = 0;
}

long b3Date::b3Check(
	unsigned short t_hour,
	unsigned short t_min,
	unsigned short t_sec,
	unsigned short t_day,
	unsigned short t_month,
	unsigned long  t_year,
	unsigned short t_wday,
	bool           t_dls)
{
	long code = 0;

	// If we are in summer time we have one hour later.
	// So do it so!
	if (t_dls)
	{
		t_hour++;
	}

	code |= ((long)(hour  != t_hour)  << 0);
	code |= ((long)(min   != t_min)   << 1);
	code |= ((long)(sec   != t_sec)   << 2);
	code |= ((long)(day   != t_day)   << 3);
	code |= ((long)(month != t_month) << 4);
	code |= ((long)(year  != t_year)  << 5);
	code |= ((long)(wday  != t_wday)  << 6);
	code |= ((long)(dls   != t_dls)   << 7);

	return code;
}

bool b3Date::b3Y2K_Selftest()
{
	long   code,diff,aux;
	bool   success = true;
	time_t actual  = time_code;
	
	b3PrintF (B3LOG_DEBUG,"\n");
	b3PrintF (B3LOG_DEBUG,"### Blizzard III YEAR 2000 Check ###\n");
	b3PrintF (B3LOG_DEBUG,"###                   +--------- hour\n");
	b3PrintF (B3LOG_DEBUG,"###                   |+-------- minute\n");
	b3PrintF (B3LOG_DEBUG,"###                   ||+------- second\n");
	b3PrintF (B3LOG_DEBUG,"###                   ||| +----- day\n");
	b3PrintF (B3LOG_DEBUG,"###                   ||| |+---- month\n");
	b3PrintF (B3LOG_DEBUG,"###                   ||| ||+--- year\n");
	b3PrintF (B3LOG_DEBUG,"###                   ||| ||| +- week day\n");
	b3PrintF (B3LOG_DEBUG,"###                   ||| ||| |+ daylight saving\n");
	b3PrintF (B3LOG_DEBUG,"### Y2K - Check code: hms dmy wl (OK = \"-\" |  Error = \"*\")\n");

	time_code = 12 * TICKS_HOUR;
	b3LocalTime();
	aux = hour;
	b3GMTime();
	diff = (aux - hour) * TICKS_HOUR;

	time_code = DATE_000000_09091999 - diff;
	b3LocalTime();
	code = b3Check(0,0,0,9,9,1999,B3_THURSDAY,true);
	if (code != 0)
	{
		success = false;
	}
	b3PrintF (B3LOG_DEBUG,"### Y2K - 09.09.1999: %c%c%c %c%c%c %c%c (%s)\n",
		code &   1 ? '*' : '-',
		code &   2 ? '*' : '-',
		code &   4 ? '*' : '-',
		code &   8 ? '*' : '-',
		code &  16 ? '*' : '-',
		code &  32 ? '*' : '-',
		code &  64 ? '*' : '-',
		code & 128 ? '*' : '-',
		code ==  0 ? "OK" : "ERROR");

	time_code = DATE_000000_10091999 - diff;
	b3LocalTime();
	code = b3Check(0,0,0,10,9,1999,B3_FRIDAY,true);
	if (code != 0)
	{
		success = false;
	}
	b3PrintF (B3LOG_DEBUG,"### Y2K - 10.09.1999: %c%c%c %c%c%c %c%c (%s)\n",
		code &   1 ? '*' : '-',
		code &   2 ? '*' : '-',
		code &   4 ? '*' : '-',
		code &   8 ? '*' : '-',
		code &  16 ? '*' : '-',
		code &  32 ? '*' : '-',
		code &  64 ? '*' : '-',
		code & 128 ? '*' : '-',
		code ==  0 ? "OK" : "ERROR");

	time_code = DATE_000000_31121999 - diff;
	b3LocalTime();
	code = b3Check(0,0,0,31,12,1999,B3_FRIDAY,false);
	if (code != 0)
	{
		success = false;
	}
	b3PrintF (B3LOG_DEBUG,"### Y2K - 31.12.1999: %c%c%c %c%c%c %c%c (%s)\n",
		code &   1 ? '*' : '-',
		code &   2 ? '*' : '-',
		code &   4 ? '*' : '-',
		code &   8 ? '*' : '-',
		code &  16 ? '*' : '-',
		code &  32 ? '*' : '-',
		code &  64 ? '*' : '-',
		code & 128 ? '*' : '-',
		code ==  0 ? "OK" : "ERROR");

	time_code = DATE_000000_01012000 - diff;
	b3LocalTime();
	code = b3Check(0,0,0,1,1,2000,B3_SATURDAY,false);
	if (code != 0)
	{
		success = false;
	}
	b3PrintF (B3LOG_DEBUG,"### Y2K - 01.01.2000: %c%c%c %c%c%c %c%c (%s)\n",
		code &   1 ? '*' : '-',
		code &   2 ? '*' : '-',
		code &   4 ? '*' : '-',
		code &   8 ? '*' : '-',
		code &  16 ? '*' : '-',
		code &  32 ? '*' : '-',
		code &  64 ? '*' : '-',
		code & 128 ? '*' : '-',
		code ==  0 ? "OK" : "ERROR");

	time_code = DATE_000000_29022000 - diff;
	b3LocalTime();
	code = b3Check(0,0,0,29,2,2000,B3_TUESDAY,false);
	if (code != 0)
	{
		success = false;
	}
	b3PrintF (B3LOG_DEBUG,"### Y2K - 29.02.2000: %c%c%c %c%c%c %c%c (%s)\n",
		code &   1 ? '*' : '-',
		code &   2 ? '*' : '-',
		code &   4 ? '*' : '-',
		code &   8 ? '*' : '-',
		code &  16 ? '*' : '-',
		code &  32 ? '*' : '-',
		code &  64 ? '*' : '-',
		code & 128 ? '*' : '-',
		code ==  0 ? "OK" : "ERROR");

	time_code = DATE_120000_01042001 - diff;
	b3LocalTime();
	code = b3Check(12,0,0,1,4,2001,B3_SUNDAY,true);
	if (code != 0)
	{
		success = false;
	}
	b3PrintF (B3LOG_DEBUG,"### Y2K - 01.04.2001: %c%c%c %c%c%c %c%c (%s)\n",
		code &   1 ? '*' : '-',
		code &   2 ? '*' : '-',
		code &   4 ? '*' : '-',
		code &   8 ? '*' : '-',
		code &  16 ? '*' : '-',
		code &  32 ? '*' : '-',
		code &  64 ? '*' : '-',
		code & 128 ? '*' : '-',
		code ==  0 ? "OK" : "ERROR");

	time_code = DATE_120000_08042001 - diff;
	b3LocalTime();
	code = b3Check(12,0,0,8,4,2001,B3_SUNDAY,true);
	if (code != 0)
	{
		success = false;
	}
	b3PrintF (B3LOG_DEBUG,"### Y2K - 08.04.2001: %c%c%c %c%c%c %c%c (%s)\n",
		code &   1 ? '*' : '-',
		code &   2 ? '*' : '-',
		code &   4 ? '*' : '-',
		code &   8 ? '*' : '-',
		code &  16 ? '*' : '-',
		code &  32 ? '*' : '-',
		code &  64 ? '*' : '-',
		code & 128 ? '*' : '-',
		code ==  0 ? "OK" : "ERROR");

	time_code = DATE_LAST;
	b3GMTime();
	b3PrintF (B3LOG_DEBUG,"### Y2K\n");
	b3PrintF (B3LOG_DEBUG,"### Y2K - The last possible time code is at:\n");
	b3PrintF (B3LOG_DEBUG,"### Y2K - %02ld:%02ld:%02ld - %02ld.%02ld.%ld GMT\n",
		hour,min,sec,day,month,year);

	// Set previous mode;
	time_code = actual;
	b3SetMode(mode);

	// Display error message
	if (!success)
	{
		b3PrintF (B3LOG_NORMAL,"### CLASS: b3Date !!! WARNING: This OS is not \"Year 2000\" safe !!!\n\n");
	}
	else
	{
		b3PrintF (B3LOG_DEBUG,"### Blizzard III YEAR 2000 Check complete ###\n\n");
	}

	return success;
}

void b3Date::operator-=(const b3Date &diff)
{
	if (time_code < diff.time_code)
	{
		time_code  = diff.time_code - time_code;
	}
	else
	{
		time_code -= diff.time_code;
	}

	b3SetMode (B3_DT_DIFF);
}

void b3Date::operator+=(const b3Date &diff)
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
		throw b3DateException(B3_DATE_ILLEGAL_MODE);
	}
	time_code += diff.time_code;
	b3SetMode (mode);
}

void b3Date::operator=(const b3Date &eq)
{
	time_code = eq.time_code;
	b3SetMode(eq.mode);
}

void b3Date::operator=(const time_t &eq)
{
	time_code = eq;
	b3SetMode(B3_DT_LOCAL);
}

bool b3Date::b3SetDate(
	unsigned short newDay,
	unsigned short newMonth,
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

time_t b3Date::operator ()()
{
	return time_code;
}

b3Date b3Date::operator+(const b3Date &b)
{
	b3Date a;

	a  = *this;
	a += b;

	return a;
}

b3Date b3Date::operator-(const b3Date &b)
{
	b3Date a;

	a  = *this;
	a -= b;

	return a;
}

// Comparisons
int b3Date::operator<(const b3Date &comp)
{
	return time_code < comp.time_code;
}

int b3Date::operator<=(const b3Date &comp)
{
	return time_code <= comp.time_code;
}

int b3Date::operator==(const b3Date &comp)
{
	return time_code == comp.time_code;
}

int b3Date::operator!=(const b3Date &comp)
{
	return time_code != comp.time_code;
}

int b3Date::operator>=(const b3Date &comp)
{
	return time_code >= comp.time_code;
}

int b3Date::operator>(const b3Date &comp)
{
	return time_code > comp.time_code;
}
