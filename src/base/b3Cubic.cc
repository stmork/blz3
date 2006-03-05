/*
**
**	$Filename:	b3Cubic.cc $
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

#include "b3BaseInclude.h"
#include "blz3/base/b3Cubic.h"
#include "blz3/base/b3Math.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2006/03/05 21:22:33  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.3  2004/03/21 16:08:35  sm
**	- Moved b3Cbrt from b3Cubic into b3Math and made it inlined.
**	
**	Revision 1.2  2002/12/20 15:32:55  sm
**	- Made some ICC optimazations :-)
**	
**	Revision 1.1  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**
*/

#define b3IsZero(x) (fabs(x) < 4.656613e-10)

b3_count b3Cubic::b3SolveOrd2 (b3_f64 *Coeffs,b3_f64 *x)
{
	b3_f64 p,q,D;

    p = Coeffs[1] * 0.5;
    q = Coeffs[0];

    D = p * p - q;
    if (b3IsZero(D))
    {
		x[0] = -p;
		return 1;
    }
    else if (D > 0)
    {
    	q = sqrt(D);
    	x[0] = -q - p;
    	x[1] =  q - p;
		return 2;
    }
    else return 0;
}

b3_count b3Cubic::b3SolveOrd3(b3_f64 *Coeffs,b3_f64 *x)
{
	b3_count i,NumOfX;
	b3_f64   Sub,A,B,C;
	b3_f64   sq_A,p,q;
	b3_f64   cb_P,D;

    A = Coeffs[2];
    B = Coeffs[1];
    C = Coeffs[0];

    sq_A = A * A;
    p = 1.0/3 * (- 1.0/3  * sq_A     + B);
    q = 0.5   * (  2.0/27 * sq_A * A - 1.0/3 * A * B + C);

    cb_P = p * p * p;
    D    = q * q + cb_P;

    if (b3IsZero(D))
    {
		if (b3IsZero(q))
		{
			NumOfX = 0;
		}
		else
		{
	    	Sub  = b3Math::b3Cbrt(-q);
		    x[0] = 2 * Sub;
		    x[1] =   - Sub;
	    	NumOfX = 2;
		}
    }
    else if (D < 0)
 	{
		Sub = 1.0/3 * acos(-q / sqrt(-cb_P));
		C = 2 * sqrt(-p);

		x[0] = C *  cos(Sub);
		x[1] = C * -cos(Sub + M_PI / 3);
		x[2] = C * -cos(Sub - M_PI / 3);
		NumOfX = 3;
    }
    else
   	{
		Sub    = sqrt(D);
		x[0]   = b3Math::b3Cbrt(Sub - q) - b3Math::b3Cbrt(Sub + q);
		NumOfX = 1;
    }

    Sub = 1.0/3 * A;
    for (i = 0;i < NumOfX; ++i)
    {
		x[i] -= Sub;
	}

    return NumOfX;
}

b3_count b3Cubic::b3SolveOrd4(b3_f64 *c,b3_f64 *x)
{
	b3_f64	 Coeffs[4];
	b3_f64	 Sub,B,C,D,A;
	b3_f64	 sq_A,p,q,r;
	b3_count i,NumOfX;

    /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

    A = c[3];
    B = c[2];
    C = c[1];
    D = c[0];

    sq_A = A * A;
    p    = -0.375   * sq_A + B;
    q    =  0.125   * sq_A *    A - 0.5    *    A * B + C;
    r    = -3.0/256 * sq_A * sq_A + 0.0625 * sq_A * B - 0.25 * A * C + D;

    if (b3IsZero(r))
    {
		Coeffs[0] = q;
		Coeffs[1] = p;
		Coeffs[2] = 0;

		NumOfX = b3SolveOrd3(Coeffs,x);
    }
    else
    {
		Coeffs[0] =  0.5 * r * p - 0.125 * q * q;
		Coeffs[1] = -r;
		Coeffs[2] = -0.5 * p;

		b3SolveOrd3(Coeffs,x);

		B = x[0];
		C = B * B - r;
		D = 2 * B - p;

		if (b3IsZero(C))  C = 0;
		else if (C > 0) C = sqrt(C);
		else return 0;

		if (b3IsZero(D))  D = 0;
		else if (D > 0) D = sqrt(D);
		else return 0;

		Coeffs[0]  = B;
		Coeffs[0] -= C;		/* Compilerfehler Aztec 3.6a ! */
		Coeffs[1]  = q < 0 ? -D : D;
		NumOfX     = b3SolveOrd2(Coeffs,x);

		Coeffs[0]  = B + C;
		Coeffs[1]  = q < 0 ? D : -D;
		NumOfX    += b3SolveOrd2(Coeffs,&x[NumOfX]);
    }

    Sub = 0.25 * A;
    for (i = 0;i < NumOfX; ++i)
    {
		x[i] -= Sub;
	}
    return NumOfX;
}
