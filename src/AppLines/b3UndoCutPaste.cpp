/*
**
**	$Filename:	b3UndoCutPaste.cpp
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3UndoCutPaste.h"
#include "blz3/base/b3FileMem.h"
#include "MainFrm.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2003/01/11 17:16:15  sm
**	- Object handling with undo/redo
**	- Light handling with undo/redo
**
**	Revision 1.1  2003/01/05 16:13:24  sm
**	- First undo/redo implementations
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo Cut                                 **
**                                                                      **
*************************************************************************/

b3OpCut::b3OpCut()
{
}

void b3OpCut::b3Undo()
{
}

void b3OpCut::b3Redo()
{
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo Paste                               **
**                                                                      **
*************************************************************************/

b3OpPaste::b3OpPaste(
	CDlgHierarchy *hierarchy,
	b3Scene       *scene,
	b3_bool        insert_sub)
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	void           *ptr;
	b3_size         size;
	HANDLE          handle;

	m_DlgHierarchy = hierarchy;
	m_Scene        = scene;
	selected       = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	if (insert_sub)
	{
		base = selected->b3GetBBoxHead();
		insert_after = base->Last;
	}
	else
	{
		base = m_Scene->b3FindBBoxHead(selected);
		insert_after = selected;
	}
	B3_ASSERT(base != null);

	if (main->OpenClipboard())
	{
		handle = ::GetClipboardData(app->m_ClipboardFormatForBlizzardObject);
		if (handle != 0)
		{
			size = ::GlobalSize(handle);
			ptr  = ::GlobalLock(handle);
			try
			{
				b3FileMem file(B_WRITE);

				file.b3Write(ptr,size);
				file.b3Seek(0,B3_SEEK_START);

				if(world.b3Read(&file) == B3_WORLD_OK)
				{
					bbox  = (b3BBox *)world.b3GetFirst();
					b3Initialize();
				}
			}
			catch(b3FileException &f)
			{
				b3PrintF(B3LOG_NORMAL,"I/O ERROR: reading object from clipboard (code: %d)\n",
					f.b3GetError());
				B3_MSG_ERROR(f);
			}
			catch(b3WorldException &w)
			{
				b3PrintF(B3LOG_NORMAL,"ERROR: reading object from clipboard (code: %d)\n",
					w.b3GetError());
				B3_MSG_ERROR(w);
			}
			::GlobalUnlock(handle);
		}
		::CloseClipboard();
	}
}

void b3OpPaste::b3Undo()
{

}

void b3OpPaste::b3Redo()
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	CMainFrame     *main = CB3GetMainFrame();
	b3_count        level;

	main->b3SetStatusMessage(IDS_DOC_REORG);
	level = bbox->b3GetClassType() & 0xffff;
	b3BBox::b3Reorg(world.b3GetHead(),base,level,1,insert_after);
	b3BBox::b3Recount(m_Scene->b3GetBBoxHead());
	m_Scene->b3BacktraceRecompute(bbox);
	m_DlgHierarchy->b3SelectItem(bbox);
}
