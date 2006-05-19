/*
**
**	$Filename:	unittest.cc $
**	$Release:	Dortmund 2006 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Base and system unit tests
**
**      (C) Copyright 2006  Steffen A. Mork
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

#include "blz3/b3Config.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#endif

using namespace std;

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.4  2006/05/19 07:02:58  sm
**	- Corrected FFT unit test.
**
**	Revision 1.3  2006/05/18 10:06:42  sm
**	- Some SSE tests.
**	
**	Revision 1.2  2006/05/12 14:06:28  smork
**	- Added configurable CPPUNIT tests.
**	
**	Revision 1.1  2006/05/11 15:34:23  sm
**	- Added unit tests
**	- Corrected normal computation for ocean waves
**	- Optimized b3Complex
**	- Added new FFT
**	- Added own assertion include
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Main unit test runner                         **
**                                                                      **
*************************************************************************/

int main(int argc, char *argv[])
{
	b3PrintF(B3LOG_NORMAL, "Compile date: %s %s\n",__DATE__,__TIME__);
	b3PrintF(B3LOG_NORMAL, "%s\n",b3Runtime::b3GetCompiler());

#ifdef BLZ3_USE_SSE
	b3PrintF(B3LOG_NORMAL, "Using SSE intrinsics.\n");
#endif

#ifdef BLZ3_USE_SSE2
	b3PrintF(B3LOG_NORMAL, "Using SSE2 intrinsics.\n");
#endif

#ifdef BLZ3_USE_SSE3
	b3PrintF(B3LOG_NORMAL, "Using SSE3 intrinsics.\n");
#endif

#ifdef B3_SSE
	b3PrintF(B3LOG_NORMAL, "Using SSE algorithms.\n");
#endif

#ifdef B3_SSE1
	b3PrintF(B3LOG_NORMAL, "Using SSE (1) algorithms.\n");
#endif

#ifdef B3_SSE2
	b3PrintF(B3LOG_NORMAL, "Using SSE2 algorithms.\n");
#endif
#ifdef HAVE_LIBCPPUNIT
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

	b3Log::b3SetLevel(B3LOG_NONE);
	if ((argc >= 2) && (argv[1][0] == '-'))
	{
		switch(argv[1][1])
		{
		case 'f':
			b3Log::b3SetLevel(B3LOG_FULL);
			break;
		case 'd':
			b3Log::b3SetLevel(B3LOG_DEBUG);
			break;
		case 'n':
			b3Log::b3SetLevel(B3LOG_NONE);
			break;
		default:
			b3Log::b3SetLevel(B3LOG_NONE);
			break;
		}
	}
	runner.addTest( registry.makeTest());
	
	return runner.run ("", false) ? EXIT_SUCCESS : EXIT_FAILURE;
#else
	b3PrintF(B3LOG_NORMAL, "No cppunit package available.\n");
	return EXIT_SUCCESS;
#endif
}
