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
#ifdef BLZ3_USE_OPENGL
	b3_index   i,offset = SinCosSteps * 2;
	b3_vector *Vector;

	Vector = (b3_vector *)glVertices;
	for (i = 0;i < SinCosSteps;i++)
	{
		b3Vector::b3LinearCombine(&m_Base,&m_Dir1,&m_Dir2,Cos[i],Sin[i],Vector);
		b3Vector::b3Add(&Vector[0],&m_Dir3,&Vector[1]);
		Vector += 2;
	}

	// Create copy
	Vector = (b3_vector *)glVertices;
	for (i = 0;i < offset;i++)
	{
		Vector[i + offset] = Vector[i];
	}
	Vector += offset;
	Vector += offset;

	*Vector++ = m_Base;
	b3Vector::b3Add(&m_Base,&m_Dir3,Vector);
#endif
}

void b3CSGCylinder::b3ComputeIndices()
{
#ifdef BLZ3_USE_OPENGL
	GLushort *gPtr    = glGrids;
	GLushort *pPtr    = glPolygons;
	b3_index   offset = SinCosSteps * 2;
	b3_index   mid    = SinCosSteps * 4;
	b3_index   i;

	for (i = 0;i < offset;i+=2)
	{
		// bottom line
		*gPtr++ =  i;
		*gPtr++ = (i + 2) % offset;

		// up line
		*gPtr++ = i;
		*gPtr++ = i + 1;

		// top line
		*gPtr++ =  i + 1;
		*gPtr++ = (i + 3) % offset;

		// bottom face
		*pPtr++ = offset +  i;
		*pPtr++ = offset + (i + 2) % offset;
		*pPtr++ = mid;

		// cylinder face lower left
		*pPtr++ =  i;
		*pPtr++ = (i + 2) % offset;
		*pPtr++ =  i + 1;

		// cylinder face upper right
		*pPtr++ = (i + 3) % offset;
		*pPtr++ =  i + 1;
		*pPtr++ = (i + 2) % offset;

		// top face
		*pPtr++ = offset +  i + 1;
		*pPtr++ = offset + (i + 3) % offset;
		*pPtr++ = mid + 1;
	}
#endif
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
