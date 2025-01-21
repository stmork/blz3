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
	static_assert(std::is_floating_point<T>(), "Complex numbers should be floating point!");

protected:
	enum b3_complex_index
	{
		Re = 0, //!< Index of real part of complex number
		Im,     //!< Index of imaginary part of complex number
		Max
	};

	/**
	 * The value array. We advise to use b3_f64 as template class.
	 */
	alignas(16) T v[Max] {};

public:

	/** The used template parameter type. */
	using type = T;

	/*************************************************************************
	**                                                                      **
	**                        Constructors                                  **
	**                                                                      **
	*************************************************************************/

	/**
	 * Simple constructor which does nothing.
	 */
	constexpr b3Complex<T>() = default;

	/**
	 * Copy constructor.
	 *
	 * @param other The complex number we have to copy.
	 */
	constexpr b3Complex<T>(const b3Complex<T> & other)
	{
		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] = other.v[i];
		}
	}

	/**
	 * This converting constructor copies the contents of a @c std::complex
	 * instance into a new b3Complex type.
	 *
	 * @param other The other std::complex instance to copy from.
	 */
	template<typename F>
	constexpr b3Complex<T>(const std::complex<F> & other)
	{
		v[Re] = other.real();
		v[Im] = other.imag();
	}

	/**
	 * Initializer constructor which defines the components of
	 * the complex number.
	 *
	 * @param re Real part of complex number.
	 * @param im Imaginary part of the complex number.
	 */
	constexpr b3Complex<T>(const T re, const T im = 0)
	{
		v[Re] = re;
		v[Im] = im;
	}

	/*************************************************************************
	**                                                                      **
	**                        Assignment operators                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * This operator assigns a real number instance to this instance.
	 *
	 * @param value The real component to initialize.
	 */
	template<typename F>
	constexpr b3Complex<T> & operator=(const F value)
	{
		v[Re] = value;
		v[Im] = 0;

		return *this;
	}

	/**
	 * This operator assigns a @c std::complex instance to this instance.
	 *
	 * @param other The instance to copy from.
	 */
	template<typename F>
	constexpr b3Complex<T> & operator=(const std::complex<F> & other)
	{
		v[Re] = other.real();
		v[Im] = other.imag();

		return *this;
	}

	/**
	 * This operator assigns a b3Complex instance to this instance.
	 *
	 * @param other The instance to copy from.
	 */
	constexpr b3Complex<T> & operator=(const b3Complex<T> & other)
	{
		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] = other.v[i];
		}

		return *this;
	}

	/**
	 * This operator assigns a b3Complex instance of another template type to
	 * this instance.
	 *
	 * @param other The instance to copy from.
	 */
	template<class F>
	constexpr b3Complex<T> & operator=(b3Complex<F> & other)
	{
		v[Re] = other.b3Real();
		v[Im] = other.b3Imag();

		return *this;
	}

	/*************************************************************************
	**                                                                      **
	**                        Cast operators                                **
	**                                                                      **
	*************************************************************************/

	/**
	 * This returns a @c std::complex instance from this complex representation.
	 */
	constexpr operator std::complex<T>() const
	{
		return std::complex<T>(v[Re], v[Im]);
	}

	/*************************************************************************
	**                                                                      **
	**                        Comparison operators                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * This method compares another b3Complex instance with this instance.
	 *
	 * @param other The other instance to compare to.
	 * @return True if both instances are equal.
	 */
	inline bool operator==(const b3Complex<T> & other) const
	{
		return (v[Re] == other.v[Re]) && (v[Im] == other.v[Im]);
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic operators                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * The += operator. The operation is <code>this += other</code>.
	 * Where @c other is a complex number.
	 *
	 * @param other The complex number to add.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator+=(const b3Complex<T> & other)
	{
		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] += other.v[i];
		}

		return *this;
	}

	/**
	 * The -= operator. The operation is <code>this -= other</code>.
	 * Where @c other is a complex number.
	 *
	 * @param other The complex number to subtract.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator-=(const b3Complex<T> & other)
	{
		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] -= other.v[i];
		}

		return *this;
	}

	/**
	 * The *= operator. The operation is <code>this *= other</code>.
	 * Where @c other is a complex number.
	 *
	 * @param a The complex number to multiply.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator*=(const b3Complex<T> & other)
	{
		alignas(16) T val[Max]
		{
			v[Re] * other.v[Re] - v[Im] * other.v[Im],
			v[Im] * other.v[Re] + v[Re] * other.v[Im]
		};

		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] = val[i];
		}

		return *this;
	}

	/**
	 * The /= operator. The operation is <code>this /= other</code>.
	 * Where @c other is a complex number.
	 *
	 * @param a The complex number to divide.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator/=(const b3Complex<T> & other)
	{
		alignas(16) T val[Max];
		alignas(16) T den[Max];
		alignas(16) T nom[Max];
		T             denom;

		for (b3_loop i = 0; i < Max; i++)
		{
			nom[i] =       v[i] * other.v[i];
			den[i] = other.v[i] * other.v[i];
		}
		val[Im] = v[Im] * other.v[Re] - v[Re] * other.v[Im];
		denom   = den[Re] + den[Im];
		val[Re] = nom[Re] + nom[Im];

		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] = val[i] / denom;
		}

		return *this;
	}

	/**
	 * The += operator. The operation adds the scalar @c value
	 * to the real part of this complex number.
	 *
	 * @param value The scalar to add to the real part.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator+=(const T value)
	{
		v[Re] += value;

		return *this;
	}

	/**
	 * The -= operator. The operation subtracts the scalar @c value
	 * from the real part of this complex number.
	 *
	 * @param value The scalar to subtract from the real part.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator-=(const T value)
	{
		v[Re] -= value;

		return *this;
	}

	/**
	 * The *= operator. The operation scales this complex number
	 * with the scalar @c value.
	 *
	 * @param value The scalar to scale this complex number.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator*=(const T value)
	{
		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] *= value;
		}

		return *this;
	}

	/**
	 * The /= operator. The operation divides this complex number
	 * by the scalar @c value.
	 *
	 * @param value The scalar to divide this complex number.
	 * @return This as result.
	 */
	inline b3Complex<T> & operator/=(const T value)
	{
		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] /= value;
		}

		return *this;
	}

	/**
	 * The + operator. The operation is <code>result = this + other</code>.
	 * Where @c other is a b3Complex number.
	 *
	 * @param other The complex number to add.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator+(const b3Complex<T> & other) const
	{
		return b3Complex<T>(*this) += other;
	}

	/**
	 * The + operator. The operation is <code>result = this + other</code>.
	 * Where @c other is a @c std::complex number.
	 *
	 * @param other The std::complex number to add.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator+(const std::complex<T> & other) const
	{
		return b3Complex<T>(*this) += b3Complex<T>(other);
	}

	/**
	 * The - operator. The operation is <code>result = this - other</code>.
	 * Where @c other is a complex number.
	 *
	 * @param other The complex number.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator-(const b3Complex<T> & other) const
	{
		return b3Complex<T>(*this) -= other;
	}

	/**
	 * The - operator. The operation is <code>result = this - other</code>.
	 * Where <code>other</code> is a std::complex number.
	 *
	 * @param other The @c std::complex number to add.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator-(const std::complex<T> & other) const
	{
		return b3Complex<T>(*this) -= b3Complex<T>(other);
	}

	/**
	 * The * operator. The operation is <code>result = this * other</code>.
	 * Where @c other is a complex number.
	 *
	 * @param other The complex number to multiply.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator*(const b3Complex<T> & other) const
	{
		return b3Complex<T>(
				v[Re] * other.v[Re] - v[Im] * other.v[Im],
				v[Im] * other.v[Re] + v[Re] * other.v[Im]);
	}

	/**
	 * The / operator. The operation is <code>result = this / other</code>.
	 * Where @c other is a complex number.
	 *
	 * @param other The complex number to divide.
	 * @return This as result.
	 */
	inline const b3Complex<T> operator/(const b3Complex<T> & other) const
	{
		return b3Complex<T>(*this) /= other;
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic modifiers                          **
	**                                                                      **
	*************************************************************************/

	/**
	 * This method squares its components.
	 *
	 * @return This as squared result.
	 */
	constexpr b3Complex<T> & b3Square()
	{
		alignas(16) T val[Max]
		{
			v[Re] * v[Re] - v[Im] * v[Im],
			v[Im] * v[Re] + v[Re] * v[Im]
		};

		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] = val[i];
		}

		return *this;
	}

	/**
	 * This method computes the length as square of this complex number.
	 *
	 * @return Squared length of this complex number.
	 */
	constexpr T b3SquareLength() const
	{
		alignas(16) T val[Max];

		for (b3_loop i = 0; i < Max; i++)
		{
			val[i] = v[i] * v[i];
		}
		return val[Re] + val[Im];
	}

	/**
	 * This method computes the magnitude of this complex number.
	 *
	 * @return Length of this complex number.
	 */
	constexpr T b3Length() const
	{
		return sqrt(b3SquareLength());
	}

	/**
	 * This method normalizes the magnitude of this complex numer to the
	 * desired length.
	 *
	 * @param len The wanted magnitude of the complex number.
	 * @returns True if the magnitude is a positive non zero value.
	 */
	inline bool    b3Normalize(const T len = 1)
	{
		T old_len = b3Length();

		if (old_len != 0)
		{
			T new_len = len / old_len;

			for (b3_loop i = 0; i < Max; i++)
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
	 * This method returns the phase of both complex parts of
	 * this complex number.
	 *
	 * @return Phase of this complex number.
	 */
	constexpr T b3Phase() const
	{
		return atan2(v[Im], v[Re]);
	}

	/**
	 * This method returns the reference to the real part of this complex
	 * number.
	 *
	 * @return The reference to the real part.
	 */
	constexpr T & b3Real()
	{
		return v[Re];
	}

	/**
	 * This method returns the reference to the imaginary part of this complex
	 * number.
	 *
	 * @return The reference to the imaginary part.
	 */
	constexpr T & b3Imag()
	{
		return v[Im];
	}

	/**
	 * This method computes the square root of each complex component.
	 *
	 * @return The square rooted complex number.
	 */
	inline static b3Complex<T> b3Sqrt(const b3Complex<T> & a)
	{
		b3Complex<T> result;

		if ((a.v[Re] < 0) || (a.v[Im] < 0))
		{
			throw std::domain_error("negative component for sqrt()");
		}
		for (b3_loop i = 0; i < Max; i++)
		{
			result.v[i] = sqrt(a.v[i]);
		}
		return result;
	}

	/**
	 * This method computes the square root of each complex component.
	 *
	 * @return The square rooted complex number.
	 */
	inline b3Complex<T> b3Sqrt() const
	{
		return b3Sqrt(*this);
	}

	/**
	 * This method scales the complex number instance by the components of
	 * another complex number.
	 *
	 * @param other
	 */
	inline b3Complex<T> & b3Scale(const b3Complex<T> & other)
	{
		for (b3_loop i = 0; i < Max; i++)
		{
			v[i] *= other.v[i];
		}

		return *this;
	}

	/**
	 * This method swaps the contents of two complex numbers.
	 *
	 * @param a The first value to swap.
	 * @param b The second value to swap.
	 */
	inline static void b3Swap(b3Complex<T> & a, b3Complex<T> & b)
	{
		std::swap(a, b);
	}

	/**
	 * This method dumps the contents.
	 *
	 */
	inline void b3Dump(
		const char    *    variable,
		const b3_log_level level = B3LOG_NORMAL) const
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

		snprintf(buffer, sizeof(buffer), "%s(re=%2.3f im=%2.3f)",
			variable == nullptr ? "b3Complex" : variable,
			v[Re], v[Im]);

		return std::string(buffer);
	}

	friend inline std::ostream & operator<<(
		std::ostream & os, const b3Complex<T> & complex)
	{
		const std::string & dump(complex);

		os << dump;
		return os;
	}
};

#endif
