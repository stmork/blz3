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

#include <stdafx.h>
#include <sys/timeb.h>

#include "blz3/b3Types.h"
#include "blz3/system/b3Log.h"

#define B3_MAX_TIME_SLICE 100

class b3TimeAccum
{
	b3_u32        size;
	b3_u32        count;
	b3_u32        pos;
	struct _timeb buffer[B3_MAX_TIME_SLICE];
public:
	      b3TimeAccum();
	void  b3Init(b3_u32 slice=10);
	void  b3Get(b3_u32 &refSpan,b3_u32 &refCount);
};

class b3TimeSpan
{
	HANDLE        m_ThreadHandle;
	FILETIME      m_uStart;
	FILETIME      m_sStart;
	struct timeb  m_RealTime;

	public:
	b3_s32        m_uTime;
	b3_s32        m_sTime;
	b3_s32        m_rTime;

public:
	               b3TimeSpan();
	       void    b3Start();
	       void    b3Stop();
	       void    b3Print(b3_log_level level = B3LOG_NORMAL);
	       b3_f64  b3GetUsage();
private:
	static b3_u32  b3Diff(FILETIME *first,FILETIME *last);
	static char   *b3PrintTime(char *buffer,b3_s32 time_needed);
};

#endif
