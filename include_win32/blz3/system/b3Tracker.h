/*
**
**	$Filename:	b3Tracker.h $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - tracker control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_RACKER_H
#define B3_SYSTEM_TRACKER_H

#include "blz3/b3Config.h"
#include "blz3/image/b3Tx.h"

class CB3Tracker : public CRectTracker, public CObject
{
protected:
	b3_bool       create;
	b3Tx         *tx;
public:
	// virtual ovverride from CRectTracker
	              CB3Tracker(b3Tx *);
	        void  b3Create(b3_bool mode=true);
	virtual void  AdjustRect(int,RECT *);
};

#endif
