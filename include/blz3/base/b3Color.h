/*
**
**      $Filename:      b3Color.h $
**      $Release:       Dortmund 2003 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Color handling
**
**      (C) Copyright  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_COLOR_H
#define B3_BASE_COLOR_H

#include "blz3/b3Config.h"

#define B3_WHITE       ((b3_pkd_color)0x00ffffff)
#define B3_GREY        ((b3_pkd_color)0x00808080)
#define B3_BLACK       ((b3_pkd_color)0x00000000)
#define B3_RED         ((b3_pkd_color)0x00ff0000)
#define B3_GREEN       ((b3_pkd_color)0x0000ff00)
#define B3_BLUE        ((b3_pkd_color)0x000000ff)
#define B3_YELLOW      ((b3_pkd_color)0x00ffff00)
#define B3_MAGENTA     ((b3_pkd_color)0x00ff00ff)
#define B3_CYAN        ((b3_pkd_color)0x0000ffff)
#define B3_TRANSPARENT ((b3_pkd_color)0xff000000)

class b3Color
{
	b3_f32 B3_ALIGN_16 v[4];

public:
	enum b3_color_index
	{
		A = 0,
		R,
		G,
		B
	};

public:
	inline b3Color()
	{
	}

	inline b3Color(
		b3_f64 r,
		b3_f64 g,
		b3_f64 b,
		b3_f64 a = 0)
	{
		b3Init(r,g,b,a);
	}

	inline void b3SetAlpha(const b3_f64 alpha)
	{
		v[A] = alpha;
	}

	inline b3Color(const b3Color &color)
	{
		for (int i = 0;i < 4;i++)
		{
			v[i] = color.v[i];
		}
	}

	inline b3Color(const b3_color &color)
	{
		b3Init(color.r,color.g,color.b,color.a);
	}

	inline b3Color(const b3_color *color)
	{
		b3Init(color->r,color->g,color->b,color->a);
	}

	inline b3Color(const b3_pkd_color input)
	{
		b3_pkd_color mask  = 0xff000000;
		int          shift = 24;

		for (int i = 0;i < 4;i++)
		{
			v[i] = (b3_f32)((input & mask) >> shift) * 0.0039215686f;
			mask >>= 8;
			shift -= 8;
		}
	}

	inline b3_f32 operator[](const b3_color_index index)
	{
		return v[index];
	}

	inline void b3Init(
		b3_f64 r = 0,
		b3_f64 g = 0,
		b3_f64 b = 0,
		b3_f64 a = 0)
	{
		v[R] = (b3_f32)r;
		v[G] = (b3_f32)g;
		v[B] = (b3_f32)b;
		v[A] = (b3_f32)a;
	}

	inline static b3_color *b3Init(b3_color *color,
		b3_f64 r = 0,
		b3_f64 g = 0,
		b3_f64 b = 0,
		b3_f64 a = 0)
	{
		color->r = (b3_f32)r;
		color->g = (b3_f32)g;
		color->b = (b3_f32)b;
		color->a = (b3_f32)a;
		return color;
	}

	inline b3Color &operator+=(const b3Color &a)
	{
		for (int i = 0;i < 4;i++)
		{
			v[i] += a.v[i];
		}
		return *this;
	}

	inline b3Color operator+(const b3Color &a)
	{
		b3Color result;

		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] + a.v[i];
		}
		return result;
	}

	inline b3Color &operator-=(const b3Color &a)
	{
		for (int i = 0;i < 4;i++)
		{
			v[i] -= a.v[i];
		}
		return *this;
	}

	inline b3Color operator-(const b3Color &a)
	{
		b3Color result;

		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] - a.v[i];
		}
		return result;
	}

	inline b3Color &operator*=(const b3Color &a)
	{
		for (int i = 0;i < 4;i++)
		{
			v[i] *= a.v[i];
		}
		return *this;
	}

	inline b3Color operator*(const b3Color &a)
	{
		b3Color result;

		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * a.v[i];
		}
		return result;
	}

	inline b3Color &operator*=(const b3_f32 value)
	{
		b3Color prod(value,value,value,value);

		for (int i = 0;i < 4;i++)
		{
			v[i] *= prod.v[i];
		}
		return *this;
	}

	inline b3Color &operator*=(const b3_f64 value)
	{
		b3Color prod(value,value,value,value);

		for (int i = 0;i < 4;i++)
		{
			v[i] *= prod.v[i];
		}
		return *this;
	}

	inline b3Color operator*(const b3_f32 value)
	{
		b3Color result,multiplicator(value,value,value,value);

		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * multiplicator.v[i];
		}
		return result;
	}

	inline b3Color operator*(const b3_f64 value)
	{
		b3Color result,multiplicator(value,value,value,value);

		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * multiplicator.v[i];
		}
		return result;
	}

	inline b3Color &operator/=(const b3_f32 value)
	{
		b3Color prod(value,value,value,1);

		B3_ASSERT(value != 0);
		for (int i = 0;i < 4;i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	inline b3Color operator/(const b3_f32 value)
	{
		b3Color result,divisor(value,value,value,1);

		B3_ASSERT(value != 0);
		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	inline b3Color &operator/=(const b3_f64 value)
	{
		b3Color prod(value,value,value,1);

		B3_ASSERT(value != 0);
		for (int i = 0;i < 4;i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	inline b3Color operator/(const b3_f64 value)
	{
		b3Color result,divisor(value,value,value,1);

		B3_ASSERT(value != 0);
		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	inline b3Color operator/(const b3_count value)
	{
		b3Color result,divisor(value,value,value,1);

		B3_ASSERT(value != 0);
		for (int i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	inline b3_bool b3IsGreater(b3Color &limit)
	{
		return (fabs(v[R]) >= limit[R]) || (fabs(v[G]) >= limit[G]) || (fabs(v[B]) >= limit[B]);
	}

	inline void b3Abs()
	{
		for (int i = 0;i < 4;i++)
		{
			v[i] = fabs(v[i]);
		}
	}

	inline operator b3_pkd_color()
	{
		return
			((b3_pkd_color)(v[A] > 1.0 ? 255 : (b3_u08)(v[A] * 255)) << 24) |
			((b3_pkd_color)(v[R] > 1.0 ? 255 : (b3_u08)(v[R] * 255)) << 16) |
			((b3_pkd_color)(v[G] > 1.0 ? 255 : (b3_u08)(v[G] * 255)) <<  8) |
			 (b3_pkd_color)(v[B] > 1.0 ? 255 : (b3_u08)(v[B] * 255));
	}

	inline operator b3_color()
	{
		b3_color result;

		result.r = v[R];
		result.g = v[G];
		result.b = v[B];
		result.a = v[A];
		return result;
	}

	inline void b3Sat(b3_f64 sat = 1.0)
	{
		for (int i = 0;i < 4;i++)
		{
			if (v[i] > sat)
			{
				v[i] = sat;
			}
		}
	}

#ifdef WIN32
	inline COLORREF b3GetColorref()
	{
		return RGB(
			(b3_pkd_color)(v[R] * 255),
			(b3_pkd_color)(v[G] * 255),
			(b3_pkd_color)(v[B] * 255));
	}

	inline void b3SetColorref(const COLORREF value)
	{
		b3Init(
			(b3_f64)((value & 0x0000ff))       * 0.0039215686,
			(b3_f64)((value & 0x00ff00) >>  8) * 0.0039215686,
			(b3_f64)((value & 0xff0000) >> 16) * 0.0039215686);
	}
#endif
};

#endif
