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
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

b3PickInfo::b3PickInfo()
{
	m_Shape = null;
}

b3PickInfo::~b3PickInfo()
{
	b3Free();
}

b3_bool b3PickInfo::b3SetShape(b3Shape *shape)
{
	b3_bool result = false;

	if (m_Shape != shape)
	{
		b3Free();
		m_Grid.b3Clear();
		m_Shape = shape;
		if (m_Shape != null)
		{
			m_Shape->b3SetupPicking(this);
			m_Grid.b3ComputeVertices();
			m_Grid.b3ComputeGrid();
		}
		result = true;
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        b3ControlGrid implementation                  **
**                                                                      **
*************************************************************************/

void b3ControlGrid::b3AllocVertices(b3RenderContext *context)
{
}

void b3ControlGrid::b3FreeVertices()
{
}

void b3ControlGrid::b3ComputeVertices()
{
	glVertex      = m_Vertices.b3GetBuffer();
	glVertexCount = m_Vertices.b3GetCount();
}

void b3ControlGrid::b3ComputeGrid()
{
	glGrids     = m_Grid.b3GetBuffer();
	glGridCount = m_Grid.b3GetCount();
}

void b3ControlGrid::b3GetGridColor(b3_color *color)
{
	b3Color::b3Init(color,0.7,0.7,0.7);
}

void b3ControlGrid::b3Clear()
{
	m_Vertices.b3Clear();
	m_Grid.b3Clear();
}
