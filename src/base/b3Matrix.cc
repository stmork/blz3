/*
**
**	$Filename:	b3Matrix.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 2.02 
**	$Date:		05-Jun-1998 
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

#define MATRIX_INV_4D

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Aux.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.26  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**
**	Revision 1.25  2003/01/06 19:16:03  sm
**	- Removed use of b3TriangleRef into an b3Array<b3_index>.
**	- Camera transformations are now matrix transformations.
**	
**	Revision 1.24  2002/08/17 17:31:22  sm
**	- Introduced animation support (Puh!)
**	
**	Revision 1.23  2002/03/10 13:55:15  sm
**	- Added creation dialog for rotation shapes.
**	- Cleaned up derivation of b3SplineRotShape.
**	- Added support for foreign BLZ3_HOME directories.
**	
**	Revision 1.22  2002/03/05 20:38:25  sm
**	- Added first profile (beveled spline shape).
**	- Added some features to b3SplineTemplate class.
**	- Added simple control to display 2 dimensional spline.
**	- Fine tuned the profile dialogs.
**	
**	Revision 1.21  2002/03/02 19:52:39  sm
**	- Nasty UnCR
**	- Fixed some compile bugs due to incompatibilities to Visual C++
**	
**	Revision 1.20  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.19  2002/02/12 18:39:03  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.18  2002/01/20 12:48:51  sm
**	- Added splash screen
**	- Corrected repeat buttons (capture change)
**	
**	Revision 1.17  2001/11/25 19:20:32  sm
**	- Added new acting methods:
**	  o Camera move
**	  o Camera turn around itself
**	  o Camera rotate around fulcrum
**	
**	Revision 1.16  2001/10/19 18:27:28  sm
**	- Fixing LDC bug
**	- Optimizing color routines
**	
**	Revision 1.15  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.14  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**	Revision 1.13  2001/10/05 20:30:45  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**	
**	Revision 1.12  2001/09/30 16:27:48  sm
**	- Raytracing with diffuse color without shading
**	- Sphere intersection fixed (now using normalized rays)
**	
**	Revision 1.11  2001/09/23 18:50:27  sm
**	- Created first raytracing image with Blizzard III. It shows
**	  simply "hit" or "no hit". Spheres and boxes aren't running
**	  yet. Next step: shading!
**	
**	Revision 1.10  2001/09/22 16:19:51  sm
**	- Adding basic shape intersection routines
**	
**	Revision 1.9  2001/09/04 20:37:53  sm
**	- Some minor updates
**	
**	Revision 1.8  2001/09/04 15:15:57  sm
**	- Added rotating objects
**	
**	Revision 1.7  2001/08/10 15:14:36  sm
**	- Now having all shapes implemented for drawing lines.
**	
**	Revision 1.6  2001/08/05 19:53:43  sm
**	- Removing some nasty CR/LF
**	
**	Revision 1.5  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**	Revision 1.4  2001/08/05 09:23:22  sm
**	- Introducing vectors, matrices, Splines and NURBS
**	
**
*/

/*************************************************************************
**                                                                      **
**                        variables                                     **
**                                                                      **
*************************************************************************/

b3_matrix b3Matrix::m_UnitMatrix =
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

b3_vector b3Matrix::m_EmptyVector =
{
	0,0,0,1
};

typedef b3_f32 b3_matrix_array[4][4];

b3_bool b3Matrix::b3NormalizeCol (
	b3_matrix *Matrix,
	b3_count   col)
{
	b3_f32  *column;
	b3_f64   Denom;
	b3_index i,index;

	column = &Matrix->m11;

	Denom  = 0;
	index  = col;
	for (i = 0;i < 4;i++)
	{
		Denom += (column[index] * column[index]);
		index += 4;
	}
	
	if (Denom != 0)
	{
		Denom = 1.0 / sqrt(Denom);
		index = col;
		for (i = 0;i < 4;i++)
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

b3_bool b3Matrix::b3NormalizeRow (
	b3_matrix *Matrix,
	b3_count   row)
{
	b3_f32  *column;
	b3_f64   Denom;
	b3_index i,index;

	column = &Matrix->m11;

	Denom  = 0;
	index  = row << 2;
	for (i = 0;i < 4;i++)
	{
		Denom += (column[index] * column[index]);
		index++;
	}
	
	if (Denom != 0)
	{
		Denom = 1.0 / sqrt(Denom);
		index = row << 2;
		for (i = 0;i < 4;i++)
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

b3_f64 b3Matrix::b3Det4(b3_matrix *Matrix)
{
	b3_vector Row1,Row2,Row3,Row4;
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

	Result  = Matrix->m11 * b3Det3 (&Row2,&Row3,&Row4);
	Result -= Matrix->m12 * b3Det3 (&Row1,&Row3,&Row4);
	Result += Matrix->m13 * b3Det3 (&Row1,&Row2,&Row4);
	Result -= Matrix->m14 * b3Det3 (&Row1,&Row2,&Row3);

	return (Result);
}

b3_matrix * b3Matrix::b3Unit (b3_matrix *Matrix)
{
	memcpy(Matrix,&m_UnitMatrix,sizeof(b3_matrix));
	return Matrix;
}

b3_matrix * b3Matrix::b3Transport (
	b3_matrix *From,
	b3_matrix *To)
{
	To->m11 = From->m11; To->m21 = From->m12; To->m31 = From->m13; To->m41 = From->m14;
	To->m12 = From->m21; To->m22 = From->m22; To->m32 = From->m23; To->m42 = From->m24;
	To->m13 = From->m31; To->m23 = From->m32; To->m33 = From->m33; To->m43 = From->m34;
	To->m14 = From->m41; To->m24 = From->m42; To->m34 = From->m43; To->m44 = From->m44;

	return (To);
}

b3_matrix * b3Matrix::b3Inverse (
	b3_matrix *From,
	b3_matrix *To)
{
#	ifdef MATRIX_INV_4D
		b3_f64    Denom;
		b3_vector Row1,Row2,Row3,Row4;

		Denom = b3Det4 (From);
		if (Denom == 0)
		{
			return (null);
		}
		Denom = 1 /Denom;

		Row1.x  = From->m12;	Row2.x  = From->m22;
		Row1.y  = From->m13;	Row2.y  = From->m23;
		Row1.z  = From->m14;	Row2.z  = From->m24;
  
		Row3.x  = From->m32;	Row4.x  = From->m42;
		Row3.y  = From->m33;	Row4.y  = From->m43;
		Row3.z  = From->m34;	Row4.z  = From->m44;
  
		/* inverting first line */
		To->m11 =   (b3_f32)(b3Det3 (&Row2,&Row3,&Row4) * Denom);
		To->m12 = - (b3_f32)(b3Det3 (&Row1,&Row3,&Row4) * Denom);
		To->m13 =   (b3_f32)(b3Det3 (&Row1,&Row2,&Row4) * Denom);
		To->m14 = - (b3_f32)(b3Det3 (&Row1,&Row2,&Row3) * Denom);

		/* inverting second line */
		Row1.x  =   From->m11;
		Row2.x  =   From->m21;
		Row3.x  =   From->m31;
		Row4.x  =   From->m41;
		To->m21 = - (b3_f32)(b3Det3 (&Row2,&Row3,&Row4) * Denom);
		To->m22 =   (b3_f32)(b3Det3 (&Row1,&Row3,&Row4) * Denom);
		To->m23 = - (b3_f32)(b3Det3 (&Row1,&Row2,&Row4) * Denom);
		To->m24 =   (b3_f32)(b3Det3 (&Row1,&Row2,&Row3) * Denom);

		/* inverting third line */
		Row1.y  =   From->m12;
		Row2.y  =   From->m22;
		Row3.y  =   From->m32;
		Row4.y  =   From->m42;
		To->m31 =   (b3_f32)(b3Det3 (&Row2,&Row3,&Row4) * Denom);
		To->m32 = - (b3_f32)(b3Det3 (&Row1,&Row3,&Row4) * Denom);
		To->m33 =   (b3_f32)(b3Det3 (&Row1,&Row2,&Row4) * Denom);
		To->m34 = - (b3_f32)(b3Det3 (&Row1,&Row2,&Row3) * Denom);

		/* inverting third fourth */
		Row1.z  =   From->m13;
		Row2.z  =   From->m23;
		Row3.z  =   From->m33;
		Row4.z  =   From->m43;
		To->m41 = - (b3_f32)(b3Det3 (&Row2,&Row3,&Row4) * Denom);
		To->m42 =   (b3_f32)(b3Det3 (&Row1,&Row3,&Row4) * Denom);
		To->m43 = - (b3_f32)(b3Det3 (&Row1,&Row2,&Row4) * Denom);
		To->m44 =   (b3_f32)(b3Det3 (&Row1,&Row2,&Row3) * Denom);
#	else
		b3_f64    Denom;
		b3_vector Row1,Row2,Row3;

		Row1.x  = From->m11;
		Row1.y  = From->m21;
		Row1.z  = From->m31;

		Row2.x  = From->m12;
		Row2.y  = From->m22;
		Row2.z  = From->m32;
  
		Row3.x  = From->m13;
		Row3.y  = From->m23;
		Row3.z  = From->m33;

		Denom = Det3 (&Row1,&Row2,&Row3);
		if (Denom == 0)
		{
			return null;
		}
		Denom = 1 / Denom;

		To->m11 =  (b3_f32)(b3Det2 (From->m22,From->m23,From->m32,From->m33) * Denom);
		To->m12 = -(b3_f32)(b3Det2 (From->m12,From->m13,From->m32,From->m33) * Denom);
		To->m13 =  (b3_f32)(b3Det2 (From->m12,From->m13,From->m22,From->m23) * Denom);
		To->m14 = -From->m14;

		To->m21 = -(b3_f32)(b3Det2 (From->m21,From->m23,From->m31,From->m33) * Denom);
		To->m22 =  (b3_f32)(b3Det2 (From->m11,From->m13,From->m31,From->m33) * Denom);
		To->m23 = -(b3_f32)(b3Det2 (From->m11,From->m13,From->m21,From->m23) * Denom);
		To->m24 = -From->m24;

		To->m31 =  (b3_f32)(b3Det2 (From->m21,From->m22,From->m31,From->m32) * Denom);
		To->m32 = -(b3_f32)(b3Det2 (From->m11,From->m12,From->m31,From->m32) * Denom);
		To->m33 =  (b3_f32)(b3Det2 (From->m11,From->m12,From->m21,From->m22) * Denom);
		To->m34 = -From->m34;

		To->m41 =
		To->m42 =
		To->m43 =  0;
		To->m44 =  1;
#	endif

	return (To);
}

b3_matrix * b3Matrix::b3MMul (
	b3_matrix	*B,
	b3_matrix	*A,
	b3_matrix	*C)
{
	b3_matrix	 Result;

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
	return (C);
}

b3_matrix * b3Matrix::b3SMul (
	b3_matrix *A,
	b3_matrix *B,
	b3_f64     Value)
{
	B->m11 = (b3_f32)(A->m11 * Value);
	B->m12 = (b3_f32)(A->m12 * Value);
	B->m13 = (b3_f32)(A->m13 * Value);
	B->m14 = (b3_f32)(A->m14 * Value);

	B->m21 = (b3_f32)(A->m21 * Value);
	B->m22 = (b3_f32)(A->m22 * Value);
	B->m23 = (b3_f32)(A->m23 * Value);
	B->m24 = (b3_f32)(A->m24 * Value);

	B->m31 = (b3_f32)(A->m31 * Value);
	B->m32 = (b3_f32)(A->m32 * Value);
	B->m33 = (b3_f32)(A->m33 * Value);
	B->m34 = (b3_f32)(A->m34 * Value);

	B->m41 = (b3_f32)(A->m41 * Value);
	B->m42 = (b3_f32)(A->m42 * Value);
	B->m43 = (b3_f32)(A->m43 * Value);
	B->m44 = (b3_f32)(A->m44 * Value);

	return (B);
}

b3_matrix * b3Matrix::b3MAdd (
	b3_matrix *A,
	b3_matrix *B,
	b3_matrix *C)
{
	C->m11 = A->m11 + B->m11;
	C->m12 = A->m12 + B->m12;
	C->m13 = A->m13 + B->m13;
	C->m14 = A->m14 + B->m14;

	C->m21 = A->m21 + B->m21;
	C->m22 = A->m22 + B->m22;
	C->m23 = A->m23 + B->m23;
	C->m24 = A->m24 + B->m24;

	C->m31 = A->m31 + B->m31;
	C->m32 = A->m32 + B->m32;
	C->m33 = A->m33 + B->m33;
	C->m34 = A->m34 + B->m34;

	C->m41 = A->m41 + B->m41;
	C->m42 = A->m42 + B->m42;
	C->m43 = A->m43 + B->m43;
	C->m44 = A->m44 + B->m44;

	return C;
}

b3_matrix * b3Matrix::b3Move (
	b3_matrix *A,
	b3_matrix *B,
	b3_f64     x,
	b3_f64     y,
	b3_f64     z)
{
	b3_vector move;

	b3Vector::b3Init(&move,x,y,z);
	return b3Move(A,B,&move);
}

b3_matrix * b3Matrix::b3Move (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Translation)
{
	b3_matrix Move;

	if (A == null)
	{
		A = &m_UnitMatrix;
	}

	Move     =  m_UnitMatrix;
	Move.m14 =  Translation->x;
	Move.m24 =  Translation->y;
	Move.m34 =  Translation->z;

	return b3MMul (A,&Move,B);
}

b3_matrix * b3Matrix::b3MoveNegative (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Translation)
{
	b3_matrix Move;

	if (A == null)
	{
		A = &m_UnitMatrix;
	}

	Move     =   m_UnitMatrix;
	Move.m14 = - (Translation->x);
	Move.m24 = - (Translation->y);
	Move.m34 = - (Translation->z);

	return b3MMul (A,&Move,B);
}

b3_matrix * b3Matrix::b3Scale (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Center,
	b3_f64     x,
	b3_f64     y,
	b3_f64     z)
{
	b3_vector scale;

	b3Vector::b3Init(&scale,x,y,z);
	return b3Scale(A,B,Center,&scale);
}

b3_matrix * b3Matrix::b3Scale (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Center,
	b3_vector *Scale)
{
	b3_matrix Operator;

	if (Center == null)
	{
		Center = &m_EmptyVector;
	}

	Operator     = m_UnitMatrix;
	Operator.m11 = Scale->x;
	Operator.m22 = Scale->y;
	Operator.m33 = Scale->z;

	b3MoveNegative (A,B,Center);
	b3MMul    (B,&Operator,B);

	return b3Move (B,B,Center);
}

b3_matrix * b3Matrix::b3RotateX (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Center,
	b3_f64     Angle)
{
	b3_matrix Result,CenterMatrix;
	b3_f32    Cos,Sin;

	if (A == null)
	{
		A = &m_UnitMatrix;
	}
	if (Center == null)
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

	b3MoveNegative (A,&CenterMatrix,Center);
	b3MMul    (&CenterMatrix,&Result,&Result);
	return b3Move (&Result,B,Center);
}

b3_matrix * b3Matrix::b3RotateY (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Center,
	b3_f64     Angle)
{
	b3_matrix Result,CenterMatrix;
	b3_f32    Cos,Sin;

	if (A == null)
	{
		A = &m_UnitMatrix;
	}
	if (Center == null)
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

	b3MoveNegative (A,&CenterMatrix,Center);
	b3MMul    (&CenterMatrix,&Result,&Result);
	return b3Move (&Result,B,Center);
}

b3_matrix *b3Matrix::b3RotateZ (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Center,
	b3_f64     Angle)
{
	b3_matrix Result,CenterMatrix;
	b3_f32    Cos,Sin;

	if (A == null)
	{
		A = &m_UnitMatrix;
	}
	if (Center == null)
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

	b3MoveNegative (A,&CenterMatrix,Center);
	b3MMul    (&CenterMatrix,&Result,&Result);
	return b3Move (&Result,B,Center);
}

b3_matrix *b3Matrix::b3Align (
	b3_matrix     *result,
	const b3_line *Axis)
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

	b3NormalizeCol (result,0);
	b3NormalizeCol (result,1);

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

b3_matrix *b3Matrix::b3RotateVector (
	b3_matrix *A,
	b3_matrix *B,
	b3_line   *Axis,
	b3_f64     Angle)
{
	b3_f32    Cos,Sin;
	b3_matrix System,InvSystem,Result,Rotate;

	if (A == null)
	{
		A = &m_UnitMatrix;
	}

	b3Align(&System,Axis);

	if (b3Inverse (&System,&InvSystem))
	{
		Rotate     =  m_UnitMatrix;
		Cos        =  (b3_f32)cos(Angle);
		Sin        =  (b3_f32)sin(Angle);
		Rotate.m22 =  Cos;
		Rotate.m32 =  Sin;
		Rotate.m23 = -Sin;
		Rotate.m33 =  Cos;

		b3MMul (A,      &InvSystem, B);
		b3MMul (B,      &Rotate,   &Result);
		b3MMul (&Result,&System,    B);
	}
	else
	{
		B = null;
	}

	return B;
}

b3_matrix *b3Matrix::b3MirrorPoint (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Center,
	b3_f64     a)
{
	b3_matrix Mirror,Mirrored;

	if (A == null)
	{
		A = &m_UnitMatrix;
	}
	if (Center == null)
	{
		Center = &m_EmptyVector;
	}

	b3MoveNegative (A,B,Center);

	Mirror     = m_UnitMatrix;
	Mirror.m11 = -(b3_f32)a;
	Mirror.m22 = -(b3_f32)a;
	Mirror.m33 = -(b3_f32)a;
	b3MMul (B,&Mirror,&Mirrored);
	b3Move (&Mirrored,B,Center);

	return B;
}

b3_matrix *b3Matrix::b3MirrorAxis (
	b3_matrix *A,
	b3_matrix *B,
	b3_line   *Axis,
	b3_f64     a)
{
	b3_matrix Mirror,Mirrored,Result;
	b3_matrix System,InvSystem;

	if (A == null)
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

	b3NormalizeCol (&System,0);
	b3NormalizeCol (&System,1);

	System.m13 = System.m21 * System.m32 - System.m31 * System.m22;
	System.m23 = System.m31 * System.m12 - System.m11 * System.m32;
	System.m33 = System.m11 * System.m22 - System.m21 * System.m12;
	System.m43 = 0;

	System.m14 = Axis->pos.x;
	System.m24 = Axis->pos.y;
	System.m34 = Axis->pos.z;
	System.m44 = 1;

	if (!b3Inverse (&System,&InvSystem))
	{
		return null;
	}

	Mirror = m_UnitMatrix;
	Mirror.m22 = -(b3_f32)a;
	Mirror.m33 = -(b3_f32)a;

	b3MMul (A,        &InvSystem,&Result);
	b3MMul (&Result,  &Mirror,   &Mirrored);
	b3MMul (&Mirrored,&System,    B);
	return B;
}

b3_matrix *b3Matrix::b3MirrorPlane (
	b3_matrix *A,
	b3_matrix *B,
	b3_vector *Base,
	b3_vector *Dir1,
	b3_vector *Dir2,
	b3_f64     a)
{
	b3_matrix Mirror,Mirrored,Result;
	b3_matrix System,InvSystem;

	if (A == null)
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

	b3NormalizeCol (&System,0);
	b3NormalizeCol (&System,1);

	System.m13 = System.m21 * System.m32 - System.m31 * System.m22;
	System.m23 = System.m31 * System.m12 - System.m11 * System.m32;
	System.m33 = System.m11 * System.m22 - System.m21 * System.m12;
	System.m43 = 0;

	System.m14 = Base->x;
	System.m24 = Base->y;
	System.m34 = Base->z;
	System.m44 = 1;

	if (!b3Inverse (&System,&InvSystem))
	{
		return null;
	}

	Mirror     = m_UnitMatrix;
	Mirror.m33 = -(b3_f32)a;

	b3MMul (A,        &InvSystem,&Result);
	b3MMul (&Result,  &Mirror,   &Mirrored);
	b3MMul (&Mirrored,&System,   B);
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

b3_matrix * b3Matrix::b3Dress (
	b3_matrix *prev,
	b3_matrix *transform,
	b3_vector *center,
	b3_vector *dir1,
	b3_vector *dir2,
	b3_bool    negate)
{
	b3_matrix lookTo;
	b3_vector normal;

	// dress vector is x axis
	lookTo.m11 = dir1->x;
	lookTo.m21 = dir1->y;
	lookTo.m31 = dir1->z;
	lookTo.m41 = 0;
	b3NormalizeCol (&lookTo,0);

	// now compute z axis from dress vector and direction vector
	normal.x = dir1->y * dir2->z    - dir1->z * dir2->y; 
	normal.y = dir1->z * dir2->x    - dir1->x * dir2->z; 
	normal.z = dir1->x * dir2->y    - dir1->y * dir2->x; 

	if (negate)
	{
		b3Vector::b3Negate(&normal);
	}
	lookTo.m13 = normal.x;
	lookTo.m23 = normal.y;
	lookTo.m33 = normal.z;
	lookTo.m43 = 0;
	b3NormalizeCol (&lookTo,2);

	// now compute y axis from x and z axis
	lookTo.m12 = dir1->y * lookTo.m33 - dir1->z * lookTo.m23; 
	lookTo.m22 = dir1->z * lookTo.m13 - dir1->x * lookTo.m33; 
	lookTo.m32 = dir1->x * lookTo.m23 - dir1->y * lookTo.m13; 
	lookTo.m42 = 0;
	b3NormalizeCol (&lookTo,1);

	// reposition object
	lookTo.m14 = center->x;
	lookTo.m24 = center->y;
	lookTo.m34 = center->z;
	lookTo.m44 = 1;

	// "undo" from transformation, "do" to transformation
	b3MMul (prev,&lookTo,transform);
	return transform;
}
