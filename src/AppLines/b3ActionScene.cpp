/*
**
**      $Filename:      b3ActionScene.cpp $
**      $Release:       Dortmund 2002 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - What to do on mouse input (scene action)
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
**	Revision 1.2  2002/02/13 16:13:08  sm
**	- Created spotlight view
**	- Changed camera properties dialog to reflect scene units
**	  on example camera settings.
**
**	Revision 1.1  2002/01/16 16:17:13  sm
**	- Introducing object edit painting and acting.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3ActionMagnify implementation               **
**                                                                      **
*************************************************************************/

CB3ActionMagnify::CB3ActionMagnify(CAppRenderView *window) :
	CB3Action(window)
{
}

void CB3ActionMagnify::b3LMove(b3_coord x,b3_coord y)
{
	m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
	m_View->b3DrawRect(m_xStart,m_yStart,x,y);
}

void CB3ActionMagnify::b3LUp(b3_coord x,b3_coord y)
{
	b3_f64 x1,y1,x2,y2;

	b3GetRelCoord(m_xStart,m_yStart,x1,y1);
	b3GetRelCoord(x,y,x2,y2);
	m_View->m_RenderView.b3Select(x1,y1,x2,y2);
	m_View->b3Update(B3_UPDATE_VIEW);
	m_View->b3UnsetMagnification();
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectSelect implementation          **
**                                                                      **
*************************************************************************/

CB3ActionObjectSelect::CB3ActionObjectSelect(CAppLinesView *window) :
	CB3Action(window)
{
}

void CB3ActionObjectSelect::b3LMove(b3_coord x,b3_coord y)
{
	m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
	m_View->b3DrawRect(m_xStart,m_yStart,x,y);
}

void CB3ActionObjectSelect::b3LUp(b3_coord x,b3_coord y)
{
	m_View->b3Update(B3_UPDATE_VIEW);
}

void CB3ActionObjectSelect::b3RMove(b3_coord x,b3_coord y)
{
	m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
	m_View->b3DrawRect(m_xStart,m_yStart,x,y);
}

void CB3ActionObjectSelect::b3RUp(b3_coord x,b3_coord y)
{
	m_View->b3Update(B3_UPDATE_VIEW);
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectMove implementation            **
**                                                                      **
*************************************************************************/

CB3ActionObjectMove::CB3ActionObjectMove(CAppLinesView *window) :
	CB3MoveAction(window)
{
	m_LinesView = window;
}

void CB3ActionObjectMove::b3Transform(b3_matrix *transformation)
{
	CAppLinesDoc *pDoc = m_LinesView->GetDocument();
	
	pDoc->m_Scene->b3Transform(transformation);
	pDoc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectRotate implementation          **
**                                                                      **
*************************************************************************/

CB3ActionObjectRotate::CB3ActionObjectRotate(CAppLinesView *window) :
	CB3Action(window)
{
	m_LinesView = window;
}

void CB3ActionObjectRotate::b3LDown(b3_coord x,b3_coord y)
{
	b3_f64 xRel,yRel;

	m_Center     = m_Doc->b3GetFulcrum();
	m_StartPoint = *m_Center;
	m_Axis.pos   = *m_Center;
	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_View->m_RenderView.b3GetViewDirection(&m_Axis.dir);
		m_View->m_RenderView.b3Unproject(xRel,yRel,&m_StartPoint);
	
		m_StartAngle = m_View->m_RenderView.b3GetPositionAngle(m_Center,&m_StartPoint);
		m_LastAngle  = m_StartAngle;
	}
	else
	{
		m_Axis.dir.x = 0;
		m_Axis.dir.y = 0;
		m_Axis.dir.z = 1;

		m_xRelStart = xRel;
	}
}

void CB3ActionObjectRotate::b3LMove(b3_coord x,b3_coord y)
{
	b3_vector point;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;
	b3_f64    angle;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		point = *m_Center;

		m_View->m_RenderView.b3Unproject(xRel,yRel,&point);

		angle = m_StartAngle - m_View->m_RenderView.b3GetPositionAngle(m_Center,&point);
	}
	else
	{
		angle = (xRel - m_xRelStart) * M_PI * 2;
	}

	m_Doc->m_Info->b3SnapToAngle(angle);
	if (angle != m_LastAngle)
	{
		if (b3MatrixInv(&m_Transformation,&inv))
		{
			m_LastAngle = angle;
			b3MatrixRotVec(null,&m_Transformation,&m_Axis,angle);
			b3MatrixMMul(&inv,&m_Transformation,&activity);
			m_LinesView->GetDocument()->m_Scene->b3Transform(&activity);
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

void CB3ActionObjectRotate::b3LUp(b3_coord x,b3_coord y)
{
	b3_vector point;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;
	b3_f64    angle;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		point = *m_Center;

		m_View->m_RenderView.b3Unproject(xRel,yRel,&point);

		angle = m_StartAngle - m_View->m_RenderView.b3GetPositionAngle(m_Center,&point);
	}
	else
	{
		angle = (xRel - m_xRelStart) * M_PI * 2;
	}

	m_Doc->m_Info->b3SnapToAngle(angle);
	if (b3MatrixInv(&m_Transformation,&inv))
	{
		b3MatrixRotVec(null,&m_Transformation,&m_Axis,angle);
		b3MatrixMMul(&inv,&m_Transformation,&activity);
		m_LinesView->GetDocument()->m_Scene->b3Transform(&activity);
		m_Doc->b3ComputeBounds();
		m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
	}
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectScale implementation           **
**                                                                      **
*************************************************************************/

CB3ActionObjectScale::CB3ActionObjectScale(CAppLinesView *window) :
	CB3Action(window)
{
	m_LinesView = window;
}

void CB3ActionObjectScale::b3LDown(b3_coord x,b3_coord y)
{
	b3_f64 xRel,yRel;

	m_Center = m_Doc->b3GetFulcrum();
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_StartPoint = *m_Center;

		b3GetRelCoord(x,y,xRel,yRel);
		m_View->m_RenderView.b3Unproject(xRel,yRel,&m_StartPoint);
		m_StartDiff.x = m_Center->x - m_StartPoint.x;
		m_StartDiff.y = m_Center->y - m_StartPoint.y;
		m_StartDiff.z = m_Center->z - m_StartPoint.z;
	}
}

void CB3ActionObjectScale::b3LMove(b3_coord x,b3_coord y)
{
	b3_vector diff,scale,point;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		point = *m_Center;

		b3GetRelCoord(x,y,xRel,yRel);
		m_View->m_RenderView.b3Unproject(xRel,yRel,&point);

		diff.x = m_Center->x - point.x;
		if (m_StartDiff.x == 0)
		{
			scale.x = (diff.x == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.x = diff.x / m_StartDiff.x;
		}
		
		diff.y = m_Center->y - point.y;
		if (m_StartDiff.y == 0)
		{
			scale.y = (diff.y == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.y = diff.y / m_StartDiff.y;
		}

		diff.z = m_Center->z - point.z;
		if (m_StartDiff.z == 0)
		{
			scale.z = (diff.z == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.z = diff.z / m_StartDiff.z;
		}

		if (b3MatrixInv(&m_Transformation,&inv))
		{
			b3MatrixScale(null,&m_Transformation,m_Center,&scale);
			b3MatrixMMul(&inv,&m_Transformation,&activity);
			m_LinesView->GetDocument()->m_Scene->b3Transform(&activity);
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

void CB3ActionObjectScale::b3LUp(b3_coord x,b3_coord y)
{
	b3_vector diff,scale,point;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		point = *m_Center;

		b3GetRelCoord(x,y,xRel,yRel);
		m_View->m_RenderView.b3Unproject(xRel,yRel,&point);

		diff.x = m_Center->x - point.x;
		if (m_StartDiff.x == 0)
		{
			scale.x = (diff.x == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.x = diff.x / m_StartDiff.x;
		}
		
		diff.y = m_Center->y - point.y;
		if (m_StartDiff.y == 0)
		{
			scale.y = (diff.y == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.y = diff.y / m_StartDiff.y;
		}

		diff.z = m_Center->z - point.z;
		if (m_StartDiff.z == 0)
		{
			scale.z = (diff.z == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.z = diff.z / m_StartDiff.z;
		}

		if (b3MatrixInv(&m_Transformation,&inv))
		{
			CAppLinesDoc *pDoc = m_LinesView->GetDocument();

			b3MatrixScale(null,&m_Transformation,m_Center,&scale);
			b3MatrixMMul(&inv,&m_Transformation,&activity);
			pDoc->m_Scene->b3Transform(&activity);
			pDoc->b3ComputeBounds();
			pDoc->SetModifiedFlag();
			pDoc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraMove implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraMove::CB3ActionCameraMove(CAppRenderView *window) :
	CB3MoveAction(window)
{
	m_Camera    = null;
}

void CB3ActionCameraMove::b3Transform(b3_matrix *transformation)
{
	m_Camera->b3Transform(transformation);
	m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
}

void CB3ActionCameraMove::b3LDown(b3_coord x,b3_coord y)
{
	m_Camera = m_View->m_Camera;
	CB3MoveAction::b3LDown(x,y);
}

void CB3ActionCameraMove::b3LUp(b3_coord x,b3_coord y)
{
	CB3MoveAction::b3LUp(x,y);
	m_Camera = null;
}

void CB3ActionCameraMove::b3RDown(b3_coord x,b3_coord y)
{
	CB3MoveAction::b3RDown(x,y);
	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_Camera = m_View->m_Camera;
	}
}
void CB3ActionCameraMove::b3RUp(b3_coord x,b3_coord y)
{
	CB3MoveAction::b3RUp(x,y);
	m_Camera = null;
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraTurn implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraTurn::CB3ActionCameraTurn(CAppRenderView *window) :
	CB3CameraRotateAction(window)
{
	m_Sign      = -1;
}

void CB3ActionCameraTurn::b3LDown(b3_coord x,b3_coord y)
{
	m_Camera = m_View->m_Camera;
	m_Center = &m_Camera->m_EyePoint;
	CB3CameraRotateAction::b3LDown(x,y);
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraRotate implementation          **
**                                                                      **
*************************************************************************/

CB3ActionCameraRotate::CB3ActionCameraRotate(CAppRenderView *window) :
	CB3CameraRotateAction(window)
{
	m_Sign      = -1;
}

void CB3ActionCameraRotate::b3LDown(b3_coord x,b3_coord y)
{
	m_Camera = m_View->m_Camera;
	m_Center = m_Doc->b3GetFulcrum();
	CB3CameraRotateAction::b3LDown(x,y);
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraView implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraView::CB3ActionCameraView(CAppRenderView *window) :
	CB3Action(window)
{
}

void CB3ActionCameraView::b3LDown(b3_coord x,b3_coord y)
{
	CRect  rect;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3LDown(x,y);
	}
	else
	{
		m_Camera   = m_View->m_Camera;
		m_Distance = b3Vector::b3Distance(&m_Camera->m_EyePoint,&m_Camera->m_ViewPoint);

		m_View->GetClientRect(&rect);
		m_ySize  = rect.Height();
		m_yStart = y;
	}
}

void CB3ActionCameraView::b3LMove(b3_coord x,b3_coord y)
{
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3LMove(x,y);
	}
	else
	{
		m_Camera->b3ComputeFocalLength(m_Distance * pow(2.0,(double)(m_yStart - y) / (double)m_ySize));
		m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
	}
}

void CB3ActionCameraView::b3LUp(b3_coord x,b3_coord y)
{
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3LUp(x,y);
	}
	else
	{
		m_Camera->b3ComputeFocalLength(m_Distance * pow(2.0,(double)(m_yStart - y) / (double)m_ySize));
		m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
		m_Doc->SetModifiedFlag();
	}
	m_Camera = null;
}

void CB3ActionCameraView::b3RDown(b3_coord x,b3_coord y)
{
	b3_f64 xRel,yRel;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3RDown(x,y);
	}
	else
	{
		m_Camera    = m_View->m_Camera;
		m_Axis.pos  = m_Camera->m_EyePoint;
		b3Vector::b3Sub(
			&m_Camera->m_ViewPoint,
			&m_Camera->m_EyePoint,
			&m_Axis.dir);

		b3GetRelCoord(x,y,xRel,yRel);
		m_xRelStart = xRel;
		m_LastAngle = 0;
	}
}

void CB3ActionCameraView::b3RMove(b3_coord x,b3_coord y)
{
	b3_matrix inv,activity;
	b3_f64    xRel,yRel,angle;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3RMove(x,y);
	}
	else
	{
		b3GetRelCoord(x,y,xRel,yRel);
		angle = (xRel - m_xRelStart) * M_PI * 2;
		m_Doc->m_Info->b3SnapToAngle(angle);
		if (angle != m_LastAngle)
		{
			if (b3MatrixInv(&m_Transformation,&inv))
			{
				m_LastAngle = angle;
				b3MatrixRotVec(null,&m_Transformation,&m_Axis,angle);
				b3MatrixMMul(&inv,&m_Transformation,&activity);
				m_Camera->b3Transform(&activity);
				m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
			}
		}
	}
}

void CB3ActionCameraView::b3RUp(b3_coord x,b3_coord y)
{
	b3_matrix inv,activity;
	b3_f64    xRel,yRel,angle;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3RUp(x,y);
	}
	else
	{
		b3GetRelCoord(x,y,xRel,yRel);
		angle = (xRel - m_xRelStart) * M_PI * 2;
		m_Doc->m_Info->b3SnapToAngle(angle);
		if (angle != m_LastAngle)
		{
			if (b3MatrixInv(&m_Transformation,&inv))
			{
				m_LastAngle = angle;
				b3MatrixRotVec(null,&m_Transformation,&m_Axis,angle);
				b3MatrixMMul(&inv,&m_Transformation,&activity);
				m_Camera->b3Transform(&activity);
				m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
				m_Doc->SetModifiedFlag();
			}
		}
	}
	m_Camera = null;
}

/*************************************************************************
**                                                                      **
**                        CB3ActionLightTurn implementation             **
**                                                                      **
*************************************************************************/

CB3ActionLightTurn::CB3ActionLightTurn(CAppLinesView *window) :
	CB3CameraRotateAction(window)
{
	m_Sign       = -1;
	m_LinesDoc   = window->GetDocument();
	m_UpdateHint = B3_UPDATE_LIGHTVIEW;
}

void CB3ActionLightTurn::b3LDown(b3_coord x,b3_coord y)
{
	m_Camera = m_LinesDoc->m_CameraLight;
	m_Center = &m_Camera->m_EyePoint;
	CB3CameraRotateAction::b3LDown(x,y);
}
