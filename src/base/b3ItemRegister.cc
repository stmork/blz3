/*
**
**      $Filename:      b3ItemRegister.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Blizzard file structure loading
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3World.h"
#include "blz3/system/b3File.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.12  2006/03/05 21:22:33  sm
**      - Added precompiled support for faster comiling :-)
**
**      Revision 1.11  2004/11/26 16:28:17  smork
**      - Equalizing VERBOSE print outs.
**      - pthread error messages only in _DEBUG version.
**
**      Revision 1.10  2003/07/20 10:10:16  sm
**      - Fixed typo.
**
**      Revision 1.9  2003/07/20 09:21:18  sm
**      - Added item register dump.
**
**      Revision 1.8  2003/07/12 10:20:16  sm
**      - Fixed ticketno. 12 (memory leak in b3ItemRegistry)
**
**      Revision 1.7  2003/06/26 08:15:43  sm
**      - Cleaned up b3ItemRegister
**      - Added writing support for unregistered b3Item instances.
**
**      Revision 1.6  2002/08/05 16:04:55  sm
**      - Found first texture init bug. This wasn't an OpenGL bug. This
**        couldn't be because every implementation had got the same
**        bug. The static aux image for creating textures wasn't initialized
**        at the right time.
**      - Version handling introduced: The version number is extracted
**        from the version resource now.
**      - The b3Tx::b3AllocTx() method uses b3Realloc() for better
**        memory usage.
**      - Some b3World messages removed.
**      - The 0x7fff class is registered into the b3ItemRegister now. This
**        prevents printing a warning when this class isn't found. Due to
**        the fact that *every* Blizzard data contains this class every
**        data read put out this warning.
**
**      Revision 1.5  2001/10/19 14:46:57  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.4  2001/08/14 07:03:28  sm
**      - Made some ASSERT cleanups. New define when _DEBUG is switched on:
**        B3_ASSERT(condition) abort()s when condition is false.
**
**      Revision 1.3  2001/08/05 19:53:43  sm
**      - Removing some nasty CR/LF
**
**      Revision 1.2  2001/08/05 19:51:56  sm
**      - Now having OpenGL software for Windows NT and created
**        new Lines III.
**
**      Revision 1.1  2001/08/05 12:46:06  sm
**      - Splitting b3ItemXXX routines
**
**      Revision 1.6  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
**      Revision 1.5  2001/07/08 12:30:06  sm
**      - New tool to remove nasty CR/LF from Windoze.
**      - Removing some nasty CR/LF with that new tool.
**
**      Revision 1.4  2001/07/02 19:28:25  sm
**      - Applying console application on Windows 32
**      - Added further Windows environment
**
**      Revision 1.3  2001/07/02 16:09:46  sm
**      - Added bounding box reorganization.
**
**      Revision 1.2  2001/07/01 16:31:52  sm
**      - Creating MSVC Projects
**      - Welcome to Windows 32
**
**      Revision 1.1.1.1  2001/07/01 12:24:58  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        b3FirstItem: which contains Blizzard data     **
**                                                                      **
*************************************************************************/

b3FirstItem::b3FirstItem(b3_u32  class_type) : b3Item(sizeof(b3FirstItem),class_type)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase();
}

b3FirstItem::b3FirstItem(b3_u32 *src) : b3Item(src)
{
}

void b3FirstItem::b3Write()
{
}

b3Base<b3Item> *b3FirstItem::b3GetHead()
{
	return &m_Heads[0];
}

/*************************************************************************
**                                                                      **
**                        b3Item: one node inside file (base class)     **
**                                                                      **
*************************************************************************/

// b3Base<b3ItemRegisterEntry> b3ItemRegister::m_Classes;
b3ItemRegister b3ItemRegister::m_Register;

b3ItemRegister::b3ItemRegister()
{
	b3ItemRegisterEntry *entry = new b3ItemRegisterEntry(
		&b3FirstItem::b3StaticInit,
		&b3FirstItem::b3StaticInit,B3_CLASS_MAX,true);
	b3Append(entry);
}

b3ItemRegister::~b3ItemRegister()
{
	b3Free();
}

b3ItemRegisterEntry *b3ItemRegister::b3Find(b3_u32 class_type)
{
	b3ItemRegisterEntry *entry;

	B3_FOR_BASE(&m_Register,entry)
	{
		if (entry->b3IsClassType(class_type))
		{
#ifdef VERBOSE
			b3PrintF (B3LOG_FULL,"%08lx found,\n",class_type);
#endif
			// Some kind of LRU
			m_Register.b3Remove(entry);
			m_Register.b3First(entry);
			return entry;
		}
	}
	b3PrintF (B3LOG_DEBUG,"b3ItemRegister::b3Find(%08lx) not found.\n",class_type);
	return null;
}

void b3ItemRegister::b3Dump()
{
	b3ItemRegisterEntry *entry;

	b3PrintF(B3LOG_FULL,"Item register contains following classes:\n");
	B3_FOR_BASE(&m_Register,entry)
	{
		entry->b3Dump();
	}
}
