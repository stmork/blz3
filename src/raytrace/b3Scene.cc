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
#include "blz3/image/b3TxPool.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.16  2001/10/17 14:46:02  sm
**	- Adding triangle support.
**	- Renaming b3TriangleShape into b3Triangles and introducing
**	  new b3TriangleShape as base class. This results in
**	  source file renaming, too.
**	- Fixing soft shadow bug.
**	- Only scene loading background image when activated.
**	- Fixing LDC spline initialization.
**	- Converting Windows paths into right paths on Un*x
**
**	Revision 1.15  2001/10/09 20:47:01  sm
**	- some further texture handling.
**	
**	Revision 1.14  2001/10/06 19:24:17  sm
**	- New torus intersection routines and support routines
**	- Added further shading support from materials
**	- Added stencil checking
**	- Changed support for basis transformation for shapes with
**	  at least three direction vectors.
**	
**	Revision 1.13  2001/10/05 20:30:46  sm
**	- Introducing Mork and Phong shading.
**	- Using light source when shading
**	
**	Revision 1.12  2001/10/03 18:46:45  sm
**	- Adding illumination and recursive raytracing
**	
**	Revision 1.11  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**	Revision 1.10  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.9  2001/08/20 14:16:48  sm
**	- Putting data into cmaera and light combobox.
**	- Selecting camera and light.
**
**	Revision 1.8  2001/08/18 15:38:27  sm
**	- New action toolbar
**	- Added comboboxes for camera and lights (but not filled in)
**	- Drawing Fulcrum and view volume (Clipping plane adaption is missing)
**	- Some RenderObject redesignes
**	- Color selecting bug fix in RenderObject
**
**	Revision 1.7  2001/08/11 19:59:16  sm
**	- Added orthogonal projection
**
**	Revision 1.6  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**	Revision 1.5  2001/08/05 09:23:22  sm
**	- Introducing vectors, matrices, Splines and NURBS
**
**	Revision 1.4  2001/08/03 15:54:09  sm
**	- Compilation of OpenGL under Windows NT
**
**	Revision 1.3  2001/08/02 15:37:17  sm
**	- Now we are able to draw Blizzard Scenes with OpenGL.
**
**	Revision 1.2  2001/08/02 15:21:54  sm
**	- Some minor changes
**
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
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
	b3Item::b3Register(&b3SceneMork::b3Init, &b3SceneMork::b3Init, TRACEANGLE_MORK);
	b3Item::b3Register(&b3SceneMork::b3Init, &b3SceneMork::b3Init, TRACEPHOTO_MORK);
	b3Item::b3Register(&b3ScenePhong::b3Init,&b3ScenePhong::b3Init,TRACEANGLE_PHONG);
	b3Item::b3Register(&b3ScenePhong::b3Init,&b3ScenePhong::b3Init,TRACEPHOTO_PHONG);
	b3Item::b3Register(&b3Scene::b3Init,     &b3Scene::b3Init,     TRACEPHOTO_ALBRECHT);
	b3Item::b3Register(&b3Scene::b3Init,     &b3Scene::b3Init,     GLOBAL_ILLUM);
}

b3Scene::b3Scene(b3_size class_size,b3_u32 class_type) : b3Item(class_size, class_type)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III scene init.\n");
}

b3Scene::b3Scene(b3_u32 class_type) : b3Item(sizeof(b3Scene),class_type)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III scene init.\n");
}

b3Scene::b3Scene(b3_u32 *buffer) : b3Item(buffer)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III scene load.\n");

	// Background color
	b3InitColor(&m_TopColor);
	b3InitColor(&m_BottomColor);

	// Camera
	b3InitVector(&m_EyePoint);
	b3InitVector(&m_ViewPoint);
	b3InitVector(&m_Width);
	b3InitVector(&m_Height);

	// Some other values
	b3InitNull();
	m_xAngle           = b3InitFloat();
	m_yAngle           = b3InitFloat();
	m_BBoxOverSize     = b3InitFloat();
	m_BackgroundType   = b3InitInt();
	m_TraceDepth       = b3InitInt();
	m_Flags            = b3InitInt();
	m_ShadowBrightness = b3InitFloat();
	m_Epsilon          = b3InitFloat();
	m_xSize            = b3InitInt();
	m_ySize            = b3InitInt();
	b3InitString(m_TextureName,B3_TEXSTRINGLEN);
	m_BackTexture = (m_Flags & TP_TEXTURE ? texture_pool.b3LoadTexture(m_TextureName) : null);
	m_Nebular     = null;
}

b3_bool b3Scene::b3GetDisplaySize(b3_res &xSize,b3_res &ySize)
{
	xSize = this->m_xSize;
	ySize = this->m_ySize;
	return (m_Flags & TP_SIZEVALID) != 0;
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

b3Nebular *b3Scene::b3GetNebular()
{
	b3Nebular *nebular;
	b3Item    *item;

	B3_FOR_BASE(&heads[2],item)
	{
		if (item->b3GetClassType() == NEBULAR)
		{
			return (b3Nebular *)item;
		}
	}

	nebular = new b3Nebular(NEBULAR);
	heads[2].b3Append(nebular);
	return nebular;
}

b3CameraPart *b3Scene::b3GetCamera(b3_bool must_active)
{
	b3CameraPart *camera,*first = null;
	b3Item       *item;

	B3_FOR_BASE(&heads[2],item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			camera = (b3CameraPart *)item;
			if (first == null)
			{
				first = camera;
			}
			if ((!must_active) || (camera->Flags & CAMERA_ACTIVE))
			{
				return camera;
			}
		}
	}

	if (first == null)
	{
		camera = new b3CameraPart(CAMERA);
		camera->EyePoint  = m_EyePoint;
		camera->ViewPoint = m_ViewPoint;
		camera->Width     = m_Width;
		camera->Height    = m_Height;
		strcpy(camera->CameraName,"Camera");
		heads[2].b3Append(camera);
	}
	else
	{
		camera = first;
	}

	return camera;
}

b3CameraPart *b3Scene::b3GetNextCamera(b3CameraPart *camera)
{
	while (camera = (b3CameraPart *)camera->Succ)
	{
		if (camera->b3GetClassType() == CAMERA)
		{
			return camera;
		}
	}
	return null;
}

b3Light *b3Scene::b3GetLight(b3_bool must_active)
{
	b3Light *light;
	b3Item  *item;

	B3_FOR_BASE(&heads[1],item)
	{
		light = (b3Light *)item;
		if ((!must_active) || ((light->m_Flags & LIGHT_OFF) == 0))
		{
			return light;
		}
	}

	if (heads[1].First == null)
	{
		light = new b3Light(SPOT_LIGHT);
		strcpy(light->m_Name,"Light");
		heads[1].b3Append(light);
	}

	return light;
}

b3BBox *b3Scene::b3GetFirstBBox()
{
	return (b3BBox *)heads[0].First;
}
