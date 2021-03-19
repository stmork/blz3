/*
**
**	$Filename:	b3Box.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing box
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

b3Box::b3Box(b3_u32 class_type) : b3Shape3(sizeof(b3Box), class_type)
{
}

b3Box::b3Box(b3_u32 * src) : b3Shape3(src)
{
}

void b3BBox::b3SetName(const char * name)
{
	b3Item::b3SetString(m_BoxName, sizeof(m_BoxName), name);
}

void b3Box::b3GetCount(
	b3RenderContext * context B3_UNUSED,
	b3_count     &    vertCount,
	b3_count     &    gridCount B3_UNUSED,
	b3_count     &    polyCount B3_UNUSED)
{
	vertCount = 24;
}

void b3Box::b3ComputeVertices()
{
	b3ComputeBoxVertices(m_Base, m_Dir1, m_Dir2, m_Dir3);
}

void b3Box::b3ComputeNormals(b3_bool normalize)
{
	b3RenderObject::b3ComputeNormals(normalize);
}

void b3Box::b3ComputeIndices()
{
	glGridElements->b3SetCount(12);
	glPolygonElements->b3SetCount(12);

	b3ComputeBoxIndices();
}

void b3Box::b3GetStencilBoundInfo(b3_stencil_bound * info) const
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
