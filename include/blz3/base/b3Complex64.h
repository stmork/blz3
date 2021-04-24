/*
**
**	$Filename:	b3Complex64.h $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Double precision complex number handling
**
**	(C) Copyright 2006  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_COMPLEX64_H
#define B3_BASE_COMPLEX64_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Complex.h"

#ifdef BLZ3_USE_SSE2

#include <stdexcept>

class B3_PLUGIN alignas(16) b3Complex64
{
	__m128d     v;

	/**
	 * This enumeration contains the indices of the complex number parts.
	 */
	enum b3_complex_index
	{
		Re = 0, //!< Index of real part of complex number
		Im,     //!< Index of imaginary part of complex number
		Max
	};

	inline b3Complex64(const __m128d input)
	{
		v = input;
	}

public:

	/*************************************************************************
	**                                                                      **
	**                        Constructors                                  **
	**                                                                      **
	*************************************************************************/

	inline b3Complex64() = default;

	inline b3Complex64(const b3Complex64 & other)
	{
		v = other.v;
	}

	inline b3Complex64(const std::complex<b3_f64> & other)
	{
		v = _mm_set_pd(other.imag(), other.real());
	}

	inline b3Complex64(const b3_f64 re)
	{
		v = _mm_set_sd(re);
	}

	inline b3Complex64(const b3_f64 re, const b3_f64 im)
	{
		v = _mm_set_pd(im, re);
	}

	/*************************************************************************
	**                                                                      **
	**                        Assignment operators                          **
	**                                                                      **
	*************************************************************************/

	inline b3Complex64 & operator=(const b3_f64 re)
	{
		v = _mm_set_sd(re);

		return *this;
	}

	inline b3Complex64 & operator=(const b3Complex64 & other)
	{
		v = other.v;

		return *this;
	}

	inline b3Complex64 & operator=(const std::complex<b3_f64> & other)
	{
		v = _mm_set_pd(other.imag(), other.real());

		return *this;
	}

	template<class O> inline b3Complex64 & operator=(b3Complex<O> & other)
	{
		v = _mm_set_pd(other.b3Imag(), other.b3Real());

		return *this;
	}

	/*************************************************************************
	**                                                                      **
	**                        Cast operators                                **
	**                                                                      **
	*************************************************************************/

	/**
	 * This returns a std::complex instance from this complex representation.
	 */
	inline operator const std::complex<b3_f64>() const
	{
		alignas(16) b3_f64 comp[2];

		_mm_store_pd(comp, v);

		return std::complex<b3_f64>(comp[Re], comp[Im]);
	}

	/*************************************************************************
	**                                                                      **
	**                        Comparison operators                          **
	**                                                                      **
	*************************************************************************/

	inline bool operator==(const b3Complex64 & other) const
	{
		const __m128d eq = _mm_cmpneq_pd(v, other.v);

		return _mm_movemask_pd(eq) == 0;
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic operators                          **
	**                                                                      **
	*************************************************************************/

	inline const b3Complex64 operator+(const b3Complex64 & sum) const
	{
		return _mm_add_pd(v, sum.v);
	}

	inline b3Complex64 & operator+=(const b3Complex64 & sum)
	{
		v = _mm_add_pd(v, sum.v);

		return *this;
	}

	inline const b3Complex64 operator-(const b3Complex64 & sum) const
	{
		return _mm_sub_pd(v, sum.v);
	}

	inline b3Complex64 & operator-=(const b3Complex64 & sum)
	{
		v = _mm_sub_pd(v, sum.v);

		return *this;
	}

	inline const b3Complex64 operator*(const b3Complex64 & mul) const
	{
		return product(mul);
	}

	inline b3Complex64  & operator*=(const b3Complex64 & mul)
	{
		v = product(mul);

		return *this;
	}

	inline const b3Complex64 operator*(const b3_f64 value) const
	{
		return _mm_mul_pd(v, _mm_set1_pd(value));
	}

	inline b3Complex64  & operator*=(const b3_f64 value)
	{
		v = _mm_mul_pd(v, _mm_set1_pd(value));

		return *this;
	}

	inline const b3Complex64 operator/(const b3Complex64 & divisor) const
	{
		return division(divisor);
	}

	inline b3Complex64  & operator/=(const b3Complex64 & divisor)
	{
		v = division(divisor);

		return *this;
	}

	inline const b3Complex64 operator/(const b3_f64 value) const
	{
		return _mm_div_pd(v, _mm_set1_pd(value));
	}

	inline b3Complex64 & operator/=(const b3_f64 value)
	{
		v = _mm_div_pd(v, _mm_set1_pd(value));

		return *this;
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic modifiers                          **
	**                                                                      **
	*************************************************************************/

	inline b3Complex64 & b3Square()
	{
		v = product(*this);

		return *this;
	}

	inline b3_f64 b3SquareLength() const
	{
		b3_f64 comp[2];

		_mm_store_pd(comp, _mm_mul_pd(v, v));

		return comp[Re] + comp[Im];
	}

	inline b3_f64 b3Length() const
	{
		return sqrt(b3SquareLength());
	}

	inline bool   b3Normalize(const b3_f64 len = 1)
	{
		b3_f64 old_len = b3Length();

		if (old_len != 0)
		{
			v *= len / old_len;

			return true;
		}
		else
		{
			return false;
		}
	}

	inline b3_f64 b3Phase() const
	{
		return atan2(b3Imag(), b3Real());
	}

	inline b3_f64 & b3Real() const
	{
		return (b3_f64 &)v;
	}

	inline b3_f64 & b3Imag() const
	{
		b3_f64 * ptr = (b3_f64 *)&v;

		return ptr[Im];
	}

	inline static const b3Complex64 b3Sqrt(const b3Complex64 & a)
	{
		__m128d lt = _mm_cmplt_pd(a.v, _mm_set1_pd(0.0));

		if (_mm_movemask_pd(lt) != 0)
		{
			throw std::domain_error("negative component for sqrt()");
		}

		return _mm_sqrt_pd(a.v);
	}

	inline b3Complex64 b3Sqrt() const
	{
		return b3Sqrt(*this);
	}

	inline b3Complex64 & b3Scale(const b3Complex64 & a)
	{
		v = _mm_mul_pd(v, a.v);

		return *this;
	}

	inline static void b3Swap(b3Complex64 & a, b3Complex64 & b)
	{
		const __m128d aux = a.v;

		a.v = b.v;
		b.v = aux;
	}

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
		std::ostream & os, const b3Complex64 & complex)
	{
		const std::string & dump(complex);

		os << dump;
		return os;
	}

	inline static void b3CopyUncached(b3Complex64 & dst, const b3Complex64 & src)
	{
		_mm_stream_pd(&dst.b3Real(), src.v);
	}

private:
	inline __m128d product(const b3Complex64 & mul) const
	{
		__m128d p1 = _mm_mul_pd(v,
				_mm_unpacklo_pd(mul.v, mul.v));

		__m128d p2 = _mm_mul_pd(
				_mm_shuffle_pd(v, v, _MM_SHUFFLE2(0, 1)),
				_mm_unpackhi_pd(mul.v, mul.v));

#ifdef BLZ3_USE_SSE3
		return _mm_addsub_pd(p1, p2);
#else
		return _mm_add_pd(p1, _mm_mul_pd(p2, _mm_setr_pd(-1, 1)));
#endif
	}

	inline __m128d division(const b3Complex64 & divisor) const
	{
		const b3Complex64 num   = divisor.v * v;
		const b3_f64      denom = divisor.b3SquareLength();
		const b3Complex64 val(
			num.b3Real() + num.b3Imag(),
			b3Imag() * divisor.b3Real() - b3Real() * divisor.b3Imag());

		return _mm_div_pd(val.v, _mm_set1_pd(denom));
	}
};

#else

typedef b3Complex<b3_f64> b3Complex64;

#endif
#endif
