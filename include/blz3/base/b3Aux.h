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

#ifndef B3_IMAGE_AUX_H
#define B3_IMAGE_AUX_H

#include "blz3/b3Config.h"

class B3_PLUGIN b3RGB
{
public:
	b3_u08 r,g,b;
public:

	              b3RGB();
	inline b3_pkd_color  operator()()
	{
		return (
			((b3_pkd_color)r << 16) |
			((b3_pkd_color)g <<  8) |
			 (b3_pkd_color)b);
	}
	inline               operator b3_pkd_color()
	{
		return (
			((b3_pkd_color)r << 16) |
			((b3_pkd_color)g <<  8) |
			 (b3_pkd_color)b);
	}
	inline 	void          operator=(const b3_pkd_color &color)
	{
		r = (b3_u08)((color & 0xff0000) >> 16);
		g = (b3_u08)((color & 0x00ff00) >>  8);
		b = (b3_u08)((color & 0x0000ff));
	}
	inline  void          operator=(const b3_color &color)
	{
		r = (color.r > 1.0 ? 255 : (b3_u08)(color.r * 255));
		g = (color.g > 1.0 ? 255 : (b3_u08)(color.g * 255));
		b = (color.b > 1.0 ? 255 : (b3_u08)(color.b * 255));
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

class B3_PLUGIN b3Rect
{
public:
	b3_coord x1,y1;
	b3_coord x2,y2;

public:
	        b3Rect();
	b3_res  b3Width();
	b3_res  b3Height();
	b3_bool b3UpdateBound(b3Rect *);
	b3_bool b3CheckBound(b3Rect *);
};

#endif
