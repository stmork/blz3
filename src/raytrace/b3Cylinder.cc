/*
**
**      $Filename:      b3Cylinder.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing cylinder
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
**      Revision 1.3  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.2  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
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

b3Cylinder::b3Cylinder(b3_u32 class_type) : b3Shape3(sizeof(b3Cylinder),class_type)
{
}

b3Cylinder::b3Cylinder(b3_u32 *src) : b3Shape3(src)
{
}

void b3Cylinder::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount   = SinCosSteps + SinCosSteps + 6;
}

void b3Cylinder::b3AllocVertices(b3RenderContext *context)
{
	b3RenderObject::b3AllocVertices(context);
	Grids = context->b3GetCylinderIndices();
}

void b3Cylinder::b3ComputeVertices()
{
	b3_vector *Vector;
	b3_f64     sx,sy,b,h,start,end;
	b3_index   i;
	b3_count   iMax;
	b3_vector  Bottom;

	Vector				= (b3_vector *)Vertices;

	h = Limit.y2 - Limit.y1;
	b = Limit.y1;

	Bottom.x = Base.x + b * Dir3.x;
	Bottom.y = Base.y + b * Dir3.y;
	Bottom.z = Base.z + b * Dir3.z;
	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = 2;

	if ((i - start) > Epsilon)
	{
		b = Limit.x1 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx = Cos[i % SinCosSteps];
		sy = Sin[i % SinCosSteps];

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
		Vector++;
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		b = Limit.x2 * M_PI * 2;
		sx = cos(b);
		sy = sin(b);

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
		Vector++;

		Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
		Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
		Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;

		xSize++;
	}
}

void b3Cylinder::b3ComputeIndices()
{
	b3_count Overhead;

	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		GridCount = 1;
		Overhead = -Overhead;
	}
	else
	{
		GridCount = 0;
	}
	GridCount += Overhead * 3;
}

void b3Cylinder::b3Intersect()
{
}
