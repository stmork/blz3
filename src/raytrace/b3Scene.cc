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
**      Revision 1.2  2001/08/02 15:21:54  sm
**      - Some minor changes
**
**      Revision 1.1.1.1  2001/07/01 12:24:59  sm
**      Blizzard III is born
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

void b3Scene::b3SetView(b3_res xSize,b3_res ySize)
{
#ifdef BLZ3_USE_OPENGL
b3PrintF(B3LOG_NORMAL,"view...\n");
	GLfloat aspect = (GLfloat)xSize / (GLfloat)ySize;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,xSize,ySize);
	gluPerspective (90.0f, aspect, 0.1f,10000.0f);
	gluLookAt(
		EyePoint.x, EyePoint.y, EyePoint.z,
		ViewPoint.x,ViewPoint.y,ViewPoint.z,
		Height.x,   Height.y,   Height.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
#endif
}
