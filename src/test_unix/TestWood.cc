/*
**
**	$Filename:	TestWood.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Displaying Wood
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/system/b3DisplayView.h"
#include "blz3/base/b3Procedure.h"
#include "blz3/image/b3Tx.h"
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.3  2004/04/08 14:34:42  sm
**  - Multithreading support for wood example.
**
**  Revision 1.2  2004/04/06 12:17:46  sm
**  - Optimized some noise methods.
**
**  Revision 1.1  2004/04/05 09:16:03  sm
**  - Added test wood for Lines wood dialog
**  - Optimized noise a little bit.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestWood implementation                     **
**                                                                      **
*************************************************************************/

#define WOOD_RES   800
#define WOOD_TILES   3

struct b3WoodSampleInfo
{
	b3MatWood    *m_Material;
	b3_res        m_yStart,m_yEnd;
	b3_res        m_xMax;
	b3_res        m_yMax;
	b3_pkd_color *m_Data;
};

class b3WoodSampler
{
	b3MatWood *m_Material;
	b3Display *m_Display;
	b3_res     m_xMax;
	b3_res     m_yMax;

public:
	b3WoodSampler(b3Display *display)
	{
		// Init display
		m_Display = display;
		m_Display->b3GetRes(m_xMax,m_yMax);
		
		// Init material
		m_Material = new b3MatWood(WOOD);
		m_Material->b3Prepare();
	}

	~b3WoodSampler()
	{
		delete m_Material;
	}

	void b3Sample()
	{
		b3_count          CPUs = b3Runtime::b3GetNumCPUs();
		b3_loop           i;
		b3_res            yStart,yEnd;
		b3Tx              tx;
		b3_pkd_color     *data;
		b3TimeSpan        span;
		b3WoodSampleInfo *info;

		tx.b3AllocTx(m_xMax,m_yMax,24);
		data = (b3_pkd_color *)tx.b3GetData();
		info = new b3WoodSampleInfo[CPUs];

		yStart = 0;
		for (i = 0;i < CPUs;i++)
		{
			yEnd = m_yMax * (i + 1) / CPUs;
			info[i].m_Material = m_Material;
			info[i].m_xMax   = m_xMax;
			info[i].m_yMax   = m_yMax;
			info[i].m_yStart = yStart;
			info[i].m_yEnd   = yEnd;
			info[i].m_Data = &data[yStart * m_xMax];
			yStart = yEnd;
		}

		if (CPUs > 1)
		{
			b3Thread *threads = new b3Thread[CPUs];

			span.b3Start();
			for (i = 0;i < CPUs;i++)
			{
				threads[i].b3Start(&b3SampleThread,&info[i]);
			}
			for (i = 0;i < CPUs;i++)
			{
				threads[i].b3Wait();
			}
			span.b3Stop();
			
			delete [] threads;
		}
		else
		{
			span.b3Start();
			b3SampleThread(&info[0]);
			span.b3Stop();
		}

		span.b3Print();

		delete [] info;
		m_Display->b3PutTx(&tx);
	}
private:
	static b3_u32 b3SampleThread(void *ptr)
	{
		b3WoodSampleInfo *info = (b3WoodSampleInfo *)ptr;

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
				int ix = (WOOD_TILES * x) / info->m_xMax;
				polar.box_polar.x = fmod((b3_f64)x * WOOD_TILES / info->m_xMax,1.0);
				polar.box_polar.y = 1.0 - fy;
				polar.box_polar.z = 1.0 - fy * 0.15 * ix;

				info->m_Material->b3GetColors(&polar,diffuse,ambient,specular);
				*data++ = diffuse;
			}
		}
		return 0;
	}
};

int main(int argc,char *argv[])
{
	b3Display   *display;

	b3RaytracingItems::b3Register();
	try
	{
		// Create display
		display = new b3DisplayView(WOOD_RES,WOOD_RES,"Wood");
		b3WoodSampler sampler(display);

		sampler.b3Sample();

		// We want to see the computed picture until we make input
		// into the display window.
		display->b3Wait();

		// Delete Display
		delete display;
	}
	catch(b3DisplayException &e)
	{
		b3PrintF(B3LOG_NORMAL,"### Error occured: %s\n",e.b3GetErrorMsg());
	}
	return 0;
}
