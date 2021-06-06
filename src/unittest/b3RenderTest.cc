/*
**
**	$Filename:	b3RenderTest.cc $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Skeleton for unit tests.
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

#include "b3RenderTest.h"
#include "b3TestMacros.h"

#include "blz3/base/b3World.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/raytrace/b3ShapeRenderContext.h"

/*************************************************************************
**                                                                      **
**                        OpenGL render preparation tests               **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3RenderTest);

void b3RenderTest::setUp()
{
	b3Path WORKSPACE(getenv("BLZ3_HOME"));
	b3Path BLZ3_HOME(getenv("BLZ3_HOME"));

	WORKSPACE.b3Append("data");
	BLZ3_HOME.b3Append("data");
	search_path.b3AddPath(WORKSPACE);
	search_path.b3AddPath(BLZ3_HOME);
	search_path.b3AddPath("../../data");

	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3RenderTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3RenderTest::test()
{
	b3Path                full_path;
	b3World               world;
	b3ShapeRenderContext  context;
	b3RenderView          view;

	CPPUNIT_ASSERT(search_path.b3IsValid("Shapes.bwd", full_path));
	CPPUNIT_ASSERT(world.b3Read(full_path, false));

	for (b3Item * item  = world.b3GetFirst();
		item != nullptr;
		item  = item->Succ)
	{
		b3Scene *  scene = (b3Scene *)item;
		b3_vector  eye;
		b3_res     xSize, ySize;

		CPPUNIT_ASSERT(scene != nullptr);
		CPPUNIT_ASSERT_NO_THROW(scene->b3Reorg());
		CPPUNIT_ASSERT_NO_THROW(scene->b3GetDisplaySize(xSize, ySize));
		CPPUNIT_ASSERT_NO_THROW(scene->b3SetupVertexMemory(&context));
		CPPUNIT_ASSERT(scene->b3PrepareScene(xSize, ySize));
		CPPUNIT_ASSERT_NO_THROW(scene->b3ResetAnimation());

		b3CameraPart * camera = scene->b3GetFirstCamera(false);
		CPPUNIT_ASSERT(camera != nullptr);

		CPPUNIT_ASSERT_NO_THROW(view.b3SetCamera(scene));
		CPPUNIT_ASSERT_NO_THROW(view.b3SetCamera(camera));

		CPPUNIT_ASSERT(view.b3SetBounds(scene));

		CPPUNIT_ASSERT_NO_THROW(view.b3GetProjectionBase(&eye));
#if 0
		CPPUNIT_ASSERT_EQUAL(eye.x, camera->m_EyePoint.x);
		CPPUNIT_ASSERT_EQUAL(eye.y, camera->m_EyePoint.y);
		CPPUNIT_ASSERT_EQUAL(eye.z, camera->m_EyePoint.z);
#endif
		testView(view, B3_VIEW_3D);
		testView(view, B3_VIEW_TOP);
		testView(view, B3_VIEW_RIGHT);
		testView(view, B3_VIEW_FRONT);
		testView(view, B3_VIEW_LEFT);
		testView(view, B3_VIEW_BACK);
	}
}

void b3RenderTest::testView(b3RenderView & view, const b3_view_mode view_mode)
{
	b3_vector    view_dir;
	b3_view_info info_before;
	b3_view_info info_after1;
	b3_view_info info_after2;
	b3_view_info info_after3;

	// Set view mode and compare.
	CPPUNIT_ASSERT_NO_THROW(view.b3SetViewMode(view_mode));
	CPPUNIT_ASSERT(view.b3IsViewMode(view_mode));
	CPPUNIT_ASSERT_NO_THROW(view.b3GetViewDirection(&view_dir));

	// Do some scale and move operations.
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3GetView(info_before));
	CPPUNIT_ASSERT_NO_THROW(view.b3Scale(1.25));
	CPPUNIT_ASSERT_NO_THROW(view.b3Move( 0.2,  0.0));
	CPPUNIT_ASSERT_NO_THROW(view.b3Move( 0.0,  0.2));
	CPPUNIT_ASSERT_NO_THROW(view.b3Move(-0.2,  0.0));
	CPPUNIT_ASSERT_NO_THROW(view.b3Move( 0.0, -0.2));
	CPPUNIT_ASSERT_NO_THROW(view.b3Scale(0.8));
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3GetView(info_after1));

	// Equal pushes and pops leads to empty stack
	CPPUNIT_ASSERT(!view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_NO_THROW(view.b3PopView());
	CPPUNIT_ASSERT(!view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_NO_THROW(view.b3Select(0.1, 0.2, 0.7, 0.8));
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_NO_THROW(view.b3Select(0.2, 0.3, 0.5, 0.7));
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_NO_THROW(view.b3PopView());
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_NO_THROW(view.b3PopView());
	CPPUNIT_ASSERT(!view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3GetView(info_after2));

	// Multiple pushes and final original leads to empty stack
	CPPUNIT_ASSERT_NO_THROW(view.b3Select(0.1, 0.2, 0.7, 0.8));
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_NO_THROW(view.b3Select(0.2, 0.3, 0.5, 0.7));
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_NO_THROW(view.b3Original());
	CPPUNIT_ASSERT(!view.b3ViewStackNotEmpty());
	CPPUNIT_ASSERT_EQUAL(view_mode != B3_VIEW_3D, view.b3GetView(info_after3));

	CPPUNIT_ASSERT(testViewInfo(info_before, info_after1, view_mode));
	CPPUNIT_ASSERT(testViewInfo(info_before, info_after2, view_mode));
	CPPUNIT_ASSERT(testViewInfo(info_before, info_after3, view_mode));
}

bool b3RenderTest::testViewInfo(
		const b3_view_info & first,
		const b3_view_info & second,
		const b3_view_mode   view_mode)
{
	CPPUNIT_ASSERT_EQUAL(view_mode, first.m_ViewMode);
	CPPUNIT_ASSERT_EQUAL(view_mode, second.m_ViewMode);

	if (view_mode != B3_VIEW_3D)
	{
		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_AspectRatio,  second.m_AspectRatio,  b3Math::epsilon);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_Scene.left,   second.m_Scene.left,   b3Math::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_Scene.top,    second.m_Scene.top,    b3Math::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_Scene.right,  second.m_Scene.right,  b3Math::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_Scene.bottom, second.m_Scene.bottom, b3Math::epsilon);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_View.left,    second.m_View.left,    b3Math::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_View.top,     second.m_View.top,     b3Math::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_View.right,   second.m_View.right,   b3Math::epsilon);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(first.m_View.bottom,  second.m_View.bottom,  b3Math::epsilon);
	}

	return true;
}

#endif
