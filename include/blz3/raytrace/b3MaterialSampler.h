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

struct b3MaterialSampleInfo : public b3SampleInfo
{
	b3Material *m_Material;
};

class b3MaterialSampler : public b3Sampler
{
protected:
	b3Material *m_Material;
	b3Display  *m_Display;
	b3Tx        m_Tx;

public:
	b3MaterialSampler(b3Display *display)
	{
		// Init display
		m_Display = display;
		m_Display->b3GetRes(m_xMax,m_yMax);
		
		// Init texture
		m_Tx.b3AllocTx(m_xMax,m_yMax,24);
		m_Data = (b3_pkd_color *)m_Tx.b3GetData();
	}
	
	virtual ~b3MaterialSampler()
	{
		delete m_Material;
	}
	
protected:
	b3SampleInfo * b3SampleInit(b3_count CPUs)
	{
		b3MaterialSampleInfo *info = new b3MaterialSampleInfo[CPUs];
		b3_loop           i;
		b3_res            yStart,yEnd;

		yStart = 0;
		for (i = 0;i < CPUs;i++)
		{
			yEnd = m_yMax * (i + 1) / CPUs;
			info[i].m_Sampler  = this;
			info[i].m_Material = m_Material;
			info[i].m_xMax     = m_xMax;
			info[i].m_yMax     = m_yMax;
			info[i].m_yStart   = yStart;
			info[i].m_yEnd     = yEnd;
			info[i].m_Data     = &m_Data[yStart * m_xMax];
			yStart = yEnd;
		}
		return info;
	}

	void b3SampleTask(b3SampleInfo *ptr)
	{
		b3MaterialSampleInfo * info = (b3MaterialSampleInfo *)ptr;
		b3_coord      x,y;
		b3_polar      polar;
		b3Color       ambient,diffuse,specular;
		b3Tx          tx;
		b3_f64        fy;
		b3_pkd_color *data = info->m_Data;

		for (y = info->m_yStart;y < info->m_yEnd;y++)
		{
			fy = (b3_f64)y / info->m_yMax;
			for (x = 0;x < info->m_xMax;x++)
			{
				int ix = (MATERIAL_TILES * x) / info->m_xMax;
				polar.box_polar.x = fmod((b3_f64)x * MATERIAL_TILES / info->m_xMax,1.0);
				polar.box_polar.y = 1.0 - fy;
				polar.box_polar.z = 1.0 - fy * 0.15 * ix;

				info->m_Material->b3GetColors(&polar,diffuse,ambient,specular);
				*data++ = diffuse;
			}
		}
	}

	void b3SampleDeinit()
	{
		m_Display->b3PutTx(&m_Tx);
	}
};

#endif
