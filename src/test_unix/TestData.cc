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
**	Revision 1.7  2006/03/15 13:32:30  smork
**	- Syntax corrections.
**
**	Revision 1.6  2004/11/29 09:58:01  smork
**	- Changed exit states to correct defines.
**	- Added switch for disabling VBO in OpenGL renderer.
**	- Added switches for logging level in OpenGL renderer as in brt3.
**	
**	Revision 1.5  2004/05/07 16:30:33  sm
**	- Bug #13 fixed. The BBox hierarchy is recounted on every
**	  object edit finish.
**	- Wooden materials contain a dark and a separate light
**	  material. Changes were also made in Lines.
**	- Introduced shape property copy including all materials,
**	  bumps and conditions. Multiple copy modes are possible.
**	
**	Revision 1.4  2003/07/12 17:44:47  sm
**	- Cleaned up raytracing b3Item registration
**	
**	Revision 1.3  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
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
	catch(b3WorldException &e)
	{
		b3PrintF(B3LOG_NORMAL,"Error catched processing %s (stage: %d)\n",filename,stage);
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
			b3PrintF(B3LOG_NORMAL,"  unknown error!\n");
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

	for (i = 1;i < argc;i++)
	{
		if (b3Dir::b3Exists(argv[i]) == B3_TYPE_DIR)
		{
			b3FileList   list(argv[i]);
			b3FileEntry *entry;

			for(entry = list.b3First();entry != null;entry = entry->Succ)
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
