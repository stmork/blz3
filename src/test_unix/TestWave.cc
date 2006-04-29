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
**	$Log$
**	Revision 1.1  2006/04/29 11:25:50  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestOceanWave implementation                **
**                                                                      **
*************************************************************************/

int main(int argc, char *argv[])
{
	b3Display   *display;
	b3OceanWave  wave;

	wave.m_Dim = 8;
//	b3Log::b3SetLevel(B3LOG_FULL);

	try
	{
		b3Tx   tx;
		b3_res xMax, yMax;
		
		xMax = yMax = 1 << wave.m_Dim;
		
		// Create display
		display = new b3DisplayView(xMax, yMax, "OceanWave");
		
		tx.b3AllocTx(xMax, yMax, 128);
		
		b3Time now;
		b3_f64 time_start = now.b3GetTime(),time_diff;

		do
		{
			now.b3Now();
			time_diff = now.b3GetTime() - time_start;
			b3PrintF(B3LOG_NORMAL, "t=%3.3f\n",time_diff);

			wave.b3PrepareOceanWave(time_diff);
			wave.b3GetBuffer(&tx, 1.0);
			display->b3PutTx(&tx);
		}
		while(!display->b3IsCancelled(xMax,yMax));

		// Delete Display
		delete display;
	}
	catch(b3DisplayException &e)
	{
		b3PrintF(B3LOG_NORMAL,"### Error occured: %s\n",e.b3GetErrorMsg());
	}

	return EXIT_SUCCESS;
}
