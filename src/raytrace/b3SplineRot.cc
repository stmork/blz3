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
	vertCount   = (B3_MAX_SUBDIV + 1) * SinCosSteps;
}

void b3SplineRotShape::b3ComputeVertices()
{
	b3_matrix  Matrix;
	b3_spline  AuxSpline;
	b3_vector  AuxControls[B3_MAX_CONTROLS + 1];
	b3_vector *Vector;
	b3_index   i,a;
	b3_count   Points = 0;

	b3MatrixRotVec (null,&Matrix,&Axis,M_PI * 2 / SinCosSteps);
	AuxSpline          = Spline;
	AuxSpline.controls = AuxControls;
	for (i = 0;i < AuxSpline.control_num;i++)
	{
		AuxControls[i] = Controls[i];
	}

	Vector = (b3_vector *)glVertices;
	for (a = 0;a < SinCosSteps;a++)
	{
		Points = b3DeBoor (&AuxSpline,Vector,0);
		for (i = 0;i < AuxSpline.control_num;i++)
		{
			b3MatrixVMul (&Matrix,&AuxControls[i],&AuxControls[i],true);
		}
		Vector += Points;
	}

	xSize  = SinCosSteps;
	ySize  = AuxSpline.subdiv;
}

void b3SplineRotShape::b3ComputeIndices()
{
	GLushort *Index;
	b3_count   SubDiv,sdStep,scStep;
	b3_index   i,a,b;
	b3_f64     sStep;

	SubDiv   = Spline.subdiv;
	sdStep   = SubDiv + 1;
	scStep   = (SinCosSteps < SubDiv ? SinCosSteps : SubDiv);
	sStep    = (b3_f64)SubDiv / scStep;
	if (!Spline.closed) scStep++;

	Index    = (GLushort *)b3RenderObject::b3Alloc(
		SinCosSteps * (SubDiv + scStep) * 2 * sizeof(GLushort));
	glGrids = Index;
	if (Index)
	{
		b = 0;
			/* for each curve */
#ifdef SPLINE_ROT_PROFILE_ONLY
		GridCount = SubDiv;
#else
		GridCount = SinCosSteps * (SubDiv + scStep);
		for (a = 0;a < SinCosSteps;a++)
		{
#endif
				/* curve itself */
			for (i = 0;i < SubDiv;i++)
			{
				*Index++ = b + i;
				*Index++ = b + i + 1;
			}

#ifndef SPLINE_ROT_PROFILE_ONLY
				/* lines between curves */
			for (i = 0;i < scStep;i++)
			{
				*Index++ = (GLushort)(  b                              + i * sStep);
				*Index++ = (GLushort)(((a + 1) % SinCosSteps) * sdStep + i * sStep);
			}
			b += sdStep;
		}
#endif
	}
	else
	{
		GridCount = 0;
	}

	/*
		PrintF ("lines:  %4ld\n",GridCount);
		PrintF ("sdStep: %4ld\n",sdStep);
		PrintF ("scStep: %4ld\n",scStep);
		PrintF ("sStep:  %2.2lf\n",sStep);
	*/
}

void b3SplineRotShape::b3Intersect()
{
}
