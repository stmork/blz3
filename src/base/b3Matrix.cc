/*
**
**	$Filename:	b3Matrix.c $
**	$Release:	Dortmund 2001 $
**	$Revision: 2.02 
**	$Date:		05-Jun-1998 
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - vector and matrix computation
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#if 0
#ifndef BLIZZARD_MATRIX_C
#define BLIZZARD_MATRIX_C

#define MATRIX_INV_4D

/*************************************************************************
**                                                                      **
**                        Blizzard includes                             **
**                                                                      **
*************************************************************************/

#include <blizzard/bsystem.h>
#include <blizzard/braytrace.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**
**	Revision 1.2  2001/07/01 16:31:51  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**	
**	Revision 1.1.1.1  2001/07/01 12:24:58  sm
**	Blizzard III is born
**	
*/

/*************************************************************************
**                                                                      **
**                        variables                                     **
**                                                                      **
*************************************************************************/

static matrix UnitMatrix =
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

typedef float matrixarray[4][4];

static bool NormalizeCol (Matrix,col)

register matrixarray	 Matrix;
register long			 col;

{
	register double	 Denom;
	register long	 i;

	Denom = 0;
	for (i=0;i<4;i++) Denom += (Matrix[i][col] * Matrix[i][col]);
	
	if (Denom == 0) return (false);
	Denom = 1 / sqrt(Denom);
	for (i=0;i<4;i++) Matrix[i][col] *= Denom;
	return (true);
}

/*************************************************************************
**                                                                      **
**                           global part                                **
**                                                                      **
*************************************************************************/

double Length (register vector *Vector)
{
	return (sqrt(
		Vector->x * Vector->x +
		Vector->y * Vector->y +
		Vector->z * Vector->z));
}

double Det2(
	register double a,
	register double b,
	register double c,
	register double d)
{
	return (a*d-b*c);
}

double Det3(
	vector *a,
	vector *b,
	vector *c)
{
	return (a->x * b->y * c->z -
			c->x * b->y * a->z +
			b->x * c->y * a->z -
			b->x * a->y * c->z +
			c->x * a->y * b->z -
			a->x * c->y * b->z);
}

double Det4(register matrix *Matrix)
{
	vector	 Row1,Row2,Row3,Row4;
	double	 Result;

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

	Result  = Matrix->m11 * Det3 (&Row2,&Row3,&Row4);
	Result -= Matrix->m12 * Det3 (&Row1,&Row3,&Row4);
	Result += Matrix->m13 * Det3 (&Row1,&Row2,&Row4);
	Result -= Matrix->m14 * Det3 (&Row1,&Row2,&Row3);

	return (Result);
}

matrix * MatrixUnit (register matrix *Matrix)
{
	*Matrix = UnitMatrix;
	return (Matrix);
}

matrix * MatrixTrans (
	register matrix *From,
	register matrix *To)
{
	To->m11 = From->m11; To->m21 = From->m12; To->m31 = From->m13; To->m41 = From->m14;
	To->m12 = From->m21; To->m22 = From->m22; To->m32 = From->m23; To->m42 = From->m24;
	To->m13 = From->m31; To->m23 = From->m32; To->m33 = From->m33; To->m43 = From->m34;
	To->m14 = From->m41; To->m24 = From->m42; To->m34 = From->m43; To->m44 = From->m44;

	return (To);
}

matrix * MatrixInv (
	register matrix *From,
	register matrix *To)
{
#	ifdef MATRIX_INV_4D
		double	 Denom;
		vector	 Row1,Row2,Row3,Row4;

		Denom = Det4 (From);
		if (Denom == 0) return (null);
		Denom = 1 /Denom;

		Row1.x  = From->m12;	Row2.x  = From->m22;
		Row1.y  = From->m13;	Row2.y  = From->m23;
		Row1.z  = From->m14;	Row2.z  = From->m24;
  
		Row3.x  = From->m32;	Row4.x  = From->m42;
		Row3.y  = From->m33;	Row4.y  = From->m43;
		Row3.z  = From->m34;	Row4.z  = From->m44;
  
		/* inverting first line */
		To->m11 =   Det3 (&Row2,&Row3,&Row4) * Denom;
		To->m12 = - Det3 (&Row1,&Row3,&Row4) * Denom;
		To->m13 =   Det3 (&Row1,&Row2,&Row4) * Denom;
		To->m14 = - Det3 (&Row1,&Row2,&Row3) * Denom;

		/* inverting second line */
		Row1.x  =   From->m11;
		Row2.x  =   From->m21;
		Row3.x  =   From->m31;
		Row4.x  =   From->m41;
		To->m21 = - Det3 (&Row2,&Row3,&Row4) * Denom;
		To->m22 =   Det3 (&Row1,&Row3,&Row4) * Denom;
		To->m23 = - Det3 (&Row1,&Row2,&Row4) * Denom;
		To->m24 =   Det3 (&Row1,&Row2,&Row3) * Denom;

		/* inverting third line */
		Row1.y  =   From->m12;
		Row2.y  =   From->m22;
		Row3.y  =   From->m32;
		Row4.y  =   From->m42;
		To->m31 =   Det3 (&Row2,&Row3,&Row4) * Denom;
		To->m32 = - Det3 (&Row1,&Row3,&Row4) * Denom;
		To->m33 =   Det3 (&Row1,&Row2,&Row4) * Denom;
		To->m34 = - Det3 (&Row1,&Row2,&Row3) * Denom;

		/* inverting third fourth */
		Row1.z  =   From->m13;
		Row2.z  =   From->m23;
		Row3.z  =   From->m33;
		Row4.z  =   From->m43;
		To->m41 = - Det3 (&Row2,&Row3,&Row4) * Denom;
		To->m42 =   Det3 (&Row1,&Row3,&Row4) * Denom;
		To->m43 = - Det3 (&Row1,&Row2,&Row4) * Denom;
		To->m44 =   Det3 (&Row1,&Row2,&Row3) * Denom;
#	else
		double	 Denom;
		vector	 Row1,Row2,Row3;

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
		if (Denom == 0) return (null);
		Denom = 1 / Denom;

		To->m11 =  Det2 (From->m22,From->m23,From->m32,From->m33) * Denom;
		To->m12 = -Det2 (From->m12,From->m13,From->m32,From->m33) * Denom;
		To->m13 =  Det2 (From->m12,From->m13,From->m22,From->m23) * Denom;
		To->m14 = -From->m14;

		To->m21 = -Det2 (From->m21,From->m23,From->m31,From->m33) * Denom;
		To->m22 =  Det2 (From->m11,From->m13,From->m31,From->m33) * Denom;
		To->m23 = -Det2 (From->m11,From->m13,From->m21,From->m23) * Denom;
		To->m24 = -From->m24;

		To->m31 =  Det2 (From->m21,From->m22,From->m31,From->m32) * Denom;
		To->m32 = -Det2 (From->m11,From->m12,From->m31,From->m32) * Denom;
		To->m33 =  Det2 (From->m11,From->m12,From->m21,From->m22) * Denom;
		To->m34 = -From->m34;

		To->m41 =
		To->m42 =
		To->m43 =  0;
		To->m44 =  1;
#	endif

	return (To);
}

matrix * MatrixMMul (
	register matrix	*B,
	register matrix	*A,
	register matrix	*C)
{
	matrix	 Result;

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

matrix * MatrixSMul (
	register matrix	*A,
	register matrix *B,
	register double	 Value)
{
	B->m11 = A->m11 * Value;
	B->m12 = A->m12 * Value;
	B->m13 = A->m13 * Value;
	B->m14 = A->m14 * Value;

	B->m21 = A->m21 * Value;
	B->m22 = A->m22 * Value;
	B->m23 = A->m23 * Value;
	B->m24 = A->m24 * Value;

	B->m31 = A->m31 * Value;
	B->m32 = A->m32 * Value;
	B->m33 = A->m33 * Value;
	B->m34 = A->m34 * Value;

	B->m41 = A->m41 * Value;
	B->m42 = A->m42 * Value;
	B->m43 = A->m43 * Value;
	B->m44 = A->m44 * Value;

	return (B);
}

vector * MatrixVMul (
	register matrix	*A,
	register vector	*V1,
	register vector	*V2,
	register bool	 FourDim)
{
	vector	 Result;

	if (FourDim)
	{
		Result.x = A->m11 * V1->x + A->m12 * V1->y + A->m13 * V1->z + A->m14;
		Result.y = A->m21 * V1->x + A->m22 * V1->y + A->m23 * V1->z + A->m24;
		Result.z = A->m31 * V1->x + A->m32 * V1->y + A->m33 * V1->z + A->m34;
	}
	else
	{
		Result.x = A->m11 * V1->x + A->m12 * V1->y + A->m13 * V1->z;
		Result.y = A->m21 * V1->x + A->m22 * V1->y + A->m23 * V1->z;
		Result.z = A->m31 * V1->x + A->m32 * V1->y + A->m33 * V1->z;
	}
	*V2 = Result;

	return (V2);
}

matrix * MatrixMAdd (
	register matrix	*A,
	register matrix	*B,
	register matrix	*C)
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

	return (C);
}

matrix * MatrixMove (
	register matrix	*A,
	         matrix *B,
	register vector	*Translation)
{
	matrix Move;

	if (A == null) A = &UnitMatrix;

	Move     =  UnitMatrix;
	Move.m14 =  Translation->x;
	Move.m24 =  Translation->y;
	Move.m34 =  Translation->z;

	return MatrixMMul (A,&Move,B);
}

matrix * MatrixMoveNeg (
	register matrix	*A,
	         matrix *B,
	register vector	*Translation)
{
	matrix Move;

	if (A == null) A = &UnitMatrix;

	Move     =   UnitMatrix;
	Move.m14 = - (Translation->x);
	Move.m24 = - (Translation->y);
	Move.m34 = - (Translation->z);

	return MatrixMMul (A,&Move,B);
}

matrix * MatrixScale (
	register matrix	*A,
	register matrix	*B,
	register vector	*Center,
	register vector	*Scale)
{
	matrix Operator;

	Operator     = UnitMatrix;
	Operator.m11 = Scale->x;
	Operator.m22 = Scale->y;
	Operator.m33 = Scale->z;

	MatrixMoveNeg (A,B,Center);
	MatrixMMul    (B,&Operator,B);

	return MatrixMove (B,B,Center);
}

matrix * MatrixRotX (
	register matrix	*A,
	register matrix	*B,
	register vector	*Center,
	register double	 Angle)
{
	         matrix	 Result,CenterMatrix;
	register float	 Cos,Sin;

	Cos = cos(Angle);
	Sin = sin(Angle);

	Result     =  UnitMatrix;
	Result.m22 =  Cos;
	Result.m32 =  Sin;
	Result.m23 = -Sin;
	Result.m33 =  Cos;

	MatrixMoveNeg (A,&CenterMatrix,Center);
	MatrixMMul    (&CenterMatrix,&Result,&Result);
	return MatrixMove (&Result,B,Center);
}

matrix * MatrixRotY (
	register matrix	*A,
	register matrix	*B,
	register vector	*Center,
	register double	 Angle)
{
	         matrix	 Result,CenterMatrix;
	register float	 Cos,Sin;

	Cos = cos(Angle);
	Sin = sin(Angle);

	Result     =  UnitMatrix;
	Result.m11 =  Cos;
	Result.m31 = -Sin;
	Result.m13 =  Sin;
	Result.m33 =  Cos;

	MatrixMoveNeg (A,&CenterMatrix,Center);
	MatrixMMul    (&CenterMatrix,&Result,&Result);
	return MatrixMove (&Result,B,Center);
}

matrix * MatrixRotZ (
	register matrix	*A,
	register matrix	*B,
	register vector	*Center,
	register double	 Angle)
{
	         matrix	 Result,CenterMatrix;
	register float	 Cos,Sin;

	if (A == null) A = &UnitMatrix;
	Cos = cos(Angle);
	Sin = sin(Angle);

	Result     =  UnitMatrix;
	Result.m11 =  Cos;
	Result.m21 =  Sin;
	Result.m12 = -Sin;
	Result.m22 =  Cos;

	MatrixMoveNeg (A,&CenterMatrix,Center);
	MatrixMMul    (&CenterMatrix,&Result,&Result);
	return MatrixMove (&Result,B,Center);
}

matrix * MatrixRotVec (
	register matrix      *A,
	register matrix      *B,
	register struct Line *Axis,
	register double       Angle)
{
	register double	 Cos,Sin;
	         matrix	 System,InvSystem,Result,Rotate;

	if (A == null) A = &UnitMatrix;

	System.m11 = Axis->Dir.x;
	System.m21 = Axis->Dir.y;
	System.m31 = Axis->Dir.z;
	System.m41 = 0;

	if (Axis->Dir.x == 0 && Axis->Dir.y == 0)
	{
		System.m12 = 0;
		System.m22 = Axis->Dir.z;
		System.m32 = 0;
		System.m42 = 0;
	}
	else
	{
		System.m12 = -Axis->Dir.y;
		System.m22 =  Axis->Dir.x;
		System.m32 =  0;
		System.m42 =  0;
	}

	NormalizeCol (&System,0L);
	NormalizeCol (&System,1L);

	System.m13 =  System.m21 * System.m32 - System.m31 * System.m22;
	System.m23 =  System.m31 * System.m12 - System.m11 * System.m32;
	System.m33 =  System.m11 * System.m22 - System.m21 * System.m12;
	System.m43 =  0;

	System.m14 =  Axis->Pos.x;
	System.m24 =  Axis->Pos.y;
	System.m34 =  Axis->Pos.z;
	System.m44 =  1;

	if (!MatrixInv (&System,&InvSystem)) return (null);

	Rotate     =  UnitMatrix;
	Rotate.m22 =  Cos = cos(Angle);
	Rotate.m32 =  Sin = sin(Angle);
	Rotate.m23 = -Sin;
	Rotate.m33 =  Cos;

	MatrixMMul (A,      &InvSystem, B);
	MatrixMMul (B,      &Rotate,   &Result);
	MatrixMMul (&Result,&System,    B);

	return B;
}

matrix *MatrixMirrorPoint (
	register matrix *A,
	register matrix *B,
	register vector *Center,
	double           a)
{
	matrix	 Mirror,Mirrored;

	if (A == null) A = &UnitMatrix;

	MatrixMoveNeg (A,B,Center);

	Mirror     = UnitMatrix;
	Mirror.m11 = -a;
	Mirror.m22 = -a;
	Mirror.m33 = -a;
	MatrixMMul (B,&Mirror,&Mirrored);
	MatrixMove (&Mirrored,B,Center);

	return B;
}

matrix *MatrixMirrorAxis (
	register matrix *A,
	register matrix *B,
	register line   *Axis,
	register double	 a)
{
	matrix	 Mirror,Mirrored,Result;
	matrix	 System,InvSystem;

	if (A == null) A = &UnitMatrix;

	System.m11 = Axis->Dir.x;
	System.m21 = Axis->Dir.y;
	System.m31 = Axis->Dir.z;
	System.m41 = 0;

	if (Axis->Dir.x == 0 && Axis->Dir.y == 0)
	{
		System.m12 = 0;
		System.m22 = Axis->Dir.z;
		System.m32 = 0;
		System.m42 = 0;
	}
	else
	{
		System.m12 = -Axis->Dir.y;
		System.m22 =  Axis->Dir.x;
		System.m32 =  0;
		System.m42 =  0;
	}

	NormalizeCol (&System,0L);
	NormalizeCol (&System,1L);

	System.m13 = System.m21 * System.m32 - System.m31 * System.m22;
	System.m23 = System.m31 * System.m12 - System.m11 * System.m32;
	System.m33 = System.m11 * System.m22 - System.m21 * System.m12;
	System.m43 = 0;

	System.m14 = Axis->Pos.x;
	System.m24 = Axis->Pos.y;
	System.m34 = Axis->Pos.z;
	System.m44 = 1;

	if (!MatrixInv (&System,&InvSystem)) return null;

	Mirror = UnitMatrix;
	Mirror.m22 = -a;
	Mirror.m33 = -a;

	MatrixMMul (A,        &InvSystem,&Result);
	MatrixMMul (&Result,  &Mirror,   &Mirrored);
	MatrixMMul (&Mirrored,&System,    B);
	return B;
}

matrix *MatrixMirrorPlane (
	matrix *A,
	matrix *B,
	vector *Base,
	vector *Dir1,
	vector *Dir2,
	double  a)
{
	matrix	 Mirror,Mirrored,Result;
	matrix	 System,InvSystem;

	if (A == null) A = &UnitMatrix;

	System.m11 = Dir1->x;
	System.m21 = Dir1->y;
	System.m31 = Dir1->z;
	System.m41 = 0;

	System.m12 = Dir2->x;
	System.m22 = Dir2->y;
	System.m32 = Dir2->z;
	System.m42 = 0;

	NormalizeCol (&System,0L);
	NormalizeCol (&System,1L);

	System.m13 = System.m21 * System.m32 - System.m31 * System.m22;
	System.m23 = System.m31 * System.m12 - System.m11 * System.m32;
	System.m33 = System.m11 * System.m22 - System.m21 * System.m12;
	System.m43 = 0;

	System.m14 = Base->x;
	System.m24 = Base->y;
	System.m34 = Base->z;
	System.m44 = 1;

	if (!MatrixInv (&System,&InvSystem)) return null;

	Mirror     = UnitMatrix;
	Mirror.m33 = -a;

	MatrixMMul (A,        &InvSystem,&Result);
	MatrixMMul (&Result,  &Mirror,   &Mirrored);
	MatrixMMul (&Mirrored,&System,   B);
	return B;
}

#endif
'ENDIF
