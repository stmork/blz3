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
	b3RenderShapeContext context;
	b3_vector       lower,upper;

	scene->b3AllocVertices(&context);
	scene->b3ComputeBounds(&lower,&upper);
}

b3Scene *b3ExampleScene::b3CreateNew(const char *filename)
{
	b3Scene      *scene  = new b3SceneMork(TRACEPHOTO_MORK);
	b3BBox       *bbox   = new b3BBox(BBOX);
	b3Area       *area   = new b3Area(AREA);
	b3Light      *light  = new b3Light(SPOT_LIGHT);
	b3CameraPart *camera = new b3CameraPart(CAMERA);
	b3_vector     eye,view;

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);
	scene->b3SetFilename(filename);
	bbox->b3GetShapeHead()->b3Append(area);
	
	// Init camera
	b3Vector::b3Init(&eye, 0,-350,75);
	b3Vector::b3Init(&view,0);
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
	glass->m_RefrValue  = 1.53f;

	b3Consolidate(scene);
	return scene;
}

b3Scene *b3ExampleScene::b3CreateMaterial()
{
	b3Scene     *scene = new b3SceneMork(TRACEPHOTO_MORK);
	b3BBox      *bbox  = new b3BBox(BBOX);
	b3Ellipsoid *big   = new b3Ellipsoid(ELLIPSOID);
	b3Area      *area  = new b3Area(AREA);
	b3MatChess  *chess = new b3MatChess(CHESS);
	b3Light     *light = new b3Light(SPOT_LIGHT);

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);
	bbox->b3GetShapeHead()->b3Append(area);
	bbox->b3GetShapeHead()->b3Append(big);
	area->b3GetMaterialHead()->b3Append(chess);

	b3Consolidate(scene);
	return scene;
}
