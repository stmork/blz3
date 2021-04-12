/*
**
**	$Filename:	b3ColorSSE.h $
**	$Release:	Dortmund 2005, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Color handling with SSE
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_BASE_COLORSSE_H
#define B3_BASE_COLORSSE_H

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
#ifdef SSE_ALIGNED
	__m128 v;                   //!< These are the color channels of a b3Color instance.
#else
	alignas(16) b3_f32       v[4];
#endif

public:
	/*************************************************************************
	**                                                                      **
	**                        Constructors                                  **
	**                                                                      **
	*************************************************************************/

	/**
	 * This constructor does simply nothing.
	 */
	inline b3Color()
	{
#ifdef _DEBUG
		b3Init(1.0f, 0.125f, 0.25f);
#endif
	}

	/**
	 * This constructor initializes this instance with an SSE vector.
	 *
	 * @param color The color vector in SSE representation.
	 */
	explicit inline b3Color(const __m128 color)
	{
		SSE_PS_STORE(v, color);
	}

	/**
	 * This constructor initializes this instance.
	 *
	 * @param r The new red channel.
	 * @param g The new green channel.
	 * @param b The new blue channel.
	 * @param a The new alpha channel.
	 */
	explicit inline b3Color(
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
	inline b3Color(const b3Color & color)
	{
		v = color.v;
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	inline b3Color(const b3_color & color)
	{
#ifdef SSE_ALIGNED
		v = _mm_load_ps(&color.a);
#else
		v = _mm_loadu_ps(&color.a);
#endif
//		b3Init(color.r, color.g, color.b, color.a);
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	inline b3Color(const b3_color * color)
	{
#ifdef SSE_ALIGNED
		v = _mm_load_ps(&color->a);
#else
		v = _mm_loadu_ps(&color->a);
#endif
//		b3Init(color->r, color->g, color->b, color->a);
	}

	/**
	 * This constructor initializes this instance with equal color values
	 * and a different alpha value.
	 *
	 * @param rgb The equal color components.
	 * @param a The alpha component.
	 */
	inline b3Color(const b3_f32 rgb, const b3_f32 a = 0.0)
	{
		b3Init(rgb, a);
	}

	/**
	 * This constructor initializes this instance from a ::b3_pkd_color structure.
	 *
	 * @param input The other ::b3_pkd_color instance to copy.
	 */
	inline b3Color(const b3_u16 input)
	{
		b3_u16             color = input;
		alignas(16) b3_s32 c[4];

		for (b3_loop i = 0; i < 4; i++)
		{
			c[i]  = color & 0xf;
			color = color >> 4;
		}

		const __m128i sse = _mm_shuffle_epi32(
				_mm_load_si128((const __m128i *)c),
				_MM_SHUFFLE(0, 1, 2, 3));
		v = _mm_div_ps(
				_mm_cvtepi32_ps(sse),
				_mm_set_ps1(15.0));
	}

	/**
	 * This constructor initializes this instance from a ::b3_pkd_color structure.
	 *
	 * @param input The other ::b3_pkd_color instance to copy.
	 */
	inline b3Color(const b3_pkd_color input)
	{
#ifdef BLZ3_USE_SSE2
		__m128i sse;
#ifdef BLZ3_USE_SSSE3
		const __m128i shuffle = _mm_set_epi8(
				0x80, 0x80, 0x80, 0x00,
				0x80, 0x80, 0x80, 0x01,
				0x80, 0x80, 0x80, 0x02,
				0x80, 0x80, 0x80, 0x03);

		// Extract bytes into 32 bit signed with rest zeroed.
		sse = _mm_shuffle_epi8(_mm_set1_epi32(input), shuffle);
#else
		const __m128i zero = _mm_setzero_si128();

		sse = _mm_shuffle_epi32( // swap high <-> low
				_mm_unpacklo_epi8( // extract bytes into 16 bits
					_mm_unpacklo_epi8(
						_mm_set1_epi32(input), zero), zero),
				_MM_SHUFFLE(0, 1, 2, 3));
#endif
		SSE_PS_STORE(v, _mm_div_ps(
				_mm_cvtepi32_ps(sse),
				_mm_set_ps1(255.0)));
#else
		alignas(16) b3_f32 c[4];
		b3_pkd_color       color = input;

		for (b3_loop i = 3; i >= 0; i--)
		{
			c[i]  = b3_f32(color & 0xff);
			color = color >> 8;
		}
		v = _mm_div_ps(
				_mm_load_ps(c),
				_mm_set_ps1(255.0));
#endif
	}

	/*************************************************************************
	**                                                                      **
	**                        Initialization methods                        **
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
	static inline b3_pkd_color b3MakePkdColor(
		const b3_u08 r,
		const b3_u08 g,
		const b3_u08 b,
		const b3_u08 a = 0)
	{
		const __m128i zero  = _mm_setzero_si128();
		const __m128i input = _mm_set_epi32(a, r, g, b);

		// read reversed!
		return _mm_cvtsi128_si32( // select low 32 bits only
				_mm_packus_epi16( // pack 32 bit into 16 bit signed saturated
					_mm_packs_epi32(input, zero), zero)); // pack 16 bit into 8 bit unsigned saturated
	}

	/**
	 * This method initializes all channels of this instance
	 * to zero.
	 */
	inline void b3Init()
	{
		v = _mm_setzero_ps();
	}

	/**
	 * This method initializes all channels of this instance
	 * with the same value. This is for filtering purposes.
	 *
	 * @param value The filter value as single precision floating point number.
	 */
	inline void b3InitFactor(const b3_f32 value)
	{
		v = _mm_set_ps1(value);
	}

	/**
	 * This method initializes all channels of this instance
	 * with the same value. This is for filtering purposes.
	 *
	 * @param dvalue The filter value as double precision floating point number.
	 */
	inline void b3InitFactor(const b3_f64 dvalue)
	{
		v = _mm_set_ps1(float(dvalue));
	}

	/**
	 * This method initializes the instance with a grey color.
	 *
	 * @param rgb The new grey level.
	 * @param a   The new alpha channel.
	 */
	inline void b3Init(const b3_f32 rgb, const b3_f32 a = 0)
	{
		v = _mm_set_ps(rgb, rgb, rgb, a);
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
		const b3_f32 r,
		const b3_f32 g,
		const b3_f32 b,
		const b3_f32 a = 0)
	{
		v = _mm_set_ps(b, g, r, a);
	}

	//////////////////////////////////////--------- methods and operators
	/**
	 * This method sets the alpha channel.
	 *
	 * @param alpha The new alpha channel.
	 */
	inline void b3SetAlpha(const b3_f32 alpha)
	{
		const __m128 a = _mm_set_ss(alpha);

		v = _mm_move_ss(v, a);
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
		b3_f32 * ptr = (b3_f32 *)&v;

		return ptr[index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 * This is the R value variant.
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 & operator[](const b3_color_index index)
	{
		b3_f32 * ptr = (b3_f32 *)&v;

		return ptr[index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 * This is the L value variant.
	 *
	 * @param index The color channel.
	 */
	inline const b3_f32 & operator[](const int index) const
	{
		b3_f32 * ptr = (b3_f32 *)&v;

		return ptr[index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 * This is the R value variant.
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 & operator[](const int index)
	{
		b3_f32 * ptr = (b3_f32 *)&v;

		return ptr[index];
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic operators                          **
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
		v = _mm_add_ps(v, a.v);

		return *this;
	}

	/**
	 * This operator adds a color to this instance resulting in a new instance.
	 *
	 * @param a The color to add.
	 * @return A new b3Color instance.
	 */
	inline const b3Color operator+(const b3Color & a) const
	{
		b3Color result;

		result.v = _mm_add_ps(v, a.v);

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
		v = _mm_sub_ps(v, a.v);

		return *this;
	}

	/**
	 * This operator subtracts a color from this instance resulting in a new instance.
	 *
	 * @param a The color to subtract.
	 * @return A new b3Color instance.
	 */
	inline const b3Color operator-(const b3Color & a) const
	{
		b3Color result;

		result.v = _mm_sub_ps(v, a.v);
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
		v = _mm_mul_ps(v, a.v);
		return *this;
	}

	/**
	 * This operator filters this instance with by a specified color resulting in a new instance.
	 *
	 * @param a The color filter.
	 * @return A new b3Color instance.
	 */
	inline const b3Color operator*(const b3Color & a) const
	{
		b3Color result;

		result.v = _mm_mul_ps(v, a.v);
		return result;
	}

	/**
	 * This operator filters this instance with a specified value.
	 *
	 * @param value The color filter value.
	 * @return A reference to this color.
	 */
	inline b3Color & operator*=(const b3_f32 value)
	{
		v = _mm_mul_ps(v, _mm_set_ps1(value));
		return *this;
	}

	/**
	 * This operator filters this instance with a specified value.
	 *
	 * @param value The color filter value.
	 * @return A reference to this color.
	 */
	inline b3Color & operator*=(const b3_f64 value)
	{
		v = _mm_mul_ps(v, _mm_set_ps1(float(value)));
		return *this;
	}

	/**
	 * This operator filters this instance with a specified value resulting in a new instance.
	 *
	 * @param value The color filter value.
	 * @return The new b3Color instance.
	 */
	inline const b3Color operator*(const b3_f32 value) const
	{
		b3Color result;

		result.v = _mm_mul_ps(v, _mm_set_ps1(value));
		return result;
	}

	/**
	 * This operator filters this instance with a specified value resulting in a new instance.
	 *
	 * @param value The color filter value.
	 * @return The new b3Color instance.
	 */
	inline const b3Color operator*(const b3_f64 value) const
	{
		b3Color result;

		result.v = _mm_mul_ps(v, _mm_set_ps1(float(value)));
		return result;
	}

	/**
	 * This operator divides the color channels by a given scalar.
	 *
	 * @param value The divisor.
	 * @return A reference to this instance.
	 */
	inline b3Color & operator/=(const b3_f32 value)
	{
		B3_ASSERT(value != 0);

		v = _mm_div_ps(v, _mm_set_ps1(value));
		return *this;
	}

	/**
	 * This operator divides the color channels by a given scalar.
	 *
	 * @param value The divisor.
	 * @return A reference to this instance.
	 */
	inline b3Color & operator/=(const b3_f64 value)
	{
		B3_ASSERT(value != 0);

		v = _mm_div_ps(v, _mm_set_ps1(float(value)));
		return *this;
	}

	/**
	 * This operator divides the color channels by a given scalar.
	 *
	 * @param value The divisor.
	 * @return A reference to this instance.
	 */
	inline b3Color & operator/=(const b3_count value)
	{
		B3_ASSERT(value != 0);

		v = _mm_div_ps(v, _mm_set_ps1(float(value)));
		return *this;
	}

	/**
	 * This operator divides the color channels by a given scalar resulting in a new b3Color instance.
	 *
	 * @param value The divisor.
	 * @return The resulting color.
	 */
	inline const b3Color operator/(const b3_f32 value) const
	{
		b3Color result;

		B3_ASSERT(value != 0);

		result.v = _mm_div_ps(v, _mm_set_ps1(value));
		return result;
	}

	/**
	 * This operator divides the color channels by a given scalar resulting in a new b3Color instance.
	 *
	 * @param value The divisor.
	 * @return The resulting color.
	 */
	inline const b3Color operator/(const b3_f64 value) const
	{
		b3Color result;

		B3_ASSERT(value != 0);

		result.v = _mm_div_ps(v, _mm_set_ps1(float(value)));
		return result;
	}

	/**
	 * This operator divides the color channels by a given scalar resulting in a new b3Color instance.
	 *
	 * @param value The divisor.
	 * @return The resulting color.
	 */
	inline const b3Color operator/(const b3_count value) const
	{
		b3Color result;

		B3_ASSERT(value != 0);

		result.v = _mm_div_ps(v, _mm_set_ps1(float(value)));
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
		const int result = _mm_movemask_ps(_mm_cmpge_ps(v, other.v));

		return (result & 14) == 14;
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
		const int result = _mm_movemask_ps(_mm_cmpgt_ps(v, other.v));

		return (result & 14) == 14;
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
		const int result = _mm_movemask_ps(_mm_cmple_ps(v, other.v));

		return (result & 14) == 14;
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
		const int result = _mm_movemask_ps(_mm_cmplt_ps(v, other.v));

		return (result & 14) == 14;
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
		const int result = _mm_movemask_ps(_mm_cmpeq_ps(v, other.v));

		return result == 15;
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
		const int result = _mm_movemask_ps(_mm_cmpeq_ps(v, other.v));

		return result != 15;
	}

	/*************************************************************************
	**                                                                      **
	**                        Cast operators                                **
	**                                                                      **
	*************************************************************************/

	inline operator b3_u16 () const
	{
		const b3_pkd_color px      = operator b3_pkd_color();
		b3_pkd_color mask    = 0xf0000000;
		b3_pkd_color result  = 0;

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
	 * This cast operator returns the color channels as ::b3_pkd_color.
	 *
	 * @return The color channels as ::b3_pkd_color.
	 */
	inline operator b3_pkd_color() const
	{
		const __m128 zero_floats = _mm_set_ps1(0.0);
		const __m128 ones_floats = _mm_set_ps1(1.0);
		const __m128 max_floats  = _mm_set_ps1(COLOR_TOP_BYTE);
		const __m128 sse         = _mm_mul_ps(
				_mm_min_ps(
					ones_floats,
					_mm_max_ps(v, zero_floats)),
				max_floats);

#ifdef BLZ3_USE_SSE2
		// Simply zeroes as constant
		const __m128i zero_ints = _mm_setzero_si128();

		// read reversed!
		__m128i i = _mm_shuffle_epi32( // high <-> low
				_mm_cvtps_epi32(sse), // convert 4 floats into 4 integer
				_MM_SHUFFLE(0, 1, 2, 3));

		// read reversed!
		return _mm_cvtsi128_si32( // select low 32 bits only
				_mm_packus_epi16( // pack 32 bit into 16 bit signed saturated
					_mm_packs_epi32(i, zero_ints), zero_ints)); // pack 16 bit into 8 bit unsigned saturated
#else
		alignas(16) b3_f32 sat[4];
		b3_pkd_color result      = 0;

		_mm_storeu_ps(sat, sse);

		for (b3_loop i = 0; i < 4; i++)
		{
			result = (result << 8) | b3_u32(round(sat[i]));
		}
		return result;
#endif
	}

	/**
	 * This cast operator returns the color channels as ::b3_color.
	 *
	 * @return The color channels as ::b3_color.
	 */
	inline operator b3_color() const
	{
		b3_color result;

#ifdef SSE_ALIGNED
		_mm_store_ps(&result.a, v);
#else
		_mm_storeu_ps(&result.a, v);
#endif

		return result;
	}

	inline operator std::string() const
	{
		alignas(16) b3_f32 a[4];
		char               buffer[128];

		_mm_store_ps(a, v);
		snprintf(buffer, sizeof(buffer),
			"b3Color(A=%1.03f RGB=%1.03f/%1.03f/%1.03f)",
			a[A], a[R], a[G], a[B]);

		return std::string(buffer);
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic modifiers                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * This method saturates all color channels to 1.
	 */
	inline void b3Sat()
	{
		const __m128 sat = _mm_set_ps1(1.0f);

		v = _mm_min_ps(v, sat);
	}

	/**
	 * This method saturates all color channels to a given value.
	 *
	 * @param value The saturation value.
	 */
	inline void b3Sat(const b3_f32 value)
	{
		const __m128 sat = _mm_set_ps1(value);

		v = _mm_min_ps(v, sat);
	}

	/**
	 * This method bottom clamps all color channels to 0.
	 */
	inline void b3Min()
	{
		const __m128 zero = _mm_setzero_ps();

		v = _mm_max_ps(v, zero);
	}

	/**
	 * This method bottom clamps all color channels to a given value.
	 *
	 * @param value The minimum value for each color channel.
	 */
	inline void b3Min(const b3_f32 value)
	{
		const __m128 min = _mm_set_ps1(value);

		v = _mm_max_ps(v, min);
	}

	/**
	 * This computes the absolute value of each color channel including the
	 * alpha channel.
	 */
	inline void b3Abs()
	{
		static const b3_u32 mask = 0x7fffffff;
		const __m128 sign = _mm_load1_ps((const float *)&mask);

		v = _mm_and_ps(sign, v);
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

		result.v = _mm_add_ps(
				low.v,
				_mm_mul_ps(
					_mm_set_ps1(mix),
					_mm_sub_ps(high.v, low.v)));

		return result;
	}

	/**
	 * This method mixes two colors by a mixer component. The mixer must in the
	 * range from [0..1].
	 *
	 * @param low The low component if the mixer is 0.
	 * @param high The high component if the mixer is 1.
	 * @param dMix The mixer component as double precision floating point number.
	 * @return The resulting mixed color.
	 */
	inline static b3Color b3Mix(
		const b3Color & low,
		const b3Color & high,
		const b3_f64    dMix)
	{
		b3Color result;

		result.v = _mm_add_ps(
				low.v,
				_mm_mul_ps(
					_mm_set_ps1(float(dMix)),
					_mm_sub_ps(high.v, low.v)));

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

		result.v = _mm_add_ps(
				low.v,
				_mm_mul_ps(
					mixer.v,
					_mm_sub_ps(high.v, low.v)));

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
	inline const COLORREF b3GetColorref() const
	{
		b3_f32 * ptr = (b3_f32 *)&v;
		return RGB(
				(b3_pkd_color)(ptr[R] * 255),
				(b3_pkd_color)(ptr[G] * 255),
				(b3_pkd_color)(ptr[B] * 255));
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
