/*
**
**      $Filename:      render.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Show scene with OpenGL
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include <GL/glut.h>

#include "blz3/b3Config.h" 
#include "blz3/base/b3World.h"
#include "blz3/raytrace/b3Raytrace.h"

static b3World world;

void RenderScene()
{
	b3Scene  *scene;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);

	scene = (b3Scene *)world.b3GetFirst();
	scene->b3Draw();

	glutSwapBuffers();
}

void ChangeSize(GLsizei xSize,GLsizei ySize)
{
	b3Scene  *scene;
	GLfloat  aspect = (GLfloat)xSize / (GLfloat)ySize;

	scene = (b3Scene *)world.b3GetFirst();
//	scene->b3SetView((b3_res)xSize,(b3_res)ySize);
	glViewport(0,0,xSize,ySize);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective (160.0f, aspect, 0.1f,10000.0f);	
//	glTranslated(0.0,0.0,-500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetupRC()
{
	glClearColor(0.7f,0.7f,1.0f,1.0f);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc,char *argv[])
{
	b3Item   *item;
	b3Scene  *scene;

	if (argc > 1)
	{
		b3InitRaytrace::b3Init();

		world.b3Read(argv[1]);
		for (item  = world.b3GetFirst();
		     item != null;
		     item  = scene->Succ)
		{
			scene = (b3Scene *)item;
			scene->b3Reorg();
		}

		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
		glutCreateWindow("Greetinxx");
sleep(3);
		glutDisplayFunc(RenderScene);
		glutReshapeFunc(ChangeSize);

		SetupRC();
	
		glutMainLoop();
	}
	return 0;
}
