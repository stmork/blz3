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
**	Revision 1.19  2001/10/22 14:47:38  sm
**	- Type correction vor b3Base/b3Link. So fixed a bad behaviour
**	  on Windows.
**	- Some minor fixes.
**
**	Revision 1.18  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
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

/*************************************************************************
**                                                                      **
**                        Compute one simple row...                     **
**                                                                      **
*************************************************************************/

b3RayRow::b3RayRow(
	b3Scene  *scene,
	b3_coord  y,
	b3_res    xSize,
	b3_res    ySize) : b3Row(y,xSize)
{
	m_Scene = scene;
	m_y     = y;
	m_xSize = xSize;
	m_ySize = ySize;


	// Init direction
	m_fy        =  1.0 - m_y * 2.0 / (b3_f64)m_ySize;
	m_fxStep    =  2.0 / (b3_f64)m_xSize;
	m_preDir.x  = (m_Scene->m_ViewPoint.x - m_Scene->m_EyePoint.x) - m_Scene->m_Width.x + m_fy * m_Scene->m_Height.x;
	m_preDir.y  = (m_Scene->m_ViewPoint.y - m_Scene->m_EyePoint.y) - m_Scene->m_Width.y + m_fy * m_Scene->m_Height.y;
	m_preDir.z  = (m_Scene->m_ViewPoint.z - m_Scene->m_EyePoint.z) - m_Scene->m_Width.z + m_fy * m_Scene->m_Height.z;
}

void b3RayRow::b3Raytrace()
{
	b3_res        x;
	b3_ray_info   ray;
	b3_f64        fx;

	// Init eye position
	ray.pos.x =  m_Scene->m_EyePoint.x;
	ray.pos.y =  m_Scene->m_EyePoint.y;
	ray.pos.z =  m_Scene->m_EyePoint.z;
	fx        = -1;

	// Loop one row...
	for (x = 0;x < xSize;x++)
	{
		ray.dir.x  = m_preDir.x;
		ray.dir.y  = m_preDir.y;
		ray.dir.z  = m_preDir.z;
		ray.inside = false;

		if (!m_Scene->b3Shade(&ray))
		{
			m_Scene->b3GetBackgroundColor(&ray,fx,m_fy);
		}
		m_preDir.x += m_Scene->m_xStepDir.x;
		m_preDir.y += m_Scene->m_xStepDir.y;
		m_preDir.z += m_Scene->m_xStepDir.z;
		fx         += m_fxStep;

		buffer[x] = b3Color::b3GetSatColor(&ray.color);
	}
}

/*************************************************************************
**                                                                      **
**                        Compute a row with adaptive one level         **
**                        super sampling                                **
**                                                                      **
*************************************************************************/

b3SupersamplingRayRow::b3SupersamplingRayRow(
	b3Scene  *scene,
	b3_coord  y,
	b3_res    xSize,
	b3_res    ySize) :
		b3RayRow(scene,y,xSize,ySize)
{
	m_Limit = m_Scene->m_SuperSample->m_Limit;
}

void b3SupersamplingRayRow::b3Raytrace()
{
	b3_res        x;
	b3_ray_info   ray;
	b3_color      accum,last;
	b3_bool       add;
	b3_f64        fxLeft,fxRight,fyUp,fyDown;

	// Init eye position
	ray.pos.x =  m_Scene->m_EyePoint.x;
	ray.pos.y =  m_Scene->m_EyePoint.y;
	ray.pos.z =  m_Scene->m_EyePoint.z;

	// Init coord values
	fxRight = -1;
	fxLeft  = fxRight - 0.5 * m_fxStep;
	fyDown  = m_fy;
	fyUp    = fyDown + 1.0 / (b3_f64)m_ySize;

	// Loop one row...
	for (x = 0;x < xSize;x++)
	{
		ray.dir.x  = m_preDir.x;
		ray.dir.y  = m_preDir.y;
		ray.dir.z  = m_preDir.z;
		ray.inside = false;

		if (!m_Scene->b3Shade(&ray))
		{
			m_Scene->b3GetBackgroundColor(&ray,fxRight,fyDown);
		}

		if (x > 0)
		{
			add =
				(fabs(last.r - ray.color.r) >= m_Limit.r) ||
				(fabs(last.g - ray.color.g) >= m_Limit.g) ||
				(fabs(last.b - ray.color.b) >= m_Limit.b);
		}
		else
		{
			add = false;
		}
		if (add)
		{
			accum = ray.color;

			ray.dir.x  = (m_preDir.x += m_Scene->m_yHalfDir.x);
			ray.dir.y  = (m_preDir.y += m_Scene->m_yHalfDir.y);
			ray.dir.z  = (m_preDir.z += m_Scene->m_yHalfDir.z);
			ray.inside = false;
			if (!m_Scene->b3Shade(&ray))
			{
				m_Scene->b3GetBackgroundColor(&ray,fxRight,fyUp);
			}
			b3Color::b3Add(&ray.color,&accum);

			ray.dir.x  = (m_preDir.x -= m_Scene->m_xHalfDir.x);
			ray.dir.y  = (m_preDir.y -= m_Scene->m_xHalfDir.y);
			ray.dir.z  = (m_preDir.z -= m_Scene->m_xHalfDir.z);
			ray.inside = false;
			if (!m_Scene->b3Shade(&ray))
			{
				m_Scene->b3GetBackgroundColor(&ray,fxLeft,fyUp);
			}
			b3Color::b3Add(&ray.color,&accum);

			ray.dir.x  = (m_preDir.x -= m_Scene->m_yHalfDir.x);
			ray.dir.y  = (m_preDir.y -= m_Scene->m_yHalfDir.y);
			ray.dir.z  = (m_preDir.z -= m_Scene->m_yHalfDir.z);
			ray.inside = false;
			if (!m_Scene->b3Shade(&ray))
			{
				m_Scene->b3GetBackgroundColor(&ray,fxLeft,fyDown);
			}
			b3Color::b3Add(&ray.color,&accum);

			ray.dir.x = (m_preDir.x += m_Scene->m_xHalfDir.x);
			ray.dir.y = (m_preDir.y += m_Scene->m_xHalfDir.y);
			ray.dir.z = (m_preDir.z += m_Scene->m_xHalfDir.z);
			b3Color::b3Scale(&accum,0.25,&ray.color);
		}
		m_preDir.x += m_Scene->m_xStepDir.x;
		m_preDir.y += m_Scene->m_xStepDir.y;
		m_preDir.z += m_Scene->m_xStepDir.z;
		fxRight    += m_fxStep;
		fxLeft     += m_fxStep;

		buffer[x] = b3Color::b3GetSatColor(&ray.color);
		last = ray.color;
	}
}

/*************************************************************************
**                                                                      **
**                        Raytracing routines                           **
**                                                                      **
*************************************************************************/

b3_u32 b3Scene::b3RaytraceThread(void *ptr)
{
	b3_rt_info *info  = (b3_rt_info *)ptr;
	b3Scene    *scene = info->scene;
	b3RayRow   *row;

	do
	{
		// Enter critical section
		scene->m_RowMutex.b3Lock();
		if ((row = (b3RayRow *)scene->m_Rows.First) != null)
		{
			scene->m_Rows.b3Remove(row);
		}
		scene->m_RowMutex.b3Unlock();
		// Leave critical section

		if (row != null)
		{
			// We can handle the row for its own!
			row->b3Raytrace();
			info->display->b3PutRow(row);
			delete row;
		}
	}
	while(row != null);

	// Reach this if the row list ran empty.
	return 0;
}

b3_bool b3Scene::b3Prepare(b3_res xSize,b3_res ySize)
{
	b3Nebular *nebular;
	b3Light   *light;
	b3BBox    *bbox;
	b3_f64     xDenom,yDenom;

	m_AvrgColor.r = (m_BottomColor.r + m_TopColor.r) * 0.5;
	m_AvrgColor.g = (m_BottomColor.g + m_TopColor.g) * 0.5;
	m_AvrgColor.b = (m_BottomColor.b + m_TopColor.b) * 0.5;
	m_DiffColor.r = (m_TopColor.r    - m_AvrgColor.r);
	m_DiffColor.g = (m_TopColor.g    - m_AvrgColor.g);
	m_DiffColor.b = (m_TopColor.b    - m_AvrgColor.b);

	xDenom = b3Vector::b3Length(&m_Width);
	yDenom = b3Vector::b3Length(&m_Height);
	if ((xDenom == 0) || (yDenom == 0))
	{
		return false;
	}
	m_NormWidth.x  = m_Width.x  / xDenom;
	m_NormWidth.y  = m_Width.y  / xDenom;
	m_NormWidth.z  = m_Width.z  / xDenom;
	m_NormHeight.x = m_Height.x / yDenom;
	m_NormHeight.y = m_Height.y / yDenom;
	m_NormHeight.z = m_Height.z / yDenom;

	nebular       = b3GetNebular();
	m_Nebular     = (nebular->b3IsActive() ? nebular : null);
	m_LensFlare   = b3GetLensFlare();
	m_SuperSample = b3GetSuperSample();
m_SuperSample = null;

	if (m_SuperSample != null)
	{
		// Init half steps for super sampling
		m_xHalfDir.x = m_Width.x  / (b3_f64)xSize;
		m_xHalfDir.y = m_Width.y  / (b3_f64)xSize;
		m_xHalfDir.z = m_Width.z  / (b3_f64)xSize;

		m_yHalfDir.x = m_Height.x / (b3_f64)ySize;
		m_yHalfDir.y = m_Height.y / (b3_f64)ySize;
		m_yHalfDir.z = m_Height.z / (b3_f64)ySize;

		m_xStepDir.x = m_xHalfDir.x + m_xHalfDir.x;
		m_xStepDir.y = m_xHalfDir.y + m_xHalfDir.y;
		m_xStepDir.z = m_xHalfDir.z + m_xHalfDir.z;
		b3PrintF(B3LOG_NORMAL,"Using one-level adaptive super sampling.\n");
	}
	else
	{
		m_xStepDir.x = m_Width.x * 2.0 / (b3_f64)xSize;
		m_xStepDir.y = m_Width.y * 2.0 / (b3_f64)xSize;
		m_xStepDir.z = m_Width.z * 2.0 / (b3_f64)xSize;
		b3PrintF(B3LOG_NORMAL,"Using simple sampling.\n");
	}

	for(bbox = b3GetFirstBBox();bbox != null;bbox = (b3BBox *)bbox->Succ)
	{
		if (!bbox->b3Prepare())
		{
			return false;
		}
	}

	// Init lights
	m_LightCount = 0;
	for (light = b3GetLight();light != null;light = (b3Light *)light->Succ)
	{
		if (!light->b3Prepare())
		{
			return false;
		}
		if (light->m_LightActive)
		{
			m_LightCount++;
		}
	}
	m_ShadowFactor = (b3_f64)m_ShadowBrightness / (b3_f64)m_LightCount;

	return true;
}

void b3Scene::b3Raytrace(b3Display *display)
{
	b3RayRow    *row;
	b3Thread    *threads;
	b3_res       xSize,ySize;
	b3_count     CPUs,i;
	b3_rt_info  *infos;
	b3_f64       fy,fyStep;

	try
	{
		// What resolution to use
		display->b3GetRes(xSize,ySize);

		if (!b3Prepare(xSize,ySize))
		{
			b3PrintF(B3LOG_NORMAL,"Cannot initialize raytracing!\n");
			return;
		}

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
			if (m_SuperSample != null)
			{
				row = new b3SupersamplingRayRow(this,i,xSize,ySize);
			}
			else
			{
				row = new b3RayRow(this,i,xSize,ySize);
			}
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
