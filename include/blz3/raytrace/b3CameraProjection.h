/*
**
**	$Filename:	b3CameraProjection.h $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
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

class B3_PLUGIN b3CameraProjection : public b3BaseTransformation
{
public:
	b3CameraProjection(b3CameraPart *camera);
};

#endif
