/*
**
**	$Filename:	b3Nurbs.h        $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork
**
**	Blizzard III - nurbs proto types
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3_BASE_NURBS_H
#define B3_BASE_NURBS_H

#include "blz3/b3Types.h"

/* calculate curve */

long	 BNurbsDeBoorOpened			(b3_nurbs *,b3_vector *,long,b3_f64);
long	 BNurbsDeBoorClosed			(b3_nurbs *,b3_vector *,long,b3_f64);
long	 BNurbsDeBoor				(b3_nurbs *,b3_vector *,long);
long	 BNurbsDeBoorControl		(b3_nurbs *,b3_vector *,long);

/* support routines */

b3_bool	 BNurbsInitCurve			(b3_nurbs *,long,long,b3_bool);
b3_bool	 BNurbsThroughEndControl	(b3_nurbs *);
b3_bool	 BNurbsToBezier				(b3_nurbs *);
b3_bool	 BNurbsDegree				(b3_nurbs *,long);
b3_bool	 BNurbsInsertControl		(b3_nurbs *,b3_f64,long,long);
b3_bool	 BNurbsSurfaceInsertControl	(b3_nurbs *,b3_f64,long,long,long);

#endif
