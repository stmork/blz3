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

#define COLOR_TOP_NIBBLE   15.0
#define COLOR_TOP_BYTE    255.0

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

	static const b3_u32 m_AbsMask[4];
	static const b3_f32 m_Limit_d015[4];
	static const b3_f32 m_Limit_d255[4];
	static const b3_f32 m_Limit_m255[4];

public:
	/////////////////////////////////////////////////--------  constructors
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
		SSE_PS_STORE(v, SSE_PS_LOAD(color.v));
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	inline b3Color(const b3_color & color)
	{
		b3Init(color.r, color.g, color.b, color.a);
	}

	/**
	 * This constructor initializes this instance from a ::b3_color structure.
	 *
	 * @param color The other ::b3_color instance to copy.
	 */
	inline b3Color(const b3_color * color)
	{
		b3Init(color->r, color->g, color->b, color->a);
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
		alignas(16) b3_f32 d[4];
		b3_loop            i;

		for (i = 3; i >= 0; i--)
		{
			c[i]  = color & 0xf;
			color = color >> 4;
		}

		for (i = 0; i < 4; i++)
		{
			d[i] = (b3_f32)c[i];
		}

#ifdef SSE_ALIGNED
		SSE_PS_STORE(v, _mm_mul_ps(
				_mm_load_ps(d),
				_mm_load_ps(m_Limit_d015)));
#else
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(d),
				SSE_PS_LOAD(m_Limit_d015)));
#endif
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
		static const __m128i shuffle = _mm_set_epi8(
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
					_mm_unpacklo_epi8(_mm_set1_epi32(input), zero), zero), 0x1b);
#endif
		SSE_PS_STORE(v, _mm_mul_ps(
				_mm_cvtepi32_ps(sse),
				_mm_load_ps(m_Limit_d255)));
#else
		alignas(16) b3_f32 c[4];
		b3_pkd_color       color = input;

		for (b3_loop i = 3; i >= 0; i--)
		{
			c[i]  = b3_f32(color & 0xff);
			color = color >> 8;
		}
#ifdef SSE_ALIGNED
		SSE_PS_STORE(v, _mm_mul_ps(
				_mm_load_ps(c),
				_mm_load_ps(m_Limit_d255)));
#else
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(c),
				SSE_PS_LOAD(m_Limit_d255)));
#endif
#endif
	}

	//////////////////////////////////////////--------- initializers
	/**
	 * This method initializes all channels of this instance
	 * to zero.
	 */
	inline void b3Init()
	{
		SSE_PS_STORE(v, _mm_setzero_ps());
	}

	/**
	 * This method initializes all channels of this instance
	 * with the same value. This is for filtering purposes.
	 *
	 * @param value The filter value as single precision floating point number.
	 */
	inline void b3InitFactor(const b3_f32 value)
	{
		SSE_PS_STORE(v, _mm_set_ps1(value));
	}

	/**
	 * This method initializes all channels of this instance
	 * with the same value. This is for filtering purposes.
	 *
	 * @param dvalue The filter value as double precision floating point number.
	 */
	inline void b3InitFactor(const b3_f64 dvalue)
	{
		SSE_PS_STORE(v, _mm_set_ps1(float(dvalue)));
	}

	/**
	 * This method initializes the instance with a grey color.
	 *
	 * @param rgb The new grey level.
	 * @param a   The new alpha channel.
	 */
	inline void b3Init(const b3_f32 rgb, const b3_f32 a = 0)
	{
		SSE_PS_STORE(v, _mm_set_ps(rgb, rgb, rgb, a));
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
		SSE_PS_STORE(v, _mm_set_ps(b, g, r, a));
	}

	//////////////////////////////////////--------- methods and operators
	/**
	 * This method sets the alpha channel.
	 *
	 * @param alpha The new alpha channel.
	 */
	inline void b3SetAlpha(const b3_f32 alpha)
	{
		__m128 result = SSE_PS_LOAD(v);
		__m128 a = _mm_set_ss(alpha);

		SSE_PS_STORE(v, _mm_move_ss(result, a));
	}

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
		__m128  mixer = _mm_set_ps1(mix);
		__m128  l     = SSE_PS_LOAD(low.v);
		b3Color result;

		SSE_PS_STORE(result.v, _mm_add_ps(
				l,
				_mm_mul_ps(
					mixer,
					_mm_sub_ps(SSE_PS_LOAD(high.v), l))));

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
		float mix = dMix;
		__m128  mixer = _mm_set_ps1(mix);
		__m128  l     = SSE_PS_LOAD(low.v);
		b3Color result;

		SSE_PS_STORE(result.v, _mm_add_ps(
				l,
				_mm_mul_ps(
					mixer,
					_mm_sub_ps(SSE_PS_LOAD(high.v), l))));

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
		__m128 l = SSE_PS_LOAD(low.v);

		SSE_PS_STORE(result.v, _mm_add_ps(
				l,
				_mm_mul_ps(
					SSE_PS_LOAD(mixer.v),
					_mm_sub_ps(SSE_PS_LOAD(high.v), l))));

		return result;
	}

	/**
	 * This operator adds a specified color to this instance.
	 *
	 * @param a The color to add.
	 * @return A reference to this instance.
	 */
	inline b3Color & operator+=(const b3Color & a)
	{
		SSE_PS_STORE(v, _mm_add_ps(
				SSE_PS_LOAD(v),
				SSE_PS_LOAD(a.v)));
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

		SSE_PS_STORE(result.v, _mm_add_ps(
				SSE_PS_LOAD(v),
				SSE_PS_LOAD(a.v)));

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
		SSE_PS_STORE(v, _mm_sub_ps(
				SSE_PS_LOAD(v),
				SSE_PS_LOAD(a.v)));
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

		SSE_PS_STORE(result.v, _mm_sub_ps(
				SSE_PS_LOAD(v),
				SSE_PS_LOAD(a.v)));
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
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				SSE_PS_LOAD(a.v)));
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

		SSE_PS_STORE(result.v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				SSE_PS_LOAD(a.v)));
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
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(value)));
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
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(float(value))));
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

		SSE_PS_STORE(result.v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(value)));
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

		SSE_PS_STORE(result.v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(float(value))));
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
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(1.0 / value)));
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
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(float(1.0 / value))));
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
		SSE_PS_STORE(v, _mm_mul_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(1.0 / float(value))));
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
		SSE_PS_STORE(result.v, _mm_div_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(value)));
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
		SSE_PS_STORE(result.v, _mm_div_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(float(value))));
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
		SSE_PS_STORE(result.v, _mm_div_ps(
				SSE_PS_LOAD(v),
				_mm_set_ps1(float(value))));
		return result;
	}

	/**
	 * This method computes the power on all color channels with the given exponent.
	 * This is useful for specular color computation.
	 *
	 * @param exp The exponent.
	 * @return The resulting color.
	 */
	inline const b3Color b3Pow(const b3_f32 exp) const
	{
		alignas(16) b3_f32 b[4];
		b3Color            result;

		_mm_store_ps(b, SSE_PS_LOAD(v));
		SSE_PS_STORE(result.v, _mm_set_ps(
				pow(b[A], exp),
				pow(b[R], exp),
				pow(b[G], exp),
				pow(b[B], exp)));
		return result;
	}

	/**
	 * This method returns true if any color channel is greater than any color
	 * channel in the given b3Color instance.
	 *
	 * @param limit The b3Color instance to compare to.
	 * @return True if any color channel overrides the limit.
	 */
	inline b3_bool b3IsGreater(const b3Color & limit)
	{
		const b3_f32 * a = (const b3_f32 *)&v;
		const b3_f32 * b = (const b3_f32 *)&limit.v;

		return (fabs(a[R]) >= b[R]) || (fabs(a[G]) >= b[G]) || (fabs(a[B]) >= b[B]);
	}

	/**
	 * This method removes all negative signs on any color channel.
	 */
	inline void b3Abs()
	{
		SSE_PS_STORE(v, _mm_and_ps(
				SSE_PS_LOAD(v),
				_mm_loadu_ps((const float *)m_AbsMask)));
	}

	/**
	 * This cast operator returns the color channels as ::b3_pkd_color.
	 *
	 * @return The color channels as ::b3_pkd_color.
	 */
	inline operator b3_pkd_color() const
	{
		__m128       sse;
		b3_pkd_color result = 0;

		sse = _mm_mul_ps(
				_mm_min_ps(
					_mm_set_ps1(1),
					_mm_max_ps(SSE_PS_LOAD(v), _mm_set_ps1(0))),
				_mm_set_ps1(COLOR_TOP_BYTE));
#ifdef BLZ3_USE_SSE2
		// read reversed!
		__m128i i = _mm_shuffle_epi32( // high <-> low
					_mm_cvtps_epi32(sse), 0x1b); // convert 4 floats into 4 integer

		// Simply zeroes as constant
		static const __m128i zero = _mm_setzero_si128();

		// read reversed!
		result = _mm_cvtsi128_si32( // select low 32 bits only
					_mm_packus_epi16( // pack 32 bit into 16 bit signed saturated
						_mm_packs_epi32(i, zero), zero)); // pack 16 bit into 8 bit unsigned saturated
#else
		alignas(16) b3_f32 sat[4];

		_mm_storeu_ps(sat, sse);

		for (b3_loop i = 0; i < 4; i++)
		{
			result = (result << 8) | b3_u32(round(sat[i]));
		}
#endif
		return result;
	}

	/**
	 * This cast operator returns the color channels as ::b3_color.
	 *
	 * @return The color channels as ::b3_color.
	 */
	inline operator b3_color() const
	{
		alignas(16) b3_f32  a[4];
		b3_color            result;

		_mm_store_ps(a, SSE_PS_LOAD(v));
		result.a = a[A];
		result.r = a[R];
		result.g = a[G];
		result.b = a[B];
		return result;
	}

	/**
	 * This method saturates all color channels to 1.
	 */
	inline void b3Sat()
	{
		__m128 s = _mm_set_ps1(1.0f);

		SSE_PS_STORE(v, _mm_min_ps(SSE_PS_LOAD(v), s));
	}

	/**
	 * This method saturates all color channels to a given value.
	 *
	 * @param sat The saturation value.
	 */
	inline void b3Sat(const b3_f32 sat)
	{
		__m128 s = _mm_set_ps1(sat);

		SSE_PS_STORE(v, _mm_min_ps(SSE_PS_LOAD(v), s));
	}

	/**
	 * This method bottom clamps all color channels to 0.
	 */
	inline void b3Min()
	{
		SSE_PS_STORE(v, _mm_max_ps(
				SSE_PS_LOAD(v), _mm_setzero_ps()));
	}

	/**
	 * This method bottom clamps all color channels to a given value.
	 *
	 * @param min The minimum value for each color channel.
	 */
	inline void b3Min(const b3_f32 min)
	{
		__m128 m = _mm_set_ps1(min);

		SSE_PS_STORE(v, _mm_max_ps(SSE_PS_LOAD(v), m));
	}

	inline void b3Dump() const
	{
		alignas(16) b3_f32 a[4];

		_mm_store_ps(a, SSE_PS_LOAD(v));
		b3PrintF(B3LOG_NORMAL, "r=%1.3f g=%1.3f b=%1.3f # a=%1.3f\n",
			a[R], a[G], a[B], a[A]);
	}

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
