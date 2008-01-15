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
	b3World   world;

	switch (b3Runtime::b3GetCPUType())
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
		catch (b3WorldException &e)
		{
			b3PrintF(B3LOG_NORMAL,"Error catched loading %s\n",argv[i]);
			b3PrintF(B3LOG_NORMAL,"  Error code: %d\n",e.b3GetError());
			b3PrintF(B3LOG_NORMAL,"  Error msg:  %s\n",e.b3GetErrorMsg());
		}
	}

	return EXIT_SUCCESS;
}
