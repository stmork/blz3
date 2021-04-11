/*
**
**	$Filename:	b3Complex.h $
**	$Release:	Dortmund 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Complex datatype
**
**	(C) Copyright  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_BASE_COMPLEX_H
#define B3_BASE_COMPLEX_H

#include "blz3/b3Config.h"

#include <stdexcept>
#include <complex>

/**
 * This template class defines generic operations to do
 * complex math. It provides several operators for simple
 * use. It is recommended to use b3_f64 as template
 * class because it fits best into use of the SSE2 unit of
 * most processors.
 */
template<typename T> class B3_PLUGIN b3Complex
{
	/**
	 * The value array. We advise to use b3_f64 as template class.
	 */
	alignas(16) T v[2];

public:
	enum b3_complex_index
	{
		Re = 0, //!< Index of real part of complex number
		Im = 1  //!< Index of imaginary part of complex number
	};

	/**
	 * Simple constructor which does nothing.
	 */
	inline b3Complex<T>() = default;

	/**
	 * Copy constructor.
	 *
	 * @param complex The complex number we have to copy.
	 */
	inline b3Complex<T>(const b3Complex<T> & complex)
	{
		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] = complex.v[i];
		}
	}

	/**
	 * Initializer constructor which defines the components of
	 * the complex number.
	 *
	 * @param re Real part of complex number.
	 * @param im Imaginary part of the complex number.
	 */
	inline b3Complex<T>(const T re, const T im = 0)
	{
		v[Re] = re;
		v[Im] = im;
	}

	/**
	 * This converting constructor copies the contents of a std::complex
	 * instance into a new b3Complex type.
	 *
	 * @param other The other std::complex instance to copy from.
	 */
	inline b3Complex<T>(const std::complex<T> & other)
	{
		v[Re] = other.real();
		v[Im] = other.imag();
	}

	/**
	 * This operator assigns a std::complex instance to this instance.
	 *
	 * @param other The instance to copy from.
	 */
	inline b3Complex<T> & operator=(const std::complex<T> & other)
	{
		v[Re] = other.real();
		v[Im] = other.imag();

		return *this;
	}

	/**
	 * This operator assigns a real number instance to this instance.
	 *
	 * @param other The instance to copy from.
	 */
	inline b3Complex<T> & operator=(const T value)
	{
		v[Re] = value;
		v[Im] = 0;

		return *this;
	}

	inline bool operator==(const b3Complex<T> & a) const
	{
		return (v[Re] == a.v[Re]) && (v[Im] == a.v[Im]);
	}

	/**
	 * The += operator. The operation is <code>this += a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number to add.
	 * @return This as result.
	 */
	inline b3Complex<T> operator+=(const b3Complex<T> & a)
	{
		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] += a.v[i];
		}

		return *this;
	}

	/**
	 * The -= operator. The operation is <code>this -= a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number to subtract.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator-=(const b3Complex<T> & a)
	{
		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] -= a.v[i];
		}

		return *this;
	}

	/**
	 * The *= operator. The operation is <code>this *= a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number to multiply.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator*=(const b3Complex<T> & a)
	{
		alignas(16) T val[2];

		val[Re] = v[Re] * a.v[Re] - v[Im] * a.v[Im];
		val[Im] = v[Im] * a.v[Re] + v[Re] * a.v[Im];

		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] = val[i];
		}

		return *this;
	}

	/**
	 * The /= operator. The operation is <code>this /= a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number to divide.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator/=(const b3Complex<T> & a)
	{
		alignas(16) T val[2];
		alignas(16) T den[2];
		alignas(16) T nom[2];
		T             denom;

		for (b3_loop i = 0; i < 2; i++)
		{
			nom[i] =   v[i] * a.v[i];
			den[i] = a.v[i] * a.v[i];
		}
		val[Im] = v[Im] * a.v[Re] - v[Re] * a.v[Im];
		denom   = den[Re] + den[Im];
		val[Re] = nom[Re] + nom[Im];

		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] = val[i] / denom;
		}

		return *this;
	}

	/**
	 * The += operator. The operation adds the scalar <code>a</code>
	 * to the real part of this complex number.
	 *
	 * @param a The scalar to add to the real part.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator+=(const T a)
	{
		v[Re] += a;

		return *this;
	}

	/**
	 * The -= operator. The operation subtracts the scalar <code>a</code>
	 * from the real part of this complex number.
	 *
	 * @param a The scalar to subtract from the real part.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator-=(const T a)
	{
		v[Re] -= a;

		return *this;
	}

	/**
	 * The *= operator. The operation scales this complex number
	 * with the scalar <code>a</code>.
	 *
	 * @param a The scalar to scale this complex number.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator*=(const T a)
	{
		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] *= a;
		}

		return *this;
	}

	/**
	 * The /= operator. The operation divides this complex number
	 * by the scalar <code>a</code>.
	 *
	 * @param a The scalar to divide this complex number.
	 * @return This as result.
	 */
	inline b3Complex<T> operator/=(const T a)
	{
		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] /= a;
		}

		return *this;
	}

	/**
	 * The + operator. The operation is <code>result = this + a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number to add.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator+(const b3Complex<T> & a) const
	{
		return b3Complex<T>(*this) += a;
	}

	/**
	 * The - operator. The operation is <code>result = this - a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator-(const b3Complex<T> & a) const
	{
		return b3Complex<T>(*this) -= a;
	}

	/**
	 * The * operator. The operation is <code>result = this * a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number to multiply.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator*(const b3Complex<T> & a) const
	{
		return b3Complex<T>(
				v[Re] * a.v[Re] - v[Im] * a.v[Im],
				v[Im] * a.v[Re] + v[Re] * a.v[Im]);
	}

	/**
	 * The / operator. The operation is <code>result = this / a</code>.
	 * Where <code>a</code> is a complex number.
	 *
	 * @param a The complex number to divide.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator/(const b3Complex<T> & a) const
	{
		return b3Complex<T>(*this) /= a;
	}

	/**
	 * This returns a std::complex instance from this complex representation.
	 */
	inline operator std::complex<T>() const
	{
		return std::complex<T>(v[Re], v[Im]);
	}

	/**
	 * This method computes the value/length of this complex number.
	 *
	 * @return Length of this complex number.
	 */
	inline const T b3Length() const
	{
		return sqrt(b3SquareLength());
	}

	/**
	 * This method normalizes this complex number.
	 */
	inline bool    b3Normalize(const T len = 1)
	{
		T old_len = b3Length();

		if (old_len != 0)
		{
			T new_len = len / old_len;

			for (b3_loop i = 0; i < 2; i++)
			{
				v[i] *= new_len;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * This method computes the length as square of this complex number.
	 *
	 * @return Squared length of this complex number.
	 */
	inline const T b3SquareLength() const
	{
		alignas(16) T val[2];

		for (b3_loop i = 0; i < 2; i++)
		{
			val[i] = v[i] * v[i];
		}
		return val[Re] + val[Im];
	}

	/**
	 * This method returns the phase of both complex parts of
	 * this complex number.
	 *
	 * @return Phase of this complex number.
	 */
	inline const T b3Phase() const
	{
		return atan2(v[Im], v[Re]);
	}

	/**
	 * This method multiplies this complex number with
	 * themself.
	 */
	inline void b3Square()
	{
		alignas(16) T re[2];

		for (b3_loop i = 0; i < 2; i++)
		{
			re[i] = v[i] * v[i];
		}
		v[Im]  = v[Re] * v[Im];
		v[Im] += v[Im];
		v[Re]  = re[0] - re[1];
	}

	inline T & b3Real()
	{
		return v[Re];
	}

	inline T & b3Imag()
	{
		return v[Im];
	}

	inline static b3Complex<T> b3Sqrt(const b3Complex<T> & a)
	{
		b3Complex<T> result;

		if ((a.v[0] < 0) || (a.v[1] < 0))
		{
			throw std::domain_error("negative component for sqrt()");
		}
		for (b3_loop i = 0; i < 2; i++)
		{
			result.v[i] = sqrt(a.v[i]);
		}
		return result;
	}

	inline void b3Scale(const b3Complex<T> & a)
	{
		for (b3_loop i = 0; i < 2; i++)
		{
			v[i] *= a.v[i];
		}
	}

	inline static void b3Swap(b3Complex<T> & a, b3Complex & b)
	{
		b3Complex<T> aux;

		aux = a;
		a = b;
		b = aux;
	}

	/**
	 * This method dumps the contents.
	 *
	 */
	inline void b3Dump(const char * variable, const b3_log_level level = B3LOG_NORMAL) const
	{
		b3PrintF(level, "%s\n", b3ToString(variable).c_str());
	}

	[[nodiscard]]
	inline operator std::string() const
	{
		return b3ToString();
	}

	[[nodiscard]]
	inline std::string b3ToString(const char * variable = nullptr) const
	{
		char buffer[128];

		snprintf(buffer, sizeof(buffer), "%s(re=%2.5f im=%2.5f)",
			variable == nullptr ? "b3Complex" : variable,
			v[Re], v[Im]);

		return std::string(buffer);
	}
};

#endif
