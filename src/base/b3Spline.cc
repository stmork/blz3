/*
**
**	$Filename:	b3Spline.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 2.02 
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III Standard - This file is only a dummy file
**                          to host bspline_errno error variable
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
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
**	Revision 1.9  2002/03/02 19:19:09  sm
**	- Heavily cleaned up BSplines. Now ready!
**
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

/*************************************************************************
**                                                                      **
**                        BSpline error variable                        **
**                                                                      **
*************************************************************************/

b3_bspline_error bspline_errno = BSPLINE_OK;
