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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.2  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
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

b3Sphere::b3Sphere(b3_u32 class_type) : b3Shape(sizeof(b3Sphere), class_type)
{
}

b3Sphere::b3Sphere(b3_u32 *src) : b3Shape(src)
{
	b3InitVector(&Base);
	b3InitVector(&Dir);
}

void b3Sphere::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount   = ((SinCosSteps >> 1) + 1) * (SinCosSteps + 1);
}

void b3Sphere::b3ComputeVertices()
{
}

void b3Sphere::b3ComputeIndices()
{
	b3_vector *Vector;
	b3_index   i,j;
	b3_count   Circles;
	b3_f64     cx,sx,cy,sy,a;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32     Rad;
	b3_vector  Aux,Dir1,Dir2,Dir3;

	Vector = (b3_vector *)Vertices;
	Aux    = Base;
	Dir1.x = Rad = b3Length (&Dir);
	Dir1.y = 0;
	Dir1.z = 0;
	Dir2.x = 0;
	Dir2.y = Rad;
	Dir2.z = 0;
	Dir3.x = 0;
	Dir3.y = 0;
	Dir3.z = Rad;
	Circles = (SinCosSteps >> 1) + 1;
	xSize	 = SinCosSteps + 1;
	ySize  = Circles;

	a = 2.0/SinCosSteps;
	for (j =0 ;j < Circles;j++)
	{
		LocalCos[j] = Rad = -cos(j * a * M_PI);
		LocalSin[j] = sqrt(1.0-Rad*Rad);
	}

	for (i = 0;i <= SinCosSteps;i++)
	{
		cx = Cos[i];
		sx = Sin[i];
		for (j=0;j<Circles;j++)
		{
			cy = LocalCos[j];
			sy = LocalSin[j];

			Vector->x = Base.x + cx * sy * Dir1.x + sx * sy * Dir2.x + cy * Dir3.x;
			Vector->y = Base.y + cx * sy * Dir1.y + sx * sy * Dir2.y + cy * Dir3.y;
			Vector->z = Base.z + cx * sy * Dir1.z + sx * sy * Dir2.z + cy * Dir3.z;
			Vector++;
		}
	}
	/*
		PrintF ("\n");
		PrintF ("Points: %3ld\n",Points);
		PrintF ("Circles:%3ld\n",Circles);
	*/
}

void b3Sphere::b3Intersect()
{
}
