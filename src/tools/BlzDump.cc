/*
**
**      $Filename:      BlzDump.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Make hexdump of Blizzard file
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "blz3/b3Config.h"
#include "blz3/base/b3World.h"
#include "blz3/raytrace/b3Raytrace.h"

int main(int argc,char *argv[])
{
	b3_index  i;
	b3_u32    v1,v2;
	b3World   world;
	b3Item   *item;
	b3Scene  *scene;

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

	for (i = 1;i < argc;i++)
	{
		b3PrintF(B3LOG_NORMAL,"Checking >%s<\n",argv[i]);
		try
		{
			world.b3ReadDump(argv[i]);
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
	return 0;
}
