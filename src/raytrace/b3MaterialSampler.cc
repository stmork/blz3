/*
**
**	$Filename:	b3MaterialSampler.h $ 
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

b3MaterialSampler::b3MaterialSampler(b3Tx *tx,b3_count tiles)
{
	// Init texture
	m_Material = null;
	m_Tx       = tx;
	m_xMax     = m_Tx->xSize;
	m_yMax     = m_Tx->ySize;
	m_Data     = (b3_pkd_color *)m_Tx->b3GetData();
	m_Tiles    = tiles;
}
	
void b3MaterialSampler::b3SetMaterial(b3Material *material)
{
	m_Material = material;
}

b3SampleInfo *b3MaterialSampler::b3SampleInit(b3_count CPUs)
{
	b3SampleInfo *info = new b3SampleInfo[CPUs];
	b3_loop       i;
	b3_res        yStart,yEnd;

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
		info[i].m_Data    = &m_Data[yStart * m_xMax];
		yStart = yEnd;
	}
	return info;
}

void b3MaterialSampler::b3SampleTask(b3SampleInfo *info)
{
	b3Material *material = (b3Material *)info->m_Ptr;
	b3_coord              x,y;
	b3_ray                ray;
	b3Color               ambient,diffuse,specular;
	b3_f64                reflection;
	b3_f64                refraction;
	b3_f64                ior;
	b3_f64                spec_exp;
	b3_f64                fy;
	b3_pkd_color         *data = info->m_Data;

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
			material->b3GetColors(&ray,diffuse,ambient,specular,reflection,refraction,ior,spec_exp);

			*data++ = diffuse;
		}
	}
}
