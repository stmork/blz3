/*
**
**      $Filename:      b3Vector.h $
**      $Release:       Dortmund 2002 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Vector template
**
**      (C) Copyright  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_VECTOR_H
#define B3_BASE_VECTOR_H

#include "blz3/b3Types.h"
#include <float.h>
#include "blz3/system/b3Exception.h"

enum b3_vector_index
{
	X = 0,
	Y,
	Z,
	W,
	B3_MAX_DIM
};

enum b3_vector_error
{
	B3_VECTOR_ERROR = -1,
	B3_VECTOR_OK    =  0,
	B3_VECTOR_OUT_OF_BOUNDS
};

typedef b3Exception<b3_vector_error,'VEC'> b3VectorException;

template<class F,b3_loop dim> class B3_PLUGIN b3VectorTemplate
{
	F B3_ALIGN_16 v[dim];

public:
	inline b3VectorTemplate()
	{
	}

	inline b3VectorTemplate(F value)
	{
		b3Value(value);
	}

	inline b3VectorTemplate(
		b3_f32 x,
		b3_f32 y,
		b3_f32 z,
		b3_f32 w)
	{
		b3Init(x,y,z,w);
	}

	inline b3VectorTemplate(
		b3_f64 x,
		b3_f64 y,
		b3_f64 z,
		b3_f64 w)
	{
		b3Init(x,y,z,w);
	}

	inline b3VectorTemplate(const b3VectorTemplate<b3_f32,B3_MAX_DIM> &src)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] = src.v[i];
	}

	inline b3VectorTemplate(const b3VectorTemplate<b3_f64,B3_MAX_DIM> &src)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] = src.v[i];
	}

	inline b3VectorTemplate(const b3_vector32 &vec)
	{
		b3Init(vec.x,vec.y,vec.z);
	}

	inline b3VectorTemplate(const b3_gl_vector &vec)
	{
		b3Init(vec.x,vec.y,vec.z);
	}

	inline b3VectorTemplate(const b3_vector64 &vec)
	{
		b3Init(vec.x,vec.y,vec.z);
	}

	inline b3VectorTemplate(const b3_vector32_4D &vec)
	{
		b3Init(vec.x,vec.y,vec.z,vec.w);
	}

	inline b3VectorTemplate(const b3_vector64_4D &vec)
	{
		b3Init(vec.x,vec.y,vec.z,vec.w);
	}

	inline void b3Zero()
	{
		b3Value((F)0.0);
	}

	inline void b3Value(b3_f32 value = 0)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] = (F)value;
	}

	inline void b3Value(b3_f64 value = 0)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] = (F)value;
	}

	inline void b3Init(
		b3_f64 x = 0,
		b3_f64 y = 0,
		b3_f64 z = 0,
		b3_f64 w = 0)
	{
		v[X] = (F)x;
		v[Y] = (F)y;
		v[Z] = (F)z;
		v[W] = (F)w;
	}

	inline operator b3_vector32 ()
	{
		b3_vector32 result;

		result.x = (b3_f32)v[X];
		result.y = (b3_f32)v[Y];
		result.z = (b3_f32)v[Z];
		result.pad = 0;
		return result;
	}

	inline operator b3_gl_vector ()
	{
		b3_gl_vector result;

		result.x = (b3_f32)v[X];
		result.y = (b3_f32)v[Y];
		result.z = (b3_f32)v[Z];
		return result;
	}

	inline operator b3_vector64 ()
	{
		b3_vector64 result;

		result.x = (b3_f64)v[X];
		result.y = (b3_f64)v[Y];
		result.z = (b3_f64)v[Z];
		result.pad = 0;
		return result;
	}

	inline F operator [](const b3_vector_index index)
	{
#ifdef _DEBUG
		if ((index < 0) || (index >= dim))
		{
			B3_THROW(b3VectorException,B3_VECTOR_OUT_OF_BOUNDS);
		}
#endif
		return v[index];
	}

	inline b3VectorTemplate<F,dim> &operator+=(const b3VectorTemplate<F,dim> &a)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] += a.v[i];
		return *this;
	}

	inline b3VectorTemplate<F,dim> operator+(const b3VectorTemplate<F,dim> &a)
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++) result.v[i] = v[i] + (F)a.v[i];
		return result;
	}

	inline b3VectorTemplate<F,dim> &operator-=(const b3VectorTemplate<F,dim> &a)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] -= a.v[i];
		return *this;
	}

	inline b3VectorTemplate<F,dim> operator-(const b3VectorTemplate<F,dim> &a)
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++) result.v[i] = v[i] - (F)a.v[i];
		return result;
	}

	inline b3VectorTemplate<F,dim> operator-()
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++) result.v[i] = -v[i];
		return result;
	}

	inline b3VectorTemplate<F,dim> &operator*=(const b3VectorTemplate<F,dim> &a)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] *= a.v[i];
		return *this;
	}

	inline b3VectorTemplate<F,dim> operator*(const b3VectorTemplate<F,dim> &a)
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++) result.v[i] = v[i] * a.v[i];
		return result;
	}

	inline b3VectorTemplate<F,dim> &operator*=(const b3_f64 value)
	{
		b3VectorTemplate<F,dim> prod(value);

		for (b3_loop i = 0;i < dim;i++) v[i] *= prod.v[i];
		return *this;
	}

	inline b3VectorTemplate<F,dim> operator*(const b3_f64 value)
	{
		b3VectorTemplate<F,dim> result,multiplicator(value);

		for (b3_loop i = 0;i < dim;i++) result.v[i] = v[i] * multiplicator.v[i];
		return result;
	}

	inline b3VectorTemplate<F,dim> &operator/=(const b3VectorTemplate<F,dim> &a)
	{
		for (b3_loop i = 0;i < dim;i++) v[i] /= a.v[i];
		return *this;
	}

	inline b3VectorTemplate<F,dim> operator/(const b3VectorTemplate<F,dim> &a)
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++) result.v[i] = v[i] / a.v[i];
		return result;
	}

	inline b3VectorTemplate<F,dim> &operator/=(const b3_f64 value)
	{
		b3VectorTemplate<F,dim> prod(value);

		for (b3_loop i = 0;i < dim;i++) v[i] /= prod.v[i];
		return *this;
	}

	inline b3VectorTemplate<F,dim> operator/(const b3_f64 value)
	{
		b3VectorTemplate<F,dim> result,divisor(value);

		for (b3_loop i = 0;i < dim;i++) result.v[i] = v[i] / divisor.v[i];
		return result;
	}

	inline void b3Negate()
	{
		for (b3_loop i = 0;i < dim;i++) v[i] = -v[i];
	}

	inline F b3Length()
	{
		return sqrt(b3QuadLength());
	}

	inline static F b3SMul(
		const b3VectorTemplate<F,dim> &a,
		const b3VectorTemplate<F,dim> &b)
	{
		F B3_ALIGN_16 r[dim];

		for (b3_loop i = 0;i < dim;i++)
		{
			r[i] = a.v[i] * b.v[i];
		}
		return r[0] + r[1] + r[2] + r[3];
	}

	inline F b3SMul(
		const b3VectorTemplate<F,dim> &a)
	{
		F result = 0;

		for (b3_loop i = 0;i < dim;i++)
		{
			result += (a.v[i] * v[i]);
		}
		return result;
	}

	inline static b3VectorTemplate<F,dim> b3CrossProduct(
		const b3VectorTemplate<F,dim> &a,
		const b3VectorTemplate<F,dim> &b)
	{
		return b3VectorTemplate<F,dim>(
			a.v[Y] * b.v[Z] - a.v[Z] * b.v[Y],
			a.v[Z] * b.v[X] - a.v[X] * b.v[Z],
			a.v[X] * b.v[Y] - a.v[Y] * b.v[X],(F)0.0);
	}

	inline b3VectorTemplate<F,dim> b3CrossProduct(
		const b3VectorTemplate<F,dim> &b)
	{
		return b3VectorTemplate<F,dim>(
			v[Y] * b.v[Z] - v[Z] * b.v[Y],
			v[Z] * b.v[X] - v[X] * b.v[Z],
			v[X] * b.v[Y] - v[Y] * b.v[X],(F)0.0);
	}

	inline F b3QuadLength()
	{
		F result = 0;

		for (b3_loop i = 0;i < dim;i++)
		{
			result += (v[i] * v[i]);
		}
		return result;
	}

	inline static F b3Distance(
		b3VectorTemplate<F,dim> &a,
		b3VectorTemplate<F,dim> &b)
	{
		b3VectorTemplate<F,dim> diff = a - b;

		return diff.b3Length();
	}

	inline F b3Normalize(const b3_f64 len = 1.0)
	{
		F old = b3Length();

		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] = v[i] * len / old;
		}
		return old;
	}

	static inline void b3InitBound(
		b3VectorTemplate<F,dim> &lower,
		b3VectorTemplate<F,dim> &upper)
	{
		lower.b3Value( FLT_MAX);
		upper.b3Value(-FLT_MAX);
	}

	inline void b3SetMinimum(F min)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] < min) v[i] = min;
		}
	}

	inline void b3CheckLowerBound(const b3VectorTemplate<F,dim> &lower)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] > lower.v[i]) v[i] = lower.v[i];
		}
	}

	inline void b3SetMaximum(F max)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] > max) v[i] = max;
		}
	}

	inline void b3CheckUpperBound(const b3VectorTemplate<F,dim> &upper)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] < upper.v[i]) v[i] = upper.v[i];
		}
	}

	static inline F b3AngleOfVectors(
		b3VectorTemplate<F,dim> &Vector1,
		b3VectorTemplate<F,dim> &Vector2)
	{
		F Denom;

		if ((Denom = Vector1.b3Length() * Vector2.b3Length()) != 0)
		{
			return b3SMul(Vector1,Vector2) / Denom;

		}
		return 0;
	}

	inline void b3AdjustBound(
		b3VectorTemplate<F,dim> &lower,
		b3VectorTemplate<F,dim> &upper)
	{
		lower.b3CheckLowerBound(*this);
		upper.b3CheckUpperBound(*this);
	}

	inline void b3MatrixVMul(const b3_matrix *Mat,const b3_bool Use4D)
	{
		F x = v[X],y = v[Y],z = v[Z];

		if (Use4D)
		{
			v[X] = (F)(x * Mat->m11 + y * Mat->m12 + z * Mat->m13 + Mat->m14);
			v[Y] = (F)(x * Mat->m21 + y * Mat->m22 + z * Mat->m23 + Mat->m24);
			v[Z] = (F)(x * Mat->m31 + y * Mat->m32 + z * Mat->m33 + Mat->m34);
		}
		else
		{
			v[X] = (F)(x * Mat->m11 + y * Mat->m12 + z * Mat->m13);
			v[Y] = (F)(x * Mat->m21 + y * Mat->m22 + z * Mat->m23);
			v[Z] = (F)(x * Mat->m31 + y * Mat->m32 + z * Mat->m33);
		}
	}

	inline void b3Print(const char *comment)
	{
		if (comment != 0)
		{
			b3PrintF(B3LOG_NORMAL,comment);
		}
		for (b3_loop i = 0;i < dim;i++)
		{
			b3PrintF(B3LOG_NORMAL,"%3.5f ",v[i]);
		}
		b3PrintF(B3LOG_NORMAL,"\n");
	}
};

typedef b3VectorTemplate<b3_f32,B3_MAX_DIM> b3Vector32;
typedef b3VectorTemplate<b3_f64,B3_MAX_DIM> b3Vector64;

struct b3Line32
{
	b3Vector32 pos;
	b3Vector32 dir;
};

struct b3Line64
{
	b3Vector64 pos;
	b3Vector64 dir;
};

#endif
