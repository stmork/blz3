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

	CPPUNIT_ASSERT(search_path.b3IsValid("Shapes.bwd", full_path));
	CPPUNIT_ASSERT(world.b3Read(full_path, false));

	for (b3Item * item  = world.b3GetFirst();
		item != nullptr;
		item  = item->Succ)
	{
		b3Scene * scene = (b3Scene *)item;
		b3_res   xSize, ySize;

		CPPUNIT_ASSERT(scene != nullptr);
		CPPUNIT_ASSERT_NO_THROW(scene->b3Reorg());
		CPPUNIT_ASSERT_NO_THROW(scene->b3GetDisplaySize(xSize, ySize));
		CPPUNIT_ASSERT_NO_THROW(scene->b3SetupVertexMemory(&context));
		CPPUNIT_ASSERT(scene->b3PrepareScene(xSize, ySize));
		CPPUNIT_ASSERT_NO_THROW(scene->b3ResetAnimation());
	}
}

#endif
