/*
**
**	$Filename:	b3Sampler.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Sample something
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

#include "blz3/image/b3Sampler.h"
#include "blz3/image/b3Tx.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.7  2004/05/16 18:50:59  sm
**  - Added new simple image sampler.
**  - We need better water!
**
**  Revision 1.6  2004/04/09 17:30:31  sm
**  - Wood dialog fine tuning.
**
**  Revision 1.5  2004/04/09 14:11:58  sm
**  - Removed CRs
**
**  Revision 1.4  2004/04/09 14:09:36  sm
**  - Wood sampling corrected.
**  - b3FloatSliderCtrls range computation corrected.
**
**  Revision 1.3  2004/04/09 12:08:05  sm
**  - New CStatic control introduced for material sampling.
**
**  Revision 1.2  2004/04/09 11:09:01  sm
**  - Removed any display reference from sampler
**
**  Revision 1.1  2004/04/09 08:49:16  sm
**  - Splitted up sampler for Lines use and capable for
**    using other metherials.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3Sampler implementation                      **
**                                                                      **
*************************************************************************/

void b3Sampler::b3Sample()
{
	b3_count          CPUs = b3Runtime::b3GetNumCPUs();
	b3_loop           i;
	b3SampleInfo     *info;

	info = b3SampleInit(CPUs);
	if (CPUs > 1)
	{
		b3Thread *threads = new b3Thread[CPUs];

		for (i = 0;i < CPUs;i++)
		{
			threads[i].b3Start(b3SampleThread,&info[i]);
		}
		for (i = 0;i < CPUs;i++)
		{
			threads[i].b3Wait();
		}
			
		delete [] threads;
	}
	else
	{
		b3SampleThread(&info[0]);
	}

	delete [] info;
}
	
b3_u32 b3Sampler::b3SampleThread(void *ptr)
{
	b3SampleInfo *info    = (b3SampleInfo *)ptr;
	b3Sampler    *sampler = info->m_Sampler;

	sampler->b3SampleTask(info);
	return 0;
}

/*************************************************************************
**                                                                      **
**                        b3ImageSampler implementation                 **
**                                                                      **
*************************************************************************/

b3ImageSampler::b3ImageSampler(b3Tx *tx)
{
	// Init texture
	m_xMax  = tx->xSize;
	m_yMax  = tx->ySize;
	m_Data  = (b3_pkd_color *)tx->b3GetData();
}

b3SampleInfo *b3ImageSampler::b3SampleInit(b3_count CPUs)
{
	b3SampleInfo *info = new b3SampleInfo[CPUs];
	b3_loop       i;
	b3_res        yStart,yEnd;

	yStart = 0;
	for (i = 0;i < CPUs;i++)
	{
		yEnd = m_yMax * (i + 1) / CPUs;
		info[i].m_Sampler = this;
		info[i].m_Ptr     = null;
		info[i].m_xMax    = m_xMax;
		info[i].m_yMax    = m_yMax;
		info[i].m_yStart  = yStart;
		info[i].m_yEnd    = yEnd;
		info[i].m_Data    = &m_Data[yStart * m_xMax];
		yStart = yEnd;
	}
	return info;
}

void b3ImageSampler::b3SampleTask(b3SampleInfo *info)
{
	b3_coord      x,y;
	b3_pkd_color *data = info->m_Data;

	for (y = info->m_yStart;y < info->m_yEnd;y++)
	{
		for (x = 0;x < info->m_xMax;x++)
		{
			*data++ = b3SamplePixel(x,y);
		}
	}
}
