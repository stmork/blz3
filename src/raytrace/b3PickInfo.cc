/*
**
**	$Filename:	b3PickInfo.cc $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Setup pick points depending on shapes
**
**	(C) Copyright 2003  Steffen A. Mork
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
#include "blz3/base/b3Aux.h"
#include "blz3/raytrace/b3PickInfo.h"

/*************************************************************************
**                                                                      **
**                        b3PickInfo implementation                     **
**                                                                      **
*************************************************************************/

b3Color b3PickInfo::m_GridColor(0.7f, 0.7f, 0.7f);

b3PickInfo::b3PickInfo()
{
	m_Shape = null;
}

b3PickInfo::~b3PickInfo()
{
	b3Base<b3Pick>::b3Free();
}

b3_bool b3PickInfo::b3SetShape(b3Shape * shape)
{
	b3_bool result = false;

	if(m_Shape != shape)
	{
		b3Base<b3Pick>::b3Free();
		m_Vertices.b3Clear();
		m_Grid.b3Clear();
		m_Shape = shape;
		if(m_Shape != null)
		{
			m_Shape->b3SetupPicking(this);
			m_Shape->b3SetupGrid(this);
		}
		b3Recompute();
		result = true;
	}
	return result;
}

void b3PickInfo::b3Modified()
{
	m_Vertices.b3Clear();
	m_Grid.b3Clear();
	if(m_Shape != null)
	{
		m_Shape->b3SetupGrid(this);
		m_Shape->b3Recompute();
	}
	b3Recompute();
}

void b3PickInfo::b3AddVertex(b3_vector * point)
{
	b3_gl_vertex vertex;

	vertex.v.x = point->x;
	vertex.v.y = point->y;
	vertex.v.z = point->z;
	vertex.t.s = 0;
	vertex.t.t = 0;

	m_Vertices.b3Add(vertex);
}

void b3PickInfo::b3AddLine(b3_index a, b3_index b)
{
	b3_gl_line line;

	line.a = a;
	line.b = b;

	m_Grid.b3Add(line);
}

void b3PickInfo::b3ComputeVertices()
{
	glVertexElements->b3SetVertices(m_Vertices.b3GetBuffer());
	glVertexElements->b3SetCount(m_Vertices.b3GetCount());
}

void b3PickInfo::b3ComputeIndices()
{
	glGridElements->b3SetGrids(m_Grid.b3GetBuffer());
	glGridElements->b3SetCount(m_Grid.b3GetCount());
}

void b3PickInfo::b3GetGridColor(b3Color & color)
{
	color = m_GridColor;
}
