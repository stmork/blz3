/*
**
**	$Filename:	ocean.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Ocean wave testing with real triangles.
**
**	(C) Copyright 2006  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "blz3/b3Config.h"
#include "blz3/base/b3OceanWave.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2006/05/23 20:25:12  sm
**	- Some test updates.
**	- Changed exception handling a little bit.
**
**	Revision 1.7  2006/05/13 10:01:01  sm
**	- Introduced special complex number computation for FFT handling.
**
**	Revision 1.6  2006/05/11 15:34:22  sm
**	- Added unit tests
**	- Corrected normal computation for ocean waves
**	- Optimized b3Complex
**	- Added new FFT
**	- Added own assertion include
**
**	Revision 1.5  2006/05/01 10:44:46  sm
**	- Unifying ocean wave values.
**
**	Revision 1.4  2006/04/30 08:53:24  sm
**	- Removed some signed/unsigned issues.
**	- Reflect new FFT algorithm.
**
**	Revision 1.3  2006/04/29 20:45:57  sm
**	- New scaling.
**
**	Revision 1.2  2006/04/29 17:58:27  sm
**	- Minor value changes.
**
**	Revision 1.1  2006/04/29 11:39:04  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Some static data                              **
**                                                                      **
*************************************************************************/

#if defined(BLZ3_USE_OPENGL) && defined(BLZ3_USE_GLUT)

#define OW_SKIP B3_OCEAN_XSKIP

GLfloat ambient[] =
{
	1.0, 1.0, 1.0, 1.0
};

GLfloat diffuse[] =
{
	1.0, 1.0, 0.95, 1.0
};

GLfloat light0[] =
{
	10.0, 15.0, 20.0, 1.0
};

GLfloat * vPtr = null;
GLuint  * iPtr = null;
b3_size   vSize, iSize;
GLuint    size;
b3Time    timepoint;
b3_f64    start;
b3_f64    last;
b3_f64    halfgrid;

b3OceanWave ocean;

PFNGLGENBUFFERSARBPROC    glGenBuffersARB;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
PFNGLBINDBUFFERARBPROC    glBindBufferARB;
PFNGLBUFFERDATAARBPROC    glBufferDataARB;
PFNGLMAPBUFFERARBPROC     glMapBufferARB;
PFNGLUNMAPBUFFERARBPROC   glUnmapBufferARB;

GLuint  vbo[2];
b3_bool has_vbo;

void init_vertices()
{
	b3_f64 factor;
	GLuint i, k;

	size = 1 << ocean.m_Dim;
	halfgrid = ocean.m_GridSize * 0.5;
	factor   = ocean.m_GridSize / size;
	vSize = size * size * sizeof(GLfloat) * 3;
	iSize = 2 * size * (size - 1) * sizeof(GLint) * 2;

	if(has_vbo)
	{
		printf("Vertex buffer objects available.\n");
		glGenBuffersARB(2, vbo);
	}
	else
	{
		printf("No vertex buffer objects available.\n");
	}

	if(vPtr == null)
	{
		GLfloat * ptr;

		printf("Setup vertices...\n");
		if(has_vbo)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, vSize, null, GL_DYNAMIC_DRAW_ARB);
			vPtr = (GLfloat *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
		}
		else
		{
			vPtr = (GLfloat *)malloc(vSize);
		}
		ptr = vPtr;

		for(k = 0; k < size; k++)
		{
			for(i = 0; i < size; i += OW_SKIP)
			{
				*ptr++ = i * factor;
				*ptr++ = k * factor;
				*ptr++ = 0;
			}
		}

		if(has_vbo)
		{
			glVertexPointer(3, GL_FLOAT, 0, 0);
		}
	}

	if(iPtr == null)
	{
		GLuint * ptr;
		GLuint  index;

		printf("Setup indices...\n");
		if(has_vbo)
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo[1]);
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, iSize, null, GL_DYNAMIC_DRAW_ARB);
			iPtr = (GLuint *)glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
		}
		else
		{
			iPtr  = (GLuint *)malloc(iSize);
		}
		ptr = iPtr;

		index = 0;
		for(k = 0; k < size; k++)
		{
			for(i = OW_SKIP; i < size; i += OW_SKIP)
			{
				*ptr++ = index++;
				*ptr++ = index;
			}
			index++;
		}

		for(i = 0; i < size; i += OW_SKIP)
		{
			index = i / OW_SKIP;
			for(k = 1; k < size; k++)
			{
				*ptr++ = index;
				index += (size / OW_SKIP);
				*ptr++ = index;
			}
		}

		if(has_vbo)
		{
			glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
			iPtr = null;
		}
	}

	timepoint.b3Now();
	start = timepoint.b3GetTime();
	last  = start;
}

void init_vbo()
{
	char * extensions = (char *)glGetString(GL_EXTENSIONS);
	if(strstr(extensions, "ARB_vertex_buffer_object") != 0)
	{
		glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)   b3Runtime::b3GetOpenGLExtension("glGenBuffersARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)b3Runtime::b3GetOpenGLExtension("glDeleteBuffersARB");
		glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)   b3Runtime::b3GetOpenGLExtension("glBindBufferARB");
		glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)   b3Runtime::b3GetOpenGLExtension("glBufferDataARB");
		glMapBufferARB     = (PFNGLMAPBUFFERARBPROC)    b3Runtime::b3GetOpenGLExtension("glMapBufferARB");
		glUnmapBufferARB   = (PFNGLUNMAPBUFFERARBPROC)  b3Runtime::b3GetOpenGLExtension("glUnmapBufferARB");

		has_vbo =
			false &&
			(glGenBuffersARB != null) &&
			(glBindBufferARB != null) &&
			(glBufferDataARB != null) &&
			(glMapBufferARB  != null) &&
			(glUnmapBufferARB != null);
		if(!has_vbo)
		{
			printf("glGenBuffersARB  = %p\n", glGenBuffersARB);
			printf("glBindBufferARB  = %p\n", glBindBufferARB);
			printf("glBufferDataARB  = %p\n", glBufferDataARB);
			printf("glMapBufferARB   = %p\n", glMapBufferARB);
			printf("glUnmapBufferARB = %p\n", glUnmapBufferARB);
		}
	}
	else
	{
		has_vbo = false;
	}
}


void RenderScene()
{
	b3Complex64 * src;
	GLfloat   *   dst;
	GLuint        i, k;
	b3_f64        t, now = timepoint.b3Now();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	t = now - start;
	printf("t = %1.3fs d = %1.3fs %1.3ff/s\n", t, now - last, 1.0 / (now - last));
	ocean.b3PrepareOceanWave(t);
	last = now;

	src = ocean.b3GetBuffer();
	dst = &vPtr[2];
	for(k = 0; k < size; k++)
	{
		for(i = 0; i < size; i += OW_SKIP)
		{
			*dst = src->b3Real() * 0.001;
			src += OW_SKIP;
			dst += 3;
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FLAT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.0f, 1.0f, 1.0f);

	if(has_vbo)
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo[1]);
		glDrawElements(GL_LINES, 4 * size * (size - 1) / OW_SKIP, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glVertexPointer(3, GL_FLOAT, 0, vPtr);
		glDrawElements(GL_LINES, 4 * size * (size - 1) / OW_SKIP, GL_UNSIGNED_INT, iPtr);
	}

	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}

void ChangeSize(GLsizei xSize, GLsizei ySize)
{
	GLfloat aspect = (GLfloat)xSize / (GLfloat)ySize;

	glViewport(0, 0, xSize, ySize);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#if 0
	glOrtho(-xWindow, xWindow, -yWindow, yWindow, 2.0f, -2.0f);
#else
	/*
	gluLookAt(
	0.0f, 0.0f, 5.0f,
	0.0f, 0.0f, 4.0f,
	0.0f, 1.0f, 0.0f);
	glTranslated(0.0,0.0,2.0);
	*/
	gluPerspective(60.0f, aspect, 0.1f, size);
	glTranslated(-halfgrid, -10.0, -2.0);
	glRotatef(-00.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-70.0f, 1.0f, 0.0f, 0.0f);
#endif
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetupRC()
{
	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0);

	init_vbo();
	init_vertices();
}

int main(int argc, char * argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Ozeanwellen");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRC();

	glutMainLoop();
	return EXIT_SUCCESS;
}

#else

#include <stdio.h>

int main(int argc, char * argv[])
{

	fprintf(stderr, "This platform has got no OpenGL support!\n");

	return EXIT_SUCCESS;
}

#endif
