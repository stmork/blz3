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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.24  2006/05/18 10:06:42  sm
**	- Some SSE tests.
**
**	Revision 1.23  2006/03/15 13:32:30  smork
**	- Syntax corrections.
**	
**	Revision 1.22  2005/08/12 11:35:49  sm
**	- Readded vector multiplication for performance test issues.
**	
**	Revision 1.21  2005/05/16 08:18:40  sm
**	- Some bug fixes...
**	
**	Revision 1.20  2005/05/16 07:59:23  sm
**	- Moved some self testing methods into library to make them usable
**	  from other applications.
**	
**	Revision 1.19  2005/05/07 14:06:06  sm
**	- Corrected va-list handling in Windows version.
**	- Using correct lib directory on 64 bit architectures.
**	- Added login script version for (ba)sh.
**	
**	Revision 1.18  2005/01/05 13:01:25  smork
**	- Fixed 64 bit problem.
**	
**	Revision 1.17  2005/01/04 15:13:59  smork
**	- Changed some data types.
**	
**	Revision 1.16  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.15  2004/11/25 11:09:37  smork
**	- Added b3_f96 checking in TestIo.
**	- Corrected linking in test_unix.
**	
**	Revision 1.14  2003/10/16 08:25:55  sm
**	- Moved CPU bit count into b3CPUBase class
**	
**	Revision 1.13  2003/10/15 13:12:19  sm
**	- Making 64 bit sure. I've got running the brt3 in 64 bit mode on
**	  a sun ultra sparc sucessfully!!
**	
**	Revision 1.12  2003/07/12 17:44:47  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.11  2003/02/20 16:34:47  sm
**	- Some logging cleanup
**	- New base class for b3CPU (b3CPUBase)
**	- b3Realloc bug fix on Solaris
**	
**	Revision 1.10  2003/02/19 16:52:53  sm
**	- Cleaned up logging
**	- Clean up b3CPU/b3Runtime
**	
**	Revision 1.9  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
**	Revision 1.8  2002/01/03 15:50:15  sm
**	- Added cut/copy/paste
**	
**	Revision 1.7  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

int main(int argc,char *argv[])
{
	b3_index      i;
	b3World       world;
	b3Item       *item;
	b3Scene      *scene;
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
	catch(b3MemException &m)
	{
		b3PrintF(B3LOG_NORMAL,"Memory Error - %s\n",m.b3GetErrorMsg());
	}
	catch(b3FileException &f)
	{
		b3PrintF(B3LOG_NORMAL,"I/O Error - %s\n",f.b3GetErrorMsg());
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"Unknown exception occured.\n");
	}
	
	b3PrintF(B3LOG_NORMAL,"Tests %s.\n", success ? "successful" : "errornous");

	for (i = 1;i < argc;i++)
	{
		b3PrintF(B3LOG_NORMAL,"Checking >%s<\n",argv[i]);
		try
		{
#ifndef WIN32
			world.b3Read(argv[i]);
#else
			world.b3Read("M:\\Blizzard\\Data\\AllShapes");
#endif
			for (item  = world.b3GetFirst();
			     item != null;
			     item  = scene->Succ)
			{
				scene = (b3Scene *)item;
				scene->b3Reorg();
			}
			world.b3Dump();
			world.b3Write("/tmp/test.bwd");
			b3PrintF(B3LOG_NORMAL,"  File OK!\n");
		}
		catch(b3WorldException &e)
		{
			b3PrintF(B3LOG_NORMAL,"Error catched loading %s\n",argv[i]);
			b3PrintF(B3LOG_NORMAL,"  Error code: %d\n",e.b3GetError());
			b3PrintF(B3LOG_NORMAL,"  Error msg:  %s\n",e.b3GetErrorMsg());
			switch (e.b3GetError())
			{
			case B3_WORLD_OPEN:
				b3PrintF(B3LOG_NORMAL,"  Cannot open file.\n");
				break;
			case B3_WORLD_READ:
				b3PrintF(B3LOG_NORMAL,"  Cannot read file.\n");
				break;
			case B3_WORLD_WRITE:
				b3PrintF(B3LOG_NORMAL,"  Cannot write file.\n");
				break;
			case B3_WORLD_PARSE:
				b3PrintF(B3LOG_NORMAL,"  Cannot parse file.\n");
				break;
			case B3_WORLD_MEMORY:
				b3PrintF(B3LOG_NORMAL,"  Cannot allocate memory.\n");
				break;
			case B3_WORLD_NOT_REGISTERED:
				b3PrintF(B3LOG_NORMAL,"  Cannot instanciate unregistered class type.\n");
				break;
			case B3_WORLD_OUT_OF_ORDER:
				b3PrintF(B3LOG_NORMAL,"  Data and string written out of order.\n");
				break;

			default:
				b3PrintF(B3LOG_NORMAL,"  unknown error (%s).\n",e.b3GetErrorMsg());
				break;
			}
		}
	}

	b3PrintF(B3LOG_FULL, "Compile date: %s %s\n",__DATE__,__TIME__);
	b3PrintF(B3LOG_FULL, "%s\n",b3Runtime::b3GetCompiler());

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
