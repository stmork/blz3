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

#include "blz3/raytrace/b3Raytrace.h"

class b3PrepareInfo
{
	friend class b3Scene;

protected:
	b3Base<b3BBoxReference>  m_BBoxRefList;
	b3Array<b3BBoxReference> m_BBoxRefArray;
	b3Mutex                  m_Mutex;

public:
	                 b3PrepareInfo(b3Scene *scene);
	b3BBoxReference *b3GetBBoxReference();
	void             b3RebuildListFromArray();
	b3_bool          b3Prepare(b3ThreadProc prepare_func);
};

#endif
