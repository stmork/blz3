/*
**
**      $Filename:      b3Sphere.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing sphere
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
**	$Log$
**	Revision 1.11  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**
**	Revision 1.10  2001/09/23 18:50:27  sm
**	- Created first raytracing image with Blizzard III. It shows
**	  simply "hit" or "no hit". Spheres and boxes aren't running
**	  yet. Next step: shading!
**	
**	Revision 1.9  2001/09/22 16:19:53  sm
**	- Adding basic shape intersection routines
**
**	Revision 1.8  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.7  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**
**	Revision 1.6  2001/08/16 14:41:24  sm
**	- Some more shading shapes added (only BSPline shapes are missing)
**
**	Revision 1.5  2001/08/11 16:29:08  sm
**	- Nasty UnCR done
**	- Compiling but not running OpenGL under Unix
**
**	Revision 1.4  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**	Revision 1.3  2001/08/10 15:14:36  sm
**	- Now having all shapes implemented for drawing lines.
**
**	Revision 1.2  2001/08/08 20:12:59  sm
**	- Fixing some makefiles
**	- introducing check/BlzDump (BlzDump moved from tools)
**	- Some further line drawing added
**	- b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**	  b3RenderObject.
**
**	Revision 1.1  2001/08/06 15:26:00  sm
**	- Splitted shapes into their own files
**	- Some preparations for shapde drawing.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3Sphere::b3Sphere(b3_u32 class_type) : b3RenderShape(sizeof(b3Sphere), class_type)
{
}

b3Sphere::b3Sphere(b3_u32 *src) : b3RenderShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir);
	m_QuadRadius = b3Vector::b3QuadLength(&m_Dir);
}

void b3Sphere::b3GetCount(
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

void b3Sphere::b3ComputeVertices()
{
	b3ComputeSphereVertices(m_Base,m_Dir);
}

void b3Sphere::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3Sphere::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir, &m_Dir, false);
	b3Recompute();
}
