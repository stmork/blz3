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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/b3Config.h" 
#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3Time.h"
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

int main(int argc,char *argv[])
{
	b3Item       *item;
	b3World      *world;
	b3Scene      *scene;
	b3CameraPart *camera;
	b3Display    *display;
	b3_res        xSize,ySize;
	b3_index      i;
	char         *HOME = getenv("HOME");
	b3TimeSpan    span;
	b3Path        textures;
	b3Path        pictures;
	b3Path        data;

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
			world->b3Read(argv[1]);
			for (item  = world->b3GetFirst();
			     item != null;
			     item  = scene->Succ)
			{
				scene = (b3Scene *)item;
#ifdef BLZ3_USE_OPENGL
				scene->b3Reorg();
#endif
				if (scene->b3GetDisplaySize(xSize,ySize))
				{
					display = new b3Display(xSize,ySize);
				}
				else
				{
					display = new b3Display();
				}

				if ((camera = scene->b3GetCamera(false)) != null)
				{
					do
					{
						if (camera->Flags & CAMERA_ACTIVE)
						{
							b3PrintF(B3LOG_NORMAL,"Rendering \"%s\"...\n",
								camera->CameraName);
							scene->m_Width     = camera->Width;
							scene->m_Height    = camera->Height;
							scene->m_EyePoint  = camera->EyePoint;
							scene->m_ViewPoint = camera->ViewPoint;

							span.b3Start();
							scene->b3Raytrace(display);
							span.b3Stop();
							span.b3Print();
						}
						else
						{
							b3PrintF(B3LOG_NORMAL,"Skipping \"%s\"...\n",
								camera->CameraName);
						}
						camera = scene->b3GetNextCamera(camera);
					}
					while (camera != null);
				}
				else
				{
					scene->b3Raytrace(display);
				}

				display->b3Wait();
				delete display;
			}
		}
		delete world;
	}
	return 0;
}
