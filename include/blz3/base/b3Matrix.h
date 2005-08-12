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

/**
 * This class gives some static convenience methods for vector handing.
 *
 * @see b3_vector
 * @see b3_vector64
 */
class b3Vector
{
	/**
	 * This enumeration lists the first four indices into the vector components.
	 */
	enum b3_vector_index
	{
		X = 0,      //!< The x component.
		Y,          //!< Thy y component.
		Z,          //!< The z component.
		W,          //!< The w (homogenous) component.
		B3_MAX_DIM  //!< The amount of labeled components.
	};

public:
	/**
	 * This method initializes a ::b3_vector structure.
	 *
	 * @param vec The ::b3_vector pointer.
	 * @param x The new x component.
	 * @param y The new y component.
	 * @param z The new z component.
	 * @return  The vec input parameter.
	 */
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

	/**
	 * This method initializes a ::b3_vector structure.
	 *
	 * @param vec The ::b3_vector pointer.
	 * @param x The new x component.
	 * @param y The new y component.
	 * @param z The new z component.
	 * @return  The vec input parameter.
	 */
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

	/**
	 * This method copies a vector.
	 *
	 * @param vec The destination vector. 
	 * @param src The source vector.
	 * @return The result (= vec input).
	 */
	static inline b3_vector	*b3Init(
		      b3_vector   *vec,
		const b3_vector   *src)
	{
		*vec = *src;
		return vec;
	}

	/**
	 * This method copies a vector.
	 *
	 * @param vec The destination vector. 
	 * @param src The source vector.
	 * @return The result (= vec input).
	 */
	static inline b3_vector *b3Init(
		      b3_vector   *vec,
		const b3_vector64 *src)
	{
		const b3_f64 B3_ALIGN_16 *s = &src->x;
		      b3_f32 B3_ALIGN_16 *d = &vec->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			d[i] = (b3_f32)s[i];
		}
		return vec;
	}

	/**
	 * This method copies a vector.
	 *
	 * @param vec The destination vector. 
	 * @param src The source vector.
	 * @return The result (= vec input).
	 */
	static inline b3_vector64 *b3Init(
		      b3_vector64 *vec,
		const b3_vector   *src)
	{
		const b3_f32 B3_ALIGN_16 *s = &src->x;
		      b3_f64 B3_ALIGN_16 *d = &vec->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			d[i] = s[i];
		}
		return vec;
	}

	/**
	 * This method copies a vector.
	 *
	 * @param vec The destination vector. 
	 * @param src The source vector.
	 * @return The result (= vec input).
	 */
	static inline b3_vector64 *b3Init(
		      b3_vector64 *vec,
		const b3_vector64 *src)
	{
		*vec = *src;

		return vec;
	}

	/**
	 * This method compares two vectors.
	 *
	 * @param vec1 The first vector.
	 * @param vec2 The second vector.
	 * @return True if both vectors are equal.
	 */
	static inline b3_bool b3IsEqual(
		const b3_vector *vec1,
		const b3_vector *vec2)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *v1 = &vec1->x;
		const b3_f32 B3_ALIGN_16 *v2 = &vec2->x;
		      b3_f32 B3_ALIGN_16  d[4];
		      b3_f32              result = 0;

		for (b3_loop i = 0;i < 4;i++)
		{
			d[i]    = v2[i] - v1[i];
		}
		for (b3_loop i = 0;i < 3;i++)
		{
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

	/**
	 * This method negates a vector.
	 *
	 * @param negate The vector to negate.
	 * @return The input param negate.
	 */
	static inline b3_vector *b3Negate(b3_vector *negate)
	{
#ifdef B3_SSE1
		b3_f32 B3_ALIGN_16 *n = &negate->x;
		
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

	/**
	 * This method negates a vector.
	 *
	 * @param negate The vector to negate.
	 * @return The input param negate.
	 */
	static inline b3_vector64 *b3Negate(b3_vector64 *negate)
	{
#ifdef B3_SSE2
		b3_f64 B3_ALIGN_16 *n = &negate->x;
		
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

	/**
	 * This method normalizes the given vector to the given length.
	 *
	 * @param vector The vector to normalize.
	 * @param length The needed length of the vector.
	 * @return The old length of the vector.
	 */
	static inline b3_f32 b3Normalize(
		      b3_vector *vector,
		const b3_f32     length = 1.0)
	{
#ifdef B3_SSE
		b3_f32 B3_ALIGN_16 *v      = &vector->x;
		b3_f32              denom  = 0;
		b3_f32              result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			denom += v[i] * v[i];
		}

		denom = length / (result = sqrt(denom));
		for (b3_loop i = 0;i < 3;i++)
		{
			v[i] *= denom;
		}
#else
		b3_f32 x      = vector->x;
		b3_f32 y      = vector->y;
		b3_f32 z      = vector->z;
		b3_f32 result = 0;
		b3_f32 denom  = length / (result = sqrt(x*x + y*y + z*z));

		vector->x *= denom;
		vector->y *= denom;
		vector->z *= denom;
#endif
		return result;
	}

	/**
	 * This method normalizes the given vector to the given length.
	 *
	 * @param vector The vector to normalize.
	 * @param length The needed length of the vector.
	 * @return The old length of the vector.
	 */
	static inline b3_f64 b3Normalize(
		      b3_vector64 *vector,
		const b3_f64       length = 1.0)
	{
#ifdef B3_SSE2
		b3_f64 B3_ALIGN_16 *v      = &vector->x;
		b3_f64              denom  = 0;
		b3_f64              result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			denom += v[i] * v[i];
		}

		denom = length / (result = sqrt(denom));
		for (b3_loop i = 0;i < 3;i++)
		{
			v[i] *= denom;
		}
#else
		b3_f64 x      = vector->x;
		b3_f64 y      = vector->y;
		b3_f64 z      = vector->z;
		b3_f64 result = 0;
		b3_f64 denom  = length / (result = sqrt(x*x + y*y + z*z));

		vector->x *= denom;
		vector->y *= denom;
		vector->z *= denom;
#endif
		return result;
	}

	/**
	 * This method normalizes the given vector to the given length.
	 *
	 * @param vector The vector to normalize.
	 * @param length The needed length of the vector.
	 * @return The old length of the vector.
	 */
	static inline b3_f32 b3Normalize(
		      b3_gl_vector *vector,
		const b3_f32        length = 1.0)
	{
#ifdef B3_SSE
		b3_f32 B3_ALIGN_16 *v      = &vector->x;
		b3_f32              denom  = 0;
		b3_f32              result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			denom += v[i] * v[i];
		}

		denom = length / (result = sqrt(denom));
		for (b3_loop i = 0;i < 3;i++)
		{
			v[i] *= denom;
		}
#else
		b3_f32 x      = vector->x;
		b3_f32 y      = vector->y;
		b3_f32 z      = vector->z;
		b3_f32 result = 0;
		b3_f32 denom  = length / (result = sqrt(x*x + y*y + z*z));

		vector->x *= denom;
		vector->y *= denom;
		vector->z *= denom;
#endif
		return result;
	}

	/**
	 * This method computes the dot product of two vectors.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @return The resulting dot product.
	 */
	static inline b3_f32 b3SMul(
		const b3_vector *aVec,
		const b3_vector *bVec)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16  p[4];
		      b3_f32              result = 0;

		for (b3_loop i = 0;i < 4;i++)
		{
			p[i] = a[i] * b[i];
		}

		for (b3_loop i = 0;i < 3;i++)
		{
			result += p[i];
		}
		return result;
#else
		return
			aVec->x * bVec->x +
			aVec->y * bVec->y +
			aVec->z * bVec->z;
#endif
	}

	/**
	 * This method computes the dot product of two vectors.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @return The resulting dot product.
	 */
	static inline b3_f64 b3SMul(
		const b3_vector   *aVec,
		const b3_vector64 *bVec)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		      b3_f64              result = 0;

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

	static inline b3_f64 b3SMul(
		const b3_vector64 *aVec,
		const b3_vector *bVec)
	{
		return b3SMul(bVec,aVec);
	}

	/**
	 * This method computes the dot product of two vectors.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @return The resulting dot product.
	 */
	static inline b3_f64 b3SMul(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		      b3_f64              result = 0;

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

	/**
	 * This method adds a vector to a given one: <code>result += aVec</code>.
	 *
	 * @param aVec The summand.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_vector *b3Add(
		const b3_vector *aVec,
		      b3_vector *result)
	{
#ifdef B3_SSE1
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method adds a vector to a given one: <code>result += aVec</code>.
	 *
	 * @param aVec The summand.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_vector64 *b3Add(
		const b3_vector64 *aVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method adds a vector to a given one: <code>result += aVec</code>.
	 *
	 * @param aVec The summand.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_gl_vector *b3Add(
		const b3_gl_vector *aVec,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method adds two vectors to a resulting one: <code>result = aVec + bVec</code>
	 *
	 * @param aVec The first summand.
	 * @param bVec The second summand.
	 * @param result The sum of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_vector *b3Add(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE1
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method adds two vectors to a resulting one: <code>result = aVec + bVec</code>
	 *
	 * @param aVec The first summand.
	 * @param bVec The second summand.
	 * @param result The sum of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_vector64 *b3Add(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method adds two vectors to a resulting one: <code>result = aVec + bVec</code>
	 *
	 * @param aVec The first summand.
	 * @param bVec The second summand.
	 * @param result The sum of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_gl_vector *b3Add(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for(b3_loop i = 0;i < 3; ;i++)
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

	/**
	 * This method subtracts a vector from a given one: <code>result -= aVec</code>.
	 *
	 * @param aVec The minuend.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_vector *b3Sub(
		const b3_vector *aVec,
		      b3_vector *result)
	{
#ifdef B3_SSE1
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method subtracts a vector from a given one: <code>result -= aVec</code>.
	 *
	 * @param aVec The minuend.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_vector64 *b3Sub(
		const b3_vector64 *aVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method subtracts two vectors to a resulting one: <code>result = aVec - bVec</code>
	 *
	 * @param aVec The subtrahend.
	 * @param bVec The minuend.
	 * @param result The difference of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_vector *b3Sub(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE1
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method subtracts two vectors to a resulting one: <code>result = aVec - bVec</code>
	 *
	 * @param aVec The subtrahend.
	 * @param bVec The minuend.
	 * @param result The difference of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_vector64 *b3Sub(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method subtracts two vectors to a resulting one: <code>result = aVec - bVec</code>
	 *
	 * @param aVec The subtrahend.
	 * @param bVec The minuend.
	 * @param result The difference of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_gl_vector *b3Sub(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method multiplies a vector to a given one: <code>result *= aVec</code>.
	 *
	 * @param aVec The factors.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_vector *b3Mul(
		const b3_vector *aVec,
		      b3_vector *result)
	{
#ifdef B3_SSE1
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method multiplies a vector to a given one: <code>result += aVec</code>.
	 *
	 * @param aVec The multiplicand.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_vector64 *b3Mul(
		const b3_vector64 *aVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method multiplies a vector to a given one: <code>result += aVec</code>.
	 *
	 * @param aVec The multiplicand.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	static inline b3_gl_vector *b3Mul(
		const b3_gl_vector *aVec,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method multiplies two vectors to a resulting one: <code>result = aVec + bVec</code>
	 *
	 * @param aVec The first multiplicand.
	 * @param bVec The second multiplicand.
	 * @param result The sum of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_vector *b3Mul(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE1
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method multiplies two vectors to a resulting one: <code>result = aVec + bVec</code>
	 *
	 * @param aVec The first multiplicand.
	 * @param bVec The second multiplicand.
	 * @param result The sum of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_vector64 *b3Mul(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method multiplies two vectors to a resulting one: <code>result = aVec + bVec</code>
	 *
	 * @param aVec The first multiplicand.
	 * @param bVec The second multiplicand.
	 * @param result The sum of both vectors.
	 * @return The result (= result input parameter)
	 */
	static inline b3_gl_vector *b3Mul(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for(b3_loop i = 0;i < 3; ;i++)
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

 	/**
	 * This method computes the cross product from two vectors: <code>result = aVec x bVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param result The result.
	 * @return The result.
	 */
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
		a[W] = c[W] =          b[W] = d[W] = 0;

		for (b3_loop i = 0;i < 4;i++)
		{
			a[i] *= b[i];
		}
		for (b3_loop i = 0;i < 4;i++)
		{
			c[i] *= d[i];
		}
		for (b3_loop i = 0;i < 4;i++)
		{
			r[i]  = a[i] + c[i];
		}
#else
		result->x = aVec->y * bVec->z - aVec->z * bVec->y;
		result->y = aVec->z * bVec->x - aVec->x * bVec->z;
		result->z = aVec->x * bVec->y - aVec->y * bVec->x;
#endif
		return result;
	}

	/**
	 * This method computes the cross product from two vectors: <code>result = aVec x bVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param result The result.
	 * @return The result.
	 */
	static inline b3_vector64 *b3CrossProduct(
		const b3_vector   *aVec,
		const b3_vector   *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE
		b3_f64 B3_ALIGN_16 *r = &result->x;
		b3_f32 B3_ALIGN_16  a[4],b[4],c[4],d[4];

		a[Z] = c[Y] = aVec->x; b[Y] = d[Z] = bVec->x;
		a[X] = c[Z] = aVec->y; b[Z] = d[X] = bVec->y;
		a[Y] = c[X] = aVec->z; b[X] = d[Y] = bVec->z;
		a[W] = c[W] =          b[W] = d[W] = 0;

		for (b3_loop i = 0;i < 4;i++)
		{
			a[i] *= b[i];
		}
		for (b3_loop i = 0;i < 4;i++)
		{
			c[i] *= d[i];
		}
		for (b3_loop i = 0;i < 4;i++)
		{
			r[i]  = a[i] + c[i];
		}
#else
		result->x = aVec->y * bVec->z - aVec->z * bVec->y;
		result->y = aVec->z * bVec->x - aVec->x * bVec->z;
		result->z = aVec->x * bVec->y - aVec->y * bVec->x;
#endif
		return result;
	}

	/**
	 * This method computes the cross product from two vectors: <code>result = aVec x bVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param result The result.
	 * @return The result.
	 */
	static inline b3_vector64 *b3CrossProduct(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
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

	/**
	 * This method computes the cross product from two vectors: <code>result = aVec x bVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param result The result.
	 * @return The result.
	 */
	static inline b3_gl_vector *b3CrossProduct(
		const b3_gl_vector *aVec,
		const b3_gl_vector *bVec,
		      b3_gl_vector *result)
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

	/**
	 * The method computes the length of the given vector.
	 *
	 * @param vector The input vector.
	 * @return The resulting vector length.
	 */
	static inline b3_f32 b3Length(const b3_vector *vector)
	{
#ifdef B3_SSE
		return sqrt(b3QuadLength(vector));
#else
		return sqrt(
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z);
#endif
	}

	/**
	 * The method computes the length of the given vector.
	 *
	 * @param vector The input vector.
	 * @return The resulting vector length.
	 */
	static inline b3_f64 b3Length(const b3_vector64 *vector)
	{
#ifdef B3_SSE
		return sqrt(b3QuadLength(vector));
#else
		return sqrt(
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z);
#endif
	}

	/**
	 * The method computes the squared length of the given vector.
	 *
	 * @param vector The input vector.
	 * @return The resulting squared vector length.
	 */
	static inline b3_f32 b3QuadLength(const b3_vector *vector)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *v = &vector->x;
		      b3_f32 B3_ALIGN_16  prod[4];
		      b3_f32              result = 0;

		for(b3_loop i = 0;i < 4;i++)
		{
			prod[i] = v[i] * v[i];
		}

		for(b3_loop i = 0;i < 3;i++)
		{
			result += prod[i];
		}
		return result;
#else
		return
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z;
#endif
	}

	/**
	 * The method computes the squared length of the given vector.
	 *
	 * @param vector The input vector.
	 * @return The resulting squared vector length.
	 */
	static inline b3_f64 b3QuadLength(const b3_vector64 *vector)
	{
#ifdef B3_SSE
		const b3_f64 B3_ALIGN_16 *v = &vector->x;
		      b3_f64              result = 0;

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

	/**
	 * This method computes the distance between the two given
	 * vectors interpreted as points.
	 *
	 * @param from The first point.
	 * @param to   The second point.
	 * @return The resulting distance.
	 */
	static inline b3_f32 b3Distance(
		const b3_vector *from,
		const b3_vector *to)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &from->x;
		const b3_f32 B3_ALIGN_16 *b = &to->x;
		      b3_f32 B3_ALIGN_16  d[4];
		      b3_f32 B3_ALIGN_16  p[4];
		      b3_f32              result = 0;

		for (b3_loop i = 0;i < 4;i++)
		{
			d[i] = b[i] - a[i];
			p[i] = d[i] * d[i];
		}

		for (b3_loop i = 0;i < 3;i++)
		{
			result += p[i];
		}
		return sqrt(result);
#else
		register b3_f64 x,y,z;

		x = to->x - from->x;
		y = to->y - from->y;
		z = to->z - from->z;
		return sqrt(x * x + y * y + z * z);
#endif
	}

	/**
	 * This method scales a vector by the given factor.
	 *
	 * @param vector The vector to scale.
	 * @param factor The scale factor.
	 */
	static inline void b3Scale(
		      b3_vector *vector,
		const b3_f32 factor)
	{
#ifdef B3_SSE1
		b3_f32 B3_ALIGN_16 *v = &vector->x;

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

	/**
	 * This method scales a given vector with a given factor and
	 * stores the result in a different location.
	 *
	 * @param result The result vector.
	 * @param vector The source vector to scale.
	 * @param factor The scale factor.
	 * @return The result.
	 */
	static inline b3_vector *b3Scale(
		      b3_vector *result,
		const b3_vector *vector,
		const b3_f32     factor)
	{
#ifdef B3_SSE1
		const b3_f32 B3_ALIGN_16 *v = &vector->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method scales a vector by the given factor.
	 *
	 * @param vector The vector to scale.
	 * @param factor The scale factor.
	 */
	static inline void b3Scale(
		      b3_vector64 *vector,
		const b3_f64       factor)
	{
#ifdef B3_SSE2
		b3_f64 B3_ALIGN_16 *v = &vector->x;

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

	/**
	 * This method scales a given vector with a given factor and
	 * stores the result in a different location.
	 *
	 * @param result The result vector.
	 * @param vector The source vector to scale.
	 * @param factor The scale factor.
	 * @return The result.
	 */
	static inline b3_vector64 *b3Scale(
		      b3_vector64 *result,
		const b3_vector64 *vector,
		const b3_f64       factor)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *v = &vector->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

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

	/**
	 * This method computes transforms a vector with the givven
	 * transformation matrix. This operation is meant for direction vectors
	 * because directions cannot be moved.
	 *
	 * @param A The transformation matrix.
	 * @param vector The direction vector to transform.
	 * @return The vector as result.
	 */
	static inline b3_vector *b3MatrixMul3D(const b3_matrix *A,b3_vector *vector)
	{
#ifdef B3_SSE2
		const b3_f32 B3_ALIGN_16 *m = &A->m11;
		      b3_f32 B3_ALIGN_16 *v = &vector->x;
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
		register b3_f32 x,y,z;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		vector->x = (b3_f32)(x * A->m11 + y * A->m12 + z * A->m13);
		vector->y = (b3_f32)(x * A->m21 + y * A->m22 + z * A->m23);
		vector->z = (b3_f32)(x * A->m31 + y * A->m32 + z * A->m33);
#endif
		return vector;
	}

	/**
	 * This method computes transforms a vector with the givven
	 * transformation matrix. This operation is meant for position vectors
	 * because positions can be moved.
	 *
	 * @param A The transformation matrix.
	 * @param vector The position vector to transform.
	 * @return The vector as result.
	 */
	static inline b3_vector *b3MatrixMul4D(const b3_matrix *A,b3_vector *vector)
	{
#ifdef B3_SSE2
		const b3_f32 B3_ALIGN_16 *m = &A->m11;
		      b3_f32 B3_ALIGN_16 *v = &vector->x;
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

	/**
	 * This method computes the angle between two vectors.
	 * 
	 * @param Vector1 The first vector.
	 * @param Vector2 The second vector.
	 * @return The resulting angle.
	 */
	static inline b3_f32 b3AngleOfVectors(
		const b3_vector *Vector1,
		const b3_vector *Vector2)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *v1 = &Vector1->x;
		const b3_f32 B3_ALIGN_16 *v2 = &Vector2->x;
		      b3_f64              Denom = b3Length(Vector1) * b3Length(Vector2);
		      b3_f32              result = 0;

		for (b3_loop i = 0;i < 3;i++)
		{
			result += v1[i] * v2[i];
		}
		return result / Denom;
#else
		b3_f64 Denom = b3Length(Vector1) * b3Length(Vector2);

		return (
			Vector1->x * Vector2->x  +
			Vector1->y * Vector2->y  +
			Vector1->z * Vector2->z) / Denom;
#endif
	}

	/**
	 * This method computes the angle from three points at the
	 * base position.
	 *
	 * @param base The base position where the angle should be computed.
	 * @param point1 The first point which defines the first direction.
	 * @param point2 The second point which defines the second direction.
	 * @return The resulting angle.
	 */
	static inline b3_f32 b3AngleOfPoints(
		const b3_vector *base,
		const b3_vector *point1,
		const b3_vector *point2)
	{
#ifdef B3_SSE
#if 0
		const b3_f32 B3_ALIGN_16 *b  = &base->x;
		const b3_f32 B3_ALIGN_16 *p1 = &point1->x;
		const b3_f32 B3_ALIGN_16 *p2 = &point2->x;
		      b3_f32 B3_ALIGN_16 aDir[4];
		      b3_f32 B3_ALIGN_16 bDir[4];
		      b3_f32             len1 = 0;
		      b3_f32             len2 = 0;
		      b3_f64             denom;
		      b3_f32             result;

		for (b3_loop i = 0;i < 3;i++)
		{
			aDir[i] = p1[i] - b[i];
			bDir[i] = p2[i] - b[i];

			len1   += aDir[i] * aDir[i];
			len2   += bDir[i] * bDir[i];
		}

		denom = len1 * len2;

		for (b3_loop i = 0;i < 3;i++)
		{
			result += aDir[i] * bDir[i];
		}
		return result / denom;
#else
		const b3_f32 B3_ALIGN_16 *b  = &base->x;
		const b3_f32 B3_ALIGN_16 *p1 = &point1->x;
		const b3_f32 B3_ALIGN_16 *p2 = &point2->x;
		      b3_f32 B3_ALIGN_16  aDir[4];
		      b3_f32 B3_ALIGN_16  bDir[4];
		
		for (b3_loop i = 0;i < 3;i++)
		{
			aDir[i] = p1[i] - b[i];
			bDir[i] = p2[i] - b[i];
		}
		return b3AngleOfVectors((const b3_vector *)aDir, (const b3_vector *)bDir);
#endif
#else
		b3_vector a,b;

		a.x = point1->x - base->x;
		a.y = point1->y - base->y;
		a.z = point1->z - base->z;

		b.x = point2->x - base->x;
		b.y = point2->y - base->y;
		b.z = point2->z - base->z;

		return b3AngleOfVectors(&a,&b);
#endif
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = aVec + factor * bVec</code>.
	 *
	 * @param aVec The first vector of order 0.
	 * @param bVec The second vector of order 1.
	 * @param factor The factor for the bVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector *b3LinearCombine(
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_f32     factor,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + factor * b[i];
		}
#else
		result->x = aVec->x + factor * bVec->x;
		result->y = aVec->y + factor * bVec->y;
		result->z = aVec->z + factor * bVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = aVec + factor * bVec</code>.
	 *
	 * @param aVec The first vector of order 0.
	 * @param bVec The second vector of order 1.
	 * @param factor The factor for the bVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector64 *b3LinearCombine(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		const b3_f64       factor,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + factor * b[i];
		}
#else
		result->x = aVec->x + factor * bVec->x;
		result->y = aVec->y + factor * bVec->y;
		result->z = aVec->z + factor * bVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = aVec + factor * bVec</code>.
	 *
	 * @param aVec The first vector of order 0.
	 * @param bVec The second vector of order 1.
	 * @param factor The factor for the bVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_gl_vector *b3LinearCombine(
		const b3_vector    *aVec,
		const b3_vector    *bVec,
		const b3_f32        factor,
		      b3_gl_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + factor * b[i];
		}
#else
		result->x = aVec->x + factor * bVec->x;
		result->y = aVec->y + factor * bVec->y;
		result->z = aVec->z + factor * bVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = x * bVec + y * cVec</code>.
	 *
	 * @param bVec The first vector.
	 * @param cVec The second vector.
	 * @param x The factor for the bVec parameter.
	 * @param y The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector *b3LinearCombine(
		const b3_vector *bVec,
		const b3_vector *cVec,
		const b3_f32     x,
		const b3_f32     y,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		const b3_f32 B3_ALIGN_16 *c = &cVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = x * b[i] + y * c[i];
		}
#else
		result->x = x * bVec->x + y * cVec->x;
		result->y = x * bVec->y + y * cVec->y;
		result->z = x * bVec->z + y * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = x * bVec + y * cVec</code>.
	 *
	 * @param bVec The first vector.
	 * @param cVec The second vector.
	 * @param x The factor for the bVec parameter.
	 * @param y The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector64 *b3LinearCombine(
		const b3_vector   *bVec,
		const b3_vector   *cVec,
		const b3_f32       x,
		const b3_f32       y,
		      b3_vector64 *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		const b3_f32 B3_ALIGN_16 *c = &cVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = x * b[i] + y * c[i];
		}
#else
		result->x = x * bVec->x + y * cVec->x;
		result->y = x * bVec->y + y * cVec->y;
		result->z = x * bVec->z + y * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = x * bVec + y * cVec</code>.
	 *
	 * @param bVec The first vector.
	 * @param cVec The second vector.
	 * @param x The factor for the bVec parameter.
	 * @param y The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector64 *b3LinearCombine(
		const b3_vector64 *bVec,
		const b3_vector64 *cVec,
		const b3_f64       x,
		const b3_f64       y,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		const b3_f64 B3_ALIGN_16 *c = &cVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = x * b[i] + y * c[i];
		}
#else
		result->x = x * bVec->x + y * cVec->x;
		result->y = x * bVec->y + y * cVec->y;
		result->z = x * bVec->z + y * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = aVec + x * bVec + y * cVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param cVec The third vector.
	 * @param x The factor for the bVec parameter.
	 * @param y The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector *b3LinearCombine(
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_vector *cVec,
		const b3_f32     x,
		const b3_f32     y,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		const b3_f32 B3_ALIGN_16 *c = &cVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + x * b[i] + y * c[i];
		}
#else
		result->x = aVec->x + x * bVec->x + y * cVec->x;
		result->y = aVec->y + x * bVec->y + y * cVec->y;
		result->z = aVec->z + x * bVec->z + y * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = aVec + x * bVec + y * cVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param cVec The third vector.
	 * @param x The factor for the bVec parameter.
	 * @param y The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector64 *b3LinearCombine(
		const b3_vector64 *aVec,
		const b3_vector64 *bVec,
		const b3_vector64 *cVec,
		const b3_f64       x,
		const b3_f64       y,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *a = &aVec->x;
		const b3_f64 B3_ALIGN_16 *b = &bVec->x;
		const b3_f64 B3_ALIGN_16 *c = &cVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + x * b[i] + y * c[i];
		}
#else
		result->x = aVec->x + x * bVec->x + y * cVec->x;
		result->y = aVec->y + x * bVec->y + y * cVec->y;
		result->z = aVec->z + x * bVec->z + y * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = aVec + x * bVec + y * cVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param cVec The third vector.
	 * @param x The factor for the bVec parameter.
	 * @param y The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector64 *b3LinearCombine(
		const b3_vector   *aVec,
		const b3_vector   *bVec,
		const b3_vector   *cVec,
		const b3_f32       x,
		const b3_f32       y,
		      b3_vector64 *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		const b3_f32 B3_ALIGN_16 *c = &cVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] + x * b[i] + y * c[i];
		}
#else
		result->x = aVec->x + x * bVec->x + y * cVec->x;
		result->y = aVec->y + x * bVec->y + y * cVec->y;
		result->z = aVec->z + x * bVec->z + y * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = aVec + x * bVec + y * cVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param cVec The third vector.
	 * @param x The factor for the bVec parameter.
	 * @param y The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_gl_vector *b3LinearCombine(
		const b3_vector    *aVec,
		const b3_vector    *bVec,
		const b3_vector    *cVec,
		const b3_f32        x,
		const b3_f32        y,
		      b3_gl_vector *result)
	{
		return (b3_gl_vector *)b3LinearCombine(aVec,bVec,cVec,x,y,(b3_vector *)result);
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = x * aVec + y * bVec + z * cVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param cVec The third vector.
	 * @param x The factor for the aVec parameter.
	 * @param y The factor for the bVec parameter.
	 * @param z The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector64 *b3LinearCombine(
		const b3_vector   *aVec,
		const b3_vector   *bVec,
		const b3_vector   *cVec,
		const b3_f32       x,
		const b3_f32       y,
		const b3_f32       z,
		      b3_vector64 *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		const b3_f32 B3_ALIGN_16 *c = &cVec->x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = x * a[i] + y * b[i] + z * c[i];
		}
#else
		result->x = x * aVec->x + y * bVec->x + z * cVec->x;
		result->y = x * aVec->y + y * bVec->y + z * cVec->y;
		result->z = x * aVec->z + y * bVec->z + z * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = x * aVec + y * bVec + z * cVec</code>.
	 *
	 * @param aVec The first vector.
	 * @param bVec The second vector.
	 * @param cVec The third vector.
	 * @param x The factor for the aVec parameter.
	 * @param y The factor for the bVec parameter.
	 * @param z The factor for the cVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector *b3LinearCombine(
		const b3_vector   *aVec,
		const b3_vector   *bVec,
		const b3_vector   *cVec,
		const b3_f32       x,
		const b3_f32       y,
		const b3_f32       z,
		      b3_vector   *result)
	{
#ifdef B3_SSE
		const b3_f32 B3_ALIGN_16 *a = &aVec->x;
		const b3_f32 B3_ALIGN_16 *b = &bVec->x;
		const b3_f32 B3_ALIGN_16 *c = &cVec->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = x * a[i] + y * b[i] + z * c[i];
		}
#else
		result->x = x * aVec->x + y * bVec->x + z * cVec->x;
		result->y = x * aVec->y + y * bVec->y + z * cVec->y;
		result->z = x * aVec->z + y * bVec->z + z * cVec->z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = line->pos + l * line->dir</code>.
	 *
	 * @param line The ray containing the position and direction vector.
	 * @param l The factor for the bVec parameter.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector64 *b3LinearCombine(
		const b3_line64   *line,
		const b3_f64       l,
		      b3_vector64 *result)
	{
#ifdef B3_SSE2
		const b3_f64 B3_ALIGN_16 *p = &line->pos.x;
		const b3_f64 B3_ALIGN_16 *d = &line->dir.x;
		      b3_f64 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = p[i] + l * d[i];
		}
#else
		result->x = line->pos.x + l * line->dir.x;
		result->y = line->pos.y + l * line->dir.y;
		result->z = line->pos.z + l * line->dir.z;
#endif
		return result;
	}

	/**
	 * This method computes the linear combination of the given input parameters:
	 * <code>result = line->pos + l * line->dir - base</code>.
	 *
	 * @param line The ray containing the position and direction vector.
	 * @param l The factor for the bVec parameter.
	 * @param base A base position to subtract.
	 * @param result The resulting linear combination.
	 * @return The result.
	 */
	static inline b3_vector *b3LinearCombine(
		const b3_line64   *line,
		const b3_f64       l,
		const b3_vector   *base,
		      b3_vector   *result)
	{
#ifdef B3_SSE
		const b3_f64 B3_ALIGN_16 *p = &line->pos.x;
		const b3_f64 B3_ALIGN_16 *d = &line->dir.x;
		const b3_f32 B3_ALIGN_16 *b = &base->x;
		      b3_f32 B3_ALIGN_16 *r = &result->x;

		for (b3_loop i = 0;i < 3;i++)
		{
			r[i] = p[i] + l * d[i] - b[i];
		}
#else
		result->x = line->pos.x + l * line->dir.x - base->x;
		result->y = line->pos.y + l * line->dir.y - base->y;
		result->z = line->pos.z + l * line->dir.z - base->z;
#endif
		return result;
	}

	/**
	 * This method takes two vectors and sorts each component into
	 * their lower and upper value.
	 *
	 * @param lower The sorted lower vector.
	 * @param upper The sorted upper vector.
	 */
	static inline void b3Sort(
		b3_vector *lower,
		b3_vector *upper)
	{
#ifdef B3_SSE
		b3_f32 B3_ALIGN_16 *l = &lower->x;
		b3_f32 B3_ALIGN_16 *u = &upper->x;
		b3_f32              aux;

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

	/**
	 * This method takes two vectors and sorts each component into
	 * their lower and upper value.
	 *
	 * @param lower The sorted lower vector.
	 * @param upper The sorted upper vector.
	 */
	static inline void b3Sort(b3_vector64 *lower,b3_vector64 *upper)
	{
#ifdef B3_SSE2
		b3_f64 B3_ALIGN_16 *l = &lower->x;
		b3_f64 B3_ALIGN_16 *u = &upper->x;
		b3_f64              aux;

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

	/**
	 * This method initializes two vectors for bounding box
	 * computation purposes.
	 *
	 * @param lower The lower bound of the bounding box.
	 * @param upper The upper bound of the bounding box.
	 */
	static inline void b3InitBound(
		b3_vector *lower,
		b3_vector *upper)
	{
#ifdef B3_SSE
		b3_f32 B3_ALIGN_16 *l = &lower->x;
		b3_f32 B3_ALIGN_16 *u = &upper->x;

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

	/**
	 * This method clamps each vector component to a given value.
	 *
	 * @param vector The vector to clamp.
	 * @param min The lower level to clamp.
	 */
	static inline void b3SetMinimum(
		b3_vector *vector,
		b3_f32     min)
	{
#ifdef B3_SSE
		b3_f32 B3_ALIGN_16 *v = &vector->x;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (v[i] < min) v[i] = min;
		}
#else
		b3_f32 m = (b3_f32)min;

		if (vector->x < m) vector->x = m;
		if (vector->y < m) vector->y = m;
		if (vector->z < m) vector->z = m;
#endif
	}

	/**
	 * This method adjusts a lower corner with the values of this instance.
	 * It's a min() function for each component.
	 *
	 * @param lower The lower corner to adjust.
	 * @param point The point to compare to.
	 */
	static inline void b3CheckLowerBound(
		      b3_vector *lower,
		const b3_vector *point)
	{
#ifdef B3_SSE
		      b3_f32 B3_ALIGN_16 *l = &lower->x;
		const b3_f32 B3_ALIGN_16 *p = &point->x;

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

	/**
	 * This method saturates each vector component to a given value.
	 *
	 * @param vector The vector to saturate.
	 * @param max The upper level to saturate.
	 */
	static inline void b3SetMaximum(
		b3_vector *vector,
		b3_f32     max)
	{
#ifdef B3_SSE
		b3_f32 B3_ALIGN_16 *v = &vector->x;

		for (b3_loop i = 0;i < 3; i++)
		{
			if (v[i] > max) v[i] = max;
		}
#else
		b3_f32 m = (b3_f32)max;

		if (vector->x > m) vector->x = m;
		if (vector->y > m) vector->y = m;
		if (vector->z > m) vector->z = m;
#endif
	}

	/**
	 * This method adjusts a upper corner with the values of this instance.
	 * It's a max() function for each component.
	 *
	 * @param upper The lower corner to adjust.
	 * @param point The point to compare to.
	 */
	static inline void b3CheckUpperBound(
		      b3_vector *upper,
		const b3_vector *point)
	{
#ifdef B3_SSE
		      b3_f32 B3_ALIGN_16 *u = &upper->x;
		const b3_f32 B3_ALIGN_16 *p = &point->x;

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

	/**
	 * This method adjusts a bounding box specified by a lower and an
	 * upper corner with a point.
	 *
	 * @param point The point to compare to.
	 * @param lower The lower corner of the bounding box to adjust.
	 * @param upper The upper corner of the bounding box to adjust.
	 */
	static inline void b3AdjustBound(
		const b3_vector *point,
		      b3_vector *lower,
		      b3_vector *upper)
	{
		b3CheckLowerBound(lower,point);
		b3CheckUpperBound(upper,point);
	}

	/**
	 * This method adjusts a bounding box specified by a lower and an
	 * upper corner with a point.
	 *
	 * @param point The point to compare to.
	 * @param lower The lower corner of the bounding box to adjust.
	 * @param upper The upper corner of the bounding box to adjust.
	 */
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

/**
 * This class provides methods for generating different transformations.
 */
class b3Matrix
{
	static b3_matrix m_UnitMatrix;
	static b3_vector m_EmptyVector;

	static b3_bool    b3NormalizeCol (b3_matrix *Dst,b3_index col_num);
	static b3_bool    b3NormalizeRow (b3_matrix *Dst,b3_index row_num);
	static b3_f64     b3Det4         (b3_matrix *Matrix);

public:
	/**
	 * This method initializes the given matrix pointer with the unit matrix.
	 *
	 * @param Dst The matrix to initialize with the unit matrix.
	 * @return The pointer to the initialized unit matrix (= the Dst parameter).
	 */
	static b3_matrix *b3Unit         (b3_matrix *Dst);

	/**
	 * This method computes the inverse transformation from an input matrix.
	 *
	 * @param Src The source matrix.
	 * @param Dst The inverted destination matrix.
	 * @return The result (= Dst).
	 */
	static b3_matrix *b3Inverse      (b3_matrix *Src,b3_matrix *Dst);

	/**
	 * This method copies the content of one matrix to another.
	 *
	 * @param Src The soruce matrix to copy.
	 * @param Dst The destination matrix.
	 * @return The result (= Dst).
	 */
	static b3_matrix *b3Transport    (b3_matrix *Src,b3_matrix *Dst);

	/**
	 * This method creates a translation transformation, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Move The translation vector to use to create the translation transformation.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3Move         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Move);

	/**
	 * This method creates a translation transformation, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param x The x component of the translation vector to use to create the translation transformation.
	 * @param y The y component of the translation vector to use to create the translation transformation.
	 * @param z The z component of the translation vector to use to create the translation transformation.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3Move         (b3_matrix *Src,b3_matrix *Dst,b3_f64 x,b3_f64 y,b3_f64 z);

	/**
	 * This method creates a translation transformation in negative direction, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. 
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param MoveNeg The translation vector to use to create the translation transformation.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3MoveNegative (b3_matrix *Src,b3_matrix *Dst,b3_vector *MoveNeg);

	/**
	 * This method creates a scale transformation, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the scaling center.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Center The scaling center.
	 * @param Scale The scaling factor.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3Scale        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_vector *Scale);

	/**
	 * This method creates a scale transformation, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the scaling center.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Center The scaling center.
	 * @param x The x component of the scale vector to use to create the translation transformation.
	 * @param y The y component of the scale vector to use to create the translation transformation.
	 * @param z The z component of the scale vector to use to create the translation transformation.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3Scale        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 x = 1.0,b3_f64 y = 1.0,b3_f64 z = 1.0);

	/**
	 * This method creates a rotation transformation around the x axis, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the rotating center. This transformation
	 * uses the thumb rule for the rotation direction.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Center The scaling center.
	 * @param angle The rotation angle.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3RotateX      (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);

	/**
	 * This method creates a rotation transformation around the y axis, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the rotating center. This transformation
	 * uses the thumb rule for the rotation direction.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Center The scaling center.
	 * @param angle The rotation angle.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3RotateY      (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);

	/**
	 * This method creates a rotation transformation around the z axis, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the rotating center. This transformation
	 * uses the thumb rule for the rotation direction.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Center The scaling center.
	 * @param angle The rotation angle.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3RotateZ      (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);

	/**
	 * This method creates a rotation transformation around a given half ray, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the rotating center specified in the half ray. This transformation
	 * uses the thumb rule for the rotation direction. The rotation axis may not be a unit vector.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param axis The rotation center and the rotation axis.
	 * @param angle The rotation angle.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3RotateVector (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,  b3_f64 angle);
	
	/**
	 * This method creates a point mirror transformation around a given center point, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the mirror center.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Center The mirror center.
	 * @param scale The mirror scaling. This value should be 1 or -1 otherwise it is simple scaling.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3MirrorPoint  (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 scale);

	/**
	 * This method creates a axis mirror transformation around a given ray, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the mirror center.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param axis The mirror axis including the mirror axis and the mirror center position.
	 * @param scale The mirror scaling. This value should be 1 or -1 otherwise it is simple scaling.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3MirrorAxis   (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,  b3_f64 scale);

	/**
	 * This method creates a plane mirror transformation around a given plane, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a temporarily translation towards the mirror center.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param center The mirror center.
	 * @param uDir The first direction vector for spanning the mirror plane.
	 * @param vDir The second direction vector for spanning the mirror plane.
	 * @param scale The mirror scaling. This value should be 1 or -1 otherwise it is simple scaling.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3MirrorPlane  (b3_matrix *Src,b3_matrix *Dst,b3_vector *center,b3_vector *uDir,b3_vector *vDir,b3_f64 scale);

	/**
	 * This method creates an alignment transformation to a given half ray. The transformation includes a
	 * translation towards the position specified in the half ray.
	 * The alignment vectors needs to be a unit vector. The new y axis lies in the old xy plane. The new
	 * z axis is the cross product from the new xy plane but if the direction is up or down depends on the
	 * future flag and the previous alignment vector. The new base lies in the axis' position.
	 * 
	 * \param *M The transformation matrix to fill.
	 * \param *axis The alignment axis.
	 * \return The input matrix.
	 */
	static b3_matrix *b3Align        (b3_matrix *M,const b3_line *axis);

	/**
	 * This method creates an alignment transformation to a given half ray, multiplies it with an input matrix and stores the 
	 * resulting transformation matrix into a buffer. The source matrix may be null. Then a unit matrix
	 * will be assumed. The destination must not be null. The source and the destination matrix may be
	 * equal. The transformation includes a translation towards the position specified in the half ray.
	 * The alignment vectors need not to be a unit vector. The new y axis lies in the old xy plane. The new
	 * z axis is the cross product from the new xy plane but if the direction is up or down depends on the
	 * future flag and the previous alignment vector. The new base lies in the axis' position.
	 *
	 * @param Src The source matrix or null.
	 * @param Dst The destination matrix.
	 * @param Center The alignment center.
	 * @param Dir1 The old alignment direction.
	 * @param Dir2 The new alignment direction
	 * @param future controls whether we move forward or backward.
	 * @return The resulting matrix.
	 */
	static b3_matrix *b3Dress        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_vector *Dir1,b3_vector *Dir2,b3_bool future);

	/**
	 * This method multiplies matrix A and B and stores the result. The result pointer may be equal to
	 * The matrices A or B.
	 *
	 * @param A The first matrix.
	 * @param B The second matrix.
	 * @param Result The resulting matrix.
	 * @return The result matrix pointer.
	 */
	static b3_matrix *b3MMul         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);

	/**
	 * This method adds matrix A and B and stores the result. The result pointer may be equal to
	 * The matrices A or B.
	 *
	 * @param A The first matrix.
	 * @param B The second matrix.
	 * @param Result The resulting matrix.
	 * @return The result matrix pointer.
	 */
	static b3_matrix *b3MAdd         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);

	/**
	 * This method prints the content of the specified matrix with an additional comment.
	 * This is for debugging purposes.
	 *
	 * @param mtx The matrix to dump.
	 * @param title An additional comment.
	 * @return The input matrix.
	 */
	static b3_matrix *b3Dump         (b3_matrix *mtx,const char *title=null);

	/**
	 * This method tests if the given transformation matrix is a unit matrix.
	 *
	 * @param A The matrix to test.
	 * @return True if the given matrix is a unit matrix.
	 */
	static inline b3_bool    b3IsUnitMatrix(b3_matrix *A)
	{
		b3_f32 B3_ALIGN_16 *ptr1 = &A->m11;
		b3_f32 B3_ALIGN_16 *ptr2 = &m_UnitMatrix.m11;

		for (int i = 0;i < 16;i++)
		{
			if (fabs(*ptr1++ - *ptr2++) > 0.001f)
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * This method computes the 1d determinand from one component. Just for fun :-)
	 *
	 * @param a The input component.
	 * @return The resulting 1d determinand.
	 */
	static inline b3_f64 b3Det1(b3_f64 a)
	{
		return a;
	}

	/**
	 * This method computes the 2d determinand from the four components
	 * of a 2x2 matrix.
	 *
	 * @param a The upper left component.
	 * @param b The lower left component.
	 * @param c The upper right component.
	 * @param d The lower right component.
	 * @return The resulting determinand.
	 */
	static inline b3_f64     b3Det2  (b3_f64 a,b3_f64 b,b3_f64 c,b3_f64 d)
	{
		return a * d - b * c;
	}

	/**
	 * This method computes the 3d determinand from three input vectors.
	 *
	 * @param a The first column.
	 * @param b The second column.
	 * @param c The third column.
	 * @return The resulting determinand.
	 */
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

	/**
	 * This method computes a transformation on an input vector and stores the result into
	 * a given result buffer.
	 *
	 * @param A The transformation matrix.
	 * @param Src The vector to transform.
	 * @param Dst The result vector.
	 * @param Use4D If true the input vector is a position. Otherwise it is a direction.
	 * @return The result.
	 */
	static inline b3_vector *b3VMul  (
		b3_matrix *A,
		b3_vector *Src,
		b3_vector *Dst,
		b3_bool    Use4D)
	{
#ifdef B3_SSE2
		const b3_f32 B3_ALIGN_16 *m = &A->m11;
		const b3_f32 B3_ALIGN_16 *s = &Src->x;
		      b3_f32 B3_ALIGN_16 *d = &Dst->x;
		      b3_f32 B3_ALIGN_16  aux[4];

		for (b3_loop o = 0;o < 3;o++)
		{
			aux[o] = s[o];
		}
		aux[3] = Use4D ? 1 : 0;

		for (b3_loop o = 0; o < 3; o++)
		{
			b3_f32 B3_ALIGN_16 prod[4];

			for (b3_loop i = 0; i < 4; i++)
			{
				prod[i] = m[i] * aux[i];
			}

			d[o] = 0;
			for (b3_loop i = 0; i < 4; i++)
			{
				d[o] += prod[i];
			}
			m    += 4;
		}
#else
		b3_f32 x,y,z;

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

	/**
	 * This method computes a transformation on an input vector and stores the result into
	 * a given result buffer.
	 *
	 * @param A The transformation matrix.
	 * @param Src The vector to transform.
	 * @param Dst The result vector.
	 * @param Use4D If true the input vector is a position. Otherwise it is a direction.
	 * @return The result.
	 */
	static inline b3_vector64 *b3VMul  (b3_matrix *A,b3_vector64 *Src,b3_vector64 *Dst,b3_bool Use4D)
	{
#ifdef B3_SSE2
		const b3_f32 B3_ALIGN_16 *m = &A->m11;
		const b3_f64 B3_ALIGN_16 *s = &Src->x;
		      b3_f64 B3_ALIGN_16 *d = &Dst->x;
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
		b3_f64 x,y,z;

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
