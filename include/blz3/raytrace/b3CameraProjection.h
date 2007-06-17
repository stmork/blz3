/*
**
**	$Filename:	b3CameraProjection.h $
**	$Release:	Dortmund 2005 $
**	$Revision: 1449 $
**	$Date: 2006-06-28 19:17:02 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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
