/*
**
**	$Filename:	b3UndoAction.cpp
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for mouse actions
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

#include "b3UndoAction.h"
#include "b3Action.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2003/01/11 17:16:15  sm
**	- Object handling with undo/redo
**	- Light handling with undo/redo
**
**	Revision 1.2  2003/01/07 16:14:38  sm
**	- Lines III: object editing didn't prepared any more. Fixed.
**	- Some prepare optimizations.
**	
**	Revision 1.1  2003/01/05 16:13:24  sm
**	- First undo/redo implementations
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo object action                       **
**                                                                      **
*************************************************************************/

b3OpObjectAction::b3OpObjectAction(b3Scene *scene,b3_matrix *action)
{
	m_Scene       = scene;
	m_Scene->b3CollectActiveBBoxes(&m_Active,true);
	m_RedoAction  = *action;
	b3Initialize(b3MatrixInv(action,&m_UndoAction) != null);
}

void b3OpObjectAction::b3Do()
{
	// Do nothing after initialization
}

void b3OpObjectAction::b3Undo()
{
	m_Scene->b3Activate(false);
	for (int i = 0;i < m_Active.b3GetCount();i++)
	{
		m_Active[i]->b3Activate(true,false);
	}
	m_Scene->b3Transform(&m_UndoAction);
}

void b3OpObjectAction::b3Redo()
{
	m_Scene->b3Activate(false);
	for (int i = 0;i < m_Active.b3GetCount();i++)
	{
		m_Active[i]->b3Activate(true,false);
	}
	m_Scene->b3Transform(&m_RedoAction);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera action                       **
**                                                                      **
*************************************************************************/

b3OpCameraAction::b3OpCameraAction(b3CameraPart *camera,b3_matrix *action)
{
	m_Camera      = camera;
	m_RedoAction  = *action;
	b3Initialize(b3MatrixInv(action,&m_UndoAction) != null);
}

void b3OpCameraAction::b3Do()
{
	// Do nothing after initialization
}

void b3OpCameraAction::b3Undo()
{
	m_Camera->b3Transform(&m_UndoAction);
}

void b3OpCameraAction::b3Redo()
{
	m_Camera->b3Transform(&m_RedoAction);
}

void b3OpCameraAction::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
}
