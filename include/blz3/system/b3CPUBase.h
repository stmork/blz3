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
enum B3_PLUGIN b3_cpu_type
{
	B3_UNKNOWN_ENDIAN = 0,      //!< Unknown endian
	B3_BIG_ENDIAN     = 0x4d4d, //!< Big endian like most SUN Solaris machines or MC680x0
	B3_LITTLE_ENDIAN  = 0x4949  //!< Little endian like Intel x86 machines.
};

/**
 * This class provides some information about the used CPUs.
 */
class B3_PLUGIN b3CPUBase
{
	static b3_cpu_type cpu_type;
	static b3_count    cpu_bits;

protected:
	static b3_count    cpu_count; //!< Number of usable CPUs.

public:
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
};

#endif
