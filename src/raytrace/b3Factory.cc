/*
**
**	$Filename:	b3Factory.cpp $
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
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/raytrace/b3Factory.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

/*************************************************************************
**                                                                      **
**                        b3Factory implementation                      **
**                                                                      **
*************************************************************************/

inline void b3Factory::b3Consolidate(b3Scene * scene)
{
	b3ShapeRenderContext context;
	b3_vector            lower, upper;

	scene->b3SetupVertexMemory(&context);
	scene->b3ComputeBounds(&lower, &upper);
}

b3Scene * b3Factory::b3CreateNew(
	const char  *  filename,
	const b3_u32   class_type)
{
	b3Scene    *   scene  = new b3Scene(class_type);
	b3BBox    *    bbox   = new b3BBox(BBOX);
	b3Area    *    area   = new b3Area(AREA);
	b3Light    *   light  = new b3Light(SPOT_LIGHT);
	b3CameraPart * camera = new b3CameraPart(CAMERA);
	b3_vector      eye, view;

	/*
		b3SetCameraName(camera,IDS_NEW_CAMERA_NAME);
		b3SetLightName(light,IDS_NEW_LIGHT_NAME);
		b3SetObjectName(bbox,IDS_NEW_OBJECT_NAME);
	*/

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);
	scene->b3SetFilename(filename);
	bbox->b3GetShapeHead()->b3Append(area);

	// Init camera
	b3Vector::b3Init(&eye, 0, -350, 75);
	b3Vector::b3Init(&view);
	camera->b3Orientate(&eye, &view, 100.0, 50.0, 50.0);
	scene->b3SetCamera(camera);
	scene->b3GetSpecialHead()->b3Append(camera);

	b3Consolidate(scene);
	return scene;
}

b3Scene * b3Factory::b3CreateGlobal(const b3_u32 class_type)
{
	b3Scene    *   scene  = new b3Scene(class_type);
	b3BBox    *    bbox1  = new b3BBox(BBOX);
	b3BBox    *    bbox2  = new b3BBox(BBOX);
	b3Sphere   *   medium = new b3Sphere(SPHERE);
	b3Sphere   *   big    = new b3Sphere(SPHERE);
	b3Area    *    area   = new b3Area(AREA);
	b3MatChess  *  chess  = new b3MatChess(CHESS);
	b3MatNormal  * mirror = new b3MatNormal(NORMAL_MATERIAL);
	b3MatNormal  * glass  = new b3MatNormal(NORMAL_MATERIAL);
	b3Light    *   light  = new b3Light(SPOT_LIGHT);
	b3CameraPart * camera = new b3CameraPart(CAMERA);
	b3_vector      eye, view;

	scene->b3GetBBoxHead()->b3Append(bbox1);
	scene->b3GetBBoxHead()->b3Append(bbox2);
	scene->b3GetLightHead()->b3Append(light);
	scene->b3GetSpecialHead()->b3Append(camera);
	bbox1->b3GetShapeHead()->b3Append(area);
	bbox2->b3GetShapeHead()->b3Append(big);
	bbox2->b3GetShapeHead()->b3Append(medium);

	// Init camera
	b3Vector::b3Init(&eye, 0, -350, 75);
	b3Vector::b3Init(&view, 0, -250, 25);
	camera->b3Orientate(&eye, &view, 100.0, 50.0, 37.5);
	scene->b3SetCamera(camera);

	// Init checker board
	area->m_Base.z = -100;
	area->m_Base.y =   50;
	area->m_Dir1.x =  200;
	area->m_Dir2.y =  150;
	area->b3GetMaterialHead()->b3Append(chess);

	// Big spere
	b3Vector::b3Init(&big->m_Base, -50, 50, -50);
	big->m_Dir.x = 50;
	big->b3GetMaterialHead()->b3Append(mirror);

	// Small spere
	b3Vector::b3Init(&medium->m_Base, -10, -10, -70);
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

b3CameraPart * b3Factory::b3CreateCamera(
	b3Scene * scene,
	b3_f64    xAngle,
	b3_f64    yAngle)
{
	b3CameraPart * camera = new b3CameraPart(CAMERA);
	b3_vector     lower, upper;
	b3_vector     center, size;

	scene->b3ComputeBounds(&lower, &upper);
	center.x = (lower.x + upper.x) * 0.5;
	center.y = (lower.y + upper.y) * 0.5;
	center.z = (lower.z + upper.z) * 0.5;
	size.x   =  upper.x - lower.x;
	size.y   =  upper.y - lower.y;
	size.z   =  upper.z - lower.z;
	camera->b3Overview(&center, &size, xAngle * M_PI / 180, yAngle * M_PI / 180);
	return camera;
}

b3Scene * b3Factory::b3CreateBBox(b3BBox * original_bbox, b3_u32 class_type, b3CameraPart * original_camera)
{
	b3Scene    *   scene  = new b3Scene(class_type);
	b3BBox    *    bbox   = (b3BBox *)b3World::b3Clone(original_bbox);
	b3Light    *   light  = new b3Light(SPOT_LIGHT);
	b3CameraPart * camera;
	b3_f64         rad;

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);

	rad = b3Vector::b3Length(&bbox->m_DimSize);
	light->m_Position.x = bbox->m_DimBase.x + 10.0 * rad;
	light->m_Position.y = bbox->m_DimBase.y - 10.0 * rad;
	light->m_Position.z = bbox->m_DimBase.z + 10.0 * rad;
	light->m_Distance   = 40.0 * rad;

	b3Consolidate(scene);
	if (original_camera != nullptr)
	{
		b3_f64    xAngle, yAngle;

		original_camera->b3ComputeAngles(xAngle, yAngle);
		camera = b3CreateCamera(scene, xAngle * 180 / M_PI, yAngle * 180 / M_PI);
	}
	else
	{
		camera = b3CreateCamera(scene);
	}
	scene->b3GetSpecialHead()->b3Append(camera);
	scene->b3SetCamera(camera);

	return scene;
}

b3Scene * b3Factory::b3CreateMaterial(b3Base<b3Item> ** ptrMatHead, const b3_u32 class_type)
{
	b3Scene    *   scene = new b3Scene(class_type);
	b3BBox    *    bbox  = new b3BBox(BBOX);
	b3Ellipsoid  * big   = new b3Ellipsoid(ELLIPSOID);
	b3Area    *    area  = new b3Area(AREA);
	b3MatChess  *  chess = new b3MatChess(CHESS);
	b3Light    *   light = new b3Light(SPOT_LIGHT);
	b3CameraPart * camera;
	b3_matrix      transform;

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);

	bbox->b3GetShapeHead()->b3Append(area);
	bbox->b3GetShapeHead()->b3Append(big);

	area->b3GetMaterialHead()->b3Append(chess);

	// Transform ellipsoid
	b3Matrix::b3Scale(nullptr, &transform, nullptr, 0.4, 0.4, 0.4);
	b3Matrix::b3Move(&transform, &transform, 0, 0, 20);
	big->b3Transform(&transform, true);

	// Enlarge whole scene
	b3Matrix::b3Scale(nullptr, &transform, nullptr, 5, 5, 5);
	bbox->b3Transform(&transform, true, true);

	// Create camera
	b3Consolidate(scene);
	scene->b3GetSpecialHead()->b3Append(camera = b3CreateCamera(scene, 225, 60));
	camera->b3ScaleFocalLength(2.7);
	scene->b3SetCamera(camera);

	*ptrMatHead = big->b3GetMaterialHead();
	return scene;
}

b3Scene * b3Factory::b3CreateBump(b3Base<b3Item> ** ptrBumpHead, const b3_u32 class_type)
{
	b3Scene    *   scene = new b3Scene(class_type);
	b3BBox    *    bbox  = new b3BBox(BBOX);
	b3Ellipsoid  * big   = new b3Ellipsoid(ELLIPSOID);
	b3Area    *    area  = new b3Area(AREA);
	b3MatChess  *  chess = new b3MatChess(CHESS);
	b3MatNormal  * mat   = new b3MatNormal(NORMAL_MATERIAL);
	b3Light    *   light = new b3Light(SPOT_LIGHT);
	b3CameraPart * camera;
	b3_matrix      transform;

	scene->b3GetBBoxHead()->b3Append(bbox);
	scene->b3GetLightHead()->b3Append(light);

	bbox->b3GetShapeHead()->b3Append(area);
	bbox->b3GetShapeHead()->b3Append(big);

	big->b3GetMaterialHead()->b3Append(chess);

	mat->m_Reflection = 0.2f;
	mat->m_Diffuse    = b3_pkd_color(0x886644);
	mat->m_Ambient    = mat->m_Diffuse * 0.2;
	area->b3GetMaterialHead()->b3Append(mat);

	// Transform ellipsoid
	b3Matrix::b3Scale(nullptr, &transform, nullptr, 0.2, 0.2, 0.2);
	b3Matrix::b3Move(&transform, &transform, 15, 3, 10);
	big->b3Transform(&transform, true);

	// Create camera
	b3Consolidate(scene);
	scene->b3GetSpecialHead()->b3Append(camera = b3CreateCamera(scene, 225, 60));
	camera->b3ScaleFocalLength(2.7);
	scene->b3SetCamera(camera);

	*ptrBumpHead = area->b3GetBumpHead();
	return scene;
}
