/*
**
**	$Filename:	b3CameraProjection.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base transformation for camera projection
**
**	(C) Copyright 2005  Steffen A. Mork
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

#include "blz3/raytrace/b3CameraProjection.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2005/04/27 13:55:01  sm
**	- Fixed open/new file error when last path is not accessable.
**	- Divided base transformation into more general version and
**	  some special versions for quadric shapes and camera
**	  projections.
**	- Optimized noise initialization.
**	- Added correct picking with project/unproject for all
**	  view modes. This uses GLU projectton methods.
**	- Added optimization for first level bounding box intersections.
**
**
*/

/*************************************************************************
**                                                                      **
**                        camera projection class                       **
**                                                                      **
*************************************************************************/

b3CameraProjection::b3CameraProjection(b3CameraPart *camera)
{
	m_Base = camera->m_EyePoint;
	m_Dir1 = camera->m_Width;
	m_Dir2 = camera->m_Height;
	b3Vector::b3Sub(&camera->m_ViewPoint, &camera->m_EyePoint, &m_Dir3);
	b3Prepare();
}
