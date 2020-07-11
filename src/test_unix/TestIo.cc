/*
**
**      $Filename:      TestIo.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Test routines
**
**      (C) Copyright 2001  Steffen A. Mork
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
#include "blz3/base/b3World.h"
#include "blz3/raytrace/b3Activation.h"
#include "blz3/raytrace/b3Scene.h"
#include "blz3/system/b3SelfTest.h"

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

int main(int argc, char * argv[])
{
	b3_index      i;
	b3World       world;
	b3Item    *   item;
	b3Scene   *   scene;
	b3_bool       success = false;

	b3Log::b3SetLevel(B3LOG_FULL);

	try
	{
		success =
			b3SelfTest::b3TestDataSize() &&
			b3SelfTest::b3TestDate() &&
			b3SelfTest::b3TestMemory() &&
			b3SelfTest::b3TestIO();
	}
	catch(b3MemException & m)
	{
		b3PrintF(B3LOG_NORMAL, "Memory Error - %s\n", m.b3GetErrorMsg());
	}
	catch(b3FileException & f)
	{
		b3PrintF(B3LOG_NORMAL, "I/O Error - %s\n", f.b3GetErrorMsg());
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL, "Unknown exception occured.\n");
	}

	b3PrintF(B3LOG_NORMAL, "Tests %s.\n", success ? "successful" : "errornous");

	for(i = 1; i < argc; i++)
	{
		b3PrintF(B3LOG_NORMAL, "Checking >%s<\n", argv[i]);
		try
		{
#ifndef WIN32
			world.b3Read(argv[i]);
#else
			world.b3Read("M:\\Blizzard\\Data\\AllShapes");
#endif
			for(item  = world.b3GetFirst();
				item != null;
				item  = scene->Succ)
			{
				scene = (b3Scene *)item;
				scene->b3Reorg();
			}
			world.b3Dump();
			world.b3Write("/tmp/test.bwd");
			b3PrintF(B3LOG_NORMAL, "  File OK!\n");
		}
		catch(b3WorldException & e)
		{
			b3PrintF(B3LOG_NORMAL, "Error catched loading %s\n", argv[i]);
			b3PrintF(B3LOG_NORMAL, "  Error code: %d\n", e.b3GetError());
			b3PrintF(B3LOG_NORMAL, "  Error msg:  %s\n", e.b3GetErrorMsg());
			switch(e.b3GetError())
			{
			case B3_WORLD_OPEN:
				b3PrintF(B3LOG_NORMAL, "  Cannot open file.\n");
				break;
			case B3_WORLD_READ:
				b3PrintF(B3LOG_NORMAL, "  Cannot read file.\n");
				break;
			case B3_WORLD_WRITE:
				b3PrintF(B3LOG_NORMAL, "  Cannot write file.\n");
				break;
			case B3_WORLD_PARSE:
				b3PrintF(B3LOG_NORMAL, "  Cannot parse file.\n");
				break;
			case B3_WORLD_MEMORY:
				b3PrintF(B3LOG_NORMAL, "  Cannot allocate memory.\n");
				break;
			case B3_WORLD_NOT_REGISTERED:
				b3PrintF(B3LOG_NORMAL, "  Cannot instanciate unregistered class type.\n");
				break;
			case B3_WORLD_OUT_OF_ORDER:
				b3PrintF(B3LOG_NORMAL, "  Data and string written out of order.\n");
				break;

			default:
				b3PrintF(B3LOG_NORMAL, "  unknown error (%s).\n", e.b3GetErrorMsg());
				break;
			}
		}
	}

	b3PrintF(B3LOG_FULL, "Compile date: %s %s\n", __DATE__, __TIME__);
	b3PrintF(B3LOG_FULL, "%s\n", b3Runtime::b3GetCompiler());

#ifdef BLZ3_USE_SSE
	b3PrintF(B3LOG_FULL, "Using SSE intrinsics.\n");
#endif

#ifdef BLZ3_USE_SSE2
	b3PrintF(B3LOG_FULL, "Using SSE2 intrinsics.\n");
#endif

#ifdef BLZ3_USE_SSE3
	b3PrintF(B3LOG_FULL, "Using SSE3 intrinsics.\n");
#endif

#ifdef B3_SSE
	b3PrintF(B3LOG_FULL, "Using SSE algorithms.\n");
#endif

#ifdef B3_SSE1
	b3PrintF(B3LOG_FULL, "Using SSE (1) algorithms.\n");
#endif

#ifdef B3_SSE2
	b3PrintF(B3LOG_FULL, "Using SSE2 algorithms.\n");
#endif

	B3_BEEP;
	return EXIT_SUCCESS;
}
