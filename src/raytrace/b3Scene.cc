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
**	Revision 1.28  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**
**	Revision 1.27  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.26  2001/12/02 15:43:49  sm
**	- Creation/Deletion/Editing of lights
**	- Creation/Deletion of cameras
**	- New toolbars introduced.
**	
**	Revision 1.25  2001/11/26 17:16:37  sm
**	- Linux b3TimeSpan fix
**	
**	Revision 1.24  2001/11/25 12:25:31  sm
**	- Completing some dialogs:
**	  o super sampling
**	  o distributed raytracing
**	
**	Revision 1.23  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.22  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**	
**	Revision 1.21  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.20  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.19  2001/11/01 09:43:11  sm
**	- Some image logging cleanups.
**	- Texture preparing now in b3Prepare().
**	- Done some minor fixes.
**	
**	Revision 1.18  2001/10/21 16:55:20  sm
**	- Introducing lens flares.
**	- Introducing different modes of background computation.
**	- Introducing different types of row sampling.
**	
**	Revision 1.17  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
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

	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BBOX);
	m_Heads[1].b3InitBase(CLASS_LIGHT);
	m_Heads[2].b3InitBase(CLASS_SPECIAL);

	m_TopColor.a = 0;
	m_TopColor.r = 0.5;
	m_TopColor.g = 0;
	m_TopColor.b = 1;
	m_BottomColor.a = 0;
	m_BottomColor.r = 0;
	m_BottomColor.g = 0;
	m_BottomColor.b = 1;

	m_xAngle           = 0;
	m_yAngle           = 0;
	m_BBoxOverSize     = 0.02f;
	m_BackgroundType   = TP_NOTHING;
	m_TraceDepth       = 5;
	m_Flags            = TP_SIZEVALID;
	m_ShadowBrightness = 0.5f;
	m_Epsilon          = 0.001f;
	m_xSize            = 200;
	m_ySize            = 150;
	m_BackTexture      = null;
}

b3Scene::b3Scene(b3_u32 class_type) : b3Item(sizeof(b3Scene),class_type)
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III scene init.\n");

	b3AllocHeads(3);
	m_Heads[0].b3InitBase(CLASS_BBOX);
	m_Heads[1].b3InitBase(CLASS_LIGHT);
	m_Heads[2].b3InitBase(CLASS_SPECIAL);

	m_TopColor.a = 0;
	m_TopColor.r = 0.5;
	m_TopColor.g = 0;
	m_TopColor.b = 1;
	m_BottomColor.a = 0;
	m_BottomColor.r = 0;
	m_BottomColor.g = 0;
	m_BottomColor.b = 1;

	m_xAngle           = 0;
	m_yAngle           = 0;
	m_BBoxOverSize     = 0.02f;
	m_BackgroundType   = TP_NOTHING;
	m_TraceDepth       = 5;
	m_Flags            = TP_SIZEVALID;
	m_ShadowBrightness = 0.5f;
	m_Epsilon          = 0.001f;
	m_xSize            = 200;
	m_ySize            = 150;
	m_BackTexture      = null;
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
	m_BackTexture      = null;
	m_ActualCamera     = null;
	m_Nebular          = null;
	m_LensFlare        = null;
	m_SuperSample      = null;
}

void b3Scene::b3Write()
{
	b3PrintF(B3LOG_NORMAL,"Blizzard III scene storage.\n");

	// Background color
	b3StoreColor(&m_TopColor);
	b3StoreColor(&m_BottomColor);

	// Camera
	b3StoreVector(&m_EyePoint);
	b3StoreVector(&m_ViewPoint);
	b3StoreVector(&m_Width);
	b3StoreVector(&m_Height);

	// Some other values
	b3StoreNull  ();
	b3StoreFloat (m_xAngle);
	b3StoreFloat (m_yAngle);
	b3StoreFloat (m_BBoxOverSize);
	b3StoreInt   (m_BackgroundType);
	b3StoreCount (m_TraceDepth);
	b3StoreInt   (m_Flags);
	b3StoreFloat (m_ShadowBrightness);
	b3StoreFloat (m_Epsilon);
	b3StoreRes   (m_xSize);
	b3StoreRes   (m_ySize);
	b3StoreString(m_TextureName,B3_TEXSTRINGLEN);
}

b3_bool b3Scene::b3GetDisplaySize(b3_res &xSize,b3_res &ySize)
{
	xSize = this->m_xSize;
	ySize = this->m_ySize;
	return (m_Flags & TP_SIZEVALID) != 0;
}

b3Base<b3Item> *b3Scene::b3GetBBoxHead()
{
	return &m_Heads[0];
}

b3Base<b3Item> *b3Scene::b3GetLightHead()
{
	return &m_Heads[1];
}

b3Base<b3Item> *b3Scene::b3GetSpecialHead()
{
	return &m_Heads[2];
}

b3ModellerInfo *b3Scene::b3GetModellerInfo()
{
	b3ModellerInfo *info;
	b3Item         *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == LINES_INFO)
		{
			return (b3ModellerInfo *)item;
		}
	}

	info = new b3ModellerInfo(LINES_INFO);
	b3GetSpecialHead()->b3Append(info);
	return info;
}

b3Distribute *b3Scene::b3GetDistributed(b3_bool force)
{
	b3Distribute *distributed;
	b3Item       *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == DISTRIBUTE)
		{
			return (b3Distribute *)item;
		}
	}

	if (force)
	{
		distributed = new b3Distribute(DISTRIBUTE);
		b3GetSpecialHead()->b3Append(distributed);
	}
	else
	{
		distributed = null;
	}
	return distributed;
}

b3Nebular *b3Scene::b3GetNebular(b3_bool force)
{
	b3Nebular *nebular;
	b3Item    *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == NEBULAR)
		{
			return (b3Nebular *)item;
		}
	}

	if (force)
	{
		nebular = new b3Nebular(NEBULAR);
		b3GetSpecialHead()->b3Append(nebular);
	}
	else
	{
		nebular = null;
	}
	return nebular;
}

b3SuperSample *b3Scene::b3GetSuperSample(b3_bool force)
{
	b3SuperSample *supersample;
	b3Item        *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == SUPERSAMPLE4)
		{
			return (b3SuperSample *)item;
		}
	}
	
	if (force)
	{
		supersample = new b3SuperSample(SUPERSAMPLE4);
		b3GetSpecialHead()->b3Append(supersample);
	}
	else
	{
		supersample = null;
	}
	return supersample;
}

b3LensFlare *b3Scene::b3GetLensFlare(b3_bool force)
{
	b3LensFlare *lensflare;
	b3Item      *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == LENSFLARE)
		{
			return (b3LensFlare *)item;
		}
	}

	if (force)
	{
		lensflare = new b3LensFlare(LENSFLARE);
		b3GetSpecialHead()->b3Append(lensflare);
	}
	else
	{
		lensflare = null;
	}

	return lensflare;
}

b3CameraPart *b3Scene::b3GetCamera(b3_bool must_active)
{
	b3CameraPart *camera,*first = null;
	b3Item       *item;

	B3_FOR_BASE(b3GetSpecialHead(),item)
	{
		if (item->b3GetClassType() == CAMERA)
		{
			camera = (b3CameraPart *)item;
			if (first == null)
			{
				first = camera;
			}
			if ((!must_active) || (camera->m_Flags & CAMERA_ACTIVE))
			{
				return camera;
			}
		}
	}

	if (first == null)
	{
		camera = new b3CameraPart(CAMERA);
		camera->m_EyePoint  = m_EyePoint;
		camera->m_ViewPoint = m_ViewPoint;
		camera->m_Width     = m_Width;
		camera->m_Height    = m_Height;
		camera->m_Flags     = CAMERA_ACTIVE;
		b3GetTitle(camera->m_CameraName);
		b3GetSpecialHead()->b3Append(camera);
	}
	else
	{
		camera = first;
	}

	return camera;
}

b3CameraPart *b3Scene::b3GetNextCamera(b3CameraPart *camera)
{
	while ((camera = (b3CameraPart *)camera->Succ) != null)
	{
		if (camera->b3GetClassType() == CAMERA)
		{
			return camera;
		}
	}
	return null;
}

void b3Scene::b3SetCamera(b3CameraPart *camera)
{
	if (camera != null)
	{
		m_EyePoint 	= camera->m_EyePoint;
		m_ViewPoint = camera->m_ViewPoint;
		m_Width     = camera->m_Width;
		m_Height    = camera->m_Height;
	}
	m_ActualCamera = camera;
}

void b3Scene::b3SetFilename(const char *filename)
{
	strcpy(m_Filename,filename);
}

b3_bool b3Scene::b3GetTitle(char *title)
{
	title[0] = 0;
	if (m_ActualCamera != null)
	{
		strcpy(title,m_ActualCamera->m_CameraName);
	}
	else
	{
		b3Dir::b3SplitFileName(m_Filename,null,title);
	}
	return strlen(title) > 0;
}

b3Light *b3Scene::b3GetLight(b3_bool must_active)
{
	b3Light *light;
	b3Item  *item;

	B3_FOR_BASE(b3GetLightHead(),item)
	{
		light = (b3Light *)item;
		if ((!must_active) || ((light->m_Flags & LIGHT_OFF) == 0))
		{
			return light;
		}
	}

	if ((light = (b3Light *)b3GetLightHead()->First) == null)
	{
		light = new b3Light(SPOT_LIGHT);
		strcpy(light->m_Name,"Light");
		b3GetLightHead()->b3Append(light);
	}

	return light;
}

b3BBox *b3Scene::b3GetFirstBBox()
{
	return (b3BBox *)b3GetBBoxHead()->First;
}
