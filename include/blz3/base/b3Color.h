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
#define B3_MARKER      ((b3_pkd_color)0x00ff1144)
#define B3_TRANSPARENT ((b3_pkd_color)0xff000000)

class B3_PLUGIN b3Color
{
	b3_f32 B3_ALIGN_16 v[4];

	static b3_f32 B3_ALIGN_16 m_Limit_m000[4];
	static b3_f32 B3_ALIGN_16 m_Limit_m255[4];
	static b3_f32 B3_ALIGN_16 m_Limit_d255[4];

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

	inline b3Color(const b3_f32 init)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] = init;
		}
	}

	inline b3Color(
		b3_f64 r,
		b3_f64 g,
		b3_f64 b,
		b3_f64 a = 0)
	{
		b3Init(r,g,b,a);
	}

	inline b3Color(const b3Color &color)
	{
		for (b3_loop i = 0;i < 4;i++)
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
		b3_pkd_color       color = input;
		b3_s32 B3_ALIGN_16 c[4];
		b3_loop                i;

		for (i = 0;i < 4;i++)
		{
			c[3-i] = color & 0xff;
			color  = color >> 8;
		}

		for (i = 0;i < 4;i++)
		{
			v[i] = (b3_f32)c[i];
		}

		for (i = 0;i < 4;i++)
		{
			v[i] *= m_Limit_d255[i];
		}
	}

	inline void b3SetAlpha(const b3_f64 alpha)
	{
		v[A] = alpha;
	}

	inline b3_f32 &operator[](const b3_color_index index)
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
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] += a.v[i];
		}
		return *this;
	}

	inline b3Color operator+(const b3Color &a)
	{
		b3Color result;

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] + a.v[i];
		}
		return result;
	}

	inline b3Color &operator-=(const b3Color &a)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] -= a.v[i];
		}
		return *this;
	}

	inline b3Color operator-(const b3Color &a)
	{
		b3Color result;

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] - a.v[i];
		}
		return result;
	}

	inline b3Color &operator*=(const b3Color &a)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] *= a.v[i];
		}
		return *this;
	}

	inline b3Color operator*(const b3Color &a)
	{
		b3Color result;

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * a.v[i];
		}
		return result;
	}

	inline b3Color &operator*=(const b3_f32 value)
	{
		b3Color prod(value);

		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] *= prod.v[i];
		}
		return *this;
	}

	inline b3Color &operator*=(const b3_f64 value)
	{
		b3Color prod((b3_f32)value);

		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] *= prod.v[i];
		}
		return *this;
	}

	inline b3Color operator*(const b3_f32 value)
	{
		b3Color result,multiplicator(value);

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * multiplicator.v[i];
		}
		return result;
	}

	inline b3Color operator*(const b3_f64 value)
	{
		b3Color result,multiplicator((b3_f32)value);

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * multiplicator.v[i];
		}
		return result;
	}

	inline b3Color &operator/=(const b3_f32 value)
	{
		b3Color prod(value);

		B3_ASSERT(value != 0);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	inline b3Color &operator/=(const b3_f64 value)
	{
		b3Color prod((b3_f32)value);

		B3_ASSERT(value != 0);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	inline b3Color &operator/=(const b3_count value)
	{
		b3Color divisor((b3_f32)value);

		B3_ASSERT(value != 0);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] /= divisor.v[i];
		}
		return *this;
	}

	inline b3Color operator/(const b3_f32 value)
	{
		b3Color result,divisor(value);

		B3_ASSERT(value != 0);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	inline b3Color operator/(const b3_f64 value)
	{
		b3Color result,divisor((b3_f32)value);

		B3_ASSERT(value != 0);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	inline b3Color operator/(const b3_count value)
	{
		b3Color result;
		b3Color divisor((b3_f32)value);

		B3_ASSERT(value != 0);
		for (b3_loop i = 0;i < 4;i++)
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
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] = fabs(v[i]);
		}
	}

	inline operator b3_pkd_color()
	{
		b3_pkd_color       result = 0;
		b3_loop                i;
		b3_s32 B3_ALIGN_16 c[4];
		b3_f32 B3_ALIGN_16 sat[4];

		for (i = 0;i < 4;i++)
		{
			sat[i] = v[i] * m_Limit_m255[i];
			if (sat[i] < m_Limit_m000[i])
			{
				sat[i] = m_Limit_m000[i];
			}
			if (sat[i] > m_Limit_m255[i])
			{
				sat[i] = m_Limit_m255[i];
			}
		}

		for (i = 0;i < 4;i++)
		{
			c[i] = (b3_s32)sat[i];
		}

		for (i = 0;i < 4;i++)
		{
			result = (result << 8) | c[i];
		}
		return result;
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

	inline void b3Sat(b3_f32 sat = 1.0)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			if (v[i] > sat)
			{
				v[i] = sat;
			}
		}
	}

	inline void b3Min(b3_f32 min = 0.0)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			if (v[i] < min)
			{
				v[i] = min;
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
