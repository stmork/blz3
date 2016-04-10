/*
**
**	$Filename:	b3Vector.h $
**	$Release:	Dortmund 2002, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Vector template
**
**	(C) Copyright  Steffen A. Mork
**	    All Rights Reserved
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

/**
 * This enumeration lists all possible vector error states.
 */
enum b3_vector_error
{
	B3_VECTOR_ERROR = -1,
	B3_VECTOR_OK    =  0,
	B3_VECTOR_OUT_OF_BOUNDS
};

typedef b3Exception<b3_vector_error,0x564543> b3VectorException;

/**
 * This is a template class fur use in the b3Spline template class.
 * It can take a component type and the dimension of the vector space.
 */
template<class F,b3_loop dim> class B3_PLUGIN b3VectorTemplate
{
	F B3_ALIGN_16 v[dim]; //!< The values.

public:
	/**
	 * This constructor does simply nothing.
	 */
	inline b3VectorTemplate<F, dim>()
	{
	}

	/**
	 * This copy constructor copies one vector template instance.
	 *
	 * @param src The source vector template.
	 */
	inline b3VectorTemplate<F, dim>(const b3VectorTemplate<F, dim> &src)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] = src.v[i];
		}
	}

	/**
	 * This constructor initializes all vector components with the same value.
	 *
	 * @param value The value to initialize.
	 */
	inline b3VectorTemplate<F, dim>(F value)
	{
		b3Value(value);
	}

	/**
	 * This constructor initializes this instance with the given components.
	 *
	 * @param x The new x value.
	 * @param y The new y value.
	 * @param z The new z value.
	 * @param w The new w value.
	 */
	inline b3VectorTemplate<F, dim>(
		b3_f32 x,
		b3_f32 y,
		b3_f32 z,
		b3_f32 w)
	{
		b3Init(x,y,z,w);
	}

	/**
	 * This constructor initializes this instance with the given components.
	 *
	 * @param x The new x value.
	 * @param y The new y value.
	 * @param z The new z value.
	 * @param w The new w value.
	 */
	inline b3VectorTemplate<F, dim>(
		b3_f64 x,
		b3_f64 y,
		b3_f64 z,
		b3_f64 w)
	{
		b3Init(x,y,z,w);
	}

	/**
	 * This copy constructor copies a b3Vector into this instance.
	 *
	 * @param vec The source vector.
	 */
	inline b3VectorTemplate(const b3_vector32 &vec)
	{
		b3Init(vec.x,vec.y,vec.z);
	}

	/**
	 * This copy constructor copies a b3Vector into this instance.
	 *
	 * @param vec The source vector.
	 */
	inline b3VectorTemplate(const b3_gl_vector &vec)
	{
		b3Init(vec.x,vec.y,vec.z);
	}

	/**
	 * This copy constructor copies a b3Vector into this instance.
	 *
	 * @param vec The source vector.
	 */
	inline b3VectorTemplate(const b3_vector64 &vec)
	{
		b3Init(vec.x,vec.y,vec.z);
	}

	/**
	 * This copy constructor copies a b3Vector into this instance.
	 *
	 * @param vec The source vector.
	 */
	inline b3VectorTemplate(const b3_vector32_4D &vec)
	{
		b3Init(vec.x,vec.y,vec.z,vec.w);
	}

	/**
	 * This copy constructor copies a b3Vector into this instance.
	 *
	 * @param vec The source vector.
	 */
	inline b3VectorTemplate(const b3_vector64_4D &vec)
	{
		b3Init(vec.x,vec.y,vec.z,vec.w);
	}

	/**
	 * This method initializes all vector components to zero.
	 */
	inline void b3Zero()
	{
		b3PrintF(B3LOG_FULL, "--zero %p %p\n", this, v);
		for (b3_loop i = 0;i < dim; i++)
		{
			v[i] = 0;
		}
	}

	/**
	 * This method sets all vector components to a given value.
	 *
	 * @param value The components value.
	 */
	inline void b3Value(const b3_f32 value = 0)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] = (F)value;
		}
	}

	/**
	 * This method sets all vector components to a given value.
	 *
	 * @param value The components value.
	 */
	inline void b3Value(const b3_f64 value = 0)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] = (F)value;
		}
	}

	/**
	 * This method initialize the vector components with the given values.
	 *
	 * @param x The new x value.
	 * @param y The new y value.
	 * @param z The new z value.
	 * @param w The new w value.
	 */
	inline void b3Init(
		const b3_f64 x = 0,
		const b3_f64 y = 0,
		const b3_f64 z = 0,
		const b3_f64 w = 0)
	{
		b3_f64  param[4];
		b3_loop max = B3_MIN(4, dim);

		param[0] = x;
		param[1] = y;
		param[2] = z;
		param[3] = w;

		for (b3_loop i = 0; i < max; i++)
		{
			v[i] = static_cast<F>(param[i]);
		}
	}

	/**
	 * This cast operator returns a b3_vector32 instance.
	 *
	 * @return A new b3_vector32 instance.
	 */
	inline operator b3_vector32 () const
	{
		b3_vector32 result;

		result.x = (b3_f32)v[X];
		result.y = (b3_f32)v[Y];
		result.z = (b3_f32)v[Z];

		return result;
	}


	/**
	 * This cast operator returns a b3_gl_vector instance.
	 *
	 * @return A new b3_gl_vector instance.
	 */
	inline operator b3_gl_vector ()
	{
		b3_gl_vector result;

		result.x = (b3_f32)v[X];
		result.y = (b3_f32)v[Y];
		result.z = (b3_f32)v[Z];
		return result;
	}

	/**
	 * This cast operator returns a b3_vector64 instance.
	 *
	 * @return A new b3_vector64 instance.
	 */
	inline operator b3_vector64 () const
	{
		b3_vector64 result;

		result.x = (b3_f64)v[X];
		result.y = (b3_f64)v[Y];
		result.z = (b3_f64)v[Z];

		return result;
	}

	/**
	 * This index operator returns a vector component depending on
	 * the given index.
	 *
	 * @param index The component index.
	 * @return The indexed compontent.
	 */
	inline const F & operator [](const b3_vector_index index) const
	{
#ifdef _DEBUG
		if ((index < 0) || (index >= dim))
		{
			B3_THROW(b3VectorException,B3_VECTOR_OUT_OF_BOUNDS);
		}
#endif
		return v[index];
	}

	/**
	 * This index operator returns a vector component depending on
	 * the given index.
	 *
	 * @param index The component index.
	 * @return The indexed compontent.
	 */
	inline F & operator [](const b3_vector_index index)
	{
#ifdef _DEBUG
		if ((index < 0) || (index >= dim))
		{
			B3_THROW(b3VectorException,B3_VECTOR_OUT_OF_BOUNDS);
		}
#endif
		return v[index];
	}

	inline const bool operator==(const b3VectorTemplate<F,dim> &a) const
	{
		bool result = true;

		for (b3_loop i = 0;i < dim;i++)
		{
			result &= (v[i] == a.v[i]);
		}
		return result;
	}

	inline const bool operator!=(const b3VectorTemplate<F,dim> &a) const
	{
		bool result = false;

		for (b3_loop i = 0;i < dim;i++)
		{
			result |= (v[i] != a.v[i]);
		}
		return result;
	}

	/**
	 * This operator adds a specified vector to this instance.
	 *
	 * @param a The vector to add.
	 * @return A reference to this instance.
	 */
	inline b3VectorTemplate<F,dim> &operator+=(const b3VectorTemplate<F,dim> &a)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] += a.v[i];
		}
		return *this;
	}

	/**
	 * This operator adds a vector to this instance resulting in a new instance.
	 *
	 * @param a The vector to add.
	 * @return A new b3VectorTemplate instance.
	 */
	inline const b3VectorTemplate<F,dim> operator+(const b3VectorTemplate<F,dim> &a) const
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++)
		{
			result.v[i] = v[i] + (F)a.v[i];
		}
		return result;
	}

	/**
	 * This operator subtracts a specified vector from this instance.
	 *
	 * @param a The vector to subtract.
	 * @return A reference to this instance.
	 */
	inline b3VectorTemplate<F,dim> &operator-=(const b3VectorTemplate<F,dim> &a)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] -= a.v[i];
		}
		return *this;
	}

	/**
	 * This operator subtracts a vector from this instance resulting in a new instance.
	 *
	 * @param a The vector to subtract.
	 * @return A new b3VectorTemplate instance.
	 */
	inline const b3VectorTemplate<F,dim> operator-(const b3VectorTemplate<F,dim> &a) const
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++)
		{
			result.v[i] = v[i] - (F)a.v[i];
		}
		return result;
	}

	/**
	 * This method negates this instance into a new one.
	 *
	 * @return The resulting negated vector.
	 */
	inline const b3VectorTemplate<F,dim> operator-() const
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++)
		{
			result.v[i] = -v[i];
		}
		return result;
	}

	/**
	 * This operator multipliess a specified vector to this instance.
	 *
	 * @param a The vector to multiply.
	 * @return A reference to this instance.
	 */
	inline b3VectorTemplate<F,dim> &operator*=(const b3VectorTemplate<F,dim> &a)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] *= a.v[i];
		}
		return *this;
	}

	/**
	 * This operator multiplies a vector to this instance resulting in a new instance.
	 *
	 * @param a The vector to multiply.
	 * @return A new b3VectorTemplate instance.
	 */
	inline const b3VectorTemplate<F,dim> operator*(const b3VectorTemplate<F,dim> &a) const
	{
		b3VectorTemplate<F,dim> result;

		for (b3_loop i = 0;i < dim;i++)
		{
			result.v[i] = v[i] * (F)a.v[i];
		}
		return result;
	}

	/**
	 * This operator multiplies this instance with a specified value.
	 *
	 * @param value The scaling factor.
	 * @return A reference to this vector.
	 */
	inline b3VectorTemplate<F,dim> &operator*=(const b3_f64 value)
	{
		b3VectorTemplate<F,dim> prod(value);

		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] *= prod.v[i];
		}
		return *this;
	}

	/**
	 * This operator scales this instance with a specified value resulting in a new instance.
	 *
	 * @param value The scaling factor.
	 * @return The new b3VectorTemplate instance.
	 */
	inline const b3VectorTemplate<F,dim> operator*(const b3_f64 value) const
	{
		b3VectorTemplate<F,dim> result,multiplicator(value);

		for (b3_loop i = 0;i < dim;i++)
		{
			result.v[i] = v[i] * multiplicator.v[i];
		}
		return result;
	}

	/**
	 * This operator divides this instance by a specified divisor.
	 *
	 * @param value The divisor.
	 * @return A reference to this vector.
	 */
	inline b3VectorTemplate<F,dim> &operator/=(const b3_f64 value)
	{
		b3VectorTemplate<F,dim> prod(value);

		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] /= prod.v[i];
		}
		return *this;
	}

	/**
	 * This operator divides this instance by a specified divisor resulting in a new instance.
	 *
	 * @param value The divisor.
	 * @return The new b3VectorTemplate instance.
	 */
	inline const b3VectorTemplate<F,dim> operator/(const b3_f64 value) const
	{
		b3VectorTemplate<F,dim> result,divisor(value);

		for (b3_loop i = 0;i < dim;i++)
		{
			result.v[i] = v[i] / divisor.v[i];
		}
		return result;
	}

	/**
	 * This method negates all components of this vector.
	 */
	inline void b3Negate()
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] = -v[i];
		}
	}

	/**
	 * This method returns the length of this vector.
	 *
	 * @return The length of this vector.
	 */
	inline const F b3Length() const
	{
		return sqrt(b3QuadLength());
	}

	/**
	 * This method computes the dot product of two vectors.
	 *
	 * @param a The first vector.
	 * @param b The second vector.
	 * @return The resulting dot product.
	 */
	inline static F b3SMul(
		const b3VectorTemplate<F,dim> &a,
		const b3VectorTemplate<F,dim> &b)
	{
		F B3_ALIGN_16 r[dim];
		F             result = 0;
		b3_loop       i;

		for (i = 0;i < dim;i++)
		{
			r[i] = a.v[i] * b.v[i];
		}

		for (i = 0;i < dim;i++)
		{
			result += r[i];
		}
		return result;
	}

	/**
	 * This method computes the dot product of two vectors.
	 *
	 * @param a The first vector the second one is this instance.
	 * @return The resulting dot product.
	 */
	inline F b3SMul(
		const b3VectorTemplate<F,dim> &a) const
	{
		F result = 0;

		for (b3_loop i = 0;i < dim;i++)
		{
			result += (a.v[i] * v[i]);
		}
		return result;
	}

	/**
	* This method computes the cross product from two vectors: <code>result = aVec x bVec</code>.
	*
	* @param a The first vector.
	* @param b The second vector.
	* @return The result as a new b3VectorTemplate instance.
	*/
	inline static b3VectorTemplate<F,dim> b3CrossProduct(
		const b3VectorTemplate<F,dim> &a,
		const b3VectorTemplate<F,dim> &b)
	{
		return b3VectorTemplate<F,dim>(
				   a.v[Y] * b.v[Z] - a.v[Z] * b.v[Y],
				   a.v[Z] * b.v[X] - a.v[X] * b.v[Z],
				   a.v[X] * b.v[Y] - a.v[Y] * b.v[X],(F)0.0);
	}

	/**
	* This method computes the cross product from two vectors: <code>result = this x bVec</code>.
	*
	* @param b The second vector the first vector is this instance.
	* @return The result as a new b3VectorTemplate instance.
	*/
	inline b3VectorTemplate<F,dim> b3CrossProduct(
		const b3VectorTemplate<F,dim> &b) const
	{
		return b3VectorTemplate<F,dim>(
				   v[Y] * b.v[Z] - v[Z] * b.v[Y],
				   v[Z] * b.v[X] - v[X] * b.v[Z],
				   v[X] * b.v[Y] - v[Y] * b.v[X],(F)0.0);
	}

	/**
	 * The method computes the squared length of this vector.
	 *
	 * @return The resulting squared vector length.
	 */
	inline const F b3QuadLength() const
	{
		F result = 0;

		for (b3_loop i = 0;i < dim;i++)
		{
			result += (v[i] * v[i]);
		}
		return result;
	}

	/**
	 * This method computes the distance between the two given
	 * vectors interpreted as points.
	 *
	 * @param a   The first point.
	 * @param b   The second point.
	 * @return The resulting distance.
	 */
	inline static F b3Distance(
		const b3VectorTemplate<F,dim> &a,
		const b3VectorTemplate<F,dim> &b)
	{
		b3VectorTemplate<F,dim> diff = b3VectorTemplate<F,dim>(a) - b3VectorTemplate<F,dim>(b);

		return diff.b3Length();
	}

	/**
	 * This method normalizes this instance to the given length.
	 *
	 * @param len The needed length of the vector.
	 * @return The old length of the vector.
	 */
	inline F b3Normalize(const b3_f64 len = 1.0)
	{
		F old = b3Length();

		for (b3_loop i = 0;i < dim;i++)
		{
			v[i] = v[i] * len / old;
		}
		return old;
	}

	/**
	 * This method initializes two vectors for bounding box
	 * computation purposes.
	 *
	 * @param lower The lower bound of the bounding box.
	 * @param upper The upper bound of the bounding box.
	 */
	static inline void b3InitBound(
		b3VectorTemplate<F,dim> &lower,
		b3VectorTemplate<F,dim> &upper)
	{
		lower.b3Value( FLT_MAX);
		upper.b3Value(-FLT_MAX);
	}

	/**
	 * This method clamps each vector component to a given value.
	 *
	 * @param min The lower level to clamp.
	 */
	inline void b3SetMinimum(const F min)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] < min) v[i] = min;
		}
	}

	/**
	 * This method adjusts a lower corner with the values of this instance.
	 * It's a min() function for each component.
	 *
	 * @param lower The lower corner to adjust.
	 */
	inline void b3CheckLowerBound(const b3VectorTemplate<F,dim> &lower)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] > lower.v[i]) v[i] = lower.v[i];
		}
	}

	/**
	 * This method saturates each vector component to a given value.
	 *
	 * @param max The upper level to saturate.
	 */
	inline void b3SetMaximum(const F max)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] > max) v[i] = max;
		}
	}

	/**
	 * This method adjusts a upper corner with the values of this instance.
	 * It's a max() function for each component.
	 *
	 * @param upper The lower corner to adjust.
	 */
	inline void b3CheckUpperBound(const b3VectorTemplate<F,dim> &upper)
	{
		for (b3_loop i = 0;i < dim;i++)
		{
			if (v[i] < upper.v[i]) v[i] = upper.v[i];
		}
	}

	/**
	 * This method takes two vectors and sorts each component into
	 * their lower and upper value.
	 *
	 * @param lower The sorted lower vector.
	 * @param upper The sorted upper vector.
	 */
	static inline void b3Sort(
		b3VectorTemplate<F,dim> &lower,
		b3VectorTemplate<F,dim> &upper)
	{
		F aux;

		for (b3_loop i = 0;i < dim;i++)
		{
			if (lower.v[i] > upper.v[i])
			{
				aux = lower.v[i];
				lower.v[i] = upper.v[i];
				upper.v[i] = aux;
			}
		}
	}

	/**
	 * This function computes the angle between two vectors.
	 *
	 * @param Vector1 The first vector.
	 * @param Vector2 The second vector.
	 * @return The resulting angle.
	 */
	static inline F b3AngleOfVectors(
		const b3VectorTemplate<F,dim> &Vector1,
		const b3VectorTemplate<F,dim> &Vector2)
	{
		F Denom;

		if ((Denom = Vector1.b3Length() * Vector2.b3Length()) != 0)
		{
			return b3SMul(Vector1,Vector2) / Denom;

		}
		return 0;
	}

	/**
	 * This method adjusts a bounding box specified by a lower and an
	 * upper corner with this instance.
	 *
	 * @param lower The lower corner of the bounding box to adjust.
	 * @param upper The upper corner of the bounding box to adjust.
	 */
	inline void b3AdjustBound(
		b3VectorTemplate<F,dim> &lower,
		b3VectorTemplate<F,dim> &upper) const
	{
		lower.b3CheckLowerBound(*this);
		upper.b3CheckUpperBound(*this);
	}

	/**
	 * This method computes a transformation on of this vector.
	 *
	 * @param Mat The transformation matrix.
	 * @param Use4D If true this vector is a position. Otherwise it is a direction.
	 */
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

	/**
	 * This method prints the content of all vector components and adds.
	 * a specified comment.
	 *
	 * @param comment An additional comment.
	 */
	inline void b3Print(const char *comment) const
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

/**
 * This structure holds a half ray in single precision.
 */
struct b3Line32
{
	b3Vector32 pos; //!< The position.
	b3Vector32 dir; //!< The direction.
};

/**
 * This structure holds a half ray in double precision.
 */
struct b3Line64
{
	b3Vector64 pos; //!< The position.
	b3Vector64 dir; //!< The direction.
};

#endif
