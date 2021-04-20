/*
**
**	$Filename:	b3Raytrace.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - The new Blizzard III raytracer
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3RaytraceInclude.h"
#include "blz3/system/b3Display.h"

#define no_DEBUG_SS4

struct b3_rt_info
{
	b3Display * m_Display;
	b3Scene  *  m_Scene;
	bool        m_Loop;
	b3_count    m_Num;
	b3Event     m_WaitForAnimation;
	b3Event     m_WaitForCompletion;
};

b3TxPool b3Scene::m_TexturePool;
b3_s32   b3Scene::m_RenderPriority = -2;

/*************************************************************************
**                                                                      **
**                        Raytracing routines                           **
**                                                                      **
*************************************************************************/

bool b3Scene::b3RaytraceThread(void * ptr)
{
	b3_rt_info * info  = (b3_rt_info *)ptr;
	b3Scene   *  scene = info->m_Scene;
	b3RayRow  *  row;

	do
	{
		// Enter critical section
		{
			b3CriticalSection lock(scene->m_PoolMutex);

			row = (b3RayRow *)scene->m_RowPool.b3RemoveFirst();
		}
		// Leave critical section

		if (row != nullptr)
		{
			// We can handle the row for its own!
			row->b3Raytrace();

			b3CriticalSection lock(scene->m_TrashMutex);
			scene->m_TrashPool.b3Append(row);
		}
	}
	while (row != nullptr);

	// Reach this if the row list ran empty.
	b3PrintF(B3LOG_FULL, "  Raytracing thread %d terminates...\n", info->m_Num);
	return 0;
}

bool b3Scene::b3RaytraceMotionBlurThread(void * ptr)
{
	b3_rt_info * info  = (b3_rt_info *)ptr;
	b3Scene   *  scene = info->m_Scene;
	b3RayRow  *  row;

	do
	{
		b3PrintF(B3LOG_FULL, "  Thread %d is waiting to start job.\n", info->m_Num);
		info->m_WaitForAnimation.b3Wait();
		b3PrintF(B3LOG_FULL, "  Thread %d is doing his job...\n", info->m_Num);

		do
		{
			// Enter critical section
			{
				b3CriticalSection lock(scene->m_PoolMutex);

				row = (b3RayRow *)scene->m_RowPool.b3RemoveFirst();
			}
			// Leave critical section

			if (row != nullptr)
			{
				// We can handle the row for its own!
				row->b3Raytrace();

				b3CriticalSection lock(scene->m_TrashMutex);

				scene->m_TrashPool.b3Append(row);
			}
		}
		while (row != nullptr);

		b3PrintF(B3LOG_FULL, "  Signalling main thread done job of thread %d.\n", info->m_Num);
		info->m_WaitForCompletion.b3Pulse();
	}
	while (info->m_Loop);

	b3PrintF(B3LOG_FULL, "  Raytracing thread %d for motion blur terminates...\n", info->m_Num);
	return 0;
}

void b3Scene::b3DoRaytrace(b3Display * display, b3_count CPUs)
{
	b3_rt_info * infos;
	b3TimeSpan   span;
	b3Thread  *  threads;
	b3_count     i;

	// Allocate some instances
	infos       = new b3_rt_info[CPUs];
	threads     = new b3Thread[CPUs];

	b3PrintF(B3LOG_NORMAL, "Starting threads...\n");
	span.b3Start();
	for (i = 0; i < CPUs; i++)
	{
		infos[i].m_Display = display;
		infos[i].m_Scene   = this;
		infos[i].m_Loop    = false;
		infos[i].m_Num     = i;

		if (!threads[i].b3Start(b3RaytraceThread, &infos[i], m_RenderPriority))
		{
			delete [] threads;
			delete [] infos;
			B3_THROW(b3PrepareException, B3_PREPARE_NO_THREAD);
		}
	}

	// Wait for completion
	b3PrintF(B3LOG_NORMAL, "Waiting for threads...\n");
	for (i = 0; i < CPUs; i++)
	{
		threads[i].b3Wait();
		threads[i].b3AddTimeSpan(&span);
	}
	span.b3Stop();
	span.b3Print();

	// Free what we have allocated.
	delete [] threads;
	delete [] infos;
}

void b3Scene::b3DoRaytraceMotionBlur(b3Display * display, b3_count CPUs)
{
	b3_rt_info * infos;
	b3Animation * anim = b3GetAnimation();
	b3TimeSpan   span;
	b3Thread  *  threads;
	b3_count     i, k;
	b3_f64       t, base = anim->m_Time;

	// Allocate some instances
	infos       = new b3_rt_info[CPUs];
	threads     = new b3Thread[CPUs];

	b3PrintF(B3LOG_NORMAL, "Starting threads...\n");
	b3PrintF(B3LOG_FULL,  "  Reference time point: %3.3lf FPS: %d\n",
		base, anim->m_FramesPerSecond);
	for (i = 0; i < CPUs; i++)
	{
		infos[i].m_Display = display;
		infos[i].m_Scene   = this;
		infos[i].m_Loop    = true;
		infos[i].m_Num     = i;

		if (!threads[i].b3Start(b3RaytraceMotionBlurThread, &infos[i], m_RenderPriority))
		{
			delete [] threads;
			delete [] infos;
			B3_THROW(b3PrepareException, B3_PREPARE_NO_THREAD);
		}
	}

	span.b3Start();
	for (k = 0; k < m_Distributed->m_SamplesPerFrame; k++)
	{
		// Animate!
		t = base + m_Distributed->m_MotionBlur[k];
		b3SetAnimation(t);

		// We have to update the actual time point
		B3_FOR_TYPED_BASE(b3MotionBlurRayRow, &m_RowPool, row)
		{
			row->b3SetTimePoint(t);
		}

		// Start raytracing at this time point
		b3PrintF(B3LOG_FULL, "  Activating threads at index %d, time point %3.3lf...\n", k, t);
		for (i = 0; i < CPUs; i++)
		{
			infos[i].m_WaitForAnimation.b3Pulse();
		}

		// Ensure that nobody is raytracing
		b3PrintF(B3LOG_FULL, "  Waiting for thread completion...\n");
		for (i = 0; i < CPUs; i++)
		{
			infos[i].m_WaitForCompletion.b3Wait();
		}

		// Move back rows for iterative reuse
		m_RowPool.b3Move(&m_TrashPool);
	}

	// Signalling stop to threads but first we have to empty the row
	// pool
	m_TrashPool.b3Move(&m_RowPool);

	b3PrintF(B3LOG_FULL, "  Signalling threads to terminate...\n");
	for (i = 0; i < CPUs; i++)
	{
		infos[i].m_Loop = false;
		infos[i].m_WaitForAnimation.b3Pulse();
	}

	// Do some cleanup during thread completion (They simply
	// have to realize that there is nothing to do anymore...
	b3SetAnimation(base);

	// Wait for completion
	b3PrintF(B3LOG_NORMAL, "Waiting for threads...\n");
	for (i = 0; i < CPUs; i++)
	{
		threads[i].b3Wait();
		threads[i].b3AddTimeSpan(&span);
	}

	span.b3Stop();
	span.b3Print();

	// Free what we have allocated.
	delete [] threads;
	delete [] infos;
}

/*************************************************************************
**                                                                      **
**                        Raytracing preparation                        **
**                                                                      **
*************************************************************************/

bool b3Scene::b3PrepareBBoxThread(b3BBox * bbox, void * ptr)
{
	return bbox->b3PrepareBBox((b3_scene_preparation *)ptr);
}

bool b3Scene::b3PrepareScene(b3_res xSize, b3_res ySize)
{
	b3Item        *        item;
	b3Nebular       *      nebular;
	b3Distribute     *     distributed;
	b3SuperSample     *    supersample;
	b3Time                 timepoint;
	b3Light        *       light;
	b3_f64                 xDenom, yDenom, tStart, tEnd;
	b3_scene_preparation * info = b3GetPrepareInfo();

	tStart       = timepoint;
	b3PrintF(B3LOG_FULL, "b3Scene::b3PrepareScene(%d,%d)\n", xSize, ySize);
	b3PrintF(B3LOG_FULL, "  preparing background color...\n");
	m_AvrgColor = (m_BottomColor + m_TopColor) * 0.5;
	m_DiffColor = (m_TopColor    - m_AvrgColor);

	b3PrintF(B3LOG_FULL, "  preparing special items...\n");
	B3_FOR_BASE(b3GetSpecialHead(), item)
	{
		if (!item->b3Prepare(info))
		{
			B3_THROW(b3PrepareException, B3_PREPARE_ERROR);
		}
	}

	xDenom = b3Vector::b3Length(&m_Width);
	yDenom = b3Vector::b3Length(&m_Height);
	if ((xDenom < b3Math::epsilon) || (yDenom < b3Math::epsilon))
	{
		b3PrintF(B3LOG_NORMAL, "Camera has got zero extend...\n");
		return false;
	}
	m_NormWidth.x  = m_Width.x  / xDenom;
	m_NormWidth.y  = m_Width.y  / xDenom;
	m_NormWidth.z  = m_Width.z  / xDenom;
	m_NormHeight.x = m_Height.x / yDenom;
	m_NormHeight.y = m_Height.y / yDenom;
	m_NormHeight.z = m_Height.z / yDenom;
	m_ViewAxis.x   = m_ViewPoint.x - m_EyePoint.x;
	m_ViewAxis.y   = m_ViewPoint.y - m_EyePoint.y;
	m_ViewAxis.z   = m_ViewPoint.z - m_EyePoint.z;
	m_ViewAxisLen  = b3Vector::b3Length(&m_ViewAxis);

	b3PrintF(B3LOG_FULL, "  preparing lensflare...\n");
	m_LensFlare = b3GetLensFlare();
	if (m_LensFlare != nullptr)
	{
		if (!m_LensFlare->b3IsActive())
		{
			m_LensFlare = nullptr;
		}
	}

	b3PrintF(B3LOG_FULL, "  preparing nebular...\n");
	nebular = b3GetNebular();
	if (nebular->b3IsActive())
	{
		m_Nebular = nebular;
		b3PrintF(B3LOG_DEBUG, "Using nebular with %3.2f units distance to half value.\n",
			m_Nebular->m_NebularVal);
	}
	else
	{
		m_Nebular = nullptr;
	}

	if (m_BackgroundType == TP_SKY_N_HELL)
	{
		b3PrintF(B3LOG_FULL, "  preparing clouds...\n");
		m_Clouds = b3GetCloudBackground(true);
		m_Clouds->b3PrepareClouds();
	}
	else
	{
		m_Clouds = nullptr;
	}

	b3PrintF(B3LOG_FULL, "  preparing distributed raytracing...\n");
	distributed = b3GetDistributed();
	if (distributed->b3IsActive())
	{
		b3Animation * animation = b3GetAnimation();

		m_Distributed = distributed;
		m_Distributed->b3PrepareAnimation(xSize, animation);
		m_SuperSample = nullptr;
	}
	else
	{
		m_Distributed = nullptr;
		b3PrintF(B3LOG_FULL, "  preparing super sampling...\n");
		supersample = b3GetSuperSample();
		if (supersample->b3IsActive())
		{
			m_SuperSample = supersample;
			b3PrintF(B3LOG_NORMAL, "Using one-level adaptive super sampling.\n");
		}
		else
		{
			m_SuperSample = nullptr;
			b3PrintF(B3LOG_NORMAL, "Using simple sampling.\n");
		}
	}
	if ((m_Distributed != nullptr) || (m_SuperSample != nullptr))
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
	b3PrintF(B3LOG_FULL, "  preparing lights...\n");
	m_LightCount = 0;
	for (light = b3GetLight(); light != nullptr; light = (b3Light *)light->Succ)
	{
		if (!light->b3Prepare(info))
		{
			b3PrintF(B3LOG_NORMAL, "Lights doesn't initialize itself...\n");
			return false;
		}
		if (light->m_LightActive)
		{
			m_LightCount++;
		}
	}

	// Init geometry
	b3PrintF(B3LOG_FULL, "  preparing geometry...\n");
	m_PrepareInfo.b3CollectBBoxes(this);
	if (!m_PrepareInfo.b3Prepare(b3PrepareBBoxThread, info))
	{
		b3PrintF(B3LOG_NORMAL, "Geometry preparation didn't succeed!\n");
		return false;
	}

	b3PrintF(B3LOG_FULL, "  preparing global shader...\n");
	B3_ASSERT(m_Shader != nullptr);
	m_Shader->b3Prepare(info);

	timepoint.b3Now();
	tEnd = timepoint;
	b3PrintF(B3LOG_NORMAL, "Preparing took %3.3fs.\n", tEnd - tStart);

	return (m_BackgroundType == TP_TEXTURE ?
			b3CheckTexture(&m_BackTexture, m_TextureName) :
			true);
}

void b3Scene::b3Raytrace(b3Display * display, bool multi_threaded)
{
	b3Row   *   row;
	b3_res      xSize, ySize;
	b3_count    CPUs, i;
	b3_f64      fy, fyStep;
	bool        isMotionBlur = false;

	try
	{
		b3UpdateCamera();

		// What resolution to use
		display->b3GetRes(xSize, ySize);
		if (!b3PrepareScene(xSize, ySize))
		{
			b3PrintF(B3LOG_NORMAL, "Cannot initialize raytracing!\n");
			return;
		}

		b3ComputeVisibility();

		// Determine CPU count
		CPUs = multi_threaded ? b3Runtime::b3GetNumCPUs() : 1;

		// add rows to list
		fy     = 1.0;
		fyStep = 2.0 / (b3_f64)ySize;
		for (i = 0; i < ySize; i++)
		{
			row = nullptr;
			if (m_Distributed != nullptr)
			{
				isMotionBlur = m_Distributed->b3IsMotionBlur();

				row = isMotionBlur ?
					new b3MotionBlurRayRow(this, display, i, xSize, ySize) :
					new b3DistributedRayRow(this, display, i, xSize, ySize);
			}
			if (m_SuperSample != nullptr)
			{
				row = new b3SupersamplingRayRow(this, display, i, xSize, ySize,
					(b3SupersamplingRayRow *)m_RowPool.Last);
			}

			// Add default row
			if (row == nullptr)
			{
				row = new b3RayRow(this, display, i, xSize, ySize);
			}
			m_RowPool.b3Append(row);
			fy -= fyStep;
		}

		if (isMotionBlur)
		{
			b3DoRaytraceMotionBlur(display, CPUs);
		}
		else
		{
			b3DoRaytrace(display, CPUs);
		}
	}
	catch (b3PrepareException & p)
	{
		b3PrintF(B3LOG_NORMAL, "### Problems preparing scene: %s\n", p.b3GetErrorMsg());
	}
	catch (b3DisplayException & e)
	{
		b3PrintF(B3LOG_NORMAL, "### Error occured: %s\n", e.b3GetErrorMsg());
	}

	// CRITICAL SECTION
	{
		b3CriticalSection lock(m_PoolMutex);

		B3_DELETE_BASE(&m_RowPool, row);
	}
	// CRITICAL SECTION
	{
		b3CriticalSection lock(m_TrashMutex);

		B3_DELETE_BASE(&m_TrashPool, row);
	}

	b3PrintF(B3LOG_NORMAL, "Done.\n\n");
}

void b3Scene::b3AbortRaytrace()
{
	b3Row * row;

	do
	{
		// Enter critical section
		{
			b3CriticalSection lock(m_PoolMutex);

			row = m_RowPool.b3RemoveFirst();
		}
		// Leave critical section

		if (row != nullptr)
		{
			// We can handle the row for its own!
			b3CriticalSection lock(m_TrashMutex);

			m_TrashPool.b3Append(row);
		}
	}
	while (row != nullptr);
}
