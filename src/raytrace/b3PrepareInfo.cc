/*
**
**	$Filename:	b3PrepareInfo.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Base class for multhithreaded scene initialization
**
**      (C) Copyright 2002  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/raytrace/b3PrepareInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/08/02 11:59:25  sm
**	- b3Thread::b3Wait now returns thread result.
**	- b3Log_SetLevel returns old log level.
**	- Introduced b3PrepareInfo class for multithreaded initialization
**	  support. Should be used for b3AllocVertices and b3ComputeVertices:-)
**	- b3TxPool class is now thread safe.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3PrepareInfo implementation                  **
**                                                                      **
*************************************************************************/

b3PrepareInfo::b3PrepareInfo(b3Scene *scene)
{
	b3BBox *bbox;

	for (bbox = scene->b3GetFirstBBox();bbox != null;bbox = (b3BBox *)bbox->Succ)
	{
		bbox->b3CollectBBoxes(m_BBoxRefArray);
	}
}

b3BBoxReference *b3PrepareInfo::b3GetBBoxReference()
{
	b3BBoxReference *removed;

	m_Mutex.b3Lock();
	removed = m_BBoxRefList.b3RemoveFirst();
	m_Mutex.b3Unlock();
	return removed;
}

void b3PrepareInfo::b3RebuildListFromArray()
{
	m_BBoxRefList.b3RemoveAll();
	for (int i = 0;i < m_BBoxRefArray.b3GetCount();i++)
	{
		m_BBoxRefList.b3Append(&m_BBoxRefArray[i]);
	}
}

b3_bool b3PrepareInfo::b3Prepare(b3ThreadProc prepare_func)
{
	b3_count CPUs   = b3Runtime::b3GetNumCPUs();
	b3_bool  result = true;

	b3RebuildListFromArray();
	if (CPUs > 1)
	{
		b3Thread *threads = new b3Thread[CPUs];
		int       i;

		b3PrintF(B3LOG_FULL,"    Starting %d prepare threads\n",CPUs);
		for (i = 0;i < CPUs;i++)
		{
			threads[i].b3Start(prepare_func,this);
		}
		b3PrintF(B3LOG_FULL,"    Waiting for prepare threads...\n");
		for (i = 0;i < CPUs;i++)
		{
			result &= (threads[i].b3Wait() != 0);
		}
	}
	else
	{
		b3PrintF(B3LOG_FULL,"    Doing prepare thread...\n",CPUs);
		result = prepare_func(this) != 0;
	}
	return result;
}
