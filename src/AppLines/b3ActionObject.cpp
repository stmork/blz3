/*
**
**      $Filename:      b3ActionObject.cpp $
**      $Release:       Dortmund 2002 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - What to do on mouse input (object action)
**
**      (C) Copyright 2002  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3Action.h"
#include "AppLines.h"
#include "MainFrm.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2002/01/16 16:17:13  sm
**	- Introducing object edit painting and acting.
**
**
*/

/*************************************************************************
**                                                                      **
**                        CB3ActionShapeMove implementation             **
**                                                                      **
*************************************************************************/

CB3ActionShapeMove::CB3ActionShapeMove(CAppObjectView *window) :
	CB3ShapeAction(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionShapeRotatePoint implementation      **
**                                                                      **
*************************************************************************/

CB3ActionShapeRotatePoint::CB3ActionShapeRotatePoint(CAppObjectView *window) :
	CB3ShapeAction(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionShapeRotateAxis implementation       **
**                                                                      **
*************************************************************************/

CB3ActionShapeRotateAxis::CB3ActionShapeRotateAxis(CAppObjectView *window) :
	CB3ShapeAction(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionShapeScale implementation            **
**                                                                      **
*************************************************************************/

CB3ActionShapeScale::CB3ActionShapeScale(CAppObjectView *window) :
	CB3ShapeAction(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionShapeMirrorPoint implementation      **
**                                                                      **
*************************************************************************/

CB3ActionShapeMirrorPoint::CB3ActionShapeMirrorPoint(CAppObjectView *window) :
	CB3ShapeAction(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionShapeMirrorAxis implementation       **
**                                                                      **
*************************************************************************/

CB3ActionShapeMirrorAxis::CB3ActionShapeMirrorAxis(CAppObjectView *window) :
	CB3ShapeAction(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionShapeMirrorArea implementation       **
**                                                                      **
*************************************************************************/

CB3ActionShapeMirrorArea::CB3ActionShapeMirrorArea(CAppObjectView *window) :
	CB3ShapeAction(window)
{
}
