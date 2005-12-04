/*
**
**      $Filename:      b3CSGCone.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing CSG cone
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

#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.24  2005/12/04 16:48:40  sm
**      - Converted some defines into inlined methods.
**
**      Revision 1.23  2004/12/09 08:38:29  smork
**      - Corrected Grid for CSG cylinder/CSG cone.
**
**      Revision 1.22  2004/11/21 14:56:58  sm
**      - Merged VBO development into main trunk.
**
**      Revision 1.21.2.1  2004/11/19 19:38:43  sm
**      - OK. The arrays are drawing correctly and the ATi VBOs are drawing
**        something. The draw buffer seams to be defective. Now we should
**        look what nVIDIA is doing with my code.
**
**      Revision 1.21  2004/07/02 19:28:03  sm
**      - Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**      - Recoupled b3Scene include from CApp*Doc header files to allow
**        faster compilation.
**      - Removed intersection counter completely because of a mysterious
**        destruction problem of b3Mutex.
**
**      Revision 1.20  2004/06/21 09:26:18  sm
**      - Changed rendering: The constant sin/cos tables are now directly
**        used from b3ShapeRenderContext.
**      - Memory allocation for polygons/grid removed from disks.
**
**      Revision 1.19  2004/04/17 09:40:55  sm
**      - Splitting b3Raytrace.h into their components for
**        better oversightment.
**
**      Revision 1.18  2004/04/11 14:05:11  sm
**      - Raytracer redesign:
**        o The reflection/refraction/ior/specular exponent getter
**          are removed. The values are copied via the b3GetColors()
**          method.
**        o The polar members are renamed.
**        o The shape/bbox pointers moved into the ray structure
**      - Introduced wood bump mapping.
**
**      Revision 1.17  2002/08/16 11:40:38  sm
**      - Changed vertex handling for use without OpenGL. Vertex computation
**        is needed for bound computation which is needed for animation. There
**        are still some problems so we have to work further on Windows for
**        better debugging.
**      - b3ExtractExt searches from right instead from left.
**
**      Revision 1.16  2002/07/31 07:30:44  sm
**      - New normal computation. Textures are rendered correctly and
**        quadrics are shaded correctly. Spheres and doughnuts have
**        got their own more simple computation.
**
**      Revision 1.15  2002/07/27 18:51:31  sm
**      - Drawing changed to glInterleavedArrays(). This means that
**        extra normal and texture arrays are omitted. This simplifies
**        correct programming, too.
**
**      Revision 1.14  2002/03/02 19:52:39  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.13  2002/02/28 16:58:45  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.12  2002/02/17 21:58:11  sm
**      - Done UnCR
**      - Modified makefiles
**
**      Revision 1.11  2002/02/17 21:25:06  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.10  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.9  2001/09/22 16:19:51  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.8  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.7  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.6  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.5  2001/08/11 15:59:58  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.4  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.3  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.2  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
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

b3CSGCone::b3CSGCone(b3_u32 class_type) : b3CSGShape3(sizeof(b3CSGCone),class_type)
{
}

b3CSGCone::b3CSGCone(b3_u32 *src) : b3CSGShape3(src)
{
}

void b3CSGCone::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = SinCosSteps * 2 + 2;
	gridCount   = SinCosSteps * 3;
	polyCount   = SinCosSteps * 2;
}

void b3CSGCone::b3ComputeVertices()
{
	b3_index      i;
	b3_gl_vertex *Vector      = *glVertexElements;
	b3_count      SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	for (i = 0;i < SinCosSteps;i++)
	{
		b3Vector::b3LinearCombine(&m_Base,&m_Dir1,&m_Dir2,
			b3ShapeRenderContext::m_Cos[i],
			b3ShapeRenderContext::m_Sin[i],&Vector[i].v);
		Vector[i + SinCosSteps] = Vector[i];
	}
	Vector += (SinCosSteps << 1);

	Vector[1].v.x = (Vector[0].v.x = m_Base.x) + m_Dir3.x;
	Vector[1].v.y = (Vector[0].v.y = m_Base.y) + m_Dir3.y;
	Vector[1].v.z = (Vector[0].v.z = m_Base.z) + m_Dir3.z;
}

void b3CSGCone::b3ComputeIndices()
{
	b3_gl_line    *gPtr        = *glGridElements;
	b3_gl_polygon *pPtr        = *glPolygonElements;
	b3_count       SinCosSteps = b3ShapeRenderContext::m_SubDiv;
	b3_index       offset = SinCosSteps << 1;
	b3_index       i;

	for (i = 0;i < SinCosSteps;i++)
	{
		B3_GL_LINIT(gPtr,i,(i + 1) % SinCosSteps);
		B3_GL_LINIT(gPtr,i,offset);
		B3_GL_LINIT(gPtr,i,offset + 1);

		B3_GL_PINIT(pPtr,i,(i + 1) % SinCosSteps,offset);
		B3_GL_PINIT(pPtr,i + SinCosSteps,(i + 1) % SinCosSteps + SinCosSteps,offset + 1);
	}
}

void b3CSGCone::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
	b3_polar  *polar  = &ray->polar;
	b3_line64 *BTLine = point->m_BTLine;
	b3_f64     Q      = ray->Q;

	polar->m_ObjectPolar.x = BTLine->pos.x + Q * BTLine->dir.x;
	polar->m_ObjectPolar.y = BTLine->pos.y + Q * BTLine->dir.y;
	polar->m_ObjectPolar.z = BTLine->pos.z + Q * BTLine->dir.z;
	polar->m_Polar.x	= b3Math::b3RelAngleOfScalars(
		polar->m_ObjectPolar.x,
		polar->m_ObjectPolar.y);
	polar->m_Polar.y     = polar->m_ObjectPolar.z;
	polar->m_Polar.z     = 0;
	polar->m_NormalIndex = point->m_Index;
}
