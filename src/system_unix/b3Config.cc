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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"

#include <unistd.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2002/12/20 15:32:55  sm
**	- Made some ICC optimazations :-)
**
**	Revision 1.3  2001/10/20 16:15:00  sm
**	- Some runtime environment cleanups. The CPU count is determined
**	  only once.
**	- Introduced preparing routines for raytring to shapes.
**	- Found 5% performance loss: No problem, this was eaten by
**	  bug fxing of the rotation spline shapes. (Phuu!)
**	- The next job is to implement different row sampler. Then we
**	  should implemented the base set of the Blizzard II raytracer.
**	
**	Revision 1.2  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Runtime static methods                      **
**                                                                      **
*************************************************************************/

static b3Runtime static_runtime_environment;

b3_cpu_type b3Runtime::cpu_type;
char        b3Runtime::compiler[128];

b3Runtime::b3Runtime()
{
	b3_u32  value = 0x01020304;
	b3_u08 *ptr   = (b3_u08 *)&value;

	cpu_type = (ptr[0] == 0x01 ? B3_BIG_ENDIAN : B3_LITTLE_ENDIAN);
#ifdef __GNUC__
#	ifdef __GNUC_PATCHLEVEL__
	sprintf(compiler,"GCC V%d.%d.%d",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
#	else
	sprintf(compiler,"GCC V%d.%d",__GNUC__,__GNUC_MINOR__);
#	endif
#elif __ICC
	sprintf(compiler,"Intel CC V%d.%d",__ICC / 100,(__ICC / 10) % 10);
#else
	sprintf(compiler,"Unknown compiler");
#endif
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

b3_count b3Runtime::b3GetNumThreads()
{
	return static_runtime_environment.b3CPU::b3GetNumThreads();
}

b3_count b3Runtime::b3GetNumCPUs()
{
	return static_runtime_environment.b3CPU::b3GetNumCPUs();
}


char *b3Runtime::b3GetCompiler()
{
	return compiler;
}
