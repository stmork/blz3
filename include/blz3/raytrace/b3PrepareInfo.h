/*
**
**	$Filename:	b3PrepareInfo.h $ 
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
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

#ifndef B3_RAYTRACE_PREPAREINFO_H
#define B3_RAYTRACE_PREPAREINFO_H

#include "blz3/system/b3Thread.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3List.h"

class b3Scene;
class b3BBox;

class b3BBoxReference : public b3Link<b3BBoxReference>
{
public:
	b3BBox *m_BBox;

	b3BBoxReference(b3BBox *bbox = null) : b3Link<b3BBoxReference>(sizeof(b3BBoxReference))
	{
		m_BBox = bbox;
	}
};

enum b3_error_prepare
{
	B3_PREPARE_ERROR = -1,
	B3_PREPARE_OK    =  0,
	B3_PREPARE_NO_MEMORY,
	B3_PREPARE_NO_THREAD
};

typedef b3_bool (*b3PrepareProc)(b3BBox *bbox,void *ptr);
typedef b3Exception<b3_error_prepare,'PRP'> b3PrepareException;

#define B3_MIN_BBOXES_FOR_THREADING 50

class b3PrepareInfo
{
	b3Thread                *m_Threads;
	b3PrepareProc            m_PrepareProc;
	void                    *m_Ptr;
	b3_count                 m_CPUs;

protected:
	b3Base<b3BBoxReference>  m_BBoxRefList;
	b3Array<b3BBoxReference> m_BBoxRefArray;
	b3Mutex                  m_Mutex;
	b3_count                 m_MinBBoxesForThreading;

public:
	                 b3PrepareInfo();
	                ~b3PrepareInfo();
	void             b3CollectBBoxes(b3Scene *scene);
	void             b3CollectBBoxes(b3BBox  *firstBBox);
	b3_bool          b3Prepare(b3PrepareProc prepare_func,void *ptr = null,b3_bool threaded = true);

private:
	b3BBoxReference *b3GetBBoxReference();
	void             b3RebuildListFromArray();
	static b3_u32    b3PrepareThread(void *ptr);
};

#endif
