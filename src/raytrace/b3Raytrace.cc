/*
**
**	$Filename:	b3Raytrace.cc $
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
**	Revision 1.4  2001/09/30 15:46:07  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**
**	Revision 1.3  2001/09/23 18:50:27  sm
**	- Created first raytracing image with Blizzard III. It shows
**	  simply "hit" or "no hit". Spheres and boxes aren't running
**	  yet. Next step: shading!
**	
**	Revision 1.2  2001/09/23 15:37:15  sm
**	- Introducing raytracing for Lines III. There is much work
**	  for a b3Display-CScrollView.
**	
**	Revision 1.1  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**
*/

struct b3_ray_info
{
	b3Display *display;
	b3Scene   *scene;
};

b3RayRow::b3RayRow(
	b3_coord y,
	b3_res   xSize,
	b3_res   ySize) : b3Row(y,xSize)
{
	m_y     = y;
	m_xSize = xSize;
	m_ySize = ySize;
}

b3_pkd_color *b3RayRow::b3GetBuffer()
{
	return buffer;
}

void b3Scene::b3RaytraceOneRow(b3RayRow *row)
{
	b3_res        x,xSize;
	b3_ray        ray;
	b3_dVector    preDir;
	b3_f64        fx,fxStep;
	b3_f64        fy;
	b3_pkd_color *buffer,r,g,b;

	buffer = row->b3GetBuffer();
	xSize  = row->m_xSize;
	fx     = -1.0;
	fy     =  1.0 - row->m_y * 2.0 / (double)row->m_ySize;
	fxStep =  2.0 / (double)xSize;

	// Init eye position
	ray.ray.pos.x =  m_EyePoint.x;
	ray.ray.pos.y =  m_EyePoint.y;
	ray.ray.pos.z =  m_EyePoint.z;

	// Init height of ray
	preDir.x  = (m_ViewPoint.x - m_EyePoint.x) + fy * m_Height.x;
	preDir.y  = (m_ViewPoint.y - m_EyePoint.y) + fy * m_Height.y;
	preDir.z  = (m_ViewPoint.z - m_EyePoint.z) + fy * m_Height.z;

	// Loop one row...
	for (x = 0;x < xSize;x++)
	{
		ray.ray.dir.x = preDir.x + fx * m_Width.x;
		ray.ray.dir.y = preDir.y + fx * m_Width.y;
		ray.ray.dir.z = preDir.z + fx * m_Width.z;

		b3Shade(&ray);
		r = (b3_pkd_color)(ray.color.r * 255.0);
		g = (b3_pkd_color)(ray.color.g * 255.0);
		b = (b3_pkd_color)(ray.color.b * 255.0);
		buffer[x] = (r << 16) | (g << 8) | b;

		fx += fxStep;
	}
}

b3_u32 b3Scene::b3RaytraceThread(void *ptr)
{
	b3_ray_info *info  = (b3_ray_info *)ptr;
	b3Scene     *scene = info->scene;
	b3RayRow    *row;

	do
	{
		// Enter critical section
		scene->m_RowMutex.b3Lock();
		if (row = scene->m_Rows.First)
		{
			scene->m_Rows.b3Remove(row);
		}
		scene->m_RowMutex.b3Unlock();
		// Leave critical section

		if (row != null)
		{
			// We can handle the row for its own!
			scene->b3RaytraceOneRow(row);
			info->display->b3PutRow(row);
			delete row;
		}
	}
	while(row != null);

	// Reach this if the row list ran empty.
	return 0;
}

void b3Scene::b3Raytrace(b3Display *display)
{
	b3RayRow    *row;
	b3Thread    *threads;
	b3_res       xSize,ySize;
	b3_count     CPUs,i;
	b3_ray_info *infos;

	b3_f64       fy,fyStep;

	try
	{
		// What resolution to use
		display->b3GetRes(xSize,ySize);

		// Determine CPU count
		CPUs = m_CPU.b3GetNumCPU();
		b3PrintF (B3LOG_NORMAL,"Using %d CPU%s.\n",
			CPUs,
			CPUs > 1 ? "'s" : "");

		// Allocate some instances
		infos       = new b3_ray_info[CPUs];
		threads     = new b3Thread[CPUs];

		// add rows to list
		fy     = 1.0;
		fyStep = 2.0 / (double)ySize;
		m_Rows.b3InitBase();
		for (i = 0;i < ySize;i++)
		{
			row = new b3RayRow(i,xSize,ySize);
			m_Rows.b3Append(row);
			fy -= fyStep;
		}

		b3PrintF (B3LOG_NORMAL,"Starting threads...\n");
		for (i = 0;i < CPUs;i++)
		{
			infos[i].display = display;
			infos[i].scene   = this;

			threads[i].b3Start(b3RaytraceThread,&infos[i]);
		}

		// Wait for completion
		b3PrintF (B3LOG_NORMAL,"Waiting for threads...\n");
		for (i = 0;i < CPUs;i++)
		{
			threads[i].b3Wait();
		}

		// Free what we have allocated.
		delete [] threads;
		delete [] infos;
		b3PrintF (B3LOG_NORMAL,"Done.\n");
	}
	catch(b3DisplayException *e)
	{
		b3PrintF(B3LOG_NORMAL,"### Error occured: %d\n",e->b3GetError());
	}
}
