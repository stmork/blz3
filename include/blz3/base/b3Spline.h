/*
**
**	$Filename:	b3Spline.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III Spline proto types
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_BASE_SPLINE_H
#define B3_BASE_SPLINE_H

#include "blz3/b3Types.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Some base defines                             **
**                                                                      **
*************************************************************************/

#define B3_MAX_CONTROLS   32
#define B3_MAX_SUBDIV     64
#define B3_MAX_DEGREE    (B3_MAX_CONTROLS)
#define B3_MAX_KNOTS    ((B3_MAX_CONTROLS) + (B3_MAX_DEGREE) + 1)

#define B3_BSPLINE_SEGMENTS(s) ((s)->closed ?\
	(s)->control_num :\
	(s)->control_num - (s)->degree)

#define B3_BSPLINE_SEGMENTKNOTS(s) ((s)->closed ?\
	(s)->control_num :\
	(s)->control_num - (s)->degree + 1)

#define B3_BSPLINE_EPSILON 0.0001

/*************************************************************************
**                                                                      **
**                        error codes                                   **
**                                                                      **
*************************************************************************/

enum b3_bspline_error
{
	BSPLINE_OK = 0,
	BSPLINE_TOO_MUCH_CONTROLS,
	BSPLINE_TOO_FEW_CONTROLS,
	BSPLINE_TOO_FEW_MAXKNOTS,
	BSPLINE_TOO_FEW_MAXCONTROLS,
	BSPLINE_MISSING_KNOTS,
	BSPLINE_CLOSED,
	BSPLINE_TOO_LOW_MULTIPLICATION
};

extern b3_bspline_error bspline_errno;

/*************************************************************************
**                                                                      **
**                        Simple data structure containing              **
**                        B spline values                               **
**                                                                      **
*************************************************************************/

template<class VECTOR> struct b3_spline_template
{
	b3_count     control_num,knot_num;
	b3_count     degree,subdiv;         // values of B-Spline
	b3_count     control_max,knot_max;  // max. values
	b3_index     offset;                // offset of controls
	b3_bool      closed;                // open/close curve
	VECTOR      *controls;              // control sequence
	b3_f32      *knots;                 // knot sequence
};

typedef b3_spline_template<b3_vector>   b3_spline;
typedef b3_spline_template<b3_vector4D> b3_nurbs;

template<class VECTOR> class b3SplineTemplate : public b3_spline_template<VECTOR>
{

public:
	b3SplineTemplate<VECTOR>()
	{
		control_max = B3_MAX_CONTROLS;
		knot_max    = B3_MAX_KNOTS;
		degree      = 0;
		control_num = 0;
		knot_num    = 0;
		offset      = 1;
		controls    = null;
		knots       = null;
		subdiv      = B3_MAX_SUBDIV;
		closed      = false;
	}

	b3_bool b3InitCurve(const b3_count Degree,const b3_count ControlNum,const b3_bool Closed)
	{
		b3_index  i;

		if (ControlNum > control_max)
		{
			bspline_errno = BSPLINE_TOO_MUCH_CONTROLS;
			return false;
		}
		if ((ControlNum + Degree + 1) > knot_max)
		{
			bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		if (knots == null)
		{
			bspline_errno = BSPLINE_MISSING_KNOTS;
			return false;
		}
		bspline_errno = BSPLINE_OK;

		degree      = Degree;
		control_num = ControlNum;
		knot_num    = ControlNum + Degree + 1;
		closed      = Closed;

		// Create knot vector
		for (i=0;i < knot_max;i++)
		{
			knots[i] = (b3_f32)i;
		}
		return true;
	}

	b3_bool b3ThroughEndControl()
	{
		b3_f64    start,end;
		b3_index  i;

		if (closed)
		{
			bspline_errno = BSPLINE_CLOSED;
			return false;
		}
		bspline_errno = BSPLINE_OK;

		knot_num = control_num + degree + 1;
		start    = knots[degree];
		end      = knots[control_num];

		for (i = 0;i < degree;i++)
		{
			knots[i]                   = (b3_f32)start;
			knots[control_num + i + 1] = (b3_f32)end;
		}
		return true;
	}

	b3_bool b3ToBezier()
	{
		b3_index i;

		if  (closed)
		{
			bspline_errno = BSPLINE_CLOSED;
			return false;
		}
		if ((control_num * 2) > knot_max)
		{
			bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = BSPLINE_OK;

		knot_num = control_num * 2;
		degree   = control_num - 1;
		for (i = 0;i < control_num;i++)
		{
			knots[i] = 0;
		}
		for (i = control_num;i < knot_num;i++)
		{
			knots[i] = 1;
		}

		return true;
	}

	b3_bool b3Degree(const b3_count newDegree)
	{
		b3_f32	  start,end;
		b3_index  i,diff;

		bspline_errno = BSPLINE_OK;
		if (degree == newDegree)
		{
			return true;
		}
		if (degree >= control_num)
		{
			bspline_errno = BSPLINE_TOO_FEW_CONTROLS;
			return false;
		}
		if ((control_num + newDegree + 1) > knot_max)
		{
			bspline_errno = BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}

		if (newDegree < degree)     /* decreasing degree */
		{
			diff = degree - newDegree;
			end = knots[control_num];
			for (i = 0;i <  diff;i++)
			{
				knots[control_num+i] = end+i;
			}
			for (i = 0;i <= control_num;i++)
			{
				knots[i + newDegree] = knots[i + degree];
			}
			if (closed)
			{
				start = knots[newDegree];
				end   = knots[control_num + newDegree];
				for (i = 0;i < newDegree;i++)
				{
					knots[i] = knots[control_num + i] - end + start;
				}
				start = -knots[0];
				for (i = newDegree + control_num;i >= 0;i--)
				{
					knots[i] += start;
				}
			}
		}
		else                             /* increasing degree */
		{
			for (i = control_num;i >= 0;i--)
			{
				knots[i + newDegree] = knots[i + degree];
			}
			if (closed)
			{
				start = knots[newDegree];
				end   = knots[control_num + newDegree];
				for (i = 0;i < newDegree;i++)
				{
					knots[i] = knots[control_num + i] - end + start;
				}
				start = -knots[0];
				for (i = newDegree + control_num;i>=0;i--)
				{
					knots[i] += start;
				}
			}
		}

		knot_num = control_num + newDegree + 1;
		degree   = newDegree;
		return true;
	}

	b3_index  b3DeBoor(VECTOR *point,b3_index index)
	{
		b3_f64	 q,qStep;
		b3_index i;

		if (closed)
		{
			q     =  knots[0];
			qStep = (knots[control_num] - q - B3_BSPLINE_EPSILON) / (b3_f64)subdiv;

			for (i = 0;i <= subdiv;i++)
			{
				b3DeBoorClosed (point++,index,q);
				q += qStep;
			}
		}
		else
		{
			q     =  knots[degree];
			qStep = (knots[control_num] - q - B3_BSPLINE_EPSILON) / (b3_f64)subdiv;

			for (i = 0;i <= subdiv;i++)
			{
				b3DeBoorOpened (point++,index,q);
				q += qStep;
			}
		}

		return i;
	}

	b3_index  b3DeBoorControl(VECTOR *point,b3_index index)
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

	// The routine Mansfield computes the basis coefficents for the
	// appropriate control points and returns the highest index of
	// the corresponding control point. This routine is independend
	// of the spline type (e.g. openend or closed).
	// ------------------------------------------------------------
	// it:     array where to store the basis function coefficents
	// qStart: parameter value inside the curve
	b3_index  b3Mansfield(b3_f64 *it,b3_f64 qStart)
	{
		b3_index  l,i,j,k;
		b3_f64	  r,denom,q,diff;

		if (closed)
		{
			diff    = knots[control_num] - knots[0];
			if  ((i = iFind (qStart)) >= control_num)
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
			i      = iFind (qStart);
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

	// This routine computes the real curve point according to the basis
	// coefficients computed by Mansfield(). The value "i" is the knot
	// index returned by b3Mansfield(). The start index is like the b3DeBoor-
	// routines.
	// ------------------------------------------------------------------
	// point:  where to store the curve point
	// it:     basis coefficients computed by b3Mansfield()
	// i:      knot index returned by b3Mansfield()
	// index:  start index of control points
	void b3MansfieldVector(VECTOR *point,b3_f64 *it,b3_index i,b3_index index)
	{
		b3_index  l,j;
		VECTOR   *ctrls;

		b3Vector::b3Init(point);
		if (closed)
		{
			j = i;
			for (l = degree;l >= 0;l--)
			{
				b3Vector::b3LinearCombine(point,it[l],&controls[j * offset + index],point);
				if (--j < 0)
				{
					j += control_num;
				}
			}
		}
		else
		{
			ctrls = &controls[i * offset + index];
			for (l = degree;l >= 0;l--)
			{
				b3Vector::b3LinearCombine(point,it[l],ctrls,point);
				ctrls -= offset;
			}
		}
	}

	b3_index b3DeBoorOpened (VECTOR *point,b3_index index,b3_f64 q)
	{
		b3_index  l,i,j;
		b3_f64    r,denom;
		b3_f64    it[B3_MAX_DEGREE + 1];

		i = iFind (q);

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

		b3Vector::b3Init(point);
		j = i * offset + index;
		for (l = degree;l >= 0;l--)
		{
			b3Vector::b3LinearCombine(point,it[l],&controls[j],point);
			j -= offset;
		}
		return i;
	}
	
	b3_index  b3DeBoorClosed (VECTOR *point,b3_index index,b3_f64 qStart)
	{
		b3_index   l,i,j,k;
		b3_f64     r,denom,diff,q;
		b3_f64     it[B3_MAX_DEGREE + 1];

		diff    = knots[control_num] - knots[0];
		if  ((i = iFind (qStart)) >= control_num)
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

		b3Vector::b3Init(point);
		j = i;
		for (l = degree;l >= 0;l--)
		{
			b3Vector::b3LinearCombine(point,it[l],&controls[j * offset + index],point);
			if (--j < 0)
			{
				j += control_num;
			}
		}
		return i;
	}
	
	static b3_count b3DeBoorSurfaceControl(
		b3SplineTemplate<VECTOR> *controlSpline,
		b3SplineTemplate<VECTOR> *curveSpline,
		VECTOR                   *point)
	{
		b3_f32   *knot_ptr;
		b3_index  i,x,end,index;
		b3_count  ControlNum;
		b3_f64    it[B3_MAX_DEGREE + 1];

		ControlNum = B3_BSPLINE_SEGMENTKNOTS(curveSpline);
		knot_ptr   = curveSpline->knots;
		end        = curveSpline->control_num;
		if (!curveSpline->closed) end++;

		for (i = (curveSpline->closed ? 0 : curveSpline->degree);i < end;i++)
		{
			index = curveSpline->b3Mansfield(it,knot_ptr[i]);
			for (x = 0;x < controlSpline->control_num;x++)
			{
				curveSpline->b3MansfieldVector (&point[x * ControlNum],it,
					index,x * controlSpline->offset);
			}
			point++;
		}
		return ControlNum;
	}

private:
	inline b3_index iFind(b3_f64 q)
	{
		b3_index i;
		b3_count max = (closed ? control_num : knot_num) - 1;

		for (i = 0;i < max;i++)
		{						 
			if ((knots[i] <= q) && (q < knots[i+1]))
			{
				return i;
			}
		}
		if (!closed)
		{
			i--;
			while (knots[i-1] == knots[i])
			{
				i--;
			}
		}
		return i;
	}
};

typedef b3SplineTemplate<b3_vector>   b3Spline;
typedef b3SplineTemplate<b3_vector4D> b3Nurbs;

/*************************************************************************
**                                                                      **
**                        support routines                              **
**                                                                      **
*************************************************************************/

b3_bool	 b3BSplineInsertControl       (b3_spline *Spline,b3_f64 q,b3_count Mult,b3_index index);
b3_bool	 b3BSplineSurfaceInsertControl(b3_spline *Spline,b3_f64 q,b3_count Mult,b3_index uIndex,b3_index vIndex);
b3_bool	 b3BSplineAppendControl       (b3_spline *Spline,b3_bool,b3_f64,b3_index,b3_index);
b3_bool	 b3BSplineSurfaceAppendControl(b3_spline *Spline,b3_bool,b3_f64,b3_index,b3_index,b3_index);

b3_bool	 b3BNurbsInsertControl       (b3_nurbs *Nurbs,b3_f64,b3_index,b3_index);
b3_bool	 b3BNurbsSurfaceInsertControl(b3_nurbs *Nurbs,b3_f64,b3_index,b3_index,b3_index);
b3_bool	 b3BNurbsAppendControl       (b3_nurbs *Nurbs,b3_bool,b3_f64,b3_index,b3_index);
b3_bool	 b3BNurbsSurfaceAppendControl(b3_nurbs *Nurbs,b3_bool,b3_f64,b3_index,b3_index,b3_index);

#endif
