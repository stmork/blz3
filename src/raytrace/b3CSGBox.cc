/*
**
**	$Filename:	b3CSGBox.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing CSG box
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

b3CSGBox::b3CSGBox(b3_u32 class_type) : b3CSGShape3(sizeof(b3CSGBox),class_type)
{
}

b3CSGBox::b3CSGBox(b3_u32 *src) : b3CSGShape3(src)
{
}

void b3CSGBox::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	vertCount = 24;
}

void b3CSGBox::b3ComputeVertices()
{
	b3ComputeBoxVertices(m_Base,m_Dir1,m_Dir2,m_Dir3);
}

void b3CSGBox::b3ComputeNormals(b3_bool normalize)
{
	b3RenderObject::b3ComputeNormals(normalize);
}

void b3CSGBox::b3ComputeIndices()
{
	glGridElements->b3SetCount(12);
	glPolygonElements->b3SetCount(12);

	b3ComputeBoxIndices();
}

void b3CSGBox::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
	b3_polar  *polar  = &ray->polar;
	b3_line64 *BTLine = point->m_BTLine;
	b3_f64     Q      = ray->Q;
	b3_f64     x,y;

	x = BTLine->pos.x + Q * BTLine->dir.x;
	y = BTLine->pos.y + Q * BTLine->dir.y;
	if (x < 0)               x = 0;
	else if (x >= 0.9999999) x = 0.9999999;
	if (y < 0)               y = 0;
	else if (y >= 0.9999999) y = 0.9999999;

	polar->m_Polar.x = polar->m_ObjectPolar.x = x;
	polar->m_Polar.y = polar->m_ObjectPolar.y = y;
	polar->m_Polar.z = polar->m_ObjectPolar.z = BTLine->pos.z + Q * BTLine->dir.z;
	polar->m_NormalIndex = point->m_Index;
}

void b3CSGBox::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xInfo.min    = 0;
	info->xInfo.max    = 1;
	info->yInfo.min    = 0;
	info->yInfo.max    = 1;
	info->xInfo.unit   = B3_STENCIL_LENGTH;
	info->yInfo.unit   = B3_STENCIL_LENGTH;
	info->xInfo.factor = b3Vector::b3Length(&m_Dir1);
	info->yInfo.factor = b3Vector::b3Length(&m_Dir2);
}
