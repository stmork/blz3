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

#pragma once

#ifndef B3_SYSTEM_DATE_H
#define B3_SYSTEM_DATE_H

#include <time.h>
#include "blz3/system/b3Exception.h"

// week days
enum b3_week_day
{
	B3_SUNDAY = 0,
	B3_MONDAY,
	B3_TUESDAY,
	B3_WEDNESDAY,
	B3_THURSDAY,
	B3_FRIDAY,
	B3_SATURDAY,
	B3_WEEKDAY_COUNT
};

// months
enum b3_month
{
	B3_NONE = 0,
	B3_JANUARY = 1,
	B3_FEBRUARY,
	B3_MARCH,
	B3_APRIL,
	B3_MAY,
	B3_JUNE,
	B3_JULY,
	B3_AUGUST,
	B3_SEPTEMBER,
	B3_OCTOBER,
	B3_NOVEMBER,
	B3_DECEMBER
};

enum b3_daytime
{
	B3_DT_GM = 0,
	B3_DT_LOCAL,
	B3_DT_DIFF
};

/**
 * This enumeration lists error codes for date handling.
 */
enum b3_date_error
{
	B3_DATE_ERROR = -1,   //!< General date error.
	B3_DATE_OK    =  0,   //!< Date handling is OK.
	B3_DATE_ILLEGAL,      //!< Input is illegal.
	B3_DATE_ILLEGAL_MODE  //!< Unknown time zone.
};

typedef b3Exception<b3_date_error, 0x444154> b3DateException;

/**
 * This class provides methods for dealing with dates and to
 * calculate with dates.
 */
class B3_PLUGIN b3Date
{
	time_t         time_code = 0;
	long           mode      = 0;

public:
	// date
	signed         year;     //!< The year.
	b3_month       month;    //!< The month.
	unsigned       day;      //!< The day of month.
	b3_week_day    wday;     //!< The day of week.

	// time
	unsigned       hour;     //!< The hour.
	unsigned       min;      //!< The minute.
	unsigned       sec;      //!< The second.
	unsigned       microsec; //!< The microseconds

	bool           dls;      //!< Daylight saving time.

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3Date();

	/**
	 * The copy constructor copies the contents of another
	 * instance.
	 *
	 * @param other The the other instance to copy from.
	 */
	b3Date(const b3Date & other);

	/**
	 * This constructor initializes this instance with a date by
	 * calling the b3SetDate() method.
	 *
	 * @param day   The day to set.
	 * @param month The month to set.
	 * @param year  The year to set.
	 * @throws b3DateException
	 */
	b3Date(unsigned short day, b3_month month, unsigned long year);

	/**
	 * This method sets the date values to the actual time point.
	 */
	void       b3Now();

	/**
	 * This sets the date values to the day/month/year values.
	 *
	 * @param day   The day to set.
	 * @param month The month to set.
	 * @param year  The year to set.
	 * @return Success.
	 */
	bool       b3SetDate(unsigned short day, b3_month month, unsigned long year);

	/**
	 * This method sets the date values to the days time point.
	 *
	 * @param hour   The hours to set.
	 * @param minute The minuts to set.
	 * @param second The seconds to set.
	 * @return Success.
	 */
	bool       b3SetTime(unsigned short hour = 0, unsigned short minute = 0, unsigned short second = 0);

	/**
	 * This method sets the time zone.
	 *
	 * @param mode New time zone.
	 */
	void       b3SetMode(long mode);

	/**
	 * This method gets the time zone mode.
	 *
	 * @return Used time zone.
	 */
	long       b3GetMode();

	/**
	 * This fills a <code>tm</code> struct with the values of this instance.
	 *
	 * @param time_tm The tm structure to fill.
	 * @return The given tm structure.
	 */
	struct tm * b3TM(struct tm * time_tm);


	/////////////////////////////// Operators
	/**
	 * This operator subtract the given date from this instance.
	 *
	 * @param date The date to subtract.
	 */
	void     operator-=(const b3Date & date);

	/**
	 * This operator adds the given date to this instance.
	 *
	 * @param date The date to add.
	 * @throws b3DateException
	 */
	void     operator+=(const b3Date & date);

	/**
	 * This operator assigns a given date to this instance.
	 *
	 * @param date The date to assign.
	 */
	b3Date & operator=(const b3Date & date);

	/**
	 * This operator assigns a given date as <code>time_t</code> to this instance.
	 *
	 * @param time_point The time value to assign.
	 */
	b3Date & operator =(const time_t & time_point);

	/**
	 * This operator adds a given Date to this instance and results in a new date.
	 *
	 * @param date The date to add.
	 * @return The resulting date.
	 */
	b3Date   operator+(const b3Date & date);

	/**
	 * This operator subtracts a given Date from this instance and results in a new date.
	 *
	 * @param date The date to subtract.
	 * @return The resulting date.
	 */
	b3Date   operator-(const b3Date & date);

	/**
	 * This operator returns the date of this instance as <code>time_t</code> value.
	 *
	 * @return The date as time_t.
	 */
	inline operator time_t () const
	{
		return time_code;
	}

	/////////////////////////////// Comparisons
	/**
	 * This operator returns a non zero value if this instance is smaller than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator <(const b3Date & date);

	/**
	 * This operator returns a non zero value if this instance is smaller or equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator<=(const b3Date & date);

	/**
	 * This operator returns a non zero value if this instance is equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator ==(const b3Date & date);

	/**
	 * This operator returns a non zero value if this instance is not equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator!=(const b3Date & date);

	/**
	 * This operator returns a non zero value if this instance is greater or equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator >=(const b3Date & date);

	/**
	 * This operator returns a non zero value if this instance is greater than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator>(const b3Date & date);

	/////////////////////////////// Special case...
	/**
	 * This is a self test for Y2K purposes.
	 *
	 * @return Success (= correct Y2K handling).
	 */
	bool b3Y2K_Selftest();

private:
	void b3LocalTime();
	void b3GMTime();
	void b3DiffTime();
	long b3Check(
		unsigned h, unsigned m, unsigned s,
		unsigned day, b3_month, signed year,
		b3_week_day w, bool dls);

	void b3Dump(const long code);
};

#endif
