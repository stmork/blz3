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
	b3TimeSpan        span;
	b3SampleInfo     *info;

	info = b3SampleInit(CPUs);
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
}
	
b3_u32 b3Sampler::b3SampleThread(void *ptr)
{
	b3SampleInfo *info = (b3SampleInfo *)ptr;
	
	info->m_Sampler->b3SampleTask(info);
	return 0;
}

