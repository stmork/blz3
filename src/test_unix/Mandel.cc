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

#include "blz3/b3Config.h"
#include "blz3/system/b3DisplayView.h"

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
**	Revision 1.11  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**
**	Revision 1.10  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.9  2002/08/11 11:22:33  sm
**	- Adjusted some includes concerning b3DisplayView.
**	
**	Revision 1.8  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.7  2002/01/22 17:11:18  sm
**	- brt3 is now able to save images. The selection of image type
**	  is unsoved yet.
**	- Better b3DisplayView in Un*x port.
**	- Fixed stricmp() in Un*x port.
**	
**	Revision 1.6  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**	
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

	b3Log::b3SetLevel(B3LOG_FULL);
	if (argc != 6)
	{
		printf("USAGE:\n");
		printf("%s xMin xMax yMin yMax iterations\n",argv[0]);
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
			display = new b3DisplayView();

			// Compute Mandelbrot set
			b3Mandel::b3Compute(display,
				xMin,xMax,yMin,yMax,iter);

			// We want to see the computed picture until we make input
			// into the display window.
			display->b3Wait();

			// Delete Display
			delete display;
		}
		catch(b3DisplayException &e)
		{
			b3PrintF(B3LOG_NORMAL,"### Error occured: %s\n",e.b3GetErrorMsg());
		}
	}
	return EXIT_SUCCESS;
}
