/*
**
**      $Filename:      b3Torus.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing CSG torus
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
**      Revision 1.17  2002/02/27 20:14:51  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
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

b3CSGTorus::b3CSGTorus(b3_u32 class_type) : b3CSGShape(sizeof(b3CSGTorus), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0,10);
	m_aRad = 1;
	m_bRad = 1;
}

b3CSGTorus::b3CSGTorus(b3_u32 *src) : b3CSGShape(src)
{
	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitVector(&m_Dir3);

	b3InitFloat(); // This is lSize
	b3InitFloat(); // This is Denom
	b3InitFloat(); // This is DirLen[0]
	b3InitFloat(); // This is DirLen[1]
	b3InitFloat(); // This is DirLen[2]

	m_aRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir1 and Dir2
	m_bRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir3

	b3InitFloat(); // This is aQuad
	b3InitFloat(); // This is bQuad
	b3InitInt();   // This Index

	m_Operation = (b3_csg_operation)b3InitInt();
	b3InitVector(); // This is BTLine.pos
	b3InitVector(); // This is BTLine.dir
	b3InitActivation();
}

void b3CSGTorus::b3StoreShape()
{
	b3StoreVector(&m_Normals[0]);
	b3StoreVector(&m_Normals[1]);
	b3StoreVector(&m_Normals[2]);
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir1);
	b3StoreVector(&m_Dir2);
	b3StoreVector(&m_Dir3);

	b3StoreInt  (0);
	b3StoreFloat(m_Denom);
	b3StoreFloat(m_DirLen[0]);
	b3StoreFloat(m_DirLen[1]);
	b3StoreFloat(m_DirLen[2]);

	b3StoreFloat(m_aRad);
	b3StoreFloat(m_bRad);

	b3StoreFloat(m_aQuad);
	b3StoreFloat(m_bQuad);
	b3StoreInt(m_Index);

	b3StoreInt(m_Operation);
	b3StoreVector(); // This is BTLine.pos
	b3StoreVector(); // This is BTLine.dir
}

void b3CSGTorus::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3RenderShapeContext *context = (b3RenderShapeContext *)ctx;

	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount = (SinCosSteps + 2) * (SinCosSteps + 2);
}

void b3CSGTorus::b3ComputeVertices()
{
	b3ComputeTorusVertices(m_Base,m_Dir1,m_Dir2,m_Dir3,m_aRad,m_bRad);
}

void b3CSGTorus::b3ComputeIndices()
{
	b3ComputeTorusIndices();
}

void b3CSGTorus::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir1,&m_Dir1,false);
	b3MatrixVMul (transformation,&m_Dir2,&m_Dir2,false);
	b3MatrixVMul (transformation,&m_Dir3,&m_Dir3,false);
	b3TransformVertices(transformation);
}

b3_bool b3CSGTorus::b3Prepare()
{
	b3_f64  denom,scale;
	b3_bool result = false;

	if ((scale = b3Vector::b3Normalize(&m_Dir1)) == 0)
	{
		scale = 1;
	}

	if ((denom = b3Vector::b3Normalize(&m_Dir2)) != 0)
	{
		scale += denom;
	}
	else
	{
		scale += 1;
	}
	m_aRad *= (scale * 0.5);

	denom = b3Vector::b3Normalize(&m_Dir3);
	if ((denom != 1) && (denom != 0))
	{
		m_bRad   *= denom;
	}
	m_aQuad = m_aRad * m_aRad;
	m_bQuad = m_bRad * m_bRad;

	if (b3ShapeBaseTrans::b3Prepare())
	{
		result = b3Shape::b3Prepare();
	}
	return result;
}

void b3CSGTorus::b3InverseMap(b3_ray *ray,b3_csg_point *point)
{
	b3_polar  *polar  = &ray->polar;
	b3_line64 *BTLine = point->m_BTLine;
	b3_f64     Q      = ray->Q;
	b3_f64     aQuad,bQuad,val;

	aQuad = polar->object_polar.x = BTLine->pos.x + Q * BTLine->dir.x;
	bQuad = polar->object_polar.y = BTLine->pos.y + Q * BTLine->dir.y;
	        polar->object_polar.z = BTLine->pos.z + Q * BTLine->dir.z;

	val = m_aRad - m_aQuad / sqrt(aQuad * aQuad + bQuad * bQuad);

	polar->polar.x = b3RelAngleOfScalars(aQuad,bQuad);
	polar->polar.y = b3RelAngleOfScalars (val,polar->object_polar.z);
	polar->polar.z = 0;
}

b3_count b3CSGTorus::b3GetMaxIntersections()
{
	return 4;
}

void b3CSGTorus::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xMin    = 0;
	info->xMax    = 1;
	info->yMin    = 0;
	info->yMax    = 1;
	info->xUnit   = B3_STENCIL_ANGLE;
	info->yUnit   = B3_STENCIL_ANGLE;
	info->xFactor = 360;
	info->yFactor = 360;
}
