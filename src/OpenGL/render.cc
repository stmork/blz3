/*
**
**	$Filename:	render.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Show scene with OpenGL
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include <GL/glut.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/b3Config.h" 
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/raytrace/b3RenderView.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.8  2001/08/14 07:03:28  sm
**      - Made some ASSERT cleanups. New define when _DEBUG is switched on:
**        B3_ASSERT(condition) abort()s when condition is false.
**
**      Revision 1.7  2001/08/11 20:17:22  sm
**      - Updated OpenGL on Un*x platform.
**      - This was a great day!
**
**      Revision 1.6  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.5  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.4  2001/08/07 16:54:26  sm
**      - Checking bounds on condition base for line drawing
**      - Some object reordering
**      - Bug fix for Mandel makefile
**
**      Revision 1.3  2001/08/03 15:54:09  sm
**      - Compilation of OpenGL under Windows NT
**
**      Revision 1.2  2001/08/02 15:38:42  sm
**      - Some minor changes
**
*/

static b3World      *world = null;
static b3RenderView  view;

void RenderScene()
{
	b3Scene  *scene;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);

	scene = (b3Scene *)world->b3GetFirst();
	scene->b3Draw();

	glutSwapBuffers();
}

void ChangeSize(GLsizei xSize,GLsizei ySize)
{
	b3Scene  *scene;

	scene = (b3Scene *)world->b3GetFirst();
	view.b3SetCamera(scene);
	view.b3UpdateView(0,0,xSize,ySize);
}

void SetupRC()
{
	glClearColor(0.7f,0.7f,1.0f,1.0f);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc,char *argv[])
{
	b3Item               *item;
	b3Scene              *scene;
	b3_res                xSize,ySize;
	b3RenderShapeContext  context;

	if (argc > 1)
	{
		b3InitRaytrace::b3Init();

//		b3Log_SetLevel(B3LOG_FULL);
		world = new b3World();
		world->b3Read(argv[1]);
		for (item  = world->b3GetFirst();
		     item != null;
		     item  = scene->Succ)
		{
			scene = (b3Scene *)item;
			scene->b3Reorg();
			scene->b3AllocVertices(&context);
			scene->b3GetDisplaySize(xSize,ySize);
			view.b3SetCamera(scene);
			view.b3SetViewMode(B3_VIEW_3D);
			view.b3UpdateView(0,0,xSize,ySize);
		}

		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
		glutInitWindowSize(xSize,ySize);
		glutCreateWindow("Greetinxx");
		glutDisplayFunc(RenderScene);
		glutReshapeFunc(ChangeSize);

		context.b3StartDrawing();
	
		glutMainLoop();
	}
	return 0;
}
