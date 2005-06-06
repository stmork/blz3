/*
**
**	$Filename:	b3Matrix.h            
**	$Release:	Dortmund 2001, 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork
**
**	Blizzard III - vector and matrix calculation proto types
**
**	(C) Copyright 2001, 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_MATRIX_H
#define B3_BASE_MATRIX_H

#include "blz3/b3Config.h"
#include <float.h>

#define b3ArcAngleOfScalars(u,v) (atan2((double)v,(double)u)                + ((v) < 0 ? (M_PI * 2.0) : 0))
#define b3RelAngleOfScalars(u,v) (atan2((double)v,(double)u) *   0.5 / M_PI + ((v) < 0 ?   1.0 : 0))
#define b3AngleOfScalars(u,v)    (atan2((double)v,(double)u) * 180.0 / M_PI + ((v) < 0 ? 360.0 : 0))

#define B3_SSE_DIM 4

class b3Vector
{
	enum b3_vector_index
	{
		X = 0,
		Y,
		Z,
		W,
		B3_MAX_DIM
	};

public:
	static inline b3_vector *b3Init(
		      b3_vector *vec,
		const b3_f64     x = 0,
		const b3_f64     y = 0,
		const b3_f64     z = 0)
	{
		vec->x   = (b3_f32)x;
		vec->y   = (b3_f32)y;
		vec->z   = (b3_f32)z;

		return vec;
	}

	static inline b3_vector64 *b3Init(
		      b3_vector64 *vec,
		const b3_f64       x = 0,
		const b3_f64       y = 0,
		const b3_f64       z = 0)
	{
		vec->x   = x;
		vec->y   = y;
		vec->z   = z;

		return vec;
	}

	static inline b3_vector	*b3Init(
		      b3_vector   *vec,
		const b3_vector   *src)
	{
		*vec = *src;
		return vec;
	}

	static inline b3_vector *b3Init(
		      b3_vector   *vec,
		const b3_vector64 *src)
	{
#ifdef B3_SSE
		const b3_f64 *s = &src->x;
		      b3_f32 *d = &vec->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			d[i] = (b3_f32)s[i];
		}
#else
		vec->x   = (b3_f32)src->x;
		vec->y   = (b3_f32)src->y;
		vec->z   = (b3_f32)src->z;
#endif

		return vec;
	}

	static inline b3_vector64 *b3Init(
		      b3_vector64 *vec,
		const b3_vector   *src)
	{
#ifdef B3_SSE
		const b3_f32 *s = &src->x;
		      b3_f64 *d = &vec->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			d[i] = s[i];
		}
#else
		vec->x = src->x;
		vec->y = src->y;
		vec->z = src->z;
#endif
		return vec;
	}

	static inline b3_vector64 *b3Init(
		      b3_vector64 *vec,
		const b3_vector64 *src)
	{
		*vec = *src;

		return vec;
	}

	static inline b3_bool b3IsEqual(
		const b3_vector *vec1,
		const b3_vector *vec2)
	{
#ifdef B3_SSE
		const b3_f32 *v1 = &vec1->x;
		const b3_f32 *v2 = &vec2->x;
		      b3_f32   d[B3_SSE_DIM];
		      b3_f32   result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			d[i]    = v2[i] - v1[i];
			result += d[i]  * d[i];
		}
		return result < 0.0001;
#else
		b3_f32 x = vec2->x - vec1->x;
		b3_f32 y = vec2->y - vec1->y;
		b3_f32 z = vec2->z - vec1->z;

		return (x*x + y*y + z*z) < 0.0001;
#endif
	}

	static inline b3_vector *b3Negate(b3_vector *negate)
	{
#ifdef B3_SSE
		b3_f32 *n = &negate->x;
		
		for (b3_loop i = 0;i < 3;i++)
		{
			n[i] = -n[i];
		}
#else
		negate->x   = -negate->x;
		negate->y   = -negate->y;
		negate->z   = -negate->z;
#endif
		return negate;
	}

	static inline b3_vector64 *b3Negate(b3_vector64 *negate)
	{
#ifdef B3_SSE
		b3_f64 *n = &negate->x;
		
		for (b3_loop i = 0;i < 3;i++)
		{
			n[i] = -n[i];
		}
#else
		negate->x   = -negate->x;
		negate->y   = -negate->y;
		negate->z   = -negate->z;
#endif
		return negate;
	}

	static inline b3_f64 b3Normalize(
		      b3_vector *vector,
		const b3_f64     length = 1.0)
	{
		register b3_f64 x,y,z,denom,result = 0;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		denom = x * x + y * y + z * z;
		if (denom > 0)
		{
			denom     = length / (result = sqrt(denom));
			vector->x = (b3_f32)(x * denom);
			vector->y = (b3_f32)(y * denom);
			vector->z = (b3_f32)(z * denom);
		}
		return result;
	}

	static inline b3_f64 b3Normalize(
		      b3_vector64 *vector,
		const b3_f64       length = 1.0)
	{
		register b3_f64 x,y,z,denom,result = 0;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		denom = x * x + y * y + z * z;
		if (denom > 0)
		{
			denom     = length / (result = sqrt(denom));
			vector->x = x * denom;
			vector->y = y * denom;
			vector->z = z * denom;
		}
		return result;
	}

	static inline b3_f64 b3Normalize(
		      b3_gl_vector *vector,
		const b3_f64        length = 1.0)
	{
		register b3_f64 x,y,z,denom,result = 0;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		denom = x * x + y * y + z * z;
		if (denom > 0)
		{
			denom     = length / (result = sqrt(denom));
			vector->x = (b3_f32)(x * denom);
			vector->y = (b3_f32)(y * denom);
			vector->z = (b3_f32)(z * denom);
		}
		return result;
	}

	static inline b3_f64 b3SMul(const b3_vector *aVec,const b3_vector *bVec)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32  result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			result += a[i] * b[i];
		}
		return result;
#else
		return
			aVec->x * bVec->x +
			aVec->y * bVec->y +
			aVec->z * bVec->z;
#endif
	}

	static inline b3_f64 b3SMul(const b3_vector *aVec,const b3_vector64 *bVec)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f64 *b = &bVec->x;
		      b3_f64  result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			result += a[i] * b[i];
		}
		return result;
#else
		return
			aVec->x * bVec->x +
			aVec->y * bVec->y +
			aVec->z * bVec->z;
#endif
	}

	static inline b3_f64 b3SMul(const b3_vector64 *aVec,const b3_vector *bVec)
	{
		return b3SMul(bVec,aVec);
	}

	static inline b3_f64 b3SMul(const b3_vector64 *aVec,const b3_vector64 *bVec)
	{
#ifdef B3_SSE
		const b3_f64 *a = &aVec->x;
		const b3_f64 *b = &bVec->x;
		      b3_f64  result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			result += a[i] * b[i];
		}
		return result;
#else
		return
			aVec->x * bVec->x +
			aVec->y * bVec->y +
			aVec->z * bVec->z;
#endif
	}

	static inline b3_vector *b3Add(const b3_vector *aVec,b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < B3_SSE_DIM;i++)
		{
			r[i] += a[i];
		}
#else
		result->x += aVec->x;
		result->y += aVec->y;
		result->z += aVec->z;
#endif
		return result;
	}

	static inline b3_vector *b3Add(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < B3_SSE_DIM;i++)
		{
			r[i] = a[i] + b[i];
		}
#else
		result->x = aVec->x + bVec->x;
		result->y = aVec->y + bVec->y;
		result->z = aVec->z + bVec->z;
#endif
		return result;
	}

	static inline b3_vector64 *b3Add(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE
		const b3_f64 *a = &aVec->x;
		const b3_f64 *b = &bVec->x;
		      b3_f64 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + b[i];
		}
#else
		result->x = aVec->x + bVec->x;
		result->y = aVec->y + bVec->y;
		result->z = aVec->z + bVec->z;
#endif
		return result;
	}

	static inline b3_gl_vector *b3Add(const b3_gl_vector *aVec,b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] += a[i];
		}
#else
		result->x += aVec->x;
		result->y += aVec->y;
		result->z += aVec->z;
#endif
		return result;
	}

	static inline b3_gl_vector *b3Add(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
	{
		result->x = aVec->x + bVec->x;
		result->y = aVec->y + bVec->y;
		result->z = aVec->z + bVec->z;
		return result;
	}

	static inline b3_vector *b3Sub(const b3_vector *aVec,b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] -= a[i];
		}
#else
		result->x -= aVec->x;
		result->y -= aVec->y;
		result->z -= aVec->z;
#endif

		return result;
	}

	static inline b3_vector *b3Sub(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] - b[i];
		}
#else
		result->x = aVec->x - bVec->x;
		result->y = aVec->y - bVec->y;
		result->z = aVec->z - bVec->z;
#endif
		return result;
	}

	static inline b3_gl_vector *b3Sub(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] - b[i];
		}
#else
		result->x = aVec->x - bVec->x;
		result->y = aVec->y - bVec->y;
		result->z = aVec->z - bVec->z;
#endif
		return result;
	}

	static inline b3_vector *b3Mul(const b3_vector *aVec,b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] *= a[i];
		}
#else
		result->x *= aVec->x;
		result->y *= aVec->y;
		result->z *= aVec->z;
#endif
		return result;
	}

	static inline b3_vector *b3Mul(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] * b[i];
		}
#else
		result->x = aVec->x * bVec->x;
		result->y = aVec->y * bVec->y;
		result->z = aVec->z * bVec->z;
#endif
		return result;
	}

	static inline b3_gl_vector *b3Mul(const b3_gl_vector *aVec,b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] *= a[i];
		}
#else
		result->x *= aVec->x;
		result->y *= aVec->y;
		result->z *= aVec->z;
#endif
		return result;
	}

	static inline b3_gl_vector *b3Mul(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] * b[i];
		}
#else
		result->x = aVec->x * bVec->x;
		result->y = aVec->y * bVec->y;
		result->z = aVec->z * bVec->z;
#endif
		return result;
	}

	static inline b3_vector *b3CrossProduct(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE
		b3_f32 B3_ALIGN_16 *r = &result->x;
		b3_f32 B3_ALIGN_16  a[4],b[4],c[4],d[4];

		a[Z] = c[Y] = aVec->x; b[Y] = d[Z] = bVec->x;
		a[X] = c[Z] = aVec->y; b[Z] = d[X] = bVec->y;
		a[Y] = c[X] = aVec->z; b[X] = d[Y] = bVec->z;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] * b[i] - c[i] * d[i];
		}
#else
		result->x = aVec->y * bVec->z - aVec->z * bVec->y;
		result->y = aVec->z * bVec->x - aVec->x * bVec->z;
		result->z = aVec->x * bVec->y - aVec->y * bVec->x;
#endif
		return result;
	}

	static inline b3_vector64 *b3CrossProduct(
		const b3_vector   *aVec,
		const b3_vector   *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE
		b3_f64 B3_ALIGN_16 *r = &result->x;
		b3_f64 B3_ALIGN_16  a[4],b[4],c[4],d[4];

		a[Z] = c[Y] = aVec->x; b[Y] = d[Z] = bVec->x;
		a[X] = c[Z] = aVec->y; b[Z] = d[X] = bVec->y;
		a[Y] = c[X] = aVec->z; b[X] = d[Y] = bVec->z;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] * b[i] - c[i] * d[i];
		}
#else
		result->x = (b3_f64)aVec->y * (b3_f64)bVec->z - (b3_f64)aVec->z * (b3_f64)bVec->y;
		result->y = (b3_f64)aVec->z * (b3_f64)bVec->x - (b3_f64)aVec->x * (b3_f64)bVec->z;
		result->z = (b3_f64)aVec->x * (b3_f64)bVec->y - (b3_f64)aVec->y * (b3_f64)bVec->x;
#endif

		return result;
	}

	static inline b3_vector64 *b3CrossProduct(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE
		b3_f64 B3_ALIGN_16 *r = &result->x;
		b3_f64 B3_ALIGN_16  a[4],b[4],c[4],d[4];

		a[Z] = c[Y] = aVec->x; b[Y] = d[Z] = bVec->x;
		a[X] = c[Z] = aVec->y; b[Z] = d[X] = bVec->y;
		a[Y] = c[X] = aVec->z; b[X] = d[Y] = bVec->z;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] * b[i] - c[i] * d[i];
		}
#else
		result->x = aVec->y * bVec->z - aVec->z * bVec->y;
		result->y = aVec->z * bVec->x - aVec->x * bVec->z;
		result->z = aVec->x * bVec->y - aVec->y * bVec->x;
#endif
		return result;
	}

	static inline b3_gl_vector *b3CrossProduct(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
	{
		result->x = aVec->y * bVec->z - aVec->z * bVec->y;
		result->y = aVec->z * bVec->x - aVec->x * bVec->z;
		result->z = aVec->x * bVec->y - aVec->y * bVec->x;

		return result;
	}

	static inline b3_f64 b3Length(const b3_vector *vector)
	{
		return sqrt(b3QuadLength(vector));
	}

	static inline b3_f64 b3Length(const b3_vector64 *vector)
	{
		return sqrt(b3QuadLength(vector));
	}

	static inline b3_f64 b3QuadLength(const b3_vector *vector)
	{
#ifdef B3_SSE
		const b3_f32 *v = &vector->x;
		      b3_f32  result = 0;

		for(b3_loop i = 0;i < 3;i++)
		{
			result += v[i] * v[i];
		}
		return result;
#else
		return
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z;
#endif
	}

	static inline b3_f64 b3QuadLength(const b3_vector64 *vector)
	{
#ifdef B3_SSE
		const b3_f64 *v = &vector->x;
		      b3_f64  result = 0;

		for(b3_loop i = 0;i < 3;i++)
		{
			result += v[i] * v[i];
		}
		return result;
#else
		return
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z;
#endif
	}

	static inline b3_f64 b3Distance(
		const b3_vector *from,
		const b3_vector *to)
	{
		register b3_f64 x,y,z;

		x = to->x - from->x;
		y = to->y - from->y;
		z = to->z - from->z;
		return sqrt(x * x + y * y + z * z);
	}

	static inline void b3Scale(b3_vector *vector,const b3_f64 factor)
	{
#ifdef B3_SSE
		b3_f32 *v = &vector->x;
		b3_f32  f = (b3_f32)factor;

		for(b3_loop i = 0;i < 3;i++)
		{
			v[i] *= f;
		}
#else
		vector->x = (b3_f32)(vector->x * factor);
		vector->y = (b3_f32)(vector->y * factor);
		vector->z = (b3_f32)(vector->z * factor);
#endif
	}

	static inline b3_vector *b3Scale(
		      b3_vector *result,
		const b3_vector *vector,
		const b3_f64     factor)
	{
#ifdef B3_SSE
		const b3_f32 *v = &vector->x;
		      b3_f32 *r = &result->x;
		      b3_f32  f = (b3_f32)factor;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = v[i] * f;
		}
#else
		result->x = (b3_f32)(vector->x * factor);
		result->y = (b3_f32)(vector->y * factor);
		result->z = (b3_f32)(vector->z * factor);
#endif
		return result;
	}

	static inline void b3Scale(b3_vector64 *vector,const b3_f64 factor)
	{
#ifdef B3_SSE
		b3_f64 *v = &vector->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			v[i] *= factor;
		}
#else
		vector->x *= factor;
		vector->y *= factor;
		vector->z *= factor;
#endif
	}

	static inline b3_vector64 *b3Scale(
		      b3_vector64 *result,
		const b3_vector64 *vector,
		const b3_f64       factor)
	{
#ifdef B3_SSE
		const b3_f64 *v = &vector->x;
		      b3_f64 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = v[i] * factor;
		}
#else
		result->x = vector->x * factor;
		result->y = vector->y * factor;
		result->z = vector->z * factor;
#endif
		return result;
	}

	static inline b3_vector *b3MatrixMul3D(const b3_matrix *A,b3_vector *vector)
	{
#ifdef B3_SSE
		const b3_f32             *m = &A->m11;
		      b3_f32             *v = &vector->x;
		      b3_f32 B3_ALIGN_16  aux[4];
		      b3_f32              result;

		for (b3_loop o = 0;o < 3;o++)
		{
			aux[o] = v[o];
		}
		aux[3] = 0;

		for (b3_loop o = 0; o < 3; o++)
		{
			result = 0;
			for (b3_loop i = 0; i < 4; i++)
			{
				result += m[i] * aux[i];
			}
			v[o]  = result;
			m    += 4;
		}
#else
		register b3_f64 x,y,z;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		vector->x = (b3_f32)(x * A->m11 + y * A->m12 + z * A->m13);
		vector->y = (b3_f32)(x * A->m21 + y * A->m22 + z * A->m23);
		vector->z = (b3_f32)(x * A->m31 + y * A->m32 + z * A->m33);
#endif

		return vector;
	}

	static inline b3_vector *b3MatrixMul4D(const b3_matrix *A,b3_vector *vector)
	{
#ifdef B3_SSE
		const b3_f32             *m = &A->m11;
		      b3_f32             *v = &vector->x;
		      b3_f32 B3_ALIGN_16  aux[4];
		      b3_f32              result;

		for (b3_loop o = 0;o < 3;o++)
		{
			aux[o] = v[o];
		}
		aux[3] = 1;

		for (b3_loop o = 0; o < 3; o++)
		{
			result = 0;
			for (b3_loop i = 0; i < 4; i++)
			{
				result += m[i] * aux[i];
			}
			v[o]  = result;
			m    += 4;
		}
#else
		register b3_f64 x,y,z;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		vector->x = (b3_f32)(x * A->m11 + y * A->m12 + z * A->m13 + A->m14);
		vector->y = (b3_f32)(x * A->m21 + y * A->m22 + z * A->m23 + A->m24);
		vector->z = (b3_f32)(x * A->m31 + y * A->m32 + z * A->m33 + A->m34);
#endif

		return vector;
	}

	static inline b3_f64 b3AngleOfVectors(
		const b3_vector *Vector1,
		const b3_vector *Vector2)
	{
		b3_f64 Denom = b3Length(Vector1) * b3Length(Vector2);

		if (fabs(Denom) > 0.0001)
		{
			return (
				Vector1->x * Vector2->x  +
				Vector1->y * Vector2->y  +
				Vector1->z * Vector2->z) / Denom;

		}
		return 0.0;
	}

	static inline b3_f64 b3AngleOfPoints(
		const b3_vector *base,
		const b3_vector *point1,
		const b3_vector *point2)
	{
		b3_vector a,b;

		a.x = point1->x - base->x;
		a.y = point1->y - base->y;
		a.z = point1->z - base->z;

		b.x = point2->x - base->x;
		b.y = point2->y - base->y;
		b.z = point2->z - base->z;

		return b3AngleOfVectors(&a,&b);
	}

	static inline b3_vector *b3LinearCombine(
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_f64     factor,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;
		      b3_f32  f = (b3_f32)factor;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + f * b[i];
		}
#else
		result->x = (b3_f32)(aVec->x + factor * bVec->x);
		result->y = (b3_f32)(aVec->y + factor * bVec->y);
		result->z = (b3_f32)(aVec->z + factor * bVec->z);
#endif
		return result;
	}

	static inline b3_gl_vector *b3LinearCombine(
		const b3_vector    *aVec,
		const b3_vector    *bVec,
		const b3_f64        factor,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;
		      b3_f32  f = (b3_f32)factor;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + f * b[i];
		}
#else
		result->x = (b3_f32)(aVec->x + factor * bVec->x);
		result->y = (b3_f32)(aVec->y + factor * bVec->y);
		result->z = (b3_f32)(aVec->z + factor * bVec->z);
#endif
		return result;
	}

	static inline b3_vector *b3LinearCombine(
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_vector *cVec,
		const b3_f64     xFactor,
		const b3_f64     yFactor,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		const b3_f32 *c = &bVec->x;
		      b3_f32 *r = &result->x;
		      b3_f32  x = (b3_f32)xFactor;
		      b3_f32  y = (b3_f32)yFactor;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + x * b[i] + y * c[i];
		}
#else
		result->x = (b3_f32)(aVec->x + xFactor * bVec->x + yFactor * cVec->x);
		result->y = (b3_f32)(aVec->y + xFactor * bVec->y + yFactor * cVec->y);
		result->z = (b3_f32)(aVec->z + xFactor * bVec->z + yFactor * cVec->z);
#endif
		return result;
	}

	static inline b3_gl_vector *b3LinearCombine(
		const b3_vector   *aVec,
		const b3_vector   *bVec,
		const b3_vector   *cVec,
		const b3_f64       xFactor,
		const b3_f64       yFactor,
		      b3_gl_vector *result)
	{
		return (b3_gl_vector *)b3LinearCombine(aVec,bVec,cVec,xFactor,yFactor,(b3_vector *)result);
	}

	static inline void b3Sort(b3_vector *lower,b3_vector *upper)
	{
#ifdef B3_SSE
		b3_f32 *l = &lower->x;
		b3_f32 *u = &upper->x;
		b3_f32  aux;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (l[i] > u[i])
			{
				aux = l[i]; l[i] = u[i]; u[i] = aux;
			}
		}
#else
		b3_f32 aux;

		if (lower->x > upper->x)
		{
			aux = lower->x; lower->x = upper->x; upper->x = aux;
		}

		if (lower->y > upper->y)
		{
			aux = lower->y; lower->y = upper->y; upper->y = aux;
		}

		if (lower->z > upper->z)
		{
			aux = lower->z; lower->z = upper->z; upper->z = aux;
		}
#endif
	}

	static inline void b3Sort(b3_vector64 *lower,b3_vector64 *upper)
	{
#ifdef B3_SSE
		b3_f64 *l = &lower->x;
		b3_f64 *u = &upper->x;
		b3_f64  aux;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (l[i] > u[i])
			{
				aux = l[i]; l[i] = u[i]; u[i] = aux;
			}
		}
#else
		b3_f64 aux;

		if (lower->x > upper->x)
		{
			aux = lower->x; lower->x = upper->x; upper->x = aux;
		}

		if (lower->y > upper->y)
		{
			aux = lower->y; lower->y = upper->y; upper->y = aux;
		}

		if (lower->z > upper->z)
		{
			aux = lower->z; lower->z = upper->z; upper->z = aux;
		}
#endif
	}
	
	static inline void b3InitBound(
		b3_vector *lower,
		b3_vector *upper)
	{
#ifdef B3_SSE
		b3_f32 *l = &lower->x;
		b3_f32 *u = &upper->x;

		for (b3_loop i = 0;i < 3; i++)
		{
			l[i] =  FLT_MAX;
			u[i] = -FLT_MAX;
		}
#else
		lower->x =
		lower->y =
		lower->z =  FLT_MAX;
		upper->x =
		upper->y =
		upper->z = -FLT_MAX;
#endif
	}

	static inline void b3SetMinimum(
		b3_vector *vector,
		b3_f32     min)
	{
#ifdef B3_SSE
		b3_f32 *v = &vector->x;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (v[i] < min) v[i] = min;
		}
#else
		if (vector->x < min) vector->x = min;
		if (vector->y < min) vector->y = min;
		if (vector->z < min) vector->z = min;
#endif
	}

	static inline void b3CheckLowerBound(
		      b3_vector *lower,
		const b3_vector *point)
	{
#ifdef B3_SSE
		      b3_f32 *l = &lower->x;
		const b3_f32 *p = &point->x;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (p[i] < l[i]) l[i] = p[i];
		}
#else
		if (point->x < lower->x) lower->x = point->x;
		if (point->y < lower->y) lower->y = point->y;
		if (point->z < lower->z) lower->z = point->z;
#endif
	}

	static inline void b3SetMaximum(
		b3_vector *vector,
		b3_f32     max)
	{
#ifdef B3_SSE
		b3_f32 *v = &vector->x;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (v[i] > max) v[i] = max;
		}
#else
		if (vector->x > max) vector->x = max;
		if (vector->y > max) vector->y = max;
		if (vector->z > max) vector->z = max;
#endif
	}

	static inline void b3CheckUpperBound(
		      b3_vector *upper,
		const b3_vector *point)
	{
#ifdef B3_SSE
		      b3_f32 *u = &upper->x;
		const b3_f32 *p = &point->x;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (p[i] > u[i]) u[i] = p[i];
		}
#else
		if (point->x > upper->x) upper->x = point->x;
		if (point->y > upper->y) upper->y = point->y;
		if (point->z > upper->z) upper->z = point->z;
#endif
	}

	static inline void b3AdjustBound(
		const b3_vector *point,
		      b3_vector *lower,
		      b3_vector *upper)
	{
		b3CheckLowerBound(lower,point);
		b3CheckUpperBound(upper,point);
	}

	static inline void b3AdjustBound(
		const b3_gl_vector *point,
		      b3_vector    *lower,
		      b3_vector    *upper)
	{
		b3_vector test;

		b3Vector::b3Init(&test,point->x,point->y,point->z);
		b3CheckLowerBound(lower,&test);
		b3CheckUpperBound(upper,&test);
	}
};

class b3Matrix
{
	static b3_matrix m_UnitMatrix;

public:
	static b3_vector m_EmptyVector;

	static b3_bool    b3NormalizeCol (b3_matrix *Dst,b3_index col_num);
	static b3_bool    b3NormalizeRow (b3_matrix *Dst,b3_index row_num);
	static b3_f64     b3Det4         (b3_matrix *Matrix);

	static b3_matrix *b3Unit         (b3_matrix *Dst);
	static b3_matrix *b3Inverse      (b3_matrix *Src,b3_matrix *Dst);
	static b3_matrix *b3Transport    (b3_matrix *Src,b3_matrix *Dst);
	static b3_matrix *b3Move         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Move);
	static b3_matrix *b3Move         (b3_matrix *Src,b3_matrix *Dst,b3_f64 x,b3_f64 y,b3_f64 z);
	static b3_matrix *b3MoveNegative (b3_matrix *Src,b3_matrix *Dst,b3_vector *MoveNeg);
	static b3_matrix *b3Scale        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_vector *Scale);
	static b3_matrix *b3Scale        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 x = 1.0,b3_f64 y = 1.0,b3_f64 z = 1.0);
	static b3_matrix *b3RotateX      (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
	static b3_matrix *b3RotateY      (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
	static b3_matrix *b3RotateZ      (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
	static b3_matrix *b3MirrorPoint  (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 scale);
	static b3_matrix *b3MirrorAxis   (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,  b3_f64 scale);
	static b3_matrix *b3MirrorPlane  (b3_matrix *Src,b3_matrix *Dst,b3_vector *base,b3_vector *uDir,b3_vector *vDir,b3_f64 scale);
	static b3_matrix *b3MMul         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);
	static b3_matrix *b3SMul         (b3_matrix *Src,b3_matrix *Dst,b3_f64     factor);
	static b3_matrix *b3MAdd         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);
	static b3_matrix *b3Align        (b3_matrix *Dst,const b3_line *axis);
	static b3_matrix *b3RotateVector (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,b3_f64 angle);
	static b3_matrix *b3Dress        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_vector *Dir1,b3_vector *Dir2,b3_bool future);
	static b3_matrix *b3Dump         (b3_matrix *mtx,const char *title=null);

	static inline b3_bool    b3IsUnitMatrix(b3_matrix *test_matrix)
	{
		b3_f32 *ptr1 = &test_matrix->m11;
		b3_f32 *ptr2 = &m_UnitMatrix.m11;

		for (int i = 0;i < 16;i++)
		{
			if (fabs(*ptr1++ - *ptr2++) > 0.001f)
			{
				return false;
			}
		}
		return true;
	}

	static inline b3_f64     b3Det2  (b3_f64 a,b3_f64 b,b3_f64 c,b3_f64 d)
	{
		return a * d - b * c;
	}

	static inline b3_f64     b3Det3  (b3_vector *a,b3_vector *b,b3_vector *c)
	{
		return
			a->x * b->y * c->z -
			c->x * b->y * a->z +
			b->x * c->y * a->z -
			b->x * a->y * c->z +
			c->x * a->y * b->z -
			a->x * c->y * b->z;
	}

	static inline b3_vector *b3VMul  (b3_matrix *A,b3_vector *Src,b3_vector *Dst,b3_bool Use4D)
	{
#ifdef B3_SSE
		const b3_f32             *m = &A->m11;
		const b3_f32             *s = &Src->x;
		      b3_f32             *d = &Dst->x;
		      b3_f32 B3_ALIGN_16  aux[4];

		for (b3_loop o = 0;o < 3;o++)
		{
			aux[o] = s[o];
		}
		aux[3] = Use4D ? 1 : 0;

		for (b3_loop o = 0; o < 3; o++)
		{
			d[o] = 0;
			for (b3_loop i = 0; i < 4; i++)
			{
				d[o] += m[i] * aux[i];
			}
			m    += 4;
		}
#else
		register b3_f64 x,y,z;

		x = Src->x;
		y = Src->y;
		z = Src->z;
		if (Use4D)
		{
			Dst->x = (b3_f32)(x * A->m11 + y * A->m12 + z * A->m13 + A->m14);
			Dst->y = (b3_f32)(x * A->m21 + y * A->m22 + z * A->m23 + A->m24);
			Dst->z = (b3_f32)(x * A->m31 + y * A->m32 + z * A->m33 + A->m34);
		}
		else
		{
			Dst->x = (b3_f32)(x * A->m11 + y * A->m12 + z * A->m13);
			Dst->y = (b3_f32)(x * A->m21 + y * A->m22 + z * A->m23);
			Dst->z = (b3_f32)(x * A->m31 + y * A->m32 + z * A->m33);
		}
#endif
		return Dst;
	}

	static inline b3_vector64 *b3VMul  (b3_matrix *A,b3_vector64 *Src,b3_vector64 *Dst,b3_bool Use4D)
	{
#ifdef B3_SSE
		const b3_f32             *m = &A->m11;
		const b3_f64             *s = &Src->x;
		      b3_f64             *d = &Dst->x;
		      b3_f64 B3_ALIGN_16  aux[4];

		for (b3_loop o = 0;o < 3;o++)
		{
			aux[o] = s[o];
		}
		aux[3] = Use4D ? 1 : 0;

		for (b3_loop o = 0; o < 3; o++)
		{
			d[o] = 0;
			for (b3_loop i = 0; i < 4; i++)
			{
				d[o] += m[i] * aux[i];
			}
			m    += 4;
		}
#else
		register b3_f64 x,y,z;

		x = Src->x;
		y = Src->y;
		z = Src->z;
		if (Use4D)
		{
			Dst->x = (b3_f64)(x * A->m11 + y * A->m12 + z * A->m13 + A->m14);
			Dst->y = (b3_f64)(x * A->m21 + y * A->m22 + z * A->m23 + A->m24);
			Dst->z = (b3_f64)(x * A->m31 + y * A->m32 + z * A->m33 + A->m34);
		}
		else
		{
			Dst->x = (b3_f64)(x * A->m11 + y * A->m12 + z * A->m13);
			Dst->y = (b3_f64)(x * A->m21 + y * A->m22 + z * A->m23);
			Dst->z = (b3_f64)(x * A->m31 + y * A->m32 + z * A->m33);
		}
#endif
		return Dst;
	}
};

#endif
