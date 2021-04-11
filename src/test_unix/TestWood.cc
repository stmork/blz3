/*
**
**	$Filename:	TestWood.cc $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Displaying Wood
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

#include "blz3/system/b3DisplayView.h"
#include "blz3/raytrace/b3BumpSampler.h"
#include "blz3/raytrace/b3MaterialSampler.h"

#define WOOD_RES   640
#define no_CREATE_ICON

/*************************************************************************
**                                                                      **
**                        b3WoodSampler implementation                  **
**                                                                      **
*************************************************************************/

class b3WoodSampler : public b3ImageSampler, public b3Wood
{
	b3_coord m_Translate;
	b3Color  m_Dark;
	b3Color  m_Light;

public:
	b3WoodSampler(b3Tx * tx) : b3ImageSampler(tx)
	{
		b3_vector scale;
		b3_f64    scalar = 200.0 / WOOD_RES;

		m_Translate = WOOD_RES >> 1;
		m_Dark      = B3_BLACK;
		m_Light     = B3_WHITE;

		b3InitWood();
		b3Vector::b3Init(&scale, scalar, scalar, scalar);
		b3PrepareWood(&scale);
	}

protected:
	inline b3_color b3SamplePixel(const b3_coord x, const b3_coord y)
	{
		b3_vector sample;
		b3Color   result;

		sample.y = x - m_Translate;
		sample.x = m_Translate - y;
		sample.z = m_Translate - x * 0.02;

		b3_f64 mix = b3ComputeWood(&sample, 1.0);
		result = b3Color::b3Mix(m_Dark, m_Light, mix);
		b3PrintF(B3LOG_FULL,
			"%3zd %3zd: %06x - %1.4f %2.3f %2.3f %2.3f\n",
			x, y, b3_pkd_color(result), mix, sample.x, sample.y, sample.z);
		return result;
	}
};

/*************************************************************************
**                                                                      **
**                        b3PlankSampler implementation                 **
**                                                                      **
*************************************************************************/

class b3PlankSampler : public b3ImageSampler, public b3OakPlank
{
	b3_f64   m_Scale;
	b3Color  m_Dark;
	b3Color  m_Light;

public:
	b3PlankSampler(b3Tx * tx) : b3ImageSampler(tx)
	{
		b3_vector scale;
		b3_f64    scalar = 10;

		m_Scale     = 1.0 / WOOD_RES;
		m_Dark      = B3_BLACK;
		m_Light     = B3_WHITE;

		b3InitOakPlank();
#ifdef CREATE_ICON
		m_xOffset = 0.52;
		m_RingFrequency *= 0.1;
#endif
		b3Vector::b3Init(&scale, scalar, scalar, scalar);
		b3PrepareOakPlank(&scale);
	}

protected:
	inline b3_color b3SamplePixel(const b3_coord x, const b3_coord y)
	{
		b3_vector sample;
		b3Color   result;
		b3_index  index;

		sample.x = x * m_Scale;
		sample.y = y * m_Scale;
		sample.z = 0;

		b3_f64 mix = b3ComputeOakPlank(&sample, 1.0, index);
		result = b3Color::b3Mix(m_Dark, m_Light, mix);
		b3PrintF(B3LOG_NORMAL,
			"%3zd %3zd: %06x - %1.4f / %2zd - %2.3f %2.3f %2.3f\n",
			x, y, b3_pkd_color(result), mix, index, sample.x, sample.y, sample.z);
		return result;
	}
};

int main(int argc, char * argv[])
{
	b3Display  * display;

	b3RaytracingItems::b3Register();
	try
	{
		b3Tx   tx;
		b3_res xMax, yMax;

		// Create display
		display = new b3DisplayView(WOOD_RES, WOOD_RES, "Wood");
		display->b3GetRes(xMax, yMax);

		tx.b3AllocTx(xMax, yMax, 128);

#if 1
		b3WoodSampler  sampler(&tx);
#else
		b3PlankSampler  sampler(&tx);
#endif

		b3Time span;
		sampler.b3Sample();
		b3_f64 used = b3Time() - span;

		b3PrintF(B3LOG_NORMAL, "Time used: %1.3fs = %1.5fms/px\n", used, used * 1000.0 / (tx.xSize * tx.ySize));
		// We want to see the computed picture until we make input
		// into the display window.
		display->b3PutTx(&tx);
		display->b3Wait();

		if (argc > 1)
		{
#ifdef CREATE_ICON
			if (argc > 3)
			{
				b3Tx small, big;

				small.b3AllocTx(32, 32, 24);
				small.b3ScaleToGrey(&tx);
				small.b3SaveTGA(argv[2]);

				big.b3AllocTx(48, 48, 24);
				big.b3ScaleToGrey(&tx);
				big.b3SaveTGA(argv[3]);
			}
			tx.b3SaveTGA(argv[1]);
#else
			tx.b3SaveJPEG(argv[1]);
#endif
		}

		// Delete Display
		delete display;
	}
	catch (b3DisplayException & e)
	{
		b3PrintF(B3LOG_NORMAL, "### Error occured: %s\n", e.b3GetErrorMsg());
	}

	return EXIT_SUCCESS;
}
