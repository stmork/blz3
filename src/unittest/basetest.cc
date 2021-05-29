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

#include "blz3/system/b3Runtime.h"
#include "blz3/raytrace/b3Factory.h"

#ifdef HAVE_LIBCPPUNIT
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#endif

using namespace std;

/*************************************************************************
**                                                                      **
**                        Main unit test runner                         **
**                                                                      **
*************************************************************************/

int main(int argc, char * argv[])
{
	char hostname[_SC_HOST_NAME_MAX];

	b3PrintF(B3LOG_NORMAL, "Type of tests: %s\n", argv[0]);
	b3PrintF(B3LOG_NORMAL, "Compile date:  %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_NORMAL, "Compiler:      %s\n", b3Runtime::b3GetCompiler());

	if (b3Runtime::b3Hostname(hostname, sizeof(hostname)))
	{
		b3PrintF(B3LOG_NORMAL, "Hostname:      %s\n", hostname);
	}

#ifdef BLZ3_USE_SSE
	b3PrintF(B3LOG_NORMAL, "Using SSE intrinsics.\n");
#endif

#ifdef BLZ3_USE_SSE2
	b3PrintF(B3LOG_NORMAL, "Using SSE2 intrinsics.\n");
#endif

#ifdef BLZ3_USE_SSE3
	b3PrintF(B3LOG_NORMAL, "Using SSE3 intrinsics.\n");
#endif

#ifdef BLZ3_USE_SSSE3
	b3PrintF(B3LOG_NORMAL, "Using Supplemental SSE3 intrinsics.\n");
#endif

#ifdef SSE_ALIGNED
	b3PrintF(B3LOG_NORMAL, "Memory alignment SSE compatible.\n");
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
	std::string results_xml(argv[0]);

	ofstream xml(results_xml + "-results.xml");

	CppUnit::TextUi::TestRunner    runner;
	CppUnit::TestFactoryRegistry & registry  = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::XmlOutputter     *    outputter = new CppUnit::XmlOutputter(&runner.result(), xml);

	b3RaytracingItems::b3Register();
	b3Log::b3SetLevel(B3LOG_NONE);
	if ((argc >= 2) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
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
	runner.setOutputter(outputter);
	runner.addTest(registry.makeTest());

	int result = runner.run("", false) ? EXIT_SUCCESS : EXIT_FAILURE;
	xml.close();

	return result;
#else
	b3PrintF(B3LOG_NORMAL, "No cppunit package available.\n");
	return EXIT_SUCCESS;
#endif
}
