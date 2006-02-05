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
#include <dlfcn.h>
#include <locale.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.20  2006/02/05 18:57:14  sm
**	- Added locale support.
**
**	Revision 1.19  2005/12/19 10:45:18  sm
**	- Some locale specified.
**	
**	Revision 1.18  2005/10/23 12:30:30  sm
**	- Added some informations about vector units.
**	
**	Revision 1.17  2005/06/09 11:00:57  smork
**	- Call option cleanup.
**	
**	Revision 1.16  2005/06/09 09:24:00  smork
**	- Added image conversion tool to installation.
**	
**	Revision 1.15  2005/06/07 11:02:37  sm
**	- Marking math mode in compiler version output.
**	
**	Revision 1.14  2005/05/04 09:27:28  mork
**	- Corrected some string methods in configure script.
**	
**	Revision 1.13  2005/01/04 15:13:59  smork
**	- Changed some data types.
**	
**	Revision 1.12  2004/11/13 19:47:15  sm
**	- Corrected some OpenGL exclusions.
**	
**	Revision 1.11  2004/09/23 09:31:33  sm
**	- Changed b3Runtime into a real singleton.
**	- Added functions for OpenGL extension.
**	- Removed b3PrintF()s from singletons.
**	
**	Revision 1.10  2004/06/22 11:12:20  sm
**	- Added support for Intel C++ compiler V8
**	
**	Revision 1.9  2004/01/18 13:51:58  sm
**	- Done further security issues.
**	
**	Revision 1.8  2003/11/01 09:47:26  sm
**	- Added CPU bit version with compiler version string.
**	
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

b3Runtime b3Runtime::m_Runtime;
char      b3Runtime::m_Compiler[256];
b3Runtime::b3Runtime()
{
	char     *vu;
	b3_count  bits = b3GetCPUBits();
#if defined(B3_SSE2)
	char *math = "SSE2";
#elif defined(B3_SSE)
	char *math = "SSE";
#else
	char *math = "FPU";
#endif
	char *locale;

	locale = setlocale(LC_ALL, "");
	if (locale == null)
	{
		fprintf(stderr, "Cannot set locale. Assuming w're right ;-)\n");
	}
	else
	{
#ifdef _DEBUG
		fprintf(stderr, "Default locale: %s\n", locale);
#endif
	}

	switch (b3GetVectorUnit())
	{
	case B3_VU_FPU:     vu = "FPU"; break;
	case B3_VU_MMX:     vu = "MMX"; break;
	case B3_VU_SSE:     vu = "SSE"; break;
	case B3_VU_SSE2:    vu = "SSE2"; break;
	case B3_VU_SSE3:    vu = "SSE3"; break;
	case B3_VU_3DNOW:   vu = "3DNow!"; break;
	case B3_VU_ALTIVEC: vu = "AltiVec"; break;
	}

#ifdef __ICC
	snprintf(m_Compiler,sizeof(m_Compiler),"Intel CC V%d.%d (%ld bit) vector unit: %s math mode: %s",
		__ICC / 100,(__ICC / 10) % 10,bits,vu,math);
#elif __GNUC__
#	ifdef __GNUC_PATCHLEVEL__
	snprintf(m_Compiler,sizeof(m_Compiler),"GCC V%d.%d.%d (%ld bit) vector unit: %s math mode: %s",
		__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__,bits,vu,math);
#	else
	snprintf(m_Compiler,sizeof(m_Compiler),"GCC V%d.%d (%ld bit) vector unit: %s math mode: %s",
		__GNUC__,__GNUC_MINOR__,bits,vu,math);
#	endif
#else
	snprintf(m_Compiler,sizeof(m_Compiler),"Unknown compiler vector unit: %s math mode: %s",
		vu, math);
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
	return m_Compiler;
}

void *b3Runtime::b3GetOpenGLExtension(const char *procedure_name)
{
#if defined(BLZ3_USE_OPENGL) && defined(RTLD_NEXT)
	return dlsym(RTLD_NEXT,procedure_name);
#else
	return null;
#endif
}

int b3Runtime::b3ParseOption(
	int     argc,
	char   *argv[],
	int     i,
	char   *option,
	size_t  size)
{
	if (strlen(argv[i]) > 2)
	{
		strncpy(option,&argv[i][2],size);
	}
	else
	{
		i++;
		if (i < argc)
		{
			strncpy (option,argv[i],size);
		}
		else
		{
			option[0] = 0;
		}
	}
	return i;
}
