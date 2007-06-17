/*
**
**	$Filename:	b3Tx.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1207 $
**	$Date: 2005-08-01 11:19:25 +0200 (Mo, 01 Aug 2005) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - imaging routines (proto types)
**
**	(C) Copyright 2001 Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_ENDIAN_H
#define B3_BASE_ENDIAN_H

#include "blz3/b3Config.h"

/**
 * This class provides some static methods for endian change
 * purposes.
 */
class B3_PLUGIN b3Endian
{
public:
	/**
	 * This method gets a two byte integer in native order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The two byte integer.
	 */
	static b3_u16   b3Get16         (void *Ptr);

	/**
	 * This method gets a four byte integer in native order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The four byte integer.
	 */
	static b3_u32   b3Get32         (void *Ptr);

	/**
	 * This method gets a two byte integer in big endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The two byte integer.
	 */
	static b3_u16   b3GetMot16      (void *Ptr);

	/**
	 * This method gets a four byte integer in big endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The four byte integer.
	 */
	static b3_u32   b3GetMot32      (void *Ptr);

	/**
	 * This method gets floating point number in big endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The floating point number.
	 */
	static b3_f32   b3GetMotFloat   (void *Ptr);

	/**
	 * This method gets double precision floating point number in big endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The double precision floating point number.
	 */
	static b3_f64   b3GetMotDouble  (void *Ptr);
	

	/**
	 * This method gets a two byte integer in little endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The two byte integer.
	 */
	static b3_u16   b3GetIntel16    (void *Ptr);

	/**
	 * This method gets a four byte integer in little endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The four byte integer.
	 */
	static b3_u32   b3GetIntel32    (void *Ptr);

	/**
	 * This method gets floating point number in little endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The floating point number.
	 */
	static b3_f32   b3GetIntelFloat (void *Ptr);

	/**
	 * This method gets double precision floating point number in little endian order from the given pointer position.
	 *
	 * @param Ptr The memory position.
	 * @return The double precision floating point number.
	 */
	static b3_f64   b3GetIntelDouble(void *Ptr);


	/**
	 * This method converts a 16 bit number from another endian to the native endian and vice versa.
	 *
	 * @param Ptr The pointer zo the given 16 bit number.
	 * @return The needed size.
	 */
	static b3_size  b3ChangeEndian16(void *Ptr);

	/**
	 * This method converts a 32 bit number from another endian to the native endian and vice versa.
	 *
	 * @param Ptr The pointer zo the given 32 bit number.
	 * @return The needed size.
	 */
	static b3_size  b3ChangeEndian32(void *Ptr);

	/**
	 * This method converts a 64 bit number from another endian to the native endian and vice versa.
	 *
	 * @param Ptr The pointer zo the given 64 bit number.
	 * @return The needed size.
	 */
	static b3_size  b3ChangeEndian64(void *Ptr);
};

#endif
