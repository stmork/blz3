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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
	b3Item    *item;
	b3World   *world;
	b3Scene   *scene;
	b3Display *display;
	b3_res     xSize,ySize;
	b3_index   i;

	if (argc > 1)
	{
		b3InitRaytrace::b3Init();

		world = new b3World();
		for (i = 1;i < argc;i++)
		{
			world->b3Read(argv[1]);
			for (item  = world->b3GetFirst();
			     item != null;
			     item  = scene->Succ)
			{
				scene = (b3Scene *)item;
				scene->b3Reorg();
				if (scene->b3GetDisplaySize(xSize,ySize))
				{
					display = new b3Display(xSize,ySize);
				}
				else
				{
					display = new b3Display();
				}
				scene->b3Raytrace(display);

				display->b3Wait();
				delete display;
			}
		}
	}
	return 0;
}
