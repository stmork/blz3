/*
**
**      $Filename:      b3Action.cpp $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - What to do on mouse input
**
**      (C) Copyright 2001  Steffen A. Mork
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

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2001/09/05 18:31:07  sm
**	- Fix some minor bugs
**
**	Revision 1.8  2001/09/05 15:21:34  sm
**	- Now object moving/rotating on perspective view.
**	
**	Revision 1.7  2001/09/04 20:37:53  sm
**	- Some minor updates
**	
**	Revision 1.6  2001/09/04 15:15:57  sm
**	- Added rotating objects
**	
**	Revision 1.5  2001/09/03 19:31:21  sm
**	- Now able to scale active objects
**	
**	Revision 1.4  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**	
**	Revision 1.3  2001/09/01 15:54:53  sm
**	- Tidy up Size confusion in b3Item/b3World and derived classes
**	- Made (de-)activation of objects
**	
**	Revision 1.2  2001/08/21 18:01:11  sm
**	- Minor updates
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CB3Action default implementation              **
**                                                                      **
*************************************************************************/

CB3Action::CB3Action(CAppLinesView *window)
{
	m_View   = window;
	m_Doc    = m_View->GetDocument();
	m_Button = B3_MB_UP;
}

void CB3Action::b3DispatchMouseMove  (b3_coord x,b3_coord y)
{
	if ((x != m_xLast) || (y != m_yLast))
	{
		switch (m_Button)
		{
		case B3_MB_UP:
			b3MouseMove(x,y);
			break;
		case B3_MB_LEFT:
			b3LMove(x,y);
			break;
		case B3_MB_MIDDLE:
			b3MMove(x,y);
			break;
		case B3_MB_RIGHT:
			b3RMove(x,y);
			break;
		}
		m_xLast = x;
		m_yLast = y;
	}
}

void CB3Action::b3DispatchLButtonDown(b3_coord x,b3_coord y,b3_u32 flags)
{
	m_View->SetCapture();
	m_PressedShift = (flags & MK_SHIFT)   != 0;
	m_PressedCtrl  = (flags & MK_CONTROL) != 0;
	m_Button       = B3_MB_LEFT;
	m_xStart       = m_xLast = x;
	m_yStart       = m_yLast = y;
	b3MatrixUnit(&m_Transformation);
	b3LDown(x,y);
}

void CB3Action::b3DispatchLButtonUp  (b3_coord x,b3_coord y)
{
	b3LUp(x,y);
	m_Button = B3_MB_UP;
	::ReleaseCapture();
}

void CB3Action::b3DispatchMButtonDown(b3_coord x,b3_coord y,b3_u32 flags)
{
	m_View->SetCapture();
	m_PressedShift = (flags & MK_SHIFT)   != 0;
	m_PressedCtrl  = (flags & MK_CONTROL) != 0;
	m_Button       = B3_MB_MIDDLE;
	m_xStart       = m_xLast = x;
	m_yStart       = m_yLast = y;
	b3MatrixUnit(&m_Transformation);
	b3MDown(x,y);
}

void CB3Action::b3DispatchMButtonUp(b3_coord x,b3_coord y)
{
	b3MUp(x,y);
	m_Button = B3_MB_UP;
	::ReleaseCapture();
}

void CB3Action::b3DispatchRButtonDown(b3_coord x,b3_coord y,b3_u32 flags)
{
	m_View->SetCapture();
	m_PressedShift = (flags & MK_SHIFT)   != 0;
	m_PressedCtrl  = (flags & MK_CONTROL) != 0;
	m_Button       = B3_MB_RIGHT;
	m_xStart       = m_xLast = x;
	m_yStart       = m_yLast = y;
	b3MatrixUnit(&m_Transformation);
	b3RDown(x,y);
}

void CB3Action::b3DispatchRButtonUp(b3_coord x,b3_coord y)
{
	b3RUp(x,y);
	m_Button = B3_MB_UP;
	::ReleaseCapture();
}

void CB3Action::b3GetRelCoord(
	b3_coord  x,
	b3_coord  y,
	b3_f64   &xRel,
	b3_f64   &yRel)
{
	CRect  rect;
	b3_f64 xSize,ySize;

	m_View->GetClientRect(&rect);
	xSize = rect.Width();
	ySize = rect.Height();
	xRel  = x / xSize;
	yRel  = y / ySize;
}

void CB3Action::b3MouseMove(b3_coord x,b3_coord y)
{
}

void CB3Action::b3LDown(b3_coord x,b3_coord y)
{
}

void CB3Action::b3LMove(b3_coord x,b3_coord y)
{
}

void CB3Action::b3LUp(b3_coord x,b3_coord y)
{
}

void CB3Action::b3MDown(b3_coord x,b3_coord y)
{
}

void CB3Action::b3MMove(b3_coord x,b3_coord y)
{
}

void CB3Action::b3MUp(b3_coord x,b3_coord y)
{
}

void CB3Action::b3RDown(b3_coord x,b3_coord y)
{
}

void CB3Action::b3RMove(b3_coord x,b3_coord y)
{
	b3_vector *point = m_Doc->b3GetFulcrum();
	b3_f64     xRel,yRel;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		b3GetRelCoord(x,y,xRel,yRel);
		m_View->m_RenderView.b3Unproject(xRel,yRel,point);
		m_Doc->UpdateAllViews(NULL,B3_UPDATE_FULCRUM);
	}
}

void CB3Action::b3RUp(b3_coord x,b3_coord y)
{
	b3_vector *point = m_Doc->b3GetFulcrum();
	b3_f64     xRel,yRel;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		b3GetRelCoord(x,y,xRel,yRel);
		m_View->m_RenderView.b3Unproject(xRel,yRel,point);
		m_Doc->UpdateAllViews(NULL,B3_UPDATE_FULCRUM);
		m_Doc->SetModifiedFlag();
	}
}

/*************************************************************************
**                                                                      **
**                        CB3ActionMagnify implementation               **
**                                                                      **
*************************************************************************/

CB3ActionMagnify::CB3ActionMagnify(CAppLinesView *window) :
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
	CB3Action(window)
{
}

void CB3ActionObjectMove::b3InitTranslation(b3_f64 xRel,b3_f64 yRel)
{
	b3_f64 max = 100,size;

	// Compute max. extend in any direction
	max  = m_Doc->m_Upper.x - m_Doc->m_Lower.x;
	size = m_Doc->m_Upper.y - m_Doc->m_Lower.y;
	if (size > max) max = size;
	size = m_Doc->m_Upper.z - m_Doc->m_Lower.z;
	if (size > max) max = size;

	if (false)
	{
		m_xDir.x    = max;
		m_xDir.y    = 0;
		m_xDir.z    = 0;

		m_yDir.x    = 0;
		m_yDir.y    = max;
		m_yDir.z    = 0;
	}
	else
	{
		b3CameraPart *camera = m_View->m_Camera;
		b3_f64        x,y,denom;

		x        = camera->Width.x;
		y        = camera->Width.y;
		B3_ASSERT((x != 0.0) && (y != 0.0));
		denom    = max / sqrt(x * x + y * y);
		m_xDir.x = x * denom;
		m_xDir.y = y * denom;
		m_xDir.z = 0;

		x        = camera->ViewPoint.x - camera->EyePoint.x;
		y        = camera->ViewPoint.y - camera->EyePoint.y;
		B3_ASSERT((x != 0.0) && (y != 0.0));
		denom    = max / sqrt(x * x + y * y);
		m_yDir.x = x * denom;
		m_yDir.y = y * denom;
		m_yDir.z = 0;
	}

	m_zDir.x    = 0;
	m_zDir.y    = 0;
	m_zDir.z    = max;

	m_xRelStart = xRel;
	m_yRelStart = yRel;
}

void CB3ActionObjectMove::b3LDown(b3_coord x,b3_coord y)
{
	b3_f64 xRel,yRel;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_StartPoint.x = 0;
		m_StartPoint.y = 0;
		m_StartPoint.z = 0;
		m_LastPoint    = m_StartPoint;

		m_View->m_RenderView.b3Unproject(xRel,yRel,&m_StartPoint);
	}
	else
	{
		b3InitTranslation(xRel,yRel);
	}
}

void CB3ActionObjectMove::b3LMove(b3_coord x,b3_coord y)
{
	b3_vector diff;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		diff.x = 0;
		diff.y = 0;
		diff.z = 0;

		m_View->m_RenderView.b3Unproject(xRel,yRel,&diff);
		diff.x -= m_StartPoint.x;
		diff.y -= m_StartPoint.y;
		diff.z -= m_StartPoint.z;
	}
	else
	{
		// Compute delta coefficionts
		b3_f64 xFactor = xRel - m_xRelStart;
		b3_f64 yFactor = m_yRelStart - yRel;

		// Compute moving vector;
		diff.x = xFactor * m_xDir.x + yFactor * m_yDir.x;
		diff.y = xFactor * m_xDir.y + yFactor * m_yDir.y;
		diff.z = xFactor * m_xDir.z + yFactor * m_yDir.z;
	}

	// Do action!
	m_Doc->m_Info->b3SnapToGrid(&diff);
	if (!b3IsEqual(&diff,&m_LastDiff))
	{
		if (b3MatrixInv(&m_Transformation,&inv))
		{
			m_LastDiff = diff;
			b3MatrixMove(null,&m_Transformation,&diff);
			b3MatrixMMul(&inv,&m_Transformation,&activity);
			m_Doc->m_Scene->b3Transform(&activity);
			m_Doc->b3ComputeBounds();
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

void CB3ActionObjectMove::b3LUp(b3_coord x,b3_coord y)
{
	b3_vector diff;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		diff.x = 0;
		diff.y = 0;
		diff.z = 0;

		m_View->m_RenderView.b3Unproject(xRel,yRel,&diff);
		diff.x -= m_StartPoint.x;
		diff.y -= m_StartPoint.y;
		diff.z -= m_StartPoint.z;
	}
	else
	{
		// Compute delta coefficionts
		b3_f64 xFactor = xRel - m_xRelStart;
		b3_f64 yFactor = m_yRelStart - yRel;

		// Compute moving vector;
		diff.x = xFactor * m_xDir.x + yFactor * m_yDir.x;
		diff.y = xFactor * m_xDir.y + yFactor * m_yDir.y;
		diff.z = xFactor * m_xDir.z + yFactor * m_yDir.z;
	}

	// Do action!
	if (b3MatrixInv(&m_Transformation,&inv))
	{
		m_Doc->m_Info->b3SnapToGrid(&diff);
		b3MatrixMove(null,&m_Transformation,&diff);
		b3MatrixMMul(&inv,&m_Transformation,&activity);
		m_Doc->m_Scene->b3Transform(&activity);
		m_Doc->b3ComputeBounds();
		m_Doc->SetModifiedFlag();
		m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
	}
}

void CB3ActionObjectMove::b3RDown(b3_coord x,b3_coord y)
{
	b3_f64 xRel,yRel;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		CB3Action::b3RDown(x,y);
		return;
	}
	else
	{
		b3InitTranslation(xRel,yRel);
	}
}

void CB3ActionObjectMove::b3RMove(b3_coord x,b3_coord y)
{
	b3_vector diff;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel,yFactor;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		// Do default!
		CB3Action::b3RMove(x,y);
		return;
	}

	// Compute delta coefficionts
	b3GetRelCoord(x,y,xRel,yRel);
	yFactor = m_yRelStart - yRel;

	// Compute moving vector;
	diff.x = yFactor * m_zDir.x;
	diff.y = yFactor * m_zDir.y;
	diff.z = yFactor * m_zDir.z;

	// Do action!
	m_Doc->m_Info->b3SnapToGrid(&diff);
	if (!b3IsEqual(&diff,&m_LastDiff))
	{
		if (b3MatrixInv(&m_Transformation,&inv))
		{
			m_LastDiff = diff;
			b3MatrixMove(null,&m_Transformation,&diff);
			b3MatrixMMul(&inv,&m_Transformation,&activity);
			m_Doc->m_Scene->b3Transform(&activity);
			m_Doc->b3ComputeBounds();
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

void CB3ActionObjectMove::b3RUp(b3_coord x,b3_coord y)
{
	b3_vector diff;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel,yFactor;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		// Do default!
		CB3Action::b3RUp(x,y);
		return;
	}

	// Compute delta coefficionts
	b3GetRelCoord(x,y,xRel,yRel);
	yFactor = m_yRelStart - yRel;

	// Compute moving vector;
	diff.x = yFactor * m_zDir.x;
	diff.y = yFactor * m_zDir.y;
	diff.z = yFactor * m_zDir.z;

	if (b3MatrixInv(&m_Transformation,&inv))
	{
		b3MatrixMove(null,&m_Transformation,&diff);
		b3MatrixMMul(&inv,&m_Transformation,&activity);
		m_Doc->m_Scene->b3Transform(&activity);
		m_Doc->b3ComputeBounds();
		m_Doc->SetModifiedFlag();
		m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
	}
}

/*************************************************************************
**                                                                      **
**                        CB3ActionObjectRotate implementation          **
**                                                                      **
*************************************************************************/

CB3ActionObjectRotate::CB3ActionObjectRotate(CAppLinesView *window) :
	CB3Action(window)
{
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
			m_Doc->m_Scene->b3Transform(&activity);
			m_Doc->b3ComputeBounds();
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
		m_Doc->m_Scene->b3Transform(&activity);
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
			m_Doc->m_Scene->b3Transform(&activity);
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
			b3MatrixScale(null,&m_Transformation,m_Center,&scale);
			b3MatrixMMul(&inv,&m_Transformation,&activity);
			m_Doc->m_Scene->b3Transform(&activity);
			m_Doc->b3ComputeBounds();
			m_Doc->SetModifiedFlag();
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_GEOMETRY);
		}
	}
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraMove implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraMove::CB3ActionCameraMove(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraTurn implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraTurn::CB3ActionCameraTurn(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraRotate implementation          **
**                                                                      **
*************************************************************************/

CB3ActionCameraRotate::CB3ActionCameraRotate(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionCameraView implementation            **
**                                                                      **
*************************************************************************/

CB3ActionCameraView::CB3ActionCameraView(CAppLinesView *window) :
	CB3Action(window)
{
}

/*************************************************************************
**                                                                      **
**                        CB3ActionLightTurn implementation             **
**                                                                      **
*************************************************************************/

CB3ActionLightTurn::CB3ActionLightTurn(CAppLinesView *window) :
	CB3Action(window)
{
}
