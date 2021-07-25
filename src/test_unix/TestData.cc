/*
**
**      $Filename:      TestData.cc $
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
#include "blz3/base/b3FileList.h"
#include "blz3/base/b3World.h"

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

static void b3TestFile(const char * filename)
{
	b3World   world;
	b3_count  stage = 0;

	try
	{
		b3PrintF(B3LOG_NORMAL, "%s\n", filename);
		stage++;
		world.b3Read(filename);
		stage++;
		world.b3Write("/tmp/test.bwd");
		stage++;
		world.b3Read("/tmp/test.bwd");
		stage++;
	}
	catch (b3WorldException & e)
	{
		b3PrintF(B3LOG_NORMAL, "Error catched processing %s (stage: %d)\n", filename, stage);
		b3PrintF(B3LOG_NORMAL, "  Error code: %d\n", e.b3GetError());
		b3PrintF(B3LOG_NORMAL, "  Error msg:  %s\n", e.b3GetErrorMsg());
		switch (e.b3GetError())
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
			b3PrintF(B3LOG_NORMAL, "  unknown error!\n");
			break;
		}
		b3PrintF(B3LOG_NORMAL, "-----------------------------------------\n");
	}
	catch (...)
	{
		b3PrintF(B3LOG_NORMAL, "Another unknown error occured...\n");
		b3PrintF(B3LOG_NORMAL, "Filename: %s\n", filename);
		b3PrintF(B3LOG_NORMAL, "-----------------------------------------\n");
	}
}

int main(int argc, char * argv[])
{
	b3_index i;

	for (i = 1; i < argc; i++)
	{
		if (b3Dir::b3Exists(argv[i]) == B3_TYPE_DIR)
		{
			b3FileList   list(argv[i]);
			b3FileEntry * entry;

			for (entry = list.b3First(); entry != nullptr; entry = entry->Succ)
			{
				b3TestFile(entry->b3Name());
			}
		}
		else
		{
			b3TestFile(argv[i]);
		}
	}

	return EXIT_SUCCESS;
}
