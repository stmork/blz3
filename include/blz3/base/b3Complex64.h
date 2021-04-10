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
#ifdef BLZ3_USE_SSE2
	__m128d     v;
#else
	b3_f64      v[2];
#endif

#ifdef BLZ3_USE_SSE2
	inline b3Complex64(const __m128d in)
	{
		SSE_PD_STORE(v, in);
	}
#endif

public:
	inline b3Complex64() = default;

	inline b3Complex64(const b3_f64 re)
	{
#ifdef BLZ3_USE_SSE2
		v = _mm_set_sd(re);
#else
		v[0] = re;
		v[1] = 0;
#endif
	}

	inline void operator=(const b3_f64 re)
	{
#ifdef BLZ3_USE_SSE2
		v = _mm_set_sd(re);
#else
		v[0] = re;
		v[1] = 0;
#endif
	}

	inline b3Complex64(const b3_f64 re, const b3_f64 im)
	{
		b3Real() = re;
		b3Imag() = im;
	}

	inline b3Complex64(const b3Complex64 & orig)
	{
#ifdef BLZ3_USE_SSE2
		v = orig.v;
#else
		for (int i = 0; i < 2; i++)
		{
			v[i] = orig.v[i];
		}
#endif
	}

	inline void operator=(const b3Complex64 & orig)
	{
#ifdef BLZ3_USE_SSE2
		v = orig.v;
#else
		for (int i = 0; i < 2; i++)
		{
			v[i] = orig.v[i];
		}
#endif
	}

	inline static void b3CopyUncached(b3Complex64 & dst, const b3Complex64 & src)
	{
#ifdef BLZ3_USE_SSE2
		_mm_stream_pd(&dst.b3Real(), src.v);
#else
		const int * srcPtr = reinterpret_cast<const int *>(&src.v);
		int * dstPtr = reinterpret_cast<int *>(&dst.v);

		_mm_stream_si32(dstPtr++, *srcPtr++);
		_mm_stream_si32(dstPtr++, *srcPtr++);
		_mm_stream_si32(dstPtr++, *srcPtr++);
		_mm_stream_si32(dstPtr++, *srcPtr++);
#endif
	}

	inline void operator=(b3Complex<b3_f64> & a)
	{
		b3Real() = a.b3Real();
		b3Imag() = a.b3Imag();
	}

	inline bool operator==(const b3Complex64 & a)
	{
#ifdef BLZ3_USE_SSE2
		__m128d eq = _mm_cmpneq_pd(v, a.v);

		return _mm_movemask_pd(eq) == 0;
#else
#endif
	}

	inline b3Complex64 operator+(const b3Complex64 & sum) const
	{
		b3Complex64 result;

#ifdef BLZ3_USE_SSE2
		result.v = _mm_add_pd(v, sum.v);
#else
		for (int i = 0; i < 2; i++)
		{
			result.v[i] = v[i] + sum.v[i];
		}
#endif
		return result;
	}

	inline void operator+=(const b3Complex64 & sum)
	{
#ifdef BLZ3_USE_SSE2
		v = _mm_add_pd(v, sum.v);
#else
		for (int i = 0; i < 2; i++)
		{
			v[i] += sum.v[i];
		}
#endif
	}

	inline b3Complex64 operator-(const b3Complex64 & sum) const
	{
		b3Complex64 result;

#ifdef BLZ3_USE_SSE2
		result.v =_mm_sub_pd(v, sum.v);
#else
		for (int i = 0; i < 2; i++)
		{
			result.v[i] = v[i] - sum.v[i];
		}
#endif
		return result;
	}

	inline void operator-=(const b3Complex64 & sum)
	{
#ifdef BLZ3_USE_SSE2
		v = _mm_sub_pd(v, sum.v);
#else
		for (int i = 0; i < 2; i++)
		{
			v[i] -= sum.v[i];
		}
#endif
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

	inline void b3Square()
	{
		v = product(*this);
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

#ifdef BLZ3_USE_SSE2
		result.v = _mm_mul_pd(v, _mm_set1_pd(value));
#else
		for (int i = 0; i < 2; i++)
		{
			result.v[i] = v[i] * value;
		}
#endif
		return result;
	}

	inline void operator*=(const b3_f64 value)
	{
#ifdef BLZ3_USE_SSE2
		v = _mm_mul_pd(v, _mm_set1_pd(value));
#else
		for (int i = 0; i < 2; i++)
		{
			v[i] *= value;
		}
#endif
	}

	inline b3Complex64 operator/(const b3_f64 value) const
	{
		b3Complex64 result;

#ifdef BLZ3_USE_SSE2
		result.v = _mm_div_pd(v, _mm_set1_pd(value));
#else
		for (int i = 0; i < 2; i++)
		{
			result.v[i] = v[i] / value;
		}
#endif
		return result;
	}

	inline void operator/=(const b3_f64 value)
	{
#ifdef BLZ3_USE_SSE2
		v = _mm_div_pd(v, _mm_set1_pd(value));
#else
		for (int i = 0; i < 2; i++)
		{
			v[i] /= value;
		}
#endif
	}

	inline b3_f64 b3SquareLength() const
	{
#ifdef BLZ3_USE_SSE2
		alignas(16) b3_f64 comp[2];

		_mm_store_pd(comp, _mm_mul_pd(v, v));

		return comp[0] + comp[1];
#else
		return v[0] * v[0] + v[1] * v[1];
#endif
	}

	inline b3_f64 b3Length() const
	{
		return sqrt(b3SquareLength());
	}

	inline b3_f64 & b3Real() const
	{
#ifdef BLZ3_USE_SSE2
		return (b3_f64 &)v;
#else
		return v[0];
#endif
	}

	inline b3_f64 & b3Imag() const
	{
#ifdef BLZ3_USE_SSE2
		b3_f64 * ptr = (b3_f64 *)&v;

		return ptr[1];
#else
		return v[1];
#endif
	}

	inline static b3Complex64 b3Sqrt(const b3Complex64 & a)
	{
		_MM_SET_EXCEPTION_STATE(0);
		__m128d s = _mm_sqrt_pd(SSE_PD_LOAD(a.v));
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
#ifdef BLZ3_USE_SSE2
		__m128d aux = a.v;
		a.v = b.v;
		b.v = aux;
#else
		b3_f64 aux;

		aux = a.v[0];
		a.v[0] = b.v[0];
		b.v[0] = aux;
		aux = a.v[1];
		a.v[1] = b.v[1];
		b.v[1] = aux;
#endif
	}
};

#else

typedef b3Complex<b3_f64> b3Complex64;

#endif
#endif
