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

extern void b3LongMemSet(b3_u32 *data,b3_count DWordCount,b3_u32 pattern);
extern void b3LongMemCopy(b3_u32 *dst,b3_u32 *src,b3_count DWordCount);

class b3RGB
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

#define B3_WHITE   ((b3_pkd_color)0xffffff)
#define B3_GREY    ((b3_pkd_color)0x808080)
#define B3_BLACK   ((b3_pkd_color)0x000000)
#define B3_RED     ((b3_pkd_color)0xff0000)
#define B3_GREEN   ((b3_pkd_color)0x00ff00)
#define B3_BLUE    ((b3_pkd_color)0x0000ff)
#define B3_YELLOW  ((b3_pkd_color)0xffff00)
#define B3_MAGENTA ((b3_pkd_color)0xff00ff)
#define B3_CYAN    ((b3_pkd_color)0x00ffff)

class b3Color
{
public:
	static inline b3_color *b3Init(b3_color *col,b3_f64 r=0,b3_f64 g=0,b3_f64 b=0,b3_f64 a=0)
	{
		col->a = a;
		col->r = r;
		col->g = g;
		col->b = b;

		return col;
	}

	static inline b3_color *b3Init(b3_color *col,b3_pkd_color input)
	{
		return b3GetColor(col,input);
	}

	static inline b3_pkd_color b3GetColor(const b3_color *color)
	{
		return
			((b3_pkd_color)(color->a * 255) << 24) |
			((b3_pkd_color)(color->r * 255) << 16) |
			((b3_pkd_color)(color->g * 255) <<  8) |
			 (b3_pkd_color)(color->b * 255);
	}

#ifdef WIN32
	static inline COLORREF b3GetColorref(const b3_color *color)
	{
		return RGB(
			(b3_pkd_color)(color->r * 255),
			(b3_pkd_color)(color->g * 255),
			(b3_pkd_color)(color->b * 255));
	}
#endif

	static inline b3_pkd_color b3GetSatColor(const b3_color *color)
	{
		return
			((b3_pkd_color)(color->a > 1.0 ? 255 : (b3_u08)(color->a * 255)) << 24) |
			((b3_pkd_color)(color->r > 1.0 ? 255 : (b3_u08)(color->r * 255)) << 16) |
			((b3_pkd_color)(color->g > 1.0 ? 255 : (b3_u08)(color->g * 255)) <<  8) |
			 (b3_pkd_color)(color->b > 1.0 ? 255 : (b3_u08)(color->b * 255));
	}

#ifdef WIN32
	static inline COLORREF b3GetSatColorref(b3_color *color)
	{
		return RGB(
			(b3_pkd_color)(color->r > 1.0 ? 255 : (b3_u08)(color->r * 255)),
			(b3_pkd_color)(color->g > 1.0 ? 255 : (b3_u08)(color->g * 255)),
			(b3_pkd_color)(color->b > 1.0 ? 255 : (b3_u08)(color->b * 255)));
	}
#endif

	static inline b3_color *b3GetColor(b3_color *result,const b3_pkd_color input)
	{
		result->a = (b3_f64)((input & 0xff000000) >> 24) * 0.0039215686;
		result->r = (b3_f64)((input & 0x00ff0000) >> 16) * 0.0039215686;
		result->g = (b3_f64)((input & 0x0000ff00) >>  8) * 0.0039215686;
		result->b = (b3_f64)((input & 0x000000ff))       * 0.0039215686;

		return result;
	}

#ifdef WIN32
	static inline b3_color *b3GetColorref(b3_color *result,const COLORREF value)
	{
		result->a = 0;
		result->r = (b3_f64)((value & 0x0000ff))       * 0.0039215686;
		result->g = (b3_f64)((value & 0x00ff00) >>  8) * 0.0039215686;
		result->b = (b3_f64)((value & 0xff0000) >> 16) * 0.0039215686;

		return result;
	}
#endif

	static inline b3_color *b3Scale(b3_color *result,const b3_f64 factor)
	{
		result->a *= factor;
		result->r *= factor;
		result->g *= factor;
		result->b *= factor;
		return result;
	}

	static inline b3_color *b3Scale(const b3_color *input,const b3_f64 factor,b3_color *result)
	{
		result->a = input->a * factor;
		result->r = input->r * factor;
		result->g = input->g * factor;
		result->b = input->b * factor;
		return result;
	}

	static inline b3_color *b3Sat(b3_color *input,b3_f64 sat = 1.0)
	{
		if (input->a > sat) input->a = sat;
		if (input->r > sat) input->r = sat;
		if (input->g > sat) input->g = sat;
		if (input->b > sat) input->b = sat;

		return input;
	}

	static inline b3_color *b3Sat(const b3_color *input,b3_color *result,const b3_f64 sat = 1.0)
	{
		result->a = (input->a > sat ? sat : input->a);
		result->r = (input->r > sat ? sat : input->r);
		result->g = (input->g > sat ? sat : input->g);
		result->b = (input->b > sat ? sat : input->b);

		return result;
	}

	static inline b3_color *b3Add(const b3_color *input,b3_color *result)
	{
		result->a += input->a;
		result->r += input->r;
		result->g += input->g;
		result->b += input->b;

		return result;
	}

	static inline b3_color *b3AddScaled(const b3_color *input,b3_color *result,const b3_f64 factor)
	{
		result->a += (input->a * factor);
		result->r += (input->r * factor);
		result->g += (input->g * factor);
		result->b += (input->b * factor);

		return result;
	}

	static inline b3_color *b3LinearCombine(
		const b3_color *coeff2,
		const b3_color *coeff1,
		const b3_color *coeff0,
		const b3_f64    factor1,
		const b3_f64    factor0,
		      b3_color *result)
	{
		result->a = coeff2->a + factor1 * coeff1->a + factor0 * coeff0->a;
		result->r = coeff2->r + factor1 * coeff1->r + factor0 * coeff0->r;
		result->g = coeff2->g + factor1 * coeff1->g + factor0 * coeff0->g;
		result->b = coeff2->b + factor1 * coeff1->b + factor0 * coeff0->b;
		return result;
	}
};

class b3Rect
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
