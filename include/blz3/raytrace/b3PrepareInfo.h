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

class b3Scene;
class b3BBox;

typedef b3_bool (*b3PrepareProc)(b3BBox *bbox);

#define B3_MIN_BBOXES_FOR_THREADING 50

class b3BBoxReference : public b3Link<b3BBoxReference>
{
public:
	b3BBox *m_BBox;

	b3BBoxReference(b3BBox *bbox = null) : b3Link<b3BBoxReference>(sizeof(b3BBoxReference))
	{
		m_BBox = bbox;
	}
};

class b3PrepareInfo
{
	friend class b3Scene;

	b3PrepareProc            m_PrepareProc;

protected:
	b3Base<b3BBoxReference>  m_BBoxRefList;
	b3Array<b3BBoxReference> m_BBoxRefArray;
	b3Mutex                  m_Mutex;
	b3_count                 m_MinBBoxesForThreading;

public:
	                 b3PrepareInfo();
	void             b3CollectBBoxes(b3Scene *scene);
	void             b3CollectBBoxes(b3BBox  *firstBBox);
	b3_bool          b3Prepare(b3PrepareProc prepare_func);

private:
	b3BBoxReference *b3GetBBoxReference();
	void             b3RebuildListFromArray();
	static b3_u32    b3PrepareThread(void *ptr);
};

#endif
