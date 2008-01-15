/*
**
**	$Filename:	b3MaterialSampler.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3RaytraceInclude.h"
#include "blz3/raytrace/b3MaterialSampler.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3MaterialSampler::b3MaterialSampler(b3Tx *tx, const b3_vector *bbox_size, const b3_count tiles) :
		m_Tiles(tiles),
		m_BBoxSize(bbox_size)
{
	// Init texture
	B3_ASSERT(tx->b3IsHDR());

	m_t        = 0;
	m_Scene    = null;
	m_Material = null;
	m_Tx       = tx;
	m_xMax     = m_Tx->xSize;
	m_yMax     = m_Tx->ySize;
	m_Data     = m_Tx->b3GetHdrData();
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
	m_Material->b3Prepare(this);
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
	ray.Q              = 1;
	surface.m_Incoming = &ray;

	for (y = info->m_yStart;y < info->m_yEnd;y++)
	{
		fy = (b3_f64)y / info->m_yMax;
		for (x = 0;x < info->m_xMax;x++)
		{
			int ix = (m_Tiles * x) / info->m_xMax;

			ray.normal.x = 0;
			ray.normal.y = -0.15 *ix;
			ray.normal.z = 1;
			b3Vector::b3Normalize(&ray.normal);
			ray.polar.m_BoxPolar.x = fmod((b3_f64)x * m_Tiles / info->m_xMax,1.0);
			ray.polar.m_BoxPolar.y = 1.0 - fy;
			ray.polar.m_BoxPolar.z = 1.0 - fy * 0.15 * ix;
			ray.polar.m_ObjectPolar.x = ray.polar.m_BoxPolar.x * 2 - 1;
			ray.polar.m_ObjectPolar.y = ray.polar.m_BoxPolar.y * 2 - 1;
			ray.polar.m_ObjectPolar.z = 0;
			ray.polar.m_Polar = ray.polar.m_ObjectPolar;
			ray.ipoint.x = m_BBoxSize->x * ray.polar.m_BoxPolar.x;
			ray.ipoint.y = m_BBoxSize->y * ray.polar.m_BoxPolar.y;
			ray.ipoint.z = m_BBoxSize->z * ray.polar.m_BoxPolar.z;
			bbox.b3ComputeBoxPolar(&ray);
			material->b3GetSurfaceValues(&surface);

			*data++ = surface.m_Diffuse;
		}
	}
}
