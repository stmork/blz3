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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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

void b3Shape::b3ComputeBound(b3CondLimit *limit)
{
	b3Item      *item;
	b3Condition *cond;

	limit->x1 = -1;
	limit->y1 = -1;
	limit->x2 =  1;
	limit->y2 =  1;

	B3_FOR_BASE(&heads[1],item)
	{
		cond = (b3Condition *)item;
		cond->b3ComputeBound(limit);
	}
}

void b3Shape::b3GetDiffuseColor(b3_color *color)
{
	b3Item     *item;
	b3Material *material;
	b3_color    ambient,specular;

	color->r = 0.1f;
	color->g = 0.5f;
	color->b = 1.0f;
	color->a = 0.0f;

	B3_FOR_BASE(&heads[2],item)
	{
		material = (b3Material *)item;
		if (material->b3GetColors(color,&ambient,&specular))
		{
			return;
		}
	}
}

void b3Shape::b3Intersect()
{
}


b3Shape2::b3Shape2(b3_size class_size,b3_u32 class_type) : b3Shape(class_size, class_type)
{
}

b3Shape2::b3Shape2(b3_u32 class_type) : b3Shape(sizeof(b3Shape2), class_type)
{
}

b3Shape2::b3Shape2(b3_u32 *src) : b3Shape(src)
{
	b3InitVector(&Base);
	b3InitVector(&Dir1);
	b3InitVector(&Dir2);
}


b3Shape3::b3Shape3(b3_size class_size,b3_u32 class_type) : b3RenderShape(class_size, class_type)
{
}

b3Shape3::b3Shape3(b3_u32 class_type) : b3RenderShape(sizeof(b3Shape3), class_type)
{
}

b3Shape3::b3Shape3(b3_u32 *src) : b3RenderShape(src)
{
	b3InitVector();  // This is Base[0]
	b3InitVector();  // This is Base[1]
	b3InitVector();  // This is Base[2]
	b3InitVector(&Base);
	b3InitVector(&Dir1);
	b3InitVector(&Dir2);
	b3InitVector(&Dir3);
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
