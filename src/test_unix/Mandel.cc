/*
**
**      $Filename:      Mandel.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - computing Mandelbrot set
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Log.h"
#include "blz3/system/b3Display.h"
#include "blz3/system/b3Thread.h"

#include "b3Mandel.h"

#include <stdio.h>
#include <stdlib.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2001/09/30 15:46:07  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**
**
*/

/*************************************************************************
**                                                                      **
**                        Mandel implementation                         **
**                                                                      **
*************************************************************************/

int main(int argc,char *argv[])
{
	b3_f64       xMin,yMin;
	b3_f64       xMax,yMax;
	b3_count     iter;
	b3Display   *display;

	b3Log_SetLevel(B3LOG_FULL);
	if (argc != 6)
	{
		printf("USAGE:\n");
		printf("%s xMin yMin xMax yMax iterations\n",argv[0]);
	}
	else
	{
		b3PrintF(B3LOG_NORMAL,"Multithreaded Mandelbrot computing...\n");
		xMin = atof(argv[1]);
		xMax = atof(argv[2]);
		yMin = atof(argv[3]);
		yMax = atof(argv[4]);
		iter = atoi(argv[5]);
		b3PrintF(B3LOG_NORMAL,"Using following values:\n");
		b3PrintF(B3LOG_NORMAL,"Width  %f - %f:\n",xMin,xMax);
		b3PrintF(B3LOG_NORMAL,"Height %f - %f:\n",yMin,yMax);
		b3PrintF(B3LOG_NORMAL,"%lu iterations:\n",iter);

		try
		{
			// Create display
			display = new b3Display();

			// Compute Mandelbrot set
			b3Mandel::b3Compute(display,
				xMin,xMax,yMin,yMax,iter);

			// We want to see the computed picture until we make input
			// into the display window.
			display->b3Wait();

			// Delete Display
			delete display;
		}
		catch(b3DisplayException *e)
		{
			b3PrintF(B3LOG_NORMAL,"### Error occured: %d\n",e->b3GetError());
		}
	}
	return 0;
}
