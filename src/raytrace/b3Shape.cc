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
**      Revision 1.1  2001/07/01 12:24:59  sm
**      Initial revision
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
	b3Item::b3Register(&b3Shape1::b3Init,        &b3Shape1::b3Init,        SPHERE);
	b3Item::b3Register(&b3Shape2::b3Init,        &b3Shape2::b3Init,        AREA);
	b3Item::b3Register(&b3Shape2::b3Init,        &b3Shape2::b3Init,        DISK);
	b3Item::b3Register(&b3Shape3::b3Init,        &b3Shape3::b3Init,        CYLINDER);
	b3Item::b3Register(&b3Shape3::b3Init,        &b3Shape3::b3Init,        CONE);
	b3Item::b3Register(&b3Shape3::b3Init,        &b3Shape3::b3Init,        ELLIPSOID);
	b3Item::b3Register(&b3Shape3::b3Init,        &b3Shape3::b3Init,        BOX);
	b3Item::b3Register(&b3Shape4::b3Init,        &b3Shape4::b3Init,        TORUS);
	b3Item::b3Register(&b3TriangleShape::b3Init, &b3TriangleShape::b3Init, TRIANGLES);
	b3Item::b3Register(&b3SplineCurve::b3Init,   &b3SplineCurve::b3Init,   SPLINE);
	b3Item::b3Register(&b3SplineCurve::b3Init,   &b3SplineCurve::b3Init,   SPLINE_ROT);
	b3Item::b3Register(&b3SplineShape::b3Init,   &b3SplineShape::b3Init,   SPLINES_AREA);
	b3Item::b3Register(&b3SplineShape::b3Init,   &b3SplineShape::b3Init,   SPLINES_CYL);
	b3Item::b3Register(&b3SplineShape::b3Init,   &b3SplineShape::b3Init,   SPLINES_RING);
	b3Item::b3Register(&b3Shape::b3Init,         &b3Shape::b3Init,         SHUTUP);
	b3Item::b3Register(&b3CSGShape1::b3Init,     &b3CSGShape1::b3Init,     CSG_SPHERE);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_CYLINDER);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_CONE);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_ELLIPSOID);
	b3Item::b3Register(&b3CSGShape3::b3Init,     &b3CSGShape3::b3Init,     CSG_BOX);
	b3Item::b3Register(&b3CSGShape4::b3Init,     &b3CSGShape4::b3Init,     CSG_TORUS);
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

b3Shape1::b3Shape1(b3_u32 class_type) : b3Shape(sizeof(b3Shape1), class_type)
{
}

b3Shape1::b3Shape1(b3_u32 *src) : b3Shape(src)
{
	b3InitVector(&Base);
	b3InitVector(&Dir);
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

b3Shape4::b3Shape4(b3_u32 class_type) : b3Shape(sizeof(b3Shape4), class_type)
{
}

b3Shape4::b3Shape4(b3_u32 *src) : b3Shape(src)
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
	aRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir1 and Dir2
	bRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir3
}

b3TriangleShape::b3TriangleShape(b3_u32 class_type) : b3Shape(sizeof(b3TriangleShape), class_type)
{
}

b3TriangleShape::b3TriangleShape(b3_u32 *src) : b3Shape(src)
{
	b3InitNOP();
	b3InitVector(&Base);
	b3InitVector(&Size);
	GridSize  = b3InitInt();
	Triangles = b3InitInt();
	Vertices  = b3InitInt();
	xSize     = b3InitInt();
	ySize     = b3InitInt();

	// FIX ME: We have to convert the indices and vertices to
	//         and have to initialize the grid!
}

b3SplineCurve::b3SplineCurve(b3_u32 class_type) : b3Shape(sizeof(b3SplineCurve), class_type)
{
}

b3SplineCurve::b3SplineCurve(b3_u32 *src) : b3Shape(src)
{
	rSubDiv = b3InitInt();
	for (b3_index i;i < B3_MAX_KNOTS;i++) Knots[i] = b3InitFloat();
	b3InitSpline(&Spline,null,Knots);
	b3InitVector(&Axis.pos);
	b3InitVector(&Axis.dir);
}

b3SplineShape::b3SplineShape(b3_u32 class_type) : b3Shape(sizeof(b3SplineShape), class_type)
{
}

b3SplineShape::b3SplineShape(b3_u32 *src) : b3Shape(src)
{
	b3InitVector(&Axis.pos);
	b3InitVector(&Axis.dir);
	b3InitSpline(&Spline[0],null,Knots[0]);
	b3InitSpline(&Spline[1],null,Knots[1]);

	// FIX ME: Is the order right?
	for (b3_index i;i < B3_MAX_KNOTS;i++) Knots[0][i] = b3InitFloat();
	for (b3_index i;i < B3_MAX_KNOTS;i++) Knots[1][i] = b3InitFloat();
}

b3CSGShape1::b3CSGShape1(b3_u32 class_type) : b3Shape(sizeof(b3CSGShape1), class_type)
{
}

b3CSGShape1::b3CSGShape1(b3_u32 *src) : b3Shape(src)
{
	b3InitVector(&Base);
	b3InitVector(&Dir);

	b3InitFloat(); // This is QuadRadius
	b3InitInt();   // This is Index

	Operation = b3InitInt();
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

b3CSGShape4::b3CSGShape4(b3_u32 class_type) : b3Shape(sizeof(b3CSGShape4), class_type)
{
}

b3CSGShape4::b3CSGShape4(b3_u32 *src) : b3Shape(src)
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

	aRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir1 and Dir2
	bRad = b3InitFloat(); // Is this a real saved value? it could be computed from Dir3

	b3InitFloat(); // This is aQuad
	b3InitFloat(); // This is bQuad
	b3InitInt();   // This Index

	Operation = b3InitInt();
}
