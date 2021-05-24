/*
**
**	$Filename:	b3Spline.h $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
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

#pragma once

#ifndef B3_BASE_SPLINE_H
#define B3_BASE_SPLINE_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3SplineVector.h"

#include <functional>

/*************************************************************************
**                                                                      **
**                        Some basic defines                            **
**                                                                      **
*************************************************************************/

#define B3_BSPLINE_SEGMENTS(s)     ((s)->b3GetSegmentCount())
#define B3_BSPLINE_SEGMENTKNOTS(s) ((s)->b3GetSegmentKnotCount())

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
	B3_BSPLINE_TOO_FEW_MAXCONTROLS,    //!< Too few space for more control points.
	B3_BSPLINE_MISSING_KNOTS,          //!< No memory for knot vector defined.
	B3_BSPLINE_CLOSED,                 //!< Algorith works only on open splines.
	B3_BSPLINE_TOO_LOW_MULTIPLICATION, //!< Knot multiplicity is 1 minimum.
	B3_BSPLINE_INVALID_ARGUMENT,       //!< The input parameter are illegal.
	B3_BSPLINE_OUT_OF_RANGE            //!< The q parameter is out of range.
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
	static const     unsigned B3_MAX_CONTROLS    = 32;
	static const     unsigned B3_MAX_SUBDIV      = 64;
	static const     unsigned B3_MAX_DEGREE      = B3_MAX_CONTROLS;
	static const     unsigned B3_MAX_KNOTS       = B3_MAX_CONTROLS + B3_MAX_DEGREE + 1;

	/** The used template parameter type. */
	using             type = VECTOR;

	/** The type of the knot vector elements. */
	typedef b3_f32    b3_knot;

	/** The type of the knot vector itself. */
	typedef b3_knot * b3_knots;

	/** The type of knot vector. */
	typedef           b3_knot  b3_knot_vector[B3_MAX_KNOTS];

	/**
	 * This type definition declares a function that computes a @c q value
	 * from its index i.
	 *
	 * @see b3DeBoorSurfaceTesselate()
	 */
	typedef std::function<b3_f64(
		const b3SplineTemplate<VECTOR> &,
		const unsigned)> b3_eval_function;

	/** The allowed numerical error. */
	static constexpr  b3_knot          epsilon = 1.0 / 16384;

	/** The spline handling error code. */
	static            b3_bspline_error bspline_errno;

	VECTOR     *    m_Controls;       //!< control point sequence.
	b3_knots        m_Knots;          //!< knot sequence.
	unsigned        m_ControlNum;     //!< used number of controls.
	unsigned        m_KnotNum;        //!< used number of knots.
	unsigned        m_Degree;         //!< spline degree.
	unsigned        m_SubDiv;         //!< Sub division for triangulation.
	unsigned        m_ControlMax;     //!< max. available control points.
	unsigned        m_KnotMax;        //!< max. available knots.
	b3_index        m_Offset;         //!< index offset between each control
	bool            m_Closed;         //!< open/closed curve.

public:
	b3SplineTemplate<VECTOR>()
	{
		m_ControlMax = B3_MAX_CONTROLS;
		m_KnotMax    = B3_MAX_KNOTS;
		m_Degree     = 0;
		m_ControlNum = 0;
		m_KnotNum    = 0;
		m_Offset     = 1;
		m_Controls   = nullptr;
		m_Knots      = nullptr;
		m_SubDiv     = B3_MAX_SUBDIV;
		m_Closed     = false;
	}

	b3SplineTemplate<VECTOR>(const b3SplineTemplate<VECTOR> & src)
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

	/**
	 * This method returns the number of segments inside this spline.
	 *
	 * @return The number of segments.
	 */
	[[nodiscard]]
	inline b3_count b3GetSegmentCount() const
	{
		return m_Closed ? m_ControlNum : m_ControlNum - m_Degree;
	}

	/**
	 * This method returns the number of segment knots used inside this spline.
	 *
	 * @return The number of segment knots used.
	 */
	[[nodiscard]]
	inline b3_count b3GetSegmentKnotCount() const
	{
		return m_Closed ? m_ControlNum : m_ControlNum - m_Degree + 1;
	}

	inline b3_f64 b3ArcLengthParameter(
		const b3_count n,
		const b3_f64   scale = 1) const
	{
		B3_ASSERT(!m_Closed);
		return b3ArcLengthParameter(n, m_Degree, m_ControlNum, scale);
	}

	static inline b3_f64 b3ArcLengthParameter(
		const b3_count n,
		const b3_count Degree,
		const b3_count ControlNum,
		const b3_f64   scale = 1)
	{
		b3_f64 result;
		b3_u32 back;

		if (((Degree - 1) * 2) < ControlNum)
		{
			// No arc length parametrization
			result = (b3_f64)n / (ControlNum - 1);
		}
		else
		{
			if (n > Degree)
			{
				if (n > (ControlNum - Degree))
				{
					// last ramp
					back = Degree - (ControlNum - n);
				}
			}
			result = (b3_f64)n / (Degree * (ControlNum - Degree));
		}
		return result * scale;
	}

	/**
	 * This method initializes the fundamental values of the spline. The method
	 * does some range checks return false on error and sets the @c bspline_errno
	 * error variable. It initializes the knot vector which means that the
	 * knot vector pointer m_Knots must be valid prior call to this method. For
	 * an open spline the b3ThroughEndControl() method is called to move the
	 * endpoints of the open spline curve through the boundary control points.
	 * The sub division is also computed to get a proper value.
	 *
	 * @param Degree The degree of the spline. @note The order is the degree + 1.
	 * @param ControlNum The number of used control points.
	 * @param Closed If it should be an open or a closed spline.
	 * @param Offset The offset used for spline surfaces to jump between
	 * lanes of control points.
	 * @return True on success
	 */
	bool b3InitCurve(
		const unsigned Degree,
		const unsigned ControlNum,
		const b3_bool  Closed,
		const b3_count Offset = 1)
	{
		// Make some checks
		if ((Degree <= 0) || (ControlNum <= 0))
		{
			bspline_errno = B3_BSPLINE_INVALID_ARGUMENT;
			return false;
		}

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
		if (m_Knots == nullptr)
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
		for (unsigned i = 0; i < m_KnotMax; i++)
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
		b3_count i, segments;

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
		for (unsigned i = 0; i < m_ControlNum; i++)
		{
			b3SplineVector::b3Clear(&m_Controls[index]);
			index += m_Offset;
		}
	}

	b3_bool b3ThroughEndControl()
	{
		b3_knot   start, end;

		if (m_Closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		m_KnotNum = m_ControlNum + m_Degree + 1;
		start     = m_Knots[m_Degree];
		end       = m_Knots[m_ControlNum];

		for (unsigned i = 0; i < m_Degree; i++)
		{
			m_Knots[i]                    = start;
			m_Knots[m_ControlNum + i + 1] = end;
		}
		return true;
	}

	bool b3ToBezier()
	{
		unsigned i;

		if (m_Closed)
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
		m_Degree  = m_ControlNum - 1;
		for (i = 0; i < m_ControlNum; i++)
		{
			m_Knots[i] = 0;
		}
		for (i = m_ControlNum; i < m_KnotNum; i++)
		{
			m_Knots[i] = 1;
		}

		return true;
	}

	b3_bool b3Degree(const unsigned newDegree)
	{
		b3_knot	  start, end;
		unsigned  i;

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
			unsigned diff = m_Degree - newDegree;

			end = m_Knots[m_ControlNum];
			for (i = 0; i <  diff; i++)
			{
				m_Knots[m_ControlNum + i] = end + i;
			}
			for (i = 0; i <= m_ControlNum; i++)
			{
				m_Knots[i + newDegree] = m_Knots[i + m_Degree];
			}
			if (m_Closed)
			{
				start = m_Knots[newDegree];
				end   = m_Knots[m_ControlNum + newDegree];
				for (i = 0; i < newDegree; i++)
				{
					m_Knots[i] = m_Knots[m_ControlNum + i] - end + start;
				}
				start = -m_Knots[0];
				for (i = newDegree + m_ControlNum; i >= 0; i--)
				{
					m_Knots[i] += start;
				}
			}
		}
		else                             /* increasing degree */
		{
			for (i = m_ControlNum; i >= 0; i--)
			{
				m_Knots[i + newDegree] = m_Knots[i + m_Degree];
			}
			if (m_Closed)
			{
				start = m_Knots[newDegree];
				end   = m_Knots[m_ControlNum + newDegree];
				for (i = 0; i < newDegree; i++)
				{
					m_Knots[i] = m_Knots[m_ControlNum + i] - end + start;
				}
				start = -m_Knots[0];
				for (i = newDegree + m_ControlNum; i >= 0; i--)
				{
					m_Knots[i] += start;
				}
			}
		}

		m_KnotNum = m_ControlNum + newDegree + 1;
		m_Degree  = newDegree;
		return true;
	}

	/**
	 * This method returns the index of the first knot element which
	 * may differ between closed and opened splines.
	 *
	 * @return The index of the first knot vector value.
	 * @see b3FirstKnot()
	 */
	[[nodiscard]]
	inline unsigned b3FirstKnotIndex() const
	{
		return m_Closed ? 0 : m_Degree;
	}

	/**
	 * This method returns the reference to the first knot element which
	 * may differ between closed and opened splines.
	 *
	 * @return The reference to the first knot vector value.
	 * @see b3FirstKnotIndex()
	 */
	[[nodiscard]]
	inline const b3_knot & b3FirstKnot() const
	{
		return m_Knots[b3FirstKnotIndex()];
	}

	/**
	 * This method returns the reference to the last knot element.
	 *
	 * @return The reference to the lst knot vector value.
	 */
	[[nodiscard]]
	inline const b3_knot & b3LastKnot() const
	{
		return m_Knots[m_ControlNum];
	}

	/**
	 * This method returns the value range of the knot vector.
	 *
	 * @return The value range of the knot vector.
	 */
	[[nodiscard]]
	inline b3_knot b3KnotRange() const
	{
		return b3LastKnot() - b3FirstKnot();
	}

	/**
	* This method subdivides a curve into points reflecting the open/closed
	* definition and the @c m_SubDiv member variable. This calls the
	* b3DeBoorOpened() or b3DeBoorClosed() methods to compute a singe value
	* within the valid knot range.
	*
	* @note Important: In case you are using NURBS the control points have to
	* be in the form Ci = [wx, wy, wz, w]. After using this method the result
	* point is homogenized using the b3SplineVector::b3Homogenize() method and
	* has the form R = [x, y, z, w] for further usage.
	*
	* @note Most control point examples in the internet concerning rational
	* splines are in the form Ci = [x, y, z, w]. If you plan to use those
	* examples you have to transform the control points into the form
	* Ci = [wx, wy, wz, w] using the b3SplineVector::b3WeightInit() or
	* b3SplineVector::b3WeightSelf() methods.
	*
	* @param point The point array where the computed points are stored.
	* @param index The start index to use. Using two splines decribing surfaces
	* this is the start index and the @c m_Offset member variable contains the
	* vertical control point offset. In one dimensional curves this should
	* be @c 0.
	*/
	unsigned  b3DeBoor(VECTOR * point, const b3_index index = 0) const
	{
		const b3_f64 qStep = b3KnotRange() / m_SubDiv;
		b3_f64	     q     = b3FirstKnot();
		unsigned     i;

		if (m_Closed)
		{
			for (i = 0; i <= m_SubDiv; i++)
			{
				b3DeBoorClosed(*point++, q, index);
				q += qStep;
			}
		}
		else
		{
			for (i = 0; i <= m_SubDiv; i++)
			{
				b3DeBoorOpened(*point++, q, index);
				q += qStep;
			}
		}

		return i;
	}

	/**
	 * This method computes one single point on an open spline. The value
	 * @c q has between the valid range of the knot vector. You can determine
	 * the valid range using the b3FirstKnot(), b3LastKnot() and b3KnotRange()
	 * methods.
	 *
	 * The @c index parameter is used for spline surfaces and is the start
	 * index access the control point array. Since the control point array is
	 * used for both horizontal and vertical splines it is not two dimensional
	 * but uses the @c m_Offset member to access the correct control points.
	 *
	 * @note Important: In case you are using NURBS the control points have to
	 * be in the form Ci = [wx, wy, wz, w]. After using this method the result
	 * point is homogenized using the b3SplineVector::b3Homogenize() method and
	 * has the form R = [x, y, z, w] for further usage.
	 *
	 * @note Most control point examples in the internet concerning rational
	 * splines are in the form Ci = [x, y, z, w]. If you plan to use those
	 * examples you have to transform the control points into the form
	 * Ci = [wx, wy, wz, w] using the b3SplineVector::b3WeightInit() or
	 * b3SplineVector::b3WeightSelf() methods.
	 *
	 * @param point The point array where the computed points are stored.
	 * @param q The value with the valid knot vector range.
	 * @param index The start index to use. Using two splines decribing surfaces
	 * this is the start index and the @c m_Offset member variable contains the
	 * vertical control point offset. In one dimensional curves this should
	 * be @c 0.
	 * @return The found knot vector index using the iFind() method.
	 */
	unsigned b3DeBoorOpened(VECTOR & point, b3_knot q, b3_index index = 0) const
	{
		const unsigned  i = iFind(q);
		b3_f64          basis[m_Degree + 1];

		B3_ASSERT(!m_Closed);

		basis[0]  = 1;
		for (unsigned l = 1; l <= m_Degree; l++)
		{
			basis[l]  = 0;
			for (unsigned j = 0; j < l; j++)
			{
				const b3_f64 denom = m_Knots[i - j + l] - m_Knots[i - j];
				const b3_f64 r     = (denom != 0 ? (q - m_Knots[i - j]) / denom : 0);

				basis[l - j]     += r * basis[l - j - 1];
				basis[l - j - 1] *= (1 - r);
			}
		}

		b3SplineVector::b3Clear(point);
		b3_index j = i * m_Offset + index;
		for (b3_index l = m_Degree; l >= 0; l--)
		{
			b3SplineVector::b3AddScaled(basis[l], m_Controls[j], point);
			j -= m_Offset;
		}
		b3SplineVector::b3Homogenize(point);
		return i;
	}

	/**
	 * This method computes one single point on an closed spline. The value
	 * @c q has between the valid range of the knot vector. You can determine
	 * the valid range using the b3FirstKnot(), b3LastKnot() and b3KnotRange()
	 * methods.
	 *
	 * The @c index parameter is used for spline surfaces and is the start
	 * index access the control point array. Since the control point array is
	 * used for both horizontal and vertical splines it is not two dimensional
	 * but uses the @c m_Offset member to access the correct control points.
	 *
	 * @note Important: In case you are using NURBS the control points have to
	 * be in the form Ci = [wx, wy, wz, w]. After using this method the result
	 * vector is homogenized using the b3SplineVector::b3Homogenize() method and
	 * has the form R = [ x, y, z, w] for further usage.
	 *
	 * @note Most control point examples in the internet concerning rational
	 * splines are in the form Ci = [x, y, z, w]. If you plan to use those
	 * examples you have to transform the control points into the form
	 * Ci = [wx, wy, wz, w] using the b3SplineVector::b3WeightInit() or
	 * b3SplineVector::b3WeightSelf() methods.
	 *
	 * @param point The point array where the computed points are stored.
	 * @param q The value with the valid knot vector range.
	 * @param index The start index to use. Using two splines decribing surfaces
	 * this is the start index and the @c m_Offset member variable contains the
	 * vertical control point offset. In one dimensional curves this should
	 * be @c 0.
	 * @return The found knot vector index using the iFind() method.
	 */
	unsigned  b3DeBoorClosed(VECTOR & point, b3_knot qStart, b3_index index = 0) const
	{
		const b3_knot  range = m_Knots[m_ControlNum] - m_Knots[0];
		const unsigned i     = iFind(qStart) % m_ControlNum;
		b3_f64         q;
		b3_f64         basis[m_Degree + 1];

		B3_ASSERT(m_Closed);

		basis[0]  = 1;
		for (unsigned l = 1; l <= m_Degree; l++)
		{
			b3_index k = i;

			basis[l]  = 0;
			q      = qStart;
			for (unsigned j = 0; j < l; j++)
			{
				const b3_f64 denom = m_Knots[k + l] - m_Knots[k];
				const b3_f64 r     = (denom != 0 ? (q - m_Knots[k]) / denom : 0);

				basis[l - j]     += r * basis[l - j - 1];
				basis[l - j - 1] *= (1 - r);
				if (--k < 0) /* check underflow of knots */
				{
					k += m_ControlNum;
					q += range;
				}
			}
		}

		b3_index j = i;

		b3SplineVector::b3Clear(point);
		for (b3_index l = m_Degree; l >= 0; l--)
		{
			b3SplineVector::b3AddScaled(basis[l], m_Controls[j * m_Offset + index], point);
			if (--j < 0)
			{
				j += m_ControlNum;
			}
		}
		b3SplineVector::b3Homogenize(point);
		return i;
	}

	unsigned  b3DeBoorCurveControl(VECTOR * point, const unsigned index) const
	{
		unsigned  i;

		if (m_Closed)
		{
			for (i = 0; i < m_ControlNum; i++)
			{
				b3DeBoorClosed(point++, m_Knots[i], index);
			}
			return m_ControlNum;
		}
		else
		{
			for (i = m_Degree; i <= m_ControlNum; i++)
			{
				b3DeBoorOpened(point++, m_Knots[i], index);
			}
			return m_ControlNum - m_Degree + 1;
		}
	}

	static unsigned b3DeBoorSurfaceControl(
		const b3SplineTemplate<VECTOR> & control_spline,
		const b3SplineTemplate<VECTOR> & curve_spline,
		b3SplineTemplate<VECTOR>    &    loop_spline,
		VECTOR             *             point)
	{
		const unsigned segment_count = curve_spline.b3GetSegmentKnotCount();
		unsigned       end           = curve_spline.m_ControlNum;
		b3_f64         basis[B3_MAX_DEGREE + 1];

		// Init auxiliary loop spline
		loop_spline            = control_spline;
		loop_spline.m_Offset   = segment_count;
		loop_spline.m_Controls = point;

		if (!curve_spline.m_Closed)
		{
			end++;
		}

		for (unsigned i = curve_spline.b3FirstKnotIndex(); i < end; i++)
		{
			const unsigned index = curve_spline.b3Mansfield(basis, curve_spline.m_Knots[i]);

			for (unsigned x = 0; x < control_spline.m_ControlNum; x++)
			{
				curve_spline.b3MansfieldVector(point[x * segment_count], basis,
					index, x * control_spline.m_Offset);
			}
			point++;
		}

		return segment_count;
	}

	/**
	 * This method tesselates a surface given by the spline descriptions
	 * @c horizontal and @c vertical. The result is put into the array @c result.
	 * You can control the computation behavior by adding an evaluation function
	 * for control point subdivison.
	 *
	 * The default behaviour is that the horizontal subdivision is using the
	 * horizontal.m_SubDiv value to equal spaced @c q. So you can simply call
	 * b3DeBoorSurfaceTesselate() with three parameters to get a tesselation e.g.
	 * for triangulating the surface.
	 *
	 * If you want to subdivide horizontally that every @c q corresponds to its
	 * knot value you have to initialize the subdivision by the segment count.
	 *
	 * Example:
	 * @code
	b3Nurbs::type result[(b3Nurbs::B3_MAX_SUBDIV + 1) * (b3Nurbs::B3_MAX_SUBDIV + 1)];

	horizontal.m_SubDiv = horizontal.b3GetSegmentKnotCount();

	const unsigned point_count = b3Nurbs::b3DeBoorSurfaceTesselate(
			horizontal, vertical, result, b3Nurbs::b3FuncKnot);
	@endcode
	 *
	 * @param horizontal The horizontal spline description.
	 * @param vertical The vertical spline description.
	 * @param result The resulting homogenized points.
	 * @param func An evaluation function to compute @c q for control point mesh.
	 */
	static unsigned b3DeBoorSurfaceTesselate(
		b3SplineTemplate<VECTOR> & horizontal,
		b3SplineTemplate<VECTOR> & vertical,
		type           *           result,
		b3_eval_function           func = b3FuncSubdivision)
	{
		b3SplineTemplate<VECTOR>  aux_spline;
		type                      aux_control_points[(B3_MAX_CONTROLS + 1) * (B3_MAX_SUBDIV + 1)];
		type           *          aux_ptr       = aux_control_points;

		// Building a series of vertical splines.
		unsigned  segment_count = horizontal.m_SubDiv;

		if (!horizontal.m_Closed)
		{
			segment_count++;
		}

		// @see b3Spline::b3DeBoorSurfaceControl()
		for (unsigned i = 0; i < segment_count; i++)
		{
			b3_f64 q = func(horizontal, i);
			b3_f64 basis[B3_MAX_DEGREE + 1];

			const unsigned index = horizontal.b3Mansfield(basis, q);

			for (unsigned x = 0; x < vertical.m_ControlNum; x++)
			{
				horizontal.b3MansfieldVector(aux_ptr[x * segment_count], basis,
					index, x * vertical.m_Offset);
			}
			aux_ptr++;
		}

		// Create aux BSpline
		aux_spline            = vertical;
		aux_spline.m_Offset   = segment_count;
		aux_spline.m_Controls = aux_control_points;

		// For every sub division of vertical spline compute horizontal spline.
		unsigned point_count = 0;
		for (b3_index x = 0; x < aux_spline.m_Offset; x++)
		{
			const unsigned count = aux_spline.b3DeBoor(result, x);

			result      += count;
			point_count += count;
		}

		return point_count;
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
	[[nodiscard]]
	unsigned b3Mansfield(b3_f64 * it, const b3_f64 qStart) const
	{
		unsigned  i, j, l;
		b3_f64	  r, denom, q;

		if (m_Closed)
		{
			const b3_f64 diff = m_Knots[m_ControlNum] - m_Knots[0];

			i    = iFind(qStart);
			if (i >= m_ControlNum)
			{
				i -= m_ControlNum;
			}

			it[0]  = 1;
			for (l = 1; l <= m_Degree; l++)
			{
				b3_index k = i;

				it[l]  = 0;
				q      = qStart;
				for (j = 0; j < l; j++)
				{
					denom          = m_Knots[k + l] - m_Knots[k];
					r              = (denom != 0 ? (q - m_Knots[k]) / denom : 0);
					it[l - j]     += r * it[l - j - 1];
					it[l - j - 1] *= (1 - r);

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
			i      = iFind(qStart);
			it[0]  = 1;
			for (l = 1; l <= m_Degree; l++)
			{
				it[l]  = 0;
				for (j = 0; j < l; j++)
				{
					denom          = m_Knots[i - j + l]    - m_Knots[i - j];
					r              = (denom != 0 ? (qStart - m_Knots[i - j]) / denom : 0);
					it[l - j]     += r * it[l - j - 1];
					it[l - j - 1] *= (1 - r);
				}
			}
		}
		return i;
	}

	/**
	 * This routine computes the real curve point according to the basis
	 * coefficients computed by Mansfield(). The value @c i is the knot
	 * index returned by b3Mansfield(). The start index is like the b3DeBoor-
	 * routines.
	 *
	 * Example:
	 * @code
	b3Nurbs     nurbs; // Has to be proper initiazed!
	...
	b3_f64      it[b3Nurbs::B3_MAX_CONTROLS];
	b3_vector4D point;

	const unsigned i = nurbs.b3Mansfield(it, 1.5);
	nurbs.b3MansfieldVector(point, it, i);
	b3SplineVector::b3Homogenize(point);
	 @endcode
	 *
	 * @note In the case you are using NURBS You have to homogenize the
	 * resulting point by hand using the b3SplineVector::b3Homogenize() method.
	 *
	 * @param point  Where to store the curve point
	 * @param it     Basis coefficients computed by b3Mansfield()
	 * @param i      Knot index returned by b3Mansfield()
	 * @param index  Start index of control points
	 */
	void b3MansfieldVector(
		VECTOR     &    point,
		const b3_f64  * it,
		const unsigned  i,
		const b3_index  index = 0) const
	{
		b3_index  l, j;
		VECTOR  * ctrls;

		b3SplineVector::b3Clear(point);
		if (m_Closed)
		{
			j = i;
			for (l = m_Degree; l >= 0; l--)
			{
				b3SplineVector::b3AddScaled(it[l], m_Controls[j * m_Offset + index], point);
				if (--j < 0)
				{
					j += m_ControlNum;
				}
			}
		}
		else
		{
			ctrls = &m_Controls[i * m_Offset + index];
			for (l = m_Degree; l >= 0; l--)
			{
				b3SplineVector::b3AddScaled(it[l], *ctrls, point);
				ctrls -= m_Offset;
			}
		}
	}

	bool b3InsertCurveControl(
		b3_knot    q,
		unsigned   Mult  = 1,
		b3_index   index = 0)
	{
		unsigned m;
		unsigned l, i;
		unsigned KnotNum;
		b3_knot  start, end;
		VECTOR   o[B3_MAX_CONTROLS + 1]; /* buffer for knot insertion */

		if (Mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        =  m_ControlNum;
		KnotNum  =  m_KnotNum;

		if ((KnotNum + Mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		if ((m + Mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (m_Closed)
		{
			for (unsigned Count = 0; Count < Mult; Count++)
			{
				start = m_Knots[0];
				end   = m_Knots[m];

				i = b3InsertDeBoorClosed(o, index, q);

				// insert new knot
				for (l = KnotNum; l > i; l--)
				{
					m_Knots[l] = m_Knots[l - 1];
				}
				m_Knots[i + 1] = (b3_knot)q;
				m_KnotNum      = ++KnotNum;
				m_ControlNum   = ++m;
				for (l = 0; l <= m_Degree; l++)
				{
					m_Knots[l + m] = m_Knots[l] - start + end;
				}

				// insert o[x] into control points
				for (l = m - 1; l > i; l--)
				{
					m_Controls[l * m_Offset] = m_Controls[(l - 1) * m_Offset];
				}

				for (l = i - m_Degree + 1; l <= i; l++)
				{
					m_Controls[((l + m) % m) * m_Offset] = o[(l + (m - 1)) % (m - 1)];
				}
			}
		}
		else
		{
			for (unsigned Count = 0; Count < Mult; Count++)
			{
				i = b3InsertDeBoorOpened(o, index, q);

				/* insert new knot */
				for (l = KnotNum; l > i; l--)
				{
					m_Knots[l + 1] = m_Knots[l];
				}
				m_Knots[i + 1] = q;
				m_KnotNum = ++KnotNum;

				// insert o[x] into control points
				for (l = m; l > i; l--)
				{
					m_Controls[l * m_Offset] = m_Controls[(l - 1) * m_Offset];
				}

				for (l = i - m_Degree + 1; l <= i; l++)
				{
					m_Controls[l * m_Offset] = o[l];
				}

				m_ControlNum = ++m;
			}
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
	bool b3InsertSurfaceControl(
		b3_knot    q,
		unsigned   mult,
		b3_index   control_offset,
		b3_count   yLines)
	{
		unsigned  i = 0;
		b3_index  m, index, y;
		b3_knot	  start, end;
		VECTOR  * controls;
		VECTOR    o[B3_MAX_CONTROLS + 1]; /* buffer for knot insertion */

		bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
		if (mult < 1)
		{
			return false;
		}

		m        =  m_ControlNum;
		if (mult > (m_Degree + 1))
		{
			mult = m_Degree + 1;
		}

		if ((m + mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((m_KnotNum + mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (m_Closed)
		{
			for (unsigned count = 0; count < mult; count++)
			{
				start = m_Knots[0];
				end   = m_Knots[m];

				for (y = 0; y < yLines; y++)
				{
					index = y * control_offset;
					i = b3InsertDeBoorClosed(o, index, q);
					m++;

					controls = &m_Controls[index];

					/* insert o[x] into control points */
					for (unsigned l = m - 1; l > i; l--)
					{
						controls[l * m_Offset] = controls[(l - 1) * m_Offset];
					}
					for (unsigned l = i - m_Degree + 1; l <= i; l++)
					{
						controls[((l + m) % m) * m_Offset] = o[(l + (m - 1)) % (m - 1)];
					}
					m--;
				}

				/* insert new knot */
				for (unsigned l = m_KnotNum; l > i; l--)
				{
					m_Knots[l] = m_Knots[l - 1];
				}
				m_Knots[i + 1] = q;
				m_KnotNum++;
				m_ControlNum  = ++m;
				for (unsigned l = 0; l <= m_Degree; l++)
				{
					m_Knots[l + m] = m_Knots[l] - start + end;
				}
			}
		}
		else
		{
			for (unsigned count = 0; count < mult; count++)
			{
				for (y = 0; y < yLines; y++)
				{
					index = y * control_offset;
					i = b3InsertDeBoorOpened(o, index, q);
					m++;

					// insert o[x] into control points
					controls = &m_Controls[index];
					for (unsigned l = m; l > i; l--)
					{
						controls[l * m_Offset] = controls[(l - 1) * m_Offset];
					}

					for (unsigned l = i - m_Degree + 1; l <= i; l++)
					{
						controls[l * m_Offset] = o[l];
					}
					m--;
				}

				/* insert new knot */
				for (unsigned l = m_KnotNum; l > i; l--)
				{
					m_Knots[l + 1] = m_Knots[l];
				}
				m_Knots[i + 1]  = q;
				m_KnotNum++;
				m_ControlNum = ++m;
			}
		}
		return true;
	}

	bool b3AppendCurveControl(
		bool      append,
		b3_knot    q,
		unsigned   mult,
		b3_index   index)
	{
		VECTOR  * controls;
		VECTOR    start, diff;
		unsigned  m;

		if (m_Closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if (mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        = m_ControlNum;
		if (mult > (m_Degree + 1))
		{
			mult = m_Degree + 1;
		}

		if ((m + mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((m_KnotNum + mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (append)
		{
			if (q <= m_Knots[m_KnotNum - 1])
			{
				bspline_errno = B3_BSPLINE_OUT_OF_RANGE;
				return false;
			}
			for (unsigned i = 0; i < mult; i++)
			{
				m_Knots[m_KnotNum + i] = q;
			}

			controls = &m_Controls[index];
			start    = controls[(m - 1) * m_Offset];
			diff     = b3SplineVector::b3Sub(start, controls[(m - 2) * m_Offset]);
			for (unsigned i = 0; i < mult; i++)
			{
				controls[(m + i) * m_Offset] =
					b3SplineVector::b3LinearCombine(start, diff, i + 1);
			}
		}
		else
		{
			if (q >= m_Knots[0])
			{
				bspline_errno = B3_BSPLINE_OUT_OF_RANGE;
				return false;
			}
			for (b3_index i = m_KnotNum - 1; i >= 0; i--)
			{
				m_Knots[i + mult] = m_Knots[i];
			}

			controls = &m_Controls[index];
			start = controls[0];
			diff  = b3SplineVector::b3Sub(start, controls[m_Offset]);

			for (b3_index i = m - 1; i >= 0; i--)
			{
				controls[(i + mult) * m_Offset] = controls[i * m_Offset];
			}

			for (unsigned i = 0; i < mult; i++)
			{
				m_Knots[i] = q;
				controls[i * m_Offset] =
					b3SplineVector::b3LinearCombine(start, diff, mult - i);
			}
		}
		m_KnotNum    += mult;
		m_ControlNum += mult;
		return true;
	}

	bool b3AppendSurfaceControl(
		bool       append,
		b3_knot    q,
		unsigned   mult,
		unsigned   skip_offset,
		unsigned   yLines)
	{
		VECTOR  * controls;
		VECTOR    start, diff;
		unsigned  m, y;

		if (m_Closed)
		{
			bspline_errno = B3_BSPLINE_CLOSED;
			return false;
		}
		if (mult < 1)
		{
			bspline_errno = B3_BSPLINE_TOO_LOW_MULTIPLICATION;
			return false;
		}

		m        = m_ControlNum;
		if (mult > (m_Degree + 1))
		{
			mult = m_Degree + 1;
		}

		if ((m       + mult) > m_ControlMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXCONTROLS;
			return false;
		}
		if ((m_KnotNum + mult) > m_KnotMax)
		{
			bspline_errno = B3_BSPLINE_TOO_FEW_MAXKNOTS;
			return false;
		}
		bspline_errno = B3_BSPLINE_OK;

		if (append)
		{
			if (q <= m_Knots[m_KnotNum - 1])
			{
				bspline_errno = B3_BSPLINE_OUT_OF_RANGE;
				return false;
			}
			for (unsigned i = 0; i < mult; i++)
			{
				m_Knots[m_KnotNum + i] = q;
			}

			controls = m_Controls;
			for (y = 0; y < yLines; y++)
			{
				start = controls[(m - 1) * m_Offset];
				diff = b3SplineVector::b3Sub(start, controls[(m - 2) * m_Offset]);
				for (unsigned i = 0; i < mult; i++)
				{
					controls[(m + i) * m_Offset] =
						b3SplineVector::b3LinearCombine(start, diff, i + 1);
				}
				controls += skip_offset;
			}
		}
		else
		{
			if (q >= m_Knots[0])
			{
				bspline_errno = B3_BSPLINE_OUT_OF_RANGE;
				return false;
			}

			for (b3_index i = m_KnotNum - 1; i >= 0; i--)
			{
				m_Knots[i + mult] = m_Knots[i];
			}
			for (unsigned i = 0; i <  mult; i++)
			{
				m_Knots[i] = q;
			}

			controls = m_Controls;
			for (y = 0;  y <  yLines; y++)
			{
				start = controls[0];
				diff = b3SplineVector::b3Sub(start, controls[m_Offset]);
				for (b3_index i = m - 1; i >= 0; i--)
				{
					controls[(i + mult) * m_Offset] = controls[i * m_Offset];
				}

				for (unsigned i = 0; i < mult; i++)
				{
					controls[i * m_Offset] =
						b3SplineVector::b3LinearCombine(start, diff, mult - i);
				}
				controls += skip_offset;
			}
		}
		m_KnotNum    += mult;
		m_ControlNum += mult;
		return true;
	}

	inline void b3Dump(b3_log_level level = B3LOG_FULL)
	{
		b3PrintF(level, "Spline at %p\n", this);
		b3PrintF(level, "  Degree: %d\n", m_Degree);
		b3PrintF(level, "  Knot count: %d max: %d\n", m_KnotNum, m_KnotMax);
		b3PrintF(level, "  Control count: %d max: %d\n", m_ControlNum, m_ControlMax);
		b3PrintF(level, "  Offset: %d\n", m_Offset);
		b3PrintF(level, "  Subdivision: %u max: %u\n", m_SubDiv, B3_MAX_SUBDIV);
		b3PrintF(level, "  Spline type: %s\n", m_Closed ? "closed" : "open");
		b3PrintF(level, "  Controls: %p\n", m_Controls);
		b3PrintF(level, "  Knots: %p\n", m_Knots);
	}

	/**
	 * This method finds an index i of a knot vector K and value q where is
	 * K_i <= q < K_i+1.
	 *
	 * @param q The value to find in the knot vector.
	 */
	[[nodiscard]]
	inline unsigned iFind(const b3_knot q) const
	{
		const unsigned max = (m_Closed ? m_ControlNum : m_KnotNum) - 1;
		unsigned       i;

		for (i = 0; i < max; i++)
		{
			if ((m_Knots[i] <= q) && (q < m_Knots[i + 1]))
			{
				return i;
			}
		}
		if (!m_Closed)
		{
			i--;
			while (m_Knots[i - 1] == m_Knots[i])
			{
				i--;
			}
		}
		return i;
	}

	/**
	 * This method computes a @c q value from its subdivision index @c i and
	 * is used as callback by b3DeBoorSurfaceTesselate(). The range of @c i
	 * must inside [0, m_SubDiv]. The @c q value is equal spaced between the
	 * range given by b3FirstKnot() and b3LastKnot().
	 *
	 * @param spline The spline needed for computation.
	 * @param i The subdivision index.
	 * @return The computed @c q.
	 */
	static inline b3_f64 b3FuncSubdivision(
		const b3SplineTemplate<VECTOR> & spline,
		const unsigned                   i)
	{
		return spline.b3FirstKnot() + spline.b3KnotRange() * i / spline.m_SubDiv;
	}

	/**
	 * @brief b3FuncKnot
	 * @param spline
	 * @param i
	 * @return
	 */
	static b3_f64 b3FuncKnot(
		const b3SplineTemplate<VECTOR> & spline,
		const unsigned                   i)
	{
		return spline.m_Knots[spline.b3FirstKnotIndex() + i];
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
	[[nodiscard]]
	inline unsigned b3InsertDeBoorOpened(
		VECTOR  * ins,
		b3_index  index,
		b3_knot   q)
	{
		unsigned  l, i, j;
		b3_f64    r;
		b3_f64    Denom;
		VECTOR    it[B3_MAX_DEGREE + 1];

		i = iFind(q);
		j = i * m_Offset + index;
		for (l = 0; l <= m_Degree; l++)
		{
			it[l] = m_Controls[j];
			j    -= m_Offset;
		}

		ins += i;		/* loop for control point insertion */
		j    = i;
		for (l = 0; l < m_Degree; l++)
		{
			Denom = (m_Knots[j + m_Degree] - m_Knots[j]);
			if (Denom != 0)
			{
				r = (q - m_Knots[j]) / Denom;
			}
			else
			{
				r =      0;
			}
			it[l] = b3SplineVector::b3Mix(it[l + 1], it[l], r);
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
	[[nodiscard]]
	inline unsigned b3InsertDeBoorClosed(
		VECTOR  * ins,
		b3_index  index,
		b3_knot   qStart)
	{
		unsigned  l, i, m;
		b3_index  j;
		b3_knot   r, q, diff;
		b3_f64    Denom;
		VECTOR    it[B3_MAX_DEGREE + 1];

		m       = m_ControlNum;
		diff    = m_Knots[m] - m_Knots[0];

		if ((i = iFind(qStart)) >= m)
		{
			i -= m;
		}

		j = i;
		for (l = 0; l <= m_Degree; l++)
		{
			it[l] = m_Controls[j * m_Offset + index];
			if (--j < 0)
			{
				j += m;
			}
		}

		// loop for control point insertion
		// the interval [o[i - degree + 1] , o[i]] is valid
		ins   += i;
		j      = i;
		q      = qStart;
		for (l = 0; l < m_Degree; l++)
		{
			Denom = (m_Knots[j + m_Degree] - m_Knots[j]);
			if (Denom != 0)
			{
				r = (q - m_Knots[j]) / Denom;
			}
			else
			{
				r =      0;
			}
			it[l] = b3SplineVector::b3Mix(it[l + 1], it[l], r);
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
};

typedef b3SplineTemplate<b3_f64>      b3Curve;
typedef b3SplineTemplate<b3_vector>   b3Spline;
typedef b3SplineTemplate<b3_vector4D> b3Nurbs;

template<class T> b3_bspline_error b3SplineTemplate<T>::bspline_errno = B3_BSPLINE_OK;

#endif
