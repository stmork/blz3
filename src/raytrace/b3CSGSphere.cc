/*
**
**	$Filename:	b3CSGSphere.cc $
**	$Release:	Dortmund 2001 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing CSG sphere
**
**	(C) Copyright 2001  Steffen A. Mork
**	´   All Rights Reserved
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

b3CSGSphere::b3CSGSphere(b3_u32 class_type) : b3CSGShape(sizeof(b3CSGSphere), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir,50);
}

b3CSGSphere::b3CSGSphere(b3_u32 *src) : b3CSGShape(src)
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
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = ((SinCosSteps >> 1) + 1) * (SinCosSteps + 1);
	b3GetEllipsoidIndexCount(gridCount,polyCount);
}

void b3CSGSphere::b3GetStencilBoundInfo(b3_stencil_bound *info)
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
	b3ComputeSphereVertices(m_Base,m_Dir);
}

void b3CSGSphere::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3CSGSphere::b3ComputeNormals(b3_bool normalize)
{
	b3ComputeSphereNormals(m_Base,normalize);
}

void b3CSGSphere::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3Matrix::b3VMul (transformation,&m_Base,&m_Base,true);
	b3Matrix::b3VMul (transformation,&m_Dir, &m_Dir, false);
	b3TransformVertices(transformation,is_affine);
}

void b3CSGSphere::b3SetupPicking(b3PickInfo *info)
{
	info->b3AddPickPoint(&m_Base,"b");
	info->b3AddPickDir(  &m_Base,&m_Dir,"r");
}

b3_bool b3CSGSphere::b3Prepare(b3_preparation_info *prep_info)
{
	m_QuadRadius = b3Vector::b3QuadLength(&m_Dir);
	return b3Shape::b3Prepare(prep_info);
}

void b3CSGSphere::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
}

b3_count b3CSGSphere::b3GetMaxIntersections()
{
	return 2;
}
