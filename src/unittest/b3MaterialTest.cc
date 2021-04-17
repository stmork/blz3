/*
**
**	$Filename:	b3MaterialTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Unit tests for complex materials.
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
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

#include "b3MaterialTest.h"
#include "b3TestMacros.h"

#include "blz3/system/b3Display.h"
#include "blz3/raytrace/b3BBox.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/raytrace/b3Shape.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3MaterialTest);

class b3TestMaterial
{
	b3Scene * scene = nullptr;

public:
	explicit inline b3TestMaterial(
		b3Material * material,
		b3_vector  * dim = nullptr) : scene(new b3Scene(TRACEPHOTO_MORK))
	{
		b3BBox      *      bbox  = new b3BBox(BBOX);
		b3SplineRotShape * big   = new b3SplineRotShape(SPLINE_ROT);
		b3Area      *      area  = new b3Area(AREA);
		b3MatChess    *    chess = new b3MatChess(CHESS);
		b3Light      *     light = new b3Light(SPOT_LIGHT);
		b3SuperSample   *  ss4   = new b3SuperSample(SUPERSAMPLE4);
		b3CameraPart   *   camera;
		b3_matrix          transform;

		scene->b3GetBBoxHead()->b3Append(bbox);
		scene->b3GetLightHead()->b3Append(light);
		scene->m_xSize = scene->m_ySize = 128;

		big->b3Init(2, 9, false, 36);
		b3Vector::b3Init(&big->m_Controls[0], 50, 0, -50);
		b3Vector::b3Init(&big->m_Controls[1], 50, 0, -30);
		b3Vector::b3Init(&big->m_Controls[2], 15, 0,  10);
		b3Vector::b3Init(&big->m_Controls[3], 15, 0,  30);
		b3Vector::b3Init(&big->m_Controls[4], 25, 0,  40);
		b3Vector::b3Init(&big->m_Controls[5], 25, 0,  50);
		b3Vector::b3Init(&big->m_Controls[6], 10, 0,  50);
		b3Vector::b3Init(&big->m_Controls[7], 10, 0,  40);
		b3Vector::b3Init(&big->m_Controls[8], 15, 0,  40);
		bbox->b3GetShapeHead()->b3Append(area);
		bbox->b3GetShapeHead()->b3Append(big);

		area->b3GetMaterialHead()->b3Append(chess);

		// Transform ellipsoid
		b3Matrix::b3Scale(nullptr, &transform, nullptr, 0.4, 0.4, 0.4);
		b3Matrix::b3Move(&transform, &transform, 5, 5, 20);
		big->b3Transform(&transform, true);

		// Enlarge whole scene
		b3Matrix::b3Scale(nullptr, &transform, nullptr, 5, 5, 5);
		bbox->b3Transform(&transform, true, true);

		// Create camera
		b3Consolidate();
		scene->b3GetSpecialHead()->b3Append(camera = b3CreateCamera(225, 60));
		scene->b3GetSpecialHead()->b3Append(ss4);
		camera->b3ScaleFocalLength(2.7);
		scene->b3SetCamera(camera);

		big->b3GetMaterialHead()->b3Append(material);

		if (dim != nullptr)
		{
			b3ComputeBBoxScale(bbox, dim);
		}
	}

	inline operator b3Scene * () const
	{
		return scene;
	}

private:
	inline void b3Consolidate()
	{
		b3ShapeRenderContext context;
		b3_vector            lower, upper;

		scene->b3SetupVertexMemory(&context);
		scene->b3ComputeBounds(&lower, &upper);
	}

	inline b3CameraPart * b3CreateCamera(
		b3_f64    xAngle,
		b3_f64    yAngle)
	{
		b3CameraPart * camera = new b3CameraPart(CAMERA);
		b3_vector      lower, upper;
		b3_vector      center, size;

		scene->b3ComputeBounds(&lower, &upper);
		center.x = (lower.x + upper.x) * 0.5;
		center.y = (lower.y + upper.y) * 0.5;
		center.z = (lower.z + upper.z) * 0.5;
		size.x   =  upper.x - lower.x;
		size.y   =  upper.y - lower.y;
		size.z   =  upper.z - lower.z;

		camera->b3Overview(&center, &size, B3_RAD(xAngle), B3_RAD(yAngle));
		return camera;
	}

	inline static void b3ComputeBBoxScale(b3BBox * bbox, b3_vector * scale)
	{
		b3_f64 min = std::min(std::min(bbox->m_DimSize.x, bbox->m_DimSize.y), bbox->m_DimSize.z);

		scale->x = bbox->m_DimSize.x / min;
		scale->y = bbox->m_DimSize.y / min;
		scale->z = bbox->m_DimSize.z / min;
	}
};


void b3MaterialTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3MaterialTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3MaterialTest::b3SaveCookTorrance(b3Color & color, int y)
{
	int i;

	for (i = 0; i <= 7; i++)
	{
		b3MatCookTorrance * material = new b3MatCookTorrance(COOK_TORRANCE);
		b3TestMaterial      scene(material);
		b3Path              path;

		material->m_m       = 0.05 * i + 0.1;
		material->m_Diffuse = color;

		path.b3Format("test_material_cook_torrance_%d_%d.bwd", y, i);
		b3Save(scene, path);
	}
}

void b3MaterialTest::b3Save(b3Scene * scene, b3Path & path)
{
	b3Display   display(scene->m_xSize, scene->m_ySize);
	b3World     world;

	world.b3SetFirst(scene);
	CPPUNIT_ASSERT(world.b3Write(path));

	CPPUNIT_ASSERT_NO_THROW(scene->b3Raytrace(&display, true));

	path.b3RemoveExt();
	path.b3Append(".jpg");
	CPPUNIT_ASSERT(display.b3SaveImage(path));
}

void b3MaterialTest::testGranite()
{
	b3_vector      dim;
	b3MatGranite * material = new b3MatGranite(GRANITE);
	b3TestMaterial scene(material, &dim);
	b3Path         path;
	b3_f64         scale = 0.15;

	b3Vector::b3Init(&material->m_Scale, dim.x / scale, dim.y / scale, dim.z / scale);
	path.b3Format("test_material_granite.bwd");
	b3Save(scene, path);
}

void b3MaterialTest::testMarble()
{
	b3_vector      dim;
	b3MatMarble  * material = new b3MatMarble(MARBLE);
	b3TestMaterial scene(material, &dim);
	b3Path         path;

	path.b3Format("test_material_marble.bwd");
	b3Save(scene, path);
}

void b3MaterialTest::testWood()
{
	b3_vector      dim;
	b3MatWood   *  material = new b3MatWood(WOOD);
	b3TestMaterial scene(material, &dim);
	b3Path         path;

	path.b3Format("test_material_wood.bwd");
	b3Save(scene, path);
}

void b3MaterialTest::testCarPaint()
{
	b3_vector       dim;
	b3MatCarPaint * material = new b3MatCarPaint(CAR_PAINT);
	b3TestMaterial  scene(material, &dim);
	b3Path          path;

	path.b3Format("test_material_carpaint.bwd");
	b3Save(scene, path);
}

void b3MaterialTest::testThinFilm()
{
	b3_vector       dim;
	b3MatThinFilm * material = new b3MatThinFilm(THIN_FILM);
	b3TestMaterial  scene(material, &dim);
	b3Path          path;

	path.b3Format("test_material_thinfilm.bwd");
	b3Save(scene, path);
}

void b3MaterialTest::testCookTorrance()
{
	b3Color gold    = b3Color(0.79, 0.65, 0.2);
	b3Color messing = b3Color(0.79, 0.54, 0.2);
	b3Color copper  = b3Color(0.7,  0.32, 0.2);
	b3Color silver  = b3Color(0.5,  0.6,  0.7);
	b3Color grey    = b3Color(0.8,  0.8,  0.8);
	b3Color blue    = b3Color(0.1,  0.3,  0.8);

	b3SaveCookTorrance(gold, 0);
	b3SaveCookTorrance(messing, 1);
	b3SaveCookTorrance(copper, 2);
	b3SaveCookTorrance(silver, 3);
	b3SaveCookTorrance(grey, 4);
	b3SaveCookTorrance(blue, 5);
}

#endif
