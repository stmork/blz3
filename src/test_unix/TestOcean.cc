/*
**
**	$Filename:	TestOcean.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Animating ocean water
**
**	(C) Copyright 2006  Steffen A. Mork
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
#include "blz3/system/b3Time.h"
#include "blz3/image/b3Sampler.h"
#include "blz3/base/b3Math.h"
#include "blz3/base/b3Matrix.h"
#include "blz3/base/b3OceanWave.h"

/*************************************************************************
**                                                                      **
**                        b3TestOceanWave implementation                **
**                                                                      **
*************************************************************************/

#define WATER_RES 400

class b3OceanWaveSampler : public b3ImageSampler, public b3OceanWave
{
	b3Tx  * m_Tx;
	b3_f64  m_Factor;
	b3_f64  m_Amplitude;
	b3_f64  m_Time;

	inline b3_color b3SamplePixel(const b3_coord x, const b3_coord y)
	{
		b3_vector pos;
		b3_vector n;
		b3_f64    height;
		b3_f64    water;
		b3_f64    factor = 0.001;

#if 0
		pos.x = m_Factor * x / m_xMax;
		pos.y = m_Factor * y / m_yMax;
#else
		pos.x = x;
		pos.y = y;
#endif
		pos.z = 0;

		height = b3ComputeOceanWave(&pos);
		water = b3Math::b3Limit(height * factor * 3 + 0.5);
		n.x = n.y = height;
#if 1
		b3ComputeOceanWaveDeriv(&pos, &n);
		b3Vector::b3Normalize(&n);
#endif

		//		return b3Color(water, water, water);
		return b3Color(
				water,
				b3Math::b3Limit(water + n.x * m_Amplitude),
				b3Math::b3Limit(water + n.y * m_Amplitude));
		//	printf("%d %d %f\n",x, y, n.x * factor + 0.5);
		//		return b3Color(n.x * factor + 0.5, n.x * factor + 0.5, n.x * factor + 0.5);
		//		return b3Color(n.y * factor + 0.5, n.y * factor + 0.5, n.y * factor + 0.5);
	}

public:
	inline b3OceanWaveSampler(b3Tx * tx) : b3ImageSampler(tx)
	{
		m_Factor    = m_GridSize;
		m_Amplitude = 0.2f;
		m_Dim       = 8;
	}

	inline void b3SampleTime(b3_f64 time)
	{
		m_Time = time;
		b3PrepareOceanWave(m_Time);
		b3Sample();
	}
};

int main(int argc, char * argv[])
{
	b3Display  * display;

	//	b3Log::b3SetLevel(B3LOG_FULL);

	try
	{
		b3Tx   tx;
		b3_res xMax, yMax;

		// Create display
		display = new b3DisplayView(WATER_RES, WATER_RES, "OceanWave");
		display->b3GetRes(xMax, yMax);

		tx.b3AllocTx(xMax, yMax, 128);

		b3OceanWaveSampler sampler(&tx);

		if(argc > 1)
		{
			sampler.b3Sample();

			// We want to see the computed picture until we make input
			// into the display window.
			display->b3PutTx(&tx);
			display->b3Wait();
#ifdef CREATE_ICON
			if(argc > 3)
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
		else
		{
			b3Time now;
			b3_f64 time_start = now.b3GetTime(), time_diff;

			do
			{
				time_diff = now.b3Now() - time_start;
				b3PrintF(B3LOG_NORMAL, "t=%3.3f\n", time_diff);
				sampler.b3SampleTime(time_diff);
				display->b3PutTx(&tx);
			}
			//			while(true);
			while(!display->b3IsCancelled(xMax, yMax));
		}

		// Delete Display
		delete display;
	}
	catch(b3DisplayException & e)
	{
		b3PrintF(B3LOG_NORMAL, "### Error occured: %s\n", e.b3GetErrorMsg());
	}

	return EXIT_SUCCESS;
}
