/*
**
**	$Filename:	b3Spline.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 2.02 
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III Standard - Spline Routines
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

#include "blz3/base/b3Spline.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2001/10/11 16:06:32  sm
**	- Cleaning up b3BSpline with including isolated methods.
**	- Cleaning up endian conversion routines and collecting into
**	  b3Endian
**	- Cleaning up some datatypes for proper display in Together.
**
**	Revision 1.4  2001/08/05 19:53:43  sm
**	- Removing some nasty CR/LF
**	
**	Revision 1.3  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**	Revision 1.2  2001/08/05 09:23:22  sm
**	- Introducing vectors, matrices, Splines and NURBS
**	
**	
*/

#define EPSILON 0.0001

static b3_bspline_error bspline_errno;

b3_bspline_error b3BSplineErrno()
{
	return bspline_errno;
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

static b3_index iFind(
	b3_f32	*knots,
	b3_f64	 q,
	b3_count KnotNum,
	b3_bool	 closed)
{
	b3_index i;

	for (i=0;i < (KnotNum-1);i++)
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

b3_index b3Spline::b3Mansfield(
	b3_f64	*it,
	b3_f64	 qStart)
{
	b3_index  l,i,j,k;
	b3_f64	  r,denom,q,diff;

	if (closed)
	{
		diff    = knots[control_num] - knots[0];
		if  ((i = iFind (knots,qStart,control_num,true)) >= control_num)
		{
			i -= control_num;
		}

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
					k += control_num;
					q += diff;
				}
			}
		}
	}
	else
	{
		i      = iFind (knots,qStart,knot_num,false);
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

void b3Spline::b3MansfieldVector(
	b3_vector *point,
	b3_f64    *it,
	b3_index   i,
	b3_index   index)
{
	b3_index   l,j;
	b3_count   degree;
	b3_vector *ctrls;

	point->x =
	point->y =
	point->z = 0;
	if (closed)
	{
		j = i;
		for (l = degree;l >= 0;l--)
		{
			point->x += (b3_f32)(it[l] * controls[j * offset + index].x);
			point->y += (b3_f32)(it[l] * controls[j * offset + index].y);
			point->z += (b3_f32)(it[l] * controls[j * offset + index].z);
			if (--j < 0) j += control_num;
		}
	}
	else
	{
		ctrls = &controls[i * offset + index];
		for (l = degree;l >= 0;l--)
		{
			point->x += (b3_f32)(it[l] * ctrls->x);
			point->y += (b3_f32)(it[l] * ctrls->y);
			point->z += (b3_f32)(it[l] * ctrls->z);
			ctrls -= offset;
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

void b3MansfieldNurbsVector(
	b3_nurbs  *Nurbs,
	b3_vector *point,
	b3_f64    *it,
	b3_index   i,
	b3_index   index)
{
	b3_vector4D *controls;
	b3_index     l,m,j;
	b3_count     degree;
	b3_f64       denom;
	b3_vector4D  result;

	degree = Nurbs->degree;
	m      = Nurbs->control_num;

	result.x =
	result.y =
	result.z =
	result.w = 0;
	if (Nurbs->closed)
	{
		j = i;
		for (l = degree;l >= 0;l--)
		{
			result.x += (b3_f32)(it[l] * Nurbs->controls[j * Nurbs->offset + index].x);
			result.y += (b3_f32)(it[l] * Nurbs->controls[j * Nurbs->offset + index].y);
			result.z += (b3_f32)(it[l] * Nurbs->controls[j * Nurbs->offset + index].z);
			result.w += (b3_f32)(it[l] * Nurbs->controls[j * Nurbs->offset + index].w);
			if (--j < 0) j += m;
		}
	}
	else
	{
		controls = &Nurbs->controls[i * Nurbs->offset + index];
		for (l = degree;l >= 0;l--)
		{
			result.x += (b3_f32)(it[l] * controls->x);
			result.y += (b3_f32)(it[l] * controls->y);
			result.z += (b3_f32)(it[l] * controls->z);
			result.w += (b3_f32)(it[l] * controls->w);
			controls -= Nurbs->offset;
		}
	}

	denom = 1.0 / result.w;
	point->x = (b3_f32)(result.x * denom);
	point->y = (b3_f32)(result.y * denom);
	point->z = (b3_f32)(result.z * denom);
}


/* This routine computes the first level of the de Boor algorithm. The */
/* computed points are used for the new control points needed for */
/* knot insertion. The control points are put into the o[]-array. */
/* ------------------------------------------------------------------- */
/* Spline: I  b-spline handle */
/* ins:    O  recreated control points */
/* index:  I  start index of curve */
/* q:      I  curve parameter to be inserted into knot (later) */

static b3_index b3InsertDeBoorOpened(
	b3_spline   *Spline,
	b3_vector4D *ins,
	b3_index     index,
	b3_f64       q,
	b3_bool      useNurbs)
{
	b3_f32      *knots;
	b3_nurbs    *Nurbs = (b3_nurbs *)Spline;
	b3_index     l,i,j;
	b3_count     degree;
	b3_f64       r,Denom;
	b3_vector4D  it[B3_MAX_DEGREE + 1];

	degree  = Spline->degree;
	knots   = Spline->knots;
	i       = iFind (knots,q,Spline->knot_num,false);

	j = i * Spline->offset + index;
	if (!useNurbs) for (l = 0;l <= degree;l++)
	{
		it[l].x = Spline->controls[j].x;
		it[l].y = Spline->controls[j].y;
		it[l].z = Spline->controls[j].z;
		it[l].w = 1;
		j      -= Spline->offset;
	}
	else for (l = 0;l <= degree;l++)
	{
		it[l].x = Nurbs->controls[j].x;
		it[l].y = Nurbs->controls[j].y;
		it[l].z = Nurbs->controls[j].z;
		it[l].w = Nurbs->controls[j].w;
		j      -= Nurbs->offset;
	}

	ins += i;		/* loop for control point insertion */
	j    = i;
	for (l = 0;l < degree;l++)
	{
		Denom =   (knots[j+degree] - knots[j]);
		if (Denom != 0) r =     (q - knots[j]) / Denom;
		else            r =      0;
		it[l].x = (b3_f32)((1-r) * it[l+1].x + r * it[l].x);
		it[l].y = (b3_f32)((1-r) * it[l+1].y + r * it[l].y);
		it[l].z = (b3_f32)((1-r) * it[l+1].z + r * it[l].z);
		it[l].w = (b3_f32)((1-r) * it[l+1].w + r * it[l].w);
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
	b3_index  l,i,j;
	b3_f64    r,denom;
	b3_f64    it[B3_MAX_DEGREE + 1];

	i = iFind (knots,q,knot_num,false);

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
	j = i * offset + index;
	for (l = degree;l >= 0;l--)
	{
		point->x += (b3_f32)(it[l] * controls[j].x);
		point->y += (b3_f32)(it[l] * controls[j].y);
		point->z += (b3_f32)(it[l] * controls[j].z);
		j -= offset;
	}

#	ifdef BSPLINE_DEBUG
		b3PrintF (B3LOG_FULL,"x: % 3.5f\n",point->x);
		b3PrintF (B3LOG_FULL,"y: % 3.5f\n",point->y);
		b3PrintF (B3LOG_FULL,"z: % 3.5f\n",point->z);
		b3PrintF (B3LOG_FULL,"\n");
#	endif

	return i;
}


/* This routine computes the first level of the de Boor algorithm. The */
/* computed points are used for the new control points needed for */
/* knot insertion. The control points are put into the o[]-array. */
/* ------------------------------------------------------------------- */
/* Spline: I  b-spline handle */
/* ins:    O  recreated control points */
/* index:  I  start index of curve */
/* q:      I  curve parameter to be inserted into knot (later) */

static b3_index b3InsertDeBoorClosed (
	b3_spline   *Spline,
	b3_vector4D *ins,
	b3_index     index,
	b3_f64       qStart,
	b3_bool      useNurbs)
{
	b3_f32      *knots;
	b3_nurbs    *Nurbs = (b3_nurbs *)Spline;
	b3_index     l,i,j,m;
	b3_count     degree;
	b3_f64       r,Denom,q,diff;
	b3_vector4D  it[B3_MAX_DEGREE + 1];

	degree  = Spline->degree;
	knots   = Spline->knots;
	m       = Spline->control_num;
	diff    = knots[m] - knots[0];

	if ((i = iFind (knots,qStart,m,true)) >= m) i -= m;

	j = i;
	if (!useNurbs) for (l = 0;l <= degree;l++)
	{
		it[l].x = Spline->controls[j * Spline->offset + index].x;
		it[l].y = Spline->controls[j * Spline->offset + index].y;
		it[l].z = Spline->controls[j * Spline->offset + index].z;
		it[l].w = 1;
		if (--j < 0) j += m;
	}
	else for (l = 0;l <= degree;l++)
	{
		it[l].x = Nurbs->controls[j * Nurbs->offset + index].x;
		it[l].y = Nurbs->controls[j * Nurbs->offset + index].y;
		it[l].z = Nurbs->controls[j * Nurbs->offset + index].z;
		it[l].w = Nurbs->controls[j * Nurbs->offset + index].w;
		if (--j < 0) j += m;
	}

	// loop for control point insertion
	// the interval [o[i-degree+1] , o[i]] is valid
	ins   += i;
	j      = i;
	q      = qStart;
	for (l = 0;l < degree;l++)
	{
		Denom =   (knots[j+degree] - knots[j]);
		if (Denom != 0) r =     (q - knots[j]) / Denom;
		else            r =      0;
		it[l].x = (b3_f32)((1-r) * it[l+1].x + r * it[l].x);
		it[l].y = (b3_f32)((1-r) * it[l+1].y + r * it[l].y);
		it[l].z = (b3_f32)((1-r) * it[l+1].z + r * it[l].z);
		it[l].w = (b3_f32)((1-r) * it[l+1].w + r * it[l].w);
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

b3_index b3Spline::b3DeBoorClosed (
	b3_vector *point,  /* but DeBoor is more known among CG-freaks. */
	b3_index   index,
	b3_f64     qStart)
{
	b3_index   l,i,j,k;
	b3_f64     r,denom,diff,q;
	b3_f64     it[B3_MAX_DEGREE + 1];

	diff    = knots[control_num] - knots[0];
	if  ((i = iFind (knots,qStart,control_num,true)) >= control_num)
	{
		i -= control_num;
	}

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
				k += control_num;
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
		point->x += (b3_f32)(it[l] * controls[j * offset + index].x);
		point->y += (b3_f32)(it[l] * controls[j * offset + index].y);
		point->z += (b3_f32)(it[l] * controls[j * offset + index].z);
		if (--j < 0) j += control_num;
	}

#	ifdef BSPLINE_DEBUG
		b3PrintF (B3LOG_FULL,"x: % 3.5f\n",point->x);
		b3PrintF (B3LOG_FULL,"y: % 3.5f\n",point->y);
		b3PrintF (B3LOG_FULL,"z: % 3.5f\n",point->z);
		b3PrintF (B3LOG_FULL,"\n");
#	endif

	return i;
}

/*************************************************************************
**                                                                      **
**                        control routines                              **
**                                                                      **
*************************************************************************/

b3_bool b3BSplineInitCurve (
	b3_spline *Spline,
	b3_count   degree,
	b3_count   ControlNum,
	b3_bool    closed)
{
	b3_f32   *knots;
	b3_index  i;

	bspline_errno = BSPLINE_TOO_MUCH_CONTROLS;
	if ( ControlNum               > Spline->control_max) return false;
	bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((ControlNum + degree + 1) > Spline->knot_max)    return false;
	bspline_errno = BSPLINE_MISSING_KNOTS;
	if (Spline->knots == null)                          return false;
	bspline_errno = BSPLINE_OK;

	Spline->degree     = degree;
	Spline->control_num = ControlNum;
	Spline->knot_num    = ControlNum + degree + 1;
	Spline->closed     = closed;

	knots = Spline->knots;
	for (i=0;i < Spline->knot_max;i++) *knots++ = (b3_f32)i;
	return true;
}

b3_bool b3BSplineThroughEndControl (b3_spline *Spline)
{
	b3_f64    start,end;
	b3_f32   *knots;
	b3_index  i;
	b3_count  KnotNum,ControlNum,degree;

	bspline_errno = BSPLINE_CLOSED;
	if (Spline->closed) return false;
	bspline_errno = BSPLINE_OK;

	knots      = Spline->knots;
	ControlNum = Spline->control_num;
	degree     = Spline->degree;
	KnotNum    = ControlNum + degree + 1;
	start      = knots[degree];
	end        = knots[ControlNum];

	for (i = 0;i < degree;i++)
	{
		knots[i]                  = (b3_f32)start;
		knots[ControlNum + i + 1] = (b3_f32)end;
	}
	Spline->knot_num = KnotNum;
	return true;
}

b3_bool b3BSplineToBezier (b3_spline *Spline)
{
	b3_index i;
	b3_count KnotNum;

	bspline_errno = BSPLINE_CLOSED;
	if  (Spline->closed) return false;
	bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((Spline->control_num * 2) > Spline->knot_max) return false;
	bspline_errno = BSPLINE_OK;

	KnotNum = Spline->knot_num = Spline->control_num * 2;
	for (i = 0;i < (KnotNum / 2);    i++) Spline->knots[i] = 0;
	for (i = KnotNum / 2;i < KnotNum;i++) Spline->knots[i] = 1;
	Spline->degree = Spline->control_num - 1;

	return true;
}

b3_bool b3BSplineDegree (
	b3_spline *Spline,
	b3_count   degree)
{
	b3_f32   *knots;
	b3_f32	  start,end;
	b3_index  i,diff;
	b3_count  ControlNum;

	bspline_errno = BSPLINE_OK;
	if ( Spline->degree == degree)                           return true;
	bspline_errno = BSPLINE_TOO_FEW_CONTROLS;
	if ( Spline->degree >= Spline->control_num)               return false;
	bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((Spline->control_num + degree + 1) > Spline->knot_max) return false;
	bspline_errno = BSPLINE_OK;

	knots      = Spline->knots;
	ControlNum = Spline->control_num;
	if (degree < Spline->degree)     /* decreasing degree */
	{
		diff = Spline->degree - degree;
		end = knots[ControlNum];
		for (i = 0;i <  diff;i++) knots[ControlNum+i] = end+i;
		for (i = 0;i <= ControlNum;i++) knots[i+degree] = knots[i + Spline->degree];
		if (Spline->closed)
		{
			start = knots[degree];
			end   = knots[ControlNum+degree];
			for (i=0;i<degree;i++) knots[i] = knots[ControlNum+i] - end + start;
			start = -knots[0];
			for (i=degree+ControlNum;i>=0;i--) knots[i] += start;
		}
	}
	else                             /* increasing degree */
	{
		for (i = ControlNum;i >= 0;i--) knots[i+degree] = knots[i + Spline->degree];
		if (Spline->closed)
		{
			start = knots[degree];
			end   = knots[ControlNum+degree];
			for (i=0;i<degree;i++) knots[i] = knots[ControlNum+i] - end + start;
			start = -knots[0];
			for (i=degree+ControlNum;i>=0;i--) knots[i] += start;
		}
	}

	Spline->knot_num = ControlNum + degree + 1;
	Spline->degree  = degree;
	return true;
}

static b3_bool b3InternalInsertControl (
	b3_spline *Spline,
	b3_f64     q,
	b3_count   Mult,
	b3_index   index,
	b3_bool    useNurbs)
{
	b3_vector   *Controls;
	b3_vector4D *Controls4D;
	b3_f32      *knots;
	b3_index     l,m,i;
	b3_count     degree,KnotNum,Count;
	b3_f64       start,end;
	b3_vector4D  o[B3_MAX_CONTROLS + 1]; /* buffer for knot insertion */

	bspline_errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return false;

	m        =  Spline->control_num;
	KnotNum  =  Spline->knot_num;
	degree   =  Spline->degree;
	knots    =  Spline->knots;
	if (!useNurbs) Controls   = &Spline ->controls[index];
	else           Controls4D = &((b3_nurbs *)Spline)->controls[index];

	bspline_errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > Spline->control_max) return false;
	bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((KnotNum + Mult) > Spline->knot_max)    return false;
	bspline_errno = BSPLINE_OK;

	if (Spline->closed) for (Count = 0;Count < Mult;Count++)
	{
		start = knots[0];
		end   = knots[m];

		i = b3InsertDeBoorClosed (Spline,o,index,q,useNurbs);

			/* insert new knot */
		for (l = KnotNum;l > i;l--) knots[l] = knots[l-1];
		knots[i+1]          = (b3_f32)q;
		Spline->knot_num    = ++KnotNum;
		Spline->control_num = ++m;
		for (l = 0;l <= degree;l++)
		{
			knots[l+m] = (b3_f32)(knots[l] - start + end);
		}

			/* insert o[x] into control points */
		if (!useNurbs)
		{
			for (l=m-1;l>i;l--)
				Controls[l * Spline->offset] = Controls[(l - 1) * Spline->offset];
			for (l=i-degree+1;l <= i;l++)
			{
				Controls[((l + m) % m) * Spline->offset].x = o[(l + (m-1)) % (m-1)].x;
				Controls[((l + m) % m) * Spline->offset].y = o[(l + (m-1)) % (m-1)].y;
				Controls[((l + m) % m) * Spline->offset].z = o[(l + (m-1)) % (m-1)].z;
			}
		}
		else
		{
			for (l=m-1;l>i;l--)
				Controls4D[l * Spline->offset] = Controls4D[(l - 1) * Spline->offset];

			for (l=i-degree+1;l <= i;l++)
			{
				Controls4D[((l + m) % m) * Spline->offset].x = o[(l + (m-1)) % (m-1)].x;
				Controls4D[((l + m) % m) * Spline->offset].y = o[(l + (m-1)) % (m-1)].y;
				Controls4D[((l + m) % m) * Spline->offset].z = o[(l + (m-1)) % (m-1)].z;
				Controls4D[((l + m) % m) * Spline->offset].w = o[(l + (m-1)) % (m-1)].w;
			}
		}
	}
	else for (Count = 0;Count < Mult;Count++)
	{
		i = b3InsertDeBoorOpened (Spline,o,index,q,useNurbs);

			/* insert new knot */
		for (l=KnotNum;l > i;l--) knots[l+1] = knots[l];
		knots[i+1]       = (b3_f32)q;
		Spline->knot_num = ++KnotNum;

			/* insert o[x] into control points */
		if (!useNurbs)
		{
			for (l=m;l>i;l--)
				Controls[l * Spline->offset] = Controls[(l - 1) * Spline->offset];

			for (l=i-degree+1;l <= i;l++)
			{
				Controls[l * Spline->offset].x = o[l].x;
				Controls[l * Spline->offset].y = o[l].y;
				Controls[l * Spline->offset].z = o[l].z;
			}
		}
		else
		{
			for (l=m;l>i;l--)
				Controls4D[l * Spline->offset] = Controls4D[(l - 1) * Spline->offset];

			for (l=i-degree+1;l <= i;l++)
			{
				Controls4D[l * Spline->offset].x = o[l].x;
				Controls4D[l * Spline->offset].y = o[l].y;
				Controls4D[l * Spline->offset].z = o[l].z;
				Controls4D[l * Spline->offset].w = o[l].w;
			}
		}

		Spline->control_num = ++m;
	}
	return true;
}

b3_bool b3BSplineInsertControl (
	b3_spline *Spline,
	b3_f64     q,
	b3_count   Mult,
	b3_index   index)
{
	return b3InternalInsertControl (Spline,q,Mult,index,false);
}

b3_bool b3BNurbsInsertControl (
	b3_nurbs *Nurbs,
	b3_f64    q,
	b3_count  Mult,
	b3_index  index)
{
	return b3InternalInsertControl ((b3_spline *)Nurbs,q,Mult,index,true);
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

b3_bool b3InternalSurfaceInsertControl(
	b3_spline *Spline,
	b3_f64     q,
	b3_count   Mult,
	b3_index   ControlOffset,
	b3_count   yLines,
	b3_bool    useNurbs)
{
	b3_vector   *Controls;
	b3_vector4D *Controls4D;
	b3_f32      *knots;
	b3_index     l,m,i,index,y;
	b3_count     degree,KnotNum,Count;
	b3_f64	     start,end;
	b3_vector4D  o[B3_MAX_CONTROLS + 1]; /* buffer for knot insertion */

	bspline_errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return false;

	m        =  Spline->control_num;
	KnotNum  =  Spline->knot_num;
	degree   =  Spline->degree;
	knots    =  Spline->knots;
	if (Mult > (degree + 1)) Mult = degree + 1;

	bspline_errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > Spline->control_max) return false;
	bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((KnotNum + Mult) > Spline->knot_max)    return false;
	bspline_errno = BSPLINE_OK;

	if (Spline->closed) for (Count = 0;Count < Mult;Count++)
	{
		start = knots[0];
		end   = knots[m];

		for (y = 0;y < yLines;y++)
		{
			index = y * ControlOffset;
			i = b3InsertDeBoorClosed (Spline,o,index,q,useNurbs);
			m++;

			if (!useNurbs)
			{
				Controls = &Spline->controls[index];

					/* insert o[x] into control points */
				for (l=m-1;l>i;l--)
					Controls[l * Spline->offset] = Controls[(l - 1) * Spline->offset];
				for (l=i-degree+1;l <= i;l++)
				{
					Controls[((l + m) % m) * Spline->offset].x = o[(l + (m-1)) % (m-1)].x;
					Controls[((l + m) % m) * Spline->offset].y = o[(l + (m-1)) % (m-1)].y;
					Controls[((l + m) % m) * Spline->offset].z = o[(l + (m-1)) % (m-1)].z;
				}
			}
			else
			{
				Controls4D = &((b3_nurbs *)Spline)->controls[index = y * ControlOffset];

					/* insert o[x] into control points */
				for (l=m-1;l>i;l--)
					Controls4D[l * Spline->offset] = Controls4D[(l - 1) * Spline->offset];
				for (l=i-degree+1;l <= i;l++)
					Controls4D[((l + m) % m) * Spline->offset] = o[(l + (m-1)) % (m-1)];
			}
			m--;
		}
			/* insert new knot */
		for (l = KnotNum;l > i;l--) knots[l] = knots[l-1];
		knots[i+1]       = (b3_f32)q;
		Spline->knot_num = ++KnotNum;
		Spline->control_num = ++m;
		for (l=0;l<=degree;l++)
		{
			knots[l+m] = (b3_f32)(knots[l] - start + end);
		}
	}
	else for (Count = 0;Count < Mult;Count++)
	{
		for (y = 0;y < yLines;y++)
		{
			index = y * ControlOffset;
			i = b3InsertDeBoorOpened (Spline,o,index,q,useNurbs);
			m++;

				/* insert o[x] into control points */
			if (!useNurbs)
			{
				Controls = &Spline->controls[index];
				for (l=m;l>i;l--)
					Controls[l * Spline->offset] = Controls[(l - 1) * Spline->offset];

				for (l=i-degree+1;l <= i;l++)
				{
					Controls[l * Spline->offset].x = o[l].x;
					Controls[l * Spline->offset].y = o[l].y;
					Controls[l * Spline->offset].z = o[l].z;
				}
			}
			else
			{
				Controls4D = &((b3_nurbs *)Spline)->controls[index = y * ControlOffset];
				for (l=m;l>i;l--)
					Controls4D[l * Spline->offset] = Controls4D[(l - 1) * Spline->offset];

				for (l=i-degree+1;l <= i;l++)
					Controls4D[l * Spline->offset] = o[l];
			}
			m--;
		}

			/* insert new knot */
		for (l = KnotNum;l > i;l--) knots[l+1] = knots[l];
		knots[i+1]          = (b3_f32)q;
		Spline->knot_num    = ++KnotNum;
		Spline->control_num = ++m;
	}
	return true;
}

b3_bool b3BSplineSurfaceInsertControl(
	b3_spline *Spline,
	b3_f64     q,
	b3_count   Mult,
	b3_index   ControlOffset,
	b3_index   yLines)
{
	return b3InternalSurfaceInsertControl
		(Spline,q,Mult,ControlOffset,yLines,false);
}

b3_bool b3BNurbsSurfaceInsertControl(
	b3_nurbs *Nurbs,
	b3_f64    q,
	b3_count  Mult,
	b3_index  ControlOffset,
	b3_index  yLines)
{
	return b3InternalSurfaceInsertControl
		((b3_spline *)Nurbs,q,Mult,ControlOffset,yLines,true);
}

/*************************************************************************
**                                                                      **
**                        control point appending (curve)               **
**                                                                      **
*************************************************************************/

static b3_bool b3InternalAppendControl(
	b3_spline *Spline,
	b3_bool    append,
	b3_f64     q,
	b3_count   Mult,
	b3_index   index,
	b3_bool    useNurbs)
{
	b3_vector   *Controls;
	b3_vector4D *Controls4D;
	b3_f32      *knots;
	b3_index     m,i,offset;
	b3_count     degree,KnotNum;
	b3_vector4D  start,diff;

	bspline_errno = BSPLINE_CLOSED;
	if (Spline->closed) return false;
	bspline_errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return false;

	m        =  Spline->control_num;
	KnotNum  =  Spline->knot_num;
	degree   =  Spline->degree;
	knots    =  Spline->knots;
	offset   =  Spline->offset;
	if (Mult > (degree + 1)) Mult = degree + 1;

	bspline_errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > Spline->control_max) return false;
	bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((KnotNum + Mult) > Spline->knot_max)    return false;
	bspline_errno = BSPLINE_OK;

	if (append)
	{
		if (q <= knots[KnotNum-1]) return false;
		for (i = 0;i < Mult;i++)
		{
			knots[KnotNum + i] = (b3_f32)q;
		}

		if (!useNurbs)
		{
			Controls = &Spline->controls[index];
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
			Controls4D = &((b3_nurbs *)Spline)->controls[index];
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
			Controls = &Spline->controls[index];
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
				knots[i] = (b3_f32)q;
				Controls[i * offset].x = start.x + (Mult - i) * diff.x;
				Controls[i * offset].y = start.y + (Mult - i) * diff.y;
				Controls[i * offset].z = start.z + (Mult - i) * diff.z;
			}
		}
		else
		{
			Controls4D = &((b3_nurbs *)Spline)->controls[index];

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
				knots[i] = (b3_f32)q;
				Controls4D[i * offset].x = start.x + (Mult - i) * diff.x;
				Controls4D[i * offset].y = start.y + (Mult - i) * diff.y;
				Controls4D[i * offset].z = start.z + (Mult - i) * diff.z;
				Controls4D[i * offset].w = start.w + (Mult - i) * diff.w;
			}
		}
	}
	Spline->knot_num    += Mult;
	Spline->control_num += Mult;
	return true;
}

b3_bool b3BSplineAppendControl(
	b3_spline *Spline,
	b3_bool    append,
	b3_f64     q,
	b3_count   Mult,
	b3_index   index)
{
	return b3InternalAppendControl (Spline,append,q,Mult,index,false);
}

b3_bool b3BNurbsAppendControl(
	b3_nurbs *Nurbs,
	b3_bool   append,
	b3_f64    q,
	b3_count  Mult,
	b3_index  index)
{
	return b3InternalAppendControl ((b3_spline *)Nurbs,append,q,Mult,index,true);
}

/*************************************************************************
**                                                                      **
**                        control point appending (surface)             **
**                                                                      **
*************************************************************************/

static b3_bool b3InternalSurfaceAppendControl(
	b3_spline *Spline,
	b3_bool    append,
	b3_f64     q,
	b3_count   Mult,
	b3_index   skipOffset,
	b3_index   lines,
	b3_bool    useNurbs)
{
	b3_vector   *Controls;
	b3_vector4D *Controls4D;
	b3_f32      *knots;
	b3_index    m,i,offset,y;
	b3_count    degree,KnotNum;
	b3_vector4D start,diff;

	bspline_errno = BSPLINE_CLOSED;
	if (Spline->closed) return false;
	bspline_errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1) return false;

	m        = Spline->control_num;
	KnotNum  = Spline->knot_num;
	degree   = Spline->degree;
	knots    = Spline->knots;
	offset   = Spline->offset;
	if (Mult > (degree + 1)) Mult = degree + 1;

	bspline_errno = BSPLINE_TOO_FEW_MAXCONTROLS;
	if ((m       + Mult) > Spline->control_max) return false;
	bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
	if ((KnotNum + Mult) > Spline->knot_max)    return false;
	bspline_errno = BSPLINE_OK;

	if (append)
	{
		if (q <= knots[KnotNum-1]) return false;
		for (i=0;i<Mult;i++)
		{
			knots[KnotNum + i] = (b3_f32)q;
		}

		if (!useNurbs)
		{
			Controls = Spline->controls;
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
			Controls4D = ((b3_nurbs *)Spline)->controls;
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
		for (i = 0;i <  Mult;i++)
		{
			knots[i] = (b3_f32)q;
		}

		if (!useNurbs)
		{
			Controls = Spline->controls;
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
			Controls4D = ((b3_nurbs *)Spline)->controls;
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
	Spline->knot_num    += Mult;
	Spline->control_num += Mult;
	return true;
}

b3_bool b3BSplineSurfaceAppendControl(
	b3_spline *Spline,
	b3_bool    append,
	b3_f64     q,
	b3_count   Mult,
	b3_index   skipOffset,
	b3_index   lines)
{
	return b3InternalSurfaceAppendControl
		(Spline,append,q,Mult,skipOffset,lines,false);
}

b3_bool b3BNurbsSurfaceAppendControl(
	b3_nurbs  *Nurbs,
	b3_bool    append,
	b3_f64     q,
	b3_count   Mult,
	b3_index   skipOffset,
	b3_index   lines)
{
	return b3InternalSurfaceAppendControl
		((b3_spline *)Nurbs,append,q,Mult,skipOffset,lines,true);
}

/*************************************************************************
**                                                                      **
**                        de Boor routines                              **
**                                                                      **
*************************************************************************/

b3_index b3Spline::b3DeBoor(
	b3_vector *point,
	b3_index   index)
{
	b3_f64	 q,qStep;
	b3_index i;

	if (closed)
	{
		q     =  knots[0];
		qStep = (knots[control_num] - q - EPSILON) / (b3_f32)subdiv;

		for (i = 0;i <= subdiv;i++)
		{
			b3DeBoorClosed (point++,index,q);
			q += qStep;
		}
	}
	else
	{
		q     =  knots[degree];
		qStep = (knots[control_num] - q - EPSILON) / (b3_f32)subdiv;

		for (i = 0;i <= subdiv;i++)
		{
			b3DeBoorOpened (point++,index,q);
			q += qStep;
		}
	}

	return i;
}

b3_index b3DeBoorNurbs(
	b3_nurbs  *Nurbs,
	b3_vector *point,
	b3_index   index)
{
	b3_f32   *knots;
	b3_f64    q,qStep;
	b3_index  SubDiv,i,pos;
	b3_f64    coeffs[B3_MAX_DEGREE + 1];

	SubDiv = Nurbs->subdiv;
	knots  = Nurbs->knots;
	q      =  knots[Nurbs->closed ? 0 : Nurbs->degree];
	qStep  = (knots[Nurbs->control_num] - q - EPSILON) / (b3_f32)Nurbs->subdiv;

	for (i = 0;i <= SubDiv;i++)
	{
		pos = ((b3Spline *)Nurbs)->b3Mansfield (coeffs,q);
		b3MansfieldNurbsVector (Nurbs,point++,coeffs,pos,index);
		q += qStep;
	}
	return i;
}

b3_count b3Spline::b3DeBoorControl(
	b3_vector *point,
	b3_index   index)
{
	b3_index  i;

	if (closed)
	{
		for (i = 0;i < control_num;i++)
		{
			b3DeBoorClosed(point++,index,knots[i]);
		}
		return control_num;
	}
	else
	{
		for (i = degree;i <= control_num;i++)
		{
			b3DeBoorOpened(point++,index,knots[i]);
		}
		return control_num - degree + 1;
	}
}

b3_count b3DeBoorSurfaceControl(
	b3Spline *controlSpline,
	b3Spline *curveSpline,
	b3_vector *point)
{
	b3_f32   *knots;
	b3_index  i,x,end,index;
	b3_count  ControlNum;
	b3_f64    it[B3_MAX_DEGREE + 1];

	ControlNum = B3_BSPLINE_SEGMENTKNOTS(curveSpline);
	knots      = curveSpline->knots;
	end        = curveSpline->control_num;
	if (!curveSpline->closed) end++;

	for (i = (curveSpline->closed ? 0 : curveSpline->degree);i < end;i++)
	{
		index = curveSpline->b3Mansfield (it,knots[i]);
		for (x = 0;x < controlSpline->control_num;x++)
		{
			curveSpline->b3MansfieldVector (&point[x * ControlNum],it,
				index,x * controlSpline->offset);
		}
		point++;
	}
	return ControlNum;
}
