/*
**
**	$Filename:	b3CPUBase.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CPU information (Un*x)
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_SYSTEM_CPUBASE_H
#define B3_SYSTEM_CPUBASE_H

#include "blz3/b3Types.h"

/**
 * Info about available CPUs.
 */
enum b3_cpu_type
{
	B3_UNKNOWN_ENDIAN = 0,      //!< Unknown endian
	B3_BIG_ENDIAN     = 0x4d4d, //!< Big endian like most SUN Solaris machines or MC680x0
	B3_LITTLE_ENDIAN  = 0x4949  //!< Little endian like Intel x86 machines.
};

/**
 * Info about vectorrization unit.
 */
enum b3_vector_unit
{
	B3_VU_FPU,     //!< No vectorization unit.
	B3_VU_MMX,     //!< MMX
	B3_VU_SSE,     //!< SSE
	B3_VU_SSE2,    //!< SSE 2
	B3_VU_SSE3,    //!< SSE 3
	B3_VU_SSSE3,   //!< Supplemental SSE 3
	B3_VU_SSE41,   //!< SSE 4.1
	B3_VU_3DNOW,   //!< AMD 3DNow
	B3_VU_ALTIVEC, //!< PowerPC AltiVec/Velocity Engine
	B3_VU_NEON     //!< ARM vector unit.
};

/**
 * This class provides some information about the used CPUs.
 */
class B3_PLUGIN b3CPUBase
{
protected:
	static const b3_count    cpu_bits = sizeof(void *) * 8;
	b3_cpu_type              cpu_type = B3_UNKNOWN_ENDIAN;
	b3_count                 cpu_count = 0; //!< Number of usable CPUs.

	/**
	 * This constructor initializes information abount the installed CPUs.
	 */
	b3CPUBase();

	/**
	 * Return type of available vector unit.
	 *
	 * @return Used vector unit.
	 */
	static inline b3_vector_unit b3GetVectorUnit()
	{
#if   defined(BLZ3_USE_SSE41)
		return B3_VU_SSE41;
#elif defined(BLZ3_USE_SSSE3)
		return B3_VU_SSSE3;
#elif defined(BLZ3_USE_SSE3)
		return B3_VU_SSE3;
#elif defined(BLZ3_USE_SSE2)
		return B3_VU_SSE2;
#elif defined(BLZ3_USE_SSE)
		return B3_VU_SSE;
#elif defined(BLZ3_USE_ALTIVEC)
		return B3_VU_ALTIVEC;
#elif defined(BLZ3_USE_NEON)
		return B3_VU_NEON;
#else
		return B3_VU_FPU;
#endif
	}
};

#endif
