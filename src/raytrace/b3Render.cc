/*
**
**      $Filename:      b3Render.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Rendering some objects
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

#include "blz3/raytrace/b3Render.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Spline.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.2  2001/08/09 15:27:34  sm
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
**      Revision 1.1  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderContext::b3RenderContext(b3_count new_subdiv)
{
	CylinderIndices = null;
	ConeIndices     = null;
	Between         = null;
	b3InitSubdiv(new_subdiv);
}

void b3RenderContext::b3InitSubdiv(b3_count new_subdiv)
{
	b3_f64   aux;
	b3_index i;

	subdiv = (new_subdiv > B3_MAX_RENDER_SUBDIV ? B3_MAX_RENDER_SUBDIV : new_subdiv);
	if (subdiv < 8)
	{
		subdiv = 8;
	}

	for (i = 0;i <= subdiv;i++)
	{
		aux    = i * M_PI * 2 / subdiv;
		Sin[i] = sin(aux);
		Cos[i] = cos(aux);
	}

#ifdef BLZ3_USE_OPENGL
	GLushort *ptr;
	b3_index  a;

	if (CylinderIndices != null)
	{
		b3Free(CylinderIndices);
	}
	if (ConeIndices != null)
	{
		b3Free(ConeIndices);
	}

	CylinderIndices = (GLushort *)b3Alloc
			((subdiv + 1) * 3 * 2 * sizeof(GLushort));
	if (CylinderIndices != null)
	{
		ptr = CylinderIndices;
		a = 0;
		for (i = 0;i <= subdiv;i++)
		{
			// Conactinate vertices in this ascending order:
			//
			//   (c)
			// 1-----3
			// |
			// |(a)
			// |
			// 0-----2
			//   (b)

			// (a)
			*ptr++ = a;
			*ptr++ = a + 1;

			// (b)
			*ptr++ = a;
			*ptr++ = a + 2;

			// (c)
			*ptr++ = a + 1;
			*ptr++ = a + 3;
			a += 2;
		}
	}

	ConeIndices = (GLushort *)b3Alloc
		((subdiv + 1) * 2 * 2 * sizeof(GLushort));
	if (ConeIndices != null)
	{
		ptr = ConeIndices;
		for (i = 0;i <= subdiv;i++)
		{
			*ptr++ = 0;
			*ptr++ = i+1;

			*ptr++ = i+1;
			*ptr++ = i+2;
		}
	}
#endif
}

b3_count b3RenderContext::b3GetSubdiv()
{
	return subdiv;
}

b3_f64 *b3RenderContext::b3GetSinTable()
{
	return Sin;
}

b3_f64 *b3RenderContext::b3GetCosTable()
{
	return Cos;
}

b3_vector *b3RenderContext::b3GetSplineAux()
{
	if (Between == null)
	{
		Between = (b3_vector *)b3Alloc(
			B3_MAX_CONTROLS * B3_MAX_CONTROLS * sizeof(b3_vector));
	}
	return Between;
}

#ifdef BLZ3_USE_OPENGL
GLushort *b3RenderContext::b3GetCylinderIndices()
{
	return CylinderIndices;
}

GLushort *b3RenderContext::b3GetConeIndices()
{
	return ConeIndices;
}
#endif

b3RenderObject::b3RenderObject()
{
	VertexCount = 0;
	GridCount   = 0;
	PolyCount   = 0;
	Epsilon     = 0.001;
	Between     = null;

#ifdef BLZ3_USE_OPENGL
	Vertices = null;
	Normals  = null;
	Grids    = null;
	Polygons = null;
	Computed = false;
#endif
}

b3RenderObject::~b3RenderObject()
{
}

void b3RenderObject::b3GetCount(
	b3RenderContext *context,
	b3_count        &vertCount,
	b3_count        &gridCount,
	b3_count        &polyCount)
{
	vertCount = 0;
	gridCount = 0;
	polyCount = 0;
}

void b3RenderObject::b3AllocVertices(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	b3_count new_vertCount = 0;
	b3_count new_gridCount = 0;
	b3_count new_polyCount = 0;

	b3GetCount(context,new_vertCount,new_gridCount,new_polyCount);

	if (VertexCount != new_vertCount)
	{
		if (VertexCount > 0)
		{
			if (b3Free(Vertices))
			{
				Normals = null;
			}
			if (b3Free(Normals))
			{
				Normals = null;
			}
		}
		VertexCount = new_vertCount;

		if (VertexCount > 0)
		{
			Vertices =  (GLfloat *)b3Alloc(VertexCount * 3 * sizeof(GLfloat));
			Normals  =  (GLfloat *)b3Alloc(VertexCount * 3 * sizeof(GLfloat));
		}
		Computed = false;
	}

	if (GridCount != new_gridCount)
	{
		if (GridCount > 0)
		{
			if (b3Free(Grids))
			{
				Grids = null;
			}
		}
		GridCount = new_gridCount;

		if (GridCount > 0)
		{
			Grids    = (GLushort *)b3Alloc(GridCount   * 2 * sizeof(GLushort));
		}
		Computed = false;
	}

	if (PolyCount != new_polyCount)
	{
		if (PolyCount > 0)
		{
			if (b3Free(Polygons))
			{
				Polygons = null;
			}
		}
		PolyCount = new_polyCount;

		if (PolyCount > 0)
		{
			Polygons = (GLushort *)b3Alloc(PolyCount   * 3 * sizeof(GLushort));
		}
		Computed = false;
	}
#endif
}

void b3RenderObject::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	if (Vertices != null)
	{
		b3Free(Vertices);
		Vertices = null;
	}
	if (Normals != null)
	{
		b3Free(Normals);
		Normals = null;
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

b3_count b3RenderObject::b3GetIndexOverhead (
	b3_f64 xLeft,
	b3_f64 yLeft)
{
	b3_count Overhead;
	b3_index xs,xe;
	b3_f64   x1,x2;

	if  (Limit.x1 < xLeft) Limit.x1 = xLeft;
	if  (Limit.y1 < yLeft) Limit.y1 = yLeft;
	x1 = Limit.x1 * SinCosSteps;
	x2 = Limit.x2 * SinCosSteps;
	xs = (b3_index)ceil(x1);
	xe = (b3_index)floor(x2);
	Overhead = xe - xs;
	if ((xs - x1) > Epsilon) Overhead++;
	if ((x2 - xe) > Epsilon) Overhead++;

	return ((xs > 0)||(xe < SinCosSteps)) ? -Overhead : Overhead;
}

void b3RenderObject::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	if (!Computed)
	{
			b3ComputeIndices();
			b3ComputeVertices();
			Computed = true;
	}

	if (GridCount > 0)
	{
#ifdef _DEBUG
		// This loop collects access vialoations
		// prior calling OpenGL routines. This
		// makes it possible to catch to faulty
		// index data. The access simply compute
		// the length of the lines to be drawn.
		for (b3_index i = 0;i < GridCount;i++)
		{
			b3_vector aPoint,bPoint;
			b3_index  a,b;
			b3_f64    len;

			a = Grids[i + i]     * 3;
			aPoint.x = Vertices[a++];
			aPoint.y = Vertices[a++];
			aPoint.z = Vertices[a++];

			b = Grids[i + i + 1] * 3;
			bPoint.x = Vertices[b++];
			bPoint.y = Vertices[b++];
			bPoint.z = Vertices[b++];

			len = b3Distance(&aPoint,&bPoint);
		}
#endif
		glVertexPointer(3, GL_FLOAT, 0, Vertices);
		glDrawElements(GL_LINES,GridCount * 2,GL_UNSIGNED_SHORT,Grids);
	}
#endif
}

void b3RenderObject::b3ComputeVertices()
{
}

void b3RenderObject::b3ComputeIndices()
{
}
