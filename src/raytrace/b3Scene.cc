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
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.8  2001/08/18 15:38:27  sm
**      - New action toolbar
**      - Added comboboxes for camera and lights (but not filled in)
**      - Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**      - Some RenderObject redesignes
**      - Color selecting bug fix in RenderObject
**
**      Revision 1.7  2001/08/11 19:59:16  sm
**      - Added orthogonal projection
**
**      Revision 1.6  2001/08/11 15:59:59  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**      Revision 1.5  2001/08/05 09:23:22  sm
**      - Introducing vectors, matrices, Splines and NURBS
**
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

void b3Scene::b3GetDisplaySize(b3_res &xSize,b3_res &ySize)
{
	xSize = this->xSize;
	ySize = this->ySize;
}

b3ModellerInfo *b3Scene::b3GetModellerInfo()
{
	b3ModellerInfo *info;
	b3Item         *item;

	B3_FOR_BASE(&heads[2],item)
	{
		if (item->b3GetClassType() == LINES_INFO)
		{
			return (b3ModellerInfo *)item;
		}
	}

	info = new b3ModellerInfo(LINES_INFO);
	heads[2].b3Append(info);
	return info;
}

b3CameraPart *b3Scene::b3GetCamera()
{
	b3CameraPart *camera;
	b3Item       *item;

	B3_FOR_BASE(&heads[2],item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			return (b3CameraPart *)item;
		}
	}

	camera = new b3CameraPart(CAMERA);
	camera->EyePoint  = EyePoint;
	camera->ViewPoint = ViewPoint;
	camera->Width     = Width;
	camera->Height    = Height;
	strcpy(camera->CameraName,"Camera");

	heads[2].b3Append(camera);
	return camera;
}
