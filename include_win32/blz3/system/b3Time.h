/*
**
**	$Filename:	b3Time.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - time control
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_TIME_H
#define B3_SYSTEM_TIME_H

#include <sys/timeb.h>

#include "blz3/b3Config.h"

#define MAX_B3TIME_SLICE 100

class b3Time
{
	b3_u32        size;
	b3_u32        count;
	b3_u32        pos;
	struct _timeb buffer[MAX_B3TIME_SLICE];
public:
	      b3Time();
	void  b3Init(b3_u32 slice=10);
	void  b3Get(b3_u32 &refSpan,b3_u32 &refCount);
};

#endif
