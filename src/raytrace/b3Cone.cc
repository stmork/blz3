/*
**
**      $Filename:      b3Cone.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing cone
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

b3Cone::b3Cone(b3_u32 class_type) : b3Shape3(sizeof(b3Cone),class_type)
{
}

b3Cone::b3Cone(b3_u32 *src) : b3Shape3(src)
{
}

void b3Cone::b3GetCount(
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

void b3Cone::b3ComputeVertices()
{
	b3_vector *Vector;
	b3_f64     sx,sy,b,a,h,start,end;
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
	ySize = 1;

	if (Limit.y2 < 1)
	{
		ySize++;
		if ((i - start) > Epsilon)
		{
			a = Limit.x1 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = Bottom.x + (1-b) * sx * Dir1.x + (1-b) * sy * Dir2.x;
			Vector->y = Bottom.y + (1-b) * sx * Dir1.y + (1-b) * sy * Dir2.y;
			Vector->z = Bottom.z + (1-b) * sx * Dir1.z + (1-b) * sy * Dir2.z;
			Vector++;

			Vector->x = Bottom.x + (1-h) * sx * Dir1.x + (1-h) * sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + (1-h) * sx * Dir1.y + (1-h) * sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + (1-h) * sx * Dir1.z + (1-h) * sy * Dir2.z + h * Dir3.z;
			Vector++;
			xSize++;
		}

		for (;i<=iMax;i++)
		{
			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;

			sx = (1-h) * Cos[i % SinCosSteps];
			sy = (1-h) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z + h * Dir3.z;
			Vector++;
			xSize++;
		}

		if ((end - iMax) > Epsilon)
		{
			a  = Limit.x2 * M_PI * 2;
			sx = cos(a);
			sy = sin(a);

			Vector->x = Bottom.x + (1-b) * sx * Dir1.x + (1-b) * sy * Dir2.x;
			Vector->y = Bottom.y + (1-b) * sx * Dir1.y + (1-b) * sy * Dir2.y;
			Vector->z = Bottom.z + (1-b) * sx * Dir1.z + (1-b) * sy * Dir2.z;
			Vector++;

			Vector->x = Bottom.x + (1-h) * sx * Dir1.x + (1-h) * sy * Dir2.x + h * Dir3.x;
			Vector->y = Bottom.y + (1-h) * sx * Dir1.y + (1-h) * sy * Dir2.y + h * Dir3.y;
			Vector->z = Bottom.z + (1-h) * sx * Dir1.z + (1-h) * sy * Dir2.z + h * Dir3.z;

			xSize++;
		}
	}
	else
	{
		Vector->x = Bottom.x +  Dir3.x;
		Vector->y = Bottom.y +  Dir3.y;
		Vector->z = Bottom.z +  Dir3.z;
		Vector++;

		if ((i - start) > Epsilon)
		{
			a  = Limit.x1 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;
			xSize++;
		}

		for (;i<=iMax;i++)
		{
			sx = (1-b) * Cos[i % SinCosSteps];
			sy = (1-b) * Sin[i % SinCosSteps];
			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;
			Vector++;
			xSize++;
		}

		if ((end - iMax) > Epsilon)
		{
			a = Limit.x2 * M_PI * 2;
			sx = (1-b) * cos(a);
			sy = (1-b) * sin(a);

			Vector->x = Bottom.x + sx * Dir1.x + sy * Dir2.x;
			Vector->y = Bottom.y + sx * Dir1.y + sy * Dir2.y;
			Vector->z = Bottom.z + sx * Dir1.z + sy * Dir2.z;

			xSize++;
		}
	}
}

void b3Cone::b3ComputeIndices()
{
}

void b3Cone::b3Intersect()
{
}
