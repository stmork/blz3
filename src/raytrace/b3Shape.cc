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
	b3Item::b3Register(&b3Sphere::b3Init,           &b3Sphere::b3Init,           SPHERE);
	b3Item::b3Register(&b3Area::b3Init,             &b3Area::b3Init,             AREA);
	b3Item::b3Register(&b3Disk::b3Init,             &b3Disk::b3Init,             DISK);
	b3Item::b3Register(&b3Cylinder::b3Init,         &b3Cylinder::b3Init,         CYLINDER);
	b3Item::b3Register(&b3Cone::b3Init,             &b3Cone::b3Init,             CONE);
	b3Item::b3Register(&b3Ellipsoid::b3Init,        &b3Ellipsoid::b3Init,        ELLIPSOID);
	b3Item::b3Register(&b3Box::b3Init,              &b3Box::b3Init,              BOX);
	b3Item::b3Register(&b3Torus::b3Init,            &b3Torus::b3Init,            TORUS);
	b3Item::b3Register(&b3TriangleShape::b3Init,    &b3TriangleShape::b3Init,    TRIANGLES);
	b3Item::b3Register(&b3SplineCurveShape::b3Init, &b3SplineCurveShape::b3Init, SPLINE);
	b3Item::b3Register(&b3SplineRotShape::b3Init,   &b3SplineRotShape::b3Init,   SPLINE_ROT);
	b3Item::b3Register(&b3SplineArea::b3Init,       &b3SplineArea::b3Init,       SPLINES_AREA);
	b3Item::b3Register(&b3SplineCylinder::b3Init,   &b3SplineCylinder::b3Init,   SPLINES_CYL);
	b3Item::b3Register(&b3SplineRing::b3Init,       &b3SplineRing::b3Init,       SPLINES_RING);
	b3Item::b3Register(&b3Shape::b3Init,            &b3Shape::b3Init,            SHUTUP);
	b3Item::b3Register(&b3CSGSphere::b3Init,        &b3CSGSphere::b3Init,        CSG_SPHERE);
	b3Item::b3Register(&b3CSGCylinder::b3Init,      &b3CSGCylinder::b3Init,      CSG_CYLINDER);
	b3Item::b3Register(&b3CSGCone::b3Init,          &b3CSGCone::b3Init,          CSG_CONE);
	b3Item::b3Register(&b3CSGEllipsoid::b3Init,     &b3CSGEllipsoid::b3Init,     CSG_ELLIPSOID);
	b3Item::b3Register(&b3CSGBox::b3Init,           &b3CSGBox::b3Init,           CSG_BOX);
	b3Item::b3Register(&b3CSGTorus::b3Init,         &b3CSGTorus::b3Init,         CSG_TORUS);
}

b3Shape::b3Shape(b3_size class_size,b3_u32 class_type) : b3Item(class_size, class_type)
{
}

b3Shape::b3Shape(b3_u32 class_type) : b3Item(sizeof(b3Shape), class_type)
{
}

b3Shape::b3Shape(b3_u32 *src) : b3Item(src)
{
	b3InitVector(); // This is the normal
	b3InitVector(); // This is Polar.Polar
	b3InitVector(); // This is Polar.ObjectPolar
	b3InitVector(); // This is Polar.BoxPolar
	b3InitNOP();    // This is Custom
}

b3Base<b3Item> *b3Shape::b3GetBumpHead()
{
	return &heads[0];
}

b3Base<b3Item> *b3Shape::b3GetConditionHead()
{
	return &heads[1];
}

b3Base<b3Item> *b3Shape::b3GetMaterialHead()
{
	return &heads[2];
}

void b3Shape::b3ComputeBound(b3CondLimit *limit)
{
	b3Item      *item;
	b3Condition *cond;

	limit->x1 = -1;
	limit->y1 = -1;
	limit->x2 =  1;
	limit->y2 =  1;

	B3_FOR_BASE(b3GetConditionHead(),item)
	{
		cond = (b3Condition *)item;
		cond->b3ComputeBound(limit);
	}
}

b3_bool b3Shape::b3CheckStencil(b3_polar *polar)
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

void b3Shape::b3BumpNormal(b3_ray *ray)
{
	b3Item *item;
	b3Bump *bump;
	b3_f64  denom;

	b3Normal(ray);
	B3_FOR_BASE(b3GetBumpHead(),item)
	{
		bump = (b3Bump *)item;
		bump->b3BumpNormal(ray);
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

void b3Shape::b3GetDiffuseColor(b3_color *color)
{
	b3Item     *item;
	b3Material *material;
	b3_color    ambient,specular;
	b3_polar    polar;

	color->r = 0.1f;
	color->g = 0.5f;
	color->b = 1.0f;
	color->a = 0.0f;

	B3_FOR_BASE(b3GetMaterialHead(),item)
	{
		material = (b3Material *)item;
		if (material->b3GetColors(&polar,color,&ambient,&specular))
		{
			return;
		}
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

void b3Shape::b3Transform(b3_matrix *transformation)
{
	b3PrintF(B3LOG_NORMAL,"b3Shape::b3Transform() not overloaded!\n");
	B3_ASSERT(true);
	b3Recompute();
}

b3Shape2::b3Shape2(b3_size class_size,b3_u32 class_type) : b3Shape(class_size, class_type)
{
}

b3Shape2::b3Shape2(b3_u32 class_type) : b3Shape(sizeof(b3Shape2), class_type)
{
}

b3Shape2::b3Shape2(b3_u32 *src) : b3Shape(src)
{
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);

	m_Normal.x = m_Dir1.y * m_Dir2.z - m_Dir1.z * m_Dir2.y;
	m_Normal.y = m_Dir1.z * m_Dir2.x - m_Dir1.x * m_Dir2.z;
	m_Normal.z = m_Dir1.x * m_Dir2.y - m_Dir1.y * m_Dir2.x;
	m_NormalLength = b3Length(&m_Normal);
}

void b3Shape2::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir1,&m_Dir1,false);
	b3MatrixVMul (transformation,&m_Dir2,&m_Dir2,false);
	b3Recompute();
}


b3Shape3::b3Shape3(b3_size class_size,b3_u32 class_type) : b3RenderShape(class_size, class_type)
{
}

b3Shape3::b3Shape3(b3_u32 class_type) : b3RenderShape(sizeof(b3Shape3), class_type)
{
}

b3Shape3::b3Shape3(b3_u32 *src) : b3RenderShape(src)
{
	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&m_Base);
	b3InitVector(&m_Dir1);
	b3InitVector(&m_Dir2);
	b3InitVector(&m_Dir3);

	b3InitBaseTrans();
}

void b3ShapeBaseTrans::b3InitBaseTrans()
{
	b3_f64 denom;

	denom = b3Det3(&m_Dir1,&m_Dir2,&m_Dir3);
	if (denom != 0)
	{
		m_Denom = 1.0 / denom;

		m_Normals[0].x = (m_Dir2.y * m_Dir3.z - m_Dir2.z * m_Dir3.y) * m_Denom;
		m_Normals[0].y = (m_Dir2.z * m_Dir3.x - m_Dir2.x * m_Dir3.z) * m_Denom;
		m_Normals[0].z = (m_Dir2.x * m_Dir3.y - m_Dir2.y * m_Dir3.x) * m_Denom;

		m_Normals[1].x = (m_Dir3.y * m_Dir1.z - m_Dir3.z * m_Dir1.y) * m_Denom;
		m_Normals[1].y = (m_Dir3.z * m_Dir1.x - m_Dir3.x * m_Dir1.z) * m_Denom;
		m_Normals[1].z = (m_Dir3.x * m_Dir1.y - m_Dir3.y * m_Dir1.x) * m_Denom;

		m_Normals[2].x = (m_Dir1.y * m_Dir2.z - m_Dir1.z * m_Dir2.y) * m_Denom;
		m_Normals[2].y = (m_Dir1.z * m_Dir2.x - m_Dir1.x * m_Dir2.z) * m_Denom;
		m_Normals[2].z = (m_Dir1.x * m_Dir2.y - m_Dir1.y * m_Dir2.x) * m_Denom;
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"A quadric has zero volume!\n");
	}

	m_DirLen[0] = b3QuadLength(&m_Dir1);
	m_DirLen[1] = b3QuadLength(&m_Dir2);
	m_DirLen[2] = b3QuadLength(&m_Dir3);
}

void b3ShapeBaseTrans::b3BaseTrans(
	b3_dLine *in,
	b3_dLine *out)
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

void b3Shape3::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul (transformation,&m_Base,&m_Base,true);
	b3MatrixVMul (transformation,&m_Dir1,&m_Dir1,false);
	b3MatrixVMul (transformation,&m_Dir2,&m_Dir2,false);
	b3MatrixVMul (transformation,&m_Dir3,&m_Dir3,false);
	b3Recompute();
}


b3CSGShape3::b3CSGShape3(b3_size class_size,b3_u32 class_type) : b3RenderShape(class_size, class_type)
{
}

b3CSGShape3::b3CSGShape3(b3_u32 class_type) : b3RenderShape(sizeof(b3CSGShape3), class_type)
{
}

b3CSGShape3::b3CSGShape3(b3_u32 *src) : b3RenderShape(src)
{
	b3InitVector();  // This is Normals[0]
	b3InitVector();  // This is Normals[1]
	b3InitVector();  // This is Normals[2]
	b3InitVector(&Base);
	b3InitVector(&Dir1);
	b3InitVector(&Dir2);
	b3InitVector(&Dir3);

	b3InitFloat(); // This is lSize
	b3InitFloat(); // This is Denom
	b3InitFloat(); // This is DirLen[0]
	b3InitFloat(); // This is DirLen[1]
	b3InitFloat(); // This is DirLen[2]

	b3InitInt();   // This Index

	Operation = b3InitInt();
}

void b3CSGShape3::b3Transform(b3_matrix *transformation)
{
	b3MatrixVMul (transformation,&Base,&Base,true);
	b3MatrixVMul (transformation,&Dir1,&Dir1,false);
	b3MatrixVMul (transformation,&Dir2,&Dir2,false);
	b3MatrixVMul (transformation,&Dir3,&Dir3,false);
	b3Recompute();
}
