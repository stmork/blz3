/*
**
**      $Filename:      b3Torus.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing torus
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
**	Revision 1.28  2002/08/04 13:24:56  sm
**	- Found transformation bug: Normals have to be treated as
**	  direction vectors, aren't them?
**	- b3PrepareInfo::m_PrepareProc initialized not only in
**	  debug mode.
**
**	Revision 1.27  2002/07/31 07:30:44  sm
**	- New normal computation. Textures are rendered correctly and
**	  quadrics are shaded correctly. Spheres and doughnuts have
**	  got their own more simple computation.
**	
**	Revision 1.26  2002/03/02 19:52:40  sm
**	- Nasty UnCR
**	- Fixed some compile bugs due to incompatibilities to Visual C++
**	
**	Revision 1.25  2002/02/28 16:58:46  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.24  2002/02/27 20:14:52  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.23  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**	
**	Revision 1.22  2002/02/18 17:50:32  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.21  2002/02/17 21:25:07  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.20  2002/01/20 12:48:51  sm
**	- Added splash screen
**	- Corrected repeat buttons (capture change)
**	
**	Revision 1.19  2001/12/30 22:52:35  sm
**	- Made b3Scene::b3SetCamera() compatible to earlier versions.
**	
**	Revision 1.18  2001/12/30 16:54:15  sm
**	- Inserted safe b3Write() into Lines III
**	- Fixed b3World saving: b3StoreXXX() methods must ensure
**	  buffer before the buffer itself is used.
**	- Extended b3Shape format with shape activation flag. Nice: The
**	  new data structures don't confuse the old Lines II/Blizzard II and
**	  even stores these new values.
**	
**	Revision 1.17  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.16  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.15  2001/10/20 16:14:59  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**	
**	Revision 1.14  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.13  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**	Revision 1.12  2001/10/02 16:01:58  sm
**	- Moving b3Polar into b3Ray but that's not right at all. The
**	  result must be placed there but a simple result from one
**	  intersection must be placed into a temp instance. The same
**	  must be done for surface normals as they result from using
**	  the b3Polar class.
**	
**	Revision 1.11  2001/09/23 18:50:27  sm
**	- Created first raytracing image with Blizzard III. It shows
**	  simply "hit" or "no hit". Spheres and boxes aren't running
**	  yet. Next step: shading!
**	
**	Revision 1.10  2001/09/22 16:19:53  sm
**	- Adding basic shape intersection routines
**
**	Revision 1.9  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.8  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**
**	Revision 1.7  2001/08/16 14:41:24  sm
**	- Some more shading shapes added (only BSPline shapes are missing)
**
**	Revision 1.6  2001/08/11 16:29:08  sm
**	- Nasty UnCR done
**	- Compiling but not running OpenGL under Unix
**
**	Revision 1.5  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**	Revision 1.4  2001/08/10 15:14:37  sm
**	- Now having all shapes implemented for drawing lines.
**
**	Revision 1.3  2001/08/09 15:27:34  sm
**	- Following shapes are newly supported now:
**	  o disk
**	  o cylinder
**	  o cone
**	  o ellipsoid
**	  o torus
**	  o triangles
**	- Done some makefile fixes
**	- Everything is Windozable
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

b3Torus::b3Torus(b3_u32 class_type) : b3SimpleShape(sizeof(b3Torus), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0,10);
	m_aRad = 1;
	m_bRad = 1;
}

b3Torus::b3Torus(b3_u32 *src) : b3SimpleShape(src)
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
	m_aRad  = b3InitFloat();
	m_bRad  = b3InitFloat();
	m_aQuad = b3InitFloat();
	m_bQuad = b3InitFloat();
	b3InitActivation();
}

void b3Torus::b3StoreShape()
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
}

b3_bool b3Torus::b3Prepare()
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

void b3Torus::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3ShapeRenderContext *context = (b3ShapeRenderContext *)ctx;

	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount = (SinCosSteps + 2) * (SinCosSteps + 2);
}

void b3Torus::b3ComputeVertices()
{
	b3ComputeTorusVertices(m_Base,m_Dir1,m_Dir2,m_Dir3,m_aRad,m_bRad);
}

void b3Torus::b3ComputeNormals(b3_bool normalize)
{
	// b3ComputeVertices() does already compute the normals
	// So only normalize if needed
	if (normalize)
	{
		b3ComputeTorusNormals();
	}
}

void b3Torus::b3ComputeIndices()
{
	b3ComputeTorusIndices();
}

void b3Torus::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir1,&m_Dir1,false);
	b3MatrixVMul (transformation,&m_Dir2,&m_Dir2,false);
	b3MatrixVMul (transformation,&m_Dir3,&m_Dir3,false);
	b3TransformVertices(transformation,is_affine);
}

void b3Torus::b3GetStencilBoundInfo(b3_stencil_bound *info)
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
