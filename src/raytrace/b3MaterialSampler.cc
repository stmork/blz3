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
	b3_polar              polar;
	b3Color               ambient,diffuse,specular;
	b3_f64                fy;
	b3_pkd_color         *data = info->m_Data;

	for (y = info->m_yStart;y < info->m_yEnd;y++)
	{
		fy = (b3_f64)y / info->m_yMax;
		for (x = 0;x < info->m_xMax;x++)
		{
			int ix = (m_Tiles * x) / info->m_xMax;
			polar.box_polar.x = fmod((b3_f64)x * m_Tiles / info->m_xMax,1.0);
			polar.box_polar.y = 1.0 - fy;
			polar.box_polar.z = 1.0 - fy * 0.15 * ix;
			polar.object_polar.x = polar.box_polar.x * 2 - 1;
			polar.object_polar.y = polar.box_polar.y * 2 - 1;
			polar.object_polar.z = 0;
			material->b3GetColors(&polar,diffuse,ambient,specular);

			*data++ = diffuse;
		}
	}
}
