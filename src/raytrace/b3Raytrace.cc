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
#include "blz3/raytrace/b3PrepareInfo.h"
#include "blz3/base/b3Aux.h"
#include "blz3/base/b3Matrix.h"

#define no_DEBUG_SS4

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.42  2002/08/22 14:06:32  sm
**	- Corrected filter support and added test suite.
**	- Added animation computing to brt3. Now we are near to
**	  real time raytracing: 8 fps for Animationtest.
**
**	Revision 1.41  2002/08/21 20:13:32  sm
**	- Introduced distributed raytracing with all sampling methods
**	  and filter computations. This made some class movements
**	  inside files necessary. The next step would be to integrate
**	  motion blur.
**	
**	Revision 1.40  2002/08/21 10:16:40  sm
**	- Made some changes to the Un*x OpenGL renderer:
**	  o Added animations
**	  o Added camera switching
**	  o Corrected far clipping plane computation.
**	- Configure script tidied up.
**	
**	Revision 1.39  2002/08/18 13:05:17  sm
**	- First try to animate. We have to relink the control points which
**	  are stored in separate Blizzard classes to the b3AnimElement
**	  class.
**	
**	Revision 1.38  2002/08/09 13:20:19  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.37  2002/08/03 18:05:10  sm
**	- Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**	- Moved b3PrepareInfo into b3Scene class as member. This
**	  saves memory allocation calls and is an investment into
**	  faster Lines III object transformation.
**	
**	Revision 1.36  2002/08/02 14:52:13  sm
**	- Vertex/normal computation is now multithreaded, too.
**	- Minor changes on b3PrepareInfo class.
**	- Last changes to Windows port.
**	
**	Revision 1.35  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**	
**	Revision 1.34  2002/02/17 21:58:11  sm
**	- Done UnCR
**	- Modified makefiles
**	
**	Revision 1.33  2002/02/14 16:51:49  sm
**	- Done some cleanups concernig doubly linked lists. Moved
**	  obsolete b3Node/b3Head into their own include.
**	
**	Revision 1.32  2002/02/01 15:04:09  sm
**	- Prepared shapes for icon conversion
**	- Added to save selected/first visible item in
**	  hierarchy dialog.
**	- Some print cleanups done.
**	- Fixed activation of b3SuperSample.
**	
**	Revision 1.31  2002/01/21 16:56:46  sm
**	- Showing splash dialog only in release version.
**	- Prepared shape icons.
**	
**	Revision 1.30  2002/01/01 13:50:22  sm
**	- Fixed some memory leaks:
**	  o concerning triangle shape and derived spline shapes
**	  o concerning image pool handling. Images with windows
**	    path weren't found inside the image pool requesting
**	    further image load.
**	
**	Revision 1.29  2001/11/26 17:16:37  sm
**	- Linux b3TimeSpan fix
**	
**	Revision 1.28  2001/11/25 19:20:32  sm
**	- Added new acting methods:
**	  o Camera move
**	  o Camera turn around itself
**	  o Camera rotate around fulcrum
**	
**	Revision 1.27  2001/11/07 15:55:09  sm
**	- Introducing b3TimeSpan to Windows to get computation time on
**	  Windows as well.
**	- Changed some include dependencies.
**	
**	Revision 1.26  2001/11/04 12:15:15  sm
**	- Renaming some attributes...
**	- Taking account to redesign of b3Display
**	
**	Revision 1.25  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.24  2001/11/02 19:05:36  sm
**	- Introducing time mearuring.
**	- Fixed wrong lens flare handling.
**	
**	Revision 1.23  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.22  2001/10/31 14:46:35  sm
**	- Filling b3IsCancelled() with sense.
**	- Inlining b3RGB
**	
**	Revision 1.21  2001/10/29 19:34:02  sm
**	- Added new define B3_DELETE_BASE.
**	- Added support to abort raytrace processing.
**	- Added search path to world loading.
**	- Fixed super sampling.
**	- Fixed memory leak in raytracing row processing.
**	
**	Revision 1.20  2001/10/28 21:22:52  sm
**	- Added one level simple adaptive super sampling. It was
**	  tricky to implement this on SMP machines but got it now.
**	
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
		scene->m_PoolMutex.b3Lock();
		if ((row = (b3RayRow *)scene->m_RowPool.First) != null)
		{
			scene->m_RowPool.b3Remove(row);
		}
		scene->m_PoolMutex.b3Unlock();
		// Leave critical section

		if (row != null)
		{
			// We can handle the row for its own!
			row->b3Raytrace();

			scene->m_TrashMutex.b3Lock();
			scene->m_TrashPool.b3Append(row);
			scene->m_TrashMutex.b3Unlock();
		}
	}
	while(row != null);

	// Reach this if the row list ran empty.
	return 0;
}

b3_bool b3Scene::b3PrepareThread(b3BBox *bbox,void *ptr)
{
	return bbox->b3Prepare();
}

b3_bool b3Scene::b3Prepare(b3_res xSize,b3_res ySize)
{
	b3Nebular     *nebular;
	b3Distribute  *distributed;
	b3SuperSample *supersample;
	b3Light       *light;
	b3_f64         xDenom,yDenom;

	b3PrintF(B3LOG_FULL,"b3Scene::b3Prepare(%d,%d)\n",xSize,ySize);
	b3PrintF(B3LOG_FULL,"  preparing background color...\n");
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

	b3PrintF(B3LOG_FULL,"  preparing lensflare...\n");
	m_LensFlare = b3GetLensFlare();
	if (m_LensFlare != null)
	{
		if (!m_LensFlare->b3IsActive())
		{
			m_LensFlare = null;
		}
	}

	b3PrintF(B3LOG_FULL,"  preparing nebular...\n");
	nebular = b3GetNebular();
	if (nebular->b3IsActive())
	{
		m_Nebular = nebular;
		m_Nebular->b3Prepare();
		b3PrintF(B3LOG_DEBUG,"Using nebular with %3.2f units distance to half value.\n",
			m_Nebular->m_NebularVal);
	}
	else
	{
		m_Nebular = null;
	}

	b3PrintF(B3LOG_FULL,"  preparing distributed raytracing...\n");
	distributed = b3GetDistributed();
	if (distributed->b3IsActive())
	{
		m_Distributed = distributed;
		m_Distributed->b3Prepare(xSize);
		m_SuperSample = null;
	}
	else
	{
		m_Distributed = null;
		b3PrintF(B3LOG_FULL,"  preparing super sampling...\n");
		supersample = b3GetSuperSample();
		if (supersample->b3IsActive())
		{
			m_SuperSample = supersample;
			b3PrintF(B3LOG_NORMAL,"Using one-level adaptive super sampling.\n");
		}
		else
		{
			m_SuperSample = null;
			b3PrintF(B3LOG_NORMAL,"Using simple sampling.\n");
		}
	}
	if ((m_Distributed != null) || (m_SuperSample != null))
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
	}
	else
	{
		m_xStepDir.x  = m_Width.x * 2.0 / (b3_f64)xSize;
		m_xStepDir.y  = m_Width.y * 2.0 / (b3_f64)xSize;
		m_xStepDir.z  = m_Width.z * 2.0 / (b3_f64)xSize;
	}

	// Init lights
	b3PrintF(B3LOG_FULL,"  preparing lights...\n");
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

	// Init geometry
	b3PrintF(B3LOG_FULL,"  preparing geometry...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	if(!m_PrepareInfo.b3Prepare(b3PrepareThread))
	{
		return false;
	}

	b3PrintF(B3LOG_FULL,"  preparing done...\n");

	return (m_BackgroundType == TP_TEXTURE ?
		b3CheckTexture(&m_BackTexture,m_TextureName) :
		true);
}

void b3Scene::b3Raytrace(b3Display *display)
{
	b3Row      *row;
	b3Thread   *threads;
	b3TimeSpan  span;
	b3_res      xSize,ySize;
	b3_count    CPUs,i;
	b3_rt_info *infos;
	b3_f64      fy,fyStep;

	try
	{
		b3UpdateCamera();

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
		fyStep = 2.0 / (b3_f64)ySize;
		for (i = 0;i < ySize;i++)
		{
			row = null;
			if (m_Distributed != null)
			{
				row = new b3DistributedRayRow(this,display,i,xSize,ySize);
			}
			if (m_SuperSample != null)
			{
				row = new b3SupersamplingRayRow(this,display,i,xSize,ySize,
					(b3SupersamplingRayRow *)m_RowPool.Last);
			}

			// Add default row
			if (row == null)
			{
				row = new b3RayRow(this,display,i,xSize,ySize);
			}
			m_RowPool.b3Append(row);
			fy -= fyStep;
		}

		b3PrintF (B3LOG_NORMAL,"Starting threads...\n");
		span.b3Start();
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
#if defined(__linux__) || defined(WIN32)
			threads[i].b3AddTimeSpan(&span);
#endif
		}
		span.b3Stop();
		span.b3Print();

		// Free what we have allocated.
		delete [] threads;
		delete [] infos;
		m_TrashMutex.b3Lock();
		B3_DELETE_BASE(&m_TrashPool,row);
		m_TrashMutex.b3Unlock();

		b3PrintF (B3LOG_NORMAL,"Done.\n");
	}
	catch(b3DisplayException &e)
	{
		b3PrintF(B3LOG_NORMAL,"### Error occured: %s\n",e.b3GetErrorMsg());
	}
}

void b3Scene::b3AbortRaytrace()
{
	b3Row *row;

	do
	{
		// Enter critical section
		m_PoolMutex.b3Lock();
		if ((row = m_RowPool.First) != null)
		{
			m_RowPool.b3Remove(row);
		}
		m_PoolMutex.b3Unlock();
		// Leave critical section

		if (row != null)
		{
			// We can handle the row for its own!
			m_TrashMutex.b3Lock();
			m_TrashPool.b3Append(row);
			m_TrashMutex.b3Unlock();
		}
	}
	while(row != null);
}
