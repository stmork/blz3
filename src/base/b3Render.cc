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

#include "blz3/base/b3Render.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Spline.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.5  2001/08/12 19:47:47  sm
**      - Now having correct orthogonal projection incl. aspect ratio
**
**      Revision 1.4  2001/08/11 20:17:22  sm
**      - Updated OpenGL on Un*x platform.
**      - This was a great day!
**
**      Revision 1.3  2001/08/11 19:59:15  sm
**      - Added orthogonal projection
**
**      Revision 1.2  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.1  2001/08/11 15:59:58  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.3  2001/08/10 15:14:36  sm
**      - Now having all shapes implemented for drawing lines.
**
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

b3RenderContext::b3RenderContext()
{
	b3PrintF(B3LOG_DEBUG,"OpenGL vendor:     %s\n",glGetString(GL_VENDOR));
	b3PrintF(B3LOG_DEBUG,"OpenGL renderer:   %s\n",glGetString(GL_RENDERER));
	b3PrintF(B3LOG_DEBUG,"OpenGL version:    %s\n",glGetString(GL_VERSION));
	b3PrintF(B3LOG_DEBUG,"OpenGL extensions: %s\n",glGetString(GL_EXTENSIONS));
}

void b3RenderContext::b3StartDrawing()
{
	glClearColor(0.8f,0.8f,0.8f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
}

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderObject::b3RenderObject()
{
	VertexCount = 0;
	GridCount   = 0;
	PolyCount   = 0;

#ifdef BLZ3_USE_OPENGL
	glVertices = null;
	glNormals  = null;
	glGrids    = null;
	glPolygons = null;
	glComputed = false;
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
			if (b3Free(glVertices))
			{
				glNormals = null;
			}
			if (b3Free(glNormals))
			{
				glNormals = null;
			}
		}
		VertexCount = new_vertCount;

		if (VertexCount > 0)
		{
			glVertices =  (GLfloat *)b3Alloc(VertexCount * 3 * sizeof(GLfloat));
			glNormals  =  (GLfloat *)b3Alloc(VertexCount * 3 * sizeof(GLfloat));
		}
		glComputed = false;
	}

	if (GridCount != new_gridCount)
	{
		if (GridCount > 0)
		{
			if (b3Free(glGrids))
			{
				glGrids = null;
			}
		}
		GridCount = new_gridCount;

		if (GridCount > 0)
		{
			glGrids    = (GLushort *)b3Alloc(GridCount   * 2 * sizeof(GLushort));
		}
		glComputed = false;
	}

	if (PolyCount != new_polyCount)
	{
		if (PolyCount > 0)
		{
			if (b3Free(glPolygons))
			{
				glPolygons = null;
			}
		}
		PolyCount = new_polyCount;

		if (PolyCount > 0)
		{
			glPolygons = (GLushort *)b3Alloc(PolyCount   * 3 * sizeof(GLushort));
		}
		glComputed = false;
	}
#endif
}

void b3RenderObject::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	if (glVertices != null)
	{
		b3Free(glVertices);
		glVertices = null;
	}
	if (glNormals != null)
	{
		b3Free(glNormals);
		glNormals = null;
	}
	if (glGrids != null)
	{
		b3Free(glGrids);
		glGrids = null;
	}
	if (glPolygons != null)
	{
		b3Free(glPolygons);
		glPolygons = null;
	}
#endif
	VertexCount = 0;
	GridCount   = 0;
	PolyCount   = 0;
}

void b3RenderObject::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	if (!glComputed)
	{
			b3ComputeIndices();
			b3ComputeVertices();
			glComputed = true;
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

			a = glGrids[i + i]     * 3;
			aPoint.x = glVertices[a++];
			aPoint.y = glVertices[a++];
			aPoint.z = glVertices[a++];

			b = glGrids[i + i + 1] * 3;
			bPoint.x = glVertices[b++];
			bPoint.y = glVertices[b++];
			bPoint.z = glVertices[b++];

			len = b3Distance(&aPoint,&bPoint);
		}
#endif
		glVertexPointer(3, GL_FLOAT, 0, glVertices);
		glDrawElements(GL_LINES,GridCount * 2,GL_UNSIGNED_SHORT,glGrids);
	}
#endif
}

void b3RenderObject::b3ComputeVertices()
{
}

void b3RenderObject::b3ComputeIndices()
{
}

b3_bool b3RenderObject::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3_vector *ptr;
	b3_bool    result = false;
	b3_index   i;

#ifdef BLZ3_USE_OPENGL
	if (glComputed && (glVertices != null) && (VertexCount > 0))
	{
		ptr = (b3_vector *)glVertices;
		for (i = 0;i < VertexCount;i++)
		{
			// Check lower bound
			if (ptr[i].x < lower->x) lower->x = ptr[i].x;
			if (ptr[i].y < lower->y) lower->y = ptr[i].y;
			if (ptr[i].z < lower->z) lower->z = ptr[i].z;

			// Check upper bound
			if (ptr[i].x > upper->x) upper->x = ptr[i].x;
			if (ptr[i].y > upper->y) upper->y = ptr[i].y;
			if (ptr[i].z > upper->z) upper->z = ptr[i].z;
		}
		result = true;
	}
#endif
	return result;
}
