/*
**
**	$Filename:	render.cc $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Show scene with OpenGL
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
  
#include "blz3/b3Config.h" 
#include "blz3/system/b3Plugin.h"
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/raytrace/b3RenderLight.h"
#include "blz3/raytrace/b3RenderView.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3SearchPath.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.58  2005/06/09 11:00:57  smork
**      - Call option cleanup.
**
**      Revision 1.57  2005/01/24 14:21:00  smork
**      - Moved some static variables.
**
**      Revision 1.56  2005/01/18 11:49:05  smork
**      - Added support for single buffered OpenGL drawing.
**
**      Revision 1.55  2004/12/30 16:27:38  sm
**      - Removed assertion problem when starting Lines III: The
**        image list were initialized twice due to double calling
**        OnInitDialog() of CDialogBar. The CDialogBar::Create()
**        calls OnInitDialog() automatically sinde MFC 7
**      - Removed many global references from raytrace and base lib
**      - Fixed ticket no. 29: The b3RenderObject::b3Recompute
**        method checks the vertex maintainer against a null pointer.
**
**      Revision 1.54  2004/12/06 15:14:56  smork
**      - Minor changes
**
**      Revision 1.53  2004/12/05 08:18:22  sm
**      - Added VBO not available message.
**
**      Revision 1.52  2004/11/30 19:30:26  sm
**      - Added VBO support settings in properties dialog.
**
**      Revision 1.51  2004/11/30 10:16:14  smork
**      - Added a working VBO version which computes vertices/indices
**        completely in CPU memory and only updates the results into
**        GPU memory finally. It's faster than computing directly
**        in GPU memory but the fastest version is still the common
**        vertex array version.
**
**      Revision 1.50  2004/11/29 09:58:00  smork
**      - Changed exit states to correct defines.
**      - Added switch for disabling VBO in OpenGL renderer.
**      - Added switches for logging level in OpenGL renderer as in brt3.
**
**      Revision 1.49  2004/11/28 20:20:17  sm
**      - Added support for switchable VBOs.
**
**      Revision 1.48  2004/10/16 17:00:52  sm
**      - Moved lighting into own class to ensure light setup
**        after view setup.
**      - Fixed lighting for scene and simple overview
**      - Fixed Light cutoff exponent deadloop.
**      - Corrected OpenGL define (BLZ3_USE_OPENGL)
**
**      Revision 1.47  2004/10/13 15:33:14  smork
**      - Optimized OpenGL lights.
**
**      Revision 1.46  2004/10/12 09:15:46  smork
**      - Some more debug information.
**      - Moved light init after camera init.
**
**      Revision 1.45  2004/09/23 15:47:03  sm
**      - Splitted b3RenderContext into own file.
**      - Added vertex buffer object support which does not
**        run yet.
**
**      Revision 1.44  2004/09/20 13:40:40  sm
**      - Removed GLUT_ALPHA requirement.
**
**      Revision 1.43  2004/08/17 15:19:13  sm
**      - Some OpenGL include fixes.
**
**      Revision 1.42  2004/07/18 08:28:44  sm
**      - Added transformation optimazation: We don't need to recompute
**        vertices on unit matrix transformation. This simple test makes
**        animation a lot faster!
**
**      Revision 1.41  2004/07/15 12:12:20  sm
**      - Some FSAA debugging- Some FSAA debugging.
**
**      Revision 1.40  2004/05/16 09:21:50  sm
**      - Corrected camera access methods.
**
**      Revision 1.39  2004/01/05 08:01:55  sm
**      - Added new year.
**
**      Revision 1.38  2003/07/20 09:38:30  sm
**      - Registering raytracing items manually.
**
**      Revision 1.37  2003/07/12 17:44:46  sm
**      - Cleaned up raytracing b3Item registration
**
**      Revision 1.36  2003/06/09 08:53:48  sm
**      - Added preparation support for all b3Item objects.
**
**      Revision 1.35  2003/03/04 20:37:36  sm
**      - Introducing new b3Color which brings some
**        performance!
**
**      Revision 1.34  2003/02/26 16:36:16  sm
**      - Sorted drawing colors and added configuration support
**        to dialog.
**
**      Revision 1.33  2003/02/22 19:39:34  sm
**      - Fixed some GCC compile errors in b3TIFF stuff.
**
**      Revision 1.32  2003/02/20 16:34:47  sm
**      - Some logging cleanup
**      - New base class for b3CPU (b3CPUBase)
**      - b3Realloc bug fix on Solaris
**
**      Revision 1.31  2003/02/19 16:52:53  sm
**      - Cleaned up logging
**      - Clean up b3CPU/b3Runtime
**
**      Revision 1.30  2003/02/08 21:42:13  sm
**      - Removed some unused variables.
**
**      Revision 1.29  2003/01/26 11:53:26  sm
**      - Added support for Caligari object loading.
**
**      Revision 1.28  2002/12/20 15:32:55  sm
**      - Made some ICC optimazations :-)
**
**      Revision 1.27  2002/10/06 14:58:18  sm
**      - Done some finetuning on setup
**      - Output of GCC version under Un*x
**
**      Revision 1.26  2002/08/28 15:31:00  sm
**      - Added some help output to render and bimg3.
**
**      Revision 1.25  2002/08/21 10:16:40  sm
**      - Made some changes to the Un*x OpenGL renderer:
**        o Added animations
**        o Added camera switching
**        o Corrected far clipping plane computation.
**      - Configure script tidied up.
**
**      Revision 1.24  2002/08/09 08:00:32  sm
**      - Reimplemented b3Mem::b3Realloc and made checks.
**
**      Revision 1.23  2002/08/07 12:38:43  sm
**      - Modified exception definition. Exceptions are identified with
**        a three character code to unify error codes. This is necessary
**        to convert error codes into error messages inside applications.
**      - Added some additional b3Hash methods.
**      - Added -Wall compiler option to all C++ files.
**      - Removed some compiler warnings.
**
**      Revision 1.22  2002/08/01 15:02:56  sm
**      - Found texture missing bug when printing. There weren't any
**        selected textures inside an other OpenGL rendering context.
**        Now fixed!
**
**      Revision 1.21  2002/07/26 09:13:32  sm
**      - Found alpha problem: the Linux OpenGL renderer didn't use the
**        b3RenderContext::b3Init() method! Now everything function very well:-)
**      - The Un*x OpenGL renderer has got a key press interface now.
**      - Corrected spot lights
**      - Textures needn't to be square any more (some less memory usage)
**
**      Revision 1.20  2002/07/25 19:06:21  sm
**      - Why does not alpha channel function?
**
**      Revision 1.19  2002/07/21 17:02:36  sm
**      - Finished advanced color mix support (correct Phong/Mork shading)
**      - Added first texture mapping support. Further development on
**        Windows now...
**
**      Revision 1.18  2002/07/20 10:49:34  sm
**      - Added custom light support (not finished yet)
**      - Added b3Light::b3IsActive() for compatibility.
**      - Added texture search path support like in brt3.
**
**      Revision 1.17  2002/02/28 21:32:51  sm
**      - Corrected resource IDs because MFC used the same IDs
**        generated by Visual C++. This caused printing problems.
**
**      Revision 1.16  2002/02/08 15:53:38  sm
**      - Cleaned up makefiles for Un*x
**      - New dialog for print buffer size.
**
**      Revision 1.15  2001/11/09 16:15:35  sm
**      - Image file encoder
**      - Performance meter for triangles / second added.
**      - Corrected Windows b3TimeSpan computation
**
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

#if defined(BLZ3_USE_OPENGL) && defined(BLZ3_USE_GLUT)

static b3ShapeRenderContext  context;
static b3World              *world = null;
static b3RenderLight         lights;
static b3RenderView          view;
static b3_bool               all_lights = true;
static b3_bool               spot_light = true;
static b3_bool               double_buffered = true;
static b3_res                xWinSize,yWinSize;

static void b3SetLights()
{
	context.b3LightReset();
	if (all_lights)
	{
		b3PrintF(B3LOG_DEBUG,"Using multiple lights with%s spots...\n",
			spot_light ? "" : "out");

		lights.b3SetScene((b3Scene *)world->b3GetFirst());
		lights.b3SetLightMode(spot_light ? B3_LIGHT_SCENE_SPOT : B3_LIGHT_SCENE);
	}
	else
	{
		b3PrintF(B3LOG_DEBUG,"Using one light...\n");
		lights.b3SetLightMode(B3_LIGHT_SIMPLE);
	}
}

static void b3ReshapeFunc(GLsizei xSize,GLsizei ySize)
{
	b3PrintF(B3LOG_FULL,">b3ReshapeFunc(%d, %d );\n",xSize, ySize);
	view.b3SetupView(xWinSize = xSize,yWinSize = ySize);
	lights.b3SetupLight(&context);
	b3PrintF(B3LOG_FULL,"<b3ReshapeFunc()\n");
}

static void b3DisplayFunc()
{
	b3Scene  *scene;

	b3PrintF(B3LOG_FULL,">b3DisplayFunc()\n");
	context.b3StartDrawing();
	scene = (b3Scene *)world->b3GetFirst();
	scene->b3Draw(&context);

	glutSwapBuffers();
	b3PrintF(B3LOG_FULL,"<b3DisplayFunc()\n");
}

static void b3PlayAnimation()
{
	b3Scene     *scene;
	b3Animation *animation;

	scene = (b3Scene *)world->b3GetFirst();
	animation = scene->b3GetAnimation();
	if (animation == null)
	{
		return;
	}
	if (animation->m_Flags & ANIMF_ON)
	{
		b3Time    start,now,last;
		b3_vector lower,upper;
		b3_f64    t,span;
		b3_count  count = 0;
		b3_res    xSize,ySize;

		b3PrintF(B3LOG_FULL,"Playing animation...\n");
		last = start;
		scene->b3GetDisplaySize(xSize,ySize);
		do
		{
			now.b3Now();
			t = animation->m_Start + now - start;
			scene->b3SetAnimation(t);
			scene->b3ComputeBounds(&lower,&upper);
			context.b3StartDrawing();
			view.b3SetBounds(&lower,&upper);
			view.b3SetCamera(scene);
			b3ReshapeFunc(xWinSize,yWinSize);
			scene->b3Draw(&context);
			if (double_buffered)
			{
				glutSwapBuffers();
			}
			count++;
		}
		while(t < animation->m_End);
		
		span = now - start;
		b3PrintF(B3LOG_NORMAL,"Rendered %d frames in %3.2lf seconds with %3.3lf frames/sec.\n",
			count,span,(double)count / span);
	}
}

static void b3NextCamera(b3Scene *scene)
{
	b3CameraPart *camera,*act;

	act = scene->b3GetActualCamera();
	if (act != null)
	{
		camera = scene->b3GetNextCamera(act);
		if (camera == null)
		{
			// Take first camera
			camera = scene->b3GetFirstCamera(false);
		}
		if (camera != null)
		{
			b3PrintF(B3LOG_NORMAL,"Using camera %s\n",camera->b3GetName());
			scene->b3SetCamera(camera);
			view.b3SetCamera(scene);
			b3ReshapeFunc(xWinSize,yWinSize);
		}
	}
}

static void b3SetupRC()
{
	context.glBgColor.b3Init(0.7f,0.7f,1.0f);
	context.b3Init(double_buffered);
}

static void b3KeyboardFunc(unsigned char key,int x,int y)
{
	b3Scene  *scene;
	b3_bool   refresh = false;

	scene = (b3Scene *)world->b3GetFirst();
	switch (key)
	{
	case 'l':
		all_lights = !all_lights;
		b3SetLights();
		refresh = true;
		break;

	case 's':
		spot_light = !spot_light;
		b3SetLights();
		refresh = true;
		break;

	case 'a':
		scene->b3Activate();
		refresh = true;
		b3PrintF(B3LOG_DEBUG,"Activating all...\n");
		break;

	case 'e':
		scene->b3Activate(false);
		refresh = true;
		b3PrintF(B3LOG_DEBUG,"Deactivating all...\n");
		break;

	case 'c':
		b3Log::b3SetLevel(B3LOG_NORMAL);
		b3PrintF(B3LOG_NORMAL,"Normal logging...\n");
		break;

	case 'd':
		b3Log::b3SetLevel(B3LOG_DEBUG);
		b3PrintF(B3LOG_NORMAL,"Debug logging...\n");
		break;

	case 'f':
		b3Log::b3SetLevel(B3LOG_FULL);
		b3PrintF(B3LOG_NORMAL,"Full logging...\n");
		break;

	case 'p':
		b3PlayAnimation();
		break;

	case 'n':
		b3NextCamera(scene);
		refresh = true;
		break;

	case 'r':
		b3PrintF(B3LOG_FULL,"Refreshing...\n");
		refresh = true;
		break;

	case 27: // ESC
	case 'q':
	case 'x':
		b3PrintF(B3LOG_NORMAL,"Exit!\n");
		exit(EXIT_SUCCESS);
	}

	if (refresh)
	{
		b3ReshapeFunc(xWinSize,yWinSize);
		glutPostRedisplay();
	}
}

static void b3Update(b3Scene *scene)
{
	b3_vector       lower,upper;

	scene->b3SetupVertexMemory(&context);
	scene->b3ResetAnimation();
	scene->b3ComputeBounds(&lower,&upper);
	scene->b3Prepare();
			
	b3PrintF(B3LOG_NORMAL,"%d vertices\n", context.glVertexCount);
	b3PrintF(B3LOG_NORMAL,"%d triangles\n",context.glPolyCount);
	b3PrintF(B3LOG_NORMAL,"%d grids\n",    context.glGridCount);

	// Setup view
	view.b3SetBounds(&lower,&upper);
	view.b3SetCamera(scene);
	view.b3SetViewMode(B3_VIEW_3D);

	// then setup light(s)
	b3SetLights();

	glutPostRedisplay();
}

static void b3Prepare(b3Scene *scene)
{
	b3ModellerInfo *info;
	b3_res          xSize,ySize;

	scene->b3Reorg();
	scene->b3GetDisplaySize(xSize,ySize);
	scene->b3SetCamera(scene->b3GetFirstCamera(false));
	scene->b3PrepareScene(xSize,ySize);

	info = scene->b3GetModellerInfo();
	if (info != null)
	{
		all_lights = info->m_UseSceneLights;
	}

	xWinSize = xSize;
	yWinSize = ySize;
}

static void b3Banner(const char *command)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III OpenGL scene viewer\n");
	b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001, 2002, 2003, 2004\n");
	b3PrintF(B3LOG_NORMAL,"\n");
	if (command != null)
	{
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s [-d][-f][-v][-s] BWD-file\n", command);
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"  -d  debug level output\n");
		b3PrintF(B3LOG_NORMAL,"  -f  verbose level output\n");
		b3PrintF(B3LOG_NORMAL,"  -v  disable vertex buffer objects\n");
		b3PrintF(B3LOG_NORMAL,"  -s  draw with single buffer\n");
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
		b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
	}
}

int main(int argc,char *argv[])
{
	const char     *filename;
	b3Item         *item;
	b3Scene        *scene;
	char           *BLZ3_PLUGINS  = getenv("BLZ3_PLUGINS");
	char           *BLZ3_BIN      = getenv("BLZ3_BIN");
	char           *HOME = getenv("HOME");
	b3Path          textures;
	b3Path          pictures;
	b3Path          data;
	b3Loader        loader;
	b3_index        i;

	glutInit(&argc,argv);

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}

	for (i = 1;(i < argc) && (argv[i][0] == '-');i++)
	{
		switch(argv[i][1])
		{
		case 'v' :
			b3VectorBufferObjects::glAllowVBO = false;
			break;
		case 'd' :
			b3Log::b3SetLevel(B3LOG_DEBUG);
			break;
		case 'f' :
			b3Log::b3SetLevel(B3LOG_FULL);
			break;
		case 's' :
			double_buffered = false;
			break;
		}
	}

	if (i >= argc)
	{
		b3PrintF(B3LOG_NORMAL,"No filename given!\n");
		exit(EXIT_FAILURE);
	}

	try
	{
		b3Dir::b3LinkFileName(data,    HOME,"Blizzard/Data");
		b3Dir::b3LinkFileName(textures,HOME,"Blizzard/Textures");
		b3Dir::b3LinkFileName(pictures,HOME,"Blizzard/Pictures");
		b3Dir::b3LinkFileName(data,    HOME,"Blizzard/Data");

		b3Scene::m_TexturePool.b3AddPath(textures);
		b3Scene::m_TexturePool.b3AddPath(pictures);

		filename = (const char *)argv[i];
		world = new b3World();
		world->b3AddPath(data);

		b3RaytracingItems::b3Register();
		if (BLZ3_BIN != null)
		{
			loader.b3AddPath(BLZ3_BIN);
		}
		if (BLZ3_PLUGINS != null)
		{
			loader.b3AddPath(BLZ3_PLUGINS);
		}
		loader.b3Load();

		world->b3Read(filename);
		for (item  = world->b3GetFirst();
		     item != null;
		     item  = scene->Succ)
		{
			scene = (b3Scene *)item;
			b3Prepare(scene);

			glutInitDisplayMode((double_buffered ? GLUT_DOUBLE : 0)|GLUT_RGBA|GLUT_DEPTH);
			glutInitWindowSize(xWinSize,yWinSize);
			glutCreateWindow(filename);
			glutDisplayFunc (&b3DisplayFunc);
			glutKeyboardFunc(&b3KeyboardFunc);
			glutReshapeFunc (&b3ReshapeFunc);

			b3SetupRC();
			b3Update (scene);
			glutMainLoop();
		}
		delete world;
	}
	catch(b3ExceptionBase &e)
	{
		b3PrintF(B3LOG_NORMAL,"Error parsing %s\n",argv[1]);
		b3PrintF(B3LOG_NORMAL,"Error code: %d\n",e.b3GetError());
		b3PrintF(B3LOG_NORMAL,"Error msg:  %s\n",e.b3GetErrorMsg());
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"Unknown error occured processing %s\n",argv[1]);
	}
	return EXIT_SUCCESS;
}

#else

int main(int argc,char *argv[])
{
	b3PrintF(B3LOG_NORMAL,"This platform has got no OpenGL support!\n");

	return EXIT_SUCCESS;
}

#endif
