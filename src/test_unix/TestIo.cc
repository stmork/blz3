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
#include "blz3/system/b3Date.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3World.h"
#include "blz3/base/b3FileMem.h"
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

extern void b3TestMem();
extern void b3TestFile(b3FileAbstract &file);
extern void b3TestDir();
extern void b3TestThread();

int main(int argc,char *argv[])
{
	b3_index      i;
	b3_u32        v1,v2;
	b3Date        date;
	b3File        file;
	b3FileMem     filemem;
	b3World       world;
	b3Item       *item;
	b3Scene      *scene;
	b3_path_type  code;

	b3Log::b3SetLevel(B3LOG_FULL);
	b3InitRaytrace::b3Init();

	switch(b3Runtime::b3GetCPUType())
	{
	case B3_BIG_ENDIAN:
		b3PrintF (B3LOG_NORMAL,"Big endian (MC 680x0)\n");
		break;

	case B3_LITTLE_ENDIAN:
		b3PrintF (B3LOG_NORMAL,"Little endian (x86)\n");
		break;

	default:
		b3PrintF (B3LOG_NORMAL,"Endian type unknown!\n");
		break;
	}

	b3PrintF (B3LOG_NORMAL,"size of some basic data types:\n");
	b3PrintF (B3LOG_NORMAL,"8 bit:  uns. sgn.\n");
	b3PrintF (B3LOG_NORMAL,"Integer:\n");
	b3PrintF (B3LOG_NORMAL,"8 bit:  %3d  %3d (should be 1)\n",sizeof(b3_u08),sizeof(b3_s08));
	b3PrintF (B3LOG_NORMAL,"16 bit: %3d  %3d (should be 2)\n",sizeof(b3_u16),sizeof(b3_s16));
	b3PrintF (B3LOG_NORMAL,"32 bit: %3d  %3d (should be 4)\n",sizeof(b3_u32),sizeof(b3_s32));
	b3PrintF (B3LOG_NORMAL,"64 bit: %3d  %3d (should be 8)\n",sizeof(b3_u64),sizeof(b3_s64));
	b3PrintF (B3LOG_NORMAL,"bool:   %3d      (should be 4)\n",sizeof(b3_bool));
	b3PrintF (B3LOG_NORMAL,"Float:\n");
	b3PrintF (B3LOG_NORMAL,"32 bit: %3d      (should be 4)\n",sizeof(b3_f32));
	b3PrintF (B3LOG_NORMAL,"64 bit: %3d      (should be 8)\n",sizeof(b3_f64));
	b3PrintF (B3LOG_NORMAL,"\n");

	v1 = 1; v2 = 2;
	b3PrintF (B3LOG_NORMAL,"SWAP:  i=%ld k=%ld\n",v1,v2);
	B3_SWAP (v1,v2);
	b3PrintF (B3LOG_NORMAL,"       i=%ld k=%ld\n",v1,v2);
	v1 = 1; v2 = 2;
	b3PrintF (B3LOG_NORMAL,"PSWAP: i=%ld k=%ld\n",v1,v2);
	B3_PSWAP (&v1,&v2);
	b3PrintF (B3LOG_NORMAL,"       i=%ld k=%ld\n",v1,v2);

	b3PrintF (B3LOG_NORMAL,"\ntesting 'Bio.c'...\n");

	date.b3Y2K_Selftest();

	try
	{
		b3TestMem();
		b3PrintF(B3LOG_NORMAL,"Disk file: --------------------\n");
		if (file.b3Open("Config.tst",B_WRITE))
		{
			b3TestFile(file);
		}

		b3PrintF(B3LOG_NORMAL,"Memory file: ------------------\n");
		if (filemem.b3Open(B_WRITE))
		{
			b3TestFile(filemem);
		}

		b3PrintF(B3LOG_NORMAL,"file operations: --------------\n");
		if (remove ("Config.tst") == 0) b3PrintF (B3LOG_NORMAL,"File 'Config.tst' removed...\n");
		else b3PrintF (B3LOG_NORMAL,"File 'Config.tst' not removed...\n");

		code = b3Dir::b3Exists ("Config.tst");
		switch (code)
		{
			case B3_NOT_EXISTANT :
				b3PrintF (B3LOG_NORMAL,"Config.tst is not existant... (all right)\n");
				break;
			case B3_TYPE_DIR :
				b3PrintF (B3LOG_NORMAL,"Config.tst is a directory...\n");
				break;
			case B3_TYPE_FILE :
				b3PrintF (B3LOG_NORMAL,"Config.tst is a file...\n");
				break;

			default :
				b3PrintF (B3LOG_NORMAL,"Config.tst is if unknown file type (code %ld)\n",code);
				break;
		}
		b3TestDir();
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
			case B3_WORLD_STORAGE_NOT_IMPLEMENTED:
				b3PrintF(B3LOG_NORMAL,"  Cannot call unimplemented b3Item::b3Write().\n");
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

	B3_BEEP;
	return 0;
}
