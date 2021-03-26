/*
**
**	$Filename:	b3PrepareInfo.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Base class for multhithreaded scene initialization
**
**	(C) Copyright 2002  Steffen A. Mork
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

#include "b3RaytraceInclude.h"

/*************************************************************************
**                                                                      **
**                        b3PrepareInfo implementation                  **
**                                                                      **
*************************************************************************/

b3PrepareInfo::b3PrepareInfo()
{
	m_PrepareProc = nullptr;
	m_CPUs        = b3Runtime::b3GetNumCPUs();
	m_Threads     = m_CPUs > 1 ? new b3Thread[m_CPUs] : nullptr;

#ifdef _DEBUG
	m_MinBBoxesForThreading = 0;
#else
	m_MinBBoxesForThreading = B3_MIN_BBOXES_FOR_THREADING;
#endif
}

b3PrepareInfo::~b3PrepareInfo()
{
	if (m_Threads != nullptr)
	{
		delete [] m_Threads;
	}
}

void b3PrepareInfo::b3CollectBBoxes(b3Scene * scene)
{
	b3CollectBBoxes(scene->b3GetFirstBBox());
}

void b3PrepareInfo::b3CollectBBoxes(b3BBox * bbox)
{
	m_BBoxRefArray.b3Clear();
	while (bbox != nullptr)
	{
		bbox->b3CollectBBoxes(m_BBoxRefArray);
		bbox = (b3BBox *)bbox->Succ;
	}

}

b3BBoxReference * b3PrepareInfo::b3GetBBoxReference()
{
	b3CriticalSection lock(m_Mutex);

	return m_BBoxRefList.b3RemoveFirst();
}

void b3PrepareInfo::b3RebuildListFromArray()
{
	b3CriticalSection lock(m_Mutex);

	m_BBoxRefList.b3RemoveAll();
	for (int i = 0; i < m_BBoxRefArray.b3GetCount(); i++)
	{
		m_BBoxRefList.b3Append(&m_BBoxRefArray[i]);
	}
}

b3_u32 b3PrepareInfo::b3PrepareThread(void * ptr)
{
	b3PrepareInfo  *  info = (b3PrepareInfo *)ptr;
	b3BBoxReference * reference;

	while ((reference = info->b3GetBBoxReference()) != nullptr)
	{
		if (!info->m_PrepareProc(reference->m_BBox, info->m_Ptr))
		{
			b3PrintF(B3LOG_NORMAL, "      Object %s didn't prepare successfully!\n",
				reference->m_BBox->b3GetName());
			return 0;
		}
	}
	b3PrintF(B3LOG_FULL, "      Objects prepared successfully!\n");

	return 1;
}

b3_bool b3PrepareInfo::b3Prepare(
	b3PrepareProc  prepare_proc,
	void     *     ptr,
	b3_bool        threaded)
{
	b3_bool  result = true;

	m_PrepareProc = prepare_proc;
	m_Ptr         = ptr;
	B3_ASSERT(m_PrepareProc != nullptr);

	if ((m_Threads != nullptr) && (m_BBoxRefArray.b3GetCount() >= m_MinBBoxesForThreading) && (threaded))
	{
		int i;

		b3RebuildListFromArray();
		b3PrintF(B3LOG_FULL, "    Starting %zd prepare threads\n", m_CPUs);
		for (i = 0; i < m_CPUs; i++)
		{
			if (!m_Threads[i].b3Start(b3PrepareThread, this))
			{
				B3_THROW(b3PrepareException, B3_PREPARE_NO_THREAD);
			}
		}

		b3PrintF(B3LOG_FULL, "    Waiting for prepare threads...\n");
		for (i = 0; i < m_CPUs; i++)
		{
			if (m_Threads[i].b3Wait() == 0)
			{
				result = false;
			}
		}
	}
	else
	{
		b3PrintF(B3LOG_FULL, "    Doing preparation...\n");
		for (int i = 0; (i < m_BBoxRefArray.b3GetCount()) && result; i++)
		{
			result = m_PrepareProc(m_BBoxRefArray[i].m_BBox, m_Ptr);
		}
	}
	b3PrintF(B3LOG_FULL, "    Preparing finished %s.\n",
		result ? "successfully" : "with errors");
	return result;
}
