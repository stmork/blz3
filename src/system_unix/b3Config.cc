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

#include <unistd.h>

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
	putchar (7);
	fflush (stdout);
}

b3_bool b3Runtime::b3Hostname(char *hostname,const b3_size buffer_size)
{
	return gethostname (hostname, buffer_size) == 0;
}

b3_s32 b3Runtime::b3Execute(const char *command, const b3_bool async)
{
	char   set[1024];
	b3_s32 result = 127;

	if (strlen(command) <= (sizeof(set) - 2))
	{
		strcpy (set,command);
		if (async) strcat(set," &");
		result = system(set);
	}

	return result;
}
