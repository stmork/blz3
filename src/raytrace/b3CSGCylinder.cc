/*
**
**      $Filename:      b3CSGCylinder.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing CSG cylinder
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
**      Revision 1.13  2002/02/28 16:58:46  sm
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

b3CSGCylinder::b3CSGCylinder(b3_u32 class_type) : b3CSGShape3(sizeof(b3CSGCylinder),class_type)
{
}

b3CSGCylinder::b3CSGCylinder(b3_u32 *src) : b3CSGShape3(src)
{
}

void b3CSGCylinder::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3ShapeRenderContext *context = (b3ShapeRenderContext *)ctx;

	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount   = SinCosSteps * 4 + 2;
	gridCount   = SinCosSteps * 3;
	polyCount   = SinCosSteps * 4;
}

void b3CSGCylinder::b3ComputeVertices()
{
	b3_gl_vertex *Vector   = glVertex;
	b3_index      i,offset = SinCosSteps * 2;

	for (i = 0;i < SinCosSteps;i++)
	{
		b3Vector::b3LinearCombine(&m_Base,&m_Dir1,&m_Dir2,Cos[i],Sin[i],&Vector[0].v);
		b3Vector::b3Add(&Vector[0].v,&m_Dir3,&Vector[1].v);
		Vector += 2;
	}

	// Create copy
	Vector = glVertex;
	for (i = 0;i < offset;i++)
	{
		Vector[i + offset] = Vector[i];
	}
	Vector += offset;
	Vector += offset;

	Vector[1].v.x = (Vector[0].v.x = m_Base.x) + m_Dir3.x;
	Vector[1].v.y = (Vector[0].v.y = m_Base.y) + m_Dir3.y;
	Vector[1].v.z = (Vector[0].v.z = m_Base.z) + m_Dir3.z;
}

void b3CSGCylinder::b3ComputeIndices()
{
	b3_gl_line    *gPtr   = glGrids;
	b3_gl_polygon *pPtr   = glPolygons;
	b3_index       offset = SinCosSteps * 2;
	b3_index       mid    = SinCosSteps * 4;
	b3_index       i;

	for (i = 0;i < offset;i+=2)
	{
		B3_GL_LINIT(gPtr,i,  (i + 2) % offset);
		B3_GL_LINIT(gPtr,i,i+1);
		B3_GL_LINIT(gPtr,i+1,(i + 3) % offset);

		B3_GL_PINIT(pPtr,offset +  i,offset + (i + 2) % offset,mid);
		B3_GL_PINIT(pPtr,i,(i + 2) % offset,i+1);
		B3_GL_PINIT(pPtr,(i + 3) % offset,i+1,(i + 2) % offset);
		B3_GL_PINIT(pPtr,offset +  i + 1,offset + (i + 3) % offset,mid + 1);
	}
}

void b3CSGCylinder::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
	b3_polar  *polar  = &ray->polar;
	b3_line64 *BTLine = point->m_BTLine;
	b3_f64     Q      = ray->Q;

	polar->object_polar.x = BTLine->pos.x + Q * BTLine->dir.x;
	polar->object_polar.y = BTLine->pos.y + Q * BTLine->dir.y;
	polar->object_polar.z = BTLine->pos.z + Q * BTLine->dir.z;
	polar->polar.x	= b3RelAngleOfScalars(
		polar->object_polar.x,
		polar->object_polar.y);
	polar->polar.y	= polar->object_polar.z;
	polar->polar.z	= 0;
	polar->normal_index = point->m_Index;
}
