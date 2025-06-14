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
enum b3_cpu_type : b3_u16
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
	static constexpr union
	{
		b3_u32      value;
		b3_cpu_type array[2];
	} endianess{.value  = (((b3_u32)B3_BIG_ENDIAN) << 16) | B3_LITTLE_ENDIAN};

protected:
	static constexpr b3_count     cpu_bits  = sizeof(void *) * 8;
	const            b3_count     cpu_count; //!< Number of usable CPUs.

	/**
	 * This constructor initializes information abount the installed CPUs.
	 */
	b3CPUBase();

	/**
	 * Return type of available vector unit.
	 *
	 * @return Used vector unit.
	 */
	static constexpr b3_vector_unit b3GetVectorUnit()
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

public:
	/**
	 * This method returns the CPU's bit count. This is the size of
	 * a pointer in bits.
	 *
	 * @return The used address space.
	 */
	static constexpr inline b3_count b3GetCPUBits()
	{
		return b3CPUBase::cpu_bits;
	}

	/**
	 * This method returns the used endian type of the CPU.
	 *
	 * @return The CPUs endian.
	 */
	static constexpr inline b3_cpu_type b3GetCPUType()
	{
		static_assert(
			sizeof(endianess.value) == sizeof(endianess.array),
			"CPU endianess computation wrong!");

		return b3CPUBase::endianess.array[0];
	}
};

#endif
