/*
**
**	$Filename:	b3Thread.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Multithreading routines (for generating dead locks)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_THREAD_H
#define B3_SYSTEM_THREAD_H

#include "blz3/b3Types.h"
#include "stdafx.h"
#include "afxmt.h"

typedef b3_u32 (*b3ThreadProc)(void *);

// To protect critical sections between processes
class b3IPCMutex : public CMutex
{
public:
	bool b3Lock();
	bool b3Unlock();
};

// To protect critical sections inside threads
class b3Mutex
{
	CRITICAL_SECTION cs;
public:
	        b3Mutex();
		   ~b3Mutex();
	b3_bool  b3Lock();
	b3_bool  b3Unlock();
};

// To send other threads a signal
class b3Event : public CEvent
{
public:
	void    b3Pulse();
	b3_bool b3Wait();
};

// Thread handling
class b3Thread
{
	const char     *name;
protected:
	CWinThread     *thread;
	b3ThreadProc    callProc;
	void           *callArg;
	b3_bool         is_running;

public:
	              b3Thread(const char *task_name=null);
	             ~b3Thread();
	void          b3Name(const char *task_name=null);
	b3_bool       b3Start(b3ThreadProc,void *custom_ptr,b3_s32 pri=0);
	b3_bool       b3IsRunning();
	b3_bool       b3Stop();
	void          b3Wait();
	static b3_u32 b3Trampoline(void *custom_ptr);
};

// Info about available CPUs
class b3CPU
{
	b3_count num;
public:
	         b3CPU();
	b3_count b3GetCPU();
	b3_count b3GetNumThreads();
	b3_count b3GetNumCPUs();
};

#endif
