/*
**
**	$Filename:	b3Action.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Actione what to do with mouse input
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3ACTION_H
#define B3ACTION_H

#include "blz3/b3Config.h"
#include "blz3/base/b3Matrix.h"

#include "AppRenderView.h"
#include "AppLinesView.h"
#include "AppObjectView.h"

enum b3_mouse_button
{
	B3_MB_UP,
	B3_MB_LEFT,
	B3_MB_MIDDLE,
	B3_MB_RIGHT
};

/*************************************************************************
**                                                                      **
**                        Some base classes                             **
**                                                                      **
*************************************************************************/

class CB3Action
{
protected:
	CAppRenderView  *m_View;
	CAppRenderDoc   *m_Doc;
	b3_mouse_button  m_Button;
	b3_coord         m_xStart,m_xLast;
	b3_coord         m_yStart,m_yLast;
	b3_matrix        m_Transformation;
	b3_bool          m_PressedShift;
	b3_bool          m_PressedCtrl;

public:
	             CB3Action(CAppRenderView *window);
	        void b3DispatchMouseMove  (b3_coord x,b3_coord y);
	        void b3DispatchLButtonDown(b3_coord x,b3_coord y,b3_u32 flags);
	        void b3DispatchLButtonUp  (b3_coord x,b3_coord y);
			void b3DispatchMButtonDown(b3_coord x,b3_coord y,b3_u32 flags);
	        void b3DispatchMButtonUp  (b3_coord x,b3_coord y);
			void b3DispatchRButtonDown(b3_coord x,b3_coord y,b3_u32 flags);
	        void b3DispatchRButtonUp  (b3_coord x,b3_coord y);

			b3_matrix *b3GetTransformation();
	virtual b3_bool    b3IsObject();
	virtual b3_bool    b3IsCamera();

protected:
	        void b3GetRelCoord(b3_coord x,b3_coord y,b3_f64 &xRel,b3_f64 &yRel);
	virtual void b3MouseMove(b3_coord x,b3_coord y);

	virtual void b3LDown(b3_coord x,b3_coord y);
	virtual void b3LMove(b3_coord x,b3_coord y);
	virtual void b3LUp(b3_coord x,b3_coord y);

	virtual void b3MDown(b3_coord x,b3_coord y);
	virtual void b3MMove(b3_coord x,b3_coord y);
	virtual void b3MUp(b3_coord x,b3_coord y);

	virtual void b3RDown(b3_coord x,b3_coord y);
	virtual void b3RMove(b3_coord x,b3_coord y);
	virtual void b3RUp(b3_coord x,b3_coord y);
};

class CB3MoveAction : public CB3Action
{
protected:
	b3_f64         m_xRelStart,m_yRelStart;
	b3_vector      m_xDir,m_yDir,m_zDir;
	b3_vector      m_StartPoint;
	b3_vector      m_LastPoint;
	b3_vector      m_LastDiff;

protected:
	             CB3MoveAction(CAppRenderView *window);
	virtual void b3Transform(b3_matrix *transformation);

	virtual void b3LDown(b3_coord x,b3_coord y);
	virtual void b3LMove(b3_coord x,b3_coord y);
	virtual void b3LUp(b3_coord x,b3_coord y);
	        
	virtual void b3RDown(b3_coord x,b3_coord y);
	virtual void b3RMove(b3_coord x,b3_coord y);
	virtual void b3RUp(b3_coord x,b3_coord y);

private:
	        void b3InitTranslation(b3_f64 xRel,b3_f64 yRel);
};

class CB3CameraRotateAction : public CB3Action
{
protected:
	LPARAM         m_UpdateHint;
	b3CameraPart  *m_Camera;
	b3_vector     *m_Center;
	b3_vector      m_StartPoint;
	b3_f64         m_StartAngle;
	b3_f64         m_xRelStart;
	b3_f64         m_yRelStart;
	b3_f64         m_xLastAngle;
	b3_f64         m_yLastAngle;
	b3_line        m_Axis;
	b3_line        m_UpDown;
	b3_f64         m_Sign;

public:
	             CB3CameraRotateAction(CAppRenderView *window);
	virtual void b3LDown(b3_coord x,b3_coord y);
	virtual void b3LMove(b3_coord x,b3_coord y);
	virtual void b3LUp(b3_coord x,b3_coord y);
};

class CB3ShapeAction : public CB3Action
{
	b3_f64          m_xStartAngle;
	b3_f64          m_yStartAngle;

protected:
	b3_f64          m_xRelStart;
	b3_f64          m_yRelStart;
	CAppObjectView *m_ObjectView;

public:
	             CB3ShapeAction(CAppObjectView *window);
	virtual void b3LDown(b3_coord x,b3_coord y);
	virtual void b3LMove(b3_coord x,b3_coord y);
	virtual void b3LUp(b3_coord x,b3_coord y);
};

/*************************************************************************
**                                                                      **
**                        Magnify selection action                      **
**                                                                      **
*************************************************************************/

class CB3ActionMagnify : public CB3Action
{
public:
	CB3ActionMagnify(CAppRenderView *window);

	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);
};

/*************************************************************************
**                                                                      **
**                        Object actions                                **
**                                                                      **
*************************************************************************/

class CB3ActionObjectSelect : public CB3Action
{
	CAppLinesDoc *m_LinesDoc;

public:
	CB3ActionObjectSelect(CAppLinesView *window);

	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);

	void b3RMove(b3_coord x,b3_coord y);
	void b3RUp(b3_coord x,b3_coord y);

private:
	b3_bool b3IsPointSelection(b3_coord x,b3_coord y);
	void    b3ComputeSelectionDir(b3_coord x,b3_coord y,b3_line *selection_dir);
};

class CB3ActionObjectMove : public CB3MoveAction
{
	CAppLinesView *m_LinesView;

public:
	CB3ActionObjectMove(CAppLinesView *window);

protected:
	void b3Transform(b3_matrix *transformation);

	inline b3_bool b3IsObject()
	{
		return true;
	}
};

class CB3ActionObjectRotate : public CB3Action
{
	CAppLinesView *m_LinesView;
	b3_vector     *m_Center;
	b3_vector      m_StartPoint;
	b3_f64         m_StartAngle;
	b3_f64         m_xRelStart;
	b3_f64         m_LastAngle;
	b3_line        m_Axis;
public:
	CB3ActionObjectRotate(CAppLinesView *window);

	void b3LDown(b3_coord x,b3_coord y);
	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);

	inline b3_bool b3IsObject()
	{
		return true;
	}
};

class CB3ActionObjectScale : public CB3Action
{
	CAppLinesView *m_LinesView;
	b3_vector     *m_Center;
	b3_vector      m_StartPoint;
	b3_vector      m_StartDiff;
	b3_f64         m_LastScale;
	b3_res         m_ySize;

public:
	CB3ActionObjectScale(CAppLinesView *window);

	void b3LDown(b3_coord x,b3_coord y);
	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);

	inline b3_bool b3IsObject()
	{
		return true;
	}

private:
	b3_bool b3ComputeScaling(b3_coord x,b3_coord y,b3_vector &scale);
};

/*************************************************************************
**                                                                      **
**                        Camera actions                                **
**                                                                      **
*************************************************************************/

class CB3ActionCameraMove : public CB3MoveAction
{
	b3CameraPart *m_Camera;

public:
	     CB3ActionCameraMove(CAppRenderView *window);

	void b3Transform(b3_matrix *transformation);

	void b3LDown(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);

	void b3RDown(b3_coord x,b3_coord y);
	void b3RUp(b3_coord x,b3_coord y);

	inline b3_bool b3IsCamera()
	{
		return true;
	}
};

class CB3ActionCameraTurn : public CB3CameraRotateAction
{
public:
	CB3ActionCameraTurn(CAppRenderView *window);

	void b3LDown(b3_coord x,b3_coord y);

	inline b3_bool b3IsCamera()
	{
		return true;
	}
};

class CB3ActionCameraRotate : public CB3CameraRotateAction
{
public:
	CB3ActionCameraRotate(CAppRenderView *window);

	void b3LDown(b3_coord x,b3_coord y);

	inline b3_bool b3IsCamera()
	{
		return true;
	}
};

class CB3ActionCameraView : public CB3Action
{
	b3CameraPart  *m_Camera;
	b3_line        m_Axis;
	b3_f64         m_xRelStart;
	b3_f64         m_LastScale;
	b3_f64         m_LastAngle;
	b3_f64         m_Distance;
	b3_res         m_ySize;

public:
	     CB3ActionCameraView(CAppRenderView *window);

	void b3LDown(b3_coord x,b3_coord y);
	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);
	void b3RDown(b3_coord x,b3_coord y);
	void b3RMove(b3_coord x,b3_coord y);
	void b3RUp(b3_coord x,b3_coord y);

	inline b3_bool b3IsCamera()
	{
		return true;
	}
};

/*************************************************************************
**                                                                      **
**                        Light actions                                 **
**                                                                      **
*************************************************************************/

class CB3ActionLightTurn : public CB3CameraRotateAction
{
	CAppLinesDoc *m_LinesDoc;

public:
	CB3ActionLightTurn(CAppLinesView *window);

	void b3LDown(b3_coord x,b3_coord y);
};

/*************************************************************************
**                                                                      **
**                        Shape actions                                 **
**                                                                      **
*************************************************************************/

class CB3ActionShapeMove : public CB3ShapeAction
{
public:
	CB3ActionShapeMove(CAppObjectView *window);
};

class CB3ActionShapeRotatePoint : public CB3ShapeAction
{
public:
	CB3ActionShapeRotatePoint(CAppObjectView *window);
};

class CB3ActionShapeRotateAxis : public CB3ShapeAction
{
public:
	CB3ActionShapeRotateAxis(CAppObjectView *window);
};

class CB3ActionShapeScale : public CB3ShapeAction
{
public:
	CB3ActionShapeScale(CAppObjectView *window);
};

class CB3ActionShapeMirrorPoint : public CB3ShapeAction
{
public:
	CB3ActionShapeMirrorPoint(CAppObjectView *window);
};

class CB3ActionShapeMirrorAxis : public CB3ShapeAction
{
public:
	CB3ActionShapeMirrorAxis(CAppObjectView *window);
};

class CB3ActionShapeMirrorArea : public CB3ShapeAction
{
public:
	CB3ActionShapeMirrorArea(CAppObjectView *window);
};

#endif
