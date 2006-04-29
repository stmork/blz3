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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.15  2006/04/29 11:25:48  sm
**	- Added ocean bump to main packet.
**	- b3Prepare signature: Added further initialization information
**	  for animation preparation
**	- Added test module for ocean waves.
**	- Added module for random number generation.
**	- Adjusted material and bump sampler to reflect preparation
**	  signature change.
**	- Added OpenGL test program for ocean waves.
**	- Changed Phillips spectrum computation to be independent
**	  from time.
**	- Interpolated height field for ocean waves.
**
**	Revision 1.14  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.13  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.12  2005/01/02 19:15:25  sm
**	- Fixed signed/unsigned warnings
**	
**	Revision 1.11  2004/09/27 11:08:54  sm
**	- Added rudimental car paint material dialog.
**	
**	Revision 1.10  2004/05/19 15:35:03  sm
**	- Hope of having fixed ticket no. 13.
**	
**	Revision 1.9  2004/05/17 13:00:33  sm
**	- Fixed inverse/reverse handling of object editing.
**	- Added diverse handling vor object loading/replacing.
**	
**	Revision 1.8  2004/05/15 14:37:46  sm
**	- Added resolution combo box to scene dialog.
**	- Fixed bug no. 3
**	
**	Revision 1.7  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.6  2003/02/09 13:58:14  sm
**	- cleaned up file selection dialogs
**	
**	Revision 1.5  2003/02/05 18:42:21  sm
**	- Changed TGF to scene/bbox import
**	- Resorted some menus
**	- Added TGF import to Un*x makefile
**	
**	Revision 1.4  2003/01/26 14:11:50  sm
**	- COB support integrated into Lines III
**	
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
		m_BBox->b3SetName(dlg.m_NewName);
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
