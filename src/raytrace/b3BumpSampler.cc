/*
**
**	$Filename:	b3BumpSampler.cc $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Sampler for testing bump maps
**
**	(C) Copyright 2004  Steffen A. Mork
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
#include "blz3/raytrace/b3BumpSampler.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.12  2006/05/23 20:23:41  sm
**	- Some view/bitmap cleanups.
**	- Some more ocean wave ctrl development.
**	- Some preview property page cleanups.
**	- Changed data access methods of b3Tx.
**
**	Revision 1.11  2006/04/29 11:25:49  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**	
**	Revision 1.10  2006/03/05 21:22:35  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.9  2006/02/04 19:19:30  sm
**	- Corrected image sampler. Check the used data
**	  type in constructor by using tx->b3IsHDR or some
**	  sort!
**	- The RenderShapeSampler needs integer frame
**	  buffer for OpenGL use. All other use HDR images.
**	
**	Revision 1.8  2005/12/12 16:01:32  smork
**	- Some more const correction in samplers.
**	
**	Revision 1.7  2005/12/07 10:48:54  smork
**	- Some more const
**	
**	Revision 1.6  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.5  2004/04/24 20:15:52  sm
**	- Further slide material dialog development
**	
**	Revision 1.4  2004/04/23 18:46:17  sm
**	- Fixed bump sampler: Now using initialized derivativs
**	
**	Revision 1.3  2004/04/21 20:44:56  sm
**	- Added bump sampler to their dialogs.
**	- Added bbox dimensions for bump sampler
**	
**	Revision 1.2  2004/04/19 17:52:00  sm
**	- Compile fix for Intel compiler.
**	
**	Revision 1.1  2004/04/19 09:00:52  sm
**	- Added bump sampler.
**	- Reactivated bump sampler in bump dialogs.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

#define BUMP_SLOPE 0.15

b3_vector b3BumpSampler::m_Light =
{
	-1,1,1
};

b3BumpSampler::b3BumpSampler(b3Tx *tx,const b3_count tiles) : m_Tiles(tiles)
{
	// Init texture
	B3_ASSERT(tx->b3IsHDR());

	m_t     = 0;
	m_Scene = null;
	m_Bump  = null;
	m_Tx    = tx;
	m_xMax  = m_Tx->xSize;
	m_yMax  = m_Tx->ySize;
	m_Data  = m_Tx->b3GetHdrData();
}
	
void b3BumpSampler::b3SetBump(b3Bump *bump)
{
	m_Bump = bump;
}

b3SampleInfo *b3BumpSampler::b3SampleInit(const b3_count CPUs)
{
	b3SampleInfo         *info = new b3SampleInfo[CPUs];
	b3_loop               i;
	b3_res                yStart,yEnd;
	b3_color             *data = (b3_color *)m_Data;

	B3_ASSERT(m_Bump != null);
	m_Bump->b3Prepare(this);
	yStart = 0;
	for (i = 0;i < CPUs;i++)
	{
		yEnd = m_yMax * (i + 1) / CPUs;
		info[i].m_Sampler = this;
		info[i].m_Ptr     = m_Bump;
		info[i].m_xMax    = m_xMax;
		info[i].m_yMax    = m_yMax;
		info[i].m_yStart  = yStart;
		info[i].m_yEnd    = yEnd;
		info[i].m_Data    = &data[yStart * m_xMax];
		yStart = yEnd;
	}
	return info;
}

void b3BumpSampler::b3SampleTask(const b3SampleInfo *info)
{
	b3Bump    *bump = (b3Bump *)info->m_Ptr;
	b3BBox     bbox = BBOX;
	b3_coord   x,y;
	b3_ray     ray;
	b3_f64     fy,angle;
	b3_vector  normal;
	b3_color  *data = (b3_color *)info->m_Data;

	ray.bbox = &bbox;
	b3Vector::b3Init(&ray.xDeriv,1.0,0.0,0.0);
	for (y = info->m_yStart;y < info->m_yEnd;y++)
	{
		fy = (b3_f64)y / info->m_yMax;
		for (x = 0;x < info->m_xMax;x++)
		{
			int ix = (m_Tiles * x) / info->m_xMax;
			
			ray.polar.m_BoxPolar.x = fmod((b3_f64)x * m_Tiles / info->m_xMax,1.0);
			ray.polar.m_BoxPolar.y = 1.0 - fy;
			ray.polar.m_BoxPolar.z = 1.0 - fy * BUMP_SLOPE * ix;
			ray.polar.m_ObjectPolar.x = ray.polar.m_BoxPolar.x * 2 - 1;
			ray.polar.m_ObjectPolar.y = ray.polar.m_BoxPolar.y * 2 - 1;
			ray.polar.m_ObjectPolar.z = 0;
			ray.polar.m_Polar = ray.polar.m_ObjectPolar;
			ray.ipoint.x = 100 * ray.polar.m_BoxPolar.x;
			ray.ipoint.y = 100 * ray.polar.m_BoxPolar.y;
			ray.ipoint.z = 100 * ray.polar.m_BoxPolar.z;
			ray.normal.x =  0;
			ray.normal.y = -BUMP_SLOPE * ix;
			ray.normal.z = sqrt(1 - ray.normal.y * ray.normal.y);
			bbox.b3ComputeBoxPolar(&ray);

			b3Vector::b3Init(&ray.yDeriv,0,-ray.normal.z,ray.normal.x);
			bump->b3BumpNormal(&ray);
			b3Vector::b3Init(&normal,&ray.normal);
			angle   = b3Vector::b3AngleOfVectors(&m_Light,&normal);
			*data++ = b3Color(angle,angle,angle);
		}
	}
}
