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
**      Revision 1.17  2002/02/28 16:58:46  sm
**      - Added torus dialogs.
**      - Fixed material and stencil handling when not activating
**        sheet page.
**      - Further cleanup of edit dialogs done.
**      - Corrected shading of CSG cylinder and CSG cone (added
**        shaded top and bottom plate).
**
**      Revision 1.16  2002/02/23 22:02:49  sm
**      - Added shape/object edit.
**      - Added shape/object deletion.
**      - Added (de-)activation even for shapes.
**      - Added create/change dialogs for following shapes:
**        o sphere
**        o area, disk
**        o cylinder, cone, ellipsoid, box
**      - Changed hierarchy to reflect these changes.
**
**      Revision 1.15  2002/02/19 16:26:49  sm
**      - Further CSG interval computing cleanup done.
**
**      Revision 1.14  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
**
**      Revision 1.13  2002/02/17 21:58:11  sm
**      - Done UnCR
**      - Modified makefiles
**
**      Revision 1.12  2002/02/17 21:25:06  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.11  2002/01/20 12:48:51  sm
**      - Added splash screen
**      - Corrected repeat buttons (capture change)
**
**      Revision 1.10  2001/12/30 22:52:35  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
**      Revision 1.9  2001/12/30 16:54:15  sm
**      - Inserted safe b3Write() into Lines III
**      - Fixed b3World saving: b3StoreXXX() methods must ensure
**        buffer before the buffer itself is used.
**      - Extended b3Shape format with shape activation flag. Nice: The
**        new data structures don't confuse the old Lines II/Blizzard II and
**        even stores these new values.
**
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

b3CSGSphere::b3CSGSphere(b3_u32 class_type) : b3CSGShape(sizeof(b3CSGSphere), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir,50);
}

b3CSGSphere::b3CSGSphere(b3_u32 *src) : b3CSGShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir);

	b3InitFloat(); // This is QuadRadius
	b3InitInt();   // This is Index

	m_Operation = (b3_csg_operation)b3InitInt();
	b3InitActivation();
}

void b3CSGSphere::b3StoreShape()
{
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
	b3ShapeRenderContext *context = (b3ShapeRenderContext *)ctx;

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
	b3TransformVertices(transformation);
}

b3_bool b3CSGSphere::b3Prepare()
{
	m_QuadRadius = b3Vector::b3QuadLength(&m_Dir);
	return b3Shape::b3Prepare();
}

void b3CSGSphere::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
}

b3_count b3CSGSphere::b3GetMaxIntersections()
{
	return 2;
}
