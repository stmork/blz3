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

#define MAX_B3TIME_SLICE 100

class b3Time
{
	unsigned long size;
	unsigned long count;
	unsigned long pos;
public:
	      b3Time();
	void  b3Init(unsigned long slice=10);
	void  b3Get(unsigned long &refSpan,unsigned long &refCount);
};

#endif
