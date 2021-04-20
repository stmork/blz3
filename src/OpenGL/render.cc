/*
**
**	$Filename:	render.cc $
**	$Release:	Dortmund 2001-2007 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Show scene with OpenGL
**
**      (C) Copyright 2001-2007  Steffen A. Mork
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
**                        Implementation                                **
**                                                                      **
*************************************************************************/

#if defined(BLZ3_USE_OPENGL) && defined(BLZ3_USE_GLUT)

static b3ShapeRenderContext  context;
static b3World       *       world = nullptr;
static b3RenderLight         lights;
static b3RenderView          view;
static bool                  all_lights = true;
static bool                  spot_light = true;
static bool                  double_buffered = true;
static b3_res                xWinSize, yWinSize;

static void b3SetLights()
{
	context.b3LightReset();
	if (all_lights)
	{
		b3PrintF(B3LOG_DEBUG, "Using multiple lights with%s spots...\n",
			spot_light ? "" : "out");

		lights.b3SetScene((b3Scene *)world->b3GetFirst());
		lights.b3SetLightMode(spot_light ? B3_LIGHT_SCENE_SPOT : B3_LIGHT_SCENE);
	}
	else
	{
		b3PrintF(B3LOG_DEBUG, "Using one light...\n");
		lights.b3SetLightMode(B3_LIGHT_SIMPLE);
	}
}

static void b3ReshapeFunc(GLsizei xSize, GLsizei ySize)
{
	b3PrintF(B3LOG_FULL, ">b3ReshapeFunc(%d, %d );\n", xSize, ySize);
	view.b3SetupView(xWinSize = xSize, yWinSize = ySize);
	lights.b3SetupLight(&context);
	b3PrintF(B3LOG_FULL, "<b3ReshapeFunc()\n");
}

static void b3DisplayFunc()
{
	b3Scene * scene;

	b3PrintF(B3LOG_FULL, ">b3DisplayFunc()\n");
	context.b3StartDrawing();
	scene = (b3Scene *)world->b3GetFirst();
	scene->b3Draw(&context);

	glutSwapBuffers();
	b3PrintF(B3LOG_FULL, "<b3DisplayFunc()\n");
}

static void b3PlayAnimation()
{
	b3Scene   *   scene;
	b3Animation * animation;

	scene     = (b3Scene *)world->b3GetFirst();
	animation = scene->b3GetAnimation();
	if (animation == nullptr)
	{
		return;
	}
	if (animation->m_Flags & ANIMF_ON)
	{
		b3Time    start, now, last;
		b3_vector lower, upper;
		b3_f64    t, span;
		b3_count  count = 0;
		b3_res    xSize, ySize;

		b3PrintF(B3LOG_FULL, "Playing animation...\n");
		last = start;
		scene->b3GetDisplaySize(xSize, ySize);
		do
		{
			now.b3Now();
			t = animation->m_Start + now - start;
			scene->b3SetAnimation(t);
			scene->b3ComputeBounds(&lower, &upper);
			context.b3StartDrawing();
			view.b3SetBounds(&lower, &upper);
			view.b3SetCamera(scene);
			b3ReshapeFunc(xWinSize, yWinSize);
			scene->b3Draw(&context);
			if (double_buffered)
			{
				glutSwapBuffers();
			}
			count++;
		}
		while (t < animation->m_End);

		span = now - start;
		b3PrintF(B3LOG_NORMAL, "Rendered %d frames in %3.2lf seconds with %3.3lf frames/sec.\n",
			count, span, (double)count / span);
	}
}

static void b3NextCamera(b3Scene * scene)
{
	b3CameraPart * camera, *act;

	act = scene->b3GetActualCamera();
	if (act != nullptr)
	{
		camera = scene->b3GetNextCamera(act);
		if (camera == nullptr)
		{
			// Take first camera
			camera = scene->b3GetFirstCamera(false);
		}
		if (camera != nullptr)
		{
			b3PrintF(B3LOG_NORMAL, "Using camera %s\n", camera->b3GetName());
			scene->b3SetCamera(camera);
			view.b3SetCamera(scene);
			b3ReshapeFunc(xWinSize, yWinSize);
		}
	}
}

static void b3SetupRC()
{
	context.glBgColor.b3Init(0.7f, 0.7f, 1.0f);
	context.b3Init(double_buffered);
}

static void b3KeyboardFunc(unsigned char key, int x B3_UNUSED, int y B3_UNUSED)
{
	b3Scene * scene;
	bool      refresh = false;

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
		b3PrintF(B3LOG_DEBUG, "Activating all...\n");
		break;

	case 'e':
		scene->b3Activate(false);
		refresh = true;
		b3PrintF(B3LOG_DEBUG, "Deactivating all...\n");
		break;

	case 'c':
		b3Log::b3SetLevel(B3LOG_NORMAL);
		b3PrintF(B3LOG_NORMAL, "Normal logging...\n");
		break;

	case 'd':
		b3Log::b3SetLevel(B3LOG_DEBUG);
		b3PrintF(B3LOG_NORMAL, "Debug logging...\n");
		break;

	case 'f':
		b3Log::b3SetLevel(B3LOG_FULL);
		b3PrintF(B3LOG_NORMAL, "Full logging...\n");
		break;

	case 'p':
		b3PlayAnimation();
		break;

	case 'n':
		b3NextCamera(scene);
		refresh = true;
		break;

	case 'r':
		b3PrintF(B3LOG_FULL, "Refreshing...\n");
		refresh = true;
		break;

	case 27: // ESC
	case 'q':
	case 'x':
		b3PrintF(B3LOG_NORMAL, "Exit!\n");
		exit(EXIT_SUCCESS);
	}

	if (refresh)
	{
		b3ReshapeFunc(xWinSize, yWinSize);
		glutPostRedisplay();
	}
}

static void b3Update(b3Scene * scene)
{
	b3_vector       lower, upper;

	scene->b3SetupVertexMemory(&context);
	scene->b3ResetAnimation();
	scene->b3ComputeBounds(&lower, &upper);

	b3PrintF(B3LOG_NORMAL, "%d vertices\n",  context.glVertexCount);
	b3PrintF(B3LOG_NORMAL, "%d triangles\n", context.glPolyCount);
	b3PrintF(B3LOG_NORMAL, "%d grids\n",     context.glGridCount);

	// Setup view
	view.b3SetBounds(&lower, &upper);
	view.b3SetCamera(scene);
	view.b3SetViewMode(B3_VIEW_3D);

	// then setup light(s)
	b3SetLights();

	glutPostRedisplay();
}

static void b3Prepare(b3Scene * scene)
{
	const b3ModellerInfo * info = scene->b3GetModellerInfo();
	b3_res                 xSize, ySize;

	scene->b3Reorg();
	scene->b3GetDisplaySize(xSize, ySize);
	scene->b3PrepareScene(xSize, ySize);
	scene->b3SetCamera(scene->b3GetFirstCamera(false));

	all_lights = info->m_UseSceneLights;

	xWinSize = xSize;
	yWinSize = ySize;
}

static void b3Banner(const char * command)
{
	b3PrintF(B3LOG_NORMAL, "Blizzard III OpenGL scene viewer\n");
	b3PrintF(B3LOG_NORMAL, "Copyright (C) Steffen A. Mork  2001-2021\n");
	b3PrintF(B3LOG_NORMAL, "\n");
	if (command != nullptr)
	{
		b3PrintF(B3LOG_NORMAL, "USAGE:\n");
		b3PrintF(B3LOG_NORMAL, "%s [-d][-f][-v][-s] BWD-file\n", command);
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "  -d  debug level output\n");
		b3PrintF(B3LOG_NORMAL, "  -f  verbose level output\n");
		b3PrintF(B3LOG_NORMAL, "  -v  disable vertex buffer objects\n");
		b3PrintF(B3LOG_NORMAL, "  -s  draw with single buffer\n");
		b3PrintF(B3LOG_NORMAL, "\n");
		b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n", __DATE__, __TIME__);
		b3PrintF(B3LOG_NORMAL, "%s\n", b3Runtime::b3GetCompiler());
	}
}

int main(int argc, char * argv[])
{
	const char   *  filename;
	b3Item     *    item;
	b3Scene    *    scene;
	char      *     BLZ3_PLUGINS = getenv("BLZ3_PLUGINS");
	char      *     BLZ3_BIN     = getenv("BLZ3_BIN");
	char      *     HOME         = getenv("HOME");
	b3Path          textures;
	b3Path          pictures;
	b3Path          data;
	b3Loader        loader;
	b3_index        i;

	if (argc <= 1)
	{
		b3Banner(argv[0]);
		exit(EXIT_SUCCESS);
	}
	glutInit(&argc, argv);

	for (i = 1; (i < argc) && (argv[i][0] == '-'); i++)
	{
		switch (argv[i][1])
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
		b3PrintF(B3LOG_NORMAL, "No filename given!\n");
		exit(EXIT_FAILURE);
	}

	try
	{
		b3Dir::b3LinkFileName(data,     HOME, "Blizzard/Data");
		b3Dir::b3LinkFileName(textures, HOME, "Blizzard/Textures");
		b3Dir::b3LinkFileName(pictures, HOME, "Blizzard/Pictures");

		b3Scene::m_TexturePool.b3AddPath(textures);
		b3Scene::m_TexturePool.b3AddPath(pictures);

		filename = (const char *)argv[i];
		world = new b3World();
		world->b3AddPath(data);

		b3RaytracingItems::b3Register();
		if (BLZ3_BIN != nullptr)
		{
			loader.b3AddPath(BLZ3_BIN);
		}
		if (BLZ3_PLUGINS != nullptr)
		{
			loader.b3AddPath(BLZ3_PLUGINS);
		}
		loader.b3Load();

		world->b3Read(filename);
		for (item  = world->b3GetFirst();
			item != nullptr;
			item  = scene->Succ)
		{
			scene = (b3Scene *)item;
			b3Prepare(scene);

			//			glutInit(&argc, argv);
			glutInitDisplayMode((double_buffered ? GLUT_DOUBLE : 0) | GLUT_RGBA | GLUT_DEPTH);
			glutInitWindowSize(xWinSize, yWinSize);
			glutCreateWindow(filename);
			glutDisplayFunc(&b3DisplayFunc);
			glutKeyboardFunc(&b3KeyboardFunc);
			glutReshapeFunc(&b3ReshapeFunc);

			b3SetupRC();
			b3Update(scene);
			glutMainLoop();
		}
		delete world;
	}
	catch (b3ExceptionBase & e)
	{
		b3PrintF(B3LOG_NORMAL, "Error parsing %s\n", argv[1]);
		b3PrintF(B3LOG_NORMAL, "Error code: %d\n", e.b3GetError());
		b3PrintF(B3LOG_NORMAL, "Error msg:  %s\n", e.b3GetErrorMsg());
	}
	catch (...)
	{
		b3PrintF(B3LOG_NORMAL, "Unknown error occured processing %s\n", argv[1]);
	}
	return EXIT_SUCCESS;
}

#else

int main(int argc, char * argv[])
{
	b3PrintF(B3LOG_NORMAL, "This platform has got no OpenGL support!\n");

	return EXIT_SUCCESS;
}

#endif
