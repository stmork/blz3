/*
**
**	$Filename:	b3Tx.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (proto types)
**
**	(C) Copyright 2001 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_ENDIAN_H
#define B3_BASE_ENDIAN_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Runtime.h"

/**
 * This class provides some static methods for endian change purposes.
 */
class B3_PLUGIN b3Endian
{
public:
	/**
	* This method gets an integer in native byte order from the given pointer
	* position. The size depends on the template parameter type.
	*
	* @param Ptr The memory position.
	* @return The two byte integer.
	*/
	template<typename T>
	[[nodiscard]]
	static inline T b3Get(const void * Ptr)
	{
		const b3_u08 * Pointer = static_cast<const b3_u08 *>(Ptr);
		T              Value{0};

		if (b3Runtime::b3GetCPUType() == B3_LITTLE_ENDIAN)
		{
			Pointer += sizeof(T);
			for (b3_size i = 0; i < sizeof(T); ++i)
			{
				--Pointer;
				Value = (Value << 8) | *Pointer;
			}
		}
		else
		{
			for (b3_size i = 0; i < sizeof(T); ++i)
			{
				Value = (Value << 8) | *Pointer++;
			}
		}
		return Value;
	}

	/**
	 * This method gets a two byte integer in big endian order from the given
	 * pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The two byte integer.
	 */
	[[nodiscard]]
	static b3_u16   b3GetMot16(const void * Ptr);

	/**
	 * This method gets a four byte integer in big endian order from the given
	 * pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The four byte integer.
	 */
	[[nodiscard]]
	static b3_u32   b3GetMot32(const void * Ptr);

	/**
	 * This method gets floating point number in big endian order from the
	 * given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The floating point number.
	 */
	[[nodiscard]]
	static b3_f32   b3GetMotFloat(const void * Ptr);

	/**
	 * This method gets double precision floating point number in big endian
	 * order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The double precision floating point number.
	 */
	[[nodiscard]]
	static b3_f64   b3GetMotDouble(const void * Ptr);


	/**
	 * This method gets a two byte integer in little endian order from the
	 * given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The two byte integer.
	 */
	[[nodiscard]]
	static b3_u16   b3GetIntel16(const void * Ptr);

	/**
	 * This method gets a four byte integer in little endian order from the
	 * given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The four byte integer.
	 */
	[[nodiscard]]
	static b3_u32   b3GetIntel32(const void * Ptr);

	/**
	 * This method gets floating point number in little endian order from the
	 * given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The floating point number.
	 */
	[[nodiscard]]
	static b3_f32   b3GetIntelFloat(const void * Ptr);

	/**
	 * This method gets double precision floating point number in little endian
	 * order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The double precision floating point number.
	 */
	[[nodiscard]]
	static b3_f64   b3GetIntelDouble(const void * Ptr);

	/**
	 * This method converts a 16 bit number from another endian to the native
	 * endian and vice versa.
	 *
	 * @note This method changes the memory in situ! That means that the memory
	 * is changed on endian change.
	 *
	 * @param Ptr The pointer zo the given 16 bit number.
	 * @return The needed size.
	 */
	static b3_size  b3ChangeEndian16(void * Ptr);

	/**
	 * This method converts a 32 bit number from another endian to the native
	 * endian and vice versa.
	 *
	 * @note This method changes the memory in situ! That means that the memory
	 * is changed on endian change.
	 *
	 * @param Ptr The pointer zo the given 32 bit number.
	 * @return The needed size.
	 */
	static b3_size  b3ChangeEndian32(void * Ptr);

	/**
	 * This method converts a 64 bit number from another endian to the native
	 * endian and vice versa.
	 *
	 * @note This method changes the memory in situ! That means that the memory
	 * is changed on endian change.
	 *
	 * @param Ptr The pointer zo the given 64 bit number.
	 * @return The needed size.
	 */
	static b3_size  b3ChangeEndian64(void * Ptr);
};

#endif
