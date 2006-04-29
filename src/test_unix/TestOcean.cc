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
#include "blz3/base/b3OceanWave.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.6  2006/04/29 20:51:10  sm
**  - New scaling.
**
**  Revision 1.5  2006/04/29 11:25:50  sm
**  - Added ocean bump to main packet.
**  - b3Prepare signature: Added further initialization information
**    for animation preparation
**  - Added test module for ocean waves.
**  - Added module for random number generation.
**  - Adjusted material and bump sampler to reflect preparation
**    signature change.
**  - Added OpenGL test program for ocean waves.
**  - Changed Phillips spectrum computation to be independent
**    from time.
**  - Interpolated height field for ocean waves.
**
**  Revision 1.4  2006/04/19 15:19:58  sm
**  - The break through! Ocean waves...
**
**  Revision 1.3  2006/04/19 10:20:30  sm
**  - Adjusted splitted includes.
**  - Adjusted ocean waves values.
**
**  Revision 1.2  2006/04/17 14:42:46  sm
**  - Completed ocean waves. I see ocean waves. They are not nice but
**    I can see them!
**
**  Revision 1.1  2006/04/15 20:34:55  sm
**  - Added support for ocean surface bump mapping.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestOceanWave implementation                **
**                                                                      **
*************************************************************************/

#define WATER_RES   400

class b3OceanWaveSampler : public b3ImageSampler, public b3OceanWave
{
	b3Tx   *m_Tx;
	b3_f64  m_Factor;
	b3_f64  m_Amplitude;
	b3_f64  m_Time;

	inline b3_color b3SamplePixel(const b3_coord x,const b3_coord y)
	{
		b3_vector pos;
		b3_f64    water;

#if 0
		pos.x = m_Factor * x / m_xMax;
		pos.y = m_Factor * y / m_yMax;
#else
		pos.x = x;
		pos.y = y;
#endif
		pos.z = 0;

		water = b3Math::b3Limit(b3ComputeOceanWave(&pos) * 0.0002 + 0.5);

		return b3Color(water,water,water);
	}

public:
	inline b3OceanWaveSampler(b3Tx *tx) : b3ImageSampler(tx)
	{
		m_Factor    = m_GridSize;
		m_Amplitude = 1;
		m_Dim       = 8;
	}
	
	inline void b3SampleTime(b3_f64 time)
	{
		m_Time = time;
		b3PrepareOceanWave(m_Time);
		b3Sample();
	}
};

int main(int argc,char *argv[])
{
	b3Display   *display;

//	b3Log::b3SetLevel(B3LOG_FULL);

	try
	{
		b3Tx   tx;
		b3_res xMax,yMax;
		
		// Create display
		display = new b3DisplayView(WATER_RES,WATER_RES,"OceanWave");
		display->b3GetRes(xMax,yMax);
		
		tx.b3AllocTx(xMax, yMax, 128);
		
		b3OceanWaveSampler sampler(&tx);

		if (argc > 1)
		{
			sampler.b3Sample();

			// We want to see the computed picture until we make input
			// into the display window.
			display->b3PutTx(&tx);
			display->b3Wait();
#ifdef CREATE_ICON
			if (argc > 3)
			{
				b3Tx small,big;

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
			b3_f64 time_start = now.b3GetTime(),time_diff;

			do
			{
				time_diff = now.b3Now() - time_start;
				b3PrintF(B3LOG_NORMAL, "t=%3.3f\n",time_diff);
				sampler.b3SampleTime(time_diff);
				display->b3PutTx(&tx);
			}
//			while(true);
			while(!display->b3IsCancelled(xMax, yMax));
		}

		// Delete Display
		delete display;
	}
	catch(b3DisplayException &e)
	{
		b3PrintF(B3LOG_NORMAL,"### Error occured: %s\n",e.b3GetErrorMsg());
	}

	return EXIT_SUCCESS;
}
