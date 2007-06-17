/*
**
**	$Filename:	b3UndoCamera.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
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

#ifndef B3UNDOCAMERA_H
#define B3UNDOCAMERA_H

#include "AppLines.h"
#include "MainFrm.h"
#include "b3Undo.h"

class b3OpCamera : public b3Operation
{
protected:
	CMainFrame   *m_Main;
	b3Scene      *m_Scene;
	b3CameraPart *m_Camera;

protected:
	inline b3OpCamera(b3Scene *scene,b3CameraPart *camera)
	{
		m_Scene  = scene;
		m_Camera = camera;
		m_Main   = CB3GetMainFrame();
	}
};

class b3OpCameraCreate : public b3OpCamera
{
	b3CameraPart *m_Selected;

public:
	b3OpCameraCreate(b3Scene *scene,b3CameraPart *camera);
	
protected:
	void b3Undo();
	void b3Redo();
	void b3Prepare(CAppRenderDoc *pDoc);
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_CAMERA_CREATE;
	}
};

class b3OpCameraDelete : public b3OpCamera
{
	b3CameraPart *m_Selected;

public:
	           b3OpCameraDelete(b3Scene *scene,b3CameraPart *camera);
protected:
	void b3Undo();
	void b3Redo();
	void b3Prepare(CAppRenderDoc *pDoc);
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_CAMERA_DELETE;
	}
};

class b3OpCameraOrientate : public b3OpCamera
{
	b3CameraPart   *m_Prev;      // previous camera dimensions
	b3CameraPart   *m_Computed;  // computed camera dimensions

public:
	b3OpCameraOrientate(
		b3Scene      *scene,
		b3CameraPart *camera,
		b3_vector    *fulcrum);
	
protected:
	void b3Delete();
	void b3Undo();
	void b3Redo();
	void b3Prepare(CAppRenderDoc *pDoc);

	inline int  b3GetId()
	{
		return IDS_OP_CAMERA_ORIENTATE;
	}

private:
	void b3Copy(const b3CameraPart *src);
};

class b3OpCameraEnable : public b3OpCamera
{
	b3_bool       m_Activation;

public:
	     b3OpCameraEnable(b3Scene *scene,b3CameraPart *camera);

protected:
	void b3Prepare(CAppRenderDoc *pDoc);
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_CAMERA_ENABLE;
	}
};

#endif
