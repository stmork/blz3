/*
**
**	$Filename:      RestoreExt.cc $
**	$Release:       Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Setting correct extension on each file
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/raytrace/b3Raytrace.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**  $Log$
**  Revision 1.1  2002/08/25 13:03:02  sm
**  - Added a tool to restore correct file extensions.
**  - b3Tx can determine the image types' file extension.
**
**
*/

static const char *b3GetClassExt(b3_u32 world_class)
{
	switch(world_class)
	{
	case CLASS_GLOBAL:
		return "bwd";
	case CLASS_MATERIAL:
		return "bmd";
	case CLASS_BBOX:
		return "bod";
	case CLASS_CONDITION:
		return "bcd";
	case CLASS_BUMP:
		return "bbd";
	}
	return null;
}

static void b3TestFile(const char *filename)
{
	const char *ext = null;
	b3Path      new_filename;

	try
	{
		b3World  world;
		b3Item  *first;

		world.b3Read(filename);
		first = world.b3GetFirst();
		b3PrintF(B3LOG_DEBUG,"%s -> %08x\n",
			filename,first->b3GetClassType());
		ext = b3GetClassExt(first->b3GetClass());
	}
	catch(b3WorldException &w)
	{
		b3Tx image;

		if (image.b3LoadImage(filename) == B3_OK)
		{
			ext = image.b3GetExt();
		}
	}
	catch(...)
	{
		b3PrintF(B3LOG_NORMAL,"Unknown error with file %s\n",
			filename);
		return;
	}

	if (ext != null)
	{
		new_filename.b3RemoveExt(filename);
		strcat(new_filename,".");
		strcat(new_filename,ext);
		if (strcmp(filename,new_filename) != null)
		{
#if 1
			rename(filename,new_filename);
#else
			b3PrintF(B3LOG_NORMAL,"rename(%s,%s)\n",
				filename,(const char *)new_filename);
#endif
		}
	}
}

int main(int argc,char *argv[])
{
	int i;

	b3InitRaytrace::b3Init();
//	b3Log_SetLevel(B3LOG_FULL);
	for (i = 1;i < argc;i++)
	{
		b3TestFile(argv[i]);
	}
	return 0;
}