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

#include "AppLinesView.h"

typedef enum
{
	B3_MB_UP,
	B3_MB_LEFT,
	B3_MB_MIDDLE,
	B3_MB_RIGHT
} b3_mouse_button;

class CB3Action
{
protected:
	CAppLinesView   *m_View;
	CAppLinesDoc    *m_Doc;
	b3_mouse_button  m_Button;
	b3_coord         m_xStart,m_xLast;
	b3_coord         m_yStart,m_yLast;
	b3_matrix        m_Transformation;

public:
	             CB3Action(CAppLinesView *window);
	        void b3DispatchMouseMove  (b3_coord x,b3_coord y);
	        void b3DispatchLButtonDown(b3_coord x,b3_coord y);
	        void b3DispatchLButtonUp  (b3_coord x,b3_coord y);
			void b3DispatchMButtonDown(b3_coord x,b3_coord y);
	        void b3DispatchMButtonUp  (b3_coord x,b3_coord y);
			void b3DispatchRButtonDown(b3_coord x,b3_coord y);
	        void b3DispatchRButtonUp  (b3_coord x,b3_coord y);
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

class CB3ActionMagnify : public CB3Action
{
public:
	CB3ActionMagnify(CAppLinesView *window);

	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);
};

class CB3ActionObjectSelect : public CB3Action
{
public:
	CB3ActionObjectSelect(CAppLinesView *window);

	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);

	void b3RMove(b3_coord x,b3_coord y);
	void b3RUp(b3_coord x,b3_coord y);
};

class CB3ActionObjectMove : public CB3Action
{
	b3_vector m_StartPoint;
public:
	CB3ActionObjectMove(CAppLinesView *window);

	void b3LDown(b3_coord x,b3_coord y);
	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);
};

class CB3ActionObjectRotate : public CB3Action
{
public:
	CB3ActionObjectRotate(CAppLinesView *window);
};

class CB3ActionObjectScale : public CB3Action
{
	b3_vector *m_Center;
	b3_vector  m_StartPoint;
	b3_vector  m_StartDiff;
public:
	CB3ActionObjectScale(CAppLinesView *window);

	void b3LDown(b3_coord x,b3_coord y);
	void b3LMove(b3_coord x,b3_coord y);
	void b3LUp(b3_coord x,b3_coord y);
};

class CB3ActionCameraMove : public CB3Action
{
public:
	CB3ActionCameraMove(CAppLinesView *window);
};

class CB3ActionCameraTurn : public CB3Action
{
public:
	CB3ActionCameraTurn(CAppLinesView *window);
};

class CB3ActionCameraRotate : public CB3Action
{
public:
	CB3ActionCameraRotate(CAppLinesView *window);
};

class CB3ActionCameraView : public CB3Action
{
public:
	CB3ActionCameraView(CAppLinesView *window);
};

class CB3ActionLightTurn : public CB3Action
{
public:
	CB3ActionLightTurn(CAppLinesView *window);
};

#endif
