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
#include "blz3/system/b3Time.h"
#include "blz3/image/b3Sampler.h"
#include "blz3/base/b3Water.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.8  2006/04/29 11:25:50  sm
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
**  Revision 1.7  2006/04/19 10:20:30  sm
**  - Adjusted splitted includes.
**  - Adjusted ocean waves values.
**
**  Revision 1.6  2006/04/15 20:34:55  sm
**  - Added support for ocean surface bump mapping.
**
**  Revision 1.5  2005/12/12 16:01:32  smork
**  - Some more const correction in samplers.
**
**  Revision 1.4  2004/11/29 09:58:01  smork
**  - Changed exit states to correct defines.
**  - Added switch for disabling VBO in OpenGL renderer.
**  - Added switches for logging level in OpenGL renderer as in brt3.
**
**  Revision 1.3  2004/05/18 13:34:50  sm
**  - Cleaned up water animation
**
**  Revision 1.2  2004/05/18 10:44:52  sm
**  - Fine tuning animated water.
**
**  Revision 1.1  2004/05/16 18:50:59  sm
**  - Added new simple image sampler.
**  - We need better water!
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestWater implementation                    **
**                                                                      **
*************************************************************************/

#define WATER_RES   200

class b3WaterSampler : public b3ImageSampler, public b3Water
{
	b3Tx   *m_Tx;
	b3_f64  m_Factor;
	b3_f64  m_Time;

public:
	inline b3WaterSampler(b3Tx *tx) : b3ImageSampler(tx)
	{
		m_Factor = 1.0;
		m_Time   = 0.0;

		b3PrintF(B3LOG_NORMAL, "octaves=%ld\n",m_Octaves);
		b3PrintF(B3LOG_NORMAL, "Km=%3.3f\n",m_Km);
		b3PrintF(B3LOG_NORMAL, "wind: frequency=%3.3f min=%3.3f amp=%3.3f\n",
			m_WindFreq,m_MinWind,m_WindAmp);
	}
	
	inline void b3SampleTime(b3_f64 time)
	{
		m_Time = time;
		b3PrepareWater(m_Time);
		b3Sample();
	}

private:
	inline b3_color b3SamplePixel(const b3_coord x,const b3_coord y)
	{
		b3_vector pos;
		b3_f64    water;
		
		pos.x = m_Factor * x / m_xMax;
		pos.y = m_Factor * y / m_yMax;
		pos.z = 0;
		
		water = b3ComputeWater(&pos) * 0.5;
	
		return b3Color(water,water,water);
	}
};

int main(int argc,char *argv[])
{
	b3Display   *display;

	try
	{
		b3Tx   tx;
		b3_res xMax,yMax;
		
		// Create display
		display = new b3DisplayView(WATER_RES,WATER_RES,"Water");
		display->b3GetRes(xMax,yMax);
		
		tx.b3AllocTx(xMax, yMax, 128);
		
		b3WaterSampler sampler(&tx);

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
				now.b3Now();
				time_diff = now.b3GetTime() - time_start;
				b3PrintF(B3LOG_NORMAL, "t=%3.3f\n",time_diff);
				sampler.b3SampleTime(time_diff);
				display->b3PutTx(&tx);
			}
//			while(true);
			while(!display->b3IsCancelled(xMax,yMax));
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
