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

#ifndef B3_BASE_COLORSSE_H
#define B3_BASE_COLORSSE_H

#include "blz3/b3Config.h"

#include <xmmintrin.h>

/**
 * This class provides color handling. It uses modern command sets
 * like SSE if the compiler can generate this and the underlying cpu
 * architecture supports it.
 */
class B3_PLUGIN b3Color : public b3ColorBase
{
	__m128 v;                   //!< These are the color channels of a b3Color instance.

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
		v = color.v;
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
	printf("u1u1u1u1u1u\n");
#if 0
// FIXME
		b3_u16             color = input;
		b3_s32 B3_ALIGN_16 c[4];
		b3_loop            i;

		for (i = 0;i < 4;i++)
		{
			c[3-i] = color & 0xf;
			color  = color >> 4;
		}

		for (i = 0;i < 4;i++)
		{
			v[i] = (b3_f32)c[i];
		}

		for (i = 0;i < 4;i++)
		{
			v[i] *= m_Limit_d015[i];
		}
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
		b3_f32 B3_ALIGN_16 c[4];
		b3_loop            i;

		for (i = 0;i < 4;i++)
		{
			c[i] = float(color & 0xff);
			color  = color >> 8;
		}

		v = _mm_div_ps(_mm_loadr_ps(c),_mm_set_ps1(255));
	}

	//////////////////////////////////////////--------- initializers
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
	inline void b3InitFactor(b3_f32 value)
	{
		v = _mm_set_ps1(value);
	}

	/**
	 * This method initializes all channels of this instance
	 * with the same value. This is for filtering purposes.
	 *
	 * @param dvalue The filter value as double precision floating point number.
	 */
	inline void b3InitFactor(b3_f64 dvalue)
	{
		v = _mm_set_ps1(float(dvalue));
	}

	/**
	 * This method initializes the instance with a grey color.
	 *
	 * @param rgb The new grey level.
	 * @param a   The new alpha channel.
	 */
	inline void b3Init(b3_f32 rgb, b3_f32 a = 0)
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
		b3_f32 r,
		b3_f32 g,
		b3_f32 b,
		b3_f32 a = 0)
	{
		v = _mm_set_ps(b, g, r, a);
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
		__m128 a = _mm_set_ss(alpha);
		v =_mm_move_ss(v, a);
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
	inline static b3Color b3Mix(const b3Color &low,const b3Color &high,b3_f32 mix)
	{
		__m128  mixer = _mm_set_ps1(mix);
		b3Color result;

		result.v = _mm_add_ps(low.v,_mm_mul_ps(mixer,_mm_sub_ps(high.v,low.v)));

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
		__m128  mixer = _mm_set_ps1(float(mix));
		b3Color result;

		result.v = _mm_add_ps(low.v,_mm_mul_ps(mixer,_mm_sub_ps(high.v,low.v)));

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

		result.v = _mm_add_ps(low.v,_mm_mul_ps(mixer.v,_mm_sub_ps(high.v,low.v)));

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
		v = _mm_add_ps(v,a.v);
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

		result.v = _mm_add_ps(v,a.v);

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
		v = _mm_sub_ps(v, a.v);
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

		result.v = _mm_sub_ps(v, a.v);
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
		v = _mm_mul_ps(v, a.v);
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

		result.v = _mm_mul_ps(v, a.v);
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
		v = _mm_mul_ps(v, _mm_set_ps1(value));
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
		v = _mm_mul_ps(v, _mm_set_ps1(float(value)));
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

		result.v = _mm_mul_ps(v, _mm_set_ps1(value));
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

		result.v = _mm_mul_ps(v, _mm_set_ps1(float(value)));
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
		v = _mm_div_ps(v, _mm_set_ps1(value));
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
		v = _mm_div_ps(v, _mm_set_ps1(float(value)));
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
		v = _mm_div_ps(v, _mm_set_ps1(float(value)));
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
		result.v = _mm_div_ps(v, _mm_set_ps1(value));
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
		result.v = _mm_div_ps(v, _mm_set_ps1(float(value)));
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
		result.v = _mm_div_ps(v, _mm_set_ps1(float(value)));
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
		b3_f32 B3_ALIGN_16 b[4];
		b3Color            result;

		_mm_store_ps(b, v);
		result.v = _mm_set_ps(
			pow(b[A], exp), 
			pow(b[R], exp), 
			pow(b[G], exp), 
			pow(b[B], exp));
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
#if 0
	// FIXME
		return (fabs(v[R]) >= limit[R]) || (fabs(v[G]) >= limit[G]) || (fabs(v[B]) >= limit[B]);
#else
	return false;
#endif
	}

	/**
	 * This method removes all negative signs on any color channel.
	 */
	inline void b3Abs()
	{
		// FIXME
		__m128 mask = _mm_load_ps1(0);
		v = _mm_and_ps(v, mask);
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

		_mm_store_ps(
			sat,
			_mm_mul_ps(
				_mm_min_ps(
					_mm_set_ps1(1),
					_mm_max_ps(v, _mm_set_ps1(0))),
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
		b3_f32 B3_ALIGN_32  a[4];
		b3_color            result;

		_mm_store_ps(a, v);
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

		v = _mm_min_ps(v,s);
	}

	/**
	 * This method saturates all color channels to a given value.
	 *
	 * @param sat The saturation value.
	 */
	inline void b3Sat(b3_f32 sat)
	{
		__m128 s = _mm_set_ps1(sat);

		v = _mm_min_ps(v,s);
	}

	/**
	 * This method bottom clamps all color channels to 0.
	 */
	inline void b3Min()
	{
		__m128 zero = _mm_setzero_ps();

		v = _mm_max_ps(v,zero);
	}

	/**
	 * This method bottom clamps all color channels to a given value.
	 *
	 * @param min The minimum value for each color channel.
	 */
	inline void b3Min(b3_f32 min)
	{
		__m128 m = _mm_set_ps1(min);

		v = _mm_max_ps(v,m);
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
