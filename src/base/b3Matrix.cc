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

b3_matrix b3Matrix::m_UnitMatrix =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

b3_vector b3Matrix::m_EmptyVector =
{
	0, 0, 0
};

b3_bool b3Matrix::b3NormalizeCol(
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

b3_bool b3Matrix::b3NormalizeRow(
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
	b3_f64    Result;

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

	return (Result);
}

b3_matrix * b3Matrix::b3Unit(b3_matrix * Matrix)
{
	memcpy(Matrix, &m_UnitMatrix, sizeof(b3_matrix));

	return Matrix;
}

b3_matrix * b3Matrix::b3Transport(
	const b3_matrix * From,
	b3_matrix    *    To)
{
	memcpy(To, From, sizeof(b3_matrix));

	return To;
}

b3_matrix * b3Matrix::b3Inverse(
	const b3_matrix * From,
	b3_matrix    *    To)
{
	b3_f64      Denom;
	b3_vector64 Row1, Row2, Row3, Row4;

	Denom = b3Det4(From);
	if (Denom == 0)
	{
		return (nullptr);
	}
	Denom = 1 / Denom;

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
	To->m11 = (b3_f32)(b3Det3(&Row2, &Row3, &Row4) * Denom);
	To->m12 = - (b3_f32)(b3Det3(&Row1, &Row3, &Row4) * Denom);
	To->m13 = (b3_f32)(b3Det3(&Row1, &Row2, &Row4) * Denom);
	To->m14 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row3) * Denom);

	/* inverting second line */
	Row1.x  =   From->m11;
	Row2.x  =   From->m21;
	Row3.x  =   From->m31;
	Row4.x  =   From->m41;
	To->m21 = - (b3_f32)(b3Det3(&Row2, &Row3, &Row4) * Denom);
	To->m22 = (b3_f32)(b3Det3(&Row1, &Row3, &Row4) * Denom);
	To->m23 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row4) * Denom);
	To->m24 = (b3_f32)(b3Det3(&Row1, &Row2, &Row3) * Denom);

	/* inverting third line */
	Row1.y  =   From->m12;
	Row2.y  =   From->m22;
	Row3.y  =   From->m32;
	Row4.y  =   From->m42;
	To->m31 = (b3_f32)(b3Det3(&Row2, &Row3, &Row4) * Denom);
	To->m32 = - (b3_f32)(b3Det3(&Row1, &Row3, &Row4) * Denom);
	To->m33 = (b3_f32)(b3Det3(&Row1, &Row2, &Row4) * Denom);
	To->m34 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row3) * Denom);

	/* inverting third fourth */
	Row1.z  =   From->m13;
	Row2.z  =   From->m23;
	Row3.z  =   From->m33;
	Row4.z  =   From->m43;
	To->m41 = - (b3_f32)(b3Det3(&Row2, &Row3, &Row4) * Denom);
	To->m42 = (b3_f32)(b3Det3(&Row1, &Row3, &Row4) * Denom);
	To->m43 = - (b3_f32)(b3Det3(&Row1, &Row2, &Row4) * Denom);
	To->m44 = (b3_f32)(b3Det3(&Row1, &Row2, &Row3) * Denom);

	return To;
}

b3_matrix * b3Matrix::b3MMul(
	const b3_matrix	* B,
	const b3_matrix	* A,
	b3_matrix    *    C)
{
	b3_matrix Result;

	Result.m11 = A->m11 * B->m11 + A->m12 * B->m21 + A->m13 * B->m31 + A->m14 * B->m41;
	Result.m12 = A->m11 * B->m12 + A->m12 * B->m22 + A->m13 * B->m32 + A->m14 * B->m42;
	Result.m13 = A->m11 * B->m13 + A->m12 * B->m23 + A->m13 * B->m33 + A->m14 * B->m43;
	Result.m14 = A->m11 * B->m14 + A->m12 * B->m24 + A->m13 * B->m34 + A->m14 * B->m44;

	Result.m21 = A->m21 * B->m11 + A->m22 * B->m21 + A->m23 * B->m31 + A->m24 * B->m41;
	Result.m22 = A->m21 * B->m12 + A->m22 * B->m22 + A->m23 * B->m32 + A->m24 * B->m42;
	Result.m23 = A->m21 * B->m13 + A->m22 * B->m23 + A->m23 * B->m33 + A->m24 * B->m43;
	Result.m24 = A->m21 * B->m14 + A->m22 * B->m24 + A->m23 * B->m34 + A->m24 * B->m44;

	Result.m31 = A->m31 * B->m11 + A->m32 * B->m21 + A->m33 * B->m31 + A->m34 * B->m41;
	Result.m32 = A->m31 * B->m12 + A->m32 * B->m22 + A->m33 * B->m32 + A->m34 * B->m42;
	Result.m33 = A->m31 * B->m13 + A->m32 * B->m23 + A->m33 * B->m33 + A->m34 * B->m43;
	Result.m34 = A->m31 * B->m14 + A->m32 * B->m24 + A->m33 * B->m34 + A->m34 * B->m44;

	Result.m41 = A->m41 * B->m11 + A->m42 * B->m21 + A->m43 * B->m31 + A->m44 * B->m41;
	Result.m42 = A->m41 * B->m12 + A->m42 * B->m22 + A->m43 * B->m32 + A->m44 * B->m42;
	Result.m43 = A->m41 * B->m13 + A->m42 * B->m23 + A->m43 * B->m33 + A->m44 * B->m43;
	Result.m44 = A->m41 * B->m14 + A->m42 * B->m24 + A->m43 * B->m34 + A->m44 * B->m44;

	*C = Result;
	return C;
}

b3_matrix * b3Matrix::b3MAdd(
	b3_matrix * A,
	b3_matrix * B,
	b3_matrix * C)
{
	b3_f32 * aPtr = &A->m11;
	b3_f32 * bPtr = &B->m11;
	b3_f32 * dst  = &C->m11;

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
	const b3_bool     future)
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

b3_matrix * b3Matrix::b3Dump(b3_matrix * m, const char * title)
{
	b3PrintF(B3LOG_FULL, "%s (m=%p)\n", title != nullptr ? title : "b3Matrix::b3Dump", m);

	if (m != nullptr)
	{
		b3PrintF(B3LOG_FULL, "%3.3f %3.3f %3.3f %3.3f\n", m->m11, m->m12, m->m13, m->m14);
		b3PrintF(B3LOG_FULL, "%3.3f %3.3f %3.3f %3.3f\n", m->m21, m->m22, m->m23, m->m24);
		b3PrintF(B3LOG_FULL, "%3.3f %3.3f %3.3f %3.3f\n", m->m31, m->m32, m->m33, m->m34);
		b3PrintF(B3LOG_FULL, "%3.3f %3.3f %3.3f %3.3f\n", m->m41, m->m42, m->m43, m->m44);
	}
	return m;
}
