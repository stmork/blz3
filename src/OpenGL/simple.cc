#include <GL/glut.h>

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

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FLAT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.0f,0.1f,0.3f);
	glVertexPointer(3, GL_FLOAT, 0, points);
	glNormalPointer(   GL_FLOAT, 0, normals);
	glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,indices);

	glPopMatrix();	
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
