/*
**
**      $Filename:      b3CSGSphere.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing CSG sphere
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
**      Revision 1.8  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.7  2001/09/22 16:19:51  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.6  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.5  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.4  2001/08/11 15:59:58  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.3  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.2  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.1  2001/08/06 15:26:00  sm
**      - Splitted shapes into their own files
**      - Some preparations for shapde drawing.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3CSGSphere::b3CSGSphere(b3_u32 class_type) : b3RenderShape(sizeof(b3CSGSphere), class_type)
{
}

b3CSGSphere::b3CSGSphere(b3_u32 *src) : b3RenderShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir);

	b3InitFloat(); // This is QuadRadius
	b3InitInt();   // This is Index

	m_Operation = b3InitInt();
}

void b3CSGSphere::b3Write()
{
	b3Shape::b3Write();
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir);
	b3StoreFloat(m_QuadRadius);
	b3StoreInt(m_Index);
	b3StoreInt(m_Operation);
}

void b3CSGSphere::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3RenderShapeContext *context = (b3RenderShapeContext *)ctx;

	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount   = ((SinCosSteps >> 1) + 1) * (SinCosSteps + 1);
}

void b3CSGSphere::b3ComputeVertices()
{
	b3ComputeSphereVertices(m_Base,m_Dir);
}

void b3CSGSphere::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3CSGSphere::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir, &m_Dir, false);
	b3Recompute();
}
