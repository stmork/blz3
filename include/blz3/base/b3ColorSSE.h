/*
**
**	$Filename:	b3ColorSSE.h $
**	$Release:	Dortmund 2005 $
**	$Revision: 1478 $
**	$Date: 2007-01-01 19:16:28 +0100 (Mo, 01 Jan 2007) $
**	$Author: sm $
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

#ifndef B3_BASE_COLORSSE_H
#define B3_BASE_COLORSSE_H

#include "blz3/b3Config.h"

/**
 * This class provides color handling. It uses modern command sets
 * like SSE if the compiler can generate this and the underlying cpu
 * architecture supports it.
 */
class B3_PLUGIN b3Color : public b3ColorBase
{
#ifdef SSE_ALIGNED
	             __m128 v;                   //!< These are the color channels of a b3Color instance.
#else
	b3_f32 B3_ALIGN_16  v[4];
#endif

	static const b3_u32 B3_ALIGN_16 m_AbsMask[4];
	static const b3_f32 B3_ALIGN_16 m_Limit_d015[4];
	static const b3_f32 B3_ALIGN_16 m_Limit_d255[4];

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
	inline b3Color(const __m128 color)
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
	inline b3Color(
		const b3_f32 r,
		const b3_f32 g,
		const b3_f32 b,
		const b3_f32 a = 0)
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
		SSE_PS_STORE(v, SSE_PS_LOAD(color.v));
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
	inline b3Color(const b3_u16 input)
	{
		b3_u16             color = input;
		b3_s32 B3_ALIGN_16 c[4];
		b3_f32 B3_ALIGN_16 d[4];
		b3_loop            i;

		for (i = 3; i >= 0; i--)
		{
			c[i]  = color & 0xf;
			color = color >> 4;
		}

		for (i = 0;i < 4;i++)
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
		b3_pkd_color       color = input;
		b3_loop            i;

#ifdef BLZ3_USE_SSE2
		b3_s32 B3_ALIGN_16 c[4];
		__m128i            ci;

		for (i = 3;i >= 0;i--)
		{
			c[i]  = color & 0xff;
			color = color >> 8;
		}

		ci = _mm_loadu_si128((__m128i *)c);
		SSE_PS_STORE(v,_mm_mul_ps(
			_mm_cvtepi32_ps(ci),
			_mm_loadu_ps(m_Limit_d255)));
#else
		b3_f32 B3_ALIGN_16 c[4];

		for (i = 3;i >= 0; i--)
		{
			c[i]  = b3_f32(color & 0xff);
			color = color >> 8;
		}
#ifdef SSE_ALIGNED
		SSE_PS_STORE(v,_mm_mul_ps(
			_mm_load_ps(c),
			_mm_load_ps(m_Limit_d255)));
#else
		SSE_PS_STORE(v,_mm_mul_ps(
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
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 &operator[](const b3_color_index index)
	{
		b3_f32 *ptr = (b3_f32 *)&v;

		return ptr[index];
	}

	/**
	 * This index operator returns the spedified color component and returns them.
	 *
	 * @param index The color channel.
	 */
	inline b3_f32 &operator[](const int index)
	{
		b3_f32 *ptr = (b3_f32 *)&v;

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
		const b3Color &low,
		const b3Color &high,
		const b3_f32   mix)
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
	 * @param mix The mixer component as double precision floating point number.
	 * @return The resulting mixed color.
	 */
	inline static b3Color b3Mix(
		const b3Color &low,
		const b3Color &high,
		const b3_f64   dMix)
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
		const b3Color &low,
		const b3Color &high,
		const b3Color &mixer)
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
	inline b3Color &operator+=(const b3Color &a)
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
	inline b3Color operator+(const b3Color &a)
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
	inline b3Color &operator-=(const b3Color &a)
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
	inline b3Color operator-(const b3Color &a)
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
	inline b3Color &operator*=(const b3Color &a)
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
	inline b3Color operator*(const b3Color &a)
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
	inline b3Color &operator*=(const b3_f32 value)
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
	inline b3Color &operator*=(const b3_f64 value)
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
	inline b3Color operator*(const b3_f32 value)
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
	inline b3Color operator*(const b3_f64 value)
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
	inline b3Color &operator/=(const b3_f32 value)
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
	inline b3Color &operator/=(const b3_f64 value)
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
	inline b3Color &operator/=(const b3_count value)
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
	inline b3Color operator/(const b3_f32 value)
	{
		b3Color result;

		B3_ASSERT(value != 0);
		SSE_PS_STORE(result.v, _mm_mul_ps(
			SSE_PS_LOAD(v),
			_mm_set_ps1(1.0 / value)));
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
		b3Color result;

		B3_ASSERT(value != 0);
		SSE_PS_STORE(result.v, _mm_mul_ps(
			SSE_PS_LOAD(v),
			_mm_set_ps1(1.0 / float(value))));
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
		b3Color result;

		B3_ASSERT(value != 0);
		SSE_PS_STORE(result.v, _mm_mul_ps(
			SSE_PS_LOAD(v),
			_mm_set_ps1(1.0 / float(value))));
		return result;
	}

	/**
	 * This method computes the power on all color channels with the given exponent.
	 * This is useful for specular color computation.
	 *
	 * @param exp The exponent.
	 * @return The resulting color.
	 */
	inline b3Color b3Pow(const b3_f32 exp)
	{
		b3_f32 B3_ALIGN_16 b[4];
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
	inline b3_bool b3IsGreater(const b3Color &limit)
	{
		const b3_f32 *a = (const b3_f32 *)&v;
		const b3_f32 *b = (const b3_f32 *)&limit.v;

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
	inline operator b3_pkd_color()
	{
		b3_s32 B3_ALIGN_16 c[4];
		b3_f32 B3_ALIGN_16 sat[4];
		b3_pkd_color       result = 0;
		b3_loop            i;

		_mm_storeu_ps(
			sat,
			_mm_mul_ps(
				_mm_min_ps(
					_mm_set_ps1(1),
					_mm_max_ps(SSE_PS_LOAD(v), _mm_set_ps1(0))),
				_mm_set_ps1(255)));

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
		b3_f32 B3_ALIGN_16  a[4];
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

	inline void b3Dump()
	{
		b3_f32 B3_ALIGN_16  a[4];

		_mm_store_ps(a, SSE_PS_LOAD(v));
		b3PrintF(B3LOG_NORMAL,"r=%1.3f g=%1.3f b=%1.3f # a=%1.3f\n",
			a[R], a[R], a[B], a[A]);
	}

#ifdef WIN32
	/**
	 * This method returns the color values as ::COLORREF,
	 *
	 * @return The colors as ::COLORREF.
	 */
	inline COLORREF b3GetColorref()
	{
		b3_f32 *ptr = (b3_f32 *)&v;
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
