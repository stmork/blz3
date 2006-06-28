/*
**
**	$Filename:	b3Ellipsoid.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing ellipsoid
**
**	(C) Copyright 2001  Steffen A. Mork
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
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3Ellipsoid::b3Ellipsoid(b3_u32 class_type) : b3Shape3(sizeof(b3Ellipsoid),class_type)
{
}

b3Ellipsoid::b3Ellipsoid(b3_u32 *src) : b3Shape3(src)
{
}

void b3Ellipsoid::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = (SinCosSteps + 2) * ((SinCosSteps >> 1) + 1);
	b3GetEllipsoidIndexCount(gridCount,polyCount);
}

void b3Ellipsoid::b3ComputeVertices()
{
	b3ComputeEllipsoidVertices(m_Base,m_Dir1,m_Dir2,m_Dir3);
}

void b3Ellipsoid::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3Ellipsoid::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xInfo.min    =  0;
	info->xInfo.max    =  1;
	info->yInfo.min    = -1;
	info->yInfo.max    =  1;
	info->xInfo.unit   = B3_STENCIL_ANGLE;
	info->yInfo.unit   = B3_STENCIL_ANGLE;
	info->xInfo.factor = 360;
	info->yInfo.factor =  90;
}
