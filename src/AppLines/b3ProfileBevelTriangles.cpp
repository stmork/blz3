/*
**
**	$Filename:	b3ProfileBevelTriangles.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Profile template
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "b3ProfileBevelTriangles.h"
#include "DlgProfileBevelTriangles.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/03/11 13:48:54  sm
**	- Cleaned up dialog titles
**	- Fixed some texture bugs concerning palette copying.
**	- Added a triangles profile.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3Profile implementation                      **
**                                                                      **
*************************************************************************/

b3ProfileBevelTriangles static_profile_bevel_triangles;

b3ProfileBevelTriangles::b3ProfileBevelTriangles()
{
}

b3_bool b3ProfileBevelTriangles::b3Create()
{
	m_Dlg = new CDlgProfileBevelTriangles;
	m_Title.LoadString(IDS_PROFILE_BEVEL_TRIANGLES);
	return m_Dlg != null;
}

b3_bool b3ProfileBevelTriangles::b3MatchClassType(b3_u32 class_type)
{
	return class_type == TRIANGLES;
}

int b3ProfileBevelTriangles::b3AddImage(CImageList *images)
{
	return images->Add(AfxGetApp()->LoadIcon(IDI_PROFILE_BEVEL_TRIANGLES));
}

b3_bool b3ProfileBevelTriangles::b3ComputeProfile(b3Spline *spline,...)
{
	va_list    args;
	b3_f64     xEdge;
	b3_f64     yEdge;
	b3_f64     oblique;
	b3_vector *c = spline->controls;
	b3_f64     factor = 1 - sqrt(0.5);
	int        i;

	B3_ASSERT(c != null);

	va_start(args,spline);
	xEdge   = va_arg(args,b3_f64) * 0.5;
	yEdge   = va_arg(args,b3_f64) * 0.5;
	oblique = va_arg(args,b3_f64);
	va_end(args);

	spline->b3InitCurve(1,12,true);
	spline->b3ClearControls();
	spline->subdiv = 12;

	c[0].x = xEdge;
	c[0].y = yEdge - oblique;
	c[1].x = xEdge - oblique * factor;
	c[1].y = yEdge - oblique * factor;
	c[2].x = xEdge - oblique;
	c[2].y = yEdge;

	// mirror horizontally
	for (i = 0;i < 3;i++)
	{
		c[5 - i].x = -c[i].x;
		c[5 - i].y =  c[i].y;
	}

	// mirror vertically
	for (i = 0;i < 6;i++)
	{
		c[11 - i].x =  c[i].x;
		c[11 - i].y = -c[i].y;
	}
	return true;
}

b3_bool b3ProfileBevelTriangles::b3ComputeShape(b3Spline *spline,b3Shape *base_shape,...)
{
	va_list      args;
	b3Triangles *shape = (b3Triangles *)base_shape;
	b3_index     index,x,y;
	b3_f64       height;
	b3_count     xCount,yCount;
	b3_count     VertexCount,TriaCount;
	b3_vector   *c = spline->controls;
	b3_vertex   *vertex;
	b3_triangle *tria;
				
	B3_ASSERT(c != null);
	va_start(args,base_shape);
	height  = va_arg(args,b3_f64);
	yCount  = va_arg(args,b3_count);
	va_end(args);

	// Init/allocate triangle shape
	xCount      = spline->control_num;
	VertexCount = xCount * yCount + xCount;
	TriaCount   = xCount * yCount * 2;
	shape->b3Init(VertexCount,TriaCount,xCount,yCount);

	// Init vertices
	vertex = shape->m_Vertices;
	for (y = 0;y <= yCount;y++)
	{
		for (x = 0;x < xCount;x++)
		{
			vertex->Point.x = c[x].x;
			vertex->Point.y = c[x].y;
			vertex->Point.z = height * y / yCount;
			vertex++;
		}
	}

	// Init triangles
	tria  = shape->m_Triangles;
	index = 0;
	for (y = 0;y < yCount;y++)
	{
		for (x = 0;x < xCount;x++)
		{
			tria[0].P1 = index +  x;
			tria[0].P2 = index + (x + 1) % xCount;
			tria[0].P3 = index +  x + xCount;

			tria[1].P1 = index + (x + 1) % xCount + xCount;
			tria[1].P2 = index +  x + xCount;
			tria[1].P3 = index + (x + 1) % xCount;
			tria += 2;
		}
		index += xCount;
	}
	return true;
}
