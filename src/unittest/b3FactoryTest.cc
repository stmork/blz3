/*
**
**	$Filename:	b3FactoryTest.cc $
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

#include "b3FactoryTest.h"
#include "b3TestMacros.h"

#include "blz3/raytrace/b3Factory.h"
#include "blz3/raytrace/b3Material.h"
#include "blz3/raytrace/b3Bump.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3FactoryTest);

b3FactoryTest::b3FactoryTest() : m_Display(512, 512)
{
}

void b3FactoryTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3FactoryTest::tearDown()
{
	delete m_Scene;

	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3FactoryTest::testNew()
{
	m_Scene = b3Factory::b3CreateNew(__FILE__);

	CPPUNIT_ASSERT(m_Scene != nullptr);
	CPPUNIT_ASSERT_NO_THROW(m_Scene->b3Raytrace(&m_Display));
}

void b3FactoryTest::testGlobal()
{
	m_Scene = b3Factory::b3CreateGlobal();

	CPPUNIT_ASSERT(m_Scene != nullptr);
	CPPUNIT_ASSERT_NO_THROW(m_Scene->b3Raytrace(&m_Display));
}

void b3FactoryTest::testMaterial()
{
	b3Base<b3Item> * head = nullptr;

	m_Scene = b3Factory::b3CreateMaterial(&head, TRACEANGLE_MORK);

	CPPUNIT_ASSERT(m_Scene != nullptr);
	CPPUNIT_ASSERT(head != nullptr);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u32, CLASS_MATERIAL, head->b3GetClass());
	CPPUNIT_ASSERT_NO_THROW(m_Scene->b3Raytrace(&m_Display));
}

void b3FactoryTest::testBump()
{
	b3Base<b3Item> * head = nullptr;

	m_Scene = b3Factory::b3CreateBump(&head, TRACEANGLE_MORK);

	CPPUNIT_ASSERT(m_Scene != nullptr);
	CPPUNIT_ASSERT(head != nullptr);
	CPPUNIT_ASSERT_TYPED_EQUAL(b3_u32, CLASS_BUMP, head->b3GetClass());
	CPPUNIT_ASSERT_NO_THROW(m_Scene->b3Raytrace(&m_Display));
}

#endif
