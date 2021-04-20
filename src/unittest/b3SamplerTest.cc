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
#define SAMPLE_RESOLUTION   640

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

class b3WoodSampler : public b3ImageSampler, public b3Wood
{
	b3_coord m_Translate;
	b3Color  m_Dark;
	b3Color  m_Light;

public:
	b3WoodSampler(b3Tx * tx) : b3ImageSampler(tx)
	{
		b3_vector scale_vector;
		b3_f64    scalar = 200.0 / SAMPLE_RESOLUTION;

		m_Translate = SAMPLE_RESOLUTION >> 1;
		m_Dark      = B3_BLACK;
		m_Light     = B3_WHITE;

		b3InitWood();
		b3Vector::b3Init(&scale_vector, scalar, scalar, scalar);
		b3PrepareWood(&scale_vector);
	}

protected:
	inline b3_color b3SamplePixel(const b3_coord x, const b3_coord y) override
	{
		b3_vector sample;
		b3Color   result;

		sample.y = x - m_Translate;
		sample.x = m_Translate - y;
		sample.z = m_Translate - x * 0.02;

		b3_f64 mix = b3ComputeWood(&sample, 1.0);
		result = b3Color::b3Mix(m_Dark, m_Light, mix);

		return result;
	}
};

class b3PlankSampler : public b3ImageSampler, public b3OakPlank
{
	b3_f64   m_Scale;
	b3Color  m_Dark;
	b3Color  m_Light;

public:
	b3PlankSampler(b3Tx * tx) : b3ImageSampler(tx)
	{
		b3_vector scale_vector;
		b3_f64    scalar = 10;

		m_Scale     = 1.0 / SAMPLE_RESOLUTION;
		m_Dark      = B3_BLACK;
		m_Light     = B3_WHITE;

		b3InitOakPlank();
#ifdef CREATE_ICON
		m_xOffset = 0.52;
		m_RingFrequency *= 0.1;
#endif
		b3Vector::b3Init(&scale_vector, scalar, scalar, scalar);
		b3PrepareOakPlank(&scale_vector);
	}

protected:
	inline b3_color b3SamplePixel(const b3_coord x, const b3_coord y) override
	{
		b3_vector sample;
		b3Color   result;
		b3_index  index;

		sample.x = x * m_Scale;
		sample.y = y * m_Scale;
		sample.z = 0;

		b3_f64 mix = b3ComputeOakPlank(&sample, 1.0, index);
		result = b3Color::b3Mix(m_Dark, m_Light, mix);

		return result;
	}
};

class b3WaterSampler : public b3ImageSampler, public b3Water
{
	b3Tx  * m_Tx;
	b3_f64  m_Factor;
	b3_f64  m_Time;

public:
	inline b3WaterSampler(b3Tx * tx) : b3ImageSampler(tx)
	{
		m_Factor = 1.0;
		m_Time   = 0.0;

		b3PrintF(B3LOG_NORMAL, "octaves=%d\n", m_Octaves);
		b3PrintF(B3LOG_NORMAL, "Km=%3.3f\n", m_Km);
		b3PrintF(B3LOG_NORMAL, "wind: frequency=%3.3f min=%3.3f amp=%3.3f\n",
			m_WindFreq, m_MinWind, m_WindAmp);
	}

	inline void b3SampleTime(b3_f64 time)
	{
		m_Time = time;
		b3PrepareWater(m_Time);
		b3Sample();
	}

private:
	inline b3_color b3SamplePixel(const b3_coord x, const b3_coord y) override
	{
		b3_vector pos;
		b3_f64    water;

		pos.x = m_Factor * x / m_xMax;
		pos.y = m_Factor * y / m_yMax;
		pos.z = 0;

		water = b3ComputeWater(&pos) * 0.5;

		return b3Color(water, water, water);
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

void b3SamplerTest::testSampler()
{
	b3Tx tx;

	CPPUNIT_ASSERT(tx.b3AllocTx(SAMPLE_RESOLUTION, SAMPLE_RESOLUTION, 128));

	b3MatWoodSampler      sampler_mat_wood(&tx);
	b3MatOakPlankSampler  sampler_mat_oak(&tx);
	b3BumpWoodSampler     sampler_bump_wood(&tx);
	b3BumpOakPlankSampler sampler_bump_oak(&tx);
	b3WoodSampler         sampler_wood(&tx);
	b3PlankSampler        sampler_plank(&tx);
	b3WaterSampler        sampler_water(&tx);

	sample(sampler_mat_wood,  tx, "img_test_sample_mw.tga");
	sample(sampler_mat_oak,   tx, "img_test_sample_mo.tga");
	sample(sampler_bump_wood, tx, "img_test_sample_bw.tga");
	sample(sampler_bump_oak,  tx, "img_test_sample_bo.tga");
	sample(sampler_wood,      tx, "img_test_sample_wood.tga");
	sample(sampler_plank,     tx, "img_test_sample_plank.tga");
	sample(sampler_water,     tx, "img_test_sample_water.tga");
}

void b3SamplerTest::testOceanWave()
{
	b3OceanWave  wave;
	b3Tx         tx;

	wave.m_Dim = 9;

	CPPUNIT_ASSERT(tx.b3AllocTx(128, 128, 128));

	for (float t = 0.0; t < 5.0; t += 0.125)
	{
		b3Path path;

		path.b3Format("img_test_ocean_%1.3f.jpg", t);
		wave.b3PrepareOceanWave(t);
		wave.b3GetBuffer(&tx, 0.001);
		CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(path));
	}
}

void b3SamplerTest::sample(b3Sampler & sampler, b3Tx & tx, const char * image_name)
{
	CPPUNIT_ASSERT_NO_THROW(sampler.b3Sample());
	CPPUNIT_ASSERT_EQUAL(B3_OK, tx.b3SaveImage(image_name));
}

#endif
