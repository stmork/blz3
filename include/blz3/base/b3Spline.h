/*
**
**	$Filename:	b3Spline.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Spline proto types
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

#define B3_MAX_CONTROLS        64
#define B3_MAX_SUBDIV          64
#define B3_MAX_DEGREE          (B3_MAX_CONTROLS)
#define B3_MAX_KNOTS          ((B3_MAX_CONTROLS)+(B3_MAX_DEGREE)+1)

#define BSPLINE_SEGMENTS(s) ((s)->closed ?\
	(s)->control_num :\
	(s)->control_num - (s)->degree)

#define BSPLINE_SEGMENTKNOTS(s) ((s)->closed ?\
	(s)->control_num :\
	(s)->control_num - (s)->degree + 1)

/*************************************************************************
**                                                                      **
**                        error codes                                   **
**                                                                      **
*************************************************************************/

typedef enum
{
	BSPLINE_OK                     = 0,
	BSPLINE_TOO_MUCH_CONTROLS,
	BSPLINE_TOO_FEW_CONTROLS,
	BSPLINE_TOO_FEW_MAXKNOTS,
	BSPLINE_TOO_FEW_MAXCONTROLS,
	BSPLINE_MISSING_KNOTS,
	BSPLINE_CLOSED,
	BSPLINE_TOO_LOW_MULTIPLICATION
} b3_spline_error;

extern long BSplineErrno();

/*************************************************************************
**                                                                      **
**                        calculate curves                              **
**                                                                      **
*************************************************************************/

class b3SplineBase
{
protected:
	static b3_spline_error errno;
public:
	b3_count   control_num,knot_num;
	b3_count   control_max,knot_max;
	b3_count   degree,subdiv;
	b3_index   offset;
	b3_bool    closed;

public:
	static b3_spline_error b3Errno();
};

class b3_spline : public b3SplineBase
{
public:
	b3_vector3D *controls;
	b3_f32      *knots;
};

class b3_nurbs : public b3SplineBase
{
public:
	b3_vector4D *controls;
	b3_f32      *knots;
};

class b3_spline_schrott
{
public:

	long  DeBoorOpened		(b3_vector *,long,b3_f64);
	long  DeBoorClosed		(b3_vector *,long,b3_f64);
	long  DeBoor			(b3_vector *,long);
	long  DeBoorControl		(b3_vector *,long);
	long  DeBoorSurfaceControl	(b3_spline *,b3_vector *);
	long  b3Mansfield			(b3_f64 *,b3_f64);
	void  MansfieldVector		(b3_vector *,b3_f64 *,long,long);

	b3_bool	 BSplineInitCurve		(b3_spline *,long,long,b3_bool);
	b3_bool	 BSplineThroughEndControl	(b3_spline *);
	b3_bool	 BSplineToBezier		(b3_spline *);
	b3_bool	 BSplineDegree			(b3_spline *,long);

	b3_bool	 BSplineInsertControl		(b3_spline *,b3_f64,long,long);
	b3_bool	 BSplineSurfaceInsertControl(b3_spline *,b3_f64,long,long,long);
	b3_bool	 BSplineAppendControl		(b3_spline *,b3_bool,b3_f64,long,long);
	b3_bool	 BSplineSurfaceAppendControl(b3_spline *,b3_bool,b3_f64,long,long,long);

private:
	b3_index iFind(b3_f64 q);
};

class b3_nurbs_schrott
{
public:
	long  DeBoor		(b3_nurbs  *,b3_vector *,long);
	void  MansfieldVector	(b3_nurbs  *,b3_vector *,b3_f64 *,long,long);

	b3_bool	 BNurbsInsertControl		(b3_nurbs *,b3_f64,long,long);
	b3_bool	 BNurbsSurfaceInsertControl (b3_nurbs *,b3_f64,long,long,long);
	b3_bool	 BNurbsAppendControl		(b3_nurbs *,b3_bool,b3_f64,long,long);
	b3_bool	 BNurbsSurfaceAppendControl	(b3_nurbs *,b3_bool,b3_f64,long,long,long);
};

#endif
