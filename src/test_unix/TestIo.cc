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

#include "blz3/b3Config.h"
#include "blz3/system/b3Date.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Dir.h"
#include "blz3/system/b3File.h"
#include "blz3/base/b3World.h"
#include "blz3/raytrace/b3Raytrace.h"

extern void b3TestMem();
extern void b3TestFile();
extern void b3TestDir();
extern void b3TestThread();

int main(int argc,char *argv[])
{
	b3_u32   i,k;
	b3Date   date;
	b3World  world;
	b3Scene *scene;

	b3Log_SetLevel(B3LOG_FULL);
	b3InitRaytrace::b3Init();

	scene = new b3Scene(TRACEPHOTO_MORK);
	scene->b3Dump(0);

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

	i = 1; k = 2;
	b3PrintF (B3LOG_NORMAL,"SWAP:  i=%ld k=%ld\n",i,k);
	B3_SWAP (i,k);
	b3PrintF (B3LOG_NORMAL,"       i=%ld k=%ld\n",i,k);
	i = 1; k = 2;
	b3PrintF (B3LOG_NORMAL,"PSWAP: i=%ld k=%ld\n",i,k);
	B3_PSWAP (&i,&k);
	b3PrintF (B3LOG_NORMAL,"       i=%ld k=%ld\n",i,k);

	b3PrintF (B3LOG_NORMAL,"\ntesting 'Bio.c'...\n");

	date.b3Y2K_Selftest();

	b3TestMem();
	b3TestFile();
	b3TestDir();

	for (i = 1;i < argc;i++)
	{
		b3PrintF(B3LOG_NORMAL,"Checking >%s<\n",argv[i]);
		try
		{
			world.b3Read(argv[i]);
			world.b3Dump();
			b3PrintF(B3LOG_NORMAL,"  File OK!\n");
		}
		catch(b3WorldException *e)
		{
			b3PrintF(B3LOG_NORMAL,"Error catched loading %s\n",argv[i]);
			switch (e->b3GetError())
			{
			case B3_WORLD_OPEN:
				b3PrintF(B3LOG_NORMAL,"  Cannot open file.\n");
				break;
			case B3_WORLD_READ:
				b3PrintF(B3LOG_NORMAL,"  Cannot read file.\n");
				break;
			case B3_WORLD_PARSE:
				b3PrintF(B3LOG_NORMAL,"  Cannot parse file.\n");
				break;
			case B3_WORLD_MEMORY:
				b3PrintF(B3LOG_NORMAL,"  Cannot allocate memory.\n");
				break;
			default:
				b3PrintF(B3LOG_NORMAL,"  unknown error (%d).\n",e->b3GetError());
				break;
			}
		}
	}

	B3_BEEP;
}
