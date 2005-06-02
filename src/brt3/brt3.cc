/*
**
**	$Filename:	brt3.cc $
**	$Release:	Dortmund 2001, 2002, 2003 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001, 2002, 2003, 2004  Steffen A. Mork
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
#include "blz3/system/b3Plugin.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.63  2005/06/02 07:45:44  smork
**	- Fixed RGB8 image saving in brt3.
**	- Added PostScript image save.
**
**	Revision 1.62  2005/01/21 10:28:59  smork
**	- Corrected exe banner.
**	
**	Revision 1.61  2004/12/30 16:27:39  sm
**	- Removed assertion problem when starting Lines III: The
**	  image list were initialized twice due to double calling
**	  OnInitDialog() of CDialogBar. The CDialogBar::Create()
**	  calls OnInitDialog() automatically sinde MFC 7
**	- Removed many global references from raytrace and base lib
**	- Fixed ticket no. 29: The b3RenderObject::b3Recompute
**	  method checks the vertex maintainer against a null pointer.
**	
**	Revision 1.60  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.59  2004/11/07 12:20:56  sm
**	- Added support for rendering priority. The brt3 command
**	  uses the BLZ3_RENDER_PRIO environment variable for
**	  setting the priority. Valid range is from -2 to 2 where
**	  only root can use 1 to 2.
**	
**	Revision 1.58  2004/09/28 18:03:19  sm
**	- Search path correction: Home directory before environment.
**	
**	Revision 1.57  2004/09/23 09:31:33  sm
**	- Changed b3Runtime into a real singleton.
**	- Added functions for OpenGL extension.
**	- Removed b3PrintF()s from singletons.
**	
**	Revision 1.56  2004/05/31 08:42:58  sm
**	- Changed animation from time point to frame index which
**	  is more accurate.
**	
**	Revision 1.55  2004/05/30 20:25:00  sm
**	- Set paging size in supersampling dialog to 1 instead of 10.
**	- Added support for debugging super sampling.
**	- The object preview uses the shading model of its owning world.
**	- Fixed animation problem when using rotating elements on
**	  time bounds because of rounding problems. Now using
**	  b3_f32 for time points.
**	
**	Revision 1.54  2004/05/16 09:21:50  sm
**	- Corrected camera access methods.
**	
**	Revision 1.53  2004/03/02 09:07:17  sm
**	- Added read/write support for Cook/Torrance material.
**	- Added test module for Cook/Torrance reflection model.
**	- Fixed camera name handling if camera name is empty.
**	
**	Revision 1.52  2004/01/18 13:51:57  sm
**	- Done further security issues.
**	
**	Revision 1.51  2004/01/05 08:01:55  sm
**	- Added new year.
**	
**	Revision 1.50  2004/01/01 12:46:43  sm
**	- Fixed RGB8 image writing
**	- Added RGB8 image saving to brt3 and display all command line options.
**	
**	Revision 1.49  2003/08/28 14:44:26  sm
**	- Further buffer overflow prevention:
**	  o added b3Path::b3Format
**	  o added b3Path::b3Append
**	- Further strcat/strcpy removal necessary
**	
**	Revision 1.48  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**	
**	Revision 1.47  2003/08/11 08:21:40  sm
**	- Added priority scheduling to b3Thread class.
**	- Cleaned up brt3 comments.
**	
**	Revision 1.46  2003/07/26 14:03:14  sm
**	- Fixed ICC version: The b3Vector classes computed a wrong value
**	  in b3Length() because of using the uninitialized fourth vector
**	  component.
**	
**	Revision 1.45  2003/07/20 10:18:35  sm
**	- Banner compilation problem fixed.
**	
**	Revision 1.44  2003/07/20 10:11:55  sm
**	- Added banner on -v command line option.
**	
**	Revision 1.43  2003/07/20 09:38:30  sm
**	- Registering raytracing items manually.
**	
**	Revision 1.42  2003/07/12 17:44:47  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.41  2003/07/09 10:09:38  sm
**	- Changed brt3's default image file format to JPEG
**	- Increased default quality of JPEG images from 75 to 85
**	- Added house creating support ;-)
**	
**	Revision 1.40  2003/05/24 16:37:06  sm
**	- Added plugin support for Un*x
**	
**	Revision 1.39  2003/02/22 19:39:34  sm
**	- Fixed some GCC compile errors in b3TIFF stuff.
**	
**	Revision 1.38  2003/02/20 16:34:47  sm
**	- Some logging cleanup
**	- New base class for b3CPU (b3CPUBase)
**	- b3Realloc bug fix on Solaris
**	
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

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

static char BLZ3_EXTENSION[16] = ".jpg";

static b3_bool b3SaveRaytracedImage(
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
		imagename.b3Append(BLZ3_EXTENSION);
	}
	else
	{
		imagename.b3Format("%s%s",(const char *)filename,BLZ3_EXTENSION);
	}
	return display->b3SaveImage(imagename);
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

static void b3Banner(const char *command)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III Raytracer\n");
	b3PrintF(B3LOG_NORMAL,"Copyright (C) Steffen A. Mork  2001, 2002, 2003, 2004, 2005\n");
	b3PrintF(B3LOG_NORMAL,"\n");
	if (command != null)
	{
		b3PrintF(B3LOG_NORMAL,"USAGE:\n");
		b3PrintF(B3LOG_NORMAL,"%s [-d][-f][-a][-n][-w][-j][-i][-g][-r] {Blizzard World Data files}\n",command);
		b3PrintF(B3LOG_NORMAL,"  -d  debug level output\n");
		b3PrintF(B3LOG_NORMAL,"  -f  verbose level output\n");
		b3PrintF(B3LOG_NORMAL,"  -a  disable animation\n");
		b3PrintF(B3LOG_NORMAL,"  -n  disable display\n");
		b3PrintF(B3LOG_NORMAL,"  -w  nowait after display output\n");
		b3PrintF(B3LOG_NORMAL,"  -g  TGA image saving\n");
		b3PrintF(B3LOG_NORMAL,"  -i  TIFF image saving\n");
		b3PrintF(B3LOG_NORMAL,"  -j  JPEG image saving (default)\n");
		b3PrintF(B3LOG_NORMAL,"  -r  RGB8 image saving\n");
		b3PrintF(B3LOG_NORMAL,"  -p  grey PostScript image saving\n");
		b3PrintF(B3LOG_NORMAL,"\n");
	}
	b3PrintF(B3LOG_NORMAL,"Compile date: %s %s\n",__DATE__,__TIME__);
	b3PrintF(B3LOG_NORMAL,"%s\n",b3Runtime::b3GetCompiler());
}

int main(int argc,char *argv[])
{
	b3ShapeRenderContext  context; // ???
	b3World              *world;
	b3Display            *display;
	b3Scene              *scene;
	b3Animation          *animation;
	b3CameraPart         *camera;
	b3Item               *item;
	b3_vector             lower,upper;
	char                 *BLZ3_DATA        = getenv("BLZ3_DATA");
	char                 *BLZ3_TEXTURES    = getenv("BLZ3_TEXTURES");
	char                 *BLZ3_PICTURES    = getenv("BLZ3_PICTURES");
	char                 *BLZ3_PLUGINS     = getenv("BLZ3_PLUGINS");
	char                 *BLZ3_BIN         = getenv("BLZ3_BIN");
	char                 *BLZ3_RENDER_PRIO = getenv("BLZ3_RENDER_PRIO");
	char                 *HOME             = getenv("HOME");
	b3Path                textures;
	b3Path                pictures;
	b3Path                data;
	b3Loader              loader;
	b3_bool               force_no_anim    = false;
	b3_bool               force_no_display = false;
	b3_bool               force_no_wait    = false;
	b3_index              i;

	if (argc > 1)
	{
		world = new b3World();
		if (HOME != null)
		{
			b3Dir::b3LinkFileName(textures,HOME,"Blizzard/Textures");
			b3Dir::b3LinkFileName(pictures,HOME,"Blizzard/Pictures");
			b3Dir::b3LinkFileName(data,    HOME,"Blizzard/Data");
			b3Scene::m_TexturePool.b3AddPath(textures);
			if (BLZ3_TEXTURES != null)
			{
				b3Scene::m_TexturePool.b3AddPath(BLZ3_TEXTURES);
			}
			b3Scene::m_TexturePool.b3AddPath(pictures);

			world->b3AddPath(data);
			if (BLZ3_DATA != null)
			{
				world->b3AddPath(BLZ3_DATA);
			}
		}

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

		if (BLZ3_RENDER_PRIO != null)
		{
			b3Scene::m_RenderPriority = b3Math::b3Limit(atoi(BLZ3_RENDER_PRIO),-2 ,2);
		}

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
					strlcpy(BLZ3_EXTENSION,".tif",sizeof(BLZ3_EXTENSION));
					break;
				case 'j':
					strlcpy(BLZ3_EXTENSION,".jpg",sizeof(BLZ3_EXTENSION));
					break;
				case 'g':
					strlcpy(BLZ3_EXTENSION,".tga",sizeof(BLZ3_EXTENSION));
					break;
				case 'r':
					strlcpy(BLZ3_EXTENSION,".rgb8",sizeof(BLZ3_EXTENSION));
					break;
				case 'p':
					strlcpy(BLZ3_EXTENSION,".ps",sizeof(BLZ3_EXTENSION));
					break;

				case 'v' :
					b3Banner(null);
					break;
				}
			}
			else
			{
				b3_count CPUs = b3Runtime::b3GetNumCPUs();

				b3PrintF (B3LOG_NORMAL,"\nUsing %d CPU%s.\n",
					CPUs,
					CPUs > 1 ? "'s" : "");
				try
				{
					world->b3Read(argv[i]);
					for (item  = world->b3GetFirst();
					     item != null;
					     item  = scene->Succ)
					{
						scene = (b3Scene *)item;
						scene->b3Reorg();
						scene->b3SetupVertexMemory(&context);
						scene->b3SetFilename(argv[i]);
						animation = scene->b3GetAnimation();
						if (animation != null)
						{
							if ((!animation->b3IsActive()) || (force_no_anim))
							{
								b3PrintF(B3LOG_NORMAL,"Animation deactivated...\n");
								animation = null;
							}
							else
							{
								b3PrintF(B3LOG_NORMAL,"Using animation...\n");
							}
						}

						display = b3AllocDisplay(scene,force_no_display);
						if ((camera = scene->b3GetFirstCamera(false)) != null)
						{
							do
							{
								if (camera->m_Flags & CAMERA_ACTIVE)
								{
									scene->b3SetCamera(camera);
									b3PrintF(B3LOG_NORMAL,"Rendering \"%s\"...\n",
										camera->m_CameraName);

									if (animation != null)
									{
										b3_f64   t;
										b3_index iEnd,frame;
										b3Path   img_name;
										b3_count count = 0;

										b3PrintF(B3LOG_NORMAL,"Animating!!!\n\n");
										scene->b3ResetAnimation();
										iEnd = animation->b3AnimFrameIndex(animation->m_End);
										for (frame = 0;frame <= iEnd;frame++)
										{
											t = animation->b3AnimTimeCode(frame);
											b3PrintF(B3LOG_NORMAL,"Rendering frame t=%1.2f\n",t);
											scene->b3SetAnimation(t);
											scene->b3Raytrace(display);
											img_name.b3Format("%s_%04ld",camera->b3GetName(),count++);
											b3SaveRaytracedImage(
												display,
												BLZ3_PICTURES,img_name);
										}
									}
									else
									{
										scene->b3ComputeBounds(&lower,&upper);
										scene->b3Raytrace(display);
										b3SaveRaytracedImage(
											display,
											BLZ3_PICTURES,camera->b3GetName());
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
							b3PrintF(B3LOG_NORMAL,"Rendering default camera...\n");
							scene->b3ComputeBounds(&lower,&upper);
							scene->b3Raytrace(display);
							if (!b3SaveRaytracedImage(
								display,
								BLZ3_PICTURES,scene->b3GetName()))
							{
								b3PrintF(B3LOG_NORMAL, "Cannot save image %s!\n",scene->b3GetName());
							}
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
		b3Banner(argv[0]);
	}
	return EXIT_SUCCESS;
}
