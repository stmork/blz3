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
#include "blz3/system/b3Exception.h"

// week days
#define B3_MONDAY     1
#define B3_TUESDAY    2
#define B3_WEDNESDAY  3
#define B3_THURSDAY   4
#define B3_FRIDAY	  5
#define B3_SATURDAY   6
#define B3_SUNDAY     0

// months
#define B3_JANUARY    1
#define B3_FEBRUARY   2
#define B3_MARCH      3
#define B3_APRIL      4
#define B3_MAY        5
#define B3_JUNE       6
#define B3_JULY       7
#define B3_AUGUST     8
#define B3_SEPTEMBER  9
#define B3_OCTOBER   10
#define B3_NOVEMBER  11
#define B3_DECEMBER  12

#define B3_DT_GM      0
#define B3_DT_LOCAL   1
#define B3_DT_DIFF    2

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

typedef b3Exception<b3_date_error,0x444154> b3DateException;

/**
 * This class provides methods for dealing with dates and to
 * calculate with dates.
 */
class B3_PLUGIN b3Date
{
	time_t         time_code;
	long           mode;

public:
	// date
	unsigned long  year;     //!< The year.
	unsigned short month;    //!< The month.
	unsigned short day;      //!< The day of month.
	unsigned short wday;     //!< The day of week.

	// time
	unsigned short hour;     //!< The hour.
	unsigned short min;      //!< The minute.
	unsigned short sec;      //!< The second.
	unsigned long  microsec; //!< The microseconds

	bool           dls;      //!< Daylight saving time.

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3Date();

	/**
	 * This constructor initializes this instance with a date by
	 * calling the b3SetDate() method.
	 *
	 * @param day   The day to set.
	 * @param month The month to set.
	 * @param year  The year to set.
	 * @throws b3DateException
	 */
	b3Date(unsigned short day,unsigned short month,unsigned long year) throw(b3DateException);

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
	bool       b3SetDate(unsigned short day,unsigned short month,unsigned long year);

	/**
	 * This method sets the date values to the days time point.
	 *
	 * @param hour   The hours to set.
	 * @param minute The minuts to set.
	 * @param second The seconds to set.
	 * @return Success.
	 */
	bool       b3SetTime(unsigned short hour=0,unsigned short minute=0,unsigned short second=0);

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
	struct tm *b3TM(struct tm *time_tm);


	/////////////////////////////// Operators
	/**
	 * This operator subtract the given date from this instance.
	 *
	 * @param date The date to subtract.
	 */
	void     operator-=(const b3Date &date);

	/**
	 * This operator adds the given date to this instance.
	 *
	 * @param date The date to add.
	 * @throws b3DateException
	 */
	void     operator+=(const b3Date &date) throw(b3DateException);

	/**
	 * This operator assigns a given date to this instance.
	 *
	 * @param date The date to assign.
	 */
	void     operator=(const b3Date &date);

	/**
	 * This operator assigns a given date as <code>time_t</code> to this instance.
	 *
	 * @param time_point The time value to assign.
	 */
	void     operator=(const time_t &time_point);

	/**
	 * This operator adds a given Date to this instance and results in a new date.
	 *
	 * @param date The date to add.
	 * @return The resulting date.
	 */
	b3Date   operator+(const b3Date &date);

	/**
	 * This operator subtracts a given Date from this instance and results in a new date.
	 *
	 * @param date The date to subtract.
	 * @return The resulting date.
	 */
	b3Date   operator-(const b3Date &date);

	/**
	 * This operator returns the date of this instance as <code>time_t</code> value.
	 *
	 * @return The date as time_t.
	 */
	time_t   operator()();

	/////////////////////////////// Comparisons
	/**
	 * This operator returns a non zero value if this instance is smaller than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	int      operator<(const b3Date &date);

	/**
	 * This operator returns a non zero value if this instance is smaller or equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	int      operator<=(const b3Date &date);

	/**
	 * This operator returns a non zero value if this instance is equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	int      operator==(const b3Date &date);

	/**
	 * This operator returns a non zero value if this instance is not equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	int      operator!=(const b3Date &date);

	/**
	 * This operator returns a non zero value if this instance is greater or equal than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	int      operator>=(const b3Date &date);

	/**
	 * This operator returns a non zero value if this instance is greater than the given date.
	 *
	 * @param date The right hand date to compare to.
	 */
	int      operator>(const b3Date &date);

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
		unsigned short,unsigned short,unsigned short,
		unsigned short,unsigned short,unsigned long,
		unsigned short,bool);
};

#endif
