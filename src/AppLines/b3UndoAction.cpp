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

#include "AppLinesInclude.h"

#include "b3UndoAction.h"
#include "b3Action.h"

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
	m_Scene->b3Transform(&m_UndoAction, false);
}

void b3OpObjectTransformation::b3Redo()
{
	b3Reactivate();
	m_Scene->b3Transform(&m_RedoAction, false);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object action                       **
**                                                                      **
*************************************************************************/

b3OpObjectAction::b3OpObjectAction(b3Scene *scene,b3_matrix *action) : b3OpObjectTransformation(scene)
{
	m_RedoAction  = *action;
	b3Initialize(b3Matrix::b3Inverse(action,&m_UndoAction) != null);
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
	b3Matrix::b3Move(null,&m_RedoAction,stepper);
	b3Initialize(b3Matrix::b3Inverse(&m_RedoAction,&m_UndoAction) != null);
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
	b3Matrix::b3RotateVector(null,&m_RedoAction,axis,angle * M_PI / 180);
	b3Initialize(b3Matrix::b3Inverse(&m_RedoAction,&m_UndoAction) != null);
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
	b3Initialize(b3Matrix::b3Inverse(action,&m_UndoAction) != null);
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
