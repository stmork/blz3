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
#define B3_LIGHT_GREY  ((b3_pkd_color)0x00c0c0c0)
#define B3_GREY        ((b3_pkd_color)0x00808080)
#define B3_DARK_GREY   ((b3_pkd_color)0x00404040)
#define B3_BLACK       ((b3_pkd_color)0x00000000)
#define B3_DARK_RED    ((b3_pkd_color)0x00800000)
#define B3_RED         ((b3_pkd_color)0x00ff0000)
#define B3_LIGHT_RED   ((b3_pkd_color)0x00ff4040)
#define B3_DARK_GREEN  ((b3_pkd_color)0x00008000)
#define B3_GREEN       ((b3_pkd_color)0x0000ff00)
#define B3_LIGHT_GREEN ((b3_pkd_color)0x0040ff40)
#define B3_DARK_BLUE   ((b3_pkd_color)0x00000080)
#define B3_BLUE        ((b3_pkd_color)0x000000ff)
#define B3_LIGHT_BLUE  ((b3_pkd_color)0x004040ff)
#define B3_YELLOW      ((b3_pkd_color)0x00ffff00)
#define B3_MAGENTA     ((b3_pkd_color)0x00ff00ff)
#define B3_CYAN        ((b3_pkd_color)0x0000ffff)
#define B3_PEARL       ((b3_pkd_color)0x00a4aeb7)
#define B3_MARKER      ((b3_pkd_color)0x00ff1144)
#define B3_TRANSPARENT ((b3_pkd_color)0xff000000)

/**
 * This class provides color handling. It uses modern command sets
 * like SSE if the compiler can generate this and the underlying cpu
 * architecture supports it.
 */
class B3_PLUGIN b3Color
{
	b3_f32 B3_ALIGN_16 v[4];                   //!< These are the color channels of a b3Color instance.

	static b3_f32 B3_ALIGN_16 m_Limit_m000[4]; //!< These color values represent black (for clamping).
	static b3_f32 B3_ALIGN_16 m_Limit_m001[4]; //!< These color values represent transparent white (for saturating).
	static b3_f32 B3_ALIGN_16 m_Limit_m255[4]; //!< These values are used for conversion into integer.
	static b3_f32 B3_ALIGN_16 m_Limit_d255[4]; //!< These values are used for conversion from integer.

public:
	/**
	 * This enum references the seperate color channels.
	 */
	enum b3_color_index
	{
		A = 0, //!< The alpha channel.
		R,     //!< The red channel.
		G,     //!< The green channel.
		B      //!< The blue channel.
	};

public:
	/////////////////////////////////////////////////--------  constructors
	/**
	 * This constructor does simply nothing.
	 */
	inline b3Color()
	{
	}

	/**
	 * This constructor initializes this instance.
	 *
	 * @param r The new red channel.
	 * @param g The new green channel.
	 * @param b The new blue channel.
	 * @param a The new alpha channel.
	 */
	inline b3Color(
		b3_f32 r,
		b3_f32 g,
		b3_f32 b,
		b3_f32 a = 0)
	{
		b3Init(r,g,b,a);
	}

	/**
	 * This copy constructor copies another b3Color instance into
	 * this instance.
	 *
	 * @param color The other b3Color instance to copy.
	 */
	inline b3Color(const b3Color &color)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] = color.v[i];
		}
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	inline b3Color(const b3_color &color)
	{
		b3Init(color.r,color.g,color.b,color.a);
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	inline b3Color(const b3_color *color)
	{
		b3Init(color->r,color->g,color->b,color->a);
	}

	/**
	 * This constructor initializes this instance from a ::b3_pkd_color structure.
	 *
	 * @param input The other ::b3_pkd_color instance to copy.
	 */
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

	//////////////////////////////////////////--------- initializers
	/**
	 * This method initializes all channels of this instance
	 * to zero.
	 */
	inline void b3Init()
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] = 0;
		}
	}

	/**
	 * This method initializes all channels of this instance
	 * with the same value. This is for filtering purposes.
	 *
	 * @param value The filter value as single precision floating point number.
	 */
	inline void b3InitFactor(b3_f32 value)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] = value;
		}
	}

	/**
	 * This method initializes all channels of this instance
	 * with the same value. This is for filtering purposes.
	 *
	 * @param dvalue The filter value as double precision floating point number.
	 */
	inline void b3InitFactor(b3_f64 dvalue)
	{
		b3_f32 value = (b3_f32)dvalue;

		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] = value;
		}
	}

	/**
	 * This method initializes the instance with a grey color.
	 *
	 * @param rgb The new grey level.
	 * @param a   The new alpha channel.
	 */
	inline void b3Init(b3_f32 rgb, b3_f32 a = 0)
	{
		v[R] =
		v[G] =
		v[B] = rgb;
		v[A] = a;
	}

	/**
	 * This method initializes this instance.
	 *
	 * @param r The new red channel.
	 * @param g The new green channel.
	 * @param b The new blue channel.
	 * @param a The new alpha channel.
	 */
	inline void b3Init(
		b3_f32 r,
		b3_f32 g,
		b3_f32 b,
		b3_f32 a = 0)
	{
		v[R] = r;
		v[G] = g;
		v[B] = b;
		v[A] = a;
	}

	/**
	 * This method initializes a ::b3_color structure instance.
	 *
	 * @param color The other ::b3_color instance to copy.
	 * @param r The new red channel.
	 * @param g The new green channel.
	 * @param b The new blue channel.
	 * @param a The new alpha channel.
	 * @return The ::b3_color instance pointer.
	 */
	inline static b3_color *b3Init(b3_color *color,
		b3_f32 r = 0,
		b3_f32 g = 0,
		b3_f32 b = 0,
		b3_f32 a = 0)
	{
		color->r = r;
		color->g = g;
		color->b = b;
		color->a = a;
		return color;
	}

	//////////////////////////////////////--------- methods and operators
	/**
	 * This method sets the alpha channel.
	 *
	 * @param alpha The new alpha channel.
	 */
	inline void b3SetAlpha(const b3_f32 alpha)
	{
		v[A] = alpha;
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 &operator[](const b3_color_index index)
	{
		return v[index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 &operator[](const int index)
	{
		return v[(b3_color_index)index];
	}

	/**
	 * This method mixes two colors by a mixer component. The mixer must in the
	 * range from [0..1].
	 *
	 * @param low The low component if the mixer is 0.
	 * @param high The high component if the mixer is 1.
	 * @param mix The mixer component as single precision floating point number.
	 * @return The resulting mixed color.
	 */
	inline static b3Color b3Mix(const b3Color &low,const b3Color &high,b3_f32 mix)
	{
		b3Color result;
		b3Color mixer;

		mixer.b3InitFactor(mix);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = low.v[i] + mixer.v[i] * (high.v[i]  - low.v[i]);
		}
		return result;
	}

	/**
	 * This method mixes two colors by a mixer component. The mixer must in the
	 * range from [0..1].
	 *
	 * @param low The low component if the mixer is 0.
	 * @param high The high component if the mixer is 1.
	 * @param mix The mixer component as double precision floating point number.
	 * @return The resulting mixed color.
	 */
	inline static b3Color b3Mix(const b3Color &low,const b3Color &high,b3_f64 mix)
	{
		b3Color result;
		b3Color mixer;

		mixer.b3InitFactor(mix);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = low.v[i] + mixer.v[i] * (high.v[i]  - low.v[i]);
		}
		return result;
	}

	/**
	 * This method mixes two colors by a mixer component. The mixer must in the
	 * range from [0..1].
	 *
	 * @param low The low component if the mixer is 0.
	 * @param high The high component if the mixer is 1.
	 * @param mixer The mixer component as a color component. Each channel is mixed seperatly.
	 * @return The resulting mixed color.
	 */
	inline static b3Color b3Mix(const b3Color &low,const b3Color &high,b3Color &mixer)
	{
		b3Color result;

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = low.v[i] + mixer.v[i] * (high.v[i]  - low.v[i]);
		}
		return result;
	}

	/**
	 * This operator adds a specified color to this instance.
	 *
	 * @param a The color to add.
	 * @return A reference to this instance.
	 */
	inline b3Color &operator+=(const b3Color &a)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] += a.v[i];
		}
		return *this;
	}

	/**
	 * This operator adds a color to this instance resulting in a new instance.
	 *
	 * @param a The color to add.
	 * @return A new b3Color instance.
	 */
	inline b3Color operator+(const b3Color &a)
	{
		b3Color result;

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] + a.v[i];
		}
		return result;
	}

	/**
	 * This operator subtracts a specified color from this instance.
	 *
	 * @param a The color to subtract.
	 * @return A reference to this instance.
	 */
	inline b3Color &operator-=(const b3Color &a)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] -= a.v[i];
		}
		return *this;
	}

	/**
	 * This operator subtracts a color from this instance resulting in a new instance.
	 *
	 * @param a The color to subtract.
	 * @return A new b3Color instance.
	 */
	inline b3Color operator-(const b3Color &a)
	{
		b3Color result;

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] - a.v[i];
		}
		return result;
	}

	/**
	 * This operator filters this instance with by a specified color.
	 *
	 * @param a The color filter.
	 * @return A reference to this color.
	 */
	inline b3Color &operator*=(const b3Color &a)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] *= a.v[i];
		}
		return *this;
	}

	/**
	 * This operator filters this instance with by a specified color resulting in a new instance.
	 *
	 * @param a The color filter.
	 * @return A new b3Color instance.
	 */
	inline b3Color operator*(const b3Color &a)
	{
		b3Color result;

		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * a.v[i];
		}
		return result;
	}

	/**
	 * This operator filters this instance with a specified value.
	 *
	 * @param value The color filter value.
	 * @return A reference to this color.
	 */
	inline b3Color &operator*=(const b3_f32 value)
	{
		b3Color prod;

		prod.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] *= prod.v[i];
		}
		return *this;
	}

	/**
	 * This operator filters this instance with a specified value.
	 *
	 * @param value The color filter value.
	 * @return A reference to this color.
	 */
	inline b3Color &operator*=(const b3_f64 value)
	{
		b3Color prod;

		prod.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] *= prod.v[i];
		}
		return *this;
	}

	/**
	 * This operator filters this instance with a specified value resulting in a new instance.
	 *
	 * @param value The color filter value.
	 * @return The new b3Color instance.
	 */
	inline b3Color operator*(const b3_f32 value)
	{
		b3Color result,multiplicator;

		multiplicator.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * multiplicator.v[i];
		}
		return result;
	}

	/**
	 * This operator filters this instance with a specified value resulting in a new instance.
	 *
	 * @param value The color filter value.
	 * @return The new b3Color instance.
	 */
	inline b3Color operator*(const b3_f64 value)
	{
		b3Color result,multiplicator;

		multiplicator.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] * multiplicator.v[i];
		}
		return result;
	}

	/**
	 * This operator divides the color channels by a given scalar.
	 *
	 * @param value The divisor.
	 * @return A reference to this instance.
	 */
	inline b3Color &operator/=(const b3_f32 value)
	{
		b3Color prod;

		B3_ASSERT(value != 0);
		prod.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	/**
	 * This operator divides the color channels by a given scalar.
	 *
	 * @param value The divisor.
	 * @return A reference to this instance.
	 */
	inline b3Color &operator/=(const b3_f64 value)
	{
		b3Color prod;

		B3_ASSERT(value != 0);
		prod.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	/**
	 * This operator divides the color channels by a given scalar.
	 *
	 * @param value The divisor.
	 * ¶return A reference to this instance.
	 */
	inline b3Color &operator/=(const b3_count value)
	{
		b3Color divisor;

		B3_ASSERT(value != 0);
		divisor.b3InitFactor((b3_f32)value);
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] /= divisor.v[i];
		}
		return *this;
	}

	/**
	 * This operator divides the color channels by a given scalar resulting in a new b3Color instance.
	 *
	 * @param value The divisor.
	 * @return The resulting color.
	 */
	inline b3Color operator/(const b3_f32 value)
	{
		b3Color result,divisor;

		B3_ASSERT(value != 0);
		divisor.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	/**
	 * This operator divides the color channels by a given scalar resulting in a new b3Color instance.
	 *
	 * @param value The divisor.
	 * @return The resulting color.
	 */
	inline b3Color operator/(const b3_f64 value)
	{
		b3Color result,divisor;

		B3_ASSERT(value != 0);
		divisor.b3InitFactor(value);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	/**
	 * This operator divides the color channels by a given scalar resulting in a new b3Color instance.
	 *
	 * @param value The divisor.
	 * @return The resulting color.
	 */
	inline b3Color operator/(const b3_count value)
	{
		b3Color result,divisor;

		B3_ASSERT(value != 0);
		divisor.b3InitFactor((b3_f32)value);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	/**
	 * This method computes the power on all color channels with the given exponent.
	 * This is useful for specular color computation.
	 *
	 * @param exp The exponent.
	 * @return The resulting color.
	 */
	inline b3Color b3Pow(b3_f32 exp)
	{
		b3Color result,exponent;
		
		exponent.b3Init(exp);
		for (b3_loop i = 0;i < 4;i++)
		{
			result.v[i] = pow(v[i],exponent.v[i]);
		}
		return result;
	}

	/**
	 * This method returns true if any color channel is greater than any color
	 * channel in the given b3Color instance.
	 *
	 * @param limit The b3Color instance to compare to.
	 * @return True if any color channel overrides the limit.
	 */
	inline b3_bool b3IsGreater(b3Color &limit)
	{
		return (fabs(v[R]) >= limit[R]) || (fabs(v[G]) >= limit[G]) || (fabs(v[B]) >= limit[B]);
	}

	/**
	 * This method removes all negative signs on any color channel.
	 */
	inline void b3Abs()
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			v[i] = fabs(v[i]);
		}
	}

	/**
	 * This cast operator returns the color channels as ::b3_pkd_color.
	 *
	 * @return The color channels as ::b3_pkd_color.
	 */
	inline operator b3_pkd_color()
	{
		b3_pkd_color       result = 0;
		b3_loop                i;
		b3_s32 B3_ALIGN_16 c[4];
		b3_f32 B3_ALIGN_16 sat[4];

		for (i = 0;i < 4;i++)
		{
			sat[i] = v[i];
			if (sat[i] < m_Limit_m000[i])
			{
				sat[i] = m_Limit_m000[i];
			}
			if (sat[i] > m_Limit_m001[i])
			{
				sat[i] = m_Limit_m001[i];
			}
			sat[i] *= m_Limit_m255[i];
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

	/**
	 * This cast operator returns the color channels as ::b3_color.
	 *
	 * @return The color channels as ::b3_color.
	 */
	inline operator b3_color()
	{
		b3_color result;

		result.r = v[R];
		result.g = v[G];
		result.b = v[B];
		result.a = v[A];
		return result;
	}

	/**
	 * This method saturates all color channels to 1.
	 */
	inline void b3Sat()
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			if (v[i] > m_Limit_m001[i])
			{
				v[i] = m_Limit_m001[i];
			}
		}
	}

	/**
	 * This method saturates all color channels to a given value.
	 *
	 * @param sat The saturation value.
	 */
	inline void b3Sat(b3_f32 sat)
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			if (v[i] > sat)
			{
				v[i] = sat;
			}
		}
	}

	/**
	 * This method bottom clamps all color channels to 0.
	 */
	inline void b3Min()
	{
		for (b3_loop i = 0;i < 4;i++)
		{
			if (v[i] < m_Limit_m000[i])
			{
				v[i] = m_Limit_m000[i];
			}
		}
	}

	/**
	 * This method bottom clamps all color channels to a given value.
	 *
	 * @param min The minimum value for each color channel.
	 */
	inline void b3Min(b3_f32 min)
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
	/**
	 * This method returns the color values as ::COLORREF,
	 *
	 * @return The colors as ::COLORREF.
	 */
	inline COLORREF b3GetColorref()
	{
		return RGB(
			(b3_pkd_color)(v[R] * 255),
			(b3_pkd_color)(v[G] * 255),
			(b3_pkd_color)(v[B] * 255));
	}

	/**
	 * This method initializes this instance from a COLORREF.
	 *
	 * @param value The COLORREF value to set.
	 */
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
