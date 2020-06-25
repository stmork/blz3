/*
**
**	$Filename:	b3CameraProjection.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Performing base transformation
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#pragma once

#ifndef B3_RAYTRACE_CAMERAPROJECTION_H
#define B3_RAYTRACE_CAMERAPROJECTION_H

#include "blz3/raytrace/b3BaseTrans.h"
#include "blz3/raytrace/b3Special.h"

/**
 * This class is a helper class to transform between world and
 * camera coordinates.
 */
class B3_PLUGIN b3CameraProjection : public b3BaseTransformation
{
public:
	/**
	 * The constructor initializes the base transform.
	 *
	 * @param camera The camera to use.
	 */
	b3CameraProjection(b3CameraPart *camera);
};

#endif
