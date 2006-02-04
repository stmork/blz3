/*
**
**	$Filename:	b3MaterialSampler.cc $ 
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Sampler for testing materials
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

#include "blz3/raytrace/b3MaterialSampler.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.11  2006/02/04 19:19:30  sm
**	- Corrected image sampler. Check the used data
**	  type in constructor by using tx->b3IsHDR or some
**	  sort!
**	- The RenderShapeSampler needs integer frame
**	  buffer for OpenGL use. All other use HDR images.
**
**	Revision 1.10  2005/12/12 16:01:32  smork
**	- Some more const correction in samplers.
**	
**	Revision 1.9  2005/12/07 10:48:54  smork
**	- Some more const
**	
**	Revision 1.8  2004/10/05 09:29:22  sm
**	- Donw some documentations.
**	
**	Revision 1.7  2004/05/22 14:17:31  sm
**	- Merging some basic raytracing structures and gave them some
**	  self explaining names. Also cleaned up some parameter lists.
**	
**	Revision 1.6  2004/05/09 15:06:56  sm
**	- Added inverse transformation for mapping.
**	- Unified scale mapping source via b3Scaling.
**	- Moved b3Scaling in its own files.
**	- Added property pages for scaling and removed
**	  scaling input fields from dialogs.
**	
**	Revision 1.5  2004/04/19 09:00:52  sm
**	- Added bump sampler.
**	- Reactivated bump sampler in bump dialogs.
**	
**	Revision 1.4  2004/04/11 19:04:21  sm
**	- Renamed b3Material::b3GetColors into b3Material::b3GetSurfaceValues
**	
**	Revision 1.3  2004/04/11 18:21:36  sm
**	- Raytracer redesign:
**	  o The complete set of surface values moved into
**	    the b3_surface data structure when calling b3GetColors()
**	
**	Revision 1.2  2004/04/11 14:05:11  sm
**	- Raytracer redesign:
**	  o The reflection/refraction/ior/specular exponent getter
**	    are removed. The values are copied via the b3GetColors()
**	    method.
**	  o The polar members are renamed.
**	  o The shape/bbox pointers moved into the ray structure
**	- Introduced wood bump mapping.
**	
**	Revision 1.1  2004/04/10 19:12:46  sm
**	- Splitted up some header/source files:
**	  o b3Wood/b3OakPlank
**	  o b3MaterialSampler
**	- Cleaneup
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3MaterialSampler::b3MaterialSampler(b3Tx *tx, const b3_count tiles) : m_Tiles(tiles)
{
	// Init texture
	B3_ASSERT(tx->b3IsHDR());

	m_Material = null;
	m_Tx       = tx;
	m_xMax     = m_Tx->xSize;
	m_yMax     = m_Tx->ySize;
	m_Data     = m_Tx->b3GetData();
}
	
void b3MaterialSampler::b3SetMaterial(b3Material *material)
{
	m_Material = material;
}

b3SampleInfo *b3MaterialSampler::b3SampleInit(const b3_count CPUs)
{
	b3SampleInfo *info = new b3SampleInfo[CPUs];
	b3_loop       i;
	b3_res        yStart,yEnd;
	b3_color     *data = (b3_color *)m_Data;

	B3_ASSERT(m_Material != null);
	m_Material->b3Prepare();
	yStart = 0;
	for (i = 0;i < CPUs;i++)
	{
		yEnd = m_yMax * (i + 1) / CPUs;
		info[i].m_Sampler = this;
		info[i].m_Ptr     = m_Material;
		info[i].m_xMax    = m_xMax;
		info[i].m_yMax    = m_yMax;
		info[i].m_yStart  = yStart;
		info[i].m_yEnd    = yEnd;
		info[i].m_Data    = &data[yStart * m_xMax];
		yStart = yEnd;
	}
	return info;
}

void b3MaterialSampler::b3SampleTask(const b3SampleInfo *info)
{
	b3Material *material = (b3Material *)info->m_Ptr;
	b3BBox      bbox = BBOX;
	b3_coord    x,y;
	b3_ray      ray;
	b3_surface  surface;
	b3_f64      fy;
	b3_color   *data = (b3_color *)info->m_Data;

	ray.bbox           = &bbox;
	surface.m_Incoming = &ray;
	bbox.b3Prepare();
	for (y = info->m_yStart;y < info->m_yEnd;y++)
	{
		fy = (b3_f64)y / info->m_yMax;
		for (x = 0;x < info->m_xMax;x++)
		{
			int ix = (m_Tiles * x) / info->m_xMax;
			
			ray.polar.m_BoxPolar.x = fmod((b3_f64)x * m_Tiles / info->m_xMax,1.0);
			ray.polar.m_BoxPolar.y = 1.0 - fy;
			ray.polar.m_BoxPolar.z = 1.0 - fy * 0.15 * ix;
			ray.polar.m_ObjectPolar.x = ray.polar.m_BoxPolar.x * 2 - 1;
			ray.polar.m_ObjectPolar.y = ray.polar.m_BoxPolar.y * 2 - 1;
			ray.polar.m_ObjectPolar.z = 0;
			ray.polar.m_Polar = ray.polar.m_ObjectPolar;
			ray.ipoint.x = 100 * ray.polar.m_BoxPolar.x;
			ray.ipoint.y = 100 * ray.polar.m_BoxPolar.y;
			ray.ipoint.z = 100 * ray.polar.m_BoxPolar.z;
			bbox.b3ComputeBoxPolar(&ray);
			material->b3GetSurfaceValues(&surface);

			*data++ = surface.m_Diffuse;
		}
	}
}
