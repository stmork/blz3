/*
**
**	$Filename:	b3Date.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - date class
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_DATE_H
#define B3_SYSTEM_DATE_H

#include <time.h>
#include "blz3/base/b3Exception.h"

// week days
#define B3_MONDAY     1
#define B3_TUESDAY    2
#define B3_WEDNESDAY  3
#define B3_THURSDAY   4
#define B3_FRIDAY	    5
#define B3_SATURDAY   6
#define B3_SUNDAY     0

// months
#define B3_JANUARY	1
#define B3_FEBRUARY	2
#define B3_MARCH		3
#define B3_APRIL		4
#define B3_MAY		5
#define B3_JUNE		6
#define B3_JULY		7
#define B3_AUGUST		8
#define B3_SEPTEMBER	9
#define B3_OCTOBER   10
#define B3_NOVEMBER  11
#define B3_DECEMBER  12

#define B3_DT_GM      0
#define B3_DT_LOCAL   1
#define B3_DT_DIFF    2

typedef enum
{
	B3_DATE_ERROR = -1,
	B3_DATE_OK    =  0,
	B3_DATE_ILLEGAL,
	B3_DATE_ILLEGAL_MODE
} b3_date_error;

typedef b3Exception<b3_date_error,'DAT'> b3DateException;

class b3Date
{
	time_t         time_code;
	long           mode;

public:
	// date
	unsigned long  year;
	unsigned short month;
	unsigned short day;
	unsigned short wday;

	// time
	unsigned short hour;
	unsigned short min;
	unsigned short sec;
	unsigned long  microsec;

	bool           dls;

public:
	           b3Date();
			   b3Date(unsigned short day,unsigned short month,unsigned long year);
	void       b3Now();
	bool       b3SetDate(unsigned short day,unsigned short month,unsigned long year);
	bool       b3SetTime(unsigned short hour=0,unsigned short minute=0,unsigned short second=0);
	void       b3SetMode(long);
	long       b3GetMode();
	struct tm *b3TM(struct tm *time_tm);

	// Operators
	void     operator-=(const b3Date &);
	void     operator+=(const b3Date &);
	void     operator=(const b3Date &);
	void     operator=(const time_t &);
	b3Date operator+(const b3Date &);
	b3Date operator-(const b3Date &);
	time_t   operator()();

	// Comparisons
	int      operator<(const b3Date &);
	int      operator<=(const b3Date &);
	int      operator==(const b3Date &);
	int      operator!=(const b3Date &);
	int      operator>=(const b3Date &);
	int      operator>(const b3Date &);

	// Special case...
	bool b3Y2K_Selftest();

private:
	void b3LocalTime();
	void b3GMTime();
	void b3DiffTime();
	long b3Check(
		unsigned short,unsigned short,unsigned short,
		unsigned short,unsigned short,unsigned long,
		unsigned short,bool);
};

#endif
