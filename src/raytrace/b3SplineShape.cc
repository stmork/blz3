/*
**
**      $Filename:      b3SplineShape.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing BSpline shapes
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
**      Revision 1.2  2001/08/11 15:59:59  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.1  2001/08/10 15:14:37  sm
**      - Now having all shapes implemented for drawing lines.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3SplineCurve::b3SplineCurve(b3_size class_size,b3_u32 class_type) :
	b3Shape(class_size, class_type)
{
}

b3SplineCurve::b3SplineCurve(b3_u32 class_type) : b3Shape(sizeof(b3SplineCurve), class_type)
{
}

b3SplineCurve::b3SplineCurve(b3_u32 *src) : b3Shape(src)
{
	b3_index i;

	rSubDiv = b3InitInt();
	for (i = 0;i < B3_MAX_KNOTS;i++) Knots[i] = b3InitFloat();
	b3InitSpline(&Spline,null,Knots);
	b3InitVector(&Axis.pos);
	b3InitVector(&Axis.dir);

	Controls = (b3_vector *)b3Item::b3Alloc(Spline.control_max * sizeof(b3_vector));
	Spline.controls = Controls;
	for (i = 0;i < Spline.control_max;i++)
	{
		b3InitVector(&Controls[i]);
	}
}


b3SplineShape::b3SplineShape(b3_size class_size,b3_u32 class_type) :
	b3Shape(class_size, class_type)
{
}

b3SplineShape::b3SplineShape(b3_u32 class_type) : b3Shape(sizeof(b3SplineShape), class_type)
{
}

b3SplineShape::b3SplineShape(b3_u32 *src) : b3Shape(src)
{				 
	b3_index i;
	b3_count control_count;

	b3InitVector(&Axis.pos);
	b3InitVector(&Axis.dir);
	b3InitSpline(&Spline[0],null,Knots[0]);
	b3InitSpline(&Spline[1],null,Knots[1]);

	// FIX ME: Is the order right?
	for (i = 0;i < B3_MAX_KNOTS;i++) Knots[0][i] = b3InitFloat();
	for (i = 0;i < B3_MAX_KNOTS;i++) Knots[1][i] = b3InitFloat();

	control_count = Spline[0].control_max * Spline[1].control_max;
	Controls = (b3_vector *)b3Item::b3Alloc(control_count * sizeof(b3_vector));
	Spline[0].controls =
	Spline[1].controls = Controls;
	for (i = 0;i < control_count;i++) b3InitVector(&Controls[i]);
}

void b3SplineShape::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3RenderShapeContext *context = (b3RenderShapeContext *)ctx;

	Between   = context->b3GetSplineAux();
	vertCount = (B3_MAX_CONTROLS + B3_MAX_CONTROLS) * (B3_MAX_SUBDIV + 1);
}

void b3SplineShape::b3ComputeVertices()
{
	b3_vector *Vector;
	b3_spline *xSpline,*ySpline;
	b3_count   CurveNum,Points;
	b3_index   x,y;
	b3_spline  MySpline;

	Points  = 0;
	Vector  = (b3_vector *)glVertices;

	xSpline           = &Spline[0];
	ySpline           = &Spline[1];

	xSpline->controls =  Controls;
	ySpline->controls =  Controls;
	xSpline->knots    = &Knots[0][0];
	ySpline->knots    = &Knots[1][0];


	// building horizontal splines
	// first create controls for segments of vertical spline...
	b3DeBoorSurfaceControl (xSpline,ySpline,Between);
	MySpline          = *xSpline;
	MySpline.offset   =  CurveNum = B3_BSPLINE_SEGMENTKNOTS(ySpline);
	MySpline.controls =  Between;

	// ... then create real horizontal spline curve.
	for (y = 0;y < CurveNum;y++)
	{
		Points  = b3DeBoor (&MySpline,Vector,y);
		Vector += Points;
		Points += Points;
	}

	// building vertical splines
	// first create controls for segments of horizontal spline...
	b3DeBoorSurfaceControl (ySpline,xSpline,Between);
	MySpline          = *ySpline;
	MySpline.offset   =  CurveNum = B3_BSPLINE_SEGMENTKNOTS(xSpline);
	MySpline.controls =  Between;

	// ... then create real vertical spline curve.
	for (x = 0;x < CurveNum;x++)
	{
		Points  = b3DeBoor (&MySpline,Vector,x);
		Vector += Points;
		Points += Points;
	}

	xSize  = B3_BSPLINE_SEGMENTS(xSpline);
	ySize  = B3_BSPLINE_SEGMENTS(ySpline);

	/*
		PrintF ("Points: %4ld\n",Points);
		PrintF ("xSize:  %4ld\n",xSize);
		PrintF ("ySize:  %4ld\n",ySize);
	*/
}

void b3SplineShape::b3ComputeIndices()
{
	GLushort *Index;
	b3_index  x,y,i=0;
	b3_count  max;

	GridCount = 0;
	Index     = (GLushort *)b3RenderObject::b3Alloc (
		(B3_BSPLINE_SEGMENTKNOTS(&Spline[1]) *
			Spline[0].subdiv +
		 B3_BSPLINE_SEGMENTKNOTS(&Spline[0]) *
			Spline[1].subdiv) * 2 * sizeof(GLushort));
	glGrids = Index;

	if (Index)
	{
			/* horizontal splines */
		max = B3_BSPLINE_SEGMENTKNOTS(&Spline[1]);
		for (y = 0;y < max;y++)
		{
			for (x = 1;x < Spline[0].subdiv;x++)
			{
				*Index++ = i + x - 1;
				*Index++ = i + x;
			}

			*Index++ = i + x - 1;
			*Index++ = (Spline[0].closed ? i : i + x);

			i         += (Spline[0].subdiv + 1);
			GridCount +=  Spline[0].subdiv;
		}

		// vertical splines
		max = B3_BSPLINE_SEGMENTKNOTS(&Spline[0]);
		for (x = 0;x < max;x++)
		{
			for (y = 1;y < Spline[1].subdiv;y++)
			{
				*Index++ = i + y - 1;
				*Index++ = i + y;
			}

			*Index++ = i + y - 1;
			*Index++ = (Spline[1].closed ? i : i + y);

			i         += (Spline[1].subdiv + 1);
			GridCount +=  Spline[1].subdiv;
			
		}

	}

	/*
		PrintF ("GridCount: %4ld\n",GridCount);
	*/
}
