/*
**
**      $Filename:      b3Ellipsoid.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing ellipsoid
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
#include <assert.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.4  2001/08/09 15:27:34  sm
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

b3Ellipsoid::b3Ellipsoid(b3_u32 class_type) : b3Shape3(sizeof(b3Ellipsoid),class_type)
{
}

b3Ellipsoid::b3Ellipsoid(b3_u32 *src) : b3Shape3(src)
{
}

void b3Ellipsoid::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount   = (SinCosSteps + 2) * ((SinCosSteps >> 1) + 1);
}

void b3Ellipsoid::b3ComputeVertices()
{
	b3_vector *Vector;
	b3_index   i,j;
	b3_count   iMax,Circles = 0;
	b3_f64     RadX,RadY,sx,sy;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_f32     start,end,a;

	Vector = (b3_vector *)Vertices;
	start  = (Limit.y1 + 1) * SinCosSteps * 0.25;
	end    = (Limit.y2 + 1) * SinCosSteps * 0.25;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > Epsilon)	/* underflow */
	{
		LocalSin[Circles] = Limit.y1;
		Circles++;
	}
	a = 4.0 / SinCosSteps;
	for (j=i;j<=iMax;j++)		/* integers */
	{
		LocalSin[Circles] = j * a - 1;
		Circles++;
	}
	if ((end - iMax) > Epsilon)	/* Overflow */
	{
		LocalSin[Circles] = Limit.y2;
		Circles++;
	}

	for (j=0;j<Circles;j++)		/* Calculate Values */
	{
		LocalSin[j] = a = sin(LocalSin[j] * M_PI * 0.5);
		LocalCos[j] = sqrt(1.0-a*a);
	}

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	xSize = 0;
	ySize = Circles;

	if ((i - start) > Epsilon)
	{
		a  = Limit.x1 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx = Cos[i];
		sy = Sin[i];

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		a  = Limit.x2 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Base.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Base.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Base.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}
	/*
		PrintF ("\n");
		PrintF ("Points: %3ld\n",Points);
		PrintF ("Circles:%3ld\n",Circles);
	*/
}

void b3Ellipsoid::b3ComputeIndices()
{
	GLushort *Index;
	b3_bool   EndLine = false;
	b3_index  i,j,Number,s,ys,ye;
	b3_count  Heights,Widths,Overhead;
	b3_f64    y1,y2;

	GridCount = 0;
	b3ComputeBound(&Limit);
	Overhead  = b3GetIndexOverhead (0.0,-1.0);
	if (Overhead < 0)
	{
		EndLine  = true;
		Overhead = -Overhead;
	}

	y1 = (Limit.y1 + 1) * SinCosSteps * 0.25;
	y2 = (Limit.y2 + 1) * SinCosSteps * 0.25;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	Heights = ye - ys;
	if ((ys - y1) > Epsilon) Heights++;
	if ((y2 - ye) > Epsilon) Heights++;

	Widths = Heights - 1;
	if ((SinCosSteps * 0.5 - y2) > Epsilon) Widths++;
	if (                     y1  > Epsilon) Widths++;

	if (EndLine) Number = (Widths + Heights) * Overhead + Heights;
	else         Number = (Widths + Heights) * Overhead;
	Grids = Index = (GLushort *)b3RenderObject::b3Alloc
		(Number * 2 * sizeof(GLushort));
	if (Index == null)
	{
		return;
	}

	s = 0;
	for (i = 0;i < Overhead;i++)
	{
		for (j = 0;j < Heights;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + 1;
		}
		GridCount += Heights;

		if (y1 <= Epsilon) j = 1;
		else               j = 0;
		if ((SinCosSteps * 0.5 - y2) <= Epsilon) while (j < Heights)
		{
			*Index++ = s + j;
			*Index++ = s + j + Heights + 1;
			GridCount++;
			j++;
		}
		else  while (j <= Heights)
		{
			*Index++ = s + j;
			*Index++ = s + j + Heights + 1;
			GridCount++;
			j++;
		}
		s += (Heights + 1);
	}

	if (EndLine)
	{
		for (j = 0;j < Heights;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + 1;
		}
		GridCount += Heights;
	}
	assert(GridCount <= Number);
	/*
		PrintF ("\n");
		PrintF ("Heights:  %ld\n",Heights);
		PrintF ("Widths:   %ld\n",Widths);
		PrintF ("Number:   %ld\n",Number);
		PrintF ("Overhead: %ld\n",Overhead);
		PrintF ("n:        %ld\n",n);
	*/
}

void b3Ellipsoid::b3Intersect()
{
}
