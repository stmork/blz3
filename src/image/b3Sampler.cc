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
**	(C) Copyright 2004 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "b3TxInclude.h"
#include "blz3/image/b3Sampler.h"

/*************************************************************************
**                                                                      **
**                        b3Sampler implementation                      **
**                                                                      **
*************************************************************************/

void b3Sampler::b3Sample()
{
	const b3_count    CPUs = b3Runtime::b3GetNumCPUs();
	b3_loop           i;
	b3SampleInfo   *  info;

	info = b3SampleInit(CPUs);
	if (CPUs > 1)
	{
		b3Thread * threads = new b3Thread[CPUs];

		for (i = 0; i < CPUs; i++)
		{
			threads[i].b3Start(b3SampleThread, &info[i]);
		}
		for (i = 0; i < CPUs; i++)
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

bool b3Sampler::b3SampleThread(void * ptr)
{
	b3SampleInfo * info    = (b3SampleInfo *)ptr;
	b3Sampler   *  sampler = info->m_Sampler;

	sampler->b3SampleTask(info);
	return false;
}

/*************************************************************************
**                                                                      **
**                        b3ImageSampler implementation                 **
**                                                                      **
*************************************************************************/

b3ImageSampler::b3ImageSampler(b3Tx * tx)
{
	// Init texture
	m_xMax  = tx->xSize;
	m_yMax  = tx->ySize;
	m_Data  = tx->b3GetHdrData();
}

b3SampleInfo * b3ImageSampler::b3SampleInit(const b3_count CPUs)
{
	b3SampleInfo * info = new b3SampleInfo[CPUs];
	b3_loop        i;
	b3_res         yStart, yEnd;
	b3_color   *   data = m_Data;

	yStart = 0;
	for (i = 0; i < CPUs; i++)
	{
		yEnd = m_yMax * (i + 1) / CPUs;
		info[i].m_Sampler = this;
		info[i].m_Ptr     = nullptr;
		info[i].m_xMax    = m_xMax;
		info[i].m_yMax    = m_yMax;
		info[i].m_yStart  = yStart;
		info[i].m_yEnd    = yEnd;
		info[i].m_Data    = &data[yStart * m_xMax];
		yStart = yEnd;
	}
	return info;
}

void b3ImageSampler::b3SampleTask(const b3SampleInfo * info)
{
	b3_coord   x, y;
	b3_color * data = info->m_Data;

	for (y = info->m_yStart; y < info->m_yEnd; y++)
	{
		for (x = 0; x < info->m_xMax; x++)
		{
			*data++ = b3SamplePixel(x, y);
		}
	}
}
