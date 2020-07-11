/*
**
**	$Filename:	b3Cylinder.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing cylinder
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

b3Cylinder::b3Cylinder(b3_u32 class_type) : b3Shape3(sizeof(b3Cylinder), class_type)
{
}

b3Cylinder::b3Cylinder(b3_u32 * src) : b3Shape3(src)
{
}

void b3Cylinder::b3GetCount(
	b3RenderContext * ctx,
	b3_count    &    vertCount,
	b3_count    &    gridCount,
	b3_count    &    polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount = SinCosSteps + SinCosSteps + 6;
}

void b3Cylinder::b3AllocVertexMemory(b3RenderContext * ctx)
{
	b3ShapeRenderContext * context = (b3ShapeRenderContext *)ctx;

	b3RenderObject::b3AllocVertexMemory(context);
	glGridElements->b3SetGrids(context->b3GetCylinderIndices());
	glPolygonElements->b3SetPolygons(context->b3GetCylinderPolygons());
}

void b3Cylinder::b3FreeVertexMemory()
{
	glVertexElements->b3FreeVertexMemory();
}

void b3Cylinder::b3ComputeVertices()
{
	b3ComputeCylinderVertices(m_Base, m_Dir1, m_Dir2, m_Dir3);
}

void b3Cylinder::b3ComputeIndices()
{
	b3ComputeCylinderIndices();
}
