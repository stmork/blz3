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
#include <dlfcn.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
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

typedef void      (*procBindBufferARB)(GLenum target, GLuint buffer);
typedef void      (*procDeleteBuffersARB)(GLsizei n, const GLuint *buffers);
typedef void      (*procGenBuffersARB)(GLsizei n, GLuint *buffers);
typedef GLboolean (*procIsBufferARB)(GLuint buffer);
typedef void      (*procBufferDataARB)(GLenum target, GLsizei size, const GLvoid *data, GLenum usage);
typedef void      (*procBufferSubDataARB)(GLenum target, GLint *offset, GLsizei *size, const GLvoid *data);
typedef void      (*procGetBufferSubDataARB)(GLenum target, GLint *offset, GLsizei *size, GLvoid *data);
typedef void *    (*procMapBufferARB)(GLenum target, GLenum access);
typedef void      (*procUnmapBufferARB)(GLenum target);
typedef void      (*procGetBufferParameterivARB)(GLenum target, GLenum pname, GLint *params);
typedef void      (*procGetBufferPointervARB)(GLenum target, GLenum pname, void **params);

procBindBufferARB    glBindBufferARB;
procDeleteBuffersARB glDeleteBuffersARB;
procGenBuffersARB    glGenBuffersARB;
procBufferDataARB    glBufferDataARB;
procMapBufferARB     glMapBufferARB;
procUnmapBufferARB   glUnmapBufferARB;

GLuint  vbo[2];
b3_bool has_vbo;

void init_vbo()
{
	void *handle;

	char *extensions = glGetString(GL_EXTENSIONS);
	if (strstr(extensions,"vertex_buffer_object") != 0)
	{
		handle = dlopen("libGL.so",RTLD_LAZY);
		if (handle != null)
		{
			glBindBufferARB  = dlsym(handle, "glBindBufferARB");
			glGenBuffersARB  = dlsym(handle, "glGenBuffersARB");
			glBufferDataARB  = dlsym(handle, "glBufferDataARB");
			glMapBufferARB   = dlsym(handle, "glMapBufferARB");
			glUnmapBufferARB = dlsym(handle, "glUnmapBufferARB");

			has_vbo = true;
		}
		else
		{
			has_vbo = false;
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
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,vbo[1]);
		glDrawElements(GL_LINES,24,GL_UNSIGNED_BYTE,0);
	}
	else
	{
		glVertexPointer(3, GL_FLOAT, 0, boxVertices);
		glDrawElements(GL_LINES,24,GL_UNSIGNED_BYTE, boxIndices);
	}
	
	glPopMatrix();	
#endif
	glutSwapBuffers();
}

void ChangeSize(GLsizei xSize,GLsizei ySize)
{
	GLfloat xWindow, yWindow, aspect;

	aspect = (GLfloat)xSize / (GLfloat)ySize;
	if (aspect <= 1.0)
	{
		xWindow = 5 * ySize / xSize;
		yWindow = 5;
	}
	else
 	{
		xWindow = 5 * xSize / ySize;
		yWindow = 5;
	}
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

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(boxVertices), NULL, GL_DYNAMIC_DRAW_ARB);
		ptr = glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
		memcpy(ptr,boxVertices,sizeof(boxVertices));
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo[1]);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(boxIndices), NULL, GL_DYNAMIC_DRAW_ARB);
		ptr = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
		memcpy(ptr,boxIndices,sizeof(boxIndices));
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
	}
	else
	{
		printf("No vertex buffer objects available.\n");
	}
}

int main(int argc,char *argv[])
{
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutCreateWindow("Erster Test");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRC();
	
	glutMainLoop();
	return 0;
}

#else

#include <stdio.h>

int main(int argc,char *argv[])
{
	fprintf(stderr,"This platform has got no OpenGL support!\n");
	return 0;
}

#endif
