/*
**
**	$Filename:	b3PickInfo.cc $ 
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
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

#include "blz3/base/b3Aux.h"
#include "blz3/raytrace/b3PickInfo.h"
#include "blz3/raytrace/b3Shape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**
**	Revision 1.7  2003/03/04 20:37:38  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.6  2003/02/26 16:36:16  sm
**	- Sorted drawing colors and added configuration support
**	  to dialog.
**	
**	Revision 1.5  2003/02/25 19:26:07  sm
**	- Fixed missing grid update on pick move
**	
**	Revision 1.4  2003/02/25 15:56:21  sm
**	- Added SplineRot to control grid drawing.
**	- Added support for pixel format selection in dialog items
**	- Restructured b3PickInfo
**	
**	Revision 1.3  2003/02/24 19:18:06  sm
**	- spline control grid drawing completed.
**	
**	Revision 1.2  2003/02/24 17:32:38  sm
**	- Added further picking support.
**	- Fixed geometry update delay.
**	
**	Revision 1.1  2003/02/23 21:15:41  sm
**	- First shape picking
**	
*/

/*************************************************************************
**                                                                      **
**                        b3PickInfo implementation                     **
**                                                                      **
*************************************************************************/

b3Color b3PickInfo::m_GridColor(0.7f,0.7f,0.7f);

b3PickInfo::b3PickInfo()
{
	m_Shape = null;
}

b3PickInfo::~b3PickInfo()
{
	b3Base<b3Pick>::b3Free();
}

b3_bool b3PickInfo::b3SetShape(b3Shape *shape)
{
	b3_bool result = false;

	if (m_Shape != shape)
	{
		b3Base<b3Pick>::b3Free();
		m_Vertices.b3Clear();
		m_Grid.b3Clear();
		m_Shape = shape;
		if (m_Shape != null)
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
	if (m_Shape != null)
	{
		m_Shape->b3SetupGrid(this);
		m_Shape->b3Recompute();
	}
	b3Recompute();
}

void b3PickInfo::b3AddVertex(b3_vector *point)
{
	b3_gl_vertex vertex;

	vertex.v.x = point->x;
	vertex.v.y = point->y;
	vertex.v.z = point->z;
	vertex.t.s = 0;
	vertex.t.t = 0;
	
	m_Vertices.b3Add(vertex);
}

void b3PickInfo::b3AddLine(b3_index a,b3_index b)
{
	b3_gl_line line;

	line.a = a;
	line.b = b;

	m_Grid.b3Add(line);
}

void b3PickInfo::b3AllocVertices(b3RenderContext *context)
{
}

void b3PickInfo::b3FreeVertices()
{
}

void b3PickInfo::b3ComputeVertices()
{
	glVertex      = m_Vertices.b3GetBuffer();
	glVertexCount = m_Vertices.b3GetCount();
}

void b3PickInfo::b3ComputeIndices()
{
	glGrids     = m_Grid.b3GetBuffer();
	glGridCount = m_Grid.b3GetCount();
}

void b3PickInfo::b3GetGridColor(b3Color &color)
{
	color = m_GridColor;
}
