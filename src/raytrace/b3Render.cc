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
#include "blz3/base/b3Spline.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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
		aux  = i * M_PI * 2 / subdiv;
		Sin[i] = sin(aux);
		Cos[i] = cos(aux);
	}

	if (Between == null)
	{
		Between = (b3_vector *)b3Alloc(
			B3_MAX_CONTROLS * B3_MAX_CONTROLS * sizeof(b3_vector));
	}

#ifdef BLZ3_USE_OPENGL
	GLushort *ptr;
	b3_index  a;

	b3Free(CylinderIndices);
	b3Free(ConeIndices);

	CylinderIndices = (GLushort *)b3Alloc
			((subdiv + 1) * 3 * 2 * sizeof(GLushort));
	if (CylinderIndices != null)
	{
		ptr = CylinderIndices;
		a = 0;
		for (i = 0;i <= subdiv;i++)
		{
			*ptr++ = a;
			*ptr++ = a + 1;

			*ptr++ = a;
			*ptr++ = a + 2;

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
	b3_f64 xl,
	b3_f64 yl)
{
	b3_count Overhead;
	b3_index xs,xe;
	b3_f64   x1,x2;

	if  (Limit.x1 < xl) Limit.x1 = xl;
	if  (Limit.y1 < yl) Limit.y1 = yl;
	x1 = Limit.x1 * SinCosSteps;
	x2 = Limit.x2 * SinCosSteps;
	xs = (b3_index)ceil(x1);
	xe = (b3_index)floor(x2);
	Overhead = xe - xs;
	if ((xs - x1) > Epsilon) Overhead++;
	if ((x2 - xe) > Epsilon) Overhead++;
	if ((xs > 0)||(xe < SinCosSteps))
	{
		return (-Overhead);
	}

	return Overhead;
}

void b3RenderObject::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	if (!Computed)
	{
			b3ComputeVertices();
			b3ComputeIndices();
			Computed = true;
	}
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glDrawElements(GL_LINES,GridCount * 2,GL_UNSIGNED_SHORT,Grids);
#endif
}

void b3RenderObject::b3ComputeVertices()
{
}

void b3RenderObject::b3ComputeIndices()
{
}
