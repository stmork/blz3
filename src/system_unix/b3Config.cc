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
**	Revision 1.7  2003/08/28 14:44:26  sm
**	- Further buffer overflow prevention:
**	  o added b3Path::b3Format
**	  o added b3Path::b3Append
**	- Further strcat/strcpy removal necessary
**
**	Revision 1.6  2003/08/27 14:54:23  sm
**	- sprintf changed into snprintf to avoid buffer overflows.
**	
**	Revision 1.5  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
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

char        b3Runtime::compiler[128];

b3Runtime::b3Runtime()
{
#ifdef __GNUC__
#	ifdef __GNUC_PATCHLEVEL__
	snprintf(compiler,sizeof(compiler),"GCC V%d.%d.%d",__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
#	else
	snprintf(compiler,sizeof(compiler),"GCC V%d.%d",__GNUC__,__GNUC_MINOR__);
#	endif
#elif __ICC
	snprintf(compiler,sizeof(compiler),"Intel CC V%d.%d",__ICC / 100,(__ICC / 10) % 10);
#else
	snprintf(compiler,sizeof(compiler),"Unknown compiler");
#endif
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
	char        set[1024];
	const char *fmt;
	b3_s32      result = 127;
	b3_size     offset = sizeof(set) - (async ? 2 : 0);

	if (strlen(command) < offset)
	{
		fmt = async ? "%s &" : "%s";
		snprintf(set,sizeof(set),fmt,command);
		result = system(set);
	}

	return result;
}

char *b3Runtime::b3GetCompiler()
{
	return compiler;
}
