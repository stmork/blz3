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
#include "blz3/system/b3Time.h"
#include "blz3/system/b3CPUBase.h"
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
	static b3_count    m_ThreadCount;
	static b3IPCMutex  m_ThreadMutex;

	b3TimeSpan         m_Span;
	const char        *m_Name;
	b3_u32             m_Result;

protected:
	CWinThread        *m_Thread;
	b3ThreadProc       m_CallProc;
	void              *m_CallArg;
	b3_bool            m_IsRunning;

public:
	              b3Thread(const char *task_name=null);
	             ~b3Thread();
	void          b3Name(const char *task_name=null);
	b3_bool       b3Start(b3ThreadProc,void *custom_ptr,b3_s32 pri=0);
	b3_bool       b3IsRunning();
	b3_bool       b3Stop();
	b3_u32        b3Wait();
	void          b3AddTimeSpan(b3TimeSpan *span);

private:
	       void   b3Inc();
		   void   b3Dec(b3_bool incl_delete = false);
	static b3_u32 b3Trampoline(void *custom_ptr);

	friend class b3CPU;
};

class b3CPU : public b3CPUBase
{
public:
	                b3CPU();
	static b3_count b3GetNumThreads();
};

#endif
