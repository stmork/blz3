/*
**
**	$Filename:      b3TxTIFF.h $
**	$Release:       Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - TIFF classes
**
**      (C) Copyright 2005 Steffen A. Mork
**          All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_IMAGE_TXTIFF_H
#define B3_IMAGE_TXTIFF_H

#include "blz3/b3Config.h"

/*************************************************************************
**                                                                      **
**                        TIFF class definitions                        **
**                                                                      **
*************************************************************************/

/**
 * This enumeration lists the possible endian version.
 */
enum b3_tiff_endian
{
	TIFF_INTEL    = 0x4949, // Intel little endian.
	TIFF_MOTOROLA = 0x4d4d  // Motorola big endian.
};

/**
 * This structure defines a TIFF format header.
 */
struct b3HeaderTIFF
{
	b3_u16         TypeCPU;     //!< The endian version (MM or II).
	b3_u16         VersionTIFF; //!< The version (always 0x2a).
	b3_u32         FirstTag;    //!< The offset to the first TIFF directory.
};

#endif
