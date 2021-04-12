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

	inline b3Complex64(const __m128d in)
	{
		v = in;
	}

public:
	enum b3_complex_index
	{
		Re = 0, //!< Index of real part of complex number
		Im = 1  //!< Index of imaginary part of complex number
	};

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

	inline b3Complex64 & operator=(const b3_f64 re)
	{
		v = _mm_set_sd(re);

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
	inline operator std::complex<b3_f64>() const
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
		__m128d eq = _mm_cmpneq_pd(v, other.v);

		return _mm_movemask_pd(eq) == 0;
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic operators                          **
	**                                                                      **
	*************************************************************************/

	inline b3Complex64 operator+(const b3Complex64 & sum) const
	{
		b3Complex64 result;

		result.v = _mm_add_pd(v, sum.v);
		return result;
	}

	inline void operator+=(const b3Complex64 & sum)
	{
		v = _mm_add_pd(v, sum.v);
	}

	inline b3Complex64 operator-(const b3Complex64 & sum) const
	{
		b3Complex64 result;

		result.v = _mm_sub_pd(v, sum.v);
		return result;
	}

	inline void operator-=(const b3Complex64 & sum)
	{
		v = _mm_sub_pd(v, sum.v);
	}

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

	inline b3Complex64 operator*(const b3Complex64 & mul) const
	{
		return product(mul);
	}

	inline void operator*=(const b3Complex64 & mul)
	{
		v = product(mul);
	}

	inline b3Complex64 operator*(const b3_f64 value) const
	{
		b3Complex64 result;

		result.v = _mm_mul_pd(v, _mm_set1_pd(value));
		return result;
	}

	inline void operator*=(const b3_f64 value)
	{
		v = _mm_mul_pd(v, _mm_set1_pd(value));
	}

	inline b3Complex64 operator/(const b3_f64 value) const
	{
		b3Complex64 result;

		result.v = _mm_div_pd(v, _mm_set1_pd(value));
		return result;
	}

	inline void operator/=(const b3_f64 value)
	{
		v = _mm_div_pd(v, _mm_set1_pd(value));
	}

	/*************************************************************************
	**                                                                      **
	**                        Arithmetic modifiers                          **
	**                                                                      **
	*************************************************************************/

	inline void b3Square()
	{
		v = product(*this);
	}

	inline b3_f64 b3SquareLength() const
	{
		alignas(16) b3_f64 comp[2];

		_mm_store_pd(comp, _mm_mul_pd(v, v));

		return comp[Re] + comp[Im];
	}

	inline b3_f64 b3Length() const
	{
		return sqrt(b3SquareLength());
	}

	inline static b3Complex64 b3Sqrt(const b3Complex64 & a)
	{
		_MM_SET_EXCEPTION_STATE(0);
		__m128d s = _mm_sqrt_pd(a.v);
		unsigned int csr = _mm_getcsr();

		if (csr & _MM_EXCEPT_INVALID)
		{
			throw std::domain_error("negative component for sqrt()");
		}

		return b3Complex64(s);
	}

	inline void b3Scale(const b3Complex64 & a)
	{
		v = _mm_mul_pd(v, a.v);
	}

	inline static void b3Swap(b3Complex64 & a, b3Complex64 & b)
	{
		const __m128d aux = a.v;

		a.v = b.v;
		b.v = aux;
	}

	inline static void b3CopyUncached(b3Complex64 & dst, const b3Complex64 & src)
	{
		_mm_stream_pd(&dst.b3Real(), src.v);
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
};

#else

typedef b3Complex<b3_f64> b3Complex64;

#endif
#endif
