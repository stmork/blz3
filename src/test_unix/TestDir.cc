/*
**
**	$Filename:	TestDir.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Testing directory routines
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
  
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Dir.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/09/30 15:46:07  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3TestDir implementation                      **
**                                                                      **
*************************************************************************/

void b3TestDir()
{
	b3Dir        dir;
	b3Path       name;
	b3_path_type code;

	dir.b3OpenDir(".");
	name.b3Empty();
	while ((code = dir.b3DirNext(name)) != B3_NOT_EXISTANT)
	{
		switch (code)
		{
			case B3_TYPE_FILE :
				b3PrintF (B3LOG_NORMAL,"f: %s\n",(const char *)name);
				break;
			case B3_TYPE_DIR :
				b3PrintF (B3LOG_NORMAL,"d: %s\n",(const char *)name);
				break;
			default :
				b3PrintF (B3LOG_NORMAL,"?: %s\n",(const char *)name);
				break;
		}
		name.b3Empty();
	}
	dir.b3CloseDir ();
}
