/*
**
**	$Filename:	b3ExampleScene.cpp $
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
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "DlgScene.h"
#include "b3ExampleScene.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3Render.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.15  2004/04/23 13:17:17  sm
**	- Added simple material page and renamed wood material page.
**	- Reflect material member renaming.
**
**	Revision 1.14  2004/03/02 09:07:17  sm
**	- Added read/write support for Cook/Torrance material.
**	- Added test module for Cook/Torrance reflection model.
**	- Fixed camera name handling if camera name is empty.
**	
**	Revision 1.13  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.12  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**	
**	Revision 1.11  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.10  2002/12/31 15:13:07  sm
**	- Some minor fixes.
**	
**	Revision 1.9  2002/08/04 13:24:55  sm
**	- Found transformation bug: Normals have to be treated as
**	  direction vectors, aren't them?
**	- b3PrepareInfo::m_PrepareProc initialized not only in
**	  debug mode.
**	
**	Revision 1.8  2002/02/28 16:58:45  sm
**	- Added torus dialogs.
**	- Fixed material and stencil handling when not activating
**	  sheet page.
**	- Further cleanup of edit dialogs done.
**	- Corrected shading of CSG cylinder and CSG cone (added
**	  shaded top and bottom plate).
**	
**	Revision 1.7  2002/02/27 20:14:51  sm
**	- Added stencil creation for creating simple shapes.
**	- Fixed material creation.
**	- Cleaned up some files.
**	
**	Revision 1.6  2002/01/16 16:17:13  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.5  2002/01/10 17:31:11  sm
**	- Some minor GUI updates.
**	- b3BBox::b3Transform() changes m_Matrix member.
**	- Added image selection with image preview.
**	
**	Revision 1.4  2002/01/06 16:30:47  sm
**	- Added Load/Save/Replace object
**	- Enhanced "New world"
**	- Added some non static methods to b3Dir (Un*x untested, yet!)
**	- Fixed missing sphere/ellipsoid south pole triangles
**	- Fixed Spline cylinder/ring triangle indexing
**	
**	Revision 1.3  2001/11/12 16:50:29  sm
**	- Scene properties dialog coding
**	
**	Revision 1.2  2001/11/05 16:57:39  sm
**	- Creating demo scenes.
**	- Initializing some b3Item derived objects
**	
**	Revision 1.1  2001/11/04 21:12:14  sm
**	- New CB3ShowRaytrace control
**	
**
*/

/*************************************************************************
**                                                                      **
**                        b3ExampleScene implementation                 **
**                                                                      **
*************************************************************************/

inline void b3ExampleScene::b3Consolidate(b3Scene *scene)
{
	b3ShapeRenderContext context;
	b3_vector            lower,upper;

	scene->b3AllocVertices(&context);
	scene->b3ComputeBounds(&lower,&upper);
}

inline void b3ExampleScene::b3SetCameraName(b3CameraPart *camera,int id)
{
	CString name;

	name.LoadString(id);
	camera->b3SetName(name);
}

inline void b3ExampleScene::b3SetLightName(b3Light *light,int id)
{
	CString name;

	name.LoadString(id);
	light->b3SetName(name);
}

inline void b3ExampleScene::b3SetObjectName(b3BBox *bbox,int id)
{
	CString name;

	name.LoadString(id);
	bbox->b3SetName(name);
}

b3Scene *b3ExampleScene::b3CreateNew(const char *filename)
{
	b3Scene      *scene  = new b3SceneMork(TRACEPHOTO_MORK);
	b3BBox       *bbox   = new b3BBox(BBOX);
	b3Area       *area   = new b3Area(AREA);
	b3Light      *light  = new b3Light(SPOT_LIGHT);
	b3CameraPart *camera = new b3CameraPart(CAMERA);
	b3_vector     eye,view;

	b3SetCameraName(camera,IDS_NEW_CAMERA_NAME);
	b3SetLightName(light,IDS_NEW_LIGHT_NAME);
	b3SetObjectName(bbox,IDS_NEW_OBJECT_NAME);

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);
	scene->b3SetFilename(filename);
	bbox->b3GetShapeHead()->b3Append(area);
	
	// Init camera
	b3Vector::b3Init(&eye, 0,-350,75);
	b3Vector::b3Init(&view);
	camera->b3Orientate(&eye,&view,100.0,50.0,50.0);
	scene->b3SetCamera(camera);
	scene->b3GetSpecialHead()->b3Append(camera);

	b3Consolidate(scene);
	return scene;
}

b3Scene *b3ExampleScene::b3CreateGlobal()
{
	b3Scene      *scene  = new b3SceneMork(TRACEPHOTO_MORK);
	b3BBox       *bbox1  = new b3BBox(BBOX);
	b3BBox       *bbox2  = new b3BBox(BBOX);
	b3Sphere     *medium = new b3Sphere(SPHERE);
	b3Sphere     *big    = new b3Sphere(SPHERE);
	b3Area       *area   = new b3Area(AREA);
	b3MatChess   *chess  = new b3MatChess(CHESS);
	b3MatNormal  *mirror = new b3MatNormal(MATERIAL);
	b3MatNormal  *glass  = new b3MatNormal(MATERIAL);
	b3Light      *light  = new b3Light(SPOT_LIGHT);
	b3CameraPart *camera = new b3CameraPart(CAMERA);
	b3_vector     eye,view;

	scene->b3GetBBoxHead()->b3Append(bbox1);
	scene->b3GetBBoxHead()->b3Append(bbox2);
	scene->b3GetLightHead()->b3Append(light);
	scene->b3GetSpecialHead()->b3Append(camera);
	bbox1->b3GetShapeHead()->b3Append(area);
	bbox2->b3GetShapeHead()->b3Append(big);
	bbox2->b3GetShapeHead()->b3Append(medium);

	// Init camera
	b3Vector::b3Init(&eye, 0,-350,75);
	b3Vector::b3Init(&view,0,-250,25);
	camera->b3Orientate(&eye,&view,100.0,50.0,37.5);
	scene->b3SetCamera(camera);

	// Init checker board
	area->m_Base.z = -100;
	area->m_Base.y =   50;
	area->m_Dir1.x =  200;
	area->m_Dir2.y =  150;
	area->b3GetMaterialHead()->b3Append(chess);

	// Big spere
	b3Vector::b3Init(&big->m_Base,-50,50,-50);
	big->m_Dir.x = 50;
	big->b3GetMaterialHead()->b3Append(mirror);

	// Small spere
	b3Vector::b3Init(&medium->m_Base,-10,-10,-70);
	medium->m_Dir.x = 30;
	medium->b3GetMaterialHead()->b3Append(glass);

	// Init mirror sphere
	mirror->m_Reflection = 0.8f;

	// Init mirror sphere
	glass->m_Reflection = 0.1f;
	glass->m_Refraction = 0.8f;
	glass->m_Ior  = 1.53f;

	b3Consolidate(scene);
	return scene;
}

b3CameraPart *b3ExampleScene::b3CreateCamera(
	b3Scene *scene,
	b3_f64   xAngle,
	b3_f64   yAngle)
{
	b3CameraPart *camera = new b3CameraPart(CAMERA);
	b3_vector     lower,upper;
	b3_vector     center,size;

	scene->b3ComputeBounds(&lower,&upper);
	center.x = (lower.x + upper.x) * 0.5;
	center.y = (lower.y + upper.y) * 0.5;
	center.z = (lower.z + upper.z) * 0.5;
	size.x   =  upper.x - lower.x;
	size.y   =  upper.y - lower.y;
	size.z   =  upper.z - lower.z;
	camera->b3Overview(&center,&size,xAngle * M_PI / 180,yAngle * M_PI / 180);
	return camera;
}

b3Scene *b3ExampleScene::b3CreateBBox(b3BBox *original)
{
	b3Scene      *scene  = new b3SceneMork(TRACEPHOTO_MORK);
	b3BBox       *bbox   = (b3BBox *)b3World::b3Clone(original);
	b3Light      *light  = new b3Light(SPOT_LIGHT);
	b3CameraPart *camera;
	b3_f64        rad;

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);

	rad = b3Vector::b3Length(&bbox->m_DimSize);
	light->m_Position.x = bbox->m_DimBase.x + 10.0 * rad;
	light->m_Position.y = bbox->m_DimBase.y - 10.0 * rad;
	light->m_Position.z = bbox->m_DimBase.z + 10.0 * rad;
	light->m_Distance   = 40.0 * rad;

	b3Consolidate(scene);
	scene->b3GetSpecialHead()->b3Append(camera = b3CreateCamera(scene));
	scene->b3SetCamera(camera);

	return scene;
}

b3Scene *b3ExampleScene::b3CreateMaterial(b3Base<b3Item> **ptrMatHead)
{
	b3Scene      *scene = new b3SceneMork(TRACEPHOTO_MORK);
	b3BBox       *bbox  = new b3BBox(BBOX);
	b3Ellipsoid  *big   = new b3Ellipsoid(ELLIPSOID);
	b3Area       *area  = new b3Area(AREA);
	b3MatChess   *chess = new b3MatChess(CHESS);
	b3Light      *light = new b3Light(SPOT_LIGHT);
	b3CameraPart *camera;
	b3_matrix     transform;

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);

	bbox->b3GetShapeHead()->b3Append(area);
	bbox->b3GetShapeHead()->b3Append(big);
	
	area->b3GetMaterialHead()->b3Append(chess);

	// Transform ellipsoid
	b3Matrix::b3Scale(null,&transform,null,0.4,0.4,0.4);
	b3Matrix::b3Move(&transform,&transform,0,0,20);
	big->b3Transform(&transform,true);

	// Enlarge whole scene
	b3Matrix::b3Scale(null,&transform,null,5,5,5);
	bbox->b3Transform(&transform,true,true);

	// Create camera
	b3Consolidate(scene);
	scene->b3GetSpecialHead()->b3Append(camera = b3CreateCamera(scene,225,60));
	camera->b3ScaleFocalLength(2.7);
	scene->b3SetCamera(camera);

	*ptrMatHead = big->b3GetMaterialHead();
	return scene;
}
