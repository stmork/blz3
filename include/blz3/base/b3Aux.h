/*
**
**	$Filename:	b3Aux.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Master of little help
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_AUX_H
#define B3_BASE_AUX_H

#include "blz3/base/b3Color.h"

/**
 * This class represents a RGB color triplet.
 */
class B3_PLUGIN b3RGB
{
public:
	b3_u08 r = 0xff; //!< The red component.
	b3_u08 g = 0x11; //!< The green component.
	b3_u08 b = 0x44; //!< The blue component.

public:
	/**
	 * This constructor initializes the instance with default values.
	 */
	b3RGB() = default;

	/**
	 * This method returns the stored color as a b3_pkd_color type.
	 *
	 * \return The color in b3_pkd_color representation.
	 */
	inline operator b3_pkd_color() const noexcept
	{
		return b3Color::b3MakePkdColor(r, g, b);
	}

	/**
	 * This method assignes values from a given b3_pkd_color value
	 * to this class.
	 *
	 * \param color The given color representation.
	 */
	inline 	void          operator=(const b3_pkd_color & color) noexcept
	{
		r = (b3_u08)((color & 0xff0000) >> 16);
		g = (b3_u08)((color & 0x00ff00) >>  8);
		b = (b3_u08)((color & 0x0000ff));
	}

	/**
	 * This method assignes values from a given b3_color value
	 * to this class.
	 *
	 * \param color The given color representation.
	 */
	inline  void          operator=(const b3_color & color) noexcept
	{
		const b3_pkd_color pkd = b3Color(color);

		operator=(pkd);
	}

#ifdef WIN32
	// Problem: COLORREF and b3_pkd_color are of the same type ???
	/*
		inline COLORREF       operator()()
		{
			return RGB(r,g,b);
		}
		inline void           operator=(const COLORREF &color)
		{
			r = (color & 0x0000ff);
			g = (color & 0x00ff00) >>  8;
			b = (color & 0xff0000) >> 16;
		}
	*/
#endif
};

/**
 * This class represents a rectangle.
 */
class B3_PLUGIN b3Rect
{
public:
	/**
	 * The smaller edge coordinate.
	 */
	b3_coord m_x1, m_y1;

	/**
	 * The higher edge coordinate.
	 */
	b3_coord m_x2, m_y2;

public:
	/**
	 * This constructor initializes this instance to zero default values.
	 *
	 * @param x1 The left coordinate.
	 * @param y1 The top coordinate.
	 * @param x2 The right coordinate.
	 * @param y2 The bottom coordinate.
	 */
	explicit b3Rect(
		const b3_coord x1 = 0,
		const b3_coord y1 = 0,
		const b3_coord x2 = 0,
		const b3_coord y2 = 0);

	/**
	 * This method returns the rectangle width. This value may
	 * be negative,
	 *
	 * @return The rectangle width.
	 */
	inline b3_res b3Width() const
	{
		return m_x2 - m_x1;
	}

	/**
	 * This method returns the rectangle height. This value may
	 * be negative,
	 *
	 * @return The rectangle height.
	 */
	inline b3_res b3Height() const
	{
		return m_y2 - m_y1;
	}

	/**
	 * This method adjusts the rectangle of this bound so that the
	 * given ractangle fits into it. So the b3Rect instance would
	 * expand.
	 *
	 * @param rect The rectangle for adjusting.
	 * @return True if any adjustment occured.
	 */
	bool    b3UpdateBound(const b3Rect * rect);
};

/**
 * This class represents a single point.
 */
class B3_PLUGIN b3Point
{
public:
	b3_f64 x; //!< The x coordinate.
	b3_f64 y; //!< The y coordinate.

public:
	/**
	 * This constructor initializes this instance with the
	 * given coordinates.
	 *
	 * @param nx The new x coordinate.
	 * @param ny The new y coordinate.
	 */
	explicit inline b3Point(b3_f64 nx = 0, b3_f64 ny = 0)
	{
		this->x = nx;
		this->y = ny;
	}
};

/**
 * This class provides some useful string operating methods.
 */
class B3_PLUGIN b3StringTool
{
public:
	/**
	 * This method compares two string ignoring their case.
	 *
	 * @param left The left string to compare.
	 * @param right The right string to compare.
	 * @return 0 if both strings are equal ignoring their case. Negative if
	 * the left string ist lexically smaller than the right string and positive
	 * if the right string is lexically smaller.
	 */
	static int b3CaseCompare(
		const std::string & left,
		const std::string & right);

	/**
	 * This converts a std::string to lower case. This method is able to
	 * convert german umlauts.
	 *
	 * @param input The input std::string to convert.
	 * @return The lower case std::string.
	 */
	static std::string b3ToLower(const std::string & input);

	/**
	 * This converts a std::string to upper case. This method is able to
	 * convert german umlauts.
	 *
	 * @param input The input std::string to convert.
	 * @return The upper case std::string.
	 */
	static std::string b3ToUpper(const std::string & input);

};

#endif
