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
**      Revision 1.4  2001/08/03 15:54:09  sm
**      - Compilation of OpenGL under Windows NT
**
**      Revision 1.3  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
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

void b3Scene::b3GetView(b3_res &xSize,b3_res &ySize)
{
	xSize = this->xSize;
	ySize = this->ySize;
}

void b3Scene::b3SetView(b3_res xSize,b3_res ySize)
{
#ifdef BLZ3_USE_OPENGL
	b3_f64  width,height,distance,x,y,z,factor,relation;
	GLfloat aspectWindow = (GLfloat)xSize / (GLfloat)ySize;
	GLfloat aspectCamera;
	GLfloat min = 0.1f;

	x = ViewPoint.x - EyePoint.x;
	y = ViewPoint.y - EyePoint.y;
	z = ViewPoint.z - EyePoint.z;
	distance     = sqrt(x * x + y * y + z * z);
	factor       = min / distance;
	width = factor * sqrt(
		Width.x * Width.x +
		Width.y * Width.y +
		Width.z * Width.z);
	height = factor * sqrt(
		Height.x * Height.x +
		Height.y * Height.y +
		Height.z * Height.z);

	aspectCamera = (GLfloat)(width / height);
	relation     = aspectCamera / aspectWindow;
	if (relation > 1) height *= relation;
	else              width  /= relation;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,xSize,ySize);
	glFrustum(-width,width,-height,height,min,10000.0f);
	gluLookAt(
		EyePoint.x, EyePoint.y, EyePoint.z,
		ViewPoint.x,ViewPoint.y,ViewPoint.z,
		Height.x,   Height.y,   Height.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
#endif
}
