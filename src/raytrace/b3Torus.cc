/*
**
**	$Filename:	b3Torus.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing torus
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

b3Torus::b3Torus(b3_u32 class_type) : b3SimpleShape(sizeof(b3Torus), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1, 50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0, 50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0, 10);
	m_aRad = 1;
	m_bRad = 1;
}

b3Torus::b3Torus(b3_u32 * src) : b3SimpleShape(src)
{
	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitVector(&m_Dir3);
	b3InitFloat(); // This is lSize
	b3InitFloat(); // This is Denom
	b3InitFloat(); // This is DirLen[0]
	b3InitFloat(); // This is DirLen[1]
	b3InitFloat(); // This is DirLen[2]
	m_aRad  = b3InitFloat();
	m_bRad  = b3InitFloat();
	m_aQuad = b3InitFloat();
	m_bQuad = b3InitFloat();
	b3InitActivation();
}

void b3Torus::b3StoreShape()
{
	for (b3_loop i = 0; i < 3; i++)
	{
		b3_vector normal;

		b3Vector::b3Init(&normal, &m_Normals[i]);
		b3StoreVector(&normal);
	}
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir1);
	b3StoreVector(&m_Dir2);
	b3StoreVector(&m_Dir3);
	b3StoreInt(0);
	b3StoreFloat(m_Denom);
	b3StoreFloat(m_DirLen[0]);
	b3StoreFloat(m_DirLen[1]);
	b3StoreFloat(m_DirLen[2]);
	b3StoreFloat(m_aRad);
	b3StoreFloat(m_bRad);
	b3StoreFloat(m_aQuad);
	b3StoreFloat(m_bQuad);
}

bool    b3Torus::b3Prepare(b3_preparation_info * prep_info)
{
	b3_f64  denom, scale;
	b3_bool result = false;

	scale = b3Vector::b3Normalize(&m_Dir1);

	denom = b3Vector::b3Normalize(&m_Dir2);
	scale += denom;
	m_aRad *= (scale * 0.5);

	denom = b3Vector::b3Normalize(&m_Dir3);
	m_bRad   *= denom;

	m_aQuad = m_aRad * m_aRad;
	m_bQuad = m_bRad * m_bRad;

	if (b3BaseTransformation::b3Prepare())
	{
		result = b3Shape::b3Prepare(prep_info);
	}
	return result;
}

void b3Torus::b3GetCount(
	b3RenderContext * ctx B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount,
	b3_count     &    polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv + 2;

	vertCount = SinCosSteps * SinCosSteps;
	b3GetTorusIndexCount(gridCount, polyCount);
}

void b3Torus::b3ComputeVertices()
{
	b3ComputeTorusVertices(m_Base, m_Dir1, m_Dir2, m_Dir3, m_aRad, m_bRad);
}

void b3Torus::b3ComputeNormals(bool normalize)
{
	// b3ComputeVertices() does already compute the normals
	// So only normalize if needed
	if (normalize)
	{
		b3ComputeTorusNormals();
	}
}

void b3Torus::b3ComputeIndices()
{
	b3ComputeTorusIndices();
}

void b3Torus::b3Transform(b3_matrix * transformation, bool is_affine)
{
	b3Matrix::b3VMul(transformation, &m_Base, &m_Base, true);
	b3Matrix::b3VMul(transformation, &m_Dir1, &m_Dir1, false);
	b3Matrix::b3VMul(transformation, &m_Dir2, &m_Dir2, false);
	b3Matrix::b3VMul(transformation, &m_Dir3, &m_Dir3, false);
	b3TransformVertices(transformation, is_affine);
}

void b3Torus::b3SetupPicking(b3PickInfo * info)
{
	info->b3AddPickPoint(&m_Base, "b");
}

void b3Torus::b3GetStencilBoundInfo(b3_stencil_bound * info) const
{
	info->xInfo.min    = 0;
	info->xInfo.max    = 1;
	info->yInfo.min    = 0;
	info->yInfo.max    = 1;
	info->xInfo.unit   = B3_STENCIL_ANGLE;
	info->yInfo.unit   = B3_STENCIL_ANGLE;
	info->xInfo.factor = 360;
	info->yInfo.factor = 360;
}
