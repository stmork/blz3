/*
**
**      $Filename:      b3Shape.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing shapes
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
**      Revision 1.49  2003/02/17 16:57:46  sm
**      - Inlining head pointer computation.
**
**      Revision 1.48  2002/08/19 16:50:39  sm
**      - Now having animation running, running, running...
**      - Activation handling modified to reflect animation
**        and user transformation actions.
**      - Made some architectual redesigns.
**
**      Revision 1.47  2002/08/04 13:24:56  sm
**      - Found transformation bug: Normals have to be treated as
**        direction vectors, aren't them?
**      - b3PrepareInfo::m_PrepareProc initialized not only in
**        debug mode.
**
**      Revision 1.46  2002/07/31 07:30:44  sm
**      - New normal computation. Textures are rendered correctly and
**        quadrics are shaded correctly. Spheres and doughnuts have
**        got their own more simple computation.
**
**      Revision 1.45  2002/07/21 21:09:37  sm
**      - Now having texture mapping! Texture mapping is only applied to
**        areas and cylinders.
**
**      Revision 1.44  2002/03/10 13:55:15  sm
**      - Added creation dialog for rotation shapes.
**      - Cleaned up derivation of b3SplineRotShape.
**      - Added support for foreign BLZ3_HOME directories.
**
**      Revision 1.43  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.42  2002/03/02 19:52:40  sm
**      - Nasty UnCR
**      - Fixed some compile bugs due to incompatibilities to Visual C++
**
**      Revision 1.41  2002/02/27 20:14:52  sm
**      - Added stencil creation for creating simple shapes.
**      - Fixed material creation.
**      - Cleaned up some files.
**
**      Revision 1.40  2002/02/23 22:02:49  sm
**      - Added shape/object edit.
**      - Added shape/object deletion.
**      - Added (de-)activation even for shapes.
**      - Added create/change dialogs for following shapes:
**        o sphere
**        o area, disk
**        o cylinder, cone, ellipsoid, box
**      - Changed hierarchy to reflect these changes.
**
**      Revision 1.39  2002/02/22 20:18:09  sm
**      - Added shape/bbox creation in object editor. So bigger
**        icons (64x64) for shape selection are created.
**      - Created new class for image list maintainance.
**
**      Revision 1.38  2002/02/18 17:50:32  sm
**      - Corrected some intersection problems concerning CSG
**      - Added CSG shape icons
**      - renamed classes appropriate.
**
**      Revision 1.37  2002/02/17 21:58:11  sm
**      - Done UnCR
**      - Modified makefiles
**
**      Revision 1.36  2002/02/17 21:25:07  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.35  2002/01/20 12:48:51  sm
**      - Added splash screen
**      - Corrected repeat buttons (capture change)
**
**      Revision 1.34  2002/01/10 17:31:11  sm
**      - Some minor GUI updates.
**      - b3BBox::b3Transform() changes m_Matrix member.
**      - Added image selection with image preview.
**
**      Revision 1.33  2001/12/30 22:52:35  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
**      Revision 1.32  2001/12/30 16:54:15  sm
**      - Inserted safe b3Write() into Lines III
**      - Fixed b3World saving: b3StoreXXX() methods must ensure
**        buffer before the buffer itself is used.
**      - Extended b3Shape format with shape activation flag. Nice: The
**        new data structures don't confuse the old Lines II/Blizzard II and
**        even stores these new values.
**
**      Revision 1.31  2001/12/30 14:16:58  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.30  2001/11/08 19:31:33  sm
**      - Nasty CR/LF removal!
**      - Added TGA/RGB8/PostScript image saving.
**      - Hoping to win Peter H. for powerful MFC programming...
**
**      Revision 1.29  2001/11/05 16:57:39  sm
**      - Creating demo scenes.
**      - Initializing some b3Item derived objects
**
**      Revision 1.28  2001/11/01 09:43:11  sm
**      - Some image logging cleanups.
**      - Texture preparing now in b3Prepare().
**      - Done some minor fixes.
**
**      Revision 1.27  2001/10/25 17:41:32  sm
**      - Documenting stencils
**      - Cleaning up image parsing routines with using exceptions.
**      - Added bump mapping
**
**      Revision 1.26  2001/10/20 16:14:59  sm
**      - Some runtime environment cleanups. The CPU count is determined
**        only once.
**      - Introduced preparing routines for raytring to shapes.
**      - Found 5% performance loss: No problem, this was eaten by
**        bug fxing of the rotation spline shapes. (Phuu!)
**      - The next job is to implement different row sampler. Then we
**        should implemented the base set of the Blizzard II raytracer.
**
**      Revision 1.25  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.24  2001/10/18 14:48:26  sm
**      - Fixing refracting problem on some scenes with glasses.
**      - Fixing overlighting problem when using Mork shading.
**      - Finxing some memory leaks (espacially b3TriangleRefs)
**      - Adding texture support to conditions (stencil mapping).
**        Now conditions are ready to work compatible with
**        Blizzard II.
**
**      Revision 1.23  2001/10/17 14:46:02  sm
**      - Adding triangle support.
**      - Renaming b3TriangleShape into b3Triangles and introducing
**        new b3TriangleShape as base class. This results in
**        source file renaming, too.
**      - Fixing soft shadow bug.
**      - Only scene loading background image when activated.
**      - Fixing LDC spline initialization.
**      - Converting Windows paths into right paths on Un*x
**
**      Revision 1.22  2001/10/11 16:06:33  sm
**      - Cleaning up b3BSpline with including isolated methods.
**      - Cleaning up endian conversion routines and collecting into
**        b3Endian
**      - Cleaning up some datatypes for proper display in Together.
**
**      Revision 1.21  2001/10/06 19:24:17  sm
**      - New torus intersection routines and support routines
**      - Added further shading support from materials
**      - Added stencil checking
**      - Changed support for basis transformation for shapes with
**        at least three direction vectors.
**
**      Revision 1.20  2001/10/05 20:30:46  sm
**      - Introducing Mork and Phong shading.
**      - Using light source when shading
**
**      Revision 1.19  2001/10/03 20:17:56  sm
**      - Minor bugfixes
**
**      Revision 1.18  2001/10/03 18:46:45  sm
**      - Adding illumination and recursive raytracing
**
**      Revision 1.17  2001/10/02 16:01:58  sm
**      - Moving b3Polar into b3Ray but that's not right at all. The
**        result must be placed there but a simple result from one
**        intersection must be placed into a temp instance. The same
**        must be done for surface normals as they result from using
**        the b3Polar class.
**
**      Revision 1.16  2001/09/23 18:50:27  sm
**      - Created first raytracing image with Blizzard III. It shows
**        simply "hit" or "no hit". Spheres and boxes aren't running
**        yet. Next step: shading!
**
**      Revision 1.15  2001/09/22 16:19:52  sm
**      - Adding basic shape intersection routines
**
**      Revision 1.14  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.13  2001/08/16 04:28:29  sm
**      - Solving conflicts
**
**      Revision 1.12  2001/08/15 19:52:57  sm
**      - First polygon rendering with Blizzard III (areas only)
**
**      Revision 1.11  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
**      Revision 1.10  2001/08/09 15:27:34  sm
**      - Following shapes are newly supported now:
**        o disk
**        o cylinder
**        o cone
**        o ellipsoid
**        o torus
**        o triangles
**      - Done some makefile fixes
**      - Everything is Windozable
**
**      Revision 1.9  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.8  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.7  2001/08/06 20:49:39  sm
**      - Some conflicts solved
**
**      Revision 1.6  2001/08/06 19:58:59  sm
**      - Drawing area - the first shape we can see with OpenGL
**
**      Revision 1.5  2001/08/06 16:35:35  sm
**      - Drawing first area
**
**      Revision 1.4  2001/08/06 15:26:00  sm
**      - Splitted shapes into their own files
**      - Some preparations for shapde drawing.
**
**      Revision 1.3  2001/07/08 12:30:06  sm
**      - New tool to remove nasty CR/LF from Windoze.
**      - Removing some nasty CR/LF with that new tool.
**
**      Revision 1.2  2001/07/01 16:31:52  sm
**      - Creating MSVC Projects
**      - Welcome to Windows 32
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3InitShape::b3Init()
{
	b3PrintF (B3LOG_DEBUG,"Registering shapes...\n");
	b3Item::b3Register(&b3Sphere::b3StaticInit,           &b3Sphere::b3StaticInit,           SPHERE);
	b3Item::b3Register(&b3Area::b3StaticInit,             &b3Area::b3StaticInit,             AREA);
	b3Item::b3Register(&b3Disk::b3StaticInit,             &b3Disk::b3StaticInit,             DISK);
	b3Item::b3Register(&b3Cylinder::b3StaticInit,         &b3Cylinder::b3StaticInit,         CYLINDER);
	b3Item::b3Register(&b3Cone::b3StaticInit,             &b3Cone::b3StaticInit,             CONE);
	b3Item::b3Register(&b3Ellipsoid::b3StaticInit,        &b3Ellipsoid::b3StaticInit,        ELLIPSOID);
	b3Item::b3Register(&b3Box::b3StaticInit,              &b3Box::b3StaticInit,              BOX);
	b3Item::b3Register(&b3Torus::b3StaticInit,            &b3Torus::b3StaticInit,            TORUS);
	b3Item::b3Register(&b3Triangles::b3StaticInit,        &b3Triangles::b3StaticInit,        TRIANGLES);
	b3Item::b3Register(&b3SplineRotShape::b3StaticInit,   &b3SplineRotShape::b3StaticInit,   SPLINE_ROT);
	b3Item::b3Register(&b3SplineArea::b3StaticInit,       &b3SplineArea::b3StaticInit,       SPLINES_AREA);
	b3Item::b3Register(&b3SplineCylinder::b3StaticInit,   &b3SplineCylinder::b3StaticInit,   SPLINES_CYL);
	b3Item::b3Register(&b3SplineRing::b3StaticInit,       &b3SplineRing::b3StaticInit,       SPLINES_RING);
	b3Item::b3Register(&b3CSGSphere::b3StaticInit,        &b3CSGSphere::b3StaticInit,        CSG_SPHERE);
	b3Item::b3Register(&b3CSGCylinder::b3StaticInit,      &b3CSGCylinder::b3StaticInit,      CSG_CYLINDER);
	b3Item::b3Register(&b3CSGCone::b3StaticInit,          &b3CSGCone::b3StaticInit,          CSG_CONE);
	b3Item::b3Register(&b3CSGEllipsoid::b3StaticInit,     &b3CSGEllipsoid::b3StaticInit,     CSG_ELLIPSOID);
	b3Item::b3Register(&b3CSGBox::b3StaticInit,           &b3CSGBox::b3StaticInit,           CSG_BOX);
	b3Item::b3Register(&b3CSGTorus::b3StaticInit,         &b3CSGTorus::b3StaticInit,         CSG_TORUS);
}

/*************************************************************************
**                                                                      **
**                        basis transformation class                    **
**                                                                      **
*************************************************************************/

b3_bool b3ShapeBaseTrans::b3Prepare()
{
	b3_f64 denom;

	denom = b3Det3(&m_Dir1,&m_Dir2,&m_Dir3);
	if (denom != 0)
	{
		m_Denom        = 1.0 / denom;

		m_Normals[0].x = (m_Dir2.y * m_Dir3.z - m_Dir2.z * m_Dir3.y) / denom;
		m_Normals[0].y = (m_Dir2.z * m_Dir3.x - m_Dir2.x * m_Dir3.z) / denom;
		m_Normals[0].z = (m_Dir2.x * m_Dir3.y - m_Dir2.y * m_Dir3.x) / denom;

		m_Normals[1].x = (m_Dir3.y * m_Dir1.z - m_Dir3.z * m_Dir1.y) / denom;
		m_Normals[1].y = (m_Dir3.z * m_Dir1.x - m_Dir3.x * m_Dir1.z) / denom;
		m_Normals[1].z = (m_Dir3.x * m_Dir1.y - m_Dir3.y * m_Dir1.x) / denom;

		m_Normals[2].x = (m_Dir1.y * m_Dir2.z - m_Dir1.z * m_Dir2.y) / denom;
		m_Normals[2].y = (m_Dir1.z * m_Dir2.x - m_Dir1.x * m_Dir2.z) / denom;
		m_Normals[2].z = (m_Dir1.x * m_Dir2.y - m_Dir1.y * m_Dir2.x) / denom;
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"A quadric has zero volume!\n");
		m_Denom = 0;
	}

	m_DirLen[0] = b3Vector::b3QuadLength(&m_Dir1);
	m_DirLen[1] = b3Vector::b3QuadLength(&m_Dir2);
	m_DirLen[2] = b3Vector::b3QuadLength(&m_Dir3);
	return denom != 0;
}

void b3ShapeBaseTrans::b3BaseTrans(
	b3_line64 *in,
	b3_line64 *out)
{
	b3_f64 xPos,yPos,zPos;
	b3_f64 xDir,yDir,zDir;

	xPos = in->pos.x - m_Base.x;
	yPos = in->pos.y - m_Base.y;
	zPos = in->pos.z - m_Base.z;
	xDir = in->dir.x;
	yDir = in->dir.y;
	zDir = in->dir.z;

	out->pos.x =
		xPos * m_Normals[0].x +
		yPos * m_Normals[0].y +
		zPos * m_Normals[0].z;
	out->pos.y =
		xPos * m_Normals[1].x +
		yPos * m_Normals[1].y +
		zPos * m_Normals[1].z;
	out->pos.z =
		xPos * m_Normals[2].x +
		yPos * m_Normals[2].y +
		zPos * m_Normals[2].z;

	out->dir.x =
		xDir * m_Normals[0].x +
		yDir * m_Normals[0].y +
		zDir * m_Normals[0].z;
	out->dir.y =
		xDir * m_Normals[1].x +
		yDir * m_Normals[1].y +
		zDir * m_Normals[1].z;
	out->dir.z =
		xDir * m_Normals[2].x +
		yDir * m_Normals[2].y +
		zDir * m_Normals[2].z;
}

/*************************************************************************
**                                                                      **
**                        b3Activation base class                       **
**                                                                      **
*************************************************************************/

b3Activation::b3Activation()
{
	b3Activate(false);
	b3Animate(B3_ANIM_DISABLED);
}

b3_bool b3Activation::b3IsActive()
{
	switch(m_AnimActive)
	{
	case B3_ANIM_DISABLED:
		return m_Active;

	case B3_ANIM_DEACTIVE:
		return false;

	case B3_ANIM_ACTIVE:
		return true;
	}
	return false;
}

void b3Activation::b3Activate(b3_bool activate)
{
	m_Active = activate;
}

void b3Activation::b3Animate(b3_bool activate)
{
	m_AnimActive = activate ? B3_ANIM_ACTIVE : B3_ANIM_DEACTIVE;
}

void b3Activation::b3Animate(b3_anim_activation activate)
{
	m_AnimActive = activate;
}

/*************************************************************************
**                                                                      **
**                        shape base class                              **
**                                                                      **
*************************************************************************/

b3Shape::b3Shape(b3_size class_size,b3_u32 class_type) : b3Item(class_size, class_type)
{
	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BUMP);
	m_Heads[1].b3InitBase(CLASS_CONDITION);
	m_Heads[2].b3InitBase(CLASS_MATERIAL);
}

b3Shape::b3Shape(b3_u32 class_type) : b3Item(sizeof(b3Shape), class_type)
{
	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BUMP);
	m_Heads[1].b3InitBase(CLASS_CONDITION);
	m_Heads[2].b3InitBase(CLASS_MATERIAL);
}

b3Shape::b3Shape(b3_u32 *src) : b3Item(src)
{
	b3InitVector(); // This is the normal
	b3InitVector(); // This is Polar.Polar
	b3InitVector(); // This is Polar.ObjectPolar
	b3InitVector(); // This is Polar.BoxPolar
	b3InitNOP();    // This is Custom
}

void b3Shape::b3InitActivation()
{
	b3Activate(B3_PARSE_INDEX_VALID ? b3InitBool() : false);
}

void b3Shape::b3Write()
{
	b3StoreVector(); // This is the normal
	b3StoreVector(); // This is Polar.Polar
	b3StoreVector(); // This is Polar.ObjectPolar
	b3StoreVector(); // This is Polar.BoxPolar
	b3StoreNull();   // This is Custom

	// Trick! Store custom shape data...
	b3StoreShape();

	// and append activation
	b3StoreBool(b3IsActive());
}

void b3Shape::b3StoreShape()
{
}

b3_bool b3Shape::b3Prepare()
{
	b3Item      *item;
	b3Condition *cond;
	b3Bump      *bump;
	b3Material  *material;

	B3_FOR_BASE(b3GetConditionHead(),item)
	{
		cond = (b3Condition *)item;
		if (!cond->b3Prepare())
		{
			return false;
		}
	}

	B3_FOR_BASE(b3GetBumpHead(),item)
	{
		bump = (b3Bump *)item;
		if (!bump->b3Prepare())
		{
			return false;
		}
	}

	B3_FOR_BASE(b3GetMaterialHead(),item)
	{
		material = (b3Material *)item;
		if (!material->b3Prepare())
		{
			return false;
		}
	}

	return true;
}

void b3Shape::b3BumpNormal(b3_ray *ray)
{
	b3Item  *item;
	b3Bump  *bump;
	b3_f64   denom;
	b3_bool  deriv_computed = false;
	b3_bool  deriv_ok       = false;

	b3Normal(ray);
	B3_FOR_BASE(b3GetBumpHead(),item)
	{
		bump = (b3Bump *)item;
		if (bump->b3NeedDeriv())
		{
			if (!deriv_computed)
			{
				deriv_ok       = b3NormalDeriv(ray);
				deriv_computed = true;
				if (deriv_ok)
				{
					b3Vector::b3Normalize(&ray->xDeriv);
					b3Vector::b3Normalize(&ray->yDeriv);
				}
			}
			if (deriv_ok)
			{
				bump->b3BumpNormal(ray);
			}
		}
		else
		{
			bump->b3BumpNormal(ray);
		}
	}

	denom =
		ray->normal.x * ray->normal.x +
		ray->normal.y * ray->normal.y +
		ray->normal.z * ray->normal.z;
	if ((denom != 0) && (denom != 1))
	{
		denom = 1.0 / sqrt(denom);
		ray->normal.x *= denom;
		ray->normal.y *= denom;
		ray->normal.z *= denom;
	}
}

b3Material *b3Shape::b3GetColors(
	b3_ray     *ray,
	b3_surface *surface)
{
	b3Item     *item;
	b3Material *material;

	B3_FOR_BASE(b3GetMaterialHead(),item)
	{
		material = (b3Material *)item;
		if (material->b3GetColors(
			&ray->polar,
			&surface->diffuse,
			&surface->ambient,
			&surface->specular))
		{
			surface->refl = material->b3GetReflection(&ray->polar);
			surface->refr = material->b3GetRefraction(&ray->polar);
			surface->ior  = material->b3GetIndexOfRefraction(&ray->polar);
			surface->se   = material->b3GetSpecularExponent(&ray->polar);
			return material;
		}
	}
	surface->diffuse.r = 0.1f;
	surface->diffuse.g = 0.5f;
	surface->diffuse.b = 1.0f;
	surface->diffuse.a = 0.0f;

	surface->ambient.r = 0.05f;
	surface->ambient.g = 0.25f;
	surface->ambient.b = 0.5f;
	surface->ambient.a = 0.0f;

	surface->specular.r = 0.1f;
	surface->specular.g = 0.1f;
	surface->specular.b = 0.1f;
	surface->specular.a = 0.0f;

	surface->refl =      0.0f;
	surface->refr =      0.0f;
	surface->ior  =      1.0f;
	surface->se   = 100000.0f;

	return null;
}

b3_bool b3Shape::b3CheckStencil(b3_polar_precompute *polar)
{
	return true;
}

void b3Shape::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xMin    = 0;
	info->xMax    = 1;
	info->yMin    = 0;
	info->yMax    = 1;
	info->xUnit   = B3_STENCIL_UNIT;
	info->yUnit   = B3_STENCIL_UNIT;
	info->xFactor = 1;
	info->yFactor = 1;
}

void b3Shape::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3PrintF(B3LOG_NORMAL,"b3Shape::b3Transform() not overloaded!\n");
	B3_ASSERT(true);
}

/*************************************************************************
**                                                                      **
**                        Simple shapes (non CSG)                       **
**                                                                      **
*************************************************************************/

b3SimpleShape::b3SimpleShape(b3_size class_size,b3_u32 class_type) : b3ShapeRenderObject(class_size, class_type)
{
}

b3SimpleShape::b3SimpleShape(b3_u32 class_type) : b3ShapeRenderObject(sizeof(b3SimpleShape), class_type)
{
}

b3SimpleShape::b3SimpleShape(b3_u32 *src) : b3ShapeRenderObject(src)
{
}

b3_bool b3SimpleShape::b3CheckStencil(b3_polar_precompute *polar)
{
	b3Item      *item;
	b3Condition *cond;
	b3_bool      result = true;

	B3_FOR_BASE(b3GetConditionHead(),item)
	{
		cond   = (b3Condition *)item;
		result = cond->b3Conditionate(
			result,cond->b3CheckStencil(polar));
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        Base class for area shapes                    **
**                                                                      **
*************************************************************************/

b3Shape2::b3Shape2(b3_size class_size,b3_u32 class_type) : b3SimpleShape(class_size, class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
}

b3Shape2::b3Shape2(b3_u32 class_type) : b3SimpleShape(sizeof(b3Shape2), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
}

b3Shape2::b3Shape2(b3_u32 *src) : b3SimpleShape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitFloat(); // This is m_NormalLength
	b3InitActivation();
}

void b3Shape2::b3StoreShape()
{
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir1);
	b3StoreVector(&m_Dir2);
	b3StoreFloat(m_NormalLength);
}

b3_bool b3Shape2::b3Prepare()
{
	b3Vector::b3CrossProduct(&m_Dir1,&m_Dir2,&m_Normal);
	m_NormalLength = b3Vector::b3Length(&m_Normal);
	return b3Shape::b3Prepare();
}

void b3Shape2::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir1,&m_Dir1,false);
	b3MatrixVMul (transformation,&m_Dir2,&m_Dir2,false);
	b3TransformVertices(transformation,is_affine);
}


/*************************************************************************
**                                                                      **
**                        Base class for simple quadric shapes          **
**                                                                      **
*************************************************************************/

b3Shape3::b3Shape3(b3_size class_size,b3_u32 class_type) : b3SimpleShape(class_size, class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0,50);
}

b3Shape3::b3Shape3(b3_u32 class_type) : b3SimpleShape(sizeof(b3Shape3), class_type)
{
	b3Vector::b3Init(&m_Base);
	b3Vector::b3Init(&m_Dir1,50, 0, 0);
	b3Vector::b3Init(&m_Dir2, 0,50, 0);
	b3Vector::b3Init(&m_Dir3, 0, 0,50);
}

b3Shape3::b3Shape3(b3_u32 *src) : b3SimpleShape(src)
{
	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitVector(&m_Dir3);
	b3InitInt();   // This is lSize
	b3InitFloat(); // This is m_Denom
	b3InitFloat(); // This is m_DirLen[0]
	b3InitFloat(); // This is m_DirLen[1]
	b3InitFloat(); // This is m_DirLen[2]
	b3InitActivation();
}

void b3Shape3::b3StoreShape()
{
	b3StoreVector(&m_Normals[0]);
	b3StoreVector(&m_Normals[1]);
	b3StoreVector(&m_Normals[2]);
	b3StoreVector(&m_Base);
	b3StoreVector(&m_Dir1);
	b3StoreVector(&m_Dir2);
	b3StoreVector(&m_Dir3);
	b3StoreInt(0); // This is lSize
	b3StoreFloat(m_Denom);
	b3StoreFloat(m_DirLen[0]);
	b3StoreFloat(m_DirLen[1]);
	b3StoreFloat(m_DirLen[2]);
}

b3_bool b3Shape3::b3Prepare()
{
	b3_bool result = false;

	if (b3ShapeBaseTrans::b3Prepare())
	{
		result = b3Shape::b3Prepare();
	}
	return result;
}

void b3Shape3::b3GetStencilBoundInfo(b3_stencil_bound *info)
{
	info->xMin    = 0;
	info->xMax    = 1;
	info->yMin    = 0;
	info->yMax    = 1;
	info->xUnit   = B3_STENCIL_ANGLE;
	info->yUnit   = B3_STENCIL_LENGTH;
	info->xFactor = 360;
	info->yFactor = b3Vector::b3Length(&m_Dir3);;
}

void b3Shape3::b3ComputeNormals(b3_bool normalize)
{
	b3ComputeQuadricNormals(normalize);
}

void b3Shape3::b3Transform(b3_matrix *transformation,b3_bool is_affine)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir1,&m_Dir1,false);
	b3MatrixVMul (transformation,&m_Dir2,&m_Dir2,false);
	b3MatrixVMul (transformation,&m_Dir3,&m_Dir3,false);
	b3TransformVertices(transformation,is_affine);
}
