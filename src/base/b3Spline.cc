/*
**
**	$Filename:	b3Spline.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 2.02 
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Spline Routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        includes                                      **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Spline.h"

#define EPSILON 0.0001

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/01 12:24:58  sm
**	Initial revision
**
*/

b3_spline_error b3_spline::b3Errno()
{
	return errno;
}

/*************************************************************************
**                                                                      **
**                        de Boor routines                              **
**                                                                      **
*************************************************************************/


/* This routine finds the knot index where the de Boor algorithms start. */
/* The conditions is: knots[i] <= q < knots[i+1]. */
/* --------------------------------------------------------------------- */
/* knots:   knot vector */
/* q:       curve parameter */
/* KnotNum: length of knot vector */
/* closed:  open or closed curve */

long b3_spline::iFind(b3_f64 q)
{
	register long i;

	for (i=0;i < (knot_num-1);i++)
	{
		if ((knots[i] <= q) && (q < knots[i+1])) return (i);
	}
	if (!closed)
	{
		i--;
		while (knots[i-1] == knots[i]) i--;
	}
	return i;
}


/* The routine Mansfield computes the basis coefficents for the */
/* appropriate control points and returns the highest index of */
/* the corresponding control point. This routine is independend */
/* of the spline type (e.g. openend or closed). */
/* ------------------------------------------------------------ */
/* Spline: spline control handle */
/* it:     array where to store the basis function coefficents */
/* qStart: parameter value inside the curve */

long b3_spline::b3Mansfield(
	double	*it,
	double	 qStart)
{
	register int      l,i,j,m,k;
	register double	  r,denom,q,diff;

	if (closed)
	{
		m       = control_num;
		diff    = knots[m] - knots[0];
		if  ((i = iFind (qStart)) >= m) i -= m;

		it[0]  = 1;
		for (l = 1;l <= degree;l++)
		{
			it[l]  = 0;
			q      = qStart;
			k      = i;
			for (j = 0;j < l;j++)
			{
				denom     = knots[k+l] - knots[k];
				if (denom != 0) r = (q - knots[k]) / denom;
				else            r = 0;
				it[l-j]   += r * it[l-j-1];
				it[l-j-1] *= (1 - r);
				if (--k < 0) /* check underflow of knots */
				{
					k += m;
					q += diff;
				}
			}
		}
	}
	else
	{
		i      = iFind (knots,qStart,Spline->KnotNum,false);
		it[0]  = 1;
		for (l = 1;l <= degree;l++)
		{
			it[l]  = 0;
			for (j = 0;j < l;j++)
			{
				denom   = knots[i-j+l]      - knots[i-j];
				if (denom != 0) r = (qStart - knots[i-j]) / denom;
				else            r = 0;
				it[l-j]   += r * it[l-j-1];
				it[l-j-1] *= (1 - r);
			}
		}
	}
	return i;
}


/* This routine computes the real curve point according to the basis */
/* coefficients computed by Mansfield(). The value "i" is the knot */
/* index returned by Mansfield(). The start index is like the DeBoor- */
/* routines. */
/* ------------------------------------------------------------------ */
/* Spline: control handle of spline */
/* point:  where to store the curve point */
/* it:     basis coefficients computed by Mansfield() */
/* i:      knot index returned by Mansfield() */
/* index:  start index of control points */

void b3_spline::b3MansfieldVector(
	b3_vector *point,
	b3_f64    *it,
	b3_index   i,
	b3_index   index)
{
	register b3_vector *cntrls;
	register b3_index   l,m,j;

	m        = control_num;
	point->x =
	point->y =
	point->z = 0;
	if (closed)
	{
		j = i;
		for (l = degree;l >= 0;l--)
		{
			point->x += it[l] * controls[j * offset + index].x;
			point->y += it[l] * controls[j * offset + index].y;
			point->z += it[l] * controls[j * offset + index].z;
			if (--j < 0) j += m;
		}
	}
	else
	{
		cntrls = &controls[i * offset + index];
		for (l = degree;l >= 0;l--)
		{
			point->x += it[l] * cntrls->x;
			point->y += it[l] * cntrls->y;
			point->z += it[l] * cntrls->z;
			cntrls -= offset;
		}
	}
}


/* This routine computes the real curve point according to the basis */
/* coefficients computed by Mansfield(). The value "i" is the knot */
/* index returned by Mansfield(). The start index is like the DeBoor- */
/* routines. */
/* ------------------------------------------------------------------ */
/* Nurbs: control handle of NURBS */
/* point: where to store the curve point */
/* it:    basis coefficients computed by Mansfield() */
/* i:     knot index returned by Mansfield() */
/* index: start index of control points */

void b3_nurbs::b3MansfieldVector(
	b3_vector *point,
	b3_f64    *it,
	b3_index   i,
	b3_index   index)
{
	register vector4D *cntrls;
	register long      l,m,j;
	register double    denom;
	         vector4D  result;

	m      = control_num;

	result.x =
	result.y =
	result.z =
	result.w = 0;
	if (closed)
	{
		j = i;
		for (l = degree;l >= 0;l--)
		{
			result.x += it[l] * controls[j * offset + index].x;
			result.y += it[l] * controls[j * offset + index].y;
			result.z += it[l] * controls[j * offset + index].z;
			result.w += it[l] * controls[j * offset + index].w;
			if (--j < 0) j += m;
		}
	}
	else
	{
		cntrls = &controls[i * offset + index];
		for (l = degree;l >= 0;l--)
		{
			result.x += it[l] * cntrls->x;
			result.y += it[l] * cntrls->y;
			result.z += it[l] * cntrls->z;
			result.w += it[l] * cntrls->w;
			controls -= Nurbs->Offset;
		}
	}

	denom = 1.0 / result.w;
	point->x = result.x * denom;
	point->y = result.y * denom;
	point->z = result.z * denom;
}


/* This routine computes the first level of the de Boor algorithm. The */
/* computed points are used for the new control points needed for */
/* knot insertion. The control points are put into the o[]-array. */
/* ------------------------------------------------------------------- */
/* Spline: I  b-spline handle */
/* ins:    O  recreated control points */
/* index:  I  start index of curve */
/* q:      I  curve parameter to be inserted into knot (later) */

static long InsertDeBoorOpened(
	spline	 *Spline,
	vector4D *ins,
	long	  index,
	double	  q,
	bool      useNurbs)
{
	register float    *knots;
	register nurbs    *Nurbs = (nurbs *)Spline;
	register int       l,i,j,degree;
	register double    r,Denom;
	         vector4D  it[MAX_DEGREE+1];

	degree  = Spline->Degree;
	knots   = Spline->Knots;
	i       = iFind (knots,q,Spline->KnotNum,false);

	j = i * Spline->Offset + index;
	if (!useNurbs) for (l = 0;l <= degree;l++)
	{
		it[l].x = Spline->Controls[j].x;
		it[l].y = Spline->Controls[j].y;
		it[l].z = Spline->Controls[j].z;
		it[l].w = 1;
		j      -= Spline->Offset;
	}
	else for (l = 0;l <= degree;l++)
	{
		it[l].x = Nurbs->Controls[j].x;
		it[l].y = Nurbs->Controls[j].y;
		it[l].z = Nurbs->Controls[j].z;
		it[l].w = Nurbs->Controls[j].w;
		j      -= Nurbs->Offset;
	}

	ins += i;		/* loop for control point insertion */
	j    = i;
	for (l = 0;l < degree;l++)
	{
		Denom =   (knots[j+degree] - knots[j]);
		if (Denom != 0) r =     (q - knots[j]) / Denom;
		else            r =      0;
		it[l].x = (1-r) * it[l+1].x + r * it[l].x;
		it[l].y = (1-r) * it[l+1].y + r * it[l].y;
		it[l].z = (1-r) * it[l+1].z + r * it[l].z;
		it[l].w = (1-r) * it[l+1].w + r * it[l].w;
		*ins--  = it[l];
		j--;
	}
	return i;
}


/* This routine computes the same as the algorithm of de Boor. But it first */
/* computes the basis functions of the corresponding control points. The */
/* curve point is computed by computing the linear combination of the */
/* basis functions and its control points. This version is much faster than */
/* the de Boor algorithm, because the coefficients are only one dimensional. */
/* ------------------------------------------------------------------------- */
/* Spline: b-spline handle */
/* point:  point of computed curve point */
/* index:  start index */
/* qStart: curve parameter */

b3_index b3Spline::b3DeBoorOpened (
	b3_vector *point,  /* but DeBoor is more known among CG-freaks. */
	b3_index   index,
	b3_f64     q)
{
	register int      l,i,j;
	register double	  r,denom;
	         double   it[MAX_DEGREE+1];

	i      = iFind (q);

	it[0]  = 1;
	for (l = 1;l <= degree;l++)
	{
		it[l]  = 0;
		for (j = 0;j < l;j++)
		{
			denom   = knots[i-j+l] - knots[i-j];
			if (denom != 0) r = (q - knots[i-j]) / denom;
			else            r = 0;
			it[l-j]   += r * it[l-j-1];
			it[l-j-1] *= (1 - r);
		}
	}

	point->x =
	point->y =
	point->z = 0;
	j = i * Spline->Offset + index;
	for (l = degree;l >= 0;l--)
	{
		point->x += it[l] * Spline->Controls[j].x;
		point->y += it[l] * Spline->Controls[j].y;
		point->z += it[l] * Spline->Controls[j].z;
		j -= Spline->Offset;
	}

#	ifdef BSPLINE_DEBUG
		PrintF ("x: % 3.5f\n",point->x);
		PrintF ("y: % 3.5f\n",point->y);
		PrintF ("z: % 3.5f\n",point->z);
		PrintF ("\n");
#	endif

	return i;
}


/* This routine computes the first level of the de Boor algorithm. The */
/* computed points are used for the new control points needed for */
/* knot insertion. The control points are put into the o[]-array. */
/* ------------------------------------------------------------------- */
/* ins:    O  recreated control points */
/* index:  I  start index of curve */
/* q:      I  curve parameter to be inserted into knot (later) */

b3_index b3_spline::b3InsertDeBoorClosed (
	vector4D *ins,
	long	  index,
	double	  qStart,
	bool      useNurbs)
{
	register nurbs   *Nurbs = (nurbs *)Spline;
	register int      l,i,j,degree,m;
	register double   r,Denom,q,diff;
	         vector4D it[MAX_DEGREE+1];

	m       = control_num;
	diff    = knots[m] - knots[0];

	if ((i = iFind (qStart)) >= m) i -= m;

	j = i;
	if (!useNurbs) for (l = 0;l <= degree;l++)
	{
		it[l].x = Spline->Controls[j * Spline->Offset + index].x;
		it[l].y = Spline->Controls[j * Spline->Offset + index].y;
		it[l].z = Spline->Controls[j * Spline->Offset + index].z;
		it[l].w = 1;
		if (--j < 0) j += m;
	}
	else for (l = 0;l <= degree;l++)
	{
		it[l].x = Nurbs->Controls[j * Nurbs->Offset + index].x;
		it[l].y = Nurbs->Controls[j * Nurbs->Offset + index].y;
		it[l].z = Nurbs->Controls[j * Nurbs->Offset + index].z;
		it[l].w = Nurbs->Controls[j * Nurbs->Offset + index].w;
		if (--j < 0) j += m;
	}

		/* loop for control point insertion */
		/* the interval [o[i-degree+1] , o[i]] is valid */
	ins   += i;
	j      = i;
	q      = qStart;
	for (l = 0;l < degree;l++)
	{
		Denom =   (knots[j+degree] - knots[j]);
		if (Denom != 0) r =     (q - knots[j]) / Denom;
		else            r =      0;
		it[l].x = (1-r) * it[l+1].x + r * it[l].x;
		it[l].y = (1-r) * it[l+1].y + r * it[l].y;
		it[l].z = (1-r) * it[l+1].z + r * it[l].z;
		it[l].w = (1-r) * it[l+1].w + r * it[l].w;
		*ins--  = it[l];
		if (--j < 0) /* j = i-l; */
		{
			j   += m;
			q   += diff;
			ins += m;
		}
	}

	return i;
}


/* This routine computes the same as the algorithm of de Boor. But it first */
/* computes the basis functions of the corresponding control points. The */
/* curve point is computed by computing the linear combination of the */
/* basis functions and its control points. This version is much faster than */
/* the de Boor algorithm, because the coefficients are only one dimensional. */
/* ------------------------------------------------------------------------- */
/* Spline: b-spline handle */
/* point:  point of computed curve point */
/* index:  start index */
/* qStart: curve parameter */

long DeBoorClosed (
	spline	*Spline, /* this routine ought to be called MansfieldOpened, */
	vector	*point,  /* but DeBoor is more known among CG-freaks. */
	long	 index,
	double	 qStart)
{
	register float   *knots;
	register int      l,i,j,degree,m,k;
	register double	  r,denom,diff,q;
	         double   it[MAX_DEGREE+1];

	degree  = Spline->Degree;
	knots   = Spline->Knots;
	m       = Spline->ControlNum;
	diff    = knots[m] - knots[0];
	if  ((i = iFind (knots,qStart,m,true)) >= m) i -= m;

	it[0]  = 1;
	for (l = 1;l <= degree;l++)
	{
		it[l]  = 0;
		q      = qStart;
		k      = i;
		for (j = 0;j < l;j++)
		{
			denom     = knots[k+l] - knots[k];
			if (denom != 0) r = (q - knots[k]) / denom;
			else            r = 0;
			it[l-j]   += r * it[l-j-1];
			it[l-j-1] *= (1 - r);
			if (--k < 0) /* check underflow of knots */
			{
				k += m;
				q += diff;
			}
		}
	}

	point->x =
	point->y =
	point->z = 0;
	j = i;
	for (l = degree;l >= 0;l--)
	{
		point->x += it[l] * Spline->Controls[j * Spline->Offset + index].x;
		point->y += it[l] * Spline->Controls[j * Spline->Offset + index].y;
		point->z += it[l] * Spline->Controls[j * Spline->Offset + index].z;
		if (--j < 0) j += m;
	}

#	ifdef BSPLINE_DEBUG
		PrintF ("x: % 3.5f\n",point->x);
		PrintF ("y: % 3.5f\n",point->y);
		PrintF ("z: % 3.5f\n",point->z);
		PrintF ("\n");
#	endif

	return i;
}

/*************************************************************************
**                                                                      **
**                        control routines                              **
**                                                                      **
*************************************************************************/

b3_bool b3_spline::b3Init (
	long    Degree,
	long    ControlNum,
	bool    Closed)
{
	b3_index i;

	errno = BSPLINE_TOO_MUCH_CONTROLS;
	if ( ControlNum               > control_max) return false;
	errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((ControlNum + Degree + 1) > knot_max)    return false;
	errno = BSPLINE_MISSING_KNOTS;
	if (knots == null)                           return false;
	errno = BSPLINE_OK;

	degree      = Degree;
	control_num = ControlNum;
	knot_num    = ControlNum + Degree + 1;
	closed      = Closed;

	knots = Spline->Knots;
	for (i = 0; i < knot_max;i++) knots[i] = (float)i;
	return true;
}

b3_bool  b3_spline::b3SplineThroughEndControl ()
{
	register b3_f64   start,end;
	register b3_index i;

	errno = BSPLINE_CLOSED;
	if (closed) return false;
	errno = BSPLINE_OK;

	start      = knots[degree];
	end        = knots[control_num];
	for (i=0;i < degree;i++)
	{
		knots[i]                   = start;
		knots[control_num + i + 1] = end;
	}
	return true;
}

b3_bool b3_spline::b3SplineToBezier ()
{
	b3_index i;

	errno = BSPLINE_CLOSED;
	if  (closed) return false;
	errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((control_num + control_num) > kKnot_max) return false;
	errno = BSPLINE_OK;

	knot_num = control_num + control_num;
	for (i = 0;i < (knot_num >> 1);    i++) knots[i] = 0;
	for (i = knot_num >> 1;i < knot_num;i++) knots[i] = 1;
	degree = control_num - 1;

	return true;
}

b3_bool BSplineDegree (b3_count Degree)
{
	b3_f64   start,end;
	b3_index i,diff;

	errno = BSPLINE_OK;
	if ( Degree == degree)                     return true;
	errno = BSPLINE_TOO_FEW_CONTROLS;
	if ( degree >= control_num)                return false;
	errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((control_num + degree + 1) > knot_max) return false;
	errno = BSPLINE_OK;

	if (degree < Spline->Degree)     /* decreasing degree */
	{
		diff = degree - Degree;
		end = knots[control_num];
		for (i = 0;i <  diff;i++) knots[control_num + i] = end+i;
		for (i = 0;i <= control_num;i++) knots[i + Degree] = knots[i + degree];
		if (Spline->Closed)
		{
			start = knots[Degree];
			end   = knots[control_num+Degree];
			for (i = 0;i < Degree;i++)
			{
				knots[i] = knots[control_num+i] - end + start;
			}
			start = -knots[0];
			for (i = Degree + control_num;i>=0;i--)
			{
				knots[i] += start;
			}
		}
	}
	else                             /* increasing degree */
	{
		for (i = control_num;i >= 0;i--)
		{
			knots[i + Degree] = knots[i + degree];
		}
		if (closed)
		{
			start = knots[Degree];
			end   = knots[control_num + Degree];
			for (i = 0;i < Degree;i++)
			{
				knots[i] = knots[control_num + i] - end + start;
			}
			start = -knots[0];
			for (i = Degree + control_num;i>=0;i--)
			{
				knots[i] += start;
			}
		}
	}

	degree   = Degree;
	knot_num = control_num + degree + 1;
	return true;
}

b3_bool b3_spline::b3InsertControl (
	double	 q,
	long	 Mult,
	long     index,
	bool     useNurbs)
{
	register vector   *Controls;
	register vector4D *Controls4D;
	register long      l,m,i,Count;
	register double    start,end;
	         vector4D  o[MAX_CONTROLS+1]; /* buffer for knot insertion */

	errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return (false);

	m        =  control_num;
	if (!useNurbs) Controls   = &          Spline ->Controls[index];
	else           Controls4D = &((nurbs *)Spline)->Controls[index];

	errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > control_max) return false;
	errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((knot_num + Mult) > knot_max)    return false;
	errno = BSPLINE_OK;

	if (closed) for (Count = 0;Count < Mult;Count++)
	{
		start = knots[0];
		end   = knots[m];

		i = b3InsertDeBoorClosed (o,index,q,useNurbs);

		// insert new knot
		for (l = KnotNum;l > i;l--)
		{
			knots[l] = knots[l-1];
		}
		knots[i+1]         =   q;
		Spline->KnotNum    = ++KnotNum;
		Spline->ControlNum = ++m;
		for (l = 0;l <= degree;l++)
		{
			knots[l+m] = knots[l] - start + end;
		}

			/* insert o[x] into control points */
		if (!useNurbs)
		{
			for (l=m-1;l>i;l--)
				Controls[l * Spline->Offset] = Controls[(l - 1) * Spline->Offset];
			for (l=i-degree+1;l <= i;l++)
			{
				Controls[((l + m) % m) * Spline->Offset].x = o[(l + (m-1)) % (m-1)].x;
				Controls[((l + m) % m) * Spline->Offset].y = o[(l + (m-1)) % (m-1)].y;
				Controls[((l + m) % m) * Spline->Offset].z = o[(l + (m-1)) % (m-1)].z;
			}
		}
		else
		{
			for (l=m-1;l>i;l--)
				Controls4D[l * Spline->Offset] = Controls4D[(l - 1) * Spline->Offset];

			for (l=i-degree+1;l <= i;l++)
			{
				Controls4D[((l + m) % m) * Spline->Offset].x = o[(l + (m-1)) % (m-1)].x;
				Controls4D[((l + m) % m) * Spline->Offset].y = o[(l + (m-1)) % (m-1)].y;
				Controls4D[((l + m) % m) * Spline->Offset].z = o[(l + (m-1)) % (m-1)].z;
				Controls4D[((l + m) % m) * Spline->Offset].w = o[(l + (m-1)) % (m-1)].w;
			}
		}
	}
	else for (Count = 0;Count < Mult;Count++)
	{
		i = InsertDeBoorOpened (Spline,o,index,q,useNurbs);

			/* insert new knot */
		for (l=KnotNum;l > i;l--) knots[l+1] = knots[l];
		knots[i+1]         =   q;
		Spline->KnotNum    = ++KnotNum;

			/* insert o[x] into control points */
		if (!useNurbs)
		{
			for (l=m;l>i;l--)
				Controls[l * Spline->Offset] = Controls[(l - 1) * Spline->Offset];

			for (l=i-degree+1;l <= i;l++)
			{
				Controls[l * Spline->Offset].x = o[l].x;
				Controls[l * Spline->Offset].y = o[l].y;
				Controls[l * Spline->Offset].z = o[l].z;
			}
		}
		else
		{
			for (l=m;l>i;l--)
				Controls4D[l * Spline->Offset] = Controls4D[(l - 1) * Spline->Offset];

			for (l=i-degree+1;l <= i;l++)
			{
				Controls4D[l * Spline->Offset].x = o[l].x;
				Controls4D[l * Spline->Offset].y = o[l].y;
				Controls4D[l * Spline->Offset].z = o[l].z;
				Controls4D[l * Spline->Offset].w = o[l].w;
			}
		}

		Spline->ControlNum = ++m;
	}
	return (true);
}

bool BSplineInsertControl (
	spline	*Spline,
	double	 q,
	long	 Mult,
	long     index)
{
	return InternalInsertControl (Spline,q,Mult,index,false);
}

bool BNurbsInsertControl (
	nurbs   *Nurbs,
	double	 q,
	long	 Mult,
	long     index)
{
	return InternalInsertControl ((spline *)Nurbs,q,Mult,index,true);
}


/* This routine inserts control points into a de Boor net. The index between */
/* two net lines is the amount of "ControlOffset". In one line there are */
/* inserted "Mult" knots with value "q". This procedure is repeated "yLines" */
/* times. */
/* ------------------------------------------------------------------------- */
/* Spline:        spline handle */
/* q:             where to insert inside the knot vector */
/* Mult:          how many insertions to be done */
/* ControlOffset: distance between to curves */
/* yLines:        how many curves where to insert knots */

bool InternalSurfaceInsertControl(
	spline	*Spline,
	double	 q,
	long	 Mult,
	long     ControlOffset,
	long     yLines,
	
	bool     useNurbs)
{
	register vector   *Controls;
	register vector4D *Controls4D;
	register float    *knots;
	register long      l,m,degree,i,KnotNum,Count,index,y;
	register double	   start,end;
	         vector4D  o[MAX_CONTROLS+1]; /* buffer for knot insertion */

	errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return false;

	m        =  Spline->ControlNum;
	KnotNum  =  Spline->KnotNum;
	degree   =  Spline->Degree;
	knots    =  Spline->Knots;
	if (Mult > (degree + 1)) Mult = degree + 1;

	errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > Spline->ControlMax) return false;
	errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((KnotNum + Mult) > Spline->KnotMax)    return false;
	errno = BSPLINE_OK;

	if (Spline->Closed) for (Count = 0;Count < Mult;Count++)
	{
		start = knots[0];
		end   = knots[m];

		for (y = 0;y < yLines;y++)
		{
			index = y * ControlOffset;
			i = InsertDeBoorClosed (Spline,o,index,q,useNurbs);
			m++;

			if (!useNurbs)
			{
				Controls = &Spline->Controls[index];

					/* insert o[x] into control points */
				for (l=m-1;l>i;l--)
					Controls[l * Spline->Offset] = Controls[(l - 1) * Spline->Offset];
				for (l=i-degree+1;l <= i;l++)
				{
					Controls[((l + m) % m) * Spline->Offset].x = o[(l + (m-1)) % (m-1)].x;
					Controls[((l + m) % m) * Spline->Offset].y = o[(l + (m-1)) % (m-1)].y;
					Controls[((l + m) % m) * Spline->Offset].z = o[(l + (m-1)) % (m-1)].z;
				}
			}
			else
			{
				Controls4D = &((nurbs *)Spline)->Controls[index = y * ControlOffset];

					/* insert o[x] into control points */
				for (l=m-1;l>i;l--)
					Controls4D[l * Spline->Offset] = Controls4D[(l - 1) * Spline->Offset];
				for (l=i-degree+1;l <= i;l++)
					Controls4D[((l + m) % m) * Spline->Offset] = o[(l + (m-1)) % (m-1)];
			}
			m--;
		}
			/* insert new knot */
		for (l = KnotNum;l > i;l--) knots[l] = knots[l-1];
		knots[i+1]         =   q;
		Spline->KnotNum    = ++KnotNum;
		Spline->ControlNum = ++m;
		for (l=0;l<=degree;l++) knots[l+m] = knots[l] - start + end;
	}
	else for (Count = 0;Count < Mult;Count++)
	{
		for (y = 0;y < yLines;y++)
		{
			index = y * ControlOffset;
			i = InsertDeBoorOpened (Spline,o,index,q,useNurbs);
			m++;

				/* insert o[x] into control points */
			if (!useNurbs)
			{
				Controls = &Spline->Controls[index];
				for (l=m;l>i;l--)
					Controls[l * Spline->Offset] = Controls[(l - 1) * Spline->Offset];

				for (l=i-degree+1;l <= i;l++)
				{
					Controls[l * Spline->Offset].x = o[l].x;
					Controls[l * Spline->Offset].y = o[l].y;
					Controls[l * Spline->Offset].z = o[l].z;
				}
			}
			else
			{
				Controls4D = &((nurbs *)Spline)->Controls[index = y * ControlOffset];
				for (l=m;l>i;l--)
					Controls4D[l * Spline->Offset] = Controls4D[(l - 1) * Spline->Offset];

				for (l=i-degree+1;l <= i;l++)
					Controls4D[l * Spline->Offset] = o[l];
			}
			m--;
		}

			/* insert new knot */
		for (l = KnotNum;l > i;l--) knots[l+1] = knots[l];
		knots[i+1]         =   q;
		Spline->KnotNum    = ++KnotNum;
		Spline->ControlNum = ++m;
	}
	return (true);
}

bool BSplineSurfaceInsertControl(
	spline	*Spline,
	double	 q,
	long	 Mult,
	long     ControlOffset,
	long     yLines)
{
	return InternalSurfaceInsertControl
		(Spline,q,Mult,ControlOffset,yLines,false);
}

bool BNurbsSurfaceInsertControl(
	nurbs  *Nurbs,
	double	q,
	long	Mult,
	long    ControlOffset,
	long    yLines)
{
	return InternalSurfaceInsertControl
		((spline *)Nurbs,q,Mult,ControlOffset,yLines,true);
}

/*************************************************************************
**                                                                      **
**                        control point appending (curve)               **
**                                                                      **
*************************************************************************/

static bool InternalAppendControl(
	spline *Spline,
	bool    append,
	double  q,
	long    Mult,
	long    index,
	bool    useNurbs)
{
	register vector   *Controls;
	register vector4D *Controls4D;
	register float    *knots;
	register long      m,degree,i,KnotNum,offset;
	         vector4D  start,diff;

	errno = BSPLINE_CLOSED;
	if (Spline->Closed) return false;
	errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return false;

	m        =  Spline->ControlNum;
	KnotNum  =  Spline->KnotNum;
	degree   =  Spline->Degree;
	knots    =  Spline->Knots;
	offset   =  Spline->Offset;
	if (Mult > (degree + 1)) Mult = degree + 1;

	errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > Spline->ControlMax) return false;
	errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((KnotNum + Mult) > Spline->KnotMax)    return false;
	errno = BSPLINE_OK;

	if (append)
	{
		if (q <= knots[KnotNum-1]) return false;
		for (i = 0;i < Mult;i++) knots[KnotNum + i] = q;

		if (!useNurbs)
		{
			Controls = &Spline->Controls[index];
			start.x  = Controls[(m - 1) * offset].x;
			start.y  = Controls[(m - 1) * offset].y;
			start.z  = Controls[(m - 1) * offset].z;
			diff.x   = start.x - Controls[(m - 2) * offset].x;
			diff.y   = start.y - Controls[(m - 2) * offset].y;
			diff.z   = start.z - Controls[(m - 2) * offset].z;
			for (i=0;i<Mult;i++)
			{
				Controls[(m + i) * offset].x = start.x + (i+1) * diff.x;
				Controls[(m + i) * offset].y = start.y + (i+1) * diff.y;
				Controls[(m + i) * offset].z = start.z + (i+1) * diff.z;
			}
		}
		else
		{
			Controls4D = &((nurbs *)Spline)->Controls[index];
			start  = Controls4D[(m - 1) * offset];
			diff.x = start.x - Controls4D[(m - 2) * offset].x;
			diff.y = start.y - Controls4D[(m - 2) * offset].y;
			diff.z = start.z - Controls4D[(m - 2) * offset].z;
			diff.w = start.w - Controls4D[(m - 2) * offset].w;
			for (i=0;i<Mult;i++)
			{
				Controls4D[(m + i) * offset].x = start.x + (i+1) * diff.x;
				Controls4D[(m + i) * offset].y = start.y + (i+1) * diff.y;
				Controls4D[(m + i) * offset].z = start.z + (i+1) * diff.z;
				Controls4D[(m + i) * offset].w = start.w + (i+1) * diff.w;
			}
		}
	}
	else
	{
		if (q >= knots[0]) return false;
		for (i = KnotNum - 1;i >= 0;i--) knots[i+Mult] = knots[i];

		if (!useNurbs)
		{
			Controls = &Spline->Controls[index];
			start.x = Controls[0].x;
			start.y = Controls[0].y;
			start.z = Controls[0].z;
			diff.x = start.x - Controls[offset].x;
			diff.y = start.y - Controls[offset].y;
			diff.z = start.z - Controls[offset].z;

			for (i=m-1;i>=0;i--)
			{
				Controls[(i + Mult) * offset] = Controls[i * offset];
			}

			for (i=0;i<Mult;i++)
			{
				knots[i] = q;
				Controls[i * offset].x = start.x + (Mult - i) * diff.x;
				Controls[i * offset].y = start.y + (Mult - i) * diff.y;
				Controls[i * offset].z = start.z + (Mult - i) * diff.z;
			}
		}
		else
		{
			Controls4D = &((nurbs *)Spline)->Controls[index];

			start  = Controls4D[0];
			diff.x = start.x - Controls4D[offset].x;
			diff.y = start.y - Controls4D[offset].y;
			diff.z = start.z - Controls4D[offset].z;
			diff.w = start.w - Controls4D[offset].w;

			for (i=m-1;i>=0;i--)
			{
				Controls4D[(i + Mult) * offset] = Controls4D[i * offset];
			}

			for (i=0;i<Mult;i++)
			{
				knots[i] = q;
				Controls4D[i * offset].x = start.x + (Mult - i) * diff.x;
				Controls4D[i * offset].y = start.y + (Mult - i) * diff.y;
				Controls4D[i * offset].z = start.z + (Mult - i) * diff.z;
				Controls4D[i * offset].w = start.w + (Mult - i) * diff.w;
			}
		}
	}
	Spline->KnotNum    += Mult;
	Spline->ControlNum += Mult;
	return true;
}

bool BSplineAppendControl(
	spline *Spline,
	bool    append,
	double  q,
	long    Mult,
	long    index)
{
	return InternalAppendControl (Spline,append,q,Mult,index,false);
}

bool BNurbsAppendControl(
	nurbs  *Nurbs,
	bool    append,
	double  q,
	long    Mult,
	long    index)
{
	return InternalAppendControl ((spline *)Nurbs,append,q,Mult,index,true);
}

/*************************************************************************
**                                                                      **
**                        control point appending (surface)             **
**                                                                      **
*************************************************************************/

static bool InternalSurfaceAppendControl(
	spline *Spline,
	bool    append,
	double  q,
	long    Mult,
	long    skipOffset,
	long    lines,
	bool    useNurbs)
{
	register vector   *Controls;
	register vector4D *Controls4D;
	register float    *knots;
	register long      m,degree,i,KnotNum,offset,y;
	         vector4D  start,diff;

	errno = BSPLINE_CLOSED;
	if (Spline->Closed) return false;
	errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return false;

	m        = Spline->ControlNum;
	KnotNum  = Spline->KnotNum;
	degree   = Spline->Degree;
	knots    = Spline->Knots;
	offset   = Spline->Offset;
	if (Mult > (degree + 1)) Mult = degree + 1;

	errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > Spline->ControlMax) return false;
	errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((KnotNum + Mult) > Spline->KnotMax)    return false;
	errno = BSPLINE_OK;

	if (append)
	{
		if (q <= knots[KnotNum-1]) return false;
		for (i=0;i<Mult;i++) knots[KnotNum + i] = q;

		if (!useNurbs)
		{
			Controls = Spline->Controls;
			for (y=0;y<lines;y++)
			{
				start.x = Controls[(m - 1) * offset].x;
				start.y = Controls[(m - 1) * offset].y;
				start.z = Controls[(m - 1) * offset].z;
				diff.x  = start.x - Controls[(m - 2) * offset].x;
				diff.y  = start.y - Controls[(m - 2) * offset].y;
				diff.z  = start.z - Controls[(m - 2) * offset].z;
				for (i=0;i<Mult;i++)
				{
					Controls[(m + i) * offset].x = start.x + (i+1) * diff.x;
					Controls[(m + i) * offset].y = start.y + (i+1) * diff.y;
					Controls[(m + i) * offset].z = start.z + (i+1) * diff.z;
				}
				Controls += skipOffset;
			}
		}
		else
		{
			Controls4D = ((nurbs *)Spline)->Controls;
			for (y=0;y<lines;y++)
			{
				start  = Controls4D[(m - 1) * offset];
				diff.x = start.x - Controls4D[(m - 2) * offset].x;
				diff.y = start.y - Controls4D[(m - 2) * offset].y;
				diff.z = start.z - Controls4D[(m - 2) * offset].z;
				diff.w = start.w - Controls4D[(m - 2) * offset].w;
				for (i=0;i<Mult;i++)
				{
					Controls4D[(m + i) * offset].x = start.x + (i+1) * diff.x;
					Controls4D[(m + i) * offset].y = start.y + (i+1) * diff.y;
					Controls4D[(m + i) * offset].z = start.z + (i+1) * diff.z;
					Controls4D[(m + i) * offset].w = start.w + (i+1) * diff.w;
				}
				Controls4D += skipOffset;
			}
		}
	}
	else
	{
		if (q >= knots[0]) return false;

		for (i = KnotNum-1;i >= 0;   i--) knots[i+Mult] = knots[i];
		for (i = 0;i <  Mult;i++)         knots[i] = q;

		if (!useNurbs)
		{
			Controls = Spline->Controls;
			for (y = 0;  y <  lines;y++)
			{
				start.x = Controls[0].x;
				start.y = Controls[0].y;
				start.z = Controls[0].z;
				diff.x  = start.x - Controls[offset].x;
				diff.y  = start.y - Controls[offset].y;
				diff.z  = start.z - Controls[offset].z;
				for (i = m-1;i >= 0;i--)
				{
					Controls[(i + Mult) * offset] = Controls[i * offset];
				}

				for (i = 0;i < Mult;i++)
				{
					Controls[i * offset].x = start.x + (Mult - i) * diff.x;
					Controls[i * offset].y = start.y + (Mult - i) * diff.y;
					Controls[i * offset].z = start.z + (Mult - i) * diff.z;
				}
				Controls += skipOffset;
			}
		}
		else
		{
			Controls4D = ((nurbs *)Spline)->Controls;
			for (y = 0;  y <  lines;y++)
			{
				start  = Controls4D[0];
				diff.x = start.x - Controls4D[offset].x;
				diff.y = start.y - Controls4D[offset].y;
				diff.z = start.z - Controls4D[offset].z;
				diff.w = start.w - Controls4D[offset].w;
				for (i = m-1;i >= 0;i--)
				{
					Controls4D[(i + Mult) * offset] = Controls4D[i * offset];
				}

				for (i = 0;i < Mult;i++)
				{
					Controls4D[i * offset].x = start.x + (Mult - i) * diff.x;
					Controls4D[i * offset].y = start.y + (Mult - i) * diff.y;
					Controls4D[i * offset].z = start.z + (Mult - i) * diff.z;
					Controls4D[i * offset].w = start.w + (Mult - i) * diff.w;
				}
				Controls4D += skipOffset;
			}
		}
	}
	Spline->KnotNum    += Mult;
	Spline->ControlNum += Mult;
	return true;
}

bool BSplineSurfaceAppendControl(
	spline *Spline,
	bool    append,
	double  q,
	long    Mult,
	long    skipOffset,
	long    lines)
{
	return InternalSurfaceAppendControl
		(Spline,append,q,Mult,skipOffset,lines,false);
}

bool BNurbsSurfaceAppendControl(
	nurbs  *Nurbs,
	bool    append,
	double  q,
	long    Mult,
	long    skipOffset,
	long    lines)
{
	return InternalSurfaceAppendControl
		((spline *)Nurbs,append,q,Mult,skipOffset,lines,true);
}

/*************************************************************************
**                                                                      **
**                        de Boor routines                              **
**                                                                      **
*************************************************************************/

long DeBoor(
	spline	*Spline,
	vector	*point,
	long	 index)
{
	register float	*knots;
	register double	 q,qStep;
	register long	 SubDiv,i;

	SubDiv = Spline->SubDiv;
	knots  = Spline->Knots;

	if (Spline->Closed)
	{
		q     =  knots[0];
		qStep = (knots[Spline->ControlNum] - q - EPSILON) / (float)Spline->SubDiv;

		for (i = 0;i <= SubDiv;i++)
		{
			DeBoorClosed (Spline,point++,index,q);
			q += qStep;
		}
	}
	else
	{
		q     =  knots[Spline->Degree];
		qStep = (knots[Spline->ControlNum] - q - EPSILON) / (float)Spline->SubDiv;

		for (i = 0;i <= SubDiv;i++)
		{
			DeBoorOpened (Spline,point++,index,q);
			q += qStep;
		}
	}

	return i;
}

long DeBoorNurbs(
	nurbs	*Nurbs,
	vector  *point,
	long     index)
{
	register float	*knots;
	register double	 q,qStep;
	register long	 SubDiv,i,pos;
	         double  coeffs[MAX_DEGREE+1];

	SubDiv = Nurbs->SubDiv;
	knots  = Nurbs->Knots;
	q      =  knots[Nurbs->Closed ? 0 : Nurbs->Degree];
	qStep  = (knots[Nurbs->ControlNum] - q - EPSILON) / (float)Nurbs->SubDiv;

	for (i = 0;i <= SubDiv;i++)
	{
		pos = Mansfield ((spline *)Nurbs,coeffs,q);
		MansfieldNurbsVector (Nurbs,point++,coeffs,pos,index);
		q += qStep;
	}
	return i;
}

long DeBoorControl(
	spline	*Spline,
	vector	*point,
	long	 index)
{
	register float	*knots;
	register long	 ControlNum,i;

	ControlNum = Spline->ControlNum;
	knots      = Spline->Knots;

	if (Spline->Closed)
	{
		for (i = 0;i < ControlNum;i++)
			DeBoorClosed(Spline,point++,index,knots[i]);
		return ControlNum;
	}
	else
	{
		for (i = Spline->Degree;i <= ControlNum;i++)
			DeBoorOpened (Spline,point++,index,knots[i]);
		return ControlNum - Spline->Degree + 1;
	}
}

long DeBoorSurfaceControl(
	spline	*controlSpline,
	spline	*curveSpline,
	vector	*point)
{
	register float	*knots;
	register long	 ControlNum,i,index,x,end;
	         double  it[MAX_DEGREE+1];

	ControlNum = BSPLINE_SEGMENTKNOTS(curveSpline);
	knots      = curveSpline->Knots;
	end        = curveSpline->ControlNum;
	if (!curveSpline->Closed) end++;

	for (i = (curveSpline->Closed ? 0 : curveSpline->Degree);i < end;i++)
	{
		index = Mansfield (curveSpline,it,knots[i]);
		for (x = 0;x < controlSpline->ControlNum;x++)
		{
			MansfieldVector (curveSpline,&point[x * ControlNum],it,
				index,x * controlSpline->Offset);
		}
		point++;
	}
	return ControlNum;
}
