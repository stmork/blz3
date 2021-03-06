/*
**
**	$Filename:	b3ActionScene.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - What to do on mouse input (scene action)
**
**	(C) Copyright 2002  Steffen A. Mork
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
#include "b3Action.h"

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
	m_LinesDoc = window->GetDocument();
}

b3_bool CB3ActionObjectSelect::b3IsPointSelection(b3_coord x,b3_coord y)
{
	return (m_xStart == x) && (m_yStart == y);
}

void CB3ActionObjectSelect::b3ComputeSelectionDir(
	b3_coord  x,
	b3_coord  y,
	b3_line  *selection_dir)
{
	b3_f64 xRel,yRel;

	b3GetRelCoord(x,y,xRel,yRel);
	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		b3CameraPart *camera = m_View->m_Camera;
		b3_vector     view;

		// Compute selection direction
		selection_dir->pos = camera->m_EyePoint;
		m_View->m_RenderView.b3Unproject(x,y,&view);
		b3Vector::b3Sub(&view,&camera->m_EyePoint,&selection_dir->dir);
	}
	else
	{
		m_View->m_RenderView.b3GetProjectionBase(&selection_dir->pos);
		m_View->m_RenderView.b3Unproject(xRel,yRel,&selection_dir->pos);
		m_View->m_RenderView.b3GetViewDirection(&selection_dir->dir);
	}
	b3Vector::b3Normalize(&selection_dir->dir);
}

void CB3ActionObjectSelect::b3LMove(b3_coord x,b3_coord y)
{
	m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
	m_View->b3DrawRect(m_xStart,m_yStart,x,y);
}

void CB3ActionObjectSelect::b3LUp(b3_coord x,b3_coord y)
{
	if (b3IsPointSelection(x,y))
	{
		b3_line selection_dir;
	
		b3ComputeSelectionDir(x,y,&selection_dir);
		m_LinesDoc->b3Select(&selection_dir,true,m_PressedCtrl);
	}
	else
	{
		if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
		{
			b3_vector lower = m_LinesDoc->m_Lower;
			b3_vector upper = m_LinesDoc->m_Upper;

			m_View->m_RenderView.b3Unproject(  x,       y,     &lower);
			m_View->m_RenderView.b3Unproject(m_xStart,m_yStart,&upper);
			m_LinesDoc->b3Select(&lower,&upper,true,m_PressedCtrl);
		}
		else
		{
			m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
		}
	}
}

void CB3ActionObjectSelect::b3RMove(b3_coord x,b3_coord y)
{
	m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
	m_View->b3DrawRect(m_xStart,m_yStart,x,y);
}

void CB3ActionObjectSelect::b3RUp(b3_coord x,b3_coord y)
{
	b3_line selection_dir;

	if (b3IsPointSelection(x,y))
	{
		b3ComputeSelectionDir(x,y,&selection_dir);
		m_LinesDoc->b3Select(&selection_dir,false,m_PressedCtrl);
	}
	else
	{
		if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
		{
			b3_vector lower = m_LinesDoc->m_Lower;
			b3_vector upper = m_LinesDoc->m_Upper;

			m_View->m_RenderView.b3Unproject(  x,       y,     &lower);
			m_View->m_RenderView.b3Unproject(m_xStart,m_yStart,&upper);
			m_LinesDoc->b3Select(&lower,&upper,false,m_PressedCtrl);
		}
		else
		{
			m_View->b3DrawRect(m_xStart,m_yStart,m_xLast,m_yLast);
		}
	}
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
	
	pDoc->m_Scene->b3Transform(transformation, true);
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

	m_Doc->m_Info->b3SnapToObjectAngle(angle);
	if (angle != m_LastAngle)
	{
		if (b3Matrix::b3Inverse(&m_Transformation,&inv))
		{
			m_LastAngle = angle;
			b3Matrix::b3RotateVector(null,&m_Transformation,&m_Axis,angle);
			b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
			m_LinesView->GetDocument()->m_Scene->b3Transform(&activity, true);
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

	m_Doc->m_Info->b3SnapToObjectAngle(angle);
	if (b3Matrix::b3Inverse(&m_Transformation,&inv))
	{
		b3Matrix::b3RotateVector(null,&m_Transformation,&m_Axis,angle);
		b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
		m_LinesView->GetDocument()->m_Scene->b3Transform(&activity, true);
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
	else
	{
		CRect rect;

		m_LastScale = 1.0;
		m_View->GetClientRect(&rect);
		m_ySize  = rect.Height();
		m_yStart = y;
	}
}

void CB3ActionObjectScale::b3LMove(b3_coord x,b3_coord y)
{
	b3_vector scale;
	b3_matrix inv,activity;

	if (b3ComputeScaling(x,y,scale))
	{
		if (b3Matrix::b3Inverse(&m_Transformation,&inv))
		{
			b3Matrix::b3Scale(null,&m_Transformation,m_Center,&scale);
			b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
			m_LinesView->GetDocument()->m_Scene->b3Transform(&activity, false);
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

void CB3ActionObjectScale::b3LUp(b3_coord x,b3_coord y)
{
	b3_vector scale;
	b3_matrix inv,activity;

	if (b3ComputeScaling(x,y,scale))
	{
		if (b3Matrix::b3Inverse(&m_Transformation,&inv))
		{
			CAppLinesDoc *pDoc = m_LinesView->GetDocument();

			b3Matrix::b3Scale(null,&m_Transformation,m_Center,&scale);
			b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
			pDoc->m_Scene->b3Transform(&activity, false);
			pDoc->b3ComputeBounds();
			pDoc->SetModifiedFlag();
			pDoc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

b3_bool CB3ActionObjectScale::b3ComputeScaling(b3_coord x,b3_coord y,b3_vector &scale)
{
	b3_vector diff,point;
	b3_f64    xRel,yRel;

	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		if (m_PressedCtrl || m_PressedShift)
		{
			b3_f64  factor = pow(2.0,(double)(m_yStart - y) / (double)m_ySize);
			b3_bool changed;

			changed = factor != m_LastScale;
			if (changed)
			{
				scale.x =
				scale.y =
				scale.z = factor;
				m_LastScale = factor;
			}
			return changed;
		}
		return false;
	}

	point = *m_Center;

	b3GetRelCoord(x,y,xRel,yRel);
	m_View->m_RenderView.b3Unproject(xRel,yRel,&point);
	b3Vector::b3Sub(m_Center,&point,&diff);

	if (m_PressedCtrl || m_PressedShift)
	{
		scale.x =
		scale.y =
		scale.z =
			b3Vector::b3Length(&diff) /
			b3Vector::b3Length(&m_StartDiff);
	}
	else
	{
		if (m_StartDiff.x == 0)
		{
			scale.x = (diff.x == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.x = diff.x / m_StartDiff.x;
		}
		
		if (m_StartDiff.y == 0)
		{
			scale.y = (diff.y == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.y = diff.y / m_StartDiff.y;
		}

		if (m_StartDiff.z == 0)
		{
			scale.z = (diff.z == 0.0 ? 1.0 : 0.0);
		}
		else
		{
			scale.z = diff.z / m_StartDiff.z;
		}
	}
	return true;
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

void CB3ActionCameraTurn::b3RDown(b3_coord x,b3_coord y)
{
	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CAppRenderDoc *pDoc = m_View->GetDocument();
		b3_f64 unit = pDoc->m_Info->b3ScaleUnitToMM();
		m_Camera = m_View->m_Camera;

		b3GetRelCoord(x,y,m_xRelStart, m_yRelStart);
		b3Vector::b3Init(&m_LastDiff);
		b3Vector::b3Sub(&m_Camera->m_ViewPoint, &m_Camera->m_EyePoint, &m_ViewDir);
		b3Vector::b3Normalize(&m_ViewDir, 1000.0 / unit );
	}
}

void CB3ActionCameraTurn::b3RMove(b3_coord x,b3_coord y)
{
	b3_vector diff;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;

	b3GetRelCoord(x,y,xRel,yRel);

	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		// Compute delta coefficionts
		b3_f64 xFactor = xRel - m_xRelStart;
		b3_f64 yFactor = m_yRelStart - yRel;

		// Compute moving vector;
		b3Vector::b3Scale(&m_ViewDir, &diff, yFactor);

		if (!b3Vector::b3IsEqual(&diff,&m_LastDiff))
		{
			if (b3Matrix::b3Inverse(&m_Transformation,&inv))
			{
				m_LastDiff = diff;
				b3Matrix::b3Move(null,&m_Transformation,&diff);
				b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
				m_Camera->b3Transform(&activity);
				m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
			}
		}
	}	
}

void CB3ActionCameraTurn::b3RUp(b3_coord x,b3_coord y)
{
	m_Camera = null;
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
		m_Camera    = m_View->m_Camera;
		m_LastScale = 1.0;
		m_Distance  = b3Vector::b3Distance(&m_Camera->m_EyePoint,&m_Camera->m_ViewPoint);
		m_Axis.pos  = m_Camera->m_EyePoint;
		b3Vector::b3Sub(
			&m_Camera->m_ViewPoint,
			&m_Camera->m_EyePoint,
			&m_Axis.dir);

		m_View->GetClientRect(&rect);
		m_ySize  = rect.Height();
		m_yStart = y;
	}
}

void CB3ActionCameraView::b3LMove(b3_coord x,b3_coord y)
{
	b3_matrix align,invalign,activity,inv;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3LMove(x,y);
	}
	else
	{
		b3_f64 scale = pow(2.0,(double)(m_yStart - y) / (double)m_ySize);

		if (scale != m_LastScale)
		{
			b3Matrix::b3Align(&align,&m_Axis);
			m_LastScale = scale;
			if (b3Matrix::b3Inverse(&m_Transformation,&inv) &&
				b3Matrix::b3Inverse(&align,&invalign))
			{
				b3Matrix::b3Scale(&invalign,&activity,&m_Axis.pos,scale);
				b3Matrix::b3MMul(&activity,&align,&m_Transformation);
				b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
				m_Camera->b3Transform(&activity);
				m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
			}
		}
	}
}

void CB3ActionCameraView::b3LUp(b3_coord x,b3_coord y)
{
	b3_matrix align,invalign,activity,inv;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3LUp(x,y);
	}
	else
	{
		b3_f64 scale = pow(2.0,(double)(m_yStart - y) / (double)m_ySize);

		if (scale != m_LastScale)
		{
			b3Matrix::b3Align(&align,&m_Axis);
			m_LastScale = scale;
			if (b3Matrix::b3Inverse(&m_Transformation,&inv) &&
				b3Matrix::b3Inverse(&align,&invalign))
			{
				b3Matrix::b3Scale(&invalign,&activity,null,scale);
				b3Matrix::b3MMul(&activity,&align,&m_Transformation);
				b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
				m_Camera->b3Transform(&activity);
				m_Doc->UpdateAllViews(NULL,B3_UPDATE_CAMERA);
				m_Doc->SetModifiedFlag();
			}
		}
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
		m_Doc->m_Info->b3SnapToCameraAngle(angle);
		if (angle != m_LastAngle)
		{
			if (b3Matrix::b3Inverse(&m_Transformation,&inv))
			{
				m_LastAngle = angle;
				b3Matrix::b3RotateVector(null,&m_Transformation,&m_Axis,angle);
				b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
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
		m_Doc->m_Info->b3SnapToCameraAngle(angle);
		if (angle != m_LastAngle)
		{
			if (b3Matrix::b3Inverse(&m_Transformation,&inv))
			{
				m_LastAngle = angle;
				b3Matrix::b3RotateVector(null,&m_Transformation,&m_Axis,angle);
				b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
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
