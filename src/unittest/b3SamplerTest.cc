/*
**
**	$Filename:	b3SamplerTest.cc $
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

#include "b3SamplerTest.h"
#include "b3TestMacros.h"

#include "blz3/raytrace/b3BumpSampler.h"
#include "blz3/raytrace/b3MaterialSampler.h"

/*************************************************************************
**                                                                      **
**                        Unit test skeleton                            **
**                                                                      **
*************************************************************************/

#ifdef HAVE_LIBCPPUNIT

CPPUNIT_TEST_SUITE_REGISTRATION(b3SamplerTest);

/*************************************************************************
**                                                                      **
**                        Some material and bump sampler                **
**                                                                      **
*************************************************************************/

static b3_vector scale =
{
	1.0,
	1.0,
	1.0
};

class b3MatWoodSampler : public b3MaterialSampler
{
public:
	b3MatWoodSampler(b3Tx * tx) : b3MaterialSampler(tx, &scale)
	{
		// Init material
		m_Material = new b3MatWood(WOOD);
		m_Material->b3Prepare(nullptr);
	}

	virtual ~b3MatWoodSampler()
	{
		delete m_Material;
	}
};

class b3MatOakPlankSampler : public b3MaterialSampler
{
public:
	b3MatOakPlankSampler(b3Tx * tx) : b3MaterialSampler(tx, &scale, 1)
	{
		b3MatOakPlank * material = new b3MatOakPlank(OAKPLANK);

#ifdef CREATE_ICON
		material->m_LightWood = material->m_DarkWood * 2.0;
		material->m_xScale *= 7;
		material->m_yScale *= 8;
		material->m_xOffset = 0.52;
		material->m_RingFrequency *= 0.1;
#else
		material->m_xScale *= 2;
		material->m_yScale *= 2;
#endif

		// Init material
		m_Material = material;
		m_Material->b3Prepare(nullptr);
	}

	virtual ~b3MatOakPlankSampler()
	{
		delete m_Material;
	}
};

class b3BumpWoodSampler : public b3BumpSampler
{
public:
	b3BumpWoodSampler(b3Tx * tx) : b3BumpSampler(tx, &scale, 1)
	{
		// Init material
		m_Bump = (b3Bump *)b3World::b3AllocNode(BUMP_WOOD);
		m_Bump->b3Prepare(nullptr);
	}

	virtual ~b3BumpWoodSampler()
	{
		delete m_Bump;
	}
};

class b3BumpOakPlankSampler : public b3BumpSampler
{
public:
	b3BumpOakPlankSampler(b3Tx * tx) : b3BumpSampler(tx, &scale, 1)
	{
		b3BumpOakPlank * bump = new b3BumpOakPlank(BUMP_OAKPLANK);

		bump->m_xScale *= 4;
		bump->m_yScale *= 4;

		// Init material
		m_Bump = bump;
		m_Bump->b3Prepare(nullptr);
	}

	virtual ~b3BumpOakPlankSampler()
	{
		delete m_Bump;
	}
};

/*************************************************************************
**                                                                      **
**                        Sampler tests                                 **
**                                                                      **
*************************************************************************/

void b3SamplerTest::setUp()
{
	b3PrintF(B3LOG_DEBUG, "Setup: %s\n", __FILE__);
}

void b3SamplerTest::tearDown()
{
	b3PrintF(B3LOG_DEBUG, "Tear down: %s\n", __FILE__);
}

void b3SamplerTest::test()
{
	b3Tx tx;

	CPPUNIT_ASSERT(tx.b3AllocTx(512, 512, 128));

	b3MatWoodSampler      sampler_mat_wood(&tx);
	b3MatOakPlankSampler  sampler_mat_oak(&tx);
	b3BumpWoodSampler     sampler_bump_wood(&tx);
	b3BumpOakPlankSampler sampler_bump_oak(&tx);

	sample(sampler_mat_wood,  tx, "img_test_sample_mw.tga");
	sample(sampler_mat_oak,   tx, "img_test_sample_mo.tga");
	sample(sampler_bump_wood, tx, "img_test_sample_bw.tga");
	sample(sampler_bump_oak,  tx, "img_test_sample_bo.tga");
}

void b3SamplerTest::sample(b3Sampler & sampler, b3Tx & tx, const char * image_name)
{
	CPPUNIT_ASSERT_NO_THROW(sampler.b3Sample());
	CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(image_name));
}

#endif
