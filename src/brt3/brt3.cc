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
#include "blz3/system/b3DisplayView.h"
#include "blz3/system/b3Dir.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.37  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**
**	Revision 1.36  2002/12/20 15:32:55  sm
**	- Made some ICC optimazations :-)
**	
**	Revision 1.35  2002/12/11 14:47:58  sm
**	- Changed noise handling to static
**	- Fixed some error cases when image not loaded.
**	- Added some brt3 flags
**	
**	Revision 1.34  2002/12/10 20:14:59  sm
**	- Added some new brt3 features:
**	  o no wait after display output
**	  o disable animation
**	- Fixed some image exceptions
**	
**	Revision 1.33  2002/10/06 14:58:18  sm
**	- Done some finetuning on setup
**	- Output of GCC version under Un*x
**	
**	Revision 1.32  2002/08/24 13:22:02  sm
**	- Extensive debugging on threading code done!
**	  o Cleaned up POSIX threads
**	  o Made safe thread handling available in raytracing code
**	  o b3PrepareInfo instantiates threads only once.
**	- Added new thread options to gcc: "-D_REENTRAND -pthread"
**	  which I only can assume what they are doing;-)
**	- Time window in motion blur moved from [-0.5,0.5] to [0,1]
**	  and corrected upper time limit.
**	
**	Revision 1.31  2002/08/23 15:34:27  sm
**	- Added time support to water animation.
**	- Added multiple file format types to brt3.
**	
**	Revision 1.30  2002/08/23 11:35:23  sm
**	- Added motion blur raytracing. The image creation looks very
**	  nice! The algorithm is not as efficient as it could be.
**	
**	Revision 1.29  2002/08/22 14:06:32  sm
**	- Corrected filter support and added test suite.
**	- Added animation computing to brt3. Now we are near to
**	  real time raytracing: 8 fps for Animationtest.
**	
**	Revision 1.28  2002/08/21 20:13:32  sm
**	- Introduced distributed raytracing with all sampling methods
**	  and filter computations. This made some class movements
**	  inside files necessary. The next step would be to integrate
**	  motion blur.
**	
**	Revision 1.27  2002/08/16 13:20:13  sm
**	- Removed some unused methods.
**	- Allocation bug found in brt3 - the Un*x version of the
**	  Blizzard III raytracer: It's necessary to use b3ShapeRenderContext
**	  rather than b3renderContext which doesn't initialize subdivision
**	  for shapes.
**	
**	Revision 1.26  2002/08/16 11:40:38  sm
**	- Changed vertex handling for use without OpenGL. Vertex computation
**	  is needed for bound computation which is needed for animation. There
**	  are still some problems so we have to work further on Windows for
**	  better debugging.
**	- b3ExtractExt searches from right instead from left.
**	
**	Revision 1.25  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.24  2002/08/05 17:42:58  sm
**	- Displaying brt3 options.
**	- Clearing XBuffer which displayed garbage from previous X applications
**	  (This is a security leak inside X!!!)
**	
**	Revision 1.23  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**	
**	Revision 1.22  2002/07/20 10:49:34  sm
**	- Added custom light support (not finished yet)
**	- Added b3Light::b3IsActive() for compatibility.
**	- Added texture search path support like in brt3.
**	
**	Revision 1.21  2002/05/08 14:44:46  sm
**	- Insert Tag names
**	
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

static char BLZ3_EXTENSION[] = ".tga";

static void b3SaveRaytracedImage(
	b3Display  *display,
	const char *picture_home,
	const char *camera_name)
{
	b3Path imagename;
	b3Path filename;

	filename.b3RemoveExt(camera_name);
	if (picture_home != null)
	{
		imagename.b3LinkFileName(picture_home,filename);
		strcat((char *)imagename,BLZ3_EXTENSION);
	}
	else
	{	
		sprintf((char *)imagename,"%s%s",
			(const char *)filename,BLZ3_EXTENSION);
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
	b3ShapeRenderContext  context;
	b3World              *world;
	b3Display            *display;
	b3Scene              *scene;
	b3Animation          *animation;
	b3CameraPart         *camera;
	b3Item               *item;
	b3_vector             lower,upper;
	char                 *picture_home = getenv("BLZ3_PICTURES");
	char                 *HOME         = getenv("HOME");
	b3Path                textures;
	b3Path                pictures;
	b3Path                data;
	b3Path                camera_name;
	b3_bool               force_no_anim    = false;
	b3_bool               force_no_display = false;
	b3_bool               force_no_wait    = false;
	b3_index              i;

	if (argc > 1)
	{
		b3InitRaytrace::b3Init();

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
				case 'd' :
					b3Log::b3SetLevel(B3LOG_DEBUG);
					break;
				case 'f' :
					b3Log::b3SetLevel(B3LOG_FULL);
					break;
				case 'a' :
					force_no_anim = true;
					b3PrintF(B3LOG_NORMAL,"Forcing no animation\n");
					break;
				case 'n' :
					force_no_display = true;
					b3PrintF(B3LOG_NORMAL,"Forcing no display output\n");
					break;
				case 'w' :
					force_no_wait = true;
					b3PrintF(B3LOG_NORMAL,"Forcing no wait after display output\n");
					break;
				case 'i':
					strcpy(BLZ3_EXTENSION,".tif");
					break;
				case 'j':
					strcpy(BLZ3_EXTENSION,".jpg");
					break;
				case 'g':
					strcpy(BLZ3_EXTENSION,".tga");
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
						scene->b3Reorg();
						scene->b3AllocVertices(&context);
						scene->b3SetFilename(argv[i]);
						animation = scene->b3GetAnimation();
						if (animation != null)
						{
							if ((!animation->b3IsActive()) || (force_no_anim))
							{
								b3PrintF(B3LOG_DEBUG,"Animation deactivated...\n");
								animation = null;
							}
							else
							{
								b3PrintF(B3LOG_DEBUG,"Using animation...\n");
							}
						}

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
									if (animation != null)
									{
										b3_f64   t,step;
										b3Path   img_name;
										b3_count count = 0;

										scene->b3ResetAnimation();
										step = 1.0 / animation->m_FramesPerSecond;
										for (t = animation->m_Start;t < animation->m_End;t += step)
										{
											scene->b3SetAnimation(t);
											scene->b3Raytrace(display);
											sprintf((char *)img_name,"%s_%04ld",
												camera->b3GetName(),count++);
											b3SaveRaytracedImage(
												display,
												picture_home,img_name);
										}
									}
									else
									{
										scene->b3ComputeBounds(&lower,&upper);
										scene->b3Raytrace(display);
										b3SaveRaytracedImage(
											display,
											picture_home,camera->b3GetName());
										}
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
							// Default camera without any camera
							// in special list
							scene->b3ComputeBounds(&lower,&upper);
							scene->b3Raytrace(display);
							b3SaveRaytracedImage(
								display,
								picture_home,scene->b3GetName());
						}

						if (!force_no_wait)
						{
							display->b3Wait();
						}
						delete display;
					}
				}
				catch(b3WorldException &w)
				{
					b3PrintF(B3LOG_NORMAL,"Error parsing %s\n",argv[i]);
					b3PrintF(B3LOG_NORMAL,"Error code: %d\n",w.b3GetError());
					b3PrintF(B3LOG_NORMAL,"Error msg:  %s\n",w.b3GetErrorMsg());
				}
				catch(b3FileException &f)
				{
					b3PrintF(B3LOG_NORMAL,"File IO error using %s\n",argv[i]);
					b3PrintF(B3LOG_NORMAL,"Error code: %d\n",f.b3GetError());
					b3PrintF(B3LOG_NORMAL,"Error msg:  %s\n",f.b3GetErrorMsg());
				}
				catch(b3TxException &t)
				{
					b3PrintF(B3LOG_NORMAL,"Image error using %s\n",argv[i]);
					b3PrintF(B3LOG_NORMAL,"Error code: %d\n",t.b3GetError());
					b3PrintF(B3LOG_NORMAL,"Error msg:  %s\n",t.b3GetErrorMsg());
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
		b3PrintF(B3LOG_NORMAL,"%s [-d][-f][-n][-j][-i][-g] {Blizzard World Data files}\n",argv[0]);
		b3PrintF(B3LOG_NORMAL,"  -d  debug level output\n");
		b3PrintF(B3LOG_NORMAL,"  -f  verbose level output\n");
		b3PrintF(B3LOG_NORMAL,"  -a  disable animation\n");
		b3PrintF(B3LOG_NORMAL,"  -n  disable display\n");
		b3PrintF(B3LOG_NORMAL,"  -w  nowait after display output\n");
		b3PrintF(B3LOG_NORMAL,"  -g  TGA image saving (default)\n");
		b3PrintF(B3LOG_NORMAL,"  -i  TIFF image saving\n");
		b3PrintF(B3LOG_NORMAL,"  -j  JPEG image saving\n");
		b3PrintF(B3LOG_NORMAL,"\n");
		b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
		b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
	}
	return 0;
}
