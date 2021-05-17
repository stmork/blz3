/*
**
**	$Filename:	b3RayRow.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Raytracing row processing
**
**	(C) Copyright 2002  Steffen A. Mork
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

#define DEBUG_SS4

/*************************************************************************
**                                                                      **
**                        Compute one simple row...                     **
**                                                                      **
*************************************************************************/

b3RayRow::b3RayRow(
	b3Scene  * scene,
	b3Display * display,
	const b3_coord   y,
	const b3_res     xSize,
	const b3_res     ySize) :
	b3Row(y, xSize), m_ySize(ySize)
{
	m_Display = display;
	m_Scene   = scene;
	m_Shader  = scene->b3GetShader();
	m_t       = scene->b3GetTimePoint();

	// Init direction
	m_fy        =  1.0 - m_y * 2.0 / (b3_f64)m_ySize;
	m_fxStep    =  2.0 / (b3_f64)m_xSize;
	m_preDir.x  = (m_Scene->m_ViewPoint.x - m_Scene->m_EyePoint.x) - m_Scene->m_Width.x + m_fy * m_Scene->m_Height.x;
	m_preDir.y  = (m_Scene->m_ViewPoint.y - m_Scene->m_EyePoint.y) - m_Scene->m_Width.y + m_fy * m_Scene->m_Height.y;
	m_preDir.z  = (m_Scene->m_ViewPoint.z - m_Scene->m_EyePoint.z) - m_Scene->m_Width.z + m_fy * m_Scene->m_Height.z;
}

inline b3Color & b3RayRow::b3Shade(b3_ray * ray, const b3_f64 fx, const b3_f64 fy)
{
	if (!m_Shader->b3Shade(ray))
	{
		m_Scene->b3GetBackgroundColor(ray, fx, fy);
	}

	if (m_Scene->m_LensFlare != nullptr)
	{
		m_Scene->b3MixLensFlare(ray);
	}

	return ray->color;
}

void b3RayRow::b3Raytrace()
{
	b3_res   x;
	b3_ray   ray;
	b3_f64   fx;

	// Init eye position
	ray.pos.x  =  m_Scene->m_EyePoint.x;
	ray.pos.y  =  m_Scene->m_EyePoint.y;
	ray.pos.z  =  m_Scene->m_EyePoint.z;
	fx         = -1;

	// Loop one row...
	for (x = 0; x < m_xSize; x++)
	{
		ray.dir    = m_preDir;
		ray.inside = false;

		m_buffer[x] = b3Shade(&ray, fx, m_fy);

		b3Vector::b3Add(&m_Scene->m_xStepDir, &m_preDir);
		fx         += m_fxStep;

	}
	m_Display->b3PutRow(this);
	if (m_Display->b3IsCancelled(m_xSize - 1, m_y))
	{
		m_Scene->b3AbortRaytrace();
	}
}

/*************************************************************************
**                                                                      **
**                        Compute a row with adaptive one level         **
**                        super sampling                                **
**                                                                      **
*************************************************************************/

b3SupersamplingRayRow::b3SupersamplingRayRow(
	b3Scene        *       scene,
	b3Display       *      display,
	const b3_coord               y,
	const b3_res                 xSize,
	const b3_res                 ySize,
	b3SupersamplingRayRow * last) :
	b3RayRow(scene, display, y, xSize, ySize)
{
	m_Limit      = m_Scene->m_SuperSample->m_Limit;
	m_ThisResult = new b3Color[xSize];
	m_RowState   = B3_STATE_NOT_STARTED;

	m_PrevRow = last;
	m_SuccRow = nullptr;
	if (m_PrevRow != nullptr)
	{
		m_LastResult = m_PrevRow->m_ThisResult;
		m_PrevRow->m_SuccRow = this;
	}
	else
	{
		m_LastResult = nullptr;
	}
#ifndef DEBUG_SS4
	m_Debug = true;
#else
	m_Debug = false;
#endif
}

b3SupersamplingRayRow::~b3SupersamplingRayRow()
{
	delete [] m_ThisResult;
}

void b3SupersamplingRayRow::b3Raytrace()
{
	b3_res        x;
	b3_ray        ray;
	b3_f64        fxRight = -1;
	b3_vector64   dir;
	bool          do_convert = false;
	bool          do_refine  = false;
	bool          do_refine_succ = false;

	// Init eye position
	m_RowState = B3_STATE_COMPUTING;
	ray.pos.x  =  m_Scene->m_EyePoint.x;
	ray.pos.y  =  m_Scene->m_EyePoint.y;
	ray.pos.z  =  m_Scene->m_EyePoint.z;
	dir        =  m_preDir;

	// Loop one row...
	for (x = 0; x < m_xSize; x++)
	{
		ray.dir    = dir;
		ray.inside = false;

		m_ThisResult[x] = b3Shade(&ray, fxRight, m_fy);

		fxRight += m_fxStep;
		b3Vector::b3Add(&m_Scene->m_xStepDir, &dir);
	}

	// CRITICAL SECTION
	{
		b3CriticalSection lock(m_Scene->m_SamplingMutex);
		m_RowState = B3_STATE_CHECK;

		if (m_PrevRow == nullptr)
		{
			// This is the first row...
			do_convert = true;
			m_RowState = B3_STATE_READY;
			if ((m_SuccRow != nullptr) && (m_SuccRow->m_RowState == B3_STATE_CHECK))
			{
				do_refine_succ = true;
				m_SuccRow->m_RowState = B3_STATE_REFINING;
			}
		}
		else
		{
			// The previous one is already OK -> Refine this.
			if (m_PrevRow->m_RowState == B3_STATE_READY)
			{
				do_refine = true;
				m_RowState = B3_STATE_REFINING;
			}
		}
	}

	if (do_convert)
	{
		b3Convert();
	}
	if (do_refine)
	{
		b3Refine(true);
	}
	if (do_refine_succ)
	{
		m_SuccRow->b3Refine(false);
	}

	if (m_RowState == B3_STATE_CHECK)
	{
		b3Convert();
	}
}

inline bool b3SupersamplingRayRow::b3Test(const b3_res x)
{
	b3Color diff = m_LastResult[x] - m_ThisResult[x];

	diff.b3Abs();
	return diff >= m_Limit;
}

inline void b3SupersamplingRayRow::b3Convert()
{
	b3_res x;

	if (!m_Debug)
	{
		for (x = 0; x < m_xSize; x++)
		{
			m_buffer[x] = m_ThisResult[x];
		}
	}
	else
	{
		for (x = 0; x < m_xSize; x++)
		{
			m_buffer[x] = b3Color(B3_BLUE);
		}
	}

	m_Display->b3PutRow(this);
	if (m_Display->b3IsCancelled(m_xSize - 1, m_y))
	{
		m_Scene->b3AbortRaytrace();
	}
}

inline void b3SupersamplingRayRow::b3Refine(const bool this_row)
{
	b3_ray        ray;
	b3_res        x;
	b3_vector64   dir;
	b3_f64        fxLeft, fxRight, fyUp, fyDown;
	bool          do_refine_succ = false;
	b3_pkd_color  result = B3_BLACK;

	B3_ASSERT(m_RowState != B3_STATE_READY);

	// Init eye position
	ray.pos.x  = m_Scene->m_EyePoint.x;
	ray.pos.y  = m_Scene->m_EyePoint.y;
	ray.pos.z  = m_Scene->m_EyePoint.z;
	ray.dir    = dir = m_preDir;

	// Init coord values
	fxRight = -1;
	fxLeft  = fxRight - 0.5 * m_fxStep;
	fyDown  = m_fy;
	fyUp    = fyDown + 1.0 / (b3_f64)m_ySize;

	for (x = 0; x < m_xSize; x++)
	{
		const bool add = (x > 0 ? b3Test(x) : false);

		if (m_Debug)
		{
			if (x > 0)
			{
				result = (add ?
						(this_row ?
							B3_GREEN :
							B3_RED) :
						B3_GREY);
			}
			else
			{
				result = B3_BLUE;
			}
		}

		// Do the additional computations...
		if (add)
		{
			ray.dir    = *b3Vector::b3Add(&m_Scene->m_yHalfDir, &dir);
			ray.inside = false;

			m_ThisResult[x] += b3Shade(&ray, fxRight, fyUp);

			ray.dir    = *b3Vector::b3Sub(&m_Scene->m_xHalfDir, &dir);
			ray.inside = false;

			m_ThisResult[x] += b3Shade(&ray, fxLeft, fyUp);

			ray.dir    = *b3Vector::b3Sub(&m_Scene->m_yHalfDir, &dir);
			ray.inside = false;

			m_ThisResult[x]  = (m_ThisResult[x] + b3Shade(&ray, fxLeft, fyDown)) * 0.25f;

			ray.dir = *b3Vector::b3Add(&m_Scene->m_xHalfDir, &dir);
		}
		ray.dir    = *b3Vector::b3Add(&m_Scene->m_xStepDir, &dir);
		fxRight   += m_fxStep;
		fxLeft    += m_fxStep;

		m_buffer[x] = m_Debug ? b3Color(result) : m_ThisResult[x];
	}

	// CRITICAL SECTION
	{
		b3CriticalSection lock(m_Scene->m_SamplingMutex);

		m_RowState = B3_STATE_READY;
		if ((m_SuccRow != nullptr) && (m_SuccRow->m_RowState == B3_STATE_CHECK))
		{
			do_refine_succ = true;
			m_SuccRow->m_RowState = B3_STATE_REFINING;
		}
	}

	if (do_refine_succ)
	{
		m_SuccRow->b3Refine(false);
	}

	m_Display->b3PutRow(this);
	if (m_Display->b3IsCancelled(m_xSize - 1, m_y))
	{
		m_Scene->b3AbortRaytrace();
	}
}

/*************************************************************************
**                                                                      **
**                        Compute a row with adaptive one level         **
**                        super sampling                                **
**                                                                      **
*************************************************************************/

b3DistributedRayRow::b3DistributedRayRow(
	b3Scene  * scene,
	b3Display * display,
	const b3_coord   y,
	const b3_res     xSize,
	const b3_res     ySize) :
	b3RayRow(scene, display, y, xSize, ySize)
{
	m_Distr    = scene->m_Distributed;
	m_Samples  = m_Distr->m_Samples;
	m_SPP      = m_Distr->m_SPP;
	m_SPF      = m_Distr->m_SamplesPerFrame;
	m_xHalfDir = &scene->m_xHalfDir;
	m_yHalfDir = &scene->m_yHalfDir;
}

b3DistributedRayRow::~b3DistributedRayRow()
{
}

void b3DistributedRayRow::b3Raytrace()
{
	b3_ray        ray;
	b3_coord      x, s;
	b3Color       result;
	b3_f64        fx, sx, sy;
	b3_f32    *   samples = m_Samples;

	// Init eye position
	ray.pos.x  =  m_Scene->m_EyePoint.x;
	ray.pos.y  =  m_Scene->m_EyePoint.y;
	ray.pos.z  =  m_Scene->m_EyePoint.z;
	fx         = -1;

	for (x = 0; x < m_xSize; x++)
	{
		result.b3Init();
		for (s = 0; s < m_SPP; s++)
		{
			sx = *samples++;
			sy = *samples++;

			b3Vector::b3LinearCombine(&m_preDir, &m_Scene->m_xHalfDir, &m_Scene->m_yHalfDir, sx, sy, &ray.dir);
			ray.inside = false;

			result += b3Shade(&ray, fx + sx, m_fy + sy);
		}
		b3Vector::b3Add(&m_Scene->m_xStepDir, &m_preDir);
		fx         += m_fxStep;

		m_buffer[x] = result / m_SPP;
	}
	m_Display->b3PutRow(this);
	if (m_Display->b3IsCancelled(m_xSize - 1, m_y))
	{
		m_Scene->b3AbortRaytrace();
	}
}

/*************************************************************************
**                                                                      **
**                        Compute a row with adaptive one level         **
**                        super sampling                                **
**                                                                      **
*************************************************************************/

b3MotionBlurRayRow::b3MotionBlurRayRow(
	b3Scene     *    scene,
	b3Display    *   display,
	const b3_coord   y,
	const b3_res     xSize,
	const b3_res     ySize) :
	b3DistributedRayRow(scene, display, y, xSize, ySize)
{
	b3_coord x;

	m_Color = new b3Color[m_xSize];
	for (x = 0; x < m_xSize; x++)
	{
		m_Color[x].b3Init();
	}
	if (m_Color == nullptr)
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}

	m_TimeIndex = &m_Distr->m_TimeIndex[0];
	m_Index     = 0;
	m_BackupDir = m_preDir;
	m_Modulo    = m_xSize * m_SPP;
	m_Start     = B3_IRAN(m_Modulo);
}

b3MotionBlurRayRow::~b3MotionBlurRayRow()
{
	delete [] m_Color;
}

void b3MotionBlurRayRow::b3SetTimePoint(b3_f64 t)
{
	m_t = t;
}

void b3MotionBlurRayRow::b3Raytrace()
{
	b3_ray        ray;
	b3_coord      x, s;
	b3_f64        fx, sx, sy;
	b3_f32    *   samples = m_Samples;
	b3_index      count = 0;

	// Init eye position
	ray.pos.x  =  m_Scene->m_EyePoint.x;
	ray.pos.y  =  m_Scene->m_EyePoint.y;
	ray.pos.z  =  m_Scene->m_EyePoint.z;
	fx         = -1;

	m_preDir = m_BackupDir;
	for (x = 0; x < m_xSize; x++)
	{
		for (s = 0; s < m_SPP; s++)
		{
			if (m_TimeIndex[(count++ + m_Start) % m_Modulo] == m_Index)
			{
				sx = *samples++;
				sy = *samples++;

				b3Vector::b3LinearCombine(&m_preDir, &m_Scene->m_xHalfDir, &m_Scene->m_yHalfDir, sx, sy, &ray.dir);
				ray.inside = false;

				m_Color[x]  += b3Shade(&ray, fx + sx, m_fy + sy);
				m_buffer[x]  = m_Color[x] / m_SPP;
			}
			else
			{
				samples += 2;
			}
		}

		b3Vector::b3Add(&m_Scene->m_xStepDir, &m_preDir);
		fx         += m_fxStep;
	}
	m_Display->b3PutRow(this);
	if (m_Display->b3IsCancelled(m_xSize - 1, m_y))
	{
		m_Scene->b3AbortRaytrace();
	}

	m_Index++;
}
