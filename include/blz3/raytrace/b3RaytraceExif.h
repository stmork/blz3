/*
**
**	$Filename:	b3RaytraceExif.h $
**	$Release:	Dortmund 20021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Handling EXIF meta data from raytracing geometry.
**
**	(C) Copyright 2001 - 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_RAYTRACEEXIF_H
#define B3_RAYTRACE_RAYTRACEEXIF_H

#include "blz3/image/b3TxExif.h"

class b3Scene;

/**
 * This class maintains EXIF related data and expands the functionality by
 * processing camera and animation data.
 */
class b3RaytraceExif : public b3TxExif
{
public:
	b3RaytraceExif();

	/**
	 * This method adds camera and animation related information to the EXIF
	 * data.
	 *
	 * @param scene The b3Scene instance containing further information.
	 */
	void b3AddValues(b3Scene * scene);
};

#endif
