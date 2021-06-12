/*
**
**	$Filename:	b3TxExif.h $
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

class b3RaytraceExif : public b3TxExif
{
	unsigned m_xDPI = 72;
	unsigned m_yDPI = 72;

public:
	explicit b3RaytraceExif(const char * filename);
	b3RaytraceExif();

	void b3AddValues(b3Scene * scene);
};

#endif
