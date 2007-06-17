/*
**
**	$Filename:	b3UndoObject.cpp
**	$Release:	Dortmund 2003 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

#include "AppLinesInclude.h"

#include "blz3/system/b3File.h"
#include "DlgHierarchy.h"
#include "DlgCreateItem.h"
#include "DlgObjectCopy.h"
#include "DlgObjectTransformMode.h"
#include "b3UndoObject.h"
#include "b3SelectObject.h"

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
		m_BBox->b3SetName(dlg.b3GetText());
		b3Initialize();
		m_PrepareGeometry         = true;
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
	m_Scene->b3Recount();
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
**                        read file operations on objects               **
**                                                                      **
*************************************************************************/

#define B3_OBJECT_LOAD_REGITEM "file load.object"
#define B3_OBJECT_REPLACE_REGITEM "file replace.object"

b3OpObjectFile::b3OpObjectFile(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy,
	const char    *reg_entry) : b3OpObject(scene,hierarchy)
{
	CWaitCursor  wait;
	b3Path       result = "";

	m_Selected = m_DlgHierarchy->b3GetSelectedBBox();
	if (m_Selected != null)
	{
		m_Base = m_Scene->b3FindBBoxHead(m_Selected);
	}
	else
	{
		m_Base = m_Scene->b3GetBBoxHead();
	}
	if (CB3SelectLoadObject::b3Select(result,reg_entry))
	{
		try
		{
			b3File  file(result,B_READ);

			if(m_World.b3Read(&file) == B3_WORLD_OK)
			{
				b3Base<b3Item> *depot = m_World.b3GetHead();
				b3Base<b3Item>  base;
			
				m_BBox  = (b3BBox *)m_World.b3GetFirst();
				m_Level = m_BBox->b3GetType();
				base.b3InitBase(depot->b3GetClass());
				b3BBox::b3Reorg(depot,&base,m_Level,1);

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

/*************************************************************************
**                                                                      **
**                        Undo/Redo object load                         **
**                                                                      **
*************************************************************************/

b3OpObjectLoad::b3OpObjectLoad(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy) : b3OpObjectFile(scene,hierarchy,B3_OBJECT_LOAD_REGITEM)
{
	if (b3IsInitialized())
	{
		b3_object_transform_mode mode = CDlgObjectTransformLoad::b3Dialog();

		switch(mode)
		{
		case OBJECT_TRANSFORM_DELETE:
			m_BBox->b3ResetTransformation();
			break;

		case OBJECT_TRANSFORM_RESET:
			m_BBox->b3Inverse(null);
			break;
		}
		b3Initialize(mode != OBJECT_TRANSFORM_CANCEL);
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
	m_Base->b3Insert(m_Selected,m_BBox);
	m_Scene->b3Recount();
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
	CDlgHierarchy *hierarchy) : b3OpObjectFile(scene,hierarchy,B3_OBJECT_REPLACE_REGITEM)
{
	if (b3IsInitialized())
	{
		b3_object_transform_mode mode = CDlgObjectTransformReplace::b3Dialog();

		switch(mode)
		{
		case OBJECT_TRANSFORM_USE_OLD:
			m_BBox->b3Inverse(null);
			m_BBox->b3Transform(&m_Selected->m_Matrix, true, true);
			break;

		case OBJECT_TRANSFORM_DELETE:
			m_BBox->b3ResetTransformation();
			break;

		case OBJECT_TRANSFORM_RESET:
			m_BBox->b3Inverse(null);
			break;
		}
		b3Initialize(mode != OBJECT_TRANSFORM_CANCEL);
	}
}

void b3OpObjectReplace::b3Delete()
{
	if (b3IsDone())
	{
		if (m_Selected != null)
		{
			delete m_Selected;
		}
	}
	else
	{
		B3_ASSERT(m_BBox != null);
		delete m_BBox;
	}
}

void b3OpObjectReplace::b3Do()
{
	m_Base->b3Insert(m_Selected,m_BBox);
	m_Base->b3Remove(m_Selected);
	m_Scene->b3Recount();
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
**                        Undo/Redo Caligari object import              **
**                                                                      **
*************************************************************************/

b3OpObjectLoadCob::b3OpObjectLoadCob(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy) : b3OpObject(scene,hierarchy)
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	b3Path          result = "";

	m_Selected = m_DlgHierarchy->b3GetSelectedBBox();
	m_Base     = (m_Selected == null ? m_Scene->b3GetBBoxHead() : m_Scene->b3FindBBoxHead(m_Selected));
	
	if (CB3SelectLoadCOB::b3Select(result))
	{
		try
		{
			m_BBox = b3BBox::b3ReadCOB(result);
			if (m_BBox != null)
			{
				b3Path name;

				result.b3SplitFileName(null,name);
				name.b3RemoveExt();
				m_BBox->b3SetName(name);
				b3Initialize();
				m_PrepareGeometry         = true;
				m_PrepareChangedStructure = false;
			}
		}
		catch(b3FileException &f)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: reading Caligari object from file %s (code: %d)\n",
				(const char *)result,f.b3GetError());
			B3_MSG_ERROR(f);
		}
		catch(b3WorldException &w)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: reading Caligari object from file %s (code: %d)\n",
				(const char *)result,w.b3GetError());
			B3_MSG_ERROR(w);
		}
	}
}

void b3OpObjectLoadCob::b3Delete()
{
	if (!b3IsDone() && (m_BBox != null))
	{
		delete m_BBox;
	}
}

void b3OpObjectLoadCob::b3Do()
{
	m_Base->b3Insert(m_Selected,m_BBox);
	m_Scene->b3Recount();
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
	m_PrepareChangedStructure = true;
}

void b3OpObjectLoadCob::b3Undo()
{
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_Base->b3Remove(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpObjectLoadCob::b3Redo()
{
	m_Base->b3Insert(m_Selected,m_BBox);
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo ArCon object import                 **
**                                                                      **
*************************************************************************/

b3OpObjectLoadTgf::b3OpObjectLoadTgf(
	b3Scene       *scene,
	CDlgHierarchy *hierarchy) : b3OpObject(scene,hierarchy)
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	CMainFrame     *main = CB3GetMainFrame();
	CWaitCursor     wait;
	b3Path          result = "";

	m_Selected = m_DlgHierarchy->b3GetSelectedBBox();
	m_Base     = (m_Selected == null ?
		m_Scene->b3GetBBoxHead() :
		m_Scene->b3FindBBoxHead(m_Selected));

	if (CB3SelectLoadArcon::b3Select(result))
	{
		try
		{
			m_BBox = b3BBox::b3ReadTGF(result);
			if (m_BBox != null)
			{
				b3Initialize();
				m_PrepareGeometry         = true;
				m_PrepareChangedStructure = false;
			}
		}
		catch(b3FileException &f)
		{
			b3PrintF(B3LOG_NORMAL,"I/O ERROR: reading ArCon object from file %s (code: %d)\n",
				(const char *)result,f.b3GetError());
			B3_MSG_ERROR(f);
		}
		catch(b3WorldException &w)
		{
			b3PrintF(B3LOG_NORMAL,"ERROR: reading ArCon object from file %s (code: %d)\n",
				(const char *)result,w.b3GetError());
			B3_MSG_ERROR(w);
		}
	}
}

void b3OpObjectLoadTgf::b3Delete()
{
	if (!b3IsDone() && (m_BBox != null))
	{
		delete m_BBox;
	}
}

void b3OpObjectLoadTgf::b3Do()
{
	m_Base->b3Insert(m_Selected,m_BBox);
	m_Scene->b3Recount();
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
	m_PrepareChangedStructure = true;
}

void b3OpObjectLoadTgf::b3Undo()
{
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_Base->b3Remove(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpObjectLoadTgf::b3Redo()
{
	m_Base->b3Insert(m_Selected,m_BBox);
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
			cloned->b3PrepareBBox(scene->b3GetPrepareInfo(), true);
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
