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
	b3DeBoor (&Spline,(b3_vector *)glVertices,0L);

	xSize  = Spline.subdiv;
	ySize  = 0;
#endif
}

void b3SplineCurveShape::b3ComputeIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort  *Index;
	b3_count   SubDiv;
	b3_index   i;

	SubDiv   = Spline.subdiv;
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
		*Index++ = (Spline.closed ? 0 : i);
		GridCount  = SubDiv;
	}
	else
	{
		GridCount = 0;
	}
#endif
}

void b3SplineCurveShape::b3Intersect()
{
}
