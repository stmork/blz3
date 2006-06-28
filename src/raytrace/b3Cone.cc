/*
**
**      $Filename:      b3Cone.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing cone
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
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

b3Cone::b3Cone(b3_u32 class_type) : b3Shape3(sizeof(b3Cone),class_type)
{
}

b3Cone::b3Cone(b3_u32 *src) : b3Shape3(src)
{
}

void b3Cone::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	vertCount = (b3ShapeRenderContext::m_SubDiv << 1) + 6;
}

void b3Cone::b3AllocVertexMemory(b3RenderContext *ctx)
{
	b3ShapeRenderContext *context = (b3ShapeRenderContext *)ctx;

	b3RenderObject::b3AllocVertexMemory(context);
	GridsCyl  = context->b3GetCylinderIndices();
	PolysCyl  = context->b3GetCylinderPolygons();
	GridsCone = context->b3GetConeIndices();
	PolysCone = context->b3GetConePolygons();
}

void b3Cone::b3ComputeVertices()
{
	b3ComputeConeVertices(m_Base,m_Dir1,m_Dir2,m_Dir3);
}

void b3Cone::b3ComputeIndices()
{
	b3ComputeConeIndices();
}
