/*
**
**	$Filename:	b3Runtime.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Runtime environment
**
**	(C) Copyright 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_RUNTIME_H
#define B3_RUNTIME_H

#include "blz3/b3Config.h"

#define B3_NAME       "Blizzard III"

/**
 * This class provides some runtime information. See the b3CPU class
 * for some more information.
 */
class B3_PLUGIN b3Runtime : public b3CPU
{
	char               m_Compiler[256];
	char               m_Product[256];

	static const std::unordered_map<b3_vector_unit, const char *> m_VectorMap;

	b3Runtime();

public:
	/**
	 * This method returns the singleton instance of the runtime description.
	 *
	 * @return The runtime description.
	 */
	static inline b3Runtime & b3Instance()
	{
		static b3Runtime   runtime;

		return runtime;
	}

	/**
	 * This method returns the number of usable CPUs.
	 *
	 * @return Number of usable CPUs.
	 */
	static inline b3_count b3GetNumCPUs()
	{
		return b3Instance().cpu_count;
	}

	/**
	 * This method returns the used endian type of the CPU.
	 *
	 * @return The CPUs endian.
	 */
	static inline b3_cpu_type b3GetCPUType()
	{
		return b3Instance().cpu_type;
	}

	/**
	 * This method returns the CPU's bit count. This is the size of
	 * a pointer in bits.
	 *
	 * @return The used address space.
	 */
	static inline b3_count b3GetCPUBits()
	{
		return b3Instance().cpu_bits;
	}

	/**
	 * This method returns the hostname.
	 *
	 * @param hostname The buffer which receives the hostname.
	 * @param size     The buffer size.
	 * @return True on success.
	 */
	static b3_bool     b3Hostname(char * hostname, const b3_size size);

	/**
	 * This method returns information about the used compiler.
	 */
	static const char * b3GetCompiler();

	/**
	 * This method returns information about this software framework.
	 */
	static const char * b3GetProduct();

	/**
	 * This method returns an OpenGL function pointer from the given procedure name.
	 *
	 * @param procedure The procedure name to solve.
	 * @return The mapped procedure.
	 */
	static void    *   b3GetOpenGLExtension(const char * procedure);

	/**
	 * This method parses the command line arguments.
	 *
	 * @param argc The number of arguments.
	 * @param argv The arguments itself.
	 * @param index The argument index to start parsing.
	 * @param option The buffer which receives the option argument.
	 * @param size The size of the option buffer.
	 * @return The next index to process.
	 */
	static int         b3ParseOption(int argc, char * argv[], int index, char * option, size_t size);
};

#endif
