/*
**
**	$Filename:	b3Area.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing area
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

b3_gl_line b3Area::m_AreaGrids[4] =
	{
		{ 0,1 },
		{ 1,2 },
		{ 2,3 },
		{ 3,0 }
	};

b3_gl_polygon b3Area::m_AreaPolygons[2] =
	{
		{ 0,3,1 },
		{ 2,1,3 }
	};

b3_f32 b3Area::m_AreaTexcoord[8] =
	{
		0,0,
		0,1,
		1,1,
		1,0
	};

b3Area::b3Area(b3_u32 class_type) : b3Shape2(sizeof(b3Area),class_type)
{
}

b3Area::b3Area(b3_u32 *src) : b3Shape2(src)
{
}

void b3Area::b3GetCount(b3RenderContext *context,b3_count &verts,b3_count &grids,b3_count &polys)
{
	verts = 4;
}

void b3Area::b3ComputeVertices()
{
	b3_gl_vertex *Vector = *glVertexElements;
	b3_f32        x1,y1,x2,y2;

	x1 = m_Limit.x1;
	y1 = m_Limit.y1;
	x2 = m_Limit.x2;
	y2 = m_Limit.y2;

	// Setup world coordinates
	Vector->t.s = m_AreaTexcoord[0];
	Vector->t.t = m_AreaTexcoord[1];
	Vector->v.x = (b3_f32)(m_Base.x + x1 * m_Dir1.x + y1 * m_Dir2.x);
	Vector->v.y = (b3_f32)(m_Base.y + x1 * m_Dir1.y + y1 * m_Dir2.y);
	Vector->v.z = (b3_f32)(m_Base.z + x1 * m_Dir1.z + y1 * m_Dir2.z);
	Vector++;

	Vector->t.s = m_AreaTexcoord[2];
	Vector->t.t = m_AreaTexcoord[3];
	Vector->v.x = (b3_f32)(m_Base.x + x1 * m_Dir1.x + y2 * m_Dir2.x);
	Vector->v.y = (b3_f32)(m_Base.y + x1 * m_Dir1.y + y2 * m_Dir2.y);
	Vector->v.z = (b3_f32)(m_Base.z + x1 * m_Dir1.z + y2 * m_Dir2.z);
	Vector++;

	Vector->t.s = m_AreaTexcoord[4];
	Vector->t.t = m_AreaTexcoord[5];
	Vector->v.x = (b3_f32)(m_Base.x + x2 * m_Dir1.x + y2 * m_Dir2.x);
	Vector->v.y = (b3_f32)(m_Base.y + x2 * m_Dir1.y + y2 * m_Dir2.y);
	Vector->v.z = (b3_f32)(m_Base.z + x2 * m_Dir1.z + y2 * m_Dir2.z);
	Vector++;

	Vector->t.s = m_AreaTexcoord[6];
	Vector->t.t = m_AreaTexcoord[7];
	Vector->v.x = (b3_f32)(m_Base.x + x2 * m_Dir1.x + y1 * m_Dir2.x);
	Vector->v.y = (b3_f32)(m_Base.y + x2 * m_Dir1.y + y1 * m_Dir2.y);
	Vector->v.z = (b3_f32)(m_Base.z + x2 * m_Dir1.z + y1 * m_Dir2.z);

	xSize = 1;
	ySize = 1;
}

void b3Area::b3ComputeIndices()
{
	glGridElements->b3SetGrids(m_AreaGrids);
	glGridElements->b3SetCount(4);

	glPolygonElements->b3SetPolygons(m_AreaPolygons);
	glPolygonElements->b3SetCount(2);

	b3ComputeBound(&m_Limit);
}

void b3Area::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xInfo.min    = -1;
	info->xInfo.max    =  1;
	info->yInfo.min    = -1;
	info->yInfo.max    =  1;
	info->xInfo.unit   = B3_STENCIL_LENGTH;
	info->yInfo.unit   = B3_STENCIL_LENGTH;
	info->xInfo.factor = b3Vector::b3Length(&m_Dir1);
	info->yInfo.factor = b3Vector::b3Length(&m_Dir2);;
}
