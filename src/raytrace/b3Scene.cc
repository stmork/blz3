/*
**
**      $Filename:      b3Scene.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Raytracing root scene
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.1  2001/07/01 12:24:59  sm
**      Initial revision
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

void b3InitScene::b3Init()
{
	b3PrintF(B3LOG_DEBUG,"Registering scene classes...\n");
	b3Item::b3Register(&b3Scene::b3Init,&b3Scene::b3Init,TRACEANGLE_MORK);
	b3Item::b3Register(&b3Scene::b3Init,&b3Scene::b3Init,TRACEPHOTO_MORK);
	b3Item::b3Register(&b3Scene::b3Init,&b3Scene::b3Init,TRACEANGLE_PHONG);
	b3Item::b3Register(&b3Scene::b3Init,&b3Scene::b3Init,TRACEPHOTO_PHONG);
	b3Item::b3Register(&b3Scene::b3Init,&b3Scene::b3Init,TRACEPHOTO_ALBRECHT);
	b3Item::b3Register(&b3Scene::b3Init,&b3Scene::b3Init,GLOBAL_ILLUM);
}

b3Scene::b3Scene(b3_u32 class_type) : b3Item(sizeof(b3Scene),class_type)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III scene init.\n");
}

b3Scene::b3Scene(b3_u32 *buffer) : b3Item(buffer)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III scene load.\n");

	// Background color
	b3InitColor(&Top);
	b3InitColor(&Bottom);

	// Camera
	b3InitVector(&EyePoint);
	b3InitVector(&ViewPoint);
	b3InitVector(&Width);
	b3InitVector(&Height);

	// Some other values
	BackTexture      = (b3Tx *)b3InitNull();
	xAngle           = b3InitFloat();
	yAngle           = b3InitFloat();
	BBoxOverSize     = b3InitFloat();
	BackgroundType   = b3InitInt();
	TraceDepth       = b3InitInt();
	Flags            = b3InitInt();
	ShadowBrightness = b3InitFloat();
	Limit            = b3InitFloat();
	xSize            = b3InitInt();
	ySize            = b3InitInt();
}

b3Item *b3Scene::b3Init(b3_u32 class_type)
{
	return new b3Scene(class_type);
}

b3Item *b3Scene::b3Init(b3_u32 *src)
{
	return new b3Scene(src);
}
