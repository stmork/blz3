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
**      Revision 1.3  2001/08/09 15:27:34  sm
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

void b3Torus::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	SinCosSteps = context->b3GetSubdiv();
	Cos         = context->b3GetCosTable();
	Sin         = context->b3GetSinTable();
	vertCount = (SinCosSteps + 2) * (SinCosSteps + 2);
}

void b3Torus::b3ComputeVertices()
{
	b3_vector *Vector;
	b3_f64     RadX,RadY,sx,sy,start,end,a;
	b3_f64     LocalSin[B3_MAX_RENDER_SUBDIV+1],LocalCos[B3_MAX_RENDER_SUBDIV+1];
	b3_index   i,j;
	b3_count   iMax,Circles=0;
	b3_vector  Aux;

	Vector = (b3_vector *)Vertices;
	start  = Limit.y1 * SinCosSteps;
	end    = Limit.y2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);
	if ((i - start) > Epsilon)	/* underflow */
	{
		LocalSin[Circles] = Limit.y1;
		Circles++;
	}
	a = 1.0 / SinCosSteps;
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
	xSize = 0;
	ySize = Circles;

	for (j=0;j<Circles;j++)		/* Calculate Values */
	{
		LocalCos[j] = cos(LocalSin[j] * M_PI * 2) * bRad;
		LocalSin[j] = sin(LocalSin[j] * M_PI * 2) * bRad;
	}

	start  = Limit.x1 * SinCosSteps;
	end    = Limit.x2 * SinCosSteps;
	i      = (b3_index)ceil(start);
	iMax   = (b3_count)floor(end);

	if ((i - start) > Epsilon)
	{
		a  = Limit.x1 * M_PI * 2;
		sx = cos(a);
		sy = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}

	for (;i<=iMax;i++)
	{
		sx    = Cos[i];
		sy    = Sin[i];
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
			Vector++;
		}
		xSize++;
	}

	if ((end - iMax) > Epsilon)
	{
		a     = Limit.x2 * M_PI * 2;
		sx    = cos(a);
		sy    = sin(a);
		Aux.x = Base.x + sx * aRad * Dir1.x + sy * aRad * Dir2.x;
		Aux.y = Base.y + sx * aRad * Dir1.y + sy * aRad * Dir2.y;
		Aux.z = Base.z + sx * aRad * Dir1.z + sy * aRad * Dir2.z;

		for (j=0;j<Circles;j++)
		{
			RadX = LocalCos[j];
			RadY = LocalSin[j];

			Vector->x = Aux.x + sx * RadX * Dir1.x + sy * RadX * Dir2.x + RadY * Dir3.x;
			Vector->y = Aux.y + sx * RadX * Dir1.y + sy * RadX * Dir2.y + RadY * Dir3.y;
			Vector->z = Aux.z + sx * RadX * Dir1.z + sy * RadX * Dir2.z + RadY * Dir3.z;
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

void b3Torus::b3ComputeIndices()
{
	GLushort *Index;
	b3_bool   EndLine = false,EndCol = false;
	b3_index  i,j,Number,s,ys,ye;
	b3_count  Heights,Widths,Overhead;
	b3_f64    y1,y2;

	b3ComputeBound(&Limit);
	Overhead = b3GetIndexOverhead (0.0,0.0);
	if (Overhead < 0)
	{
		EndLine  =  true;
		Overhead = -Overhead;
	}

	y1 = Limit.y1 * SinCosSteps;
	y2 = Limit.y2 * SinCosSteps;
	ys = (b3_index)ceil(y1);
	ye = (b3_index)floor(y2);
	Heights = ye - ys;
	if ((ys - y1) > Epsilon) Heights++;
	if ((y2 - ye) > Epsilon) Heights++;
	if ((ys > 0) || (ye < SinCosSteps))
	{
		EndCol = true;
	}

	Widths = Heights;
	if (EndCol) Widths++;

	Number = (Widths + Heights) * Overhead;
	if (EndLine) Number += Heights;

	GridCount = 0;
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

		for (j = 0;j < Widths;j++)
		{
			*Index++ = s + j;
			*Index++ = s + j + Heights + 1;
		}
		GridCount += Widths;

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

	/*
		PrintF ("\n");
		PrintF ("Heights:  %ld\n",Heights);
		PrintF ("Widths:   %ld\n",Widths);
		PrintF ("Number:   %ld\n",Number);
		PrintF ("Overhead: %ld\n",Overhead);
		PrintF ("n:        %ld\n",n);
	*/
}

void b3Torus::b3Intersect()
{
}
