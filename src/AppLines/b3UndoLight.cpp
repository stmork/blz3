/*
**
**	$Filename:	b3UndoLight.cpp
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for light handling
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

#include "b3UndoLight.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Undo/Redo light creation                      **
**                                                                      **
*************************************************************************/

b3OpLightCreate::b3OpLightCreate(
	b3Scene  *scene,
	b3Light  *light) : b3OpLight(scene)
{
	CDlgCreateItem  dlg;

	dlg.m_Label.LoadString(IDS_NEW_LIGHT);
	dlg.m_ItemBase   = m_Scene->b3GetLightHead();
	dlg.m_MaxNameLen = B3_BOXSTRINGLEN;
	dlg.m_Suggest    = light->b3GetName();
	if (dlg.DoModal() == IDOK)
	{
		m_Light = new b3Light(AREA_LIGHT);
		m_Prev  = light;
		m_Base  = m_Scene->b3GetLightHead();
		m_Light->b3SetName(dlg.b3GetText());
		b3Initialize();
	}
}

void b3OpLightCreate::b3Delete()
{
	if (!b3IsDone() && (m_Light != null))
	{
		delete m_Light;
	}
}

void b3OpLightCreate::b3Prepare(CAppRenderDoc *pDoc)
{
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CAppLinesDoc)));
	CAppLinesDoc *pLinesDoc = (CAppLinesDoc *)pDoc;

	// Define right light
	pLinesDoc->m_Light = b3IsDone() ? m_Light : m_Prev;
	CB3GetMainFrame()->b3UpdateLightBox(m_Scene,pLinesDoc->m_Light);

	// Set document to right state
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,B3_UPDATE_LIGHT);
}

void b3OpLightCreate::b3Undo()
{
	m_Base->b3Remove(m_Light);
}

void b3OpLightCreate::b3Redo()
{
	m_Base->b3Append(m_Light);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo light deletion                      **
**                                                                      **
*************************************************************************/

b3OpLightDelete::b3OpLightDelete(
	b3Scene *scene,
	b3Light *light) : b3OpLight(scene)
{
	if (AfxMessageBox(IDS_ASK_DELETE_LIGHT,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		// Select new light
		m_Base = m_Scene->b3GetLightHead();
		m_Light = light;
		m_Prev  = m_Light->Prev;

		// Determine new light
		m_Select = (b3Light *)m_Light->Prev;
		if (m_Select == null)
		{
			m_Select = (b3Light *)m_Light->Succ;
		}
		b3Initialize();
	}
}

void b3OpLightDelete::b3Delete()
{
	if (b3IsDone() && (m_Light != null))
	{
		delete m_Light;
	}
}

void b3OpLightDelete::b3Prepare(CAppRenderDoc *pDoc)
{
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CAppLinesDoc)));
	CAppLinesDoc *pLinesDoc = (CAppLinesDoc *)pDoc;

	// Define right light
	pLinesDoc->m_Light = b3IsDone() ? m_Select : m_Light;
	CB3GetMainFrame()->b3UpdateLightBox(m_Scene,pLinesDoc->m_Light);

	// Set document to right state
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,B3_UPDATE_LIGHT);
}

void b3OpLightDelete::b3Undo()
{
	m_Base->b3Insert(m_Prev,m_Light);
}

void b3OpLightDelete::b3Redo()
{
	m_Base->b3Remove(m_Light);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo light deletion                      **
**                                                                      **
*************************************************************************/

b3OpLightMode::b3OpLightMode(b3_bool *ptr)
{
	m_FlagPtr =  ptr;
	m_Mode    = *ptr;
	b3Initialize();
}

void b3OpLightMode::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,B3_UPDATE_LIGHT);
}

void b3OpLightMode::b3Undo()
{
	*m_FlagPtr =  m_Mode;
}

void b3OpLightMode::b3Redo()
{
	*m_FlagPtr = !m_Mode;
}
