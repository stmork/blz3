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
  
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2002/08/03 18:05:10  sm
**	- Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**	- Moved b3PrepareInfo into b3Scene class as member. This
**	  saves memory allocation calls and is an investment into
**	  faster Lines III object transformation.
**
**	Revision 1.3  2002/08/02 18:55:44  sm
**	- SplineShape weren't be multi threadable - fixed.
**	
**	Revision 1.2  2002/08/02 14:52:13  sm
**	- Vertex/normal computation is now multithreaded, too.
**	- Minor changes on b3PrepareInfo class.
**	- Last changes to Windows port.
**	
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

b3PrepareInfo::b3PrepareInfo()
{
	m_PrepareProc = null;
#ifdef _DEBUG
	m_MinBBoxesForThreading = 0;
#else
	m_MinBBoxesForThreading = B3_MIN_BBOXES_FOR_THREADING;
#endif
}

void b3PrepareInfo::b3CollectBBoxes(b3Scene *scene)
{
	b3CollectBBoxes(scene->b3GetFirstBBox());
}

void b3PrepareInfo::b3CollectBBoxes(b3BBox *bbox)
{
	m_BBoxRefArray.b3Clear();
	while(bbox != null)
	{
		bbox->b3CollectBBoxes(m_BBoxRefArray);
		bbox = (b3BBox *)bbox->Succ;
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
	m_Mutex.b3Lock();
	m_BBoxRefList.b3RemoveAll();
	for (int i = 0;i < m_BBoxRefArray.b3GetCount();i++)
	{
		m_BBoxRefList.b3Append(&m_BBoxRefArray[i]);
	}
	m_Mutex.b3Unlock();
}

b3_u32 b3PrepareInfo::b3PrepareThread(void *ptr)
{
	b3PrepareInfo   *info = (b3PrepareInfo *)ptr;
	b3BBoxReference *reference;

	while (reference = info->b3GetBBoxReference())
	{
		if (!info->m_PrepareProc(reference->m_BBox))
		{
			return 0;
		}
	}
	return 1;
}

b3_bool b3PrepareInfo::b3Prepare(b3PrepareProc prepare_proc)
{
	b3_count CPUs   = b3Runtime::b3GetNumCPUs();
	b3_bool  result = true;

	b3RebuildListFromArray();
	m_PrepareProc = prepare_proc;
	if ((CPUs > 1) && (m_BBoxRefArray.b3GetCount() >= m_MinBBoxesForThreading))
	{
		b3Thread *threads = new b3Thread[CPUs];
		int       i;

		b3PrintF(B3LOG_FULL,"    Starting %d prepare threads\n",CPUs);
		for (i = 0;i < CPUs;i++)
		{
			threads[i].b3Start(b3PrepareThread,this);
		}
		b3PrintF(B3LOG_FULL,"    Waiting for prepare threads...\n");
		for (i = 0;i < CPUs;i++)
		{
			result &= (threads[i].b3Wait() != 0);
		}

		delete [] threads;
	}
	else
	{
		b3PrintF(B3LOG_FULL,"    Doing prepare thread...\n",CPUs);
		for (int i = 0;(i < m_BBoxRefArray.b3GetCount()) && result;i++)
		{
			result = m_PrepareProc(m_BBoxRefArray[i].m_BBox);
		}
	}
	return result;
}
