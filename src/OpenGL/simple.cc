/*
**
**	$Filename:	simple.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - simple OpenGL testing
**
**	(C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.11  2004/11/29 09:58:00  smork
**      - Changed exit states to correct defines.
**      - Added switch for disabling VBO in OpenGL renderer.
**      - Added switches for logging level in OpenGL renderer as in brt3.
**
**      Revision 1.10  2004/11/21 14:56:57  sm
**      - Merged VBO development into main trunk.
**
**      Revision 1.9.2.1  2004/11/21 10:17:32  sm
**      - We have to map the object before getting the pointer. Then the
**        bounding boxes can be computed correctly to setup the far and
**        near clipping plane correctly. When mapping correctly the
**        transformation can occur correctly which plays the ananimation
**        in a way we expect ;-)
**        ** That's it **
**
**      Revision 1.9  2004/09/24 11:42:13  sm
**      - First VBO run under Linux.
**
**      Revision 1.8  2004/09/23 15:47:04  sm
**      - Splitted b3RenderContext into own file.
**      - Added vertex buffer object support which does not
**        run yet.
**
**      Revision 1.7  2004/09/23 09:31:33  sm
**      - Changed b3Runtime into a real singleton.
**      - Added functions for OpenGL extension.
**      - Removed b3PrintF()s from singletons.
**
**      Revision 1.6  2004/09/22 18:56:22  sm
**      - More fail safe procedure relocation.
**
**      Revision 1.5  2004/09/22 15:02:59  sm
**      - Experimenting with OpenGL extensions. Especially the VBOs
**        are interesting to avoid a data copy everytime when
**        drawing is needed.
**
**      Revision 1.4  2004/08/26 07:08:10  sm
**      - Higher bitrate in DivX encoder.
**      - Disbled unnecessary AUTO_NORMAL option in OpenGL renderer.
**
**      Revision 1.3  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
**      Revision 1.2  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
*/

/*************************************************************************
**                                                                      **
**                        Some static data                              **
**                                                                      **
*************************************************************************/

#if defined(BLZ3_USE_OPENGL) && defined(BLZ3_USE_GLUT)

GLfloat points[] =
{
	-1., -1., -1., // front
	-1., -1.,  1.,
	1., -1.,  1.,
	1., -1., -1.,

	-1.,  1., -1., // left
	-1.,  1.,  1.,
	-1., -1.,  1.,
	-1., -1., -1.,

	1.,  1., -1., // back
	1.,  1.,  1.,
	-1.,  1.,  1.,
	-1.,  1., -1.,

	1., -1., -1., // right
	1., -1.,  1.,
	1.,  1.,  1.,
	1.,  1., -1.,

	-1., -1.,  1., // top
	-1.,  1.,  1.,
	1.,  1.,  1.,
	1., -1.,  1.,

	-1.,  1., -1., // bottom
	-1., -1., -1.,
	1., -1., -1.,
	1.,  1., -1
};

GLfloat normals[] =
{
	0.0, -1.0,  0.0, // top
	0.0, -1.0,  0.0,
	0.0, -1.0,  0.0,
	0.0, -1.0,  0.0,

	-1.0,  0.0,  0.0, // left
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,

	0.0,  1.0,  0.0, // back
	0.0,  1.0,  0.0,
	0.0,  1.0,  0.0,
	0.0,  1.0,  0.0,

	1.0,  0.0,  0.0, // right
	1.0,  0.0,  0.0,
	1.0,  0.0,  0.0,
	1.0,  0.0,  0.0,

	0.0,  0.0,  1.0, // top
	0.0,  0.0,  1.0,
	0.0,  0.0,  1.0,
	0.0,  0.0,  1.0,

	0.0,  0.0, -1.0, // bottom
	0.0,  0.0, -1.0,
	0.0,  0.0, -1.0,
	0.0,  0.0, -1.0
};

GLubyte indices[] =
{
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9,10,11,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23
};

GLfloat boxVertices[8 * 3] =
{
	-1.0,-1.0,-1.0,
	-1.0,-1.0, 1.0,
	1.0,-1.0, 1.0,
	1.0,-1.0,-1.0,
	-1.0, 1.0,-1.0,
	-1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0,-1.0
};

GLubyte boxIndices[12 * 2] =
{
	0,1,
	1,2,
	2,3,
	3,0,
	4,5,
	5,6,
	6,7,
	7,4,
	0,4,
	1,5,
	2,6,
	3,7
};

GLfloat ambient[] =
{
	1.0,1.0,1.0,1.0
};

GLfloat diffuse[] =
{
	1.0,1.0,0.95,1.0
};

GLfloat light0[] =
{
	10.0,15.0,20.0,1.0
};

PFNGLGENBUFFERSARBPROC    glGenBuffersARB;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
PFNGLBINDBUFFERARBPROC    glBindBufferARB;
PFNGLBUFFERDATAARBPROC    glBufferDataARB;
PFNGLMAPBUFFERARBPROC     glMapBufferARB;
PFNGLUNMAPBUFFERARBPROC   glUnmapBufferARB;

GLuint  vbo[2];
b3_bool has_vbo;

void init_vbo()
{
	char *extensions = (char *)glGetString(GL_EXTENSIONS);
	if (strstr(extensions,"ARB_vertex_buffer_object") != 0)
	{
		glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)   b3Runtime::b3GetOpenGLExtension("glGenBuffersARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)b3Runtime::b3GetOpenGLExtension("glDeleteBuffersARB");
		glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)   b3Runtime::b3GetOpenGLExtension("glBindBufferARB");
		glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)   b3Runtime::b3GetOpenGLExtension("glBufferDataARB");
		glMapBufferARB     = (PFNGLMAPBUFFERARBPROC)    b3Runtime::b3GetOpenGLExtension("glMapBufferARB");
		glUnmapBufferARB   = (PFNGLUNMAPBUFFERARBPROC)  b3Runtime::b3GetOpenGLExtension("glUnmapBufferARB");

		has_vbo =
			(glGenBuffersARB != null) &&
			(glBindBufferARB != null) &&
			(glBufferDataARB != null) &&
			(glMapBufferARB  != null) &&
			(glUnmapBufferARB != null);
		if (!has_vbo)
		{
			printf("glGenBuffersARB  = %p\n",glGenBuffersARB);
			printf("glBindBufferARB  = %p\n",glBindBufferARB);
			printf("glBufferDataARB  = %p\n",glBufferDataARB);
			printf("glMapBufferARB   = %p\n",glMapBufferARB);
			printf("glUnmapBufferARB = %p\n",glUnmapBufferARB);
		}
	}
	else
	{
		has_vbo = false;
	}
}


void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#if 0
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FLAT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.0f,0.1f,0.3f);
	glVertexPointer(3, GL_FLOAT, 0, points);
	glNormalPointer(   GL_FLOAT, 0, normals);
	glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,indices);

	glPopMatrix();
#else
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FLAT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.0f,1.0f,1.0f);

	if (has_vbo)
	{
		printf("drawing vertex buffer objects...\n");
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,vbo[1]);
		glDrawElements(GL_LINES,24,GL_UNSIGNED_BYTE,0);
	}
	else
	{
		printf("Drawing vertex array...\n");
		glVertexPointer(3, GL_FLOAT, 0, boxVertices);
		glDrawElements(GL_LINES,24,GL_UNSIGNED_BYTE, boxIndices);
	}

	glPopMatrix();
#endif
	glutSwapBuffers();
}

void ChangeSize(GLsizei xSize,GLsizei ySize)
{
	GLfloat aspect = (GLfloat)xSize / (GLfloat)ySize;

	glViewport(0,0,xSize,ySize);

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
	gluPerspective (60.0f, aspect, 0.1f,100.0f);
	glTranslated(0.0,0.0,-5.0);
	glRotatef(-30.0f,0.0f,1.0f,0.0f);
	glRotatef(-50.0f,1.0f,0.0f,0.0f);
#endif
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetupRC()
{
	glClearColor(0.7f,0.7f,1.0f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,light0);

	init_vbo();
	if (has_vbo)
	{
		void *ptr;

		printf("Vertex buffer objects available.\n");
		glGenBuffersARB(2,vbo);

		printf("Setup vertices...\n");
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(boxVertices), boxVertices, GL_DYNAMIC_DRAW_ARB);
		/*
				ptr = glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
				memcpy(ptr,boxVertices,sizeof(boxVertices));
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		*/
		printf("Setup indices...\n");
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo[1]);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(boxIndices), NULL, GL_DYNAMIC_DRAW_ARB);
		ptr = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
		memcpy(ptr,boxIndices,sizeof(boxIndices));
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);

		printf("Done.\n");
	}
	else
	{
		printf("No vertex buffer objects available.\n");
	}
}

int main(int argc,char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("Erster Test");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRC();

	glutMainLoop();
	return EXIT_SUCCESS;
}

#else

#include <stdio.h>

int main(int argc,char *argv[])
{
	fprintf(stderr,"This platform has got no OpenGL support!\n");

	return EXIT_SUCCESS;
}

#endif
