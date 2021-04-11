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
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_IMAGE_AUX_H
#define B3_IMAGE_AUX_H

#include "blz3/base/b3Color.h"

/**
 * This class represents a RGB color triplet.
 */
class B3_PLUGIN b3RGB
{
public:
	b3_u08 r; //!< The red component.
	b3_u08 g; //!< The green component.
	b3_u08 b; //!< The blue component.

public:
	/**
	 * This constructor initializes the instance with default values.
	 */
	b3RGB();

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
	b3_res  b3Width() const;

	/**
	 * This method returns the rectangle height. This value may
	 * be negative,
	 *
	 * @return The rectangle height.
	 */
	b3_res  b3Height() const;

	/**
	 * This method adjusts the rectangle of this bound so that the
	 * given ractangle fits into it.
	 *
	 * @param rect The rectangle for adjusting.
	 * @return True if any adjustment occured.
	 */
	bool    b3UpdateBound(b3Rect * rect);

	/**
	 * This method adjusts the rectangle of this bound so that the
	 * given ractangle fits into it. Additionally the coordinates
	 * are adjusted to at least zero dimension.
	 *
	 * @param rect The rectangle for adjusting.
	 * @return True if any adjustment occured.
	 */
	bool    b3CheckBound(b3Rect * rect);
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

#endif
