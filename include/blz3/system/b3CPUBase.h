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
	B3_VU_SSE2,    //!< SSE2
	B3_VU_SSE3,    //!< SSE3
	B3_VU_3DNOW,   //!< AMD 3DNow
	B3_VU_ALTIVEC  //!< PowerPC AltiVec/Velocity Engine
};

/**
 * This class provides some information about the used CPUs.
 */
class B3_PLUGIN b3CPUBase
{
private:
	static b3_cpu_type cpu_type;
	static b3_count    cpu_bits;

protected:
	static b3_count    cpu_count; //!< Number of usable CPUs.

public:
	/**
	 * This constructor initializes information abount the installed CPUs.
	 */
	b3CPUBase();

	/**
	 * This method returns the number of usable CPUs.
	 *
	 * @return Number of usable CPUs.
	 */
	static inline b3_count b3GetNumCPUs()
	{
		return cpu_count;
	}

	/**
	 * This method returns the used endian type of the CPU.
	 *
	 * @return The CPUs endian.
	 */
	static inline b3_cpu_type b3GetCPUType()
	{
		return cpu_type;
	}

	/**
	 * This method returns the CPU's bit count. This is the size of
	 * a pointer in bits.
	 *
	 * @return The used address space.
	 */
	static inline b3_count b3GetCPUBits()
	{
		return cpu_bits;
	}

	/**
	 * Return type of available vector unit.
	 *
	 * @return Used vector unit.
	 */
	static inline b3_vector_unit b3GetVectorUnit()
	{
#if   defined(BLZ3_USE_SSE3)
		return B3_VU_SSE3;
#elif defined(BLZ3_USE_SSE2)
		return B3_VU_SSE2;
#elif defined(BLZ3_USE_SSE)
		return B3_VU_SSE;
#elif defined(BLZ3_USE_ALTIVEC)
		return B3_VU_ALTIVEC;
#else
		return B3_VU_FPU;
#endif
	}
};

#endif
