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
**      Revision 1.7  2001/08/17 19:28:54  sm
**      - Now able to draw every shape by lines or shaded.
**        It's great!
**
**      Revision 1.6  2001/08/17 14:08:34  sm
**      - Now trying to draw BSPline surfaces with own routines.
**
**      Revision 1.5  2001/08/17 04:16:43  sm
**      - Using OpenGL NURBS zu render BSpline areas. But
**        I think mi tessalation is faster.
**
**      Revision 1.4  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.3  2001/08/11 16:29:08  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
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

	// Copy knots
	for (i = 0;i < B3_MAX_KNOTS;i++) Knots[0][i] = b3InitFloat();
	for (i = 0;i < B3_MAX_KNOTS;i++) Knots[1][i] = b3InitFloat();

	// Copy control points
	control_count = Spline[0].control_max * Spline[1].control_max;
	Controls = (b3_vector *)b3Item::b3Alloc(control_count * sizeof(b3_vector));
	Spline[0].controls =
	Spline[1].controls = Controls;
	for (i = 0;i < control_count;i++) b3InitVector(&Controls[i]);

	glSolid = true;
}

void b3SplineShape::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3RenderShapeContext *context = (b3RenderShapeContext *)ctx;
	Between   = context->b3GetSplineAux();

	// Compute number of grid vertices
	GridVertexCount = (B3_MAX_CONTROLS + B3_MAX_CONTROLS) * (B3_MAX_SUBDIV + 1);

	// Compute number of solid vertices. That
	// are usually much more.
	xSubDiv = Spline[0].subdiv;
	ySubDiv = Spline[1].subdiv;

	if (!Spline[0].closed) xSubDiv++;
	if (!Spline[1].closed) ySubDiv++;
	SolidVertexCount = (xSubDiv + 1) * (ySubDiv * 1);

	// Get enough memory
	vertCount = GridVertexCount + SolidVertexCount;
	gridCount =
		B3_BSPLINE_SEGMENTKNOTS(&Spline[1]) * Spline[0].subdiv +
		B3_BSPLINE_SEGMENTKNOTS(&Spline[0]) * Spline[1].subdiv;
	polyCount = Spline[0].subdiv * Spline[1].subdiv * 2;
}

void b3SplineShape::b3ComputeGridVertices()
{
#ifdef BLZ3_USE_OPENGL
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
#endif
}

void b3SplineShape::b3ComputeSolidVertices()
{
	b3_spline  MySpline;
	b3_index   x,y;
	b3_count   SubDiv,index;
	b3_vector *Vector;

	// Building horizontal BSplines
	Vector = Between;
	SubDiv = Spline[0].subdiv + 1;
	for (x = 0;x < SubDiv;x++)
	{
		Vector += b3DeBoor (&Spline[1],Vector,x * Spline[0].offset);
	}

	// Create aux BSpline
	MySpline          = Spline[0];
	MySpline.offset   = Spline[1].subdiv + 1;
	MySpline.controls = Between;

	Vector = (b3_vector *)&glVertices[GridVertexCount * 3];
	index  = 0;
	for (y = 0;y < ySubDiv;y++)
	{
		index += b3DeBoor (&MySpline,Vector,y);
		Vector += xSubDiv;
	}
	B3_ASSERT(index <= SolidVertexCount);
}

void b3SplineShape::b3ComputeVertices()
{
	if (GridVertexCount  > 0) b3ComputeGridVertices();
	if (SolidVertexCount > 0) b3ComputeSolidVertices();
}

void b3SplineShape::b3ComputeGridIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *pPtr;
	b3_index  x,y,i=0;
	b3_count  max;

	// horizontal splines
	pPtr = glGrids;
	max = B3_BSPLINE_SEGMENTKNOTS(&Spline[1]);
	for (y = 0;y < max;y++)
	{
		for (x = 1;x < Spline[0].subdiv;x++)
		{
			*pPtr++ = i + x - 1;
			*pPtr++ = i + x;
		}

		*pPtr++ = i + x - 1;
		*pPtr++ = (Spline[0].closed ? i : i + x);

		i += (Spline[0].subdiv + 1);
	}

	// vertical splines
	max = B3_BSPLINE_SEGMENTKNOTS(&Spline[0]);
	for (x = 0;x < max;x++)
	{
		for (y = 1;y < Spline[1].subdiv;y++)
		{
			*pPtr++ = i + y - 1;
			*pPtr++ = i + y;
		}

		*pPtr++ = i + y - 1;
		*pPtr++ = (Spline[1].closed ? i : i + y);

		i += (Spline[1].subdiv + 1);
		
	}

	/*
		PrintF ("GridCount: %4ld\n",GridCount);
	*/
#endif
}

void b3SplineShape::b3ComputeSolidIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *pPtr;
	b3_index  x,y;

	pPtr = glPolygons;
	for (y = 0;y < Spline[1].subdiv;y++)
	{
		for (x = 0;x < Spline[0].subdiv;x++)
		{
			*pPtr++ = GridVertexCount +  x              + xSubDiv *  y;
			*pPtr++ = GridVertexCount + (x+1) % xSubDiv + xSubDiv *  y;
			*pPtr++ = GridVertexCount +  x              + xSubDiv * ((y+1) % ySubDiv);

			*pPtr++ = GridVertexCount + (x+1) % xSubDiv + xSubDiv * ((y+1) % ySubDiv);
			*pPtr++ = GridVertexCount +  x              + xSubDiv * ((y+1) % ySubDiv);
			*pPtr++ = GridVertexCount + (x+1) % xSubDiv + xSubDiv *  y;
		}
	}
#endif
}

void b3SplineShape::b3ComputeIndices()
{
	if (GridCount > 0) b3ComputeGridIndices();
	if (PolyCount > 0) b3ComputeSolidIndices();
}

#ifdef GLU_NURBS
void b3SplineShape::b3Draw()
{
	// This doesn't work due to the fact that Linux GLU
	// doesn't support NURBS rendering. This doesn't matter
	// because the GLU NURBS support isn't very fast...
	if (!glSolid)
	{
		b3RenderObject::b3Draw();
	}
	else
	{
		b3_color color;

		b3GetDiffuseColor(&color);

		// Leave this for documentation purposes...
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(color.r,color.g,color.b);
		gluBeginSurface(glNURBS);
		gluNurbsSurface(glNURBS,
			Spline[0].knot_num,Spline[0].knots,
			Spline[1].knot_num,Spline[1].knots,
			Spline[0].offset * 3,
			Spline[1].offset * 3,
			(GLfloat *)Controls,
			Spline[0].degree + 1,
			Spline[1].degree + 1,
			GL_MAP2_VERTEX_3);
		gluEndSurface(glNURBS);
	}
}
#endif
