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

#include "AppLinesInclude.h"

#include "b3UndoCamera.h"
#include "DlgCreateItem.h"

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera creation                     **
**                                                                      **
*************************************************************************/

b3OpCameraCreate::b3OpCameraCreate(
	b3Scene      *scene,
	b3CameraPart *selected) : b3OpCamera(scene,selected)
{
	CDlgCreateItem  dlg;

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
		m_Camera->b3SetName(dlg.b3GetText());
		b3Initialize();
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
	m_Main->b3UpdateCameraBox(m_Scene,m_Selected);
}

void b3OpCameraCreate::b3Redo()
{
	m_Scene->b3GetSpecialHead()->b3Append(m_Camera);
	m_Scene->b3SetCamera(m_Camera,true);
	m_Main->b3UpdateCameraBox(m_Scene,m_Camera);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera deletion                     **
**                                                                      **
*************************************************************************/

b3OpCameraDelete::b3OpCameraDelete(
	b3Scene      *scene,
	b3CameraPart *camera) : b3OpCamera(scene,camera)
{
	if (AfxMessageBox(IDS_ASK_DELETE_CAMERA,MB_ICONQUESTION|MB_YESNO) == IDYES)
	{
		m_Selected = m_Scene->b3GetNextCamera(m_Camera);
		if (m_Selected == null)
		{
			m_Selected = m_Scene->b3GetFirstCamera();
			while ((m_Selected == m_Camera) && (m_Selected != null))
			{
				m_Selected = m_Scene->b3GetNextCamera(m_Selected);
			}
		}
		b3Initialize();
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
	m_Main->b3UpdateCameraBox(m_Scene,m_Camera);
}

void b3OpCameraDelete::b3Redo()
{
	m_Scene->b3GetSpecialHead()->b3Remove(m_Camera);
	m_Scene->b3SetCamera(m_Selected,true);
	m_Main->b3UpdateCameraBox(m_Scene,m_Selected);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera activation                   **
**                                                                      **
*************************************************************************/

b3OpCameraEnable::b3OpCameraEnable(
	b3Scene      *scene,
	b3CameraPart *camera) : b3OpCamera(scene,camera)
{
	m_Activation = camera->b3IsActive();
	b3Initialize();
}

void b3OpCameraEnable::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->SetModifiedFlag();
	m_Main->b3UpdateCameraBox(m_Scene,m_Camera);
}


void b3OpCameraEnable::b3Undo()
{
	m_Camera->b3Activate(m_Activation);
}

void b3OpCameraEnable::b3Redo()
{
	m_Camera->b3Activate(!m_Activation);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo camera orientation                  **
**                                                                      **
*************************************************************************/

b3OpCameraOrientate::b3OpCameraOrientate(
	b3Scene      *scene,
	b3CameraPart *camera,
	b3_vector    *fulcrum) : b3OpCamera(scene,camera)
{
	m_Prev       = (b3CameraPart *)b3World::b3Clone(camera);
	m_Computed   = (b3CameraPart *)b3World::b3Clone(camera);
	m_Computed->b3Orientate(
		&m_Camera->m_EyePoint,
		fulcrum,
		b3Vector::b3Distance(&m_Camera->m_ViewPoint,&m_Camera->m_EyePoint),
		b3Vector::b3Length(&m_Camera->m_Width),
		b3Vector::b3Length(&m_Camera->m_Height));
	b3Initialize();
}

void b3OpCameraOrientate::b3Delete()
{
	delete m_Prev;
	delete m_Computed;
}

void b3OpCameraOrientate::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->SetModifiedFlag();
	pDoc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
	m_Main->b3UpdateCameraBox(m_Scene,m_Camera);
}


void b3OpCameraOrientate::b3Undo()
{
	b3Copy(m_Prev);
}

void b3OpCameraOrientate::b3Redo()
{
	b3Copy(m_Computed);
}

void b3OpCameraOrientate::b3Copy(const b3CameraPart *src)
{
	m_Camera->m_EyePoint  = src->m_EyePoint;
	m_Camera->m_ViewPoint = src->m_ViewPoint;
	m_Camera->m_Width     = src->m_Width;
	m_Camera->m_Height    = src->m_Height;
}
