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
	b3_pkd_color  operator()();
	              operator b3_pkd_color();
	void          operator=(const b3_pkd_color &);
};

class b3Color
{
public:
	static inline b3_pkd_color b3GetColor(b3_color *color)
	{
		register b3_pkd_color r,g,b;

		r = (b3_pkd_color)(color->r * 255);
		g = (b3_pkd_color)(color->g * 255);
		b = (b3_pkd_color)(color->b * 255);
		return (r << 16) | (g << 8) | b;
	}

	static inline b3_pkd_color b3GetSatColor(b3_color *color)
	{
		register b3_pkd_color r,g,b;

		r = (b3_pkd_color)(color->r * 255);
		g = (b3_pkd_color)(color->g * 255);
		b = (b3_pkd_color)(color->b * 255);
		return
			((r > 255 ? 255 : r) << 16) |
			((g > 255 ? 255 : g) <<  8) |
			 (b > 255 ? 255 : b);
	}

	static inline b3_color *b3GetColor(b3_color *color,b3_pkd_color input)
	{
		color->r = (b3_f64)((input & 0xff0000) >> 16) * 0.0039215686;
		color->g = (b3_f64)((input & 0x00ff00) >>  8) * 0.0039215686;
		color->b = (b3_f64)((input & 0x0000ff))       * 0.0039215686;

		return color;
	}

	static inline b3_color *b3Scale(b3_color *color,b3_f64 factor)
	{
		color->a *= factor;
		color->r *= factor;
		color->g *= factor;
		color->b *= factor;
		return color;
	}

	static inline b3_color *b3Scale(b3_color *input,b3_f64 factor,b3_color *result)
	{
		result->a = input->a * factor;
		result->r = input->r * factor;
		result->g = input->g * factor;
		result->b = input->b * factor;
		return result;
	}

	static inline b3_color *b3Add(b3_color *input,b3_color *result)
	{
		result->a += input->a;
		result->r += input->r;
		result->g += input->g;
		result->b += input->b;

		return result;
	}

	static inline b3_color *b3AddScaled(b3_color *input,b3_color *result,b3_f64 factor)
	{
		result->a += (input->a * factor);
		result->r += (input->r * factor);
		result->g += (input->g * factor);
		result->b += (input->b * factor);

		return result;
	}

	static inline b3_color *b3LinearCombine(
		b3_color *coeff2,
		b3_color *coeff1,
		b3_color *coeff0,
		b3_f64    factor1,
		b3_f64    factor0,
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
