/*
**
**	$Filename:	b3ExampleScene.h $  
**	$Release:	Dortmund 2001 $
**	$Revision: 1023 $
**	$Date: 2005-01-07 13:38:51 +0100 (Fr, 07 Jan 2005) $
**	$Author: smork $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - constructing simple scenes
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_EXAMPLESCENE_H
#define B3_EXAMPLESCENE_H

#include "blz3/raytrace/b3Factory.h"

class b3ExampleScene : public b3Factory
{
private:
	static void          b3SetCameraName(b3CameraPart *camera,int id);
	static void          b3SetObjectName(b3BBox *bbox,int id);
	static void          b3SetLightName(b3Light *light,int id);
};

#endif
