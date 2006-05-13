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

#ifndef B3_BASE_COMPLEX64_H
#define B3_BASE_COMPLEX64_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Complex.h"

#ifdef BLZ3_USE_SSE2

class B3_PLUGIN b3Complex64
{
#ifdef SSE_ALIGNED
	__m128d            v;
#else
	b3_f64 B3_ALIGN_16 v[2];
#endif

public:
	inline b3Complex64()
	{
	}

	inline b3Complex64(const b3_f64 re, const b3_f64 im = 0)
	{
		SSE_PD_STORE(v, _mm_setr_pd(re, im));
	}

	inline void operator=(b3Complex<b3_f64> &orig)
	{
		b3_f64 re = orig.b3Real();
		b3_f64 im = orig.b3Imag();

		SSE_PD_STORE(v, _mm_setr_pd(re, im));
	}

	inline b3Complex64(const b3Complex64 &orig)
	{
		SSE_PD_STORE(v, SSE_PD_LOAD(orig.v));
	}

	inline bool operator==(const b3Complex64 &a)
	{
		__m128d eq = _mm_cmpneq_pd(
			SSE_PD_LOAD(v),
			SSE_PD_LOAD(a.v));

		return _mm_movemask_pd(eq) == 0;
	}

	inline b3Complex64 operator+(const b3Complex64 &sum)
	{
		b3Complex64 result;

		SSE_PD_STORE(result.v, _mm_add_pd(
			SSE_PD_LOAD(v),
			SSE_PD_LOAD(sum.v)));
		return result;
	}

	inline void operator+=(const b3Complex64 &sum)
	{
		SSE_PD_STORE(v, _mm_add_pd(
			SSE_PD_LOAD(v),
			SSE_PD_LOAD(sum.v)));
	}

	inline b3Complex64 operator-(const b3Complex64 &sum)
	{
		b3Complex64 result;

		SSE_PD_STORE(result.v, _mm_sub_pd(
			SSE_PD_LOAD(v),
			SSE_PD_LOAD(sum.v)));
		return result;
	}

	inline void operator-=(const b3Complex64 &sum)
	{
		SSE_PD_STORE(v, _mm_sub_pd(
			SSE_PD_LOAD(v),
			SSE_PD_LOAD(sum.v)));
	}

	inline b3Complex64 operator*(const b3Complex64 &sum)
	{
		b3Complex64 result;

		__m128d b = SSE_PD_LOAD(sum.v);
		__m128d a = SSE_PD_LOAD(v);

		__m128d p1 = _mm_mul_pd(
			a,
			_mm_unpacklo_pd(b,b));

		__m128d p2 = _mm_mul_pd(
			_mm_shuffle_pd(a, a, _MM_SHUFFLE2(0,1)),
			_mm_unpackhi_pd(b,b));

#ifdef BLZ3_USE_SSE3
		SSE_PD_STORE(result.v, _mm_addsub_pd(p1, p2));
#else
		SSE_PD_STORE(result.v, _mm_add_pd(p1, _mm_mul_pd(p2, _mm_setr_pd(-1,1))));
#endif
		return result;
	}

	inline void operator*=(const b3Complex64 &sum)
	{
		__m128d b = SSE_PD_LOAD(sum.v);
		__m128d a = SSE_PD_LOAD(v);

		__m128d p1 = _mm_mul_pd(
			a,
			_mm_unpacklo_pd(b,b));

		__m128d p2 = _mm_mul_pd(
			_mm_shuffle_pd(a, a, _MM_SHUFFLE2(0,1)),
			_mm_unpackhi_pd(b,b));

#ifdef BLZ3_USE_SSE3
		SSE_PD_STORE(v, _mm_addsub_pd(p1, p2));
#else
		SSE_PD_STORE(v, _mm_add_pd(
			p1,
			_mm_mul_pd(p2, _mm_setr_pd(-1,1))));
#endif
	}

	inline b3Complex64 operator*(const b3_f64 value)
	{
		b3Complex64 result;
		__m128d     mul = _mm_set1_pd(value);

		SSE_PD_STORE(result.v, _mm_mul_pd(
			SSE_PD_LOAD(v),
			mul));
		return result;
	}

	inline void operator*=(const b3_f64 value)
	{
		__m128d     mul = _mm_set1_pd(value);

		SSE_PD_STORE(v, _mm_mul_pd(
			SSE_PD_LOAD(v),
			mul));
	}

	inline b3Complex64 operator/(const b3_f64 value)
	{
		b3Complex64 result;
		__m128d     mul = _mm_set1_pd(value);

		SSE_PD_STORE(result.v, _mm_div_pd(
			SSE_PD_LOAD(v),
			mul));
		return result;
	}

	inline void operator/=(const b3_f64 value)
	{
		__m128d     mul = _mm_set1_pd(value);

		SSE_PD_STORE(v, _mm_div_pd(
			SSE_PD_LOAD(v),
			mul));
	}

	inline b3_f64 b3SquareLength()
	{
		b3_f64  B3_ALIGN_16 comp[2];

		__m128d mul =
			_mm_mul_pd(SSE_PD_LOAD(v), SSE_PD_LOAD(v));
		_mm_store_pd(comp, mul);

		return comp[0] + comp[1];
	}

	inline b3_f64 b3Length()
	{
		return sqrt(b3SquareLength());
	}

	inline b3_f64 &b3Real()
	{
#ifdef SSE_ALIGNED
		return (b3_f64 &)v;
#else
		return v[0];
#endif
	}

	inline b3_f64 &b3Imag()
	{
#ifdef SSE_ALIGNED
		b3_f64 *ptr = (b3_f64 *)&v;
		
		return ptr[1];
#else
		return v[1];
#endif
	}
};

#else

typedef b3Complex<b3_f64> b3Complex64;

#endif
#endif
