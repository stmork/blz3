/*
**
**      $Filename:      b3Config.c $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - basic platform configuration
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
*/

#include "blz3/b3Config.h"

static b3Runtime static_runtime_environment;

b3Runtime::b3Runtime()
{
	b3_u32  value = 0x01020304;
	b3_u08 *ptr   = (b3_u08 *)&value;

	cpu_type = (ptr[0] == 0x01 ? B3_BIG_ENDIAN : B3_LITTLE_ENDIAN);
}

b3_cpu_type b3Runtime::b3GetCPUType()
{
	return static_runtime_environment.cpu_type;
}

void b3Runtime::b3PSwap(
	b3_u32 *uPtr1,
	b3_u32 *uPtr2)
{
	b3_u32 aux;

	aux    = *uPtr1;
	*uPtr1 = *uPtr2;
	*uPtr2 = aux;
}

void b3Runtime::b3Beep()
{
	MessageBeep(0xffffffff);
}

b3_bool b3Runtime::b3Hostname(char *hostname,const b3_size buffer_size)
{
	unsigned long size = buffer_size;

	return GetComputerName (hostname,&size);
}

b3_s32 b3Runtime::b3Execute(const char *commandLine, const b3_bool async)
{
	PROCESS_INFORMATION procInfo;
	STARTUPINFO         strtInfo;

	strtInfo.cb          = sizeof(strtInfo);
	strtInfo.lpReserved  = null;
	strtInfo.lpDesktop   = null;
	strtInfo.lpTitle     = null;
	strtInfo.dwFlags     = 0;
	strtInfo.cbReserved2 = 0;
	strtInfo.lpReserved2 = null;
	if (!CreateProcess (null,(char *)commandLine,null,null,FALSE,
		CREATE_DEFAULT_ERROR_MODE,
		null,null,&strtInfo,&procInfo))
	{
		b3Beep();
	}
	else
	{
		if (!async) WaitForSingleObject (procInfo.hProcess,INFINITE);
	}
	return 0;
}
