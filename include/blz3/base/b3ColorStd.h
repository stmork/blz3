/*
**
**	$Filename:	b3Color.h $
**	$Release:	Dortmund 2005, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Color handling with standard methods.
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_BASE_COLORSTD_H
#define B3_BASE_COLORSTD_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Color.h"

#include <string>

/**
 * This class provides color handling. It uses modern command sets
 * like SSE if the compiler can generate this and the underlying cpu
 * architecture supports it.
 */
class B3_PLUGIN alignas(16) b3Color : public b3ColorBase
{
	b3_f32 v[4] {};           //!< These are the color channels of a b3Color instance.

public:
	/*************************************************************************
	**                                                                      **
	**                        Constructors                                  **
	**                                                                      **
	*************************************************************************/

	/**
	 * This constructor does simply nothing.
	 */
	explicit inline b3Color()
	{
#ifdef _DEBUG
		b3Init(1.0f, 0.125f, 0.25f);
#endif
	}

	/**
	 * This constructor initializes this instance.
	 *
	 * @param r The new red channel.
	 * @param g The new green channel.
	 * @param b The new blue channel.
	 * @param a The new alpha channel.
	 */
	explicit constexpr b3Color(
		const b3_f32 r,
		const b3_f32 g,
		const b3_f32 b,
		const b3_f32 a = 0)
	{
		b3Init(r, g, b, a);
	}

	/**
	 * This copy constructor copies another b3Color instance into
	 * this instance.
	 *
	 * @param color The other b3Color instance to copy.
	 */
	constexpr b3Color(const b3Color & color)
	{
		operator=(color);
	}

	/**
	 * This move constructor moves another b3Color instance into
	 * this instance.
	 *
	 * @param color The other b3Color instance to copy.
	 */
	inline b3Color(b3Color && color)
	{
		operator=(color);
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	constexpr b3Color(const b3_color & color)
	{
		b3Init(color.r, color.g, color.b, color.a);
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	explicit constexpr b3Color(const b3_color * color)
	{
		b3Init(color->r, color->g, color->b, color->a);
	}

	/**
	 * This constructor initializes this instance with equal color values
	 * and a different alpha value.
	 *
	 * @param rgb The equal color components.
	 * @param a The alpha component.
	 */
	explicit constexpr b3Color(const b3_f32 rgb, const b3_f32 a = 0.0)
	{
		b3Init(rgb, a);
	}

	/**
	 * This constructor initializes this instance from a ::b3_pkd_color
	 * structure.
	 *
	 * @param input The other ::b3_pkd_color instance to copy.
	 */
	explicit constexpr b3Color(const b3_u16 input)
	{
		b3_u16             color = input;
		alignas(16) b3_s32 c[4] {};

		for (b3_loop i = 0; i < 4; i++)
		{
			c[3 - i] = color & 0xf;
			color    = color >> 4;
		}

		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] = (b3_f32)c[i];
		}

		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] /= 15.0;
		}
	}

	/**
	 * This constructor initializes this instance from a ::b3_pkd_color
	 * structure.
	 *
	 * @param input The other ::b3_pkd_color instance to copy.
	 */
	constexpr b3Color(const b3_pkd_color input)
	{
		operator=(input);
	}

	/*************************************************************************
	**                                                                      **
	**                        Assignment operators                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * This copy operator copies another b3Color instance into
	 * this instance.
	 *
	 * @param color The other b3Color instance to copy.
	 */
	constexpr b3Color & operator=(const b3Color & color)
	{
		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] = color.v[i];
		}
		return *this;
	}

	/**
	 * This move operator moves another b3Color instance into
	 * this instance.
	 *
	 * @param color The other b3Color instance to copy.
	 */
	constexpr b3Color & operator=(b3Color && color)
	{
		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] = color.v[i];
		}
		return *this;
	}

	/**
	 * This copy operator copies another b3_color instance into
	 * this instance.
	 *
	 * @param color The other b3Color instance to copy.
	 */
	inline b3Color & operator=(const b3_color & color)
	{
		b3Init(color.r, color.g, color.b, color.a);

		return *this;
	}
	/**
	 * This assignment operator initializes this instance from a ::b3_pkd_color
	 * structure.
	 *
	 * @param input The other ::b3_pkd_color instance to copy.
	 */
	constexpr b3Color & operator=(const b3_pkd_color input)
	{
		b3_pkd_color       color = input;
		alignas(16) b3_s32 c[4] {};

		for (b3_loop i = 0; i < 4; i++)
		{
			c[3 - i] = color & 0xff;
			color  = color >> 8;
		}

		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] = (b3_f32)c[i];
		}

		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] /= 255.0;
		}

		return *this;
	}

	/*************************************************************************
	**                                                                      **
	**                        Initializers                                  **
	**                                                                      **
	*************************************************************************/

	/**
	 * This method packs four color component bytes into one unsigned 32
	 * bit integer value.
	 *
	 * @param r The red byte.
	 * @param g The green byte.
	 * @param b The blue byte.
	 * @param a The alpha channel byte.
	 * @return The packed unsigned integer value.
	 */
	static constexpr b3_pkd_color b3MakePkdColor(
		const b3_u08 r,
		const b3_u08 g,
		const b3_u08 b,
		const b3_u08 a = 0)
	{
		b3_pkd_color result = a;

		result <<= 8;
		result  |= r;
		result <<= 8;
		result  |= g;
		result <<= 8;
		result  |= b;

		return result;
	}

	/**
	 * This method initializes all channels of this instance
	 * to zero.
	 */
	inline void b3Init()
	{
		for (b3_loop i = 0; i < 4; i++)
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
	inline void b3InitFactor(const b3_f32 value)
	{
		for (b3_loop i = 0; i < 4; i++)
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
	inline void b3InitFactor(const b3_f64 dvalue)
	{
		b3_f32 value = (b3_f32)dvalue;

		for (b3_loop i = 0; i < 4; i++)
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
	inline void b3Init(const b3_f32 rgb, const b3_f32 a = 0)
	{
		v[R] = v[G] = v[B] = rgb;
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
	constexpr void b3Init(
		b3_f32 r,
		b3_f32 g,
		b3_f32 b,
		b3_f32 a = 0)
	{
		v[A] = a;
		v[R] = r;
		v[G] = g;
		v[B] = b;
	}

	/**
	 * This method sets the alpha channel.
	 *
	 * @param alpha The new alpha channel.
	 */
	inline void b3SetAlpha(const b3_f32 alpha)
	{
		v[A] = alpha;
	}

	/*************************************************************************
	**                                                                      **
	**                        Access operators                              **
	**                                                                      **
	*************************************************************************/

	/**
	 * This index operator returns the spedified color component and returns them.
	 * This is the L value variant.
	 *
	 * @param index The color channel.
	 */
	inline const b3_f32 & operator[](const b3_color_index index) const
	{
		return v[index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 * This is the R value variant.
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 & operator[](const b3_color_index index)
	{
		return v[index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 * This is the L value variant.
	 *
	 * @param index The color channel.
	 */
	inline const b3_f32 & operator[](const int index) const
	{
		return v[(b3_color_index)index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 * This is the R value variant.
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 & operator[](const int index)
	{
		return v[(b3_color_index)index];
	}

	/*************************************************************************
	**                                                                      **
	**                       Arithmetic operators                           **
	**                                                                      **
	*************************************************************************/

	/**
	 * This operator adds a specified color to this instance.
	 *
	 * @param a The color to add.
	 * @return A reference to this instance.
	 */
	inline b3Color & operator+=(const b3Color & a)
	{
		for (b3_loop i = 0; i < 4; i++)
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
	inline b3Color operator+(const b3Color & a) const
	{
		b3Color result;

		for (b3_loop i = 0; i < 4; i++)
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
	inline b3Color & operator-=(const b3Color & a)
	{
		for (b3_loop i = 0; i < 4; i++)
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
	inline b3Color operator-(const b3Color & a) const
	{
		b3Color result;

		for (b3_loop i = 0; i < 4; i++)
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
	inline b3Color & operator*=(const b3Color & a)
	{
		for (b3_loop i = 0; i < 4; i++)
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
	inline b3Color operator*(const b3Color & a) const
	{
		b3Color result;

		for (b3_loop i = 0; i < 4; i++)
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
	template<typename F>
	inline b3Color & operator*=(const F value)
	{
		b3Color prod;

		prod.b3InitFactor(value);
		for (b3_loop i = 0; i < 4; i++)
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
	template<typename F>
	inline b3Color operator*(const F value) const
	{
		b3Color result, multiplicator;

		multiplicator.b3InitFactor(value);
		for (b3_loop i = 0; i < 4; i++)
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
	template<typename F>
	inline b3Color & operator/=(const F value)
	{
		b3Color prod;

		B3_ASSERT(value != 0);
		prod.b3InitFactor((b3_f32)value);
		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	/**
	 * This operator divides the color channels by a given scalar resulting in a new b3Color instance.
	 *
	 * @param value The divisor.
	 * @return The resulting color.
	 */
	template<typename F>
	inline b3Color operator/(const F value) const
	{
		b3Color result, divisor;

		B3_ASSERT(value != 0);
		divisor.b3InitFactor((b3_f32)value);
		for (b3_loop i = 0; i < 4; i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	/*************************************************************************
	**                                                                      **
	**                        Comparison operators                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * This method returns true if any color channel is greater or equal than any color
	 * channel in the given b3Color instance without alpha.
	 *
	 * @param other The b3Color instance to compare to.
	 * @return True if any color channel is greater or equal the other color.
	 */
	inline bool    operator >= (const b3Color & other) const
	{
		bool cmp = true;

		for (b3_loop i = b3Color::R; i <= b3Color::B; i++)
		{
			cmp = cmp && (v[i] >= other.v[i]);
		}
		return cmp;
	}

	/**
	 * This method returns true if any color channel is greater than any color
	 * channel in the given b3Color instance without alpha.
	 *
	 * @param other The b3Color instance to compare to.
	 * @return True if any color channel overrides the other color.
	 */
	inline bool    operator > (const b3Color & other) const
	{
		bool cmp = true;

		for (b3_loop i = b3Color::R; i <= b3Color::B; i++)
		{
			cmp = cmp && (v[i] > other.v[i]);
		}
		return cmp;
	}

	/**
	 * This method returns true if any color channel is lower or equal than any color
	 * channel in the given b3Color instance without alpha.
	 *
	 * @param other The b3Color instance to compare to.
	 * @return True if any color channel is lower or equal the other color.
	 */
	inline bool    operator <= (const b3Color & other) const
	{
		bool cmp = true;

		for (b3_loop i = b3Color::R; i <= b3Color::B; i++)
		{
			cmp = cmp && (v[i] <= other.v[i]);
		}
		return cmp;
	}

	/**
	 * This method returns true if any color channel is lower than any color
	 * channel in the given b3Color instance without alpha.
	 *
	 * @param other The b3Color instance to compare to.
	 * @return True if any color channel overrides the other color.
	 */
	inline bool operator < (const b3Color & other) const
	{
		bool cmp = true;

		for (b3_loop i = b3Color::R; i <= b3Color::B; i++)
		{
			cmp = cmp && (v[i] < other.v[i]);
		}
		return cmp;
	}

	/**
	 * This method returns true if all color channels are equal than any color
	 * channel in the given b3Color instance <em>including</em> alpha.
	 *
	 * @param other The b3Color instance to compare to.
	 * @return True if all color channels are equal to the other color cannels.
	 */
	inline bool operator == (const b3Color & other) const
	{
		bool cmp = true;

		for (b3_loop i = 0; i < 4; i++)
		{
			cmp = cmp && (v[i] == other.v[i]);
		}
		return cmp;
	}

	/**
	 * This method returns true if any color channels is not equal than any color
	 * channel in the given b3Color instance <em>including</em> alpha.
	 *
	 * @param other The b3Color instance to compare to.
	 * @return True if any color channel is not equal to the other color cannels.
	 */
	inline bool operator != (const b3Color & other) const
	{
		for (b3_loop i = 0; i < 4; i++)
		{
			if (v[i] != other.v[i])
			{
				return true;
			}
		}
		return false;
	}

	/*************************************************************************
	**                                                                      **
	**                        Cast operators                                **
	**                                                                      **
	*************************************************************************/

	/**
	 * This cast operator returns the color channels as @c b3_u16 high color.
	 *
	 * @return The color channels as @c b3_u16 high color.
	 */
	constexpr operator b3_u16 () const
	{
		const b3_pkd_color px     = operator b3_pkd_color();
		b3_pkd_color       mask   = 0xf0000000;
		b3_pkd_color       result = 0;

		while (mask != 0)
		{
			const b3_pkd_color nibble = px & mask;

			result  |= nibble;
			result >>= 4;
			mask   >>= 8;
		}
		return b3_u16(result);
	}

	/**
	 * This cast operator returns the color channels as @c ::b3_pkd_color.
	 *
	 * @return The color channels as @c ::b3_pkd_color.
	 */
	constexpr operator b3_pkd_color() const
	{
		b3_pkd_color       result = 0;
		alignas(16) b3_s32 c[4] {};
		alignas(16) b3_f32 sat[4] {};

		for (b3_loop i = 0; i < 4; i++)
		{
			sat[i] = v[i];
			if (sat[i] < 0.0)
			{
				sat[i] = 0.0;
			}
			if (sat[i] > 1.0)
			{
				sat[i] = 1.0;
			}
			sat[i] *= 255.0;
		}

		for (b3_loop i = 0; i < 4; i++)
		{
			c[i] = (b3_s32)round(sat[i]);
		}

		for (b3_loop i = 0; i < 4; i++)
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
	constexpr operator b3_color() const
	{
		return b3_color
		{
			.a = v[A],
			.r = v[R],
			.g = v[G],
			.b = v[B]
		};
	}

	inline operator std::string() const
	{
		char buffer[128];

		snprintf(buffer, sizeof(buffer),
			"b3Color(A=%1.03f RGB=%1.03f/%1.03f/%1.03f)",
			v[A], v[R], v[G], v[B]);

		return std::string(buffer);
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic modifiers                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * This method saturates all color channels to a given value.
	 *
	 * @param sat The saturation value.
	 */
	inline void b3Sat(const b3_f32 sat = 1.0f)
	{
		for (b3_loop i = 0; i < 4; i++)
		{
			if (v[i] > sat)
			{
				v[i] = sat;
			}
		}
	}

	/**
	 * This method bottom clamps all color channels to a given value.
	 *
	 * @param min The minimum value for each color channel.
	 */
	inline void b3Min(const b3_f32 min = 0.0f)
	{
		for (b3_loop i = 0; i < 4; i++)
		{
			if (v[i] < min)
			{
				v[i] = min;
			}
		}
	}

	/**
	 * This computes the absolute value of each color channel including the
	 * alpha channel.
	 */
	inline void b3Abs()
	{
		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] = fabs(v[i]);
		}
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
	inline static b3Color b3Mix(
		const b3Color & low,
		const b3Color & high,
		const b3_f32    mix)
	{
		b3Color result;
		b3Color mixer;

		mixer.b3InitFactor(mix);
		for (b3_loop i = 0; i < 4; i++)
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
	inline static b3Color b3Mix(
		const b3Color & low,
		const b3Color & high,
		const b3_f64    mix)
	{
		b3Color result;
		b3Color mixer;

		mixer.b3InitFactor(mix);
		for (b3_loop i = 0; i < 4; i++)
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
	inline static b3Color b3Mix(
		const b3Color & low,
		const b3Color & high,
		const b3Color & mixer)
	{
		b3Color result;

		for (b3_loop i = 0; i < 4; i++)
		{
			result.v[i] = low.v[i] + mixer.v[i] * (high.v[i]  - low.v[i]);
		}
		return result;
	}

	/**
	 * This method simply dumps the contents of this b3Color instance using
	 * the b3PrintF() method.
	 */
	void b3Dump() const;

	friend std::ostream & operator<< (std::ostream & out, const b3Color & color);

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
