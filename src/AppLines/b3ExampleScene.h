/*
**
**	$Filename:	b3ExampleScene.h $  
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "blz3/raytrace/b3Scene.h"

class b3ExampleScene
{
public:
	static b3Scene      *b3CreateNew(const char *filename);
	static b3Scene      *b3CreateGlobal();
	static b3Scene      *b3CreateBBox(b3BBox *original);
	static b3Scene      *b3CreateMaterial(b3Base<b3Item> **ptrMatHead);

private:
	static void          b3Consolidate(b3Scene *scene);
	static b3CameraPart *b3CreateCamera(b3Scene *scene,b3_f64 xAngle = 225,b3_f64 yAngle =  30);

	static void          b3SetCameraName(b3CameraPart *camera,int id);
	static void          b3SetObjectName(b3BBox *bbox,int id);
	static void          b3SetLightName(b3Light *light,int id);
};

#endif
