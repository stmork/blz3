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

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.10  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.9  2001/08/16 04:28:29  sm
**      - Solving conflicts
**
**      Revision 1.8  2001/08/15 19:52:57  sm
**      - First polygon rendering with Blizzard III (areas only)
**
**      Revision 1.7  2001/08/14 19:07:43  sm
**      - Minor changes on resources.
**
**      Revision 1.6  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
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

static GLfloat ambient[] =
{
	0.25f,0.25f,0.25f,1.0f
};

static GLfloat diffuse[] =
{
	0.8f,0.8f,0.8f,1.0f
};

static GLfloat light0[] =
{
	1000.0f,-2500.0f,2000.0f,1.0f
};

b3RenderContext::b3RenderContext()
{
#ifdef BLZ3_USE_OPENGL
	b3PrintF(B3LOG_DEBUG,"OpenGL vendor:     %s\n",glGetString(GL_VENDOR));
	b3PrintF(B3LOG_DEBUG,"OpenGL renderer:   %s\n",glGetString(GL_RENDERER));
	b3PrintF(B3LOG_DEBUG,"OpenGL version:    %s\n",glGetString(GL_VERSION));
	b3PrintF(B3LOG_DEBUG,"OpenGL extensions: %s\n",glGetString(GL_EXTENSIONS));
#endif
}

void b3RenderContext::b3StartDrawing()
{
#ifdef BLZ3_USE_OPENGL
	glClearColor(0.9f,0.9f,0.9f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	// Enable light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,light0);

	// Some material settings
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
#endif
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
	glSolid    = false;
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

void b3RenderObject::b3ComputeVertices()
{
}

void b3RenderObject::b3ComputeIndices()
{
}

void b3RenderObject::b3ComputeNormals(b3_bool normalize)
{
	b3_vector *nPtr = (b3_vector *)glNormals;
	b3_vector *vPtr = (b3_vector *)glVertices;
	GLushort  *pPtr = glPolygons;
	b3_vector  normal;
	b3_vector  xDir,yDir;
	b3_f64     len;
	b3_index   i,p1,p2,p3;

	if (nPtr == null)
	{
		// Do nothing!
		return;
	}

	// Clear normals
	for (i = 0;i < VertexCount;i++)
	{
		nPtr[i].x = 0;
		nPtr[i].y = 0;
		nPtr[i].z = 0;
	}

	// Collect normals
	for (i = 0;i < PolyCount;i++)
	{
		p1 = *pPtr++;
		p2 = *pPtr++;
		p3 = *pPtr++;

		// Do some semantic checks
		B3_ASSERT(p1 < VertexCount);
		B3_ASSERT(p2 < VertexCount);
		B3_ASSERT(p3 < VertexCount);

		xDir.x = vPtr[p2].x - vPtr[p1].x;
		xDir.y = vPtr[p2].y - vPtr[p1].y;
		xDir.z = vPtr[p2].z - vPtr[p1].z;

		yDir.x = vPtr[p3].x - vPtr[p1].x;
		yDir.y = vPtr[p3].y - vPtr[p1].y;
		yDir.z = vPtr[p3].z - vPtr[p1].z;

		normal.x = xDir.y * yDir.z - xDir.z * yDir.y;
		normal.y = xDir.z * yDir.x - xDir.x * yDir.z;
		normal.z = xDir.x * yDir.y - xDir.y * yDir.x;
		len = b3Length(&normal);
		if (len > 0)
		{
			normal.x /= len;
			normal.y /= len;
			normal.z /= len;

			nPtr[p1].x += normal.x;
			nPtr[p1].y += normal.y;
			nPtr[p1].z += normal.z;

			nPtr[p2].x += normal.x;
			nPtr[p2].y += normal.y;
			nPtr[p2].z += normal.z;

			nPtr[p3].x += normal.x;
			nPtr[p3].y += normal.y;
			nPtr[p3].z += normal.z;
		}
	}

	// Normalize
	if (normalize)
	{
		for (i = 0;i < VertexCount;i++)
		{
			len = b3Length(&nPtr[i]);
			if (len > 0)
			{
				nPtr[i].x /= len;
				nPtr[i].y /= len;
				nPtr[i].z /= len;
			}
		}
	}
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

void b3RenderObject::b3GetDiffuseColor(b3_color *color)
{
	color->r = 1.0f;
	color->g = 0.1f;
	color->b = 0.4f;
	color->a = 0.0f;
}

void b3RenderObject::b3Draw()
{
#ifdef BLZ3_USE_OPENGL
	if (!glComputed)
	{
			b3ComputeIndices();
			b3ComputeVertices();
			b3ComputeNormals();
			glComputed = true;
	}

	if (GridCount > 0)
	{
		b3_color color;

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
		b3GetDiffuseColor(&color);
		glVertexPointer(3, GL_FLOAT, 0, glVertices);
		glNormalPointer(GL_FLOAT, 0, glNormals);
		if (glSolid)
		{
#ifdef VERBOSE
			b3PrintF(B3LOG_FULL,"---------- OpenGL start solid drawing\n");
#endif
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(color.r,color.g,color.b);
			glDrawElements(GL_TRIANGLES, PolyCount * 3,GL_UNSIGNED_SHORT,glPolygons);
#ifdef VERBOSE
			b3PrintF(B3LOG_FULL,"---------- OpenGL stop solid drawing\n");
#endif
		}
		else
		{
#ifdef VERBOSE
			b3PrintF(B3LOG_FULL,"---------- OpenGL start line drawing\n");
#endif
			glDisable(GL_COLOR_MATERIAL);
			glColor3f(0.2f,0.2f,0.2f);
			glDrawElements(GL_LINES,GridCount * 2,GL_UNSIGNED_SHORT,glGrids);
#ifdef VERBOSE
			b3PrintF(B3LOG_FULL,"---------- OpenGL stop line drawing\n");
#endif
		}
	}
#endif
}
