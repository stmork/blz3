/*
**
**      $Filename:      b3SplineCurve.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing BSpline curve
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

#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.7  2001/12/30 14:16:58  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.6  2001/10/11 16:06:33  sm
**      - Cleaning up b3BSpline with including isolated methods.
**      - Cleaning up endian conversion routines and collecting into
**        b3Endian
**      - Cleaning up some datatypes for proper display in Together.
**
**      Revision 1.5  2001/09/22 16:19:53  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.4  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.3  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.2  2001/08/11 16:29:08  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.1  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineCurveShape::b3SplineCurveShape(b3_u32 class_type) :
	b3SplineCurve(sizeof(b3SplineCurveShape),class_type)
{
}

b3SplineCurveShape::b3SplineCurveShape(b3_u32 *src) : b3SplineCurve(src)
{
}

void b3SplineCurveShape::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	vertCount = B3_MAX_SUBDIV + 1;
}

void b3SplineCurveShape::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	m_Spline.b3DeBoor ((b3_vector *)glVertices,0L);

	xSize  = m_Spline.subdiv;
	ySize  = 0;
#endif
}

void b3SplineCurveShape::b3ComputeIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort  *Index;
	b3_count   SubDiv;
	b3_index   i;

	SubDiv   = m_Spline.subdiv;
	Index    = (GLushort *)b3RenderObject::b3Alloc(SubDiv * 2 * sizeof(GLushort));
	glGrids = Index;
	if (Index)
	{
		for (i = 1;i < SubDiv;i++)
		{
			*Index++ = i - 1;
			*Index++ = i;
		}

		*Index++ = i - 1;
		*Index++ = (m_Spline.closed ? 0 : i);
		glGridCount  = SubDiv;
	}
	else
	{
		glGridCount = 0;
	}
#endif
}
