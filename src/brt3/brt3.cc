/*
**
**	$Filename:	brt3.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#define no_DEBUG_VIEW

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/b3Config.h" 
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3Dir.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.20  2002/05/08 14:37:56  sm
**	- Output some information when not specifying data files.
**
**	Revision 1.19  2002/02/01 15:49:23  sm
**	- Added "force no display" for remote Un*x raytracing
**	- Converted BWDs to binary
**	
**	Revision 1.18  2002/01/22 17:11:17  sm
**	- brt3 is now able to save images. The selection of image type
**	  is unsoved yet.
**	- Better b3DisplayView in Un*x port.
**	- Fixed stricmp() in Un*x port.
**	
**	Revision 1.17  2002/01/01 13:50:21  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.16  2001/12/30 22:52:35  sm
**	- Made b3Scene::b3SetCamera() compatible to earlier versions.
**	
**	Revision 1.15  2001/12/30 14:16:57  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.14  2001/12/08 21:37:38  sm
**	- Added "No Gfx" support
**	
**	Revision 1.13  2001/11/07 15:55:09  sm
**	- Introducing b3TimeSpan to Windows to get computation time on
**	  Windows as well.
**	- Changed some include dependencies.
**	
**	Revision 1.12  2001/11/04 12:15:15  sm
**	- Renaming some attributes...
**	- Taking account to redesign of b3Display
**	
**	Revision 1.11  2001/11/03 16:43:04  sm
**	- Fixed brt3
**	
**	Revision 1.9  2001/11/02 19:05:36  sm
**	- Introducing time mearuring.
**	- Fixed wrong lens flare handling.
**	
**	Revision 1.8  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.7  2001/10/29 19:34:02  sm
**	- Added new define B3_DELETE_BASE.
**	- Added support to abort raytrace processing.
**	- Added search path to world loading.
**	- Fixed super sampling.
**	- Fixed memory leak in raytracing row processing.
**	
**	Revision 1.6  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
**	
**	Revision 1.5  2001/10/10 17:52:24  sm
**	- Texture loading (only reading into memory) running.
**	- Raytracing without OpenGL must be possible!
**	
**	Revision 1.4  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.3  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**	Revision 1.2  2001/09/30 15:46:07  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**	Revision 1.1  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**
*/

#define BLZ3_EXTENSION ".tga"

static char cvs_tag[] = "$Name$";

static void b3SaveRaytracedImage(
	b3Display  *display,
	const char *picture_home,
	const char *camera_name)
{
	b3Path imagename;

	if (picture_home != null)
	{
		imagename.b3LinkFileName(picture_home,camera_name);
		strcat((char *)imagename,BLZ3_EXTENSION);
	}
	else
	{	
		sprintf((char *)imagename,"%s%s",
			camera_name,BLZ3_EXTENSION);
	}
	display->b3SaveImage(imagename);
}

static b3Display *b3AllocDisplay(b3Scene *scene,b3_bool force_no_display)
{
	b3Display *display;
	b3_res     xSize,ySize;

#ifdef DEBUG_VIEW
	// Show a small display in every case
	display = new b3DisplayView(200,150);
#else
	if (scene->b3GetDisplaySize(xSize,ySize))
	{
		if ((scene->m_Flags & TP_NO_GFX) || force_no_display)
		{
			display = new b3Display(xSize,ySize);
		}
		else
		{
			display = new b3DisplayView(xSize,ySize);
		}
	}
	else
	{
		if ((scene->m_Flags & TP_NO_GFX) || force_no_display)
		{
			display = new b3Display();
		}
		else
		{
			display = new b3DisplayView();
		}
	}
#endif
	return display;
}

int main(int argc,char *argv[])
{
	b3Item       *item;
	b3World      *world;
	b3Scene      *scene;
	b3CameraPart *camera;
	b3Display    *display;
	char         *picture_home = getenv("BLZ3_PICTURES");
	b3_index      i;
	char         *HOME = getenv("HOME");
	b3Path        textures;
	b3Path        pictures;
	b3Path        data;
	b3Path        camera_name;
	b3_bool       force_no_display = false;

	if (argc > 1)
	{
		b3InitRaytrace::b3Init();

//		b3Log_SetLevel(B3LOG_NORMAL);
//		b3Log_SetLevel(B3LOG_DEBUG);
//		b3Log_SetLevel(B3LOG_FULL);
		b3Dir::b3LinkFileName(textures,HOME,"Blizzard/Textures");
		b3Dir::b3LinkFileName(pictures,HOME,"Blizzard/Pictures");
		b3Dir::b3LinkFileName(data,    HOME,"Blizzard/Data");
		texture_pool.b3AddPath(textures);
		texture_pool.b3AddPath(pictures);
		world = new b3World();
		world->b3AddPath(data);

		for (i = 1;i < argc;i++)
		{
			if (argv[i][0] == '-')
			{
				switch(argv[i][1])
				{
				case 'n' :
					force_no_display = true;
					b3PrintF(B3LOG_NORMAL,"Forcing no display output\n");
					break;
				case 'v' :
					b3PrintF(B3LOG_NORMAL,"Blizzard III Raytracing software\n");
					break;
				}
			}
			else
			{
				try
				{
					world->b3Read(argv[i]);
					for (item  = world->b3GetFirst();
					     item != null;
					     item  = scene->Succ)
					{
						scene = (b3Scene *)item;
#ifdef BLZ3_USE_OPENGL
						scene->b3Reorg();
#endif
						scene->b3SetFilename(argv[i]);

						display = b3AllocDisplay(scene,force_no_display);
						if ((camera = scene->b3GetCamera(false)) != null)
						{
							do
							{
								scene->b3GetTitle(camera_name);
								if (camera->m_Flags & CAMERA_ACTIVE)
								{
								b3PrintF(B3LOG_NORMAL,"Rendering \"%s\"...\n",
										camera->m_CameraName);
									scene->b3SetCamera(camera);
									scene->b3Raytrace(display);
									b3SaveRaytracedImage(
										display,
										picture_home,camera->b3GetName());
								}
								else
								{
									b3PrintF(B3LOG_NORMAL,"Skipping \"%s\"...\n",
										camera->m_CameraName);
								}
								camera = scene->b3GetNextCamera(camera);
							}
							while (camera != null);
						}
						else
						{
							scene->b3Raytrace(display);
							b3SaveRaytracedImage(
							display,
							picture_home,scene->b3GetName());
						}
	
						display->b3Wait();
						delete display;
					}
				}
				catch(b3WorldException *w)
				{
					b3PrintF(B3LOG_NORMAL,"Error parsing %s\n",argv[i]);
					b3PrintF(B3LOG_NORMAL,"Error code: %d\n",w->b3GetError());
				}
				catch(b3FileException *f)
				{
					b3PrintF(B3LOG_NORMAL,"File IO error using %s\n",argv[i]);
					b3PrintF(B3LOG_NORMAL,"Error code: %d\n",f->b3GetError());
				}
				catch(...)
				{
					b3PrintF(B3LOG_NORMAL,"Unknown error occured loading %s\n",argv[i]);
				}
			}
		}
		delete world;
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"Blizzard III Raytracer\n");
		b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001, 2002\n");
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s {Bllizzard World Data files}\n",argv[0]);
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
		b3PrintF(B3LOG_NORMAL,"Compile name: %s\n",cvs_tag);
	}
	return 0;
}
