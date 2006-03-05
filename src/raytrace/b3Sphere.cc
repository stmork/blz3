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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.36  2006/03/05 21:22:36  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.35  2004/12/07 13:20:59  smork
**	- Computing spheres correctly
**	
**	Revision 1.34  2004/07/02 19:28:04  sm
**	- Hoping to have fixed ticket no. 21. But the texture initialization is still slow :-(
**	- Recoupled b3Scene include from CApp*Doc header files to allow
**	  faster compilation.
**	- Removed intersection counter completely because of a mysterious
**	  destruction problem of b3Mutex.
**	
**	Revision 1.33  2004/06/21 09:26:19  sm
**	- Changed rendering: The constant sin/cos tables are now directly
**	  used from b3ShapeRenderContext.
**	- Memory allocation for polygons/grid removed from disks.
**	
**	Revision 1.32  2004/05/10 15:12:09  sm
**	- Unified condition legends for conditions and
**	  texture materials.
**	- Added wrap texture material dialog.
**	
**	Revision 1.31  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.30  2003/02/24 17:32:38  sm
**	- Added further picking support.
**	- Fixed geometry update delay.
**	
**	Revision 1.29  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**	
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
**	Revision 1.26  2002/07/22 18:45:16  sm
**	- Further probing of texture stencil via alpha channel.
**	- Why does Mesa loose the first texture?
**	- Nasty uncr.
**	
**	Revision 1.25  2002/07/22 12:46:08  sm
**	- Added Windows Lines III support for textures
**	- Fixed sphere computation
**	
**	Revision 1.24  2002/03/02 19:52:40  sm
**	- Nasty UnCR
**	- Fixed some compile bugs due to incompatibilities to Visual C++
**	
**	Revision 1.23  2002/02/28 16:58:46  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.22  2002/02/23 22:02:49  sm
**	- Added shape/object edit.
**	- Added shape/object deletion.
**	- Added (de-)activation even for shapes.
**	- Added create/change dialogs for following shapes:
**	  o sphere
**	  o area, disk
**	  o cylinder, cone, ellipsoid, box
**	- Changed hierarchy to reflect these changes.
**	
**	Revision 1.21  2002/02/18 17:50:32  sm
**	- Corrected some intersection problems concerning CSG
**	- Added CSG shape icons
**	- renamed classes appropriate.
**	
**	Revision 1.20  2002/02/17 21:25:07  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.19  2002/01/20 12:48:51  sm
**	- Added splash screen
**	- Corrected repeat buttons (capture change)
**	
**	Revision 1.18  2001/12/30 22:52:35  sm
**	- Made b3Scene::b3SetCamera() compatible to earlier versions.
**	
**	Revision 1.17  2001/12/30 16:54:15  sm
**	- Inserted safe b3Write() into Lines III
**	- Fixed b3World saving: b3StoreXXX() methods must ensure
**	  buffer before the buffer itself is used.
**	- Extended b3Shape format with shape activation flag. Nice: The
**	  new data structures don't confuse the old Lines II/Blizzard II and
**	  even stores these new values.
**	
**	Revision 1.16  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.15  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**	
**	Revision 1.14  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.13  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.12  2001/10/20 16:14:59  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**	
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

b3Sphere::b3Sphere(b3_u32 class_type) : b3SimpleShape(sizeof(b3Sphere), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir,50);
}

b3Sphere::b3Sphere(b3_u32 *src) : b3SimpleShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir);
	b3InitFloat(); // This is m_QuadRadius
	b3InitActivation();
}

void b3Sphere::b3StoreShape()
{
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir);
	b3StoreFloat(m_QuadRadius);
}

b3_bool b3Sphere::b3Prepare()
{
	m_QuadRadius = b3Vector::b3QuadLength(&m_Dir);
	return b3Shape::b3Prepare();
}

void b3Sphere::b3GetCount(
	b3RenderContext *ctx,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	b3_count SinCosSteps = b3ShapeRenderContext::m_SubDiv;

	vertCount   = ((SinCosSteps >> 1) + 1) * (SinCosSteps + 1);
	b3GetEllipsoidIndexCount(gridCount,polyCount);
}

void b3Sphere::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xInfo.min    =  0;
	info->xInfo.max    =  1;
	info->yInfo.min    = -1;
	info->yInfo.max    =  1;
	info->xInfo.unit   = B3_STENCIL_ANGLE;
	info->yInfo.unit   = B3_STENCIL_ANGLE;
	info->xInfo.factor = 360;
	info->yInfo.factor =  90;
}

void b3Sphere::b3ComputeVertices()
{
	b3ComputeSphereVertices(m_Base,m_Dir);
}

void b3Sphere::b3ComputeIndices()
{
	b3ComputeEllipsoidIndices();
}

void b3Sphere::b3ComputeNormals(b3_bool normalize)
{
	b3ComputeSphereNormals(m_Base,normalize);
}

void b3Sphere::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3Matrix::b3VMul (transformation,&m_Base,&m_Base,true);
	b3Matrix::b3VMul (transformation,&m_Dir, &m_Dir, false);
	b3TransformVertices(transformation,is_affine);
}

void b3Sphere::b3SetupPicking(b3PickInfo *info)
{
	info->b3AddPickPoint(&m_Base,"b");
	info->b3AddPickDir(  &m_Base,&m_Dir,"r");
}
