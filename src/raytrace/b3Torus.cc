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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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

b3Torus::b3Torus(b3_u32 class_type) : b3Shape(sizeof(b3Torus), class_type)
{
}

b3Torus::b3Torus(b3_u32 *src) : b3Shape(src)
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

void b3Torus::b3ComputeVertices()
{
}

void b3Torus::b3ComputeIndices()
{
}

void b3Torus::b3Intersect()
{
}
