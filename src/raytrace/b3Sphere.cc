/*
**
**	$Filename:	b3Sphere.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing sphere
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

b3Sphere::b3Sphere(b3_u32 class_type) : b3SimpleShape(sizeof(b3Sphere), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir, 50);
}

b3Sphere::b3Sphere(b3_u32 * src) : b3SimpleShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir);
	b3InitFloat(); // This is m_QuadRadius
	b3InitActivation();
}

void b3Sphere::b3StoreShape()
{
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir);
	b3StoreFloat(m_QuadRadius);
}

b3_bool b3Sphere::b3Prepare(b3_preparation_info * prep_info)
{
	m_QuadRadius = b3Vector::b3QuadLength(&m_Dir);
	return b3Shape::b3Prepare(prep_info);
}

void b3Sphere::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = ((SinCosSteps >> 1) + 1) * (SinCosSteps + 1);
	b3GetEllipsoidIndexCount(gridCount, polyCount);
}

void b3Sphere::b3GetStencilBoundInfo(b3_stencil_bound * info)
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

void b3Sphere::b3ComputeVertices()
{
	b3ComputeSphereVertices(m_Base, m_Dir);
}

void b3Sphere::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3Sphere::b3ComputeNormals(b3_bool normalize)
{
	b3ComputeSphereNormals(m_Base, normalize);
}

void b3Sphere::b3Transform(b3_matrix * transformation, b3_bool is_affine)
{
	b3Matrix::b3VMul(transformation, &m_Base, &m_Base, true);
	b3Matrix::b3VMul(transformation, &m_Dir, &m_Dir, false);
	b3TransformVertices(transformation, is_affine);
}

void b3Sphere::b3SetupPicking(b3PickInfo * info)
{
	info->b3AddPickPoint(&m_Base, "b");
	info->b3AddPickDir(&m_Base, &m_Dir, "r");
}
