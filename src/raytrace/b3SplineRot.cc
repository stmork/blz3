/*
**
**      $Filename:      b3SplineRot.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing BSpline rotation shape
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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.8  2001/10/11 16:06:33  sm
**      - Cleaning up b3BSpline with including isolated methods.
**      - Cleaning up endian conversion routines and collecting into
**        b3Endian
**      - Cleaning up some datatypes for proper display in Together.
**
**      Revision 1.7  2001/09/22 16:19:53  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.6  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.5  2001/08/17 19:28:54  sm
**      - Now able to draw every shape by lines or shaded.
**        It's great!
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

b3SplineRotShape::b3SplineRotShape(b3_u32 class_type) :
	b3SplineCurve(sizeof(b3SplineCurve),class_type)
{
}

b3SplineRotShape::b3SplineRotShape(b3_u32 *src) : b3SplineCurve(src)
{
}

void b3SplineRotShape::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3RenderShapeContext *context = (b3RenderShapeContext *)ctx;

	SinCosSteps = context->b3GetSubdiv();

	ySubDiv   = Spline.subdiv;
	xSubDiv   = SinCosSteps;
	if (!Spline.closed) ySubDiv++;

	vertCount = (Spline.subdiv + 1) * SinCosSteps;
	gridCount = SinCosSteps * (Spline.subdiv + ySubDiv);
	polyCount = SinCosSteps *  Spline.subdiv * 2;
}

void b3SplineRotShape::b3ComputeVertices()
{
#ifdef BLZ3_USE_OPENGL
	b3_matrix  Matrix;
	b3Spline   AuxSpline;
	b3_vector  AuxControls[B3_MAX_CONTROLS + 1];
	b3_vector *Vector;
	b3_index   i,a;

	// Build rotation matrix
	b3MatrixRotVec (null,&Matrix,&Axis,M_PI * 2 / SinCosSteps);

	// Copy BSpline
	AuxSpline          = Spline;
	AuxSpline.controls = AuxControls;
	for (i = 0;i < AuxSpline.control_num;i++)
	{
		AuxControls[i] = Controls[i];
	}

	Vector = (b3_vector *)glVertices;
	for (a = 0;a < SinCosSteps;a++)
	{
		// Compute curve
		Vector += AuxSpline.b3DeBoor(Vector,0);

		// Rotate control points
		for (i = 0;i < AuxSpline.control_num;i++)
		{
			b3MatrixVMul (&Matrix,&AuxControls[i],&AuxControls[i],true);
		}
	}

	xSize  = SinCosSteps;
	ySize  = AuxSpline.subdiv;
#endif
}

void b3SplineRotShape::b3ComputeIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr;
	GLushort *pPtr;
	b3_index   a,x1,x2,y1,y2;
	b3_count   yStep;
	b3_f64     sStep;

	yStep = Spline.subdiv + 1;
	sStep = (b3_f64)Spline.subdiv / SinCosSteps;

	gPtr  = glGrids;
	pPtr  = glPolygons;
	x1    = 0;
	
	// for each curve
	for (a = 0;a < SinCosSteps;a++)
	{
		x2 = (a + 1) % SinCosSteps * yStep;

		// curve itself
		for (y1 = 0;y1 < Spline.subdiv;y1++)
		{
			y2 = (y1 + 1) % ySubDiv;

			*gPtr++ = x1 + y1;
			*gPtr++ = x1 + y2;

			*pPtr++ = x1 + y1;
			*pPtr++ = x2 + y1;
			*pPtr++ = x1 + y2;

			*pPtr++ = x2 + y2;
			*pPtr++ = x1 + y2;
			*pPtr++ = x2 + y1;
		}

		// lines between curves
		for (y1 = 0;y1 < ySubDiv;y1++)
		{
			*gPtr++ = x1 + y1;
			*gPtr++ = x2 + y1;
		}
		x1 += yStep;
	}
#endif
}
