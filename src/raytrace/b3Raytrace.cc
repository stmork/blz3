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
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.17  2001/10/20 16:14:59  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**
**	Revision 1.16  2001/10/19 18:27:28  sm
**	- Fixing LDC bug
**	- Optimizing color routines
**	
**	Revision 1.15  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.14  2001/10/18 15:16:28  sm
**	- Testing with an appropriate system - and it's good!
**	
**	Revision 1.13  2001/10/18 14:48:26  sm
**	- Fixing refracting problem on some scenes with glasses.
**	- Fixing overlighting problem when using Mork shading.
**	- Finxing some memory leaks (espacially b3TriangleRefs)
**	- Adding texture support to conditions (stencil mapping).
**	  Now conditions are ready to work compatible with
**	  Blizzard II.
**	
**	Revision 1.12  2001/10/17 14:46:02  sm
**	- Adding triangle support.
**	- Renaming b3TriangleShape into b3Triangles and introducing
**	  new b3TriangleShape as base class. This results in
**	  source file renaming, too.
**	- Fixing soft shadow bug.
**	- Only scene loading background image when activated.
**	- Fixing LDC spline initialization.
**	- Converting Windows paths into right paths on Un*x
**	
**	Revision 1.11  2001/10/07 20:17:27  sm
**	- Prepared texture support.
**	- Noise procedures added.
**	- Added bump and material support.
**	- Added soft shadows.
**	
**	Revision 1.10  2001/10/05 20:30:46  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**	
**	Revision 1.9  2001/10/03 20:17:56  sm
**	- Minor bugfixes
**	
**	Revision 1.8  2001/10/03 18:46:45  sm
**	- Adding illumination and recursive raytracing
**	
**	Revision 1.7  2001/10/02 16:01:58  sm
**	- Moving b3Polar into b3Ray but that's not right at all. The
**	  result must be placed there but a simple result from one
**	  intersection must be placed into a temp instance. The same
**	  must be done for surface normals as they result from using
**	  the b3Polar class.
**	
**	Revision 1.6  2001/09/30 16:27:48  sm
**	- Raytracing with diffuse color without shading
**	- Sphere intersection fixed (now using normalized rays)
**	
**	Revision 1.5  2001/09/30 15:53:19  sm
**	- Removing nasty CR/LF
**	
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

b3_f64 epsilon = 0.0005;

struct b3_rt_info
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
	b3_ray_info   ray;
	b3_dVector    preDir;
	b3_f64        fx,fxStep;
	b3_f64        fy;
	b3_pkd_color *buffer;

	buffer = row->b3GetBuffer();
	xSize  = row->m_xSize;
	fx     = -1.0;
	fy     =  1.0 - row->m_y * 2.0 / (double)row->m_ySize;
	fxStep =  2.0 / (double)xSize;

	// Init eye position
	ray.pos.x =  m_EyePoint.x;
	ray.pos.y =  m_EyePoint.y;
	ray.pos.z =  m_EyePoint.z;

	// Init height of ray
	preDir.x  = (m_ViewPoint.x - m_EyePoint.x) + fy * m_Height.x;
	preDir.y  = (m_ViewPoint.y - m_EyePoint.y) + fy * m_Height.y;
	preDir.z  = (m_ViewPoint.z - m_EyePoint.z) + fy * m_Height.z;

	// Loop one row...
	for (x = 0;x < xSize;x++)
	{
		ray.dir.x  = preDir.x + fx * m_Width.x;
		ray.dir.y  = preDir.y + fx * m_Width.y;
		ray.dir.z  = preDir.z + fx * m_Width.z;
		ray.inside = false;

		if (!b3Shade(&ray))
		{
			b3GetBackgroundColor(&ray.color);
		}

		buffer[x] = b3Color::b3GetSatColor(&ray.color);

		fx += fxStep;
	}
}

b3_u32 b3Scene::b3RaytraceThread(void *ptr)
{
	b3_rt_info *info  = (b3_rt_info *)ptr;
	b3Scene    *scene = info->scene;
	b3RayRow   *row;

	do
	{
		// Enter critical section
		scene->m_RowMutex.b3Lock();
		if ((row = scene->m_Rows.First) != null)
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

b3_bool b3BBox::b3Prepare()
{
	b3Item  *item;
	b3Shape *shape;
	b3BBox  *bbox;

	B3_FOR_BASE(b3GetBBoxHead(),item)
	{
		bbox = (b3BBox *)item;
		if (!bbox->b3Prepare())
		{
			return false;
		}
	}

	B3_FOR_BASE(b3GetShapeHead(),item)
	{
		shape = (b3Shape *)item;
		if (!shape->b3Prepare())
		{
			return false;
		}
	}

	return true;
}

void b3Scene::b3Raytrace(b3Display *display)
{
	b3RayRow    *row;
	b3Nebular   *nebular;
	b3Light     *light;
	b3BBox      *bbox;
	b3Thread    *threads;
	b3_res       xSize,ySize;
	b3_count     CPUs,i;
	b3_rt_info  *infos;
	b3_f64       fy,fyStep;

	nebular = b3GetNebular();
	if (nebular->b3IsActive())
	{
		m_Nebular = nebular;
	}

	for(bbox = b3GetFirstBBox();bbox != null;bbox = (b3BBox *)bbox->Succ)
	{
		bbox->b3Prepare();
	}

	m_LightCount = 0;
	for (light = b3GetLight();light != null;light = (b3Light *)light->Succ)
	{
		light->b3Prepare();
		if (light->m_LightActive)
		{
			m_LightCount++;
		}
	}
	m_ShadowFactor = (b3_f64)m_ShadowBrightness / (b3_f64)m_LightCount;

	try
	{
		// What resolution to use
		display->b3GetRes(xSize,ySize);

		// Determine CPU count
		CPUs = b3Runtime::b3GetNumCPUs();
		b3PrintF (B3LOG_NORMAL,"Using %d CPU%s.\n",
			CPUs,
			CPUs > 1 ? "'s" : "");

		// Allocate some instances
		infos       = new b3_rt_info[CPUs];
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

			threads[i].b3Start(b3RaytraceThread,&infos[i],-1);
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
