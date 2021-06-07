/*
**
**      $Filename:      b3Runtime.c $
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
#include "blz3/system/b3Runtime.h"

#include <unistd.h>
#include <dlfcn.h>
#include <pwd.h>
#include <locale.h>

/*************************************************************************
**                                                                      **
**                        b3Runtime static methods                      **
**                                                                      **
*************************************************************************/

const std::unordered_map<b3_vector_unit, const char *> b3Runtime::m_VectorMap
{
	{ B3_VU_FPU,     "FPU" },
	{ B3_VU_MMX,     "MMX" },
	{ B3_VU_SSE,     "SSE" },
	{ B3_VU_SSE2,    "SSE 2" },
	{ B3_VU_SSE3,    "SSE 3" },
	{ B3_VU_SSSE3,   "Supplemental SSE 3" },
	{ B3_VU_SSE41,   "SSE 4.1" },
	{ B3_VU_3DNOW,   "3DNow!" },
	{ B3_VU_ALTIVEC, "AltiVec" },
	{ B3_VU_NEON,    "NEON" }
};

b3Runtime::b3Runtime()
{
	const char * vu = m_VectorMap.at(b3GetVectorUnit());

#if defined(B3_SSE2)
	const char * math = "SSE2";
#elif defined(B3_SSE)
	const char * math = "SSE";
#else
	const char * math = "FPU";
#endif

#ifdef __ICC
	snprintf(m_Compiler, sizeof(m_Compiler), "Intel CC V%d.%d (%ld bit) vector unit: %s math mode: %s",
		__ICC / 100, (__ICC / 10) % 10, bits, vu, math);
#elif __GNUC__
#	ifdef __GNUC_PATCHLEVEL__
	snprintf(m_Compiler, sizeof(m_Compiler), "GCC V%d.%d.%d (%d bit) vector unit: %s math mode: %s",
		__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__, cpu_bits, vu, math);
#	else
	snprintf(m_Compiler, sizeof(m_Compiler), "GCC V%d.%d (%ld bit) vector unit: %s math mode: %s",
		__GNUC__, __GNUC_MINOR__, bits, vu, math);
#	endif
#else
	snprintf(m_Compiler, sizeof(m_Compiler), "Unknown compiler vector unit: %s math mode: %s",
		vu, math);
#endif

#ifdef B3_VERSION
	snprintf(m_Product, sizeof(m_Product), "%s V%s", B3_NAME, B3_VERSION);
#else
	strncpy(m_Product, B3_NAME, sizeof(m_Product));
#endif

	bzero(m_UserName, sizeof(m_UserName));
}

b3_bool b3Runtime::b3Hostname(char * hostname, const b3_size buffer_size)
{
	return gethostname(hostname, buffer_size) == 0;
}

const char * b3Runtime::b3GetCompiler()
{
	return b3Instance().m_Compiler;
}

const char * b3Runtime::b3GetProduct()
{
	return b3Instance().m_Product;
}

const char * b3Runtime::b3GetUserName()
{
	b3Runtime & runtime = b3Instance();
	const uid_t uid = geteuid ();
	const struct passwd  * pw = getpwuid (uid);

	if (pw != nullptr)
	{
		const std::string gecos(pw->pw_gecos);
		const size_t idx = gecos.find_first_of(',');

		strcpy(runtime.m_UserName,
			gecos.substr(0, std::min(idx, sizeof(m_UserName) - 1)).c_str());
	}
	return runtime.m_UserName;
}

void * b3Runtime::b3GetOpenGLExtension(const char * procedure_name)
{
#if defined(BLZ3_USE_OPENGL) && defined(RTLD_NEXT)
	return dlsym(RTLD_NEXT, procedure_name);
#else
	return nullptr;
#endif
}

int b3Runtime::b3ParseOption(
	int     argc,
	char  * argv[],
	int     i,
	char  * option,
	size_t  size)
{
	if (strlen(argv[i]) > 2)
	{
		strncpy(option, &argv[i][2], size);
	}
	else
	{
		i++;
		if (i < argc)
		{
			strncpy(option, argv[i], size);
		}
		else
		{
			option[0] = 0;
		}
	}
	option[size - 1] = 0;
	return i;
}
