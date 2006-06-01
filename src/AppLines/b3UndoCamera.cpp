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
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2006/06/01 14:17:24  smork
**	- Added frame work controls for simpler plugin generation:
**	  o CB3Checkbox
**	  o CB3TextEdit
**
**	Revision 1.7  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.6  2005/01/23 19:54:06  sm
**	- Experimented with OpenGL settings for Linux Wine but there
**	  is no solution for Wine/Windows MDI applications to use OpenGL.
**	- Optimized precompiled header.
**	
**	Revision 1.5  2004/05/15 14:37:46  sm
**	- Added resolution combo box to scene dialog.
**	- Fixed bug no. 3
**	
**	Revision 1.4  2003/08/31 10:44:07  sm
**	- Further buffer overflow avoidments.
**	
**	Revision 1.3  2003/01/15 16:23:53  sm
**	- Some other camera undo/redo operations added.
**	- Fixed some undo(redo operations.
**	
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
