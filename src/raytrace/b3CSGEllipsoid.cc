/*
**
**	$Filename:	b3CSGEllipsoid.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing CSG ellipsoid
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

b3CSGEllipsoid::b3CSGEllipsoid(b3_u32 class_type) : b3CSGShape3(sizeof(b3CSGEllipsoid), class_type)
{
}

b3CSGEllipsoid::b3CSGEllipsoid(b3_u32 * src) : b3CSGShape3(src)
{
}

void b3CSGEllipsoid::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = (SinCosSteps + 2) * ((SinCosSteps >> 1) + 1);
	b3GetEllipsoidIndexCount(gridCount, polyCount);
}

void b3CSGEllipsoid::b3ComputeVertices()
{
	b3ComputeEllipsoidVertices(m_Base, m_Dir1, m_Dir2, m_Dir3);
}

void b3CSGEllipsoid::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3CSGEllipsoid::b3InverseMap(b3_ray * ray, b3_csg_point * point)
{
	b3_polar * polar  = &ray->polar;
	b3_line64 * BTLine = point->m_BTLine;
	b3_f64     Q      = ray->Q;

	polar->m_ObjectPolar.x = BTLine->pos.x + Q * BTLine->dir.x;
	polar->m_ObjectPolar.y = BTLine->pos.y + Q * BTLine->dir.y;
	polar->m_ObjectPolar.z = BTLine->pos.z + Q * BTLine->dir.z;

	polar->m_Polar.x = b3Math::b3RelAngleOfScalars(
			polar->m_ObjectPolar.x,
			polar->m_ObjectPolar.y);
	polar->m_Polar.y = asin(polar->m_ObjectPolar.z) * 2.0 / M_PI;
	polar->m_Polar.z = 0;
}

void b3CSGEllipsoid::b3GetStencilBoundInfo(b3_stencil_bound * info)
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
