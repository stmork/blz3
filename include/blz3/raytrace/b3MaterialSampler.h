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

#ifndef B3_RAYTRACE_MATERIALSAMPLER_H
#define B3_RAYTRACE_MATERIALSAMPLER_H

#include "blz3/image/b3Sampler.h"
#include "blz3/raytrace/b3Raytrace.h"

#define MATERIAL_TILES 3

class b3MaterialSampler : public b3Sampler
{
protected:
	b3Tx       *m_Tx;
	b3Material *m_Material;

public:
	b3MaterialSampler(b3Tx *tx)
	{
		// Init texture
		m_Material = null;
		m_Tx       = tx;
		m_xMax     = m_Tx->xSize;
		m_yMax     = m_Tx->ySize;
		m_Data     = (b3_pkd_color *)m_Tx->b3GetData();
	}
	
	void b3SetMaterial(b3Material *material)
	{
		m_Material = material;
	}

protected:
	b3SampleInfo * b3SampleInit(b3_count CPUs)
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

	void b3SampleTask(b3SampleInfo *info)
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
				int ix = (MATERIAL_TILES * x) / info->m_xMax;
				polar.box_polar.x = fmod((b3_f64)x * MATERIAL_TILES / info->m_xMax,1.0);
				polar.box_polar.y = 1.0 - fy;
				polar.box_polar.z = 1.0 - fy * 0.15 * ix;

				material->b3GetColors(&polar,diffuse,ambient,specular);
				*data++ = diffuse;
			}
		}
	}
};

#endif
