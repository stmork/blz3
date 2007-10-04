/*
**
**	$Filename:	b3CSGCylinder.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing CSG cylinder
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

b3CSGCylinder::b3CSGCylinder(b3_u32 class_type) : b3CSGShape3(sizeof(b3CSGCylinder),class_type)
{
}

b3CSGCylinder::b3CSGCylinder(b3_u32 *src) : b3CSGShape3(src)
{
}

void b3CSGCylinder::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = SinCosSteps * 4 + 2;
	gridCount   = SinCosSteps * 5;
	polyCount   = SinCosSteps * 4;
}

void b3CSGCylinder::b3ComputeVertices()
{
	b3_gl_vertex *Vector      = *glVertexElements;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_index      i,offset    = SinCosSteps * 2;

	for (i = 0;i < SinCosSteps;i++)
	{
		b3Vector::b3LinearCombine(&m_Base,&m_Dir1,&m_Dir2,
								  b3ShapeRenderContext::m_Cos[i],
								  b3ShapeRenderContext::m_Sin[i],&Vector[0].v);
		Vector[1].v.x = Vector[0].v.x + m_Dir3.x;
		Vector[1].v.y = Vector[0].v.y + m_Dir3.y;
		Vector[1].v.z = Vector[0].v.z + m_Dir3.z;

		Vector += 2;
	}

	// Create copy
	Vector = *glVertexElements;
	for (i = 0;i < offset;i++)
	{
		Vector[i + offset] = Vector[i];
	}
	Vector += offset;
	Vector += offset;

	Vector[1].v.x = (Vector[0].v.x = m_Base.x) + m_Dir3.x;
	Vector[1].v.y = (Vector[0].v.y = m_Base.y) + m_Dir3.y;
	Vector[1].v.z = (Vector[0].v.z = m_Base.z) + m_Dir3.z;
}

void b3CSGCylinder::b3ComputeIndices()
{
	b3_gl_line    *gPtr   = *glGridElements;
	b3_gl_polygon *pPtr   = *glPolygonElements;
	b3_index       offset = b3ShapeRenderContext::m_SubDiv * 2;
	b3_index       mid    = b3ShapeRenderContext::m_SubDiv * 4;
	b3_index       i;

	for (i = 0;i < offset;i+=2)
	{
		B3_GL_LINIT(gPtr,i,  (i + 2) % offset);
		B3_GL_LINIT(gPtr,i,i+1);
		B3_GL_LINIT(gPtr,i+1,(i + 3) % offset);
		B3_GL_LINIT(gPtr,i,  mid);
		B3_GL_LINIT(gPtr,i+1,mid + 1);

		B3_GL_PINIT(pPtr,offset +  i,offset + (i + 2) % offset,mid);
		B3_GL_PINIT(pPtr,i,(i + 2) % offset,i+1);
		B3_GL_PINIT(pPtr,(i + 3) % offset,i+1,(i + 2) % offset);
		B3_GL_PINIT(pPtr,offset +  i + 1,offset + (i + 3) % offset,mid + 1);
	}
}

void b3CSGCylinder::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
	b3_polar  *polar  = &ray->polar;
	b3_line64 *BTLine = point->m_BTLine;
	b3_f64     Q      = ray->Q;

	polar->m_ObjectPolar.x = BTLine->pos.x + Q * BTLine->dir.x;
	polar->m_ObjectPolar.y = BTLine->pos.y + Q * BTLine->dir.y;
	polar->m_ObjectPolar.z = BTLine->pos.z + Q * BTLine->dir.z;
	polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(
						   polar->m_ObjectPolar.x,
						   polar->m_ObjectPolar.y);
	polar->m_Polar.y	= polar->m_ObjectPolar.z;
	polar->m_Polar.z	= 0;
	polar->m_NormalIndex = point->m_Index;
}
