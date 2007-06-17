/*
**
**	$Filename:	b3Spline.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1450 $
**	$Date: 2006-06-28 19:48:22 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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
	(s)->m_Controls = c;\
	(s)->m_Knots    = k;\
	(s)->m_ControlMax = (sizeof(c) / sizeof(c[0]));\
	(s)->m_KnotMax    = (sizeof(k) / sizeof(b3_knot)); }

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
	 * This method initializes the given scalar to 0.
	 */
	static inline void b3Clear(b3_f64 *scalar)
	{
		*scalar = 0;
	}

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

	/**
	 * This method does nothing.
	 */
	static inline void b3Homogenize(b3_f64 *vector)
	{
	}

	/**
	 * This method does nothing.
	 */
	static inline void b3Homogenize(b3_vector *vector)
	{
	}

	/**
	 * This method homogenizes the given vector.
	 *
	 * @param vector The vector to homogenize.
	 */
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
		const b3_f64 *aVec,
		const b3_f64 *bVec,
		      b3_f64 *result)
	{
		*result = *aVec - *bVec;
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
		const b3_f32  factor,
		const b3_f64 *offset,
		      b3_f64 *vector)
	{
		*vector = *vector + factor * *offset;
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
		const b3_f64 *a,
		const b3_f64 *b,
		const b3_f64  factor,
		      b3_f64 *result)
	{
		*result = *a + factor * *b;
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

	static inline void b3Mix(
		      b3_f64 *result,
		const b3_f64 *a,
		const b3_f64 *b,
		const b3_f64  f)
	{
		*result = *a + f * (*b - *a);
	}

	static inline void b3Mix(
		      b3_vector *result,
		const b3_vector *aVec,
		const b3_vector *bVec,
		const b3_f32     f)
	{
		result->x = aVec->x + f * (bVec->x - aVec->x);
		result->y = aVec->y + f * (bVec->y - aVec->y);
		result->z = aVec->z + f * (bVec->z - aVec->z);
	}

	static inline void b3Mix(
		      b3_vector4D *result,
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
 * This template class defines the behaviour of splines.
 */
template<class VECTOR> class B3_PLUGIN b3SplineTemplate
{
public:
	static b3_bspline_error bspline_errno;

	b3_count        m_ControlNum,m_KnotNum;  // used number of knots/controls.
	b3_count        m_Degree,m_SubDiv;       // values of B-Spline
	b3_count        m_ControlMax,m_KnotMax;  // max. values
	b3_index        m_Offset;                // index offset between each control
	b3_bool         m_Closed;                // open/closed curve
	VECTOR         *m_Controls;              // control sequence
	b3_knots        m_Knots;                 // knot sequence

public:
	b3SplineTemplate<VECTOR>()
	{
		m_ControlMax = B3_MAX_CONTROLS;
		m_KnotMax    = B3_MAX_KNOTS;
		m_Degree     = 0;
		m_ControlNum = 0;
		m_KnotNum    = 0;
		m_Offset     = 1;
		m_Controls   = null;
		m_Knots      = null;
		m_SubDiv     = B3_MAX_SUBDIV;
		m_Closed     = false;
	}

	b3SplineTemplate<VECTOR>(const b3SplineTemplate<VECTOR> &src)
	{
		B3_ASSERT(src.m_ControlNum < B3_MAX_CONTROLS);
		B3_ASSERT(src.m_KnotNum < B3_MAX_KNOTS);
		B3_ASSERT(src.m_SubDiv < B3_MAX_SUBDIV);

		m_ControlMax = B3_MAX_CONTROLS;
		m_KnotMax    = B3_MAX_KNOTS;
		m_Degree     = src.m_Degree;
		m_ControlNum = src.m_ControlNum;
		m_KnotNum    = src.m_KnotNum;
		m_Offset     = src.m_Offset;
		m_Controls   = src.m_Controls;
		m_Knots      = src.m_Knots;
		m_SubDiv     = src.m_SubDiv;
		m_Closed     = src.m_Closed;
	}

	inline b3_count b3GetSegmentCount()
	{
		return m_Closed ? m_ControlNum : m_ControlNum - m_Degree;
	}

	inline b3_count b3GetSegmentKnotCount()
	{
		return m_Closed ? m_ControlNum : m_ControlNum - m_Degree + 1;
	}

	inline b3_f64 b3ArcLengthParameter(
		b3_count n,
		b3_f64   scale = 1)
	{
		B3_ASSERT(!m_Closed);
		return b3ArcLengthParameter(n,m_Degree,m_ControlNum,scale);
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

	b3_bool b3InitCurve(
		const b3_count Degree,
		const b3_count ControlNum,
		const b3_bool  Closed,
		const b3_count Offset = 1)
	{
		b3_index  i;

		// Make some checks
		if (ControlNum > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_MUCH_CONTROLS;
			return false;
		}
		if ((ControlNum + Degree + 1) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		if (m_Knots == null)
		{
			bspline_errno = B3_BSPLINE_MISSING_KNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		// Copy values
		m_Degree     = Degree;
		m_ControlNum = ControlNum;
		m_KnotNum    = ControlNum + Degree + 1;
		m_Closed     = Closed;
		m_Offset     = Offset;

		// Create knot vector
		for (i=0;i < m_KnotMax;i++)
		{
			m_Knots[i] = (b3_f32)i;
		}

		// Adjust knot vector
		if (!m_Closed)
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
			m_SubDiv = segments * i--;
		}
		while (m_SubDiv > B3_MAX_SUBDIV);
	}

	void b3ClearControls(b3_index index = 0)
	{
		b3_index i;

		for (i = 0;i < m_ControlNum;i++)
		{
			b3SplineVector::b3Clear(&m_Controls[index]);
			index += m_Offset;
		}
	}

	b3_bool b3ThroughEndControl()
	{
		b3_knot   start,end;
		b3_index  i;

		if (m_Closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		m_KnotNum = m_ControlNum + m_Degree + 1;
		start    = m_Knots[m_Degree];
		end      = m_Knots[m_ControlNum];

		for (i = 0;i < m_Degree;i++)
		{
			m_Knots[i]                   = start;
			m_Knots[m_ControlNum + i + 1] = end;
		}
		return true;
	}

	b3_bool b3ToBezier()
	{
		b3_index i;

		if  (m_Closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if ((m_ControlNum * 2) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		m_KnotNum = m_ControlNum * 2;
		m_Degree   = m_ControlNum - 1;
		for (i = 0;i < m_ControlNum;i++)
		{
			m_Knots[i] = 0;
		}
		for (i = m_ControlNum;i < m_KnotNum;i++)
		{
			m_Knots[i] = 1;
		}

		return true;
	}

	b3_bool b3Degree(const b3_count newDegree)
	{
		b3_knot	  start,end;
		b3_index  i,diff;

		bspline_errno = B3_BSPLINE_OK;
		if (m_Degree == newDegree)
		{
			return true;
		}
		if (m_Degree >= m_ControlNum)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_CONTROLS;
			return false;
		}
		if ((m_ControlNum + newDegree + 1) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}

		if (newDegree < m_Degree)     /* decreasing degree */
		{
			diff = m_Degree - newDegree;
			end = m_Knots[m_ControlNum];
			for (i = 0;i <  diff;i++)
			{
				m_Knots[m_ControlNum+i] = end+i;
			}
			for (i = 0;i <= m_ControlNum;i++)
			{
				m_Knots[i + newDegree] = m_Knots[i + m_Degree];
			}
			if (m_Closed)
			{
				start = m_Knots[newDegree];
				end   = m_Knots[m_ControlNum + newDegree];
				for (i = 0;i < newDegree;i++)
				{
					m_Knots[i] = m_Knots[m_ControlNum + i] - end + start;
				}
				start = -m_Knots[0];
				for (i = newDegree + m_ControlNum;i >= 0;i--)
				{
					m_Knots[i] += start;
				}
			}
		}
		else                             /* increasing degree */
		{
			for (i = m_ControlNum;i >= 0;i--)
			{
				m_Knots[i + newDegree] = m_Knots[i + m_Degree];
			}
			if (m_Closed)
			{
				start = m_Knots[newDegree];
				end   = m_Knots[m_ControlNum + newDegree];
				for (i = 0;i < newDegree;i++)
				{
					m_Knots[i] = m_Knots[m_ControlNum + i] - end + start;
				}
				start = -m_Knots[0];
				for (i = newDegree + m_ControlNum;i>=0;i--)
				{
					m_Knots[i] += start;
				}
			}
		}

		m_KnotNum = m_ControlNum + newDegree + 1;
		m_Degree   = newDegree;
		return true;
	}

	b3_index  b3DeBoor(VECTOR *point,b3_index index = 0)
	{
		b3_knot	 q,qStep;
		b3_index i;

		if (m_Closed)
		{
			q     =  m_Knots[0];
			qStep = (m_Knots[m_ControlNum] - q - B3_BSPLINE_EPSILON) / (b3_knot)m_SubDiv;

			for (i = 0;i <= m_SubDiv;i++)
			{
				b3DeBoorClosed (point++,index,q);
				q += qStep;
			}
		}
		else
		{
			q     =  m_Knots[m_Degree];
			qStep = (m_Knots[m_ControlNum] - q - B3_BSPLINE_EPSILON) / (b3_knot)m_SubDiv;

			for (i = 0;i <= m_SubDiv;i++)
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

		if (m_Closed)
		{
			for (i = 0;i < m_ControlNum;i++)
			{
				b3DeBoorClosed(point++,index,m_Knots[i]);
			}
			return m_ControlNum;
		}
		else
		{
			for (i = m_Degree;i <= m_ControlNum;i++)
			{
				b3DeBoorOpened(point++,index,m_Knots[i]);
			}
			return m_ControlNum - m_Degree + 1;
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
	b3_index  b3Mansfield(b3_f64 *it, b3_f64 qStart)
	{
		b3_index  l,i,j,k;
		b3_knot	  r,denom,q,diff;

		if (m_Closed)
		{
			diff    = m_Knots[m_ControlNum] - m_Knots[0];
			if  ((i = iFind (qStart)) >= m_ControlNum)
			{
				i -= m_ControlNum;
			}

			it[0]  = 1;
			for (l = 1;l <= m_Degree;l++)
			{
				it[l]  = 0;
				q      = qStart;
				k      = i;
				for (j = 0;j < l;j++)
				{
					denom      = m_Knots[k+l] - m_Knots[k];
					r          = (denom != 0 ? (q - m_Knots[k]) / denom : 0);
					it[l-j]   += r * it[l-j-1];
					it[l-j-1] *= (1 - r);
					if (--k < 0) /* check underflow of knots */
					{
						k += m_ControlNum;
						q += diff;
					}
				}
			}
		}
		else
		{
			i      = iFind (qStart);
			it[0]  = 1;
			for (l = 1;l <= m_Degree;l++)
			{
				it[l]  = 0;
				for (j = 0;j < l;j++)
				{
					denom      = m_Knots[i-j+l]      - m_Knots[i-j];
					r          = (denom != 0 ? (qStart - m_Knots[i-j]) / denom : 0);
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
	void b3MansfieldVector(
		VECTOR   *point,
		b3_f64   *it,
		b3_index  i,
		b3_index  index)
	{
		b3_index  l,j;
		VECTOR   *ctrls;

		b3SplineVector::b3Clear(point);
		if (m_Closed)
		{
			j = i;
			for (l = m_Degree;l >= 0;l--)
			{
				b3SplineVector::b3AddScaled(it[l],&m_Controls[j * m_Offset + index],point);
				if (--j < 0)
				{
					j += m_ControlNum;
				}
			}
		}
		else
		{
			ctrls = &m_Controls[i * m_Offset + index];
			for (l = m_Degree;l >= 0;l--)
			{
				b3SplineVector::b3AddScaled(it[l],ctrls,point);
				ctrls -= m_Offset;
			}
		}
		b3SplineVector::b3Homogenize(point);
	}

	b3_index b3DeBoorOpened (VECTOR *point,b3_index index,b3_knot q)
	{
		b3_index  l,i,j;
		b3_knot   r;
		b3_knot    denom;
		b3_knot    it[B3_MAX_DEGREE + 1];

		i = iFind (q);

		it[0]  = 1;
		for (l = 1;l <= m_Degree;l++)
		{
			it[l]  = 0;
			for (j = 0;j < l;j++)
			{
				denom      = m_Knots[i-j+l] - m_Knots[i-j];
				r          = (denom != 0 ? (q - m_Knots[i-j]) / denom : 0);
				it[l-j]   += r * it[l-j-1];
				it[l-j-1] *= (1 - r);
			}
		}

		b3SplineVector::b3Clear(point);
		j = i * m_Offset + index;
		for (l = m_Degree;l >= 0;l--)
		{
			b3SplineVector::b3AddScaled(it[l],&m_Controls[j],point);
			j -= m_Offset;
		}
		b3SplineVector::b3Homogenize(point);
		return i;
	}

	b3_index  b3DeBoorClosed (VECTOR *point,b3_index index,b3_knot qStart)
	{
		b3_index   l,i,j,k;
		b3_knot    r,diff,q;
		b3_knot     denom;
		b3_knot     it[B3_MAX_DEGREE + 1];

		diff    = m_Knots[m_ControlNum] - m_Knots[0];
		if  ((i = iFind (qStart)) >= m_ControlNum)
		{
			i -= m_ControlNum;
		}

		it[0]  = 1;
		for (l = 1;l <= m_Degree;l++)
		{
			it[l]  = 0;
			q      = qStart;
			k      = i;
			for (j = 0;j < l;j++)
			{
				denom      = m_Knots[k+l] - m_Knots[k];
				r          = (denom != 0 ? (q - m_Knots[k]) / denom : 0);
				it[l-j]   += r * it[l-j-1];
				it[l-j-1] *= (1 - r);
				if (--k < 0) /* check underflow of knots */
				{
					k += m_ControlNum;
					q += diff;
				}
			}
		}

		b3SplineVector::b3Clear(point);
		j = i;
		for (l = m_Degree;l >= 0;l--)
		{
			b3SplineVector::b3AddScaled(it[l],&m_Controls[j * m_Offset + index],point);
			if (--j < 0)
			{
				j += m_ControlNum;
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
		knot_ptr   = curveSpline->m_Knots;
		end        = curveSpline->m_ControlNum;
		if (!curveSpline->m_Closed) end++;

		for (i = (curveSpline->m_Closed ? 0 : curveSpline->m_Degree);i < end;i++)
		{
			index = curveSpline->b3Mansfield(it,knot_ptr[i]);
			for (x = 0;x < controlSpline->m_ControlNum;x++)
			{
				curveSpline->b3MansfieldVector (&point[x * ControlNum],it,
					index,x * controlSpline->m_Offset);
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

		m        =  m_ControlNum;
		KnotNum  =  m_KnotNum;

		if ((m + Mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (m_Closed) for (Count = 0;Count < Mult;Count++)
		{
			start = m_Knots[0];
			end   = m_Knots[m];

			b3InsertDeBoorClosed (o,index,q);

			// insert new knot
			for (l = KnotNum;l > i;l--)
			{
				m_Knots[l] = m_Knots[l-1];
			}
			m_Knots[i+1]  = (b3_knot)q;
			m_KnotNum    = ++KnotNum;
			m_ControlNum = ++m;
			for (l = 0;l <= m_Degree;l++)
			{
				m_Knots[l+m] = m_Knots[l] - start + end;
			}

			// insert o[x] into control points
			for (l = m - 1;l > i;l--)
			{
				m_Controls[l * m_Offset] = m_Controls[(l - 1) * m_Offset];
			}

			for (l = i - m_Degree + 1;l <= i;l++)
			{
				m_Controls[((l + m) % m) * m_Offset] = o[(l + (m - 1)) % (m - 1)];
			}
		}
		else for (Count = 0;Count < Mult;Count++)
		{
			i = b3InsertDeBoorOpened (o,index,q);

				/* insert new knot */
			for (l = KnotNum;l > i;l--)
			{
				m_Knots[l+1] = m_Knots[l];
			}
			m_Knots[i+1] = q;
			m_KnotNum = ++KnotNum;

			// insert o[x] into control points
			for (l = m;l > i;l--)
			{
				m_Controls[l * m_Offset] = m_Controls[(l - 1) * m_Offset];
			}

			for (l = i - m_Degree + 1;l <= i;l++)
			{
				m_Controls[l * m_Offset] = o[l];
			}

			m_ControlNum = ++m;
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

		m        =  m_ControlNum;
		KnotNum  =  m_KnotNum;
		if (Mult > (m_Degree + 1))
		{
			Mult = m_Degree + 1;
		}

		if ((m + Mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (m_Closed) for (Count = 0;Count < Mult;Count++)
		{
			start = m_Knots[0];
			end   = m_Knots[m];

			for (y = 0;y < yLines;y++)
			{
				index = y * ControlOffset;
				i = b3InsertDeBoorClosed (o,index,q);
				m++;

				Controls = &m_Controls[index];

					/* insert o[x] into control points */
				for (l = m - 1;l > i;l--)
				{
					Controls[l * m_Offset] = Controls[(l - 1) * m_Offset];
				}
				for (l = i - m_Degree + 1;l <= i;l++)
				{
					Controls[((l + m) % m) * m_Offset] = o[(l + (m-1)) % (m-1)];
				}
				m--;
			}
				/* insert new knot */
			for (l = KnotNum;l > i;l--)
			{
				m_Knots[l] = m_Knots[l-1];
			}
			m_Knots[i + 1] = q;
			m_KnotNum     = ++KnotNum;
			m_ControlNum  = ++m;
			for (l = 0;l <= m_Degree;l++)
			{
				m_Knots[l + m] = m_Knots[l] - start + end;
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
				Controls = &m_Controls[index];
				for (l = m;l > i;l--)
				{
					Controls[l * m_Offset] = Controls[(l - 1) * m_Offset];
				}

				for (l = i - m_Degree + 1;l <= i;l++)
				{
					Controls[l * m_Offset] = o[l];
				}
				m--;
			}

				/* insert new knot */
			for (l = KnotNum;l > i;l--) m_Knots[l+1] = m_Knots[l];
			m_Knots[i+1]  = q;
			m_KnotNum    = ++KnotNum;
			m_ControlNum = ++m;
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

		if (m_Closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if (Mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        = m_ControlNum;
		KnotNum  = m_KnotNum;
		if (Mult > (m_Degree + 1)) Mult = m_Degree + 1;

		if ((m + Mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (append)
		{
			if (q <= m_Knots[KnotNum-1])
			{
				return false;
			}
			for (i = 0;i < Mult;i++)
			{
				m_Knots[KnotNum + i] = q;
			}

			Controls = &m_Controls[index];
			start    = Controls[(m - 1) * m_Offset];
			b3SplineVector::b3Sub(&start,&Controls[(m - 2) * m_Offset],&diff);
			for (i = 0;i < Mult;i++)
			{
				b3SplineVector::b3LinearCombine(&start,i + 1,&diff,&Controls[(m + i) * m_Offset]);
			}
		}
		else
		{
			if (q >= m_Knots[0])
			{
				return false;
			}
			for (i = KnotNum - 1;i >= 0;i--)
			{
				m_Knots[i+Mult] = m_Knots[i];
			}

			Controls = &m_Controls[index];
			start = Controls[0];
			b3SplineVector::b3Sub(&start,&Controls[m_Offset],&diff);

			for (i = m - 1;i >= 0;i--)
			{
				Controls[(i + Mult) * m_Offset] = Controls[i * m_Offset];
			}

			for (i=0;i<Mult;i++)
			{
				m_Knots[i] = q;
				b3SplineVector::b3LinearCombine(&start,&diff,Mult - i,&Controls[i * m_Offset]);
			}
		}
		m_KnotNum    += Mult;
		m_ControlNum += Mult;
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

		if (m_Closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if (Mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        = m_ControlNum;
		KnotNum  = m_KnotNum;
		if (Mult > (m_Degree + 1))
		{
			Mult = m_Degree + 1;
		}

		if ((m       + Mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((KnotNum + Mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (append)
		{
			if (q <= m_Knots[KnotNum-1])
			{
				return false;
			}
			for (i=0;i<Mult;i++)
			{
				m_Knots[KnotNum + i] = q;
			}

			Controls = m_Controls;
			for (y = 0;y < lines;y++)
			{
				start = Controls[(m - 1) * m_Offset];
				b3SplineVector::b3Sub(&start,&Controls[(m - 2) * m_Offset],&diff);
				for (i = 0;i < Mult;i++)
				{
					b3SplineVector::b3LinearCombine(&start,&diff,i+1,&Controls[(m + i) * m_Offset]);
				}
				Controls += skipOffset;
			}
		}
		else
		{
			if (q >= m_Knots[0])
			{
				return false;
			}

			for (i = KnotNum-1;i >= 0;i--)
			{
				m_Knots[i+Mult] = m_Knots[i];
			}
			for (i = 0;i <  Mult;i++)
			{
				m_Knots[i] = q;
			}

			Controls = m_Controls;
			for (y = 0;  y <  lines;y++)
			{
				start = Controls[0];
				b3SplineVector::b3Sub(&start,&Controls[m_Offset],&diff);
				for (i = m - 1;i >= 0;i--)
				{
					Controls[(i + Mult) * m_Offset] = Controls[i * m_Offset];
				}

				for (i = 0;i < Mult;i++)
				{
					b3SplineVector::b3LinearCombine(&start,&diff,Mult - i,&Controls[i * m_Offset]);
				}
				Controls += skipOffset;
			}
		}
		m_KnotNum    += Mult;
		m_ControlNum += Mult;
		return true;
	}

	inline void b3Dump(b3_log_level level = B3LOG_FULL)
	{
		b3PrintF(level, "Spline at %p\n", this);
		b3PrintF(level, "  Degree: %d\n", m_Degree);
		b3PrintF(level, "  Knot count: %d max: %d\n",m_KnotNum, m_KnotMax);
		b3PrintF(level, "  Control count: %d max: %d\n",m_ControlNum, m_ControlMax);
		b3PrintF(level, "  Offset: %d\n", m_Offset);
		b3PrintF(level, "  Subdivision: %d max: %d\n", m_SubDiv, B3_MAX_SUBDIV);
		b3PrintF(level, "  Spline type: %s\n", m_Closed ? "closed" : "open");
		b3PrintF(level, "  Controls: %p\n", m_Controls);
		b3PrintF(level, "  Knots: %p\n", m_Knots);
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
		j = i * m_Offset + index;
		for (l = 0;l <= m_Degree;l++)
		{
			it[l] = m_Controls[j];
			j    -= m_Offset;
		}

		ins += i;		/* loop for control point insertion */
		j    = i;
		for (l = 0;l < m_Degree;l++)
		{
			Denom =   (m_Knots[j+m_Degree] - m_Knots[j]);
			if (Denom != 0) r =     (q - m_Knots[j]) / Denom;
			else            r =      0;
			b3SplineVector::b3Mix(&it[l],&it[l+1],&it[l],r);
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
		b3_index  l,i,j,m;
		b3_knot   r,q,diff;
		b3_f64    Denom;
		VECTOR    it[B3_MAX_DEGREE + 1];

		m       = m_ControlNum;
		diff    = m_Knots[m] - m_Knots[0];

		if ((i = iFind (qStart)) >= m)
		{
			i -= m;
		}

		j = i;
		for (l = 0;l <= m_Degree;l++)
		{
			it[l] = m_Controls[j * m_Offset + index];
			if (--j < 0) j += m;
		}

		// loop for control point insertion
		// the interval [o[i - degree + 1] , o[i]] is valid
		ins   += i;
		j      = i;
		q      = qStart;
		for (l = 0;l < m_Degree;l++)
		{
			Denom =   (m_Knots[j + m_Degree] - m_Knots[j]);
			if (Denom != 0) r =     (q - m_Knots[j]) / Denom;
			else            r =      0;
			b3SplineVector::b3Mix(&it[l],&it[l+1],&it[l],r);
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
		b3_count max = (m_Closed ? m_ControlNum : m_KnotNum) - 1;

		for (i = 0;i < max;i++)
		{						 
			if ((m_Knots[i] <= q) && (q < m_Knots[i+1]))
			{
				return i;
			}
		}
		if (!m_Closed)
		{
			i--;
			while (m_Knots[i-1] == m_Knots[i])
			{
				i--;
			}
		}
		return i;
	}
};

typedef b3SplineTemplate<b3_f64>      b3Curve;
typedef b3SplineTemplate<b3_vector>   b3Spline;
typedef b3SplineTemplate<b3_vector4D> b3Nurbs;

template<class T> b3_bspline_error b3SplineTemplate<T>::bspline_errno = B3_BSPLINE_OK;      

#endif
