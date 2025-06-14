/*
**
**	$Filename:	b3Math.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some math convenience methods...
**
**	(C) Copyright 2004 - 2024  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_MATH_H
#define B3_BASE_MATH_H

#include "blz3/b3Config.h"

#include <climits>

/**
 * This class provides useful math helper methods.
 */
class B3_PLUGIN b3Math
{
public:
	/**
	 * The near zero value epsilon.
	 *
	 * @see: https://en.wikipedia.org/wiki/Machine_epsilon
	 */
	static constexpr b3_f32  epsilon = 1.0 / 16384.0;

	/**
	 * This method returns the number which is a power of 2 number
	 * and greater or equal to the given number.
	 *
	 * @param value The value to compute the power of 2 number from.
	 * @return The value which is a power of two number and greater or equal to
	 * the given number.
	 */
	static b3_loop  b3PowOf2(const b3_loop value);

	/**
	 * This method returns the log of the given value.
	 *
	 * @param value The value to compute the log2 from.
	 * @return The resulting log2 value.
	 */
	static b3_count b3Log2(b3_u32 value);

	/**
	 * This method returns the log of the given value.
	 *
	 * @param value The value to compute the log3 from.
	 * @return The resulting log3 value.
	 */
	static b3_count b3Log3(b3_u32 value);

	/**
	 * This method squares a value.
	 *
	 * \param value The value to square.
	 * \return The squared value.
	 */
	template<typename T>
	static constexpr T b3Sqr(const T value)
	{
		return value * value;
	}

	/**
	 * This method checks a real number if it is near zero.
	 *
	 * @param x The real number to check against zero.
	 * @return True if the real number can be assumed as zero.
	 */
	template<typename T>
	static constexpr bool b3IsZero(const T x)
	{
		return std::fabs(x) < std::numeric_limits<b3_f32>::epsilon();
	}

	/**
	 * This computes a Fresnel term from an input value.
	 *
	 * \param F0
	 * \return The computed Fresnel term.
	 */
	template<typename T>
	static constexpr T b3GetMu(const T F0)
	{
		const b3_f64 sF0 = sqrt(F0);

		return (1.0 + sF0) / (1.0 - sF0);
	}

	/**
	 * This method computes a Fresnel term from two values. This is
	 * known as Schlick's hack.
	 *
	 * \param phi
	 * \param mu
	 * \return The fresnel computed value.
	 */
	template<typename T>
	static constexpr T b3GetFresnel(b3_f64 phi, const T mu)
	{
		const b3_f64 theta = asin(sin(phi) / mu);
		const b3_f64 a     = b3Sqr(sin(phi - theta));
		const b3_f64 b     = b3Sqr(sin(phi + theta));
		const b3_f64 c     = b3Sqr(tan(phi - theta));
		const b3_f64 d     = b3Sqr(tan(phi + theta));

		return 0.5 * (a / b + c / d);
	}


	/**
	 * This method limits a value against the specified bounds. This is the
	 * floating point version.
	 *
	 * \param input The value to limit.
	 * \param min The lower limit.
	 * \param max The upper limit.
	 * \return The bound value.
	 */
	template<typename T>
	[[deprecated("Use std::clamp() instead.")]]
	static constexpr T b3Clamp(
		const T input,
		const T min,
		const T max)
	{
		return std::clamp(input, min, max);
	}

	/**
	 * This method rounds a number by the given precision.
	 *
	 * \param x The value to round.
	 * \param rnd The precision.
	 * \return The rounded value.
	 */
	template<typename T>
	static constexpr T b3Round(const T x, const T rnd)
	{
		const T mul    = std::round(0.5 / rnd);
		const T result = std::round(x * mul) / mul;

		return result;
	}

	/**
	 * This method returns the fractional part of the division <code>a/b</code>.
	 * The result is always positive or zero even the dividend is negative.
	 *
	 * \param a The dividend.
	 * \param b The divisor.
	 * \return The fractional part.
	 */
	inline static constexpr b3_f64 b3Frac(b3_f64 a, b3_f64 b)
	{
		b3_f64 remainder = fmod(a, b);

		return remainder < 0 ? remainder + b : remainder;
	}

	/**
	 * This returns the fractional part of a floating point number.
	 *
	 * \param a The complete floating point number.
	 * \return The fractional part of a float.
	 */
	inline static constexpr b3_f64 b3FracOne(const b3_f64 a)
	{
		return a - floor(a);
	}

	/**
	 * This method mixes two values with a given mixer value. The mixer
	 * must be in the range [0..1]. A mixer value of 0 results in a, a
	 * value of 1 results in b.
	 *
	 * \param a The first value to mix.
	 * \param b The second value to mix.
	 * \param x The mixer in range [0..1].
	 * \return The mixed value.
	 */
	template<typename T>
	inline static constexpr T b3Mix(const T a, const T b, const	T x)
	{
		B3_ASSERT(x >= 0);
		B3_ASSERT(x <= 1);

		return a + x * (b - a);
	}

	/**
	 * This method returns the lower value of a or b.
	 *
	 * \param a One value.
	 * \param b Another value.
	 * \return The lower value of both given values.
	 */
	template<typename T>
	[[deprecated("Use std::min() instead.")]]
	inline static constexpr T b3Min(const T a, const T b)
	{
		return std::min(a, b);
	}

	/**
	 * This method returns the greater value of a or b.
	 *
	 * \param a One value.
	 * \param b Another value.
	 * \return The greater value of both given values.
	 */
	template<typename T>
	[[deprecated("Use std::max() instead.")]]
	inline static constexpr T b3Max(const T a, const T b)
	{
		return std::max(a, b);
	}

	inline static constexpr b3_f64 b3Hermite(
		const b3_f64 t1,
		const b3_f64 s0,
		const b3_f64 s1)
	{
		b3_f64 t2 = t1 * t1;
		b3_f64 t3 = t2 * t1;

		b3_f64 P4 = -2 * t3 + 3 * t2;
		b3_f64 R1 =      t3 - 2 * t2 + t1;
		b3_f64 R4 =      t3 -     t2;

		return P4 + s0 * R1 + s1 * R4;
	}

	/**
	 * This method is a smooth hermite ramp inside the range [s..e]. Values
	 * outside this range are clamped accordingly.
	 *
	 * \param s The start value of the ramp.
	 * \param e The end value of the ramp.
	 * \param x The value to interpolate.
	 * \return The hermite interpolation.
	 */
	template<typename T>
	inline static constexpr T b3Smoothstep(
		const T s,
		const T e,
		const T x)
	{
		const T t = (std::clamp(x, s, e) - s) / (e - s);

		return (3 - 2 * t) * t * t;
	}


	/**
	 * This method is a smooth hermite ramp inside the range [0..1]. Values
	 * outside this range are clamped accordingly.
	 *
	 * \param t The input value.
	 * \return The hermite interpolation.
	 */
	template<typename T>
	inline static constexpr T b3Smoothstep(const T t)
	{
		const T c = std::clamp(t, 0.0, 1.0);

		return (3 - 2 * c) * c * c;
	}

	inline static constexpr b3_f64 b3SmoothPulse(
		const b3_f64 e0,
		const b3_f64 e1,
		const b3_f64 e2,
		const b3_f64 e3,
		const b3_f64 x)
	{
		return b3Smoothstep(e0, e1, x) - b3Smoothstep(e2, e3, x);
	}

	inline static constexpr b3_f64 b3SmoothPulseTrain(
		const b3_f64 e0,
		const b3_f64 e1,
		const b3_f64 e2,
		const b3_f64 e3,
		const b3_f64 x,
		const b3_f64 period)
	{
		return b3SmoothPulse(e0, e1, e2, e3, b3Math::b3Frac(x, period));
	}

	/**
	 * This method computes the Hermite polynoms from a value.
	 * @param t
	 * @return
	 */
	template<typename T>
	inline static constexpr T b3Fade(const T t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	/**
	 * This method aligns a value to an upper border of a given power of two.
	 *
	 * @example b3AlignUpper(0,2) = 0
	 * @example b3AlignUpper(1,2) = 4
	 *
	 * @param value The value to align up.
	 * @param bits The alignment as power of two.
	 * @return The upper aligned value.
	 */
	template<typename T>
	inline static constexpr T b3AlignUpper(const T value, const T bits)
	{
		const T mask  = (1 << bits) - 1;
		const T upper = value + mask;

		return upper & (~mask);
	}

	/**
	 * This funtion is a fast pow() version for high integer exponents.
	 *
	 * \param x The base.
	 * \param exponent The exponent.
	 * \return The power of x with integer exponent exponent.
	 */
	inline static constexpr b3_f64 b3FastPow(const b3_f64 x, b3_u32 exponent)
	{
		b3_f64 result = 1;
		b3_f64 factor = x;

		while (exponent != 0)
		{
			if (exponent & 1)
			{
				result *= factor;
			}
			exponent = exponent >> 1;
			factor *= factor;
		}
		return result;
	}

	/**
	 * This method is the clamped version of the standard asin function.
	 *
	 * \param x The sine value.
	 * \return The arc sine value.
	 */
	inline static constexpr b3_f64 b3Asin(const b3_f64 x)
	{
		if (x <= -1)
		{
			[[unlikely]]
			return -M_PI * 0.5;
		}
		else if (x >= 1)
		{
			[[unlikely]]
			return M_PI * 0.5;
		}
		return asin(x);
	}

	/**
	 * This method is the clamped version of the standard acos function.
	 *
	 * \param x The cosine value.
	 * \return The arc cosine value.
	 */
	inline static constexpr b3_f64 b3Acos(const b3_f64 x)
	{
		if (x <= -1)
		{
			[[unlikely]]
			return M_PI;
		}
		else if (x >= 1)
		{
			[[unlikely]]
			return 0;
		}
		return acos(x);
	}

	inline static constexpr b3_f64 b3ArcAngleOfScalars(const b3_f64 u, const b3_f64 v)
	{
		return atan2(v, u) + (v < 0 ? (M_PI * 2.0) : 0);
	}

	inline static constexpr b3_f64 b3RelAngleOfScalars(const b3_f64 u, const b3_f64 v)
	{
		return atan2(v, u) * 0.5 / M_PI + (v < 0 ?   1.0 : 0);
	}

	inline static constexpr b3_f64 b3AngleOfScalars(const b3_f64 u, const b3_f64 v)
	{
		return atan2(v, u) * 180.0 / M_PI + (v < 0 ? 360.0 : 0);
	}

	/**
	 * This method converts an angle in degrees into an angle of radians.
	 *
	 * @param a The angle in degree.
	 * @return The angle in radians.
	 */
	template<typename F>
	inline static constexpr F b3Rad(const F a)
	{
		return a * M_PI / 180.0;
	}

	/**
	 * This method converts an angle in radians into an angle of degrees.
	 *
	 * @param a The angle in radians.
	 * @return The angle in degrees.
	 */
	template<typename F>
	inline static constexpr F b3Degree(const F a)
	{
		return a * 180.0 / M_PI;
	}

	/**
	 * This method computes the cubic root from a value.
	 *
	 * \param x The value to compute the cubic root from.
	 * \return The cubic root.
	 */
	[[deprecated("Use std::cbrt() instead!")]]
	inline static constexpr b3_f64 b3Cbrt(const b3_f64 x)
	{
		return cbrt(x);
	}
};

#endif
