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
**	Revision 1.4  2003/01/18 14:13:49  sm
**	- Added move/rotate stepper operations
**	- Cleaned up resource IDs
**
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
**                        Undo/Redo stepper action                      **
**                                                                      **
*************************************************************************/

b3OpObjectTransformation::b3OpObjectTransformation(b3Scene *scene)
{
	m_Scene = scene;
	m_Scene->b3CollectActiveBBoxes(&m_Active,true);
}

void b3OpObjectTransformation::b3Reactivate()
{
	m_Scene->b3Activate(false);
	for (int i = 0;i < m_Active.b3GetCount();i++)
	{
		m_Active[i]->b3Activate(true,false);
	}
}

void b3OpObjectTransformation::b3Undo()
{
	b3Reactivate();
	m_Scene->b3Transform(&m_UndoAction);
}

void b3OpObjectTransformation::b3Redo()
{
	b3Reactivate();
	m_Scene->b3Transform(&m_RedoAction);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object action                       **
**                                                                      **
*************************************************************************/

b3OpObjectAction::b3OpObjectAction(b3Scene *scene,b3_matrix *action) : b3OpObjectTransformation(scene)
{
	m_RedoAction  = *action;
	b3Initialize(b3MatrixInv(action,&m_UndoAction) != null);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo move stepper action                 **
**                                                                      **
*************************************************************************/

b3OpMove::b3OpMove(
	b3Scene   *scene,
	b3_vector *stepper) : b3OpObjectTransformation(scene)
{
	b3MatrixMove(null,&m_RedoAction,stepper);
	b3Initialize(b3MatrixInv(&m_RedoAction,&m_UndoAction) != null);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo rotate stepper action               **
**                                                                      **
*************************************************************************/

b3OpRotate::b3OpRotate(
	b3Scene *scene,
	b3_line *axis,
	b3_f64   angle) : b3OpObjectTransformation(scene)
{
	b3MatrixRotVec(null,&m_RedoAction,axis,angle * M_PI / 180);
	b3Initialize(b3MatrixInv(&m_RedoAction,&m_UndoAction) != null);
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
