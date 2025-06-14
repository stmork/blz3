/*
**
**	$Filename:	b3CSGSphere.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing CSG sphere
**
**	(C) Copyright 2001  Steffen A. Mork
**	Ž   All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3CSGSphere::b3CSGSphere(const b3_u32 class_type) : b3CSGShape(sizeof(b3CSGSphere), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir, 50);
}

b3CSGSphere::b3CSGSphere(const b3_u32 * src) : b3CSGShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir);

	b3InitFloat(); // This is QuadRadius
	b3InitInt();   // This is Index

	m_Operation = (b3_csg_operation)b3InitInt();
	b3InitActivation();
}

void b3CSGSphere::b3StoreShape()
{
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir);
	b3StoreFloat(m_QuadRadius);
	b3StoreInt(m_Index);
	b3StoreInt(m_Operation);
}

void b3CSGSphere::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = ((SinCosSteps >> 1) + 1) * (SinCosSteps + 1);
	b3GetEllipsoidIndexCount(gridCount, polyCount);
}

void b3CSGSphere::b3GetStencilBoundInfo(b3_stencil_bound * info) const
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

void b3CSGSphere::b3ComputeVertices()
{
	b3ComputeSphereVertices(m_Base, m_Dir);
}

void b3CSGSphere::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3CSGSphere::b3ComputeNormals(bool normalize)
{
	b3ComputeSphereNormals(m_Base, normalize);
}

void b3CSGSphere::b3Transform(b3_matrix * transformation, bool is_affine)
{
	b3Matrix::b3VMul(transformation, &m_Base, &m_Base, true);
	b3Matrix::b3VMul(transformation, &m_Dir, &m_Dir, false);
	b3TransformVertices(transformation, is_affine);
}

void b3CSGSphere::b3SetupPicking(b3PickInfo * info)
{
	info->b3AddPickPoint(&m_Base, "b");
	info->b3AddPickDir(&m_Base, &m_Dir, "r");
}

bool b3CSGSphere::b3Prepare(b3_preparation_info * prep_info)
{
	m_QuadRadius = b3Vector::b3QuadLength(&m_Dir);
	return b3Shape::b3Prepare(prep_info);
}

void b3CSGSphere::b3InverseMap(b3_ray * ray B3_UNUSED, b3_csg_point * point B3_UNUSED)
{
}

b3_count b3CSGSphere::b3GetMaxIntersections() const
{
	return 2;
}
