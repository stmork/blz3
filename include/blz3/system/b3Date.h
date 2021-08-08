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

enum b3_daytime
{
	B3_DT_GM = 0,
	B3_DT_LOCAL
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
	std::time_t    time_code  = 0;
	b3_daytime     mode       = B3_DT_GM;

public:
	static const unsigned TICKS_SEC  =  1;
	static const unsigned TICKS_MIN  = 60;
	static const unsigned TICKS_HOUR = TICKS_MIN  *  60;
	static const unsigned TICKS_DAY  = TICKS_HOUR *  24;
	static const unsigned TICKS_YEAR = TICKS_DAY  * 365;

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

	typedef signed   b3_year;
	typedef unsigned b3_day;

	// date
	b3_year        year;     //!< The year.
	b3_month       month;    //!< The month.
	b3_day         day;      //!< The day of month.
	b3_week_day    wday;     //!< The day of week.

	// time
	unsigned       hour;     //!< The hour.
	unsigned       min;      //!< The minute.
	unsigned       sec;      //!< The second.
	unsigned       microsec; //!< The microseconds

	bool           dst;      //!< Daylight saving time.
	signed         offset;   //!< The time zone offset including daylight saving time.

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
	b3Date(b3_day day, b3_month month, b3_year year);

	/**
	 * This method sets the date values to the actual time point.
	 */
	void       b3Now();

	/**
	 * This sets the date values to the day/month/year values and updates the
	 * time code value.
	 *
	 * @param day   The day to set.
	 * @param month The month to set.
	 * @param year  The year to set.
	 * @return Success.
	 */
	bool       b3SetDate(
		const b3_day   day,
		const b3_month month,
		const b3_year  year);

	/**
	 * This method sets the date values to the days time point and updates the
	 * time code value.
	 *
	 * @param hour   The hours to set.
	 * @param minute The minuts to set.
	 * @param second The seconds to set.
	 * @return Success.
	 */
	bool       b3SetTime(
		const unsigned hour   = 0,
		const unsigned minute = 0,
		const unsigned second = 0);

	/**
	 * This sets the date and time values and updates the
	 * time code value.
	 *
	 * @param day   The day to set.
	 * @param month The month to set.
	 * @param year  The year to set.
	 * @param hour   The hours to set.
	 * @param minute The minuts to set.
	 * @param second The seconds to set.
	 * @return Success.
	 */
	bool       b3SetDateTime(
		const b3_day   day,
		const b3_month month,
		const b3_year  year,
		const unsigned hour   = 0,
		const unsigned minute = 0,
		const unsigned second = 0);

	/**
	 * This method updates the time code value.
	 *
	 * @return True on success.
	 */
	[[nodiscard]]
	bool       b3Update();

	/**
	 * This method sets the time zone.
	 *
	 * @param mode New time zone.
	 */
	void       b3SetMode(const b3_daytime mode);

	/**
	 * This method gets the time zone mode.
	 *
	 * @return Used time zone.
	 */
	b3_daytime b3GetMode() const;

	/**
	 * This fills a <code>tm</code> struct with the values of this instance.
	 *
	 * @param time_tm The tm structure to fill.
	 * @return The given tm structure.
	 */
	tm * b3TM(struct std::tm * time_tm) const;

	/////////////////////////////// Operators

	/**
	 * This operator assigns a given date to this instance.
	 *
	 * @param date The date to assign.
	 */
	b3Date & operator=(const b3Date & date);

	/**
	 * This operator assigns a given date as @c std::time_t to this
	 * instance.
	 *
	 * @param time_point The time value to assign.
	 */
	b3Date & operator =(const std::time_t & time_point);

	/**
	 * This operator returns the date of this instance as @c std::time_t value.
	 *
	 * @return The date as @c std::time_t.
	 */
	inline operator std::time_t () const
	{
		return time_code;
	}

	inline operator struct timeval () const
	{
		struct timeval time_value;

		time_value.tv_sec = time_code;
		time_value.tv_usec = microsec;

		return time_value;
	}

	inline operator struct timespec () const
	{
		struct timespec time_value;

		time_value.tv_sec  = time_code;
		time_value.tv_nsec = microsec * 1000;

		return time_value;
	}

	/////////////////////////////// Comparisons
	/**
	 * This operator returns a non zero value if this instance is smaller than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator <(const b3Date & date) const;

	/**
	 * This operator returns a non zero value if this instance is smaller or equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator<=(const b3Date & date) const;

	/**
	 * This operator returns a non zero value if this instance is equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator ==(const b3Date & date) const;

	/**
	 * This operator returns a non zero value if this instance is not equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator!=(const b3Date & date) const;

	/**
	 * This operator returns a non zero value if this instance is greater or equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator >=(const b3Date & date) const;

	/**
	 * This operator returns a non zero value if this instance is greater than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	bool operator>(const b3Date & date) const;

private:
	void b3LocalTime();
	void b3GMTime();
};

#endif
