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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/b3Config.h" 
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/raytrace/b3RenderView.h"
#include "blz3/base/b3SearchPath.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.14  2001/11/01 09:43:11  sm
**      - Some image logging cleanups.
**      - Texture preparing now in b3Prepare().
**      - Done some minor fixes.
**
**      Revision 1.13  2001/10/31 14:46:35  sm
**      - Filling b3IsCancelled() with sense.
**      - Inlining b3RGB
**
**      Revision 1.12  2001/09/04 15:15:57  sm
**      - Added rotating objects
**
**      Revision 1.11  2001/08/21 14:24:14  sm
**      - New selecting code added.
**
**      Revision 1.10  2001/08/16 14:41:24  sm
**      - Some more shading shapes added (only BSPline shapes are missing)
**
**      Revision 1.9  2001/08/14 15:37:50  sm
**      - Made some cleanups when OpenGL isn't available.
**
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

#ifdef BLZ3_USE_OPENGL

static b3RenderShapeContext  context;
static b3World              *world = null;
static b3RenderView          view;

void RenderScene()
{
	b3Scene  *scene;

	context.b3StartDrawing();
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
	b3Item    *item;
	b3Scene   *scene;
	char      *HOME = getenv("HOME");
	b3Path     data;
	b3_res     xSize,ySize;

	if (argc > 1)
	{
		b3InitRaytrace::b3Init();
		b3Dir::b3LinkFileName(data,    HOME,"Blizzard/Data");

//		b3Log_SetLevel(B3LOG_NORMAL);
		b3Log_SetLevel(B3LOG_DEBUG);
//		b3Log_SetLevel(B3LOG_FULL);

		world = new b3World();
		world->b3AddPath(data);
		world->b3Read(argv[1]);
		for (item  = world->b3GetFirst();
		     item != null;
		     item  = scene->Succ)
		{
			scene = (b3Scene *)item;
			scene->b3Reorg();
			scene->b3GetDisplaySize(xSize,ySize);
			scene->b3Prepare(xSize,ySize);
			scene->b3AllocVertices(&context);
			scene->b3Activate();
			view.b3SetCamera(scene);
			view.b3SetViewMode(B3_VIEW_3D);
			view.b3UpdateView(0,0,xSize,ySize);
		}

		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
		glutInitWindowSize(xSize,ySize);
		glutCreateWindow("Greetinxx");
		glutDisplayFunc(RenderScene);
		glutReshapeFunc(ChangeSize);

		context.b3Init();
		glutMainLoop();
	}
	return 0;
}

#else

int main(int argc,char *argv[])
{
	b3PrintF(B3LOG_NORMAL,"This platform has got no OpenGL support!\n");
	return 0;
}

#endif
