/*
**
**	$Filename:	b3Cubic.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 2.02
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - vector and matrix methods
**
**	(C) Copyright 2001 - 2024  Steffen A. Mork
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

#include "b3BaseInclude.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3Polynom.h"
#include "blz3/base/b3Math.h"

/*************************************************************************
**                                                                      **
**                        b3Cubic implementation                        **
**                                                                      **
*************************************************************************/

enum Ord2 : int
{
	O2_X0 = 0,
	O2_X1
};

b3_count b3Polynom::b3SolveOrd2(const b3_f64 * Coeffs, b3_f64 * x)
{
	const b3_f64 p = Coeffs[O2_X1] * 0.5;    // x^1
	const b3_f64 D = p * p - Coeffs[O2_X0];  // x^0

	/* normal form: x^2 + px + q = 0 */

	if (b3Math::b3IsZero(D))
	{
		x[0] = -p;
		return 1;
	}
	else if (D > 0)
	{
		const b3_f64 r = sqrt(D);

		x[0] = -r - p;
		x[1] =  r - p;
		return 2;
	}
	else
	{
		return 0;
	}
}

enum Ord3 : int
{
	O3_X0 = 0,
	O3_X1,
	O3_X2
};

// https://www.e-education.psu.edu/png520/m11_p6.html
b3_count b3Polynom::b3SolveOrd3(const b3_f64 * Coeffs, b3_f64 * x)
{
	b3_count NumOfX = 0;

	/* normal form: x^3 + Ax^2 + Bx + C = 0 */

	const b3_f64 A   = Coeffs[O3_X2]; // x^2
	const b3_f64 B   = Coeffs[O3_X1]; // x^1
	const b3_f64 C   = Coeffs[O3_X0]; // x^0

	const b3_f64 AA  = A * A;
	const b3_f64 Q   = (AA - 3 * B) / 9.0;
	const b3_f64 R   = (2.0 * AA * A - 9 * A * B + 27.0 * C) / 54.0;

	const b3_f64 QQQ = Q * Q * Q;
	const b3_f64 M   = R * R - QQQ;

	if (b3Math::b3IsZero(M))
	{
		if (b3Math::b3IsZero(R))
		{
			x[NumOfX++] = 0;
		}
		else
		{
			const b3_f64 cbrt_q  = cbrt(-R);

			x[NumOfX++] = 2 * cbrt_q;
			x[NumOfX++] =   - cbrt_q;
		}
	}
	else if (M > 0)
	{
		const b3_f64 sqrt_M = sqrt(M);

		x[NumOfX++] = cbrt(- R + sqrt_M) + cbrt(-R - sqrt_M);
	}
	else
	{
		const b3_f64 theta = acos(R / sqrt(QQQ)) / 3.0;
		const b3_f64 aux   = 2.0 * sqrt(Q);

		x[NumOfX++] = aux * -cos(theta + 2.0 * M_PI / 3.0);
		x[NumOfX++] = aux * -cos(theta - 2.0 * M_PI / 3.0);
		x[NumOfX++] = aux * -cos(theta);
	}

	const b3_f64 aux = A / 3.0;
	for (b3_loop i = 0; i < NumOfX; ++i)
	{
		x[i] -= aux;
	}

	return NumOfX;
}

enum Ord4 : int
{
	O4_X0 = 0,
	O4_X1,
	O4_X2,
	O4_X3
};

// https://www.desmos.com/calculator?lang=de
b3_count b3Polynom::b3SolveOrd4(const b3_f64 * c, b3_f64 * x)
{
	b3_f64	 Coeffs[4];
	b3_f64	 D, C, B, A;
	b3_count NumOfX;

	/* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

	D = c[O4_X0]; // x^0
	C = c[O4_X1]; // x^1
	B = c[O4_X2]; // x^2
	A = c[O4_X3]; // x^3

	const b3_f64 AA = A * A;
	const b3_f64 p  = B  -  0.375 * AA;
	const b3_f64 q  =  0.125 * AA * A  -  0.5 * A * B   + C;
	const b3_f64 r  = -3.0 * AA * AA / 256.0 + 0.0625 * AA * B - 0.25 * A * C + D;

	if (b3Math::b3IsZero(r))
	{
		Coeffs[O3_X0] = q;
		Coeffs[O3_X1] = p;
		Coeffs[O3_X2] = 0;

		NumOfX = b3SolveOrd3(Coeffs, x);
		if (NumOfX < 3)
		{
			x[NumOfX++] = 0;
		}
	}
	else
	{
		Coeffs[O3_X0] =  0.5 * r * p - 0.125 * q * q;
		Coeffs[O3_X1] = -r;
		Coeffs[O3_X2] = -0.5 * p;

		b3SolveOrd3(Coeffs, x);

		B = x[0];
		C = B * B - r;
		D = 2 * B - p;

		if (b3Math::b3IsZero(C))
		{
			C = 0;
		}
		else if (C > 0)
		{
			C = sqrt(C);
		}
		else
		{
			return 0;
		}

		if (b3Math::b3IsZero(D))
		{
			D = 0;
		}
		else if (D > 0)
		{
			D = sqrt(D);
		}
		else
		{
			return 0;
		}

		Coeffs[O2_X0]  = B - C;
		Coeffs[O2_X1]  = q < 0 ? -D : D;
		NumOfX         = b3SolveOrd2(Coeffs, x);

		Coeffs[O2_X0]  = B + C;
		Coeffs[O2_X1]  = q < 0 ? D : -D;
		NumOfX        += b3SolveOrd2(Coeffs, &x[NumOfX]);

	}

	NumOfX = b3MakeUnique<b3_f64>(x, NumOfX);

	const b3_f64 aux = 0.25 * A;
	for (b3_loop i = 0; i < NumOfX; ++i)
	{
		x[i] -= aux;
	}
	return NumOfX;
}

b3_f64 b3Polynom::b3Horner(
	const b3_f64 * Coeffs,
	const b3_f64   x,
	const b3_count ord)
{
	b3_count o      = ord;
	b3_f64   result = 1;

	while (--o >= 0)
	{
		result *= x;
		result += Coeffs[o];
	}
	return result;
}
