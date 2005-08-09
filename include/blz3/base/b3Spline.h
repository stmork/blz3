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

#include "blz3/b3Config.h"
#include "blz3/base/b3Math.h"

/*************************************************************************
**                                                                      **
**                        Some basic defines                            **
**                                                                      **
*************************************************************************/

#define B3_MAX_CONTROLS   32
#define B3_MAX_SUBDIV     64
#define B3_MAX_DEGREE    (B3_MAX_CONTROLS)
#define B3_MAX_KNOTS    ((B3_MAX_CONTROLS) + (B3_MAX_DEGREE) + 1)

typedef b3_f32   b3_knot;
typedef b3_knot  b3_knot_vector[B3_MAX_KNOTS];
typedef b3_knot *b3_knots;

#define B3_BSPLINE_EPSILON 0.0001

#define B3_BSPLINE_SEGMENTS(s)     ((s)->b3GetSegmentCount())
#define B3_BSPLINE_SEGMENTKNOTS(s) ((s)->b3GetSegmentKnotCount())
#define B3_BSPLINE_GEOMETRY(s,c,k) {\
	(s)->controls = c;\
	(s)->knots    = k;\
	(s)->control_max = (sizeof(c) / sizeof(c[0]));\
	(s)->knot_max    = (sizeof(k) / sizeof(b3_knot)); }

/*************************************************************************
**                                                                      **
**                        error codes                                   **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists spline error codes.
 */
enum b3_bspline_error
{
	B3_BSPLINE_OK = 0,                 //!< Spline handling is OK.
	B3_BSPLINE_TOO_MUCH_CONTROLS,      //!< Not enough memory for spline control points.
	B3_BSPLINE_TOO_FEW_CONTROLS,       //!< Too few controls for the given spline degree.
	B3_BSPLINE_TOO_FEW_MAXKNOTS,       //!< Too few knots for the goven spline degree.
	B3_BSPLINE_TOO_FEW_MAXCONTROLS,
	B3_BSPLINE_MISSING_KNOTS,
	B3_BSPLINE_CLOSED,
	B3_BSPLINE_TOO_LOW_MULTIPLICATION
};

/*************************************************************************
**                                                                      **
**                        Simple data structure containing              **
**                        B spline values                               **
**                                                                      **
*************************************************************************/

/**
 * This class implements spline vector operations.
 */
class B3_PLUGIN b3SplineVector
{
public:
	/**
	 * This method initializes the given vector to 0.
	 */
	static inline void b3Clear(b3_vector *vector)
	{
		vector->x =
		vector->y =
		vector->z = 0;
	}

	/**
	 * This method initializes the given vector to 0.
	 */
	static inline void b3Clear(b3_vector4D *vector)
	{
		vector->x =
		vector->y =
		vector->z =
		vector->w = 0;
	}

	static inline void b3Homogenize(b3_vector *vector)
	{
	}

	static inline void b3Homogenize(b3_vector4D *vector)
	{
		register b3_f32 w;

		B3_ASSERT(vector->w != 0);
		w = 1.0 / vector->w;
		vector->x = (b3_f32)(vector->x * w);
		vector->y = (b3_f32)(vector->y * w);
		vector->z = (b3_f32)(vector->z * w);
	}

	static inline void b3Sub(
		const b3_vector *aVec,
		const b3_vector *bVec,
		      b3_vector *result)
	{
#ifdef B3_SSE
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			r[i] = a[i] - b[i];
		}
#else
		result->x = aVec->x - bVec->x;
		result->y = aVec->y - bVec->y;
		result->z = aVec->z - bVec->z;
#endif
	}

	static inline void b3Sub(
		const b3_vector4D *aVec,
		const b3_vector4D *bVec,
		      b3_vector4D *result)
	{
#ifdef B3_SSE1
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for(b3_loop i = 0;i < 4;i++)
		{
			r[i] = a[i] - b[i];
		}
#else
		result->x = aVec->x - bVec->x;
		result->y = aVec->y - bVec->y;
		result->z = aVec->z - bVec->z;
		result->w = aVec->w - bVec->w;
#endif
	}

	static inline void b3AddScaled(
		const b3_f32     factor,
		const b3_vector *offset,
		      b3_vector *vector)
	{
#ifdef B3_SSE
		const b3_f32 *o = &offset->x;
		      b3_f32 *v = &vector->x;

		for(b3_loop i = 0;i < 3;i++)
		{
			v[i] = v[i] + factor * o[i];
		}
#else
		vector->x = (b3_f32)(vector->x + factor * offset->x);
		vector->y = (b3_f32)(vector->y + factor * offset->y);
		vector->z = (b3_f32)(vector->z + factor * offset->z);
#endif
	}

	static inline void b3AddScaled(
		const b3_f32       factor,
		const b3_vector4D *offset,
		      b3_vector4D *vector)
	{
#ifdef B3_SSE1
		const b3_f32 *o = &offset->x;
		      b3_f32 *v = &vector->x;

		for(b3_loop i = 0;i < 4;i++)
		{
			v[i] = v[i] + factor * o[i];
		}
#else
		vector->x = (b3_f32)(vector->x + factor * offset->x);
		vector->y = (b3_f32)(vector->y + factor * offset->y);
		vector->z = (b3_f32)(vector->z + factor * offset->z);
		vector->w = (b3_f32)(vector->w + factor * offset->w);
#endif
	}

	static inline void b3LinearCombine(
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_f32     b,
		      b3_vector *result)
	{
		result->x = (b3_f32)(aVec->x + b * bVec->x);
		result->y = (b3_f32)(aVec->y + b * bVec->y);
		result->z = (b3_f32)(aVec->z + b * bVec->z);
	}

	static inline void b3LinearCombine(
		const b3_vector4D *aVec,
		const b3_vector4D *bVec,
		const b3_f32       f,
		      b3_vector4D *result)
	{
#ifdef B3_SSE1
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for (b3_loop i = 0;i < 4;i++)
		{
			r[i] = a[i] + f * b[i];
		}
#else
		result->x = (b3_f32)(aVec->x + f * bVec->x);
		result->y = (b3_f32)(aVec->y + f * bVec->y);
		result->z = (b3_f32)(aVec->z + f * bVec->z);
		result->w = (b3_f32)(aVec->w + f * bVec->w);
#endif
	}

	static inline void b3Weight(b3_vector *result,
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_f32     f)
	{
		result->x = aVec->x + f * (bVec->x - aVec->x);
		result->y = aVec->y + f * (bVec->y - aVec->y);
		result->z = aVec->z + f * (bVec->z - aVec->z);
	}

	static inline void b3Weight(b3_vector4D *result,
		const b3_vector4D *aVec,
		const b3_vector4D *bVec,
		const b3_f32       f)
	{
#ifdef B3_SSE1
		const b3_f32 *a = &aVec->x;
		const b3_f32 *b = &bVec->x;
		      b3_f32 *r = &result->x;

		for (b3_loop i = 0;i < 4;i++)
		{
			r[i] = a[i] + f * (b[i] - a[i]);
		}
#else
		result->x = aVec->x + f * (bVec->x - aVec->x);
		result->y = aVec->y + f * (bVec->y - aVec->y);
		result->z = aVec->z + f * (bVec->z - aVec->z);
		result->w = aVec->w + f * (bVec->w - aVec->w);
#endif
	}
};

/*************************************************************************
**                                                                      **
**                        The B-Spline template class                   **
**                                                                      **
*************************************************************************/

/**
 * This structure template defines the basic properties of a spline
 */
template<class VECTOR> struct b3_spline_template
{
	b3_count        control_num,knot_num;
	b3_count        degree,subdiv;         // values of B-Spline
	b3_count        control_max,knot_max;  // max. values
	b3_index        offset;                // offset of controls
	b3_bool         closed;                // open/close curve
	VECTOR         *controls;              // control sequence
	b3_knots        knots;                 // knot sequence
};

typedef b3_spline_template<b3_vector>   b3_spline;
typedef b3_spline_template<b3_vector4D> b3_nurbs;

/**
 * This template class defines the behaviour of splines.
 */
template<class VECTOR> class B3_PLUGIN b3SplineTemplate : public b3_spline_template<VECTOR>
{
public:
	static b3_bspline_error bspline_errno;

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

	inline b3_count b3GetSegmentCount()
	{
		return closed ? control_num : control_num - degree;
	}

	inline b3_count b3GetSegmentKnotCount()
	{
		return closed ? control_num : control_num - degree + 1;
	}

	inline b3_f64 b3ArcLengthParameter(
		b3_count n,
		b3_f64   scale = 1)
	{
		B3_ASSERT(!closed);
		return b3ArcLengthParameter(n,degree,control_num,scale);
	}

	static inline b3_f64 b3ArcLengthParameter(
		b3_count n,
		b3_count Degree,
		b3_count ControlNum,
		b3_f64   scale = 1)
	{
		b3_f64 result;
		b3_u32 sum,back;

		if (((Degree - 1) * 2) < ControlNum)
		{
			// No arc length parametrization
			result = (b3_f64)n / (ControlNum - 1);
		}
		else
		{
			if (n <= Degree)
			{
				// first ramp
				sum = (n * (n + 1)) >> 1;
			}
			else
			{
				sum = ((Degree * (Degree + 1)) >> 1) + (n - Degree) * Degree;
				if (n > (ControlNum - Degree))
				{
					// last ramp
					back = Degree - (ControlNum - n);
					sum -= ((back * (back + 1)) >> 1);
				}
			}
			result = (b3_f64)n / (Degree * (ControlNum - Degree));
		}
		return result * scale;
	}

	b3_bool b3InitCurve(const b3_count Degree,const b3_count ControlNum,const b3_bool Closed)
	{
		b3_index  i;

		// Make some checks
		if (ControlNum > control_max)
		{
			bspline_errno = B3_BSPLINE_TOO_MUCH_CONTROLS;
			return false;
		}
		if ((ControlNum + Degree + 1) > knot_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		if (knots == null)
		{
			bspline_errno = B3_BSPLINE_MISSING_KNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		// Copy values
		degree      = Degree;
		control_num = ControlNum;
		knot_num    = ControlNum + Degree + 1;
		closed      = Closed;

		// Create knot vector
		for (i=0;i < knot_max;i++)
		{
			knots[i] = (b3_f32)i;
		}

		// Addjust knot vector
		if (!closed)
		{
			b3ThroughEndControl();
		}

		// Adjust sub division
		b3OptimizeSubdivision();

		return true;
	}

	void b3OptimizeSubdivision()
	{
		b3_count i,segments;

		i = 8;
		segments = b3GetSegmentCount();
		do
		{
			subdiv = segments * i--;
		}
		while (subdiv > B3_MAX_SUBDIV);
	}

	void b3ClearControls(b3_index index = 0)
	{
		b3_index i;

		for (i = 0;i < control_num;i++)
		{
			b3SplineVector::b3Clear(&controls[index]);
			index += offset;
		}
	}

	b3_bool b3ThroughEndControl()
	{
		b3_knot   start,end;
		b3_index  i;

		if (closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		knot_num = control_num + degree + 1;
		start    = knots[degree];
		end      = knots[control_num];

		for (i = 0;i < degree;i++)
		{
			knots[i]                   = start;
			knots[control_num + i + 1] = end;
		}
		return true;
	}

	b3_bool b3ToBezier()
	{
		b3_index i;

		if  (closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if ((control_num * 2) > knot_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

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
		b3_knot	  start,end;
		b3_index  i,diff;

		bspline_errno = B3_BSPLINE_OK;
		if (degree == newDegree)
		{
			return true;
		}
		if (degree >= control_num)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_CONTROLS;
			return false;
		}
		if ((control_num + newDegree + 1) > knot_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
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

	b3_index  b3DeBoor(VECTOR *point,b3_index index = 0)
	{
		b3_knot	 q,qStep;
		b3_index i;

		if (closed)
		{
			q     =  knots[0];
			qStep = (knots[control_num] - q - B3_BSPLINE_EPSILON) / (b3_knot)subdiv;

			for (i = 0;i <= subdiv;i++)
			{
				b3DeBoorClosed (point++,index,q);
				q += qStep;
			}
		}
		else
		{
			q     =  knots[degree];
			qStep = (knots[control_num] - q - B3_BSPLINE_EPSILON) / (b3_knot)subdiv;

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

	/**
	 * The routine Mansfield computes the basis coefficents for the
	 * appropriate control points and returns the highest index of
	 * the corresponding control point. This routine is independend
	 * of the spline type (e.g. openend or closed).
	 *
	 * @param it     Array where to store the basis function coefficents
	 * @param qStart Parameter value inside the curve
	 */
	b3_index  b3Mansfield(b3_f64 *it,b3_f64 qStart)
	{
		b3_index  l,i,j,k;
		b3_knot	  r,denom,q,diff;

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
					denom      = knots[k+l] - knots[k];
					r          = (denom != 0 ? (q - knots[k]) / denom : 0);
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
					denom      = knots[i-j+l]      - knots[i-j];
					r          = (denom != 0 ? (qStart - knots[i-j]) / denom : 0);
					it[l-j]   += r * it[l-j-1];
					it[l-j-1] *= (1 - r);
				}
			}
		}
		return i;
	}

	/**
	 * This routine computes the real curve point according to the basis
	 * coefficients computed by Mansfield(). The value "i" is the knot
	 * index returned by b3Mansfield(). The start index is like the b3DeBoor-
	 * routines.
	 *
	 * @param point  Where to store the curve point
	 * @param it     Basis coefficients computed by b3Mansfield()
	 * @param i      Knot index returned by b3Mansfield()
	 * @param index  Start index of control points
	 */
	void b3MansfieldVector(VECTOR *point,b3_f64 *it,b3_index i,b3_index index)
	{
		b3_index  l,j;
		VECTOR   *ctrls;

		b3SplineVector::b3Clear(point);
		if (closed)
		{
			j = i;
			for (l = degree;l >= 0;l--)
			{
				b3SplineVector::b3AddScaled(it[l],&controls[j * offset + index],point);
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
				b3SplineVector::b3AddScaled(it[l],ctrls,point);
				ctrls -= offset;
			}
		}
		b3SplineVector::b3Homogenize(point);
	}

	b3_index b3DeBoorOpened (VECTOR *point,b3_index index,b3_knot q)
	{
		b3_index  l,i,j;
		b3_knot   r;
		b3_f64    denom;
		b3_f64    it[B3_MAX_DEGREE + 1];

		i = iFind (q);

		it[0]  = 1;
		for (l = 1;l <= degree;l++)
		{
			it[l]  = 0;
			for (j = 0;j < l;j++)
			{
				denom      = knots[i-j+l] - knots[i-j];
				r          = (denom != 0 ? (q - knots[i-j]) / denom : 0);
				it[l-j]   += r * it[l-j-1];
				it[l-j-1] *= (1 - r);
			}
		}

		b3SplineVector::b3Clear(point);
		j = i * offset + index;
		for (l = degree;l >= 0;l--)
		{
			b3SplineVector::b3AddScaled(it[l],&controls[j],point);
			j -= offset;
		}
		b3SplineVector::b3Homogenize(point);
		return i;
	}

	b3_index  b3DeBoorClosed (VECTOR *point,b3_index index,b3_knot qStart)
	{
		b3_index   l,i,j,k;
		b3_knot    r,diff,q;
		b3_f64     denom;
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
				denom      = knots[k+l] - knots[k];
				r          = (denom != 0 ? (q - knots[k]) / denom : 0);
				it[l-j]   += r * it[l-j-1];
				it[l-j-1] *= (1 - r);
				if (--k < 0) /* check underflow of knots */
				{
					k += control_num;
					q += diff;
				}
			}
		}

		b3SplineVector::b3Clear(point);
		j = i;
		for (l = degree;l >= 0;l--)
		{
			b3SplineVector::b3AddScaled(it[l],&controls[j * offset + index],point);
			if (--j < 0)
			{
				j += control_num;
			}
		}
		b3SplineVector::b3Homogenize(point);
		return i;
	}
	
	static b3_count b3DeBoorSurfaceControl(
		b3SplineTemplate<VECTOR> *controlSpline,
		b3SplineTemplate<VECTOR> *curveSpline,
		VECTOR                   *point)
	{
		b3_knots  knot_ptr;
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

	b3_bool b3InsertCurveControl(
		b3_knot    q,
		b3_count   Mult,
		b3_index   index)
	{
		b3_index l,m,i;
		b3_count KnotNum,Count;
		b3_knot  start,end;
		VECTOR   o[B3_MAX_CONTROLS + 1]; /* buffer for knot insertion */

		if (Mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        =  control_num;
		KnotNum  =  knot_num;

		if ((m + Mult) > control_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > knot_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = BSPLINE_OK;

		if (closed) for (Count = 0;Count < Mult;Count++)
		{
			start = knots[0];
			end   = knots[m];

			b3InsertDeBoorClosed (o,index,q);

			// insert new knot
			for (l = KnotNum;l > i;l--)
			{
				knots[l] = knots[l-1];
			}
			knots[i+1]  = (b3_knot)q;
			knot_num    = ++KnotNum;
			control_num = ++m;
			for (l = 0;l <= degree;l++)
			{
				knots[l+m] = knots[l] - start + end;
			}

			// insert o[x] into control points
			for (l = m - 1;l > i;l--)
			{
				controls[l * offset] = controls[(l - 1) * offset];
			}

			for (l = i - degree + 1;l <= i;l++)
			{
				controls[((l + m) % m) * offset] = o[(l + (m - 1)) % (m - 1)];
			}
		}
		else for (Count = 0;Count < Mult;Count++)
		{
			i = b3InsertDeBoorOpened (o,index,q);

				/* insert new knot */
			for (l = KnotNum;l > i;l--)
			{
				knots[l+1] = knots[l];
			}
			knots[i+1] = q;
			knot_num = ++KnotNum;

			// insert o[x] into control points
			for (l = m;l > i;l--)
			{
				controls[l * offset] = controls[(l - 1) * offset];
			}

			for (l = i - degree + 1;l <= i;l++)
			{
				controls[l * offset] = o[l];
			}

			control_num = ++m;
		}
		return true;
	}

	/**
	 * This routine inserts control points into a de Boor net. The index between
	 * two net lines is the amount of "ControlOffset". In one line there are
	 * inserted "Mult" knots with value "q". This procedure is repeated "yLines"
	 * times.
	 *
	 * @param q             Where to insert inside the knot vector
	 * @param Mult          How many insertions to be done
	 * @param ControlOffset Distance between to curves
	 * @param yLines       How many curves where to insert knots
	 */
	b3_bool b3InsertSurfaceControl(
		b3_knot    q,
		b3_count   Mult,
		b3_index   ControlOffset,
		b3_count   yLines)
	{
		b3_index  l,m,i = 0,index,y;
		b3_count  KnotNum,Count;
		b3_knot	  start,end;
		VECTOR   *Controls;
		VECTOR    o[B3_MAX_CONTROLS + 1]; /* buffer for knot insertion */

		bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
		if (Mult < 1)
		{
			return false;
		}

		m        =  control_num;
		KnotNum  =  knot_num;
		if (Mult > (degree + 1))
		{
			Mult = degree + 1;
		}

		if ((m + Mult) > control_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > knot_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (closed) for (Count = 0;Count < Mult;Count++)
		{
			start = knots[0];
			end   = knots[m];

			for (y = 0;y < yLines;y++)
			{
				index = y * ControlOffset;
				i = b3InsertDeBoorClosed (o,index,q);
				m++;

				Controls = &controls[index];

					/* insert o[x] into control points */
				for (l = m - 1;l > i;l--)
				{
					Controls[l * offset] = Controls[(l - 1) * offset];
				}
				for (l = i - degree + 1;l <= i;l++)
				{
					Controls[((l + m) % m) * offset] = o[(l + (m-1)) % (m-1)];
				}
				m--;
			}
				/* insert new knot */
			for (l = KnotNum;l > i;l--)
			{
				knots[l] = knots[l-1];
			}
			knots[i + 1] = q;
			knot_num     = ++KnotNum;
			control_num  = ++m;
			for (l = 0;l <= degree;l++)
			{
				knots[l + m] = knots[l] - start + end;
			}
		}
		else for (Count = 0;Count < Mult;Count++)
		{
			for (y = 0;y < yLines;y++)
			{
				index = y * ControlOffset;
				i = b3InsertDeBoorOpened (o,index,q);
				m++;

				// insert o[x] into control points
				Controls = &controls[index];
				for (l = m;l > i;l--)
				{
					Controls[l * offset] = Controls[(l - 1) * offset];
				}

				for (l = i - degree + 1;l <= i;l++)
				{
					Controls[l * offset] = o[l];
				}
				m--;
			}

				/* insert new knot */
			for (l = KnotNum;l > i;l--) knots[l+1] = knots[l];
			knots[i+1]  = q;
			knot_num    = ++KnotNum;
			control_num = ++m;
		}
		return true;
	}

	b3_bool b3AppendCurveControl(
		b3_bool    append,
		b3_knot    q,
		b3_count   Mult,
		b3_index   index)
	{
		VECTOR   *Controls;
		VECTOR    start,diff;
		b3_index  m,i;
		b3_count  KnotNum;

		if (closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if (Mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        = control_num;
		KnotNum  = knot_num;
		if (Mult > (degree + 1)) Mult = degree + 1;

		if ((m + Mult) > control_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > knot_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (append)
		{
			if (q <= knots[KnotNum-1])
			{
				return false;
			}
			for (i = 0;i < Mult;i++)
			{
				knots[KnotNum + i] = q;
			}

			Controls = &controls[index];
			start    = Controls[(m - 1) * offset];
			b3SplineVector::b3Sub(&start,&Controls[(m - 2) * offset],&diff);
			for (i = 0;i < Mult;i++)
			{
				b3SplineVector::b3LinearCombine(&start,i + 1,&diff,&Controls[(m + i) * offset]);
			}
		}
		else
		{
			if (q >= knots[0])
			{
				return false;
			}
			for (i = KnotNum - 1;i >= 0;i--)
			{
				knots[i+Mult] = knots[i];
			}

			Controls = &controls[index];
			start = Controls[0];
			b3SplineVector::b3Sub(&start,&Controls[offset],&diff);

			for (i = m - 1;i >= 0;i--)
			{
				Controls[(i + Mult) * offset] = Controls[i * offset];
			}

			for (i=0;i<Mult;i++)
			{
				knots[i] = q;
				b3SplineVector::b3LinearCombine(&start,&diff,Mult - i,&Controls[i * offset]);
			}
		}
		knot_num    += Mult;
		control_num += Mult;
		return true;
	}

	b3_bool b3AppendSurfaceControl(
		b3_bool    append,
		b3_knot    q,
		b3_count   Mult,
		b3_index   skipOffset,
		b3_index   lines)
	{
		VECTOR   *Controls;
		VECTOR    start,diff;
		b3_index  m,i,y;
		b3_count  KnotNum;

		if (closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if (Mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        = control_num;
		KnotNum  = knot_num;
		if (Mult > (degree + 1))
		{
			Mult = degree + 1;
		}

		if ((m       + Mult) > control_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > knot_max)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = BSPLINE_OK;

		if (append)
		{
			if (q <= knots[KnotNum-1])
			{
				return false;
			}
			for (i=0;i<Mult;i++)
			{
				knots[KnotNum + i] = q;
			}

			Controls = controls;
			for (y = 0;y < lines;y++)
			{
				start = Controls[(m - 1) * offset];
				b3SplineVector::b3Sub(&start,&Controls[(m - 2) * offset],&diff);
				for (i = 0;i < Mult;i++)
				{
					b3SplineVector::b3LinearCombine(&start,&diff,i+1,&Controls[(m + i) * offset]);
				}
				Controls += skipOffset;
			}
		}
		else
		{
			if (q >= knots[0])
			{
				return false;
			}

			for (i = KnotNum-1;i >= 0;i--)
			{
				knots[i+Mult] = knots[i];
			}
			for (i = 0;i <  Mult;i++)
			{
				knots[i] = q;
			}

			Controls = controls;
			for (y = 0;  y <  lines;y++)
			{
				start = Controls[0];
				b3SplineVector::b3Sub(&start,&Controls[offset],&diff);
				for (i = m - 1;i >= 0;i--)
				{
					Controls[(i + Mult) * offset] = Controls[i * offset];
				}

				for (i = 0;i < Mult;i++)
				{
					b3SplineVector::b3LinearCombine(&start,&diff,Mult - i,&Controls[i * offset]);
				}
				Controls += skipOffset;
			}
		}
		knot_num    += Mult;
		control_num += Mult;
		return true;
	}
private:
	/**
	 * This routine computes the first level of the de Boor algorithm. The
	 * computed points are used for the new control points needed for
	 * knot insertion. The control points are put into the o[]-array.
	 *
	 * @param ins   Recreated control points
	 * @param index Start index of curve
	 * @param q     Curve parameter to be inserted into knot (later)
	 */
	inline b3_index b3InsertDeBoorOpened(
		VECTOR   *ins,
		b3_index  index,
		b3_knot   q)
	{
		b3_index  l,i,j;
		b3_knot   r;
		b3_f64    Denom;
		VECTOR    it[B3_MAX_DEGREE + 1];

		i = iFind(q);
		j = i * offset + index;
		for (l = 0;l <= degree;l++)
		{
			it[l] = Nurbs->controls[j];
			j    -= Nurbs->offset;
		}

		ins += i;		/* loop for control point insertion */
		j    = i;
		for (l = 0;l < degree;l++)
		{
			Denom =   (knots[j+degree] - knots[j]);
			if (Denom != 0) r =     (q - knots[j]) / Denom;
			else            r =      0;
			b3SplineVector::b3Weight(&it[l],&it[l+1],&it[l],r);
			*ins-- = it[l];
			j--;
		}
		return i;
	}

	/**
	 * This routine computes the first level of the de Boor algorithm. The
	 * computed points are used for the new control points needed for
	 * knot insertion. The control points are put into the o[]-array.
	 *
	 * @paramins Recreated control points
	 * @param index Start index of curve
	 * @param q Curve parameter to be inserted into knot (later)
	 */
	inline b3_index b3InsertDeBoorClosed (
		VECTOR   *ins,
		b3_index  index,
		b3_knot   qStart)
	{
		b3_nurbs *Nurbs = (b3_nurbs *)Spline;
		b3_index  l,i,j,m;
		b3_knot   r,q,diff;
		b3_f64    Denom;
		VECTOR    it[B3_MAX_DEGREE + 1];

		m       = control_num;
		diff    = knots[m] - knots[0];

		if ((i = iFind (qStart)) >= m)
		{
			i -= m;
		}

		j = i;
		for (l = 0;l <= degree;l++)
		{
			it[l] = Nurbs->controls[j * Nurbs->offset + index];
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
			b3SplineVector::b3Weight(&it[l],&it[l+1],&it[l],r);
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

	inline b3_index iFind(b3_knot q)
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

#endif
