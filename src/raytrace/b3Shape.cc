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
	b3Item::b3Register(&b3Sphere::b3Init,        &b3Sphere::b3Init,        SPHERE);
	b3Item::b3Register(&b3Area::b3Init,          &b3Area::b3Init,          AREA);
	b3Item::b3Register(&b3Disk::b3Init,          &b3Disk::b3Init,          DISK);
	b3Item::b3Register(&b3Cylinder::b3Init,      &b3Cylinder::b3Init,      CYLINDER);
	b3Item::b3Register(&b3Cone::b3Init,          &b3Cone::b3Init,          CONE);
	b3Item::b3Register(&b3Ellipsoid::b3Init,     &b3Ellipsoid::b3Init,     ELLIPSOID);
	b3Item::b3Register(&b3Box::b3Init,           &b3Box::b3Init,           BOX);
	b3Item::b3Register(&b3Torus::b3Init,         &b3Torus::b3Init,         TORUS);
	b3Item::b3Register(&b3TriangleShape::b3Init, &b3TriangleShape::b3Init, TRIANGLES);
	b3Item::b3Register(&b3SplineCurve::b3Init,   &b3SplineCurve::b3Init,   SPLINE);
	b3Item::b3Register(&b3SplineCurve::b3Init,   &b3SplineCurve::b3Init,   SPLINE_ROT);
	b3Item::b3Register(&b3SplineArea::b3Init,    &b3SplineArea::b3Init,    SPLINES_AREA);
	b3Item::b3Register(&b3SplineCylinder::b3Init,&b3SplineCylinder::b3Init,SPLINES_CYL);
	b3Item::b3Register(&b3SplineRing::b3Init,    &b3SplineRing::b3Init,    SPLINES_RING);
	b3Item::b3Register(&b3Shape::b3Init,         &b3Shape::b3Init,         SHUTUP);
	b3Item::b3Register(&b3CSGSphere::b3Init,     &b3CSGSphere::b3Init,     CSG_SPHERE);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_CYLINDER);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_CONE);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_ELLIPSOID);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_BOX);
	b3Item::b3Register(&b3CSGTorus::b3Init,      &b3CSGTorus::b3Init,      CSG_TORUS);
}

b3Shape::b3Shape(b3_size class_size,b3_u32 class_type) : b3Item(class_size, class_type)
{
	VertexCount = 0;
	GridCount   = 0;
	PolyCount   = 0;
#ifdef BLZ3_USE_OPENGL
	Vertices = null;
	Grids    = null;
	Polygons = null;
#endif
}

b3Shape::b3Shape(b3_u32 class_type) : b3Item(sizeof(b3Shape), class_type)
{
	VertexCount = 0;
	GridCount   = 0;
	PolyCount   = 0;
#ifdef BLZ3_USE_OPENGL
	Vertices = null;
	Grids    = null;
	Polygons = null;
#endif
}

b3Shape::b3Shape(b3_u32 *src) : b3Item(src)
{
	b3InitVector(); // This is the normal
	b3InitVector(); // This is Polar.Polar
	b3InitVector(); // This is Polar.ObjectPolar
	b3InitVector(); // This is Polar.BoxPolar
	b3InitNOP();    // This is Custom
}

void b3Shape::b3AllocVertices()
{
#ifdef BLZ3_USE_OPENGL
	Vertices =  (GLfloat *)b3Alloc(VertexCount * 3 * sizeof(GLfloat));
	Grids    = (GLushort *)b3Alloc(GridCount   * 2 * sizeof(GLushort));
	Polygons = (GLushort *)b3Alloc(PolyCount   * 3 * sizeof(GLushort));
#endif
}

void b3Shape::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	if (Vertices != null)
	{
		b3Free(Vertices);
		Vertices = null;
	}
	if (Grids != null)
	{
		b3Free(Grids);
		Grids = null;
	}
	if (Polygons != null)
	{
		b3Free(Polygons);
		Polygons = null;
	}
#endif
	VertexCount = 0;
	GridCount   = 0;
	PolyCount   = 0;
}

void b3Shape::b3ComputeVertices()
{
}

void b3Shape::b3ComputeIndices()
{
}

void b3Shape::b3Intersect()
{
}

void b3Shape::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	
#endif
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

b3Shape3::b3Shape3(b3_u32 class_type) : b3Shape(sizeof(b3Shape3), class_type)
{
}

b3Shape3::b3Shape3(b3_u32 *src) : b3Shape(src)
{
	b3InitVector();  // This is Base[0]
	b3InitVector();  // This is Base[1]
	b3InitVector();  // This is Base[2]
	b3InitVector(&Base);
	b3InitVector(&Dir1);
	b3InitVector(&Dir2);
	b3InitVector(&Dir3);
}

b3SplineCurve::b3SplineCurve(b3_u32 class_type) : b3Shape(sizeof(b3SplineCurve), class_type)
{
}

b3SplineCurve::b3SplineCurve(b3_u32 *src) : b3Shape(src)
{
	rSubDiv = b3InitInt();
	for (b3_index i = 0;i < B3_MAX_KNOTS;i++) Knots[i] = b3InitFloat();
	b3InitSpline(&Spline,null,Knots);
	b3InitVector(&Axis.pos);
	b3InitVector(&Axis.dir);
}

b3SplineShape::b3SplineShape(b3_u32 class_type) : b3Shape(sizeof(b3SplineShape), class_type)
{
}

b3SplineShape::b3SplineShape(b3_u32 *src) : b3Shape(src)
{				 
	b3_index i;

	b3InitVector(&Axis.pos);
	b3InitVector(&Axis.dir);
	b3InitSpline(&Spline[0],null,Knots[0]);
	b3InitSpline(&Spline[1],null,Knots[1]);

	// FIX ME: Is the order right?
	for (i = 0;i < B3_MAX_KNOTS;i++) Knots[0][i] = b3InitFloat();
	for (i = 0;i < B3_MAX_KNOTS;i++) Knots[1][i] = b3InitFloat();
}

b3CSGShape3::b3CSGShape3(b3_u32 class_type) : b3Shape(sizeof(b3CSGShape3), class_type)
{
}

b3CSGShape3::b3CSGShape3(b3_u32 *src) : b3Shape(src)
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
