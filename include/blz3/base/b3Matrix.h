/*
**
**	$Filename:	b3Matrix.h            
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork
**
**	Blizzard III - vector and matrix calculation proto types
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_MATRIX_H
#define B3_BASE_MATRIX_H

#include "blz3/b3Types.h"
#include <float.h>

#define b3ArcAngleOfScalars(u,v) (atan2((double)v,(double)u)                + ((v) < 0 ? (M_PI * 2.0) : 0))
#define b3RelAngleOfScalars(u,v) (atan2((double)v,(double)u) *   0.5 / M_PI + ((v) < 0 ?   1.0 : 0))
#define b3AngleOfScalars(u,v)    (atan2((double)v,(double)u) * 180.0 / M_PI + ((v) < 0 ? 360.0 : 0))

class b3Vector
{
public:
	static inline b3_vector *b3Init(
		      b3_vector *vec,
		const b3_f64     x = 0,
		const b3_f64     y = 0,
		const b3_f64     z = 0)
	{
		vec->x = (b3_f32)x;
		vec->y = (b3_f32)y;
		vec->z = (b3_f32)z;

		return vec;
	}

	static inline b3_vector64 *b3Init(
		      b3_vector64 *vec,
		const b3_f64       x = 0,
		const b3_f64       y = 0,
		const b3_f64       z = 0)
	{
		vec->x = x;
		vec->y = y;
		vec->z = z;

		return vec;
	}

	static inline b3_bool b3IsEqual(
		const b3_vector *vec1,
		const b3_vector *vec2)
	{
		return
			(vec1->x == vec2->x) &&
			(vec1->y == vec2->y) &&
			(vec1->z == vec2->z);
	}

	static inline b3_vector *b3Negate(b3_vector *negate)
	{
		negate->x = -negate->x;
		negate->y = -negate->y;
		negate->z = -negate->z;
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

	static inline b3_vector *b3Add(const b3_vector *aVec,b3_vector *result)
	{
#ifdef B3_SSE
		b3_f32 *r = &result->x;
		b3_f32 *a = &aVec->x;

		for(int i = 0;i < 3;i++)
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
		b3_f32 *r = &result->x;
		b3_f32 *a = &aVec->x;
		b3_f32 *b = &bVec->x;

		for(int i = 0;i < 3;i++)
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

	static inline b3_vector *b3Sub(const b3_vector *a,b3_vector *result)
	{
		result->x -= a->x;
		result->y -= a->y;
		result->z -= a->z;

		return result;
	}

	static inline b3_vector *b3Sub(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE
		b3_f32 *r = &result->x;
		b3_f32 *a = &aVec->x;
		b3_f32 *b = &bVec->x;

		for(int i = 0;i < 3;i++)
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

	static inline b3_vector *b3CrossProduct(
		const b3_vector *a,
		const b3_vector *b,
		      b3_vector *result)
	{
		result->x = a->y * b->z - a->z * b->y;
		result->y = a->z * b->x - a->x * b->z;
		result->z = a->x * b->y - a->y * b->x;

		return result;
	}

	static inline b3_vector64 *b3CrossProduct(
		const b3_vector *a,
		const b3_vector *b,
		      b3_vector64 *result)
	{
		result->x = a->y * b->z - a->z * b->y;
		result->y = a->z * b->x - a->x * b->z;
		result->z = a->x * b->y - a->y * b->x;

		return result;
	}

	static inline b3_vector64 *b3CrossProduct(
		const b3_vector64 *a,
		const b3_vector64 *b,
		      b3_vector64 *result)
	{
		result->x = a->y * b->z - a->z * b->y;
		result->y = a->z * b->x - a->x * b->z;
		result->z = a->x * b->y - a->y * b->x;

		return result;
	}

	static inline b3_f64 b3Length(const b3_vector *vector)
	{
#ifdef B3_SSE
		b3_f32 *v = &vector->x;
		b3_f64  r = 0;

		for(int i = 0;i < 3;i++)
		{
			r += v[i] * v[i];
		}
		return sqrt(r);
#else
		return sqrt(
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z);
#endif
	}

	static inline b3_f64 b3Length(const b3_vector64 *vector)
	{
		return sqrt(
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z);
	}

	static inline b3_f64 b3QuadLength(const b3_vector *vector)
	{
		return
			vector->x * vector->x +
			vector->y * vector->y +
			vector->z * vector->z;
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
		vector->x = (b3_f32)(vector->x * factor);
		vector->y = (b3_f32)(vector->y * factor);
		vector->z = (b3_f32)(vector->z * factor);
	}

	static inline void b3Scale(b3_vector64 *vector,const b3_f64 factor)
	{
		vector->x *= factor;
		vector->y *= factor;
		vector->z *= factor;
	}

	static inline b3_vector *b3MatrixMul3D(const b3_matrix *A,b3_vector *vector)
	{
		register b3_f64 x,y,z;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		vector->x = (b3_f32)(x * A->m11 + y * A->m12 + z * A->m13);
		vector->y = (b3_f32)(x * A->m21 + y * A->m22 + z * A->m23);
		vector->z = (b3_f32)(x * A->m31 + y * A->m32 + z * A->m33);
		return vector;
	}

	static inline b3_vector *b3MatrixMul4D(const b3_matrix *A,b3_vector *vector)
	{
		register b3_f64 x,y,z;

		x = vector->x;
		y = vector->y;
		z = vector->z;
		vector->x = (b3_f32)(x * A->m11 + y * A->m12 + z * A->m13 + A->m14);
		vector->y = (b3_f32)(x * A->m21 + y * A->m22 + z * A->m23 + A->m24);
		vector->z = (b3_f32)(x * A->m31 + y * A->m32 + z * A->m33 + A->m34);
		return vector;
	}

	static inline b3_f64 b3AngleOfVectors(
		const b3_vector *Vector1,
		const b3_vector *Vector2)
	{
		b3_f64 Denom;

		if ((Denom =
			sqrt(
				Vector1->x * Vector1->x  +
				Vector1->y * Vector1->y  +
				Vector1->z * Vector1->z) *
			sqrt(
				Vector2->x * Vector2->x  +
				Vector2->y * Vector2->y  +
				Vector2->z * Vector2->z)) != 0)
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
		b3_f64    denom,result;

		a.x = point1->x - base->x;
		a.y = point1->y - base->y;
		a.z = point1->z - base->z;

		b.x = point2->x - base->x;
		b.y = point2->y - base->y;
		b.z = point2->z - base->z;

		denom =
			sqrt(a.x * a.x + a.y * a.y + a.z * a.z) *
			sqrt(b.x * b.x + b.y * b.y + b.z * b.z);
		if (denom != 0.0)
		{
			result = (
				a.x * b.x +
				a.y * b.y +
				a.z * b.z) / denom;
		}
		else
		{
			result = 0.0;
		}
		return result;
	}

	static inline b3_vector *b3LinearCombine(
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_f64     b,
		      b3_vector *result)
	{
		result->x = (b3_f32)(aVec->x + b * bVec->x);
		result->y = (b3_f32)(aVec->y + b * bVec->y);
		result->z = (b3_f32)(aVec->z + b * bVec->z);
		return result;
	}

	static inline b3_vector *b3LinearCombine(
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_vector *cVec,
		const b3_f64     b,
		const b3_f64     c,
		      b3_vector *result)
	{
		result->x = (b3_f32)(aVec->x + b * bVec->x + c * cVec->x);
		result->y = (b3_f32)(aVec->y + b * bVec->y + c * cVec->y);
		result->z = (b3_f32)(aVec->z + b * bVec->z + c * cVec->z);
		return result;
	}

	static inline void b3Sort(b3_vector *lower,b3_vector *upper)
	{
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
	}

	static inline void b3Sort(b3_vector64 *lower,b3_vector64 *upper)
	{
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
	}
	
	static inline void b3InitBound(
		b3_vector *lower,
		b3_vector *upper)
	{
		lower->x =
		lower->y =
		lower->z =  FLT_MAX;
		upper->x =
		upper->y =
		upper->z = -FLT_MAX;
	}

	static inline void b3AdjustBound(
		b3_vector *point,
		b3_vector *lower,
		b3_vector *upper)
	{
		// Check lower bound
		if (point->x < lower->x) lower->x = point->x;
		if (point->y < lower->y) lower->y = point->y;
		if (point->z < lower->z) lower->z = point->z;
												  
		// Check upper bound					  
		if (point->x > upper->x) upper->x = point->x;
		if (point->y > upper->y) upper->y = point->y;
		if (point->z > upper->z) upper->z = point->z;
	}
};

b3_bool    b3NormalizeCol       (b3_matrix *Dst,b3_index col_num);
b3_bool    b3NormalizeRow       (b3_matrix *Dst,b3_index row_num);
b3_f64     b3Det2               (b3_f64 a,b3_f64 b,b3_f64 c,b3_f64 d);
b3_f64     b3Det3               (b3_vector *left,b3_vector *mid,b3_vector *right);
b3_f64     b3Det4               (b3_matrix *Matrix);

b3_matrix *b3MatrixUnit         (b3_matrix *Dst);
b3_matrix *b3MatrixInv          (b3_matrix *Src,b3_matrix *Dst);
b3_matrix *b3MatrixTrans        (b3_matrix *Src,b3_matrix *Dst);
b3_matrix *b3MatrixMove         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Move);
b3_matrix *b3MatrixMove         (b3_matrix *Src,b3_matrix *Dst,b3_f64 x,b3_f64 y,b3_f64 z);
b3_matrix *b3MatrixMoveNeg      (b3_matrix *Src,b3_matrix *Dst,b3_vector *MoveNeg);
b3_matrix *b3MatrixScale        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_vector *Scale);
b3_matrix *b3MatrixScale        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 x,b3_f64 y,b3_f64 z);
b3_matrix *b3MatrixRotX         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
b3_matrix *b3MatrixRotY         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
b3_matrix *b3MatrixRotZ         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
b3_matrix *b3MatrixMirrorPoint  (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 scale);
b3_matrix *b3MatrixMirrorAxis   (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,  b3_f64 scale);
b3_matrix *b3MatrixMirrorPlane  (b3_matrix *Src,b3_matrix *Dst,b3_vector *base,b3_vector *uDir,b3_vector *vDir,b3_f64 scale);
b3_matrix *b3MatrixMMul         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);
b3_matrix *b3MatrixSMul         (b3_matrix *Src,b3_matrix *Dst,b3_f64     factor);
b3_matrix *b3MatrixMAdd         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);
b3_matrix *b3MatrixRotVec       (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,b3_f64 angle);
b3_matrix *b3MatrixDress        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_vector *Dir1,b3_vector *Dir2,b3_bool negate);

inline b3_vector *b3MatrixVMul  (b3_matrix *Mat,b3_vector *Src,b3_vector *Dst,b3_bool Use4D)
{
	*Dst = *Src;
	return (Use4D ? b3Vector::b3MatrixMul4D(Mat,Dst) : b3Vector::b3MatrixMul3D(Mat,Dst));
}

#endif
