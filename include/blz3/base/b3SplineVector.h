/*
**
**	$Filename:	b3SplineVector.h $
**	$Release:	Dortmund 2021 $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III Spline vector proto types
**
**	(C) Copyright 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_BASE_SPLINEVECTOR_H
#define B3_BASE_SPLINEVECTOR_H

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        Simple data structure containing              **
**                        B spline values                               **
**                                                                      **
*************************************************************************/

/**
 * This class implements spline vector operations.
 */
class B3_PLUGIN b3SplineVector
{
public:
	/**
	 * This method initializes the given scalar to 0.
	 */
	static inline void b3Clear(b3_f64 & scalar)
	{
		scalar = 0;
	}

	/**
	 * This method initializes the given vector to 0.
	 */
	static inline void b3Clear(b3_vector & vector)
	{
		vector.x = vector.y = vector.z = 0;
	}

	/**
	 * This method initializes the given vector to 0.
	 */
	static inline void b3Clear(b3_vector4D & vector)
	{
		vector.x = vector.y = vector.z = vector.w = 0;
	}

	/**
	 * This method is the non rational version of weighting a scalar. It
	 * intentionally does nothing.
	 *
	 * @param scale The type to be scaled.
	 * @param weight The weight itself.
	 * @return The "weighted" scalar.
	 * @see b3Curve
	 */
	static inline b3_f64 b3WeightInit(
		const b3_f64 scale, const b3_f64 weight = 1.0)
	{
		(void)weight;

		return scale;
	}

	/**
	 * This method is the non rational version of weighting a vector. It
	 * intentionally does nothing.
	 *
	 * @param scale The type to be scaled.
	 * @param weight The weight itself.
	 * @return The "weighted" vector.
	 * @see b3Spline
	 */
	static inline b3_vector b3WeightInit(
		const b3_vector & scale, const b3_f32 weight = 1.0)
	{
		(void)weight;

		return scale;
	}

	/**
	 * This method is the rational version of weighting a vector. It weights
	 * the x, y, and z components and stores the weight itself into the w
	 * component. This version is ment to be used in conjunctions with
	 * NURBS.
	 *
	 * @param scale The vector to be scaled.
	 * @param weight The weight itself.
	 * @return The weighted vector.
	 * @see b3Nurbs
	 */
	static inline b3_vector4D b3WeightInit(
		const b3_vector4D & scale,
		const b3_f32        weight = 1.0)
	{
		return b3_vector4D
		{
			scale.x * weight,
			scale.y * weight,
			scale.z * weight,
			weight
		};
	}

	/**
	 * This method is the opposite to b3Homogenize(). Since a simple scalar
	 * is not rational this method intentionally does nothing
	 *
	 * @param scale The non rational scalar not to be weighted.
	 * @return The resulting scalar.
	 * @see b3Homogenize()
	 */
	static inline b3_f64 b3WeightSelf(const b3_f64 scale)
	{
		return scale;
	}

	/**
	 * This method is the opposite to b3Homogenize(). Since a 3d vector
	 * is not rational this method intentionally does nothing
	 *
	 * @param scale The non rational 3d vector not to be weighted.
	 * @return The resulting vector.
	 * @see b3Homogenize()
	 */
	static inline b3_vector b3WeightSelf(const b3_vector & scale)
	{
		return scale;
	}

	/**
	 * This method is the opposite to b3Homogenize(). It scales the components
	 * x, y and z by its weight component w.
	 *
	 * @param scale The rational vector to scale.
	 * @return The weighted rational vector.
	 * @see b3Homogenize()
	 */
	static inline b3_vector4D b3WeightSelf(b3_vector4D & scale)
	{
		scale.x *= scale.w;
		scale.y *= scale.w;
		scale.z *= scale.w;

		return scale;
	}

	/**
	 * This method does nothing.
	 */
	static inline void b3Homogenize(b3_f64 & vector B3_UNUSED)
	{
	}

	/**
	 * This method does nothing.
	 */
	static inline void b3Homogenize(b3_vector & vector B3_UNUSED)
	{
	}

	/**
	 * This method homogenizes the given vector.
	 *
	 * @param vector The vector to homogenize.
	 */
	static inline void b3Homogenize(b3_vector4D & vector)
	{
		const b3_f32 w = vector.w;

		B3_ASSERT(w != 0);
		vector.x /= w;
		vector.y /= w;
		vector.z /= w;
	}

	static inline b3_f64 b3Sub(
		const b3_f64 a,
		const b3_f64 b)
	{
		return a - b;
	}

	static inline b3_vector b3Sub(
		const b3_vector & aVec,
		const b3_vector & bVec)
	{
		b3_vector result;

#ifdef B3_SSE
		const b3_f32 * a = &aVec.x;
		const b3_f32 * b = &bVec.x;
		b3_f32    *    r = &result.x;

		for (b3_loop i = 0; i < 3; i++)
		{
			r[i] = a[i] - b[i];
		}
#else
		result.x = aVec.x - bVec.x;
		result.y = aVec.y - bVec.y;
		result.z = aVec.z - bVec.z;
#endif
		return result;
	}

	static inline b3_vector4D b3Sub(
		const b3_vector4D & aVec,
		const b3_vector4D & bVec)
	{
		b3_vector4D result;

#ifdef B3_SSE1
		const b3_f32 * a = &aVec.x;
		const b3_f32 * b = &bVec.x;
		b3_f32    *    r = &result.x;

		for (b3_loop i = 0; i < 4; i++)
		{
			r[i] = a[i] - b[i];
		}
#else
		result.x = aVec.x - bVec.x;
		result.y = aVec.y - bVec.y;
		result.z = aVec.z - bVec.z;
		result.w = aVec.w - bVec.w;
#endif
		return result;
	}

	static inline b3_f64 b3AddScaled(
		const b3_f64 factor,
		const b3_f64 offset,
		b3_f64   &   vector)
	{
		vector += factor * offset;

		return vector;
	}

	static inline b3_vector & b3AddScaled(
		const b3_f64      factor,
		const b3_vector & offset,
		b3_vector    &    vector)
	{
#ifdef B3_SSE
		const b3_f32 * o = &offset.x;
		b3_f32    *    v = &vector.x;

		for (b3_loop i = 0; i < 3; i++)
		{
			v[i] += factor * o[i];
		}
#else
		vector.x += factor * offset.x;
		vector.y += factor * offset.y;
		vector.z += factor * offset.z;
#endif
		return vector;
	}

	static inline b3_vector4D & b3AddScaled(
		const b3_f32        factor,
		const b3_vector4D & offset,
		b3_vector4D    &    vector)
	{
#ifdef B3_SSE1
		const b3_f32 * o = &offset.x;
		b3_f32    *    v = &vector.x;

		for (b3_loop i = 0; i < 4; i++)
		{
			v[i] = v[i] + factor * o[i];
		}
#else
		vector.x += factor * offset.x;
		vector.y += factor * offset.y;
		vector.z += factor * offset.z;
		vector.w += factor * offset.w;
#endif
		return vector;
	}

	static inline b3_f64 b3LinearCombine(
		const b3_f64 a,
		const b3_f64 b,
		const b3_f64 factor)
	{
		return a + factor * b;
	}

	static inline b3_vector b3LinearCombine(
		const b3_vector & aVec,
		const b3_vector & bVec,
		const b3_f64      f)
	{
		b3_vector result;

		result.x = aVec.x + f * bVec.x;
		result.y = aVec.y + f * bVec.y;
		result.z = aVec.z + f * bVec.z;

		return result;
	}

	static inline b3_vector4D b3LinearCombine(
		const b3_vector4D & aVec,
		const b3_vector4D & bVec,
		const b3_f64        f)
	{
		b3_vector4D result;

#ifdef B3_SSE1
		const b3_f32 * a = &aVec.x;
		const b3_f32 * b = &bVec.x;
		b3_f32    *    r = &result.x;

		for (b3_loop i = 0; i < 4; i++)
		{
			r[i] = a[i] + f * b[i];
		}
#else
		result.x = aVec.x + f * bVec.x;
		result.y = aVec.y + f * bVec.y;
		result.z = aVec.z + f * bVec.z;
		result.w = aVec.w + f * bVec.w;
#endif
		return result;
	}

	static inline b3_f64 b3Mix(
		const b3_f64 a,
		const b3_f64 b,
		const b3_f64 f)
	{
		return a + f * (b - a);
	}

	static inline b3_vector b3Mix(
		const b3_vector & aVec,
		const b3_vector & bVec,
		const b3_f64      f)
	{
		b3_vector result;

		result.x = aVec.x + f * (bVec.x - aVec.x);
		result.y = aVec.y + f * (bVec.y - aVec.y);
		result.z = aVec.z + f * (bVec.z - aVec.z);

		return result;
	}

	static inline b3_vector4D b3Mix(
		const b3_vector4D & aVec,
		const b3_vector4D & bVec,
		const b3_f64        f)
	{
		b3_vector4D result;

#ifdef B3_SSE1
		const b3_f32 * a = &aVec.x;
		const b3_f32 * b = &bVec.x;
		b3_f32    *    r = &result.x;

		for (b3_loop i = 0; i < 4; i++)
		{
			r[i] = a[i] + f * (b[i] - a[i]);
		}
#else
		result.x = aVec.x + f * (bVec.x - aVec.x);
		result.y = aVec.y + f * (bVec.y - aVec.y);
		result.z = aVec.z + f * (bVec.z - aVec.z);
		result.w = aVec.w + f * (bVec.w - aVec.w);
#endif
		return result;
	}
};

#endif // B3SPLINEVECTOR_H
