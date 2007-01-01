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
#include <malloc.h>

/*************************************************************************
**                                                                      **
**                        b3Runtime static methods                      **
**                                                                      **
*************************************************************************/

#ifdef HAVE_NEW_OVERLOADING
void *operator new(size_t size)
{
	return memalign(16, size);
}
#endif

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
		fprintf(stderr, "Cannot set locale. Assuming we're right ;-)\n");
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
