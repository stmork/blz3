/*
**
**      $Filename:      b3Render.cc $
**      $Release:       Dortmund 2001, 2002 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Rendering some objects
**
**      (C) Copyright 2001, 2002  Steffen A. Mork
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
**      Revision 1.27  2002/07/20 10:49:34  sm
**      - Added custom light support (not finished yet)
**      - Added b3Light::b3IsActive() for compatibility.
**      - Added texture search path support like in brt3.
**
**      Revision 1.26  2002/02/17 21:58:11  sm
**      - Done UnCR
**      - Modified makefiles
**
**      Revision 1.25  2002/01/30 19:46:41  sm
**      - Trying to print in debug mode (and want to see anything)
**
**      Revision 1.24  2002/01/21 16:56:46  sm
**      - Showing splash dialog only in release version.
**      - Prepared shape icons.
**
**      Revision 1.23  2002/01/20 12:48:51  sm
**      - Added splash screen
**      - Corrected repeat buttons (capture change)
**
**      Revision 1.22  2002/01/03 19:07:27  sm
**      - Cleaned up cut/paste
**
**      Revision 1.21  2002/01/01 13:50:21  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.20  2001/11/09 16:15:35  sm
**      - Image file encoder
**      - Performance meter for triangles / second added.
**      - Corrected Windows b3TimeSpan computation
**
**      Revision 1.19  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.18  2001/10/10 17:52:24  sm
**      - Texture loading (only reading into memory) running.
**      - Raytracing without OpenGL must be possible!
**
**      Revision 1.17  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.16  2001/09/02 18:54:56  sm
**      - Moving objects
**      - BBox size recomputing fixed. Further cleanups in b3RenderObject
**        are necessary.
**      - It's really nice to see!
**
**      Revision 1.15  2001/09/01 06:16:42  sm
**      - Some merges
**      - Exchanging matrices between OpenGL nd Blizzard III.
**
**      Revision 1.14  2001/08/20 19:35:08  sm
**      - Index correction introduced (This is a hack!)
**      - Some OpenGL cleanups
**
**      Revision 1.13  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.12  2001/08/17 14:08:34  sm
**      - Now trying to draw BSPline surfaces with own routines.
**
**      Revision 1.11  2001/08/17 04:16:43  sm
**      - Using OpenGL NURBS zu render BSpline areas. But
**        I think mi tessalation is faster.
**
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

static b3_color light0_ambient =
{
	0.0f,0.25f,0.25f,0.25f
};

static b3_color light0_diffuse =
{
	0.0f,0.8f,0.8f,0.8f
};

static b3_vector light0_position =
{
	1000.0f,-2500.0f,2000.0f
};

#ifdef BLZ3_USE_OPENGL
static GLint light_num[] =
{
	GL_LIGHT0,
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
	GL_LIGHT4,
	GL_LIGHT5,
	GL_LIGHT6,
	GL_LIGHT7
};

#define VALIDATE_LIGHT_NUM(num) (((num) >= 0) && ((num) < (sizeof(light_num) / sizeof(GLint))))

#endif

b3RenderContext::b3RenderContext()
{
	b3SetBGColor(0.9,0.9,0.9);
	b3LightNum();
}

void b3RenderContext::b3Init()
{
#ifdef BLZ3_USE_OPENGL
	b3PrintF(B3LOG_DEBUG,"OpenGL vendor:     %s\n",glGetString(GL_VENDOR));
	b3PrintF(B3LOG_DEBUG,"OpenGL renderer:   %s\n",glGetString(GL_RENDERER));
	b3PrintF(B3LOG_DEBUG,"OpenGL version:    %s\n",glGetString(GL_VERSION));
	b3PrintF(B3LOG_DEBUG,"OpenGL extensions: %s\n",glGetString(GL_EXTENSIONS));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);

	// Enable light
	b3LightReset();
	b3LightSet(&light0_position);

	// Some material settings
//	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
#endif
}

void b3RenderContext::b3LightReset()
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

	// Disable all other lights
	for (int i = 0;i < (sizeof(light_num) / sizeof(GLint));i++)
	{
		glDisable(light_num[i]);
	}
}

void b3RenderContext::b3LightNum(b3_index num)
{
	if (VALIDATE_LIGHT_NUM(num))
	{
		glLightNum = num;
	}
}

void b3RenderContext::b3LightAdd(
	b3_vector *b3_position,
	b3_color  *b3_diffuse,
	b3_color  *b3_ambient)
{
	b3PrintF(B3LOG_DEBUG,"b3LightAdd(%d)\n",glLightNum);

	if (VALIDATE_LIGHT_NUM(glLightNum))
	{
		b3LightSet(b3_position,b3_ambient,b3_diffuse,glLightNum++);
	}
}

void b3RenderContext::b3LightSet(
	b3_vector *b3_position,
	b3_color  *b3_diffuse,
	b3_color  *b3_ambient,
	b3_index   num)
{
	GLfloat gl_position[4];
	GLfloat gl_ambient[4];
	GLfloat gl_diffuse[4];
	GLint   light;

	if (VALIDATE_LIGHT_NUM(num))
	{
		b3PrintF(B3LOG_DEBUG,"b3LightSet(%d)\n",num);

		light = light_num[num];

		gl_position[0] = b3_position->x;
		gl_position[1] = b3_position->y;
		gl_position[2] = b3_position->z;
		gl_position[3] = 1;

		if (b3_ambient == null)
		{
			b3_ambient = &light0_ambient;
		}
		gl_ambient[0]  =        b3_ambient->r;
		gl_ambient[1]  =        b3_ambient->g;
		gl_ambient[2]  =        b3_ambient->b;
		gl_ambient[3]  = 1.0f - b3_ambient->a;

		if (b3_diffuse == null)
		{
			b3_diffuse = &light0_diffuse;
		}
		gl_diffuse[0]  =        b3_diffuse->r;
		gl_diffuse[1]  =        b3_diffuse->g;
		gl_diffuse[2]  =        b3_diffuse->b;
		gl_diffuse[3]  = 1.0f - b3_diffuse->a;

		b3PrintF(B3LOG_NORMAL,"Light %d: %3.2f %3.2f %3.2f\n",
			light - GL_LIGHT0,gl_position[0],gl_position[1],gl_position[2]);

		glEnable( light);
		glLightfv(light,GL_AMBIENT, gl_ambient);
		glLightfv(light,GL_DIFFUSE, gl_diffuse);
		glLightfv(light,GL_POSITION,gl_position);
	}
}

void b3RenderContext::b3SetBGColor(b3_color *color)
{
	glBGColor = *color;
}

void b3RenderContext::b3SetBGColor(b3_f64 r,b3_f64 g,b3_f64 b)
{
	glBGColor.a = 0;
	glBGColor.r = r;
	glBGColor.g = g;
	glBGColor.b = b;
}

void b3RenderContext::b3StartDrawing()
{
#ifdef BLZ3_USE_OPENGL
	glClearColor(glBGColor.r,glBGColor.g,glBGColor.b,1.0 - glBGColor.a);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#endif
}

b3_bool b3RenderContext::b3GetMatrix(
	b3_matrix_mode  mode,
	b3_matrix      *matrix)
{
#ifdef BLZ3_USE_OPENGL
	b3_bool result = false;
	GLfloat values[16];

	switch(mode)
	{
	case B3_MATRIX_OBJECT:
		glGetFloatv(GL_MODELVIEW_MATRIX,values);
		result = true;
		break;

	case B3_MATRIX_PROJECTION:
		glGetFloatv(GL_PROJECTION_MATRIX,values);
		result = true;
		break;

	default:
		result = false;
		break;
	}
	if (result)
	{
		matrix->m11 = values[ 0];
		matrix->m12 = values[ 4];
		matrix->m13 = values[ 8];
		matrix->m14 = values[12];

		matrix->m21 = values[ 1];
		matrix->m22 = values[ 5];
		matrix->m23 = values[ 9];
		matrix->m24 = values[13];

		matrix->m31 = values[ 2];
		matrix->m32 = values[ 6];
		matrix->m33 = values[10];
		matrix->m34 = values[14];

		matrix->m41 = values[ 3];
		matrix->m42 = values[ 7];
		matrix->m43 = values[11];
		matrix->m44 = values[15];
	}
	return result;
#else
	return false;
#endif
}

b3_bool b3RenderContext::b3PutMatrix(
	b3_matrix_mode  mode,
	b3_matrix      *matrix)
{
#ifdef BLZ3_USE_OPENGL
	b3_bool result = false;
	GLfloat values[16];

	values[ 0] = matrix->m11;
	values[ 1] = matrix->m21;
	values[ 2] = matrix->m31;
	values[ 3] = matrix->m41;

	values[ 4] = matrix->m12;
	values[ 5] = matrix->m22;
	values[ 6] = matrix->m32;
	values[ 7] = matrix->m42;

	values[ 8] = matrix->m13;
	values[ 9] = matrix->m23;
	values[10] = matrix->m33;
	values[11] = matrix->m43;

	values[12] = matrix->m14;
	values[13] = matrix->m24;
	values[14] = matrix->m34;
	values[15] = matrix->m44;

	switch(mode)
	{
	case B3_MATRIX_OBJECT:
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(values);
		result = true;
		break;

	case B3_MATRIX_PROJECTION:
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(values);
		result = true;
		break;

	default:
		result = false;
		break;
	}
	return result;
#else
	return false;
#endif
}
	
/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderObject::b3RenderObject()
{
	glVertexCount = 0;
	glGridCount   = 0;
	glPolyCount   = 0;

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

void b3RenderObject::b3AddCount(b3RenderContext *context)
{
	context->glVertexCount += glVertexCount;
	context->glPolyCount   += glPolyCount;
	context->glGridCount   += glGridCount;
}

void b3RenderObject::b3AllocVertices(b3RenderContext *context)
{
#ifdef BLZ3_USE_OPENGL
	b3_count new_vertCount = 0;
	b3_count new_gridCount = 0;
	b3_count new_polyCount = 0;

	b3GetCount(context,new_vertCount,new_gridCount,new_polyCount);

	if (glVertexCount != new_vertCount)
	{
		b3Free(glVertices);
		b3Free(glNormals);
		glVertices = null;
		glNormals  = null;
		glVertexCount = new_vertCount;

		if (glVertexCount > 0)
		{
			glVertices =  (GLfloat *)b3Alloc(glVertexCount * 3 * sizeof(GLfloat));
			glNormals  =  (GLfloat *)b3Alloc(glVertexCount * 3 * sizeof(GLfloat));
		}
		glComputed = false;
	}

	if (glGridCount != new_gridCount)
	{
		b3Free(glGrids);
		glGrids     = null;
		glGridCount = new_gridCount;

		if (glGridCount > 0)
		{
			glGrids = (GLushort *)b3Alloc(glGridCount   * 2 * sizeof(GLushort));
		}
		glComputed = false;
	}

	if (glPolyCount != new_polyCount)
	{
		b3Free(glPolygons);
		glPolygons  = null;
		glPolyCount = new_polyCount;

		if (glPolyCount > 0)
		{
			glPolygons = (GLushort *)b3Alloc(glPolyCount   * 3 * sizeof(GLushort));
		}
		glComputed = false;
	}
#endif
}

void b3RenderObject::b3FreeVertices()
{
#ifdef BLZ3_USE_OPENGL
	b3Free(glVertices);
	b3Free(glNormals);
	b3Free(glGrids);
	b3Free(glPolygons);
	glVertices = null;
	glNormals  = null;
	glGrids    = null;
	glPolygons = null;
#endif
	glVertexCount = 0;
	glGridCount   = 0;
	glPolyCount   = 0;
}

void b3RenderObject::b3ComputeVertices()
{
}

void b3RenderObject::b3ComputeIndices()
{
}

void b3RenderObject::b3ComputeNormals(b3_bool normalize)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *nPtr = (b3_vector *)glNormals;
	b3_vector *vPtr = (b3_vector *)glVertices;
	GLushort  *pPtr = glPolygons;
	b3_vector  normal;
	b3_vector  xDir,yDir;
	b3_f64     len;
	b3_index   i,p1,p2,p3,start,end;

	if (nPtr == null)
	{
		// Do nothing!
		return;
	}

	// Clear normals
	b3GetVertexRange(start,end);
	for (i = start;i < end;i++)
	{
		nPtr[i].x = 0;
		nPtr[i].y = 0;
		nPtr[i].z = 0;
	}

	// Collect normals
	for (i = 0;i < glPolyCount;i++)
	{
		p1 = *pPtr++;
		p2 = *pPtr++;
		p3 = *pPtr++;

		// Do some semantic checks
		B3_ASSERT((start <= p1) && (p1 < end));
		B3_ASSERT((start <= p2) && (p2 < end));
		B3_ASSERT((start <= p3) && (p3 < end));

		xDir.x = vPtr[p2].x - vPtr[p1].x;
		xDir.y = vPtr[p2].y - vPtr[p1].y;
		xDir.z = vPtr[p2].z - vPtr[p1].z;

		yDir.x = vPtr[p3].x - vPtr[p1].x;
		yDir.y = vPtr[p3].y - vPtr[p1].y;
		yDir.z = vPtr[p3].z - vPtr[p1].z;

		normal.x = xDir.y * yDir.z - xDir.z * yDir.y;
		normal.y = xDir.z * yDir.x - xDir.x * yDir.z;
		normal.z = xDir.x * yDir.y - xDir.y * yDir.x;
		len = b3Vector::b3Length(&normal);
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
		for (i = 0;i < glVertexCount;i++)
		{
			len = b3Vector::b3Length(&nPtr[i]);
			if (len > 0)
			{
				nPtr[i].x /= len;
				nPtr[i].y /= len;
				nPtr[i].z /= len;
			}
		}
	}
#endif
}

void b3RenderObject::b3GetVertexRange(b3_index &start,b3_index &end)
{
#ifdef BLZ3_USE_OPENGL
	start = 0;
	end   = glVertexCount;
#endif
}

b3_bool b3RenderObject::b3ComputeBounds(b3_vector *lower,b3_vector *upper)
{
	b3_vector *ptr;
	b3_bool    result = false;
	b3_index   i,start,end;

#ifdef BLZ3_USE_OPENGL
	b3Update();
	if (glComputed && (glVertices != null) && (glVertexCount > 0))
	{
		ptr = (b3_vector *)glVertices;
		b3GetVertexRange(start,end);
		for (i = start;i < end;i++)
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

void b3RenderObject::b3GetGridColor(b3_color *color)
{
	color->r = 0.2f;
	color->g = 0.2f;
	color->b = 0.2f;
	color->a = 0.0f;
}

void b3RenderObject::b3GetDiffuseColor(b3_color *color)
{
	color->r = 0.0f;
	color->g = 0.5f;
	color->b = 1.0f;
	color->a = 0.0f;
}

b3_render_mode b3RenderObject::b3GetRenderMode()
{
	return B3_RENDER_LINE;
}

void b3RenderObject::b3Recompute()
{
#ifdef BLZ3_USE_OPENGL
	glComputed = false;
#endif
}

void b3RenderObject::b3Update()
{
#ifdef BLZ3_USE_OPENGL
	if (!glComputed)
	{
		b3ComputeIndices();
		b3ComputeVertices();
		b3ComputeNormals();
		glComputed = true;
	}
#endif
}

void b3RenderObject::b3TransformVertices(b3_matrix *transformation)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector *ptr;
	b3_count   i;

	if (glVertices != null)
	{
		ptr = (b3_vector *)glVertices;
		for (i = 0;i < glVertexCount;i++)
		{
			b3Vector::b3MatrixMul4D(transformation,ptr++);
		}
	}

	if (glNormals != null)
	{
		ptr = (b3_vector *)glNormals;
		for (i = 0;i < glVertexCount;i++)
		{
			b3Vector::b3MatrixMul3D(transformation,ptr++);
		}
	}
#endif
}

void b3RenderObject::b3Draw()
{
	b3_render_mode render_mode = b3GetRenderMode();
	b3_color       color;

#ifdef BLZ3_USE_OPENGL
	b3Update();
#ifdef _DEBUG
	b3_index       i;

	switch (render_mode)
	{
	case B3_RENDER_LINE:
		// This loop collects access vialoations
		// prior calling OpenGL routines. This
		// makes it possible to catch to faulty
		// index data. The access simply compute
		// the length of the lines to be drawn.
		for (i = 0;i < glPolyCount;i++)
		{
			b3_vector aPoint,bPoint,cPoint;
			b3_index  a,b,c;
			b3_f64    aLen,bLen;

			a = glPolygons[i * 3]     * 3;
			aPoint.x = glVertices[a++];
			aPoint.y = glVertices[a++];
			aPoint.z = glVertices[a++];

			b = glPolygons[i * 3] * 3;
			bPoint.x = glVertices[b++];
			bPoint.y = glVertices[b++];
			bPoint.z = glVertices[b++];

			c = glPolygons[i * 3] * 3;
			cPoint.x = glVertices[c++];
			cPoint.y = glVertices[c++];
			cPoint.z = glVertices[c++];

			aLen = b3Vector::b3Distance(&aPoint,&bPoint);
			bLen = b3Vector::b3Distance(&aPoint,&cPoint);
		}
		break;

	case B3_RENDER_FILLED:
		for (i = 0;i < glGridCount;i++)
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

			len = b3Vector::b3Distance(&aPoint,&bPoint);
		}
		break;
	}
#endif

	switch (render_mode)
	{
	case B3_RENDER_NOTHING:
		// Do nothing!
		break;

	case B3_RENDER_LINE:
		if (glGridCount > 0)
		{
			glVertexPointer(3, GL_FLOAT, 0, glVertices);
			glNormalPointer(GL_FLOAT,    0, glNormals);

			b3GetGridColor(&color);
			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glColor3f(color.r,color.g,color.b);
			glDrawElements(GL_LINES,glGridCount * 2,GL_UNSIGNED_SHORT,glGrids);
		}
		break;

	case B3_RENDER_FILLED:
		if (glPolyCount > 0)
		{
			glVertexPointer(3, GL_FLOAT, 0, glVertices);
			glNormalPointer(GL_FLOAT,    0, glNormals);

			b3GetDiffuseColor(&color);
			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(color.r,color.g,color.b);
			glDrawElements(GL_TRIANGLES, glPolyCount * 3,GL_UNSIGNED_SHORT,glPolygons);
		}
		break;

	default:
		b3PrintF(B3LOG_NORMAL,"Illegal render mode selected.\n");
		break;
	}
#endif
}
