/*
**
**	$Filename:	b3UndoCamera.cpp $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for camera actions
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

#include "b3UndoCamera.h"
#include "DlgCreateItem.h"
#include "MainFrm.h"
#include "AppRenderView.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2003/01/14 19:07:35  sm
**	- Added some camera undo/redo actions.
**
**	Revision 1.1  2003/01/12 19:21:37  sm
**	- Some other undo/redo actions added (camera etc.)
**	
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera creation                     **
**                                                                      **
*************************************************************************/

b3OpCameraCreate::b3OpCameraCreate(
	b3Scene      *scene,
	b3CameraPart *selected) : b3OpCamera(selected)
{
	CDlgCreateItem  dlg;

	m_Scene = scene;
	dlg.m_Label.LoadString(IDS_NEW_CAMERA);
	dlg.m_ItemBase   = m_Scene->b3GetSpecialHead();
	dlg.m_ClassType  = CAMERA;
	dlg.m_MaxNameLen = B3_CAMERANAMELEN;
	dlg.m_Suggest    = selected->b3GetName();
	if (dlg.DoModal() == IDOK)
	{
		m_Selected = selected;
		m_Camera   = new b3CameraPart(CAMERA);
		m_Camera->m_EyePoint  = selected->m_EyePoint;
		m_Camera->m_ViewPoint = selected->m_ViewPoint;
		m_Camera->m_Width     = selected->m_Width;
		m_Camera->m_Height    = selected->m_Height;
		m_Camera->m_Flags     = CAMERA_ACTIVE;
		strcpy (m_Camera->b3GetName(),dlg.m_NewName);

	}
}

void b3OpCameraCreate::b3Delete()
{
	if(!b3IsDone() && (m_Camera != null))
	{
		delete m_Camera;
	}
}

void b3OpCameraCreate::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
}

void b3OpCameraCreate::b3Undo()
{
	m_Scene->b3GetSpecialHead()->b3Remove(m_Camera);
	m_Scene->b3SetCamera(m_Selected,true);
	CB3GetMainFrame()->b3UpdateCameraBox(m_Scene,m_Selected);
}

void b3OpCameraCreate::b3Redo()
{
	m_Scene->b3GetSpecialHead()->b3Append(m_Camera);
	m_Scene->b3SetCamera(m_Camera,true);
	CB3GetMainFrame()->b3UpdateCameraBox(m_Scene,m_Camera);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera deletion                     **
**                                                                      **
*************************************************************************/

b3OpCameraDelete::b3OpCameraDelete(
	b3Scene      *scene,
	b3CameraPart *camera) : b3OpCamera(camera)
{
	if (AfxMessageBox(IDS_ASK_DELETE_CAMERA,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		m_Scene    = scene;
		m_Selected = m_Scene->b3GetNextCamera(m_Camera);
		if (m_Selected == null)
		{
			m_Selected = m_Scene->b3GetCamera();
		}
	}
}

void b3OpCameraDelete::b3Delete()
{
	if(b3IsDone() && (m_Camera != null))
	{
		delete m_Camera;
	}
}

void b3OpCameraDelete::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
}

void b3OpCameraDelete::b3Undo()
{
	m_Scene->b3GetSpecialHead()->b3Append(m_Camera);
	m_Scene->b3SetCamera(m_Camera,true);
	CB3GetMainFrame()->b3UpdateCameraBox(m_Scene,m_Camera);
}

void b3OpCameraDelete::b3Redo()
{
	m_Scene->b3GetSpecialHead()->b3Remove(m_Camera);
	m_Scene->b3SetCamera(m_Selected,true);
	CB3GetMainFrame()->b3UpdateCameraBox(m_Scene,m_Selected);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera activation                   **
**                                                                      **
*************************************************************************/

b3OpCameraEnable::b3OpCameraEnable(b3CameraPart *camera) : b3OpCamera(camera)
{
	m_Activation = camera->b3IsActive();
	b3Initialize();
}

void b3OpCameraEnable::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->SetModifiedFlag();
}


void b3OpCameraEnable::b3Undo()
{
	m_Camera->b3Activate(m_Activation);
}

void b3OpCameraEnable::b3Redo()
{
	m_Camera->b3Activate(!m_Activation);
}
