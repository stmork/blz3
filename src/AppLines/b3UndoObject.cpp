/*
**
**	$Filename:	b3UndoObject.cpp
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for object handling
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

#include "b3UndoObject.h"
#include "DlgHierarchy.h"
#include "MainFrm.h"
#include "DlgCreateItem.h"
#include "DlgObjectCopy.h"
#include "b3SelectObject.h"
#include "blz3/system/b3File.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2003/01/12 19:21:37  sm
**	- Some other undo/redo actions added (camera etc.)
**
**	Revision 1.2  2003/01/11 17:16:15  sm
**	- Object handling with undo/redo
**	- Light handling with undo/redo
**	
**	Revision 1.1  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo object creation                     **
**                                                                      **
*************************************************************************/

b3OpObjectCreate::b3OpObjectCreate(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy,
	b3_bool        insert_sub) : b3OpObject(scene,hierarchy)
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	CMainFrame     *main = CB3GetMainFrame();
	CDlgCreateItem  dlg;

	m_Selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(m_Selected != null);
	if (insert_sub)
	{
		m_Base = m_Selected->b3GetBBoxHead();
		m_InsertAfter = m_Base->Last;
	}
	else
	{
		m_Base = m_Scene->b3FindBBoxHead(m_Selected);
		m_InsertAfter = m_Selected;
	}
	B3_ASSERT(m_Base != null);

	dlg.m_ClassType   = CLASS_BBOX;
	dlg.m_MaxNameLen  = B3_BOXSTRINGLEN;
	dlg.m_ItemBase    = m_Base;
	dlg.m_NoNameCheck = true;
	dlg.m_Suggest.LoadString(IDS_NEW_OBJECT);
	if (dlg.DoModal() == IDOK)
	{
		m_BBox = new b3BBox(BBOX);
		strcpy (m_BBox->m_BoxName,dlg.m_NewName);
		b3Initialize();
		m_PrepareGeometry         = false;
		m_PrepareChangedStructure = true;
	}
}

void b3OpObjectCreate::b3Delete()
{
	if (!b3IsDone() && (m_BBox != null))
	{
		delete m_BBox;
	}
}

void b3OpObjectCreate::b3Undo()
{
	m_Base->b3Remove(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpObjectCreate::b3Redo()
{
	m_Base->b3Insert(m_InsertAfter,m_BBox);
	b3BBox::b3Recount(m_Scene->b3GetBBoxHead());
	m_DlgHierarchy->b3SelectItem(m_BBox);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object deletion                     **
**                                                                      **
*************************************************************************/

b3OpObjectDelete::b3OpObjectDelete(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy) : b3OpObject(scene,hierarchy)
{
	if (AfxMessageBox(IDS_ASK_DELETE_OBJECT,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		m_Selected = m_DlgHierarchy->b3GetSelectedBBox();
		m_Prev     = m_Selected->Prev;
		m_Select   = (m_Selected->Succ != null ? m_Selected->Succ : m_Selected->Prev);
		m_Base = m_Scene->b3FindBBoxHead(m_Selected);
		b3Initialize(m_Selected != null);
		m_PrepareGeometry         = false;
		m_PrepareChangedStructure = true;
	}
}

void b3OpObjectDelete::b3Delete()
{
	if (b3IsDone() && (m_Selected != null))
	{
		delete m_Selected;
	}
}

void b3OpObjectDelete::b3Undo()
{
	m_Base->b3Insert(m_Prev,m_Selected);
	m_DlgHierarchy->b3SelectItem(m_Selected);
	m_Scene->b3BacktraceRecompute(m_Selected);
}

void b3OpObjectDelete::b3Redo()
{
	m_Scene->b3BacktraceRecompute(m_Selected);
	m_Base->b3Remove(m_Selected);
	m_DlgHierarchy->b3SelectItem(m_Select);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object load                         **
**                                                                      **
*************************************************************************/

b3OpObjectLoad::b3OpObjectLoad(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy,
	const char    *regitem) : b3OpObject(scene,hierarchy)
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	CString         suggest;
	b3Path          filepath;
	b3Path          result;

	m_Selected = m_DlgHierarchy->b3GetSelectedBBox();
	m_Base     = (m_Selected == null ? m_Scene->b3GetBBoxHead() : m_Scene->b3FindBBoxHead(m_Selected));
	suggest    = app->GetProfileString(CB3ClientString(),regitem,"");

	b3Path::b3SplitFileName(suggest,filepath,null);
	if (m_Selected != null)
	{
		result.b3LinkFileName(filepath,m_Selected->b3GetName());
		result.b3RemoveExt();
		strcat((char *)result,".bod");
	}
	else
	{
		strcpy((char *)result,suggest);
	}
	
	if (CB3SelectObject::b3Select((char *)result))
	{
		app->WriteProfileString(CB3ClientString(),regitem,result);
		
		try
		{
			b3File  file(result,B_READ);

			if(m_World.b3Read(&file) == B3_WORLD_OK)
			{
				m_BBox  = (b3BBox *)m_World.b3GetFirst();
				m_Level = m_BBox->b3GetClassType() & 0xffff;
				b3Initialize();
				m_PrepareGeometry         = true;
				m_PrepareChangedStructure = false;
			}
		}
		catch(b3FileException &f)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: reading object from file %s (code: %d)\n",
				(const char *)result,f.b3GetError());
			B3_MSG_ERROR(f);
		}
		catch(b3WorldException &w)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: reading object from file %s (code: %d)\n",
				(const char *)result,w.b3GetError());
			B3_MSG_ERROR(w);
		}
	}
}

void b3OpObjectLoad::b3Delete()
{
	if (!b3IsDone() && (m_BBox != null))
	{
		delete m_BBox;
	}
}

void b3OpObjectLoad::b3Do()
{
	b3BBox::b3Reorg(m_World.b3GetHead(),m_Base,m_Level,1,m_Selected);
	b3BBox::b3Recount(m_Scene->b3GetBBoxHead());
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
	m_PrepareChangedStructure = true;
}

void b3OpObjectLoad::b3Undo()
{
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_Base->b3Remove(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpObjectLoad::b3Redo()
{
	m_Base->b3Insert(m_Selected,m_BBox);
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object replacement                  **
**                                                                      **
*************************************************************************/

b3OpObjectReplace::b3OpObjectReplace(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy,
	const char    *regitem) : b3OpObjectLoad(scene,hierarchy,regitem)
{
}

void b3OpObjectReplace::b3Delete()
{
	if (b3IsDone() && (m_Selected != null))
	{
		delete m_Selected;
	}
}

void b3OpObjectReplace::b3Do()
{
	b3BBox::b3Reorg(m_World.b3GetHead(),m_Base,m_Level,1,m_Selected);
	m_Base->b3Remove(m_Selected);
	b3BBox::b3Recount(m_Scene->b3GetBBoxHead());
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
	m_PrepareChangedStructure = true;
}

void b3OpObjectReplace::b3Undo()
{
	m_Base->b3Insert(m_BBox,m_Selected);
	m_Base->b3Remove(m_BBox);
	m_Scene->b3BacktraceRecompute(m_Selected);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpObjectReplace::b3Redo()
{
	m_Base->b3Insert(m_Selected,m_BBox);
	m_Base->b3Remove(m_Selected);
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object copy                         **
**                                                                      **
*************************************************************************/

b3OpObjectCopy::b3OpObjectCopy(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy,
	b3_vector     *fulcrum) : b3OpObject(scene,hierarchy)
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	CMainFrame     *main = CB3GetMainFrame();
	CDlgObjectCopy  dlg;
	b3BBox         *bbox;
	b3BBox         *cloned;
	b3_count        i;

	m_Selected = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(m_Selected != null);
	m_Base     = m_Scene->b3FindBBoxHead(m_Selected);
	dlg.m_OrigBBox = m_Selected;
	dlg.m_Center   = fulcrum;
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor     wait;

		bbox = m_Selected;
		for (i = 0;i < dlg.m_NumCopies;i++)
		{
			cloned = (b3BBox *)b3World::b3Clone(bbox);
			cloned->b3Transform(&dlg.m_Transformation,true,true);
			cloned->b3Prepare(true);
			m_ClonedBBoxes.b3Add(cloned);

			bbox = cloned;
		}
		b3Initialize();
		m_PrepareChangedStructure = true;
	}
}

void b3OpObjectCopy::b3Delete()
{
	if (!b3IsDone())
	{
		for (b3_index i = 0;i < m_ClonedBBoxes.b3GetCount();i++)
		{
			delete m_ClonedBBoxes[i];
		}
	}
}

void b3OpObjectCopy::b3Undo()
{
	CWaitCursor  wait;

	for (b3_index i = 0;i < m_ClonedBBoxes.b3GetCount();i++)
	{
		m_Base->b3Remove(m_ClonedBBoxes[i]);
	}
}

void b3OpObjectCopy::b3Redo()
{
	CWaitCursor  wait;
	b3BBox      *prev;
	b3BBox      *cloned;

	prev = m_Selected;
	for (b3_index i = 0;i < m_ClonedBBoxes.b3GetCount();i++)
	{
		cloned = m_ClonedBBoxes[i];
		m_Base->b3Insert(prev,cloned);
		prev = cloned;
	}
}
