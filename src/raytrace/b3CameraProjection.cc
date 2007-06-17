/*
**
**	$Filename:	b3CameraProjection.cc $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3RaytraceInclude.h"

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
