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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2002/03/02 15:24:35  sm
**	- Templetized splines (uhff).
**	- Prepared spline shapes for their creation.
**	  *** And now: Testing! Testing! Testing! ***
**
**	Revision 1.7  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.6  2001/10/13 09:56:44  sm
**	- Minor corrections
**	
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

b3_bspline_error bspline_errno = BSPLINE_OK;

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

static inline b3_index iFind(
	b3_f32	*knots,
	b3_f64	 q,
	b3_count max,
	b3_bool	 closed)
{
	b3_index i;

	for (i=0;i < (max - 1);i++)
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

/*************************************************************************
**                                                                      **
**                        control routines                              **
**                                                                      **
*************************************************************************/

static b3_bool b3InternalInsertControl (
	b3_spline *Spline,
	b3_f64     q,
	b3_count   Mult,
	b3_index   index,
	b3_bool    useNurbs)
{
	b3_vector   *Controls   = null;
	b3_vector4D *Controls4D = null;
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
			for (l = m - 1;l > i;l--)
			{
				Controls[l * Spline->offset] = Controls[(l - 1) * Spline->offset];
			}

			for (l=i-degree+1;l <= i;l++)
			{
				Controls[((l + m) % m) * Spline->offset].x = o[(l + (m-1)) % (m-1)].x;
				Controls[((l + m) % m) * Spline->offset].y = o[(l + (m-1)) % (m-1)].y;
				Controls[((l + m) % m) * Spline->offset].z = o[(l + (m-1)) % (m-1)].z;
			}
		}
		else
		{
			for (l = m - 1;l > i;l--)
			{
				Controls4D[l * Spline->offset] = Controls4D[(l - 1) * Spline->offset];
			}

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
			for (l = m;l > i;l--)
			{
				Controls[l * Spline->offset] = Controls[(l - 1) * Spline->offset];
			}

			for (l=i-degree+1;l <= i;l++)
			{
				Controls[l * Spline->offset].x = o[l].x;
				Controls[l * Spline->offset].y = o[l].y;
				Controls[l * Spline->offset].z = o[l].z;
			}
		}
		else
		{
			for (l = m;l > i;l--)
			{
				Controls4D[l * Spline->offset] = Controls4D[(l - 1) * Spline->offset];
			}

			for (l = i - degree + 1;l <= i;l++)
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
	b3_index     l,m,i = 0,index,y;
	b3_count     degree,KnotNum,Count;
	b3_f64	     start,end;
	b3_vector4D  o[B3_MAX_CONTROLS + 1]; /* buffer for knot insertion */

	bspline_errno = BSPLINE_TOO_LOW_MULTIPLICATION;
	if (Mult < 1)
	{
		return false;
	}

	m        =  Spline->control_num;
	KnotNum  =  Spline->knot_num;
	degree   =  Spline->degree;
	knots    =  Spline->knots;
	if (Mult > (degree + 1))
	{
		Mult = degree + 1;
	}

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
				for (l = m - 1;l > i;l--)
				{
					Controls[l * Spline->offset] = Controls[(l - 1) * Spline->offset];
				}

				for (l = i - degree + 1;l <= i;l++)
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
				for (l = m - 1;l > i;l--)
				{
					Controls4D[l * Spline->offset] = Controls4D[(l - 1) * Spline->offset];
				}
				for (l = i - degree+1;l <= i;l++)
				{
					Controls4D[((l + m) % m) * Spline->offset] = o[(l + (m-1)) % (m-1)];
				}
			}
			m--;
		}
			/* insert new knot */
		for (l = KnotNum;l > i;l--)
		{
			knots[l] = knots[l-1];
		}
		knots[i + 1]     = (b3_f32)q;
		Spline->knot_num = ++KnotNum;
		Spline->control_num = ++m;
		for (l = 0;l <= degree;l++)
		{
			knots[l + m] = (b3_f32)(knots[l] - start + end);
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
