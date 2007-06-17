/*
**
**	$Filename:	TestFilter.cc $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Testing filter computations
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Filter.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**
**	Revision 1.2  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.1  2002/08/22 14:06:32  sm
**	- Corrected filter support and added test suite.
**	- Added animation computing to brt3. Now we are near to
**	  real time raytracing: 8 fps for Animationtest.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

#define TEST_STEP  0.05
#define TEST_MIN  -1.2
#define TEST_MAX   (1.2 + (TEST_STEP * 0.5))

int main(int argc,char *argv[])
{
	b3Log::b3SetLevel(B3LOG_FULL);

	b3BoxFilter     box;
	b3GaussFilter   gauss;
	b3ShutterFilter shutter;
	b3_f64          x;

	b3PrintF(B3LOG_NORMAL,"x,"
		"Box f(x),Box Int(x),Box Kern(x),"
		"Gauss f(x),Gauss Int(x),Gauss Kern(x),"
		"Shutter f(x),Shutter Int(x),Shutter Kern(x)\n");
	for (x = TEST_MIN;x <= TEST_MAX;x += TEST_STEP)
	{
		b3PrintF(B3LOG_NORMAL,"%1.2lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",x,
			box.b3Func(x),    box.b3Integral(x),    box.b3InvIntegral(x),
			gauss.b3Func(x),  gauss.b3Integral(x),  gauss.b3InvIntegral(x),
			shutter.b3Func(x),shutter.b3Integral(x),shutter.b3InvIntegral(x));
	}
	return EXIT_SUCCESS;
}
