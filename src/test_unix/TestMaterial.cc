/*
**
**	$Filename:	TestMaterial.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - constructing some example materials
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2004/03/03 08:13:59  sm
**	- Cook/Torrance examples ready with vases.
**
**	Revision 1.1  2004/03/02 09:07:17  sm
**	- Added read/write support for Cook/Torrance material.
**	- Added test module for Cook/Torrance reflection model.
**	- Fixed camera name handling if camera name is empty.
**	
*/

/*************************************************************************
**                                                                      **
**                        b3TestMaterial implementation                 **
**                                                                      **
*************************************************************************/

class b3TestMaterial
{
	inline static void b3Consolidate(b3Scene *scene)
	{
		b3ShapeRenderContext context;
		b3_vector            lower,upper;

		scene->b3AllocVertices(&context);
		scene->b3ComputeBounds(&lower,&upper);
	}

	inline static b3CameraPart *b3CreateCamera(
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

public:
	inline static b3Scene *b3CreateMaterial(b3Material *material)
	{
		b3Scene          *scene = new b3SceneMork(TRACEPHOTO_MORK);
		b3BBox           *bbox  = new b3BBox(BBOX);
		b3SplineRotShape *big = new b3SplineRotShape(SPLINE_ROT);
		b3Area           *area  = new b3Area(AREA);
		b3MatChess       *chess = new b3MatChess(CHESS);
		b3Light          *light = new b3Light(SPOT_LIGHT);
		b3CameraPart     *camera;
		b3_matrix         transform;

		scene->b3GetBBoxHead()->b3Append(bbox);
		scene->b3GetLightHead()->b3Append(light);
		scene->m_xSize =
		scene->m_ySize = 256;

		big->b3Init(2,9,false,30);
		b3Vector::b3Init(&big->m_Controls[0],50,0,-50);
		b3Vector::b3Init(&big->m_Controls[1],50,0,-30);
		b3Vector::b3Init(&big->m_Controls[2],15,0, 10);
		b3Vector::b3Init(&big->m_Controls[3],15,0, 30);
		b3Vector::b3Init(&big->m_Controls[4],25,0, 40);
 		b3Vector::b3Init(&big->m_Controls[5],25,0, 50);
		b3Vector::b3Init(&big->m_Controls[6],10,0, 50);
		b3Vector::b3Init(&big->m_Controls[7],10,0, 40);
		b3Vector::b3Init(&big->m_Controls[8],15,0, 40);
		bbox->b3GetShapeHead()->b3Append(area);
		bbox->b3GetShapeHead()->b3Append(big);
	
		area->b3GetMaterialHead()->b3Append(chess);

		// Transform ellipsoid
		b3Matrix::b3Scale(null,&transform,null,0.4,0.4,0.4);
		b3Matrix::b3Move(&transform,&transform,5,5,20);
		big->b3Transform(&transform,true);

		// Enlarge whole scene
		b3Matrix::b3Scale(null,&transform,null,5,5,5);
		bbox->b3Transform(&transform,true,true);

		// Create camera
		b3Consolidate(scene);
		scene->b3GetSpecialHead()->b3Append(camera = b3CreateCamera(scene,225,60));
		camera->b3ScaleFocalLength(2.7);
		scene->b3SetCamera(camera);

		big->b3GetMaterialHead()->b3Append(material);

		return scene;
	}
};

static void b3SaveOneColor(b3Color &color,int y)
{
	int i;

	for (i = 0; i <= 7;i++)
	{
		b3MatCookTorrance *material = new b3MatCookTorrance(COOK_TORRANCE);
		b3Scene           *scene    = b3TestMaterial::b3CreateMaterial(material);
		b3World            world;
		b3Path             name;

		material->m_m         = 0.05 * i + 0.1;
		material->m_DiffColor = color;

		name.b3Format("/tmp/material_%d_%d.bwd",y,i);

		world.b3SetFirst(scene);
		world.b3Write(name);
	}
}

int main(int argc,char *argv[])
{
	b3Color gold   = b3Color(0.79,0.54,0.2);
	b3Color copper = b3Color(0.7, 0.32,0.2);
	b3Color silver = b3Color(0.5, 0.6, 0.7);

	b3SaveOneColor(gold,0);
	b3SaveOneColor(copper,1);
	b3SaveOneColor(silver,2);

	return 0;
}
