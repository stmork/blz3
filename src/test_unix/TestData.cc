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
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/01/03 15:50:15  sm
**	- Added cut/copy/paste
**
**	Revision 1.1  2001/12/31 11:05:18  sm
**	- Added TestData for testing Blizzard data structures for reading
**	  and writing.
**	- Fixed bugs found with previous mentioned tool:
**	  o Some b3AnimElement errors found to be fixed under Windows.
**	  o b3TriangleShape destructor handled unchecked m_GridList pointer
**	- Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**	  B3LOG_NORMAL to B3LOG_DEBUG.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

static void b3TestFile(const char *filename)
{
	b3World   world;
	b3_count  stage = 0;

	try
	{
		b3PrintF(B3LOG_NORMAL,"%s\n",filename);
		stage++;
		world.b3Read(filename);
		stage++;
		world.b3Write("/tmp/test.bwd");
		stage++;
		world.b3Read("/tmp/test.bwd");
		stage++;
	}
	catch(b3WorldException *e)
	{
		b3PrintF(B3LOG_NORMAL,"Error catched processing %s (stage: %d)\n",filename,stage);
		switch (e->b3GetError())
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
			b3PrintF(B3LOG_NORMAL,"  unknown error (%d).\n",e->b3GetError());
			break;
		}
		b3PrintF(B3LOG_NORMAL,"-----------------------------------------\n");
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"Another unknown error occured...\n");
		b3PrintF(B3LOG_NORMAL,"Filename: %s\n",filename);
		b3PrintF(B3LOG_NORMAL,"-----------------------------------------\n");
	}
}

int main(int argc,char *argv[])
{
	b3_index i;

	b3InitRaytrace::b3Init();
	for (i = 1;i < argc;i++)
	{
		b3FileList   list(argv[i]);
		b3FileEntry *entry;

		for(entry = list.b3First();entry != null;entry = entry->Succ)
		{
			b3TestFile(entry->b3Name());
		}
	}

	return 0;
}
