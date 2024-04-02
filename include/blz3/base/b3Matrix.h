/*
**
**	$Filename:	b3Matrix.h
**	$Release:	Dortmund 2001, 2003, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork
**
**	Blizzard III - vector and matrix calculation proto types
**
**	(C) Copyright 2001, 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_MATRIX_H
#define B3_BASE_MATRIX_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Math.h"

#include <type_traits>
#include <cfloat>
#include <string>

/**
 * This class gives some static convenience methods for vector handing.
 *
 * @see b3_vector
 * @see b3_vector64
 */
class B3_PLUGIN b3Vector
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
	 * This method initializes a b3_vector structure.
	 *
	 * @param vec The b3_vector pointer.
	 * @param x The new x component.
	 * @param y The new y component.
	 * @param z The new z component.
	 * @return  The vec input parameter.
	 */
	template<class V>
	static inline V * b3Init(
		V      *      vec,
		const b3_f64  x = 0,
		const b3_f64  y = 0,
		const b3_f64  z = 0)
	{
		using F = decltype(V::x);

		vec->x = F(x);
		vec->y = F(y);
		vec->z = F(z);

		return vec;
	}

	/**
	 * This method copies a vector.
	 *
	 * @param vec The destination vector.
	 * @param src The source vector.
	 * @return The result (= vec input).
	 */
	template<class S, class D>
	static inline D * b3Init(
		D    *    vec,
		const S * src)
	{
		using Fs = decltype(S::x);
		using Fd = decltype(D::x);

		const Fs * s = &src->x;
		Fd    *    d = &vec->x;

		for (b3_loop i = 0; i < 3; i++)
		{
			d[i] = Fd(s[i]);
		}
		return vec;
	}

	/**
	 * This method initializes a b3_line64 structure.
	 *
	 * @param line The b3_line64 structure to initialize.
	 * @param base The base position.
	 * @param dir  The direction vector.
	 * @return The result (= line input).
	 */
	template<class L, class V>
	static inline L * b3Init(
		L     *     line,
		const V * base,
		const V * dir)
	{
		b3Init(&line->pos, base);
		b3Init(&line->dir, dir);

		return line;
	}

	/**
	 * This method compares two vectors.
	 *
	 * @param vec1 The first vector.
	 * @param vec2 The second vector.
	 * @return True if both vectors are equal.
	 */
	template<class V>
	static inline bool    b3IsEqual(
		const V * vec1,
		const V * vec2)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		const b3_f64 x = vec2->x - vec1->x;
		const b3_f64 y = vec2->y - vec1->y;
		const b3_f64 z = vec2->z - vec1->z;

		return (x * x + y * y + z * z) < b3Math::epsilon;
	}

	/**
	 * This method negates a vector.
	 *
	 * @param negate The vector to negate.
	 * @return The input param negate.
	 */
	template<class V>
	static inline V * b3Negate(V * negate)
	{
		negate->x   = -negate->x;
		negate->y   = -negate->y;
		negate->z   = -negate->z;

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
		b3_vector  *  vector,
		const b3_f64  length = 1.0)
	{
#ifdef BLZ3_USE_SSE41
		const __m128 v     = _mm_load_ps(&vector->x);
		const __m128 denom = _mm_sqrt_ps(_mm_dp_ps(v, v, 0x77));

		_mm_store_ps(&vector->x,
			_mm_mul_ps(v, _mm_div_ps(
					_mm_set1_ps(length), denom)));

		return _mm_cvtss_f32(denom);
#else
		const b3_f64 x        = vector->x;
		const b3_f64 y        = vector->y;
		const b3_f64 z        = vector->z;
		const b3_f64 denom    = sqrt(x * x + y * y + z * z);
		const b3_f64 quotient = length / denom;

		vector->x *= quotient;
		vector->y *= quotient;
		vector->z *= quotient;

		return denom;
#endif
	}

	/**
	 * This method normalizes the given vector to the given length.
	 *
	 * @param vector The vector to normalize.
	 * @param length The needed length of the vector.
	 * @return The old length of the vector.
	 */
	static inline b3_f64 b3Normalize(
		b3_vector64 * vector,
		const b3_f64  length = 1.0)
	{
#ifdef B3_SSE2
		b3_f64 * v      = &vector->x;
		b3_f64   denom  = 0;

		for (b3_loop i = 0; i < 3; i++)
		{
			denom += v[i] * v[i];
		}

		const b3_f64 result    = sqrt(denom);
		const b3_f64 quotient  = length / result;
		for (b3_loop i = 0; i < 3; i++)
		{
			v[i]   *= quotient;
		}
#else
		const b3_f64 x        = vector->x;
		const b3_f64 y        = vector->y;
		const b3_f64 z        = vector->z;
		const b3_f64 result   = sqrt(x * x + y * y + z * z);
		const b3_f64 quotient = length / result;

		vector->x *= quotient;
		vector->y *= quotient;
		vector->z *= quotient;
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
		b3_gl_vector * vector,
		const b3_f32   length = 1.0)
	{
		const b3_f64 x        = vector->x;
		const b3_f64 y        = vector->y;
		const b3_f64 z        = vector->z;
		const b3_f64 result   = sqrt(x * x + y * y + z * z);
		const b3_f64 quotient = length / result;

		vector->x *= quotient;
		vector->y *= quotient;
		vector->z *= quotient;

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
		const b3_vector * aVec,
		const b3_vector * bVec)
	{
#ifdef BLZ3_USE_SSE41
		return _mm_cvtss_f32(
				_mm_dp_ps(
					_mm_load_ps(&aVec->x),
					_mm_load_ps(&bVec->x), 0x71));
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
	template<class L, class R>
	static inline b3_f64 b3SMul(
		const L * aVec,
		const R * bVec)
	{
		return
			aVec->x * bVec->x +
			aVec->y * bVec->y +
			aVec->z * bVec->z;
	}

	/**
	 * This method adds a vector to a given one: <code>result += aVec</code>.
	 *
	 * @param aVec The summand.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	template<class V>
	static inline V * b3Add(
		const V * aVec,
		V    *    result)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		result->x += aVec->x;
		result->y += aVec->y;
		result->z += aVec->z;

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
	template<class V>
	static inline V * b3Add(
		const V * aVec,
		const V * bVec,
		V    *    result)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		result->x = aVec->x + bVec->x;
		result->y = aVec->y + bVec->y;
		result->z = aVec->z + bVec->z;

		return result;
	}

	/**
	 * This method subtracts a vector from a given one: <code>result -= aVec</code>.
	 *
	 * @param aVec The minuend.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	template<class V>
	static inline V * b3Sub(
		const V * aVec,
		V    *    result)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		result->x -= aVec->x;
		result->y -= aVec->y;
		result->z -= aVec->z;

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
	template<class V>
	static inline V * b3Sub(
		const V * aVec,
		const V * bVec,
		V    *    result)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		result->x = aVec->x - bVec->x;
		result->y = aVec->y - bVec->y;
		result->z = aVec->z - bVec->z;

		return result;
	}

	/**
	 * This method multiplies a vector to a given one: <code>result += aVec</code>.
	 *
	 * @param aVec The multiplicand.
	 * @param result The resulting vector.
	 * @return The result.
	 */
	template<class V>
	static inline V * b3Mul(
		const V * aVec,
		V    *    result)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		result->x *= aVec->x;
		result->y *= aVec->y;
		result->z *= aVec->z;

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
	template<class V>
	static inline V * b3Mul(
		const V * aVec,
		const V * bVec,
		V    *    result)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		result->x = aVec->x * bVec->x;
		result->y = aVec->y * bVec->y;
		result->z = aVec->z * bVec->z;

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
	static inline b3_vector * b3CrossProduct(
		const b3_vector * aVec,
		const b3_vector * bVec,
		b3_vector    *    result)
	{
#ifdef BLZ3_USE_SSE
		const int    s1 = _MM_SHUFFLE(3, 0, 2, 1);
		const int    s2 = _MM_SHUFFLE(3, 1, 0, 2);
		const __m128 l  = _mm_load_ps(&aVec->x);
		const __m128 r  = _mm_load_ps(&bVec->x);

		_mm_store_ps(&result->x, _mm_sub_ps(
				_mm_mul_ps(_mm_shuffle_ps(l, l, s1), _mm_shuffle_ps(r, r, s2)),
				_mm_mul_ps(_mm_shuffle_ps(l, l, s2), _mm_shuffle_ps(r, r, s1))));
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
	static inline b3_vector64 * b3CrossProduct(
		const b3_vector * aVec,
		const b3_vector * bVec,
		b3_vector64   *   result)
	{
		result->x = aVec->y * bVec->z - aVec->z * bVec->y;
		result->y = aVec->z * bVec->x - aVec->x * bVec->z;
		result->z = aVec->x * bVec->y - aVec->y * bVec->x;

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
	static inline b3_vector64 * b3CrossProduct(
		const b3_vector64 * aVec,
		const b3_vector64 * bVec,
		b3_vector64    *    result)
	{
#ifdef B3_SSE2
		b3_f64       *       r = &result->x;
		alignas(16) b3_f64   a[4], b[4], c[4], d[4];

		a[Z] = c[Y] = aVec->x;
		b[Y] = d[Z] = bVec->x;
		a[X] = c[Z] = aVec->y;
		b[Z] = d[X] = bVec->y;
		a[Y] = c[X] = aVec->z;
		b[X] = d[Y] = bVec->z;

		for (b3_loop i = 0; i < 3; i++)
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
	static inline b3_gl_vector * b3CrossProduct(
		const b3_gl_vector * aVec,
		const b3_gl_vector * bVec,
		b3_gl_vector    *    result)
	{
		result->x = aVec->y * bVec->z - aVec->z * bVec->y;
		result->y = aVec->z * bVec->x - aVec->x * bVec->z;
		result->z = aVec->x * bVec->y - aVec->y * bVec->x;

		return result;
	}

	/**
	 * The method computes the length of the given vector.
	 *
	 * @param vector The input vector.
	 * @return The resulting vector length.
	 */
	template<class V>
	static inline decltype(V::x) b3Length(const V * vector)
	{
		return sqrt(
				vector->x * vector->x +
				vector->y * vector->y +
				vector->z * vector->z);
	}

	/**
	 * The method computes the squared length of the given vector.
	 *
	 * @param vector The input vector.
	 * @return The resulting squared vector length.
	 */
	template<class V>
	static inline decltype(V::x) b3QuadLength(const V * vector)
	{
		return
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z;
	}

	/**
	 * This method computes the distance between the two given
	 * vectors interpreted as points.
	 *
	 * @param from The first point.
	 * @param to   The second point.
	 * @return The resulting distance.
	 */
	template<typename T> static inline T b3Distance(
		const b3_vector_3D_base<T> * from,
		const b3_vector_3D_base<T> * to)
	{
		const T x = to->x - from->x;
		const T y = to->y - from->y;
		const T z = to->z - from->z;

		return sqrt(x * x + y * y + z * z);
	}

	/**
	 * This method scales a vector by the given factor.
	 *
	 * @param vector The vector to scale.
	 * @param factor The scale factor.
	 */
	template<class V>
	static inline V * b3Scale(
		V           *          vector,
		const decltype(V::x)   factor)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		vector->x *= factor;
		vector->y *= factor;
		vector->z *= factor;

		return vector;
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
	template<class V>
	static inline V * b3Scale(
		const V        *       vector,
		V           *          result,
		const decltype(V::x)   factor)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		result->x = vector->x * factor;
		result->y = vector->y * factor;
		result->z = vector->z * factor;

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
	static inline b3_vector * b3MatrixMul3D(
		const b3_matrix * A,
		b3_vector    *    vector)
	{
		return b3MatrixMul(A, vector, 0.0f);
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
	static inline b3_vector * b3MatrixMul4D(
		const b3_matrix * A,
		b3_vector    *    vector)
	{
		return b3MatrixMul(A, vector, 1.0f);
	}

	/**
	 * This method computes the angle between two vectors.
	 *
	 * @param Vector1 The first vector.
	 * @param Vector2 The second vector.
	 * @return The resulting angle.
	 */
	template<class V>
	static inline decltype(V::x) b3AngleOfVectors(
		const V * Vector1,
		const V * Vector2)
	{
		b3_f64 Denom = b3Length(Vector1) * b3Length(Vector2);

		return (
				Vector1->x * Vector2->x  +
				Vector1->y * Vector2->y  +
				Vector1->z * Vector2->z) / Denom;
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
	template<class V>
	static inline decltype(V::x) b3AngleOfPoints(
		const V * base,
		const V * point1,
		const V * point2)
	{
		V a, b;

		a.x = point1->x - base->x;
		a.y = point1->y - base->y;
		a.z = point1->z - base->z;

		b.x = point2->x - base->x;
		b.y = point2->y - base->y;
		b.z = point2->z - base->z;

		return b3AngleOfVectors(&a, &b);
	}

	/**
	 * This method mixes two vectors by a mixer component. The mixer must in the
	 * range from [0..1].
	 *
	 * @param low The low component if the mixer is 0.
	 * @param high The high component if the mixer is 1.
	 * @param mix The mixer component as single precision floating point number.
	 * @param result The mixer result.
	 * @return The resulting mixed vector.
	 */
	template<class V>
	inline static V * b3Mix(
		const V        *       low,
		const V        *       high,
		const decltype(V::x)   mix,
		V           *          result)
	{
		result->x = low->x + mix * (high->x - low->x);
		result->y = low->y + mix * (high->y - low->y);
		result->z = low->z + mix * (high->z - low->z);

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
	template<class V, class R>
	static inline R * b3LinearCombine(
		const V        *       aVec,
		const V        *       bVec,
		const decltype(V::x)   factor,
		R           *          result)
	{
		using Fv = decltype(V::x);
		using Fr = decltype(R::x);

		static_assert(std::is_floating_point<Fv>(), "Expect float as input vector member type!");
		static_assert(std::is_floating_point<Fr>(), "Expect float as result vector member type!");

		result->x = aVec->x + factor * bVec->x;
		result->y = aVec->y + factor * bVec->y;
		result->z = aVec->z + factor * bVec->z;

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
	template<class V, class R>
	static inline R * b3LinearCombine(
		const V        *       bVec,
		const V        *       cVec,
		const decltype(V::x)   x,
		const decltype(V::y)   y,
		R           *          result)
	{
		using Fv = decltype(V::x);
		using Fr = decltype(R::x);

		static_assert(std::is_floating_point<Fv>(), "Expect float as input vector member type!");
		static_assert(std::is_floating_point<Fr>(), "Expect float as result vector member type!");

		result->x = x * bVec->x + y * cVec->x;
		result->y = x * bVec->y + y * cVec->y;
		result->z = x * bVec->z + y * cVec->z;

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
	template<class V, class R>
	static inline R * b3LinearCombine(
		const V        *       aVec,
		const V        *       bVec,
		const V        *       cVec,
		const decltype(V::x)   x,
		const decltype(V::y)   y,
		R           *          result)
	{
		using Fv = decltype(V::x);
		using Fr = decltype(R::x);

		static_assert(std::is_floating_point<Fv>(), "Expect float as input vector member type!");
		static_assert(std::is_floating_point<Fr>(), "Expect float as result vector member type!");

		result->x = aVec->x + x * bVec->x + y * cVec->x;
		result->y = aVec->y + x * bVec->y + y * cVec->y;
		result->z = aVec->z + x * bVec->z + y * cVec->z;

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
	template<class V, class R>
	static inline R * b3LinearCombine(
		const V        *       aVec,
		const V        *       bVec,
		const V        *       cVec,
		const decltype(V::x)   x,
		const decltype(V::y)   y,
		const decltype(V::z)   z,
		R           *          result)
	{
		using Fv = decltype(V::x);
		using Fr = decltype(R::x);

		static_assert(std::is_floating_point<Fv>(), "Expect float as input vector member type!");
		static_assert(std::is_floating_point<Fr>(), "Expect float as result vector member type!");

		result->x = x * aVec->x + y * bVec->x + z * cVec->x;
		result->y = x * aVec->y + y * bVec->y + z * cVec->y;
		result->z = x * aVec->z + y * bVec->z + z * cVec->z;

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
	static inline b3_vector64 * b3LinearCombine(
		const b3_line64 * line,
		const b3_f64      l,
		b3_vector64   *   result)
	{
		result->x = line->pos.x + l * line->dir.x;
		result->y = line->pos.y + l * line->dir.y;
		result->z = line->pos.z + l * line->dir.z;

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
	static inline b3_vector * b3LinearCombine(
		const b3_line64  * line,
		const b3_f64       l,
		const b3_vector  * base,
		b3_vector     *    result)
	{
		result->x = line->pos.x + l * line->dir.x - base->x;
		result->y = line->pos.y + l * line->dir.y - base->y;
		result->z = line->pos.z + l * line->dir.z - base->z;

		return result;
	}

	/**
	 * This method takes two vectors and sorts each component into
	 * their lower and upper value.
	 *
	 * @param lower The sorted lower vector.
	 * @param upper The sorted upper vector.
	 */
	template<class V>
	static inline void b3Sort(
		V * lower,
		V * upper)
	{
		if (lower->x > upper->x)
		{
			std::swap(lower->x, upper->x);
		}

		if (lower->y > upper->y)
		{
			std::swap(lower->y, upper->y);
		}

		if (lower->z > upper->z)
		{
			std::swap(lower->z, upper->z);
		}
	}

	/**
	 * This method initializes two vectors for bounding box
	 * computation purposes.
	 *
	 * @param lower The lower bound of the bounding box.
	 * @param upper The upper bound of the bounding box.
	 */
	static inline void b3InitBound(
		b3_vector * lower,
		b3_vector * upper)
	{
		lower->x = lower->y = lower->z =  FLT_MAX;
		upper->x = upper->y = upper->z = -FLT_MAX;
	}

	/**
	 * This method clamps each vector component to a given value.
	 *
	 * @param vector The vector to clamp.
	 * @param min The lower level to clamp.
	 */
	static inline b3_vector * b3SetMinimum(
		b3_vector * vector,
		b3_f32      min)
	{
#ifdef BLZ3_USE_SSE
		_mm_store_ps(&vector->x,
			_mm_max_ps(
				_mm_load_ps(&vector->x),
				_mm_set1_ps(min)));
#else
		b3_f32 m = (b3_f32)min;

		if (vector->x < m)
		{
			vector->x = m;
		}
		if (vector->y < m)
		{
			vector->y = m;
		}
		if (vector->z < m)
		{
			vector->z = m;
		}
#endif

		return vector;
	}

	/**
	 * This method saturates each vector component to a given value.
	 *
	 * @param vector The vector to saturate.
	 * @param max The upper level to saturate.
	 */
	static inline b3_vector * b3SetMaximum(
		b3_vector * vector,
		b3_f32      max)
	{
#ifdef BLZ3_USE_SSE
		_mm_store_ps(&vector->x,
			_mm_min_ps(
				_mm_load_ps(&vector->x),
				_mm_set1_ps(max)));
#else
		b3_f32 m = (b3_f32)max;

		if (vector->x > m)
		{
			vector->x = m;
		}
		if (vector->y > m)
		{
			vector->y = m;
		}
		if (vector->z > m)
		{
			vector->z = m;
		}
#endif
		return vector;
	}

	/**
	 * This method adjusts a lower corner with the values of this instance.
	 * It's a min() function for each component.
	 *
	 * @param lower The lower corner to adjust.
	 * @param point The point to compare to.
	 */
	static inline b3_vector * b3CheckLowerBound(
		b3_vector    *    lower,
		const b3_vector * point)
	{
#ifdef BLZ3_USE_SSE
		_mm_store_ps(&lower->x,
			_mm_min_ps(
				_mm_load_ps(&lower->x),
				_mm_load_ps(&point->x)));
#else
		if (point->x < lower->x)
		{
			lower->x = point->x;
		}
		if (point->y < lower->y)
		{
			lower->y = point->y;
		}
		if (point->z < lower->z)
		{
			lower->z = point->z;
		}
#endif
		return lower;
	}

	/**
	 * This method adjusts a upper corner with the values of this instance.
	 * It's a max() function for each component.
	 *
	 * @param upper The lower corner to adjust.
	 * @param point The point to compare to.
	 */
	static inline b3_vector * b3CheckUpperBound(
		b3_vector    *    upper,
		const b3_vector * point)
	{
#ifdef BLZ3_USE_SSE
		_mm_store_ps(&upper->x,
			_mm_max_ps(
				_mm_load_ps(&upper->x),
				_mm_load_ps(&point->x)));
#else
		if (point->x > upper->x)
		{
			upper->x = point->x;
		}
		if (point->y > upper->y)
		{
			upper->y = point->y;
		}
		if (point->z > upper->z)
		{
			upper->z = point->z;
		}
#endif

		return upper;
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
		const b3_vector * point,
		b3_vector    *    lower,
		b3_vector    *    upper)
	{
		b3CheckLowerBound(lower, point);
		b3CheckUpperBound(upper, point);
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
		const b3_gl_vector * point,
		b3_vector      *     lower,
		b3_vector      *     upper)
	{
		b3_vector test;

		b3Init(&test, point->x, point->y, point->z);
		b3CheckLowerBound(lower, &test);
		b3CheckUpperBound(upper, &test);
	}

private:

	static b3_vector * b3MatrixMul(
		const b3_matrix * A,
		b3_vector    *    vector,
		const b3_f32      w);
};

/**
 * This class provides methods for generating different transformations.
 */
class B3_PLUGIN b3Matrix
{
	static const b3_matrix m_UnitMatrix;
	static const b3_vector m_EmptyVector;

	static bool       b3NormalizeCol(b3_matrix * Dst, b3_index col_num);
	static bool       b3NormalizeRow(b3_matrix * Dst, b3_index row_num);
	static b3_f64     b3Det4(const b3_matrix * Matrix);

public:
	/**
	 * This method initializes the given matrix pointer with the unit matrix.
	 *
	 * @param Dst The matrix to initialize with the unit matrix.
	 * @return The pointer to the initialized unit matrix (= the Dst parameter).
	 */
	static b3_matrix * b3Unit(b3_matrix * Dst);

	/**
	 * This method transposes a matrix Src and stores it into the given result
	 * pointer.
	 *
	 * @param Src The matrix to transpose.
	 * @param Dst The transposed matrix.
	 *
	 * @return The transposed matrix equal to Dst.
	 */
	static b3_matrix * b3Transpose(const b3_matrix * Src, b3_matrix * Dst);

	/**
	 * This method computes the inverse transformation from an input matrix.
	 *
	 * @param Src The source matrix.
	 * @param Dst The inverted destination matrix.
	 * @return The result (= Dst).
	 */
	static b3_matrix * b3Inverse(const b3_matrix * Src, b3_matrix * Dst);

	/**
	 * This method copies the content of one matrix to another.
	 *
	 * @param Src The soruce matrix to copy.
	 * @param Dst The destination matrix.
	 * @return The result (= Dst).
	 */
	static b3_matrix * b3Copy(const b3_matrix * Src, b3_matrix * Dst);

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
	static b3_matrix * b3Move(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Move);

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
	static b3_matrix * b3Move(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_f64 x, const b3_f64 y, const  b3_f64 z);

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
	static b3_matrix * b3MoveNegative(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * MoveNeg);

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
	static b3_matrix * b3Scale(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Center,
		const b3_vector * Scale);

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
	static b3_matrix * b3Scale(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Center,
		const b3_f64      x = 1.0,
		const b3_f64      y = 1.0,
		const b3_f64      z = 1.0);

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
	static b3_matrix * b3RotateX(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Center,
		const b3_f64      angle);

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
	static b3_matrix * b3RotateY(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Center,
		const b3_f64      angle);

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
	static b3_matrix * b3RotateZ(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Center,
		const b3_f64      angle);

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
	static b3_matrix * b3RotateVector(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_line  *  axis,
		const b3_f64      angle);

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
	static b3_matrix * b3MirrorPoint(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Center,
		const b3_f64      scale);

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
	static b3_matrix * b3MirrorAxis(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_line  *  axis,
		const b3_f64      scale);

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
	static b3_matrix * b3MirrorPlane(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * center,
		const b3_vector * uDir,
		const b3_vector * vDir,
		const b3_f64      scale);

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
	static b3_matrix * b3Align(b3_matrix * M, const b3_line * axis);

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
	static b3_matrix * b3Dress(
		const b3_matrix * Src, b3_matrix * Dst,
		const b3_vector * Center,
		const b3_vector * Dir1,
		const b3_vector * Dir2,
		const bool        future);

	/**
	 * This method multiplies matrix A and B and stores the result. The result
	 * pointer may be equal to the matrices A or B.
	 *
	 * @param A The first matrix.
	 * @param B The second matrix.
	 * @param Result The resulting matrix.
	 * @return The result matrix pointer.
	 */
	static b3_matrix * b3MMul(
		const b3_matrix * A,
		const b3_matrix * B,
		b3_matrix    *    Result);

	/**
	 * This method adds matrix A and B and stores the result. The result pointer may be equal to
	 * The matrices A or B.
	 *
	 * @param A The first matrix.
	 * @param B The second matrix.
	 * @param Result The resulting matrix.
	 * @return The result matrix pointer.
	 */
	static b3_matrix * b3MAdd(
		const b3_matrix * A,
		const b3_matrix * B,
		b3_matrix    *    Result);

	/**
	 * This method prints the content of the specified matrix with an additional comment.
	 * This is for debugging purposes.
	 *
	 * @param mtx The matrix to dump.
	 * @param title An additional comment.
	 * @return The input matrix.
	 */
	static const b3_matrix * b3Dump(const b3_matrix * mtx, const char * title = nullptr);

	static std::string b3ToString(const b3_matrix * mtx, const char * title);

	/**
	 * This method tests if the given transformation matrix is a unit matrix.
	 *
	 * @param A The matrix to test.
	 * @return True if the given matrix is a unit matrix.
	 * @see b3IsEqual()
	 */
	static inline bool       b3IsUnitMatrix(const b3_matrix * A)
	{
		return b3IsEqual(A, &m_UnitMatrix);
	}

	/**
	 * This method tests if the given matrices are nearly equal.
	 *
	 * @param A The first matrix to compare.
	 * @param B The second matrix to compare.
	 * @return True if the given matrix is a unit matrix.
	 */
	static inline bool b3IsEqual(const b3_matrix * A, const b3_matrix * B)
	{
		const b3_f32 * ptr1 = &A->m11;
		const b3_f32 * ptr2 = &B->m11;

		return std::equal(ptr1, ptr1 + 16, ptr2, ptr2 + 16, [] (const b3_f32 l, const b3_f32 r)
		{
			return fabs(l - r) <= 0.001f;
		});
	}

	/**
	 * This method computes the 1d determinand from one component. Just for fun :-)
	 *
	 * @param a The input component.
	 * @return The resulting 1d determinand.
	 */
	static inline b3_f64 b3Det1(const b3_f64 a)
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
	static inline b3_f64     b3Det2(
		const b3_f64 a,
		const b3_f64 b,
		const b3_f64 c,
		const b3_f64 d)
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
	template<class V>
	static inline b3_f64     b3Det3(
		const V * a,
		const V * b,
		const V * c)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		return
			a->x * b->y * c->z -
			c->x * b->y * a->z +
			b->x * c->y * a->z -
			b->x * a->y * c->z +
			c->x * a->y * b->z -
			a->x * c->y * b->z;
	}

	/**
	 * This method computes a transformation on an input vector and stores the
	 * result into a given result buffer.
	 *
	 * @param A The transformation matrix.
	 * @param Src The vector to transform.
	 * @param Dst The result vector.
	 * @param Use4D If true the input vector is a position. Otherwise it is a
	 * direction.
	 * @return The result.
	 */
	template<class V>
	static inline V * b3VMul(
		const b3_matrix * A,
		const V     *     Src, V * Dst,
		const bool        Use4D)
	{
		using F = decltype(V::x);

		static_assert(std::is_floating_point<F>(), "Expect float as vector member type!");

		alignas(16) const F aux[4]
		{
			Src->x,
			Src->y,
			Src->z,
			Use4D ? 1.0f : 0.0f
		};

		const b3_f32 * m = &A->m11;
		F       *      d = &Dst->x;

		for (b3_loop o = 0; o < 3; o++)
		{
			alignas(16) F prod[4];

			for (b3_loop i = 0; i < 4; i++)
			{
				prod[i] = m[i] * aux[i];
			}

			d[o] = 0;
			for (b3_loop i = 0; i < 4; i++)
			{
				d[o] += prod[i];
			}
			m += 4;
		}

		return Dst;
	}
};

#endif
