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

typedef struct b3Spline
{
	b3_count     control_num,knot_num;
	b3_count     degree,subdiv;         // values of B-Spline
	b3_count     control_max,knot_max;  // max. values
	b3_index     offset;                // offset of controls
	b3_bool      closed;                // open/close curve
	b3_vector3D *controls;              // control sequence
	b3_f32      *knots;                 // knot sequence
} b3_spline;

typedef struct b3Nurbs
{
	b3_count     control_num,knot_num;
	b3_count     degree,subdiv;         // values of B-Spline
	b3_count     control_max,knot_max;  // max. values
	b3_index     offset;                // offset of controls
	b3_bool      closed;                // open/close curve
	b3_vector4D *controls;              // control sequence
	b3_f32      *knots;                 // knot sequence
} b3_nurbs;

/*************************************************************************
**                                                                      **
**                        error codes                                   **
**                                                                      **
*************************************************************************/

typedef enum
{
	BSPLINE_OK = 0,
	BSPLINE_TOO_MUCH_CONTROLS,
	BSPLINE_TOO_FEW_CONTROLS,
	BSPLINE_TOO_FEW_MAXKNOTS,
	BSPLINE_TOO_FEW_MAXCONTROLS,
	BSPLINE_MISSING_KNOTS,
	BSPLINE_CLOSED,
	BSPLINE_TOO_LOW_MULTIPLICATION
} b3_bspline_error;

extern b3_bspline_error b3BSplineErrno();

/*************************************************************************
**                                                                      **
**                        calculate curves                              **
**                                                                      **
*************************************************************************/

b3_index  b3DeBoorOpened        (b3_spline *Spline,b3_vector *Result,b3_index index,b3_f64 q);
b3_index  b3DeBoorClosed        (b3_spline *Spline,b3_vector *Result,b3_index index,b3_f64 q);
b3_index  b3DeBoor              (b3_spline *Spline,b3_vector *Result,b3_index index);
b3_index  b3DeBoorNurbs         (b3_nurbs  *Nurbs, b3_vector *Result,b3_index index);
b3_index  b3DeBoorControl       (b3_spline *Spline,b3_vector *,b3_index index);
b3_count  b3DeBoorSurfaceControl(b3_spline *Spline,b3_spline *Curve,b3_vector *Result);
b3_index  b3Mansfield           (b3_spline *Spline,b3_f64    *,b3_f64);
void      b3MansfieldVector     (b3_spline *Spline,b3_vector *Result,b3_f64 *,b3_index,b3_index);
void      b3MansfieldNurbsVector(b3_nurbs  *Spline,b3_vector *Result,b3_f64 *,b3_index,b3_index);

/*************************************************************************
**                                                                      **
**                        support routines                              **
**                                                                      **
*************************************************************************/

b3_bool	 b3BSplineInitCurve			(b3_spline *Spline,b3_index,b3_index,b3_bool);
b3_bool	 b3BSplineThroughEndControl	(b3_spline *Spline);
b3_bool	 b3BSplineToBezier			(b3_spline *Spline);
b3_bool	 b3BSplineDegree			(b3_spline *Spline,b3_index);

b3_bool	 b3BSplineInsertControl       (b3_spline *Spline,b3_f64 q,b3_count Mult,b3_index index);
b3_bool	 b3BSplineSurfaceInsertControl(b3_spline *Spline,b3_f64 q,b3_count Mult,b3_index uIndex,b3_index vIndex);
b3_bool	 b3BSplineAppendControl       (b3_spline *Spline,b3_bool,b3_f64,b3_index,b3_index);
b3_bool	 b3BSplineSurfaceAppendControl(b3_spline *Spline,b3_bool,b3_f64,b3_index,b3_index,b3_index);

b3_bool	 b3BNurbsInsertControl       (b3_nurbs *Nurbs,b3_f64,b3_index,b3_index);
b3_bool	 b3BNurbsSurfaceInsertControl(b3_nurbs *Nurbs,b3_f64,b3_index,b3_index,b3_index);
b3_bool	 b3BNurbsAppendControl       (b3_nurbs *Nurbs,b3_bool,b3_f64,b3_index,b3_index);
b3_bool	 b3BNurbsSurfaceAppendControl(b3_nurbs *Nurbs,b3_bool,b3_f64,b3_index,b3_index,b3_index);

#endif
