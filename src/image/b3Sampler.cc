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
