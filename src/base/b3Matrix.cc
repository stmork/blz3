/*
**
**	$Filename:	b3Matrix.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 2.02
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - vector and matrix methods
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        variables                                     **
**                                                                      **
*************************************************************************/

const b3_matrix b3Matrix::m_UnitMatrix =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

const b3_vector b3Matrix::m_EmptyVector =
{
	0, 0, 0
};

bool b3Matrix::b3NormalizeCol(
	b3_matrix * Matrix,
	b3_count    col)
{
	b3_f32 * column;
	b3_f64   Denom;
	b3_index i, index;

	column = &Matrix->m11;

	Denom  = 0;
	index  = col;
	for (i = 0; i < 4; i++)
	{
		Denom += (column[index] * column[index]);
		index += 4;
	}

	if (Denom != 0)
	{
		Denom = 1.0 / sqrt(Denom);
		index = col;
		for (i = 0; i < 4; i++)
		{
			column[index] = (b3_f32)(column[index] * Denom);
			index += 4;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool b3Matrix::b3NormalizeRow(
	b3_matrix * Matrix,
	b3_count    row)
{
	b3_f32 * column;
	b3_f64   Denom;
	b3_index i, index;

	column = &Matrix->m11;

	Denom  = 0;
	index  = row << 2;
	for (i = 0; i < 4; i++)
	{
		Denom += (column[index] * column[index]);
		index++;
	}

	if (Denom != 0)
	{
		Denom = 1.0 / sqrt(Denom);
		index = row << 2;
		for (i = 0; i < 4; i++)
		{
			column[index] = (b3_f32)(column[index] * Denom);
			index++;
		}
		return true;
	}
	else
	{
		return false;
	}
}

/*************************************************************************
**                                                                      **
**                           global part                                **
**                                                                      **
*************************************************************************/

b3_f64 b3Matrix::b3Det4(const b3_matrix * Matrix)
{
	b3_vector64 Row1, Row2, Row3, Row4;
	b3_f64      Result;

	Row1.x  = Matrix->m21;
	Row1.y  = Matrix->m31;
	Row1.z  = Matrix->m41;

	Row2.x  = Matrix->m22;
	Row2.y  = Matrix->m32;
	Row2.z  = Matrix->m42;

	Row3.x  = Matrix->m23;
	Row3.y  = Matrix->m33;
	Row3.z  = Matrix->m43;

	Row4.x  = Matrix->m24;
	Row4.y  = Matrix->m34;
	Row4.z  = Matrix->m44;

	Result  = Matrix->m11 * b3Det3(&Row2, &Row3, &Row4);
	Result -= Matrix->m12 * b3Det3(&Row1, &Row3, &Row4);
	Result += Matrix->m13 * b3Det3(&Row1, &Row2, &Row4);
	Result -= Matrix->m14 * b3Det3(&Row1, &Row2, &Row3);

	return Result;
}

b3_matrix * b3Matrix::b3Unit(b3_matrix * Matrix)
{
	*Matrix = m_UnitMatrix;

	return Matrix;
}

b3_matrix * b3Matrix::b3Copy(
	const b3_matrix * From,
	b3_matrix    *    To)
{
	memcpy(To, From, sizeof(b3_matrix));

	return To;
}

b3_matrix * b3Matrix::b3Transpose(const b3_matrix * Src, b3_matrix * Dst)
{
	// Astonishing! The gcc is able to reduce these two loops into 20
	// SSE instructions!
	const b3_f32 * src = &Src->m11;
	b3_f32    *    dst = &Dst->m11;

	for (b3_loop x = 0; x < 4; ++x)
	{
		for (b3_loop y = 0; y < 4; ++y)
		{
			*dst++ = src[y * 4 + x];
		}
	}

	return Dst;
}

b3_matrix * b3Matrix::b3Inverse(
	const b3_matrix * From,
	b3_matrix    *    To)
{
	const b3_f64      Denom = b3Det4(From);
	b3_vector64       Row1, Row2, Row3, Row4;

	if (Denom == 0)
	{
		return (nullptr);
	}

	Row1.x  = From->m12;
	Row2.x  = From->m22;
	Row1.y  = From->m13;
	Row2.y  = From->m23;
	Row1.z  = From->m14;
	Row2.z  = From->m24;

	Row3.x  = From->m32;
	Row4.x  = From->m42;
	Row3.y  = From->m33;
	Row4.y  = From->m43;
	Row3.z  = From->m34;
	Row4.z  = From->m44;

	/* inverting first line */
	To->m11 =   (b3_f32)(b3Det3(&Row2, &Row3, &Row4) / Denom);
	To->m12 = - (b3_f32)(b3Det3(&Row1, &Row3, &Row4) / Denom);
	To->m13 =   (b3_f32)(b3Det3(&Row1, &Row2, &Row4) / Denom);
	To->m14 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row3) / Denom);

	/* inverting second line */
	Row1.x  =   From->m11;
	Row2.x  =   From->m21;
	Row3.x  =   From->m31;
	Row4.x  =   From->m41;
	To->m21 = - (b3_f32)(b3Det3(&Row2, &Row3, &Row4) / Denom);
	To->m22 =   (b3_f32)(b3Det3(&Row1, &Row3, &Row4) / Denom);
	To->m23 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row4) / Denom);
	To->m24 =   (b3_f32)(b3Det3(&Row1, &Row2, &Row3) / Denom);

	/* inverting third line */
	Row1.y  =   From->m12;
	Row2.y  =   From->m22;
	Row3.y  =   From->m32;
	Row4.y  =   From->m42;
	To->m31 =   (b3_f32)(b3Det3(&Row2, &Row3, &Row4) / Denom);
	To->m32 = - (b3_f32)(b3Det3(&Row1, &Row3, &Row4) / Denom);
	To->m33 =   (b3_f32)(b3Det3(&Row1, &Row2, &Row4) / Denom);
	To->m34 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row3) / Denom);

	/* inverting third fourth */
	Row1.z  =   From->m13;
	Row2.z  =   From->m23;
	Row3.z  =   From->m33;
	Row4.z  =   From->m43;
	To->m41 = - (b3_f32)(b3Det3(&Row2, &Row3, &Row4) / Denom);
	To->m42 =   (b3_f32)(b3Det3(&Row1, &Row3, &Row4) / Denom);
	To->m43 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row4) / Denom);
	To->m44 =   (b3_f32)(b3Det3(&Row1, &Row2, &Row3) / Denom);

	return To;
}

b3_matrix * b3Matrix::b3MMul(
	const b3_matrix	* B,
	const b3_matrix	* A,
	b3_matrix    *    C)
{
	b3_matrix t;

	/*
	 * Transposing one matrix to multiply with another matrix is a trick.
	 * Normally multiplying two matrices are done by multiplying a row with
	 * a column. If you transpose one matrix it can be done row by row which
	 * is better for caching.
	 */
	b3Transpose(B, &t);

	const b3_f32 * a   = &A->m11;
	b3_f32 *       dst = &C->m11;

	for (b3_loop k = 0; k < 4; k++)
	{
		const b3_f32 * b = &t.m11;

		for (b3_loop j = 0; j < 4; j++)
		{
			// The body of the inner loop is in fact a dot product. This may be
			// reduced to one SSE4 @c dpps instruction.
			b3_f32  cell[4];
			b3_f32  sum = 0;

			// The optimizer may reduce this to a @c mulps SSE instruction.
			for (b3_loop i = 0; i < 4; i++)
			{
				cell[i] = a[i] * b[i];
			}

			// These vector operation can be reduced to two @c haddps
			// instructions which are unfortunately very slow.
			for (b3_loop i = 0; i < 4; i++)
			{
				sum += cell[i];
			}
			*dst++ = sum;
			b += 4;
		}
		a += 4;
	}

	return C;
}

b3_matrix * b3Matrix::b3MAdd(
	const b3_matrix * A,
	const b3_matrix * B,
	b3_matrix    *    C)
{
	const b3_f32 * aPtr = &A->m11;
	const b3_f32 * bPtr = &B->m11;
	b3_f32    *    dst  = &C->m11;

	for (b3_loop i = 0; i < 16; i++)
	{
		dst[i] = aPtr[i] + bPtr[i];
	}

	return C;
}

b3_matrix * b3Matrix::b3Move(
	const b3_matrix * Src, b3_matrix * Dst,
	const b3_f64      x,
	const b3_f64      y,
	const b3_f64      z)
{
	b3_vector move;

	b3Vector::b3Init(&move, x, y, z);
	return b3Move(Src, Dst, &move);
}

b3_matrix * b3Matrix::b3Move(
	const b3_matrix * Src,
	b3_matrix    *    Dst,
	const b3_vector * Translation)
{
	b3_matrix Move;

	if (Src == nullptr)
	{
		Src = &m_UnitMatrix;
	}

	Move     =  m_UnitMatrix;
	Move.m14 =  Translation->x;
	Move.m24 =  Translation->y;
	Move.m34 =  Translation->z;

	return b3MMul(Src, &Move, Dst);
}

b3_matrix * b3Matrix::b3MoveNegative(
	const b3_matrix * Src, b3_matrix * Dst,
	const b3_vector * Translation)
{
	b3_matrix Move;

	if (Src == nullptr)
	{
		Src = &m_UnitMatrix;
	}

	Move     =  m_UnitMatrix;
	Move.m14 = -Translation->x;
	Move.m24 = -Translation->y;
	Move.m34 = -Translation->z;

	return b3MMul(Src, &Move, Dst);
}

b3_matrix * b3Matrix::b3Scale(
	const b3_matrix * Src,
	b3_matrix    *    Dst,
	const b3_vector * Center,
	const b3_f64      x,
	const b3_f64      y,
	const b3_f64      z)
{
	b3_vector scale;

	b3Vector::b3Init(&scale, x, y, z);
	return b3Scale(Src, Dst, Center, &scale);
}

b3_matrix * b3Matrix::b3Scale(
	const b3_matrix * Src, b3_matrix * Dst,
	const b3_vector * Center,
	const b3_vector * Scale)
{
	b3_matrix Operator;

	if (Center == nullptr)
	{
		Center = &m_EmptyVector;
	}

	Operator     = m_UnitMatrix;
	Operator.m11 = Scale->x;
	Operator.m22 = Scale->y;
	Operator.m33 = Scale->z;

	b3MoveNegative(Src, Dst, Center);
	b3MMul(Dst, &Operator, Dst);

	return b3Move(Dst, Dst, Center);
}

b3_matrix * b3Matrix::b3RotateX(
	const b3_matrix * A, b3_matrix * B,
	const b3_vector * Center,
	const b3_f64      Angle)
{
	b3_matrix Result, CenterMatrix;
	b3_f32    Cos, Sin;

	if (A == nullptr)
	{
		A = &m_UnitMatrix;
	}
	if (Center == nullptr)
	{
		Center = &m_EmptyVector;
	}
	Cos = (b3_f32)cos(Angle);
	Sin = (b3_f32)sin(Angle);

	Result     =  m_UnitMatrix;
	Result.m22 =  Cos;
	Result.m32 =  Sin;
	Result.m23 = -Sin;
	Result.m33 =  Cos;

	b3MoveNegative(A, &CenterMatrix, Center);
	b3MMul(&CenterMatrix, &Result, &Result);
	return b3Move(&Result, B, Center);
}

b3_matrix * b3Matrix::b3RotateY(
	const b3_matrix * A, b3_matrix * B,
	const b3_vector * Center,
	const b3_f64      Angle)
{
	b3_matrix Result, CenterMatrix;
	b3_f32    Cos, Sin;

	if (A == nullptr)
	{
		A = &m_UnitMatrix;
	}
	if (Center == nullptr)
	{
		Center = &m_EmptyVector;
	}
	Cos = (b3_f32)cos(Angle);
	Sin = (b3_f32)sin(Angle);

	Result     =  m_UnitMatrix;
	Result.m11 =  Cos;
	Result.m31 = -Sin;
	Result.m13 =  Sin;
	Result.m33 =  Cos;

	b3MoveNegative(A, &CenterMatrix, Center);
	b3MMul(&CenterMatrix, &Result, &Result);
	return b3Move(&Result, B, Center);
}

b3_matrix * b3Matrix::b3RotateZ(
	const b3_matrix * A, b3_matrix * B,
	const b3_vector * Center,
	const b3_f64      Angle)
{
	b3_matrix Result, CenterMatrix;
	b3_f32    Cos, Sin;

	if (A == nullptr)
	{
		A = &m_UnitMatrix;
	}
	if (Center == nullptr)
	{
		Center = &m_EmptyVector;
	}
	Cos = (b3_f32)cos(Angle);
	Sin = (b3_f32)sin(Angle);

	Result     =  m_UnitMatrix;
	Result.m11 =  Cos;
	Result.m21 =  Sin;
	Result.m12 = -Sin;
	Result.m22 =  Cos;

	b3MoveNegative(A, &CenterMatrix, Center);
	b3MMul(&CenterMatrix, &Result, &Result);
	return b3Move(&Result, B, Center);
}

b3_matrix * b3Matrix::b3Align(
	b3_matrix   *   result,
	const b3_line * Axis)
{
	result->m11 = Axis->dir.x;
	result->m21 = Axis->dir.y;
	result->m31 = Axis->dir.z;
	result->m41 = 0;

	if ((Axis->dir.x == 0) && (Axis->dir.y == 0))
	{
		result->m12 = 0;
		result->m22 = Axis->dir.z;
		result->m32 = 0;
		result->m42 = 0;
	}
	else
	{
		result->m12 = -Axis->dir.y;
		result->m22 =  Axis->dir.x;
		result->m32 =  0;
		result->m42 =  0;
	}

	b3NormalizeCol(result, 0);
	b3NormalizeCol(result, 1);

	result->m13 =  result->m21 * result->m32 - result->m31 * result->m22;
	result->m23 =  result->m31 * result->m12 - result->m11 * result->m32;
	result->m33 =  result->m11 * result->m22 - result->m21 * result->m12;
	result->m43 =  0;

	result->m14 =  Axis->pos.x;
	result->m24 =  Axis->pos.y;
	result->m34 =  Axis->pos.z;
	result->m44 =  1;

	return result;
}

b3_matrix * b3Matrix::b3RotateVector(
	const b3_matrix * A, b3_matrix * B,
	const b3_line  *  Axis,
	const b3_f64      Angle)
{
	b3_f32    Cos, Sin;
	b3_matrix System, InvSystem, Result, Rotate;

	if (A == nullptr)
	{
		A = &m_UnitMatrix;
	}

	b3Align(&System, Axis);

	if (b3Inverse(&System, &InvSystem))
	{
		Rotate     =  m_UnitMatrix;
		Cos        = (b3_f32)cos(Angle);
		Sin        = (b3_f32)sin(Angle);
		Rotate.m22 =  Cos;
		Rotate.m32 =  Sin;
		Rotate.m23 = -Sin;
		Rotate.m33 =  Cos;

		b3MMul(A,      &InvSystem, B);
		b3MMul(B,      &Rotate,   &Result);
		b3MMul(&Result, &System,    B);
	}
	else
	{
		B = nullptr;
	}

	return B;
}

b3_matrix * b3Matrix::b3MirrorPoint(
	const b3_matrix * A, b3_matrix * B,
	const b3_vector * Center,
	const b3_f64      a)
{
	b3_matrix Mirror, Mirrored;

	if (A == nullptr)
	{
		A = &m_UnitMatrix;
	}
	if (Center == nullptr)
	{
		Center = &m_EmptyVector;
	}

	b3MoveNegative(A, B, Center);

	Mirror     = m_UnitMatrix;
	Mirror.m11 = -(b3_f32)a;
	Mirror.m22 = -(b3_f32)a;
	Mirror.m33 = -(b3_f32)a;
	b3MMul(B, &Mirror, &Mirrored);
	b3Move(&Mirrored, B, Center);

	return B;
}

b3_matrix * b3Matrix::b3MirrorAxis(
	const b3_matrix * A,
	b3_matrix    *    B,
	const b3_line  *  Axis,
	const b3_f64      a)
{
	b3_matrix Mirror, Mirrored, Result;
	b3_matrix System, InvSystem;

	if (A == nullptr)
	{
		A = &m_UnitMatrix;
	}

	System.m11 = Axis->dir.x;
	System.m21 = Axis->dir.y;
	System.m31 = Axis->dir.z;
	System.m41 = 0;

	if ((Axis->dir.x == 0) && (Axis->dir.y == 0))
	{
		System.m12 = 0;
		System.m22 = Axis->dir.z;
		System.m32 = 0;
		System.m42 = 0;
	}
	else
	{
		System.m12 = -Axis->dir.y;
		System.m22 =  Axis->dir.x;
		System.m32 =  0;
		System.m42 =  0;
	}

	b3NormalizeCol(&System, 0);
	b3NormalizeCol(&System, 1);

	System.m13 = System.m21 * System.m32 - System.m31 * System.m22;
	System.m23 = System.m31 * System.m12 - System.m11 * System.m32;
	System.m33 = System.m11 * System.m22 - System.m21 * System.m12;
	System.m43 = 0;

	System.m14 = Axis->pos.x;
	System.m24 = Axis->pos.y;
	System.m34 = Axis->pos.z;
	System.m44 = 1;

	if (!b3Inverse(&System, &InvSystem))
	{
		return nullptr;
	}

	Mirror = m_UnitMatrix;
	Mirror.m22 = -(b3_f32)a;
	Mirror.m33 = -(b3_f32)a;

	b3MMul(A,        &InvSystem, &Result);
	b3MMul(&Result,  &Mirror,   &Mirrored);
	b3MMul(&Mirrored, &System,    B);
	return B;
}

b3_matrix * b3Matrix::b3MirrorPlane(
	const b3_matrix * A,
	b3_matrix * B,
	const b3_vector * Base,
	const b3_vector * Dir1,
	const b3_vector * Dir2,
	const b3_f64      a)
{
	b3_matrix Mirror, Mirrored, Result;
	b3_matrix System, InvSystem;

	if (A == nullptr)
	{
		A = &m_UnitMatrix;
	}

	System.m11 = Dir1->x;
	System.m21 = Dir1->y;
	System.m31 = Dir1->z;
	System.m41 = 0;

	System.m12 = Dir2->x;
	System.m22 = Dir2->y;
	System.m32 = Dir2->z;
	System.m42 = 0;

	b3NormalizeCol(&System, 0);
	b3NormalizeCol(&System, 1);

	System.m13 = System.m21 * System.m32 - System.m31 * System.m22;
	System.m23 = System.m31 * System.m12 - System.m11 * System.m32;
	System.m33 = System.m11 * System.m22 - System.m21 * System.m12;
	System.m43 = 0;

	System.m14 = Base->x;
	System.m24 = Base->y;
	System.m34 = Base->z;
	System.m44 = 1;

	if (!b3Inverse(&System, &InvSystem))
	{
		return nullptr;
	}

	Mirror     = m_UnitMatrix;
	Mirror.m33 = -(b3_f32)a;

	b3MMul(A,        &InvSystem, &Result);
	b3MMul(&Result,  &Mirror,   &Mirrored);
	b3MMul(&Mirrored, &System,   B);
	return B;
}

/* This routine creates a transformation matrix where an object is dressed */
/* along a specified vector. This vector is comparable to a x axis in */
/* an basis transformation. The other orthonormal basises are computed to. */
/* An additional position is specified to so this transformation moves the */
/* object, too, and you mustn't move the Object via MatrixMove()! */
/* ----------------------------------------------------------------------- */
/* prev:      the previous matrix where the transformation is linked to */
/* transform: the result transformation including prev */
/* center:    the base of the orthonormal basis */
/* dir:       equal to the new x axis. */

b3_matrix * b3Matrix::b3Dress(
	const b3_matrix * prev, b3_matrix * transform,
	const b3_vector * center,
	const b3_vector * lookTo,
	const b3_vector * oldLook,
	const bool        future)
{
	b3_matrix orientation;
	b3_vector axis;

	// dress vector is x axis
	orientation.m11 = lookTo->x;
	orientation.m21 = lookTo->y;
	orientation.m31 = lookTo->z;
	orientation.m41 = 0;
	b3NormalizeCol(&orientation, 0);

	// now compute z axis from dress vector and direction vector
	if (future)
	{
		b3Vector::b3CrossProduct(lookTo, oldLook, &axis);
	}
	else
	{
		b3Vector::b3CrossProduct(oldLook, lookTo, &axis);
	}
	if (b3Vector::b3Normalize(&axis))
	{
		if (axis.z < 0)
		{
			b3Vector::b3Negate(&axis);
		}
		orientation.m13 = axis.x;
		orientation.m23 = axis.y;
		orientation.m33 = axis.z;
		orientation.m43 = 0;

		// now compute y axis from x and z axis
		orientation.m12 = lookTo->y * axis.z - lookTo->z * axis.y;
		orientation.m22 = lookTo->z * axis.x - lookTo->x * axis.z;
		orientation.m32 = lookTo->x * axis.y - lookTo->y * axis.x;
		orientation.m42 = 0;
		b3NormalizeCol(&orientation, 1);
	}
	else
	{
		b3Unit(&orientation);
	}

	// reposition object
	orientation.m14 = center->x;
	orientation.m24 = center->y;
	orientation.m34 = center->z;
	orientation.m44 = 1;

	// "undo" from transformation, "do" to transformation
	b3MMul(prev, &orientation, transform);
	return transform;
}

const b3_matrix * b3Matrix::b3Dump(const b3_matrix * m, const char * title)
{
	const std::string & test = b3ToString(m, title);

	b3PrintF(B3LOG_FULL, "%s\n", test.c_str());
	return m;
}

std::string b3Matrix::b3ToString(const b3_matrix * m, const char * title)
{
	std::string result;
	char        head[128];

	snprintf(head, sizeof(head),
		"%s (m=%p)\n",
		title != nullptr ? title : "b3Matrix::b3Dump", m);
	result = head;

	if (m != nullptr)
	{
		char buffer[256];

		snprintf(buffer, sizeof(buffer),
			"%3.03f %3.03f %3.03f %3.03f\n"
			"%3.03f %3.03f %3.03f %3.03f\n"
			"%3.03f %3.03f %3.03f %3.03f\n"
			"%3.03f %3.03f %3.03f %3.03f\n",
			m->m11, m->m12, m->m13, m->m14,
			m->m21, m->m22, m->m23, m->m24,
			m->m31, m->m32, m->m33, m->m34,
			m->m41, m->m42, m->m43, m->m44);

		result += std::string(buffer);
	}
	return result;
}
