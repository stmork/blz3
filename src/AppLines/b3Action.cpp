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
#include "AppLines.h"
#include "MainFrm.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.24  2003/02/18 16:52:57  sm
**	- Fixed no name error on new scenes (ticket no. 4).
**	- Introduced new b3Matrix class and renamed methods.
**
**	Revision 1.23  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**	
**	Revision 1.22  2003/01/05 16:13:24  sm
**	- First undo/redo implementations
**	
**	Revision 1.21  2002/03/01 20:26:40  sm
**	- Added CB3FloatSpinButtonCtrl for conveniant input.
**	- Made some minor changes and tests.
**	
**	Revision 1.20  2002/02/13 16:13:08  sm
**	- Created spotlight view
**	- Changed camera properties dialog to reflect scene units
**	  on example camera settings.
**	
**	Revision 1.19  2002/02/12 18:39:03  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.18  2002/01/16 16:17:13  sm
**	- Introducing object edit painting and acting.
**	
**	Revision 1.17  2002/01/13 19:24:12  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**	Revision 1.16  2002/01/06 16:30:47  sm
**	- Added Load/Save/Replace object
**	- Enhanced "New world"
**	- Added some non static methods to b3Dir (Un*x untested, yet!)
**	- Fixed missing sphere/ellipsoid south pole triangles
**	- Fixed Spline cylinder/ring triangle indexing
**	
**	Revision 1.15  2001/12/26 18:17:56  sm
**	- More status bar information displayed (e.g. coordinates)
**	- Some minor UI updates
**	
**	Revision 1.14  2001/12/25 18:52:39  sm
**	- Introduced CB3Dialogbar for dialogs opened any time.
**	- Fulcrum fixed with snap to grid
**	
**	Revision 1.13  2001/12/18 14:36:25  sm
**	- Double click bug on camera acting found and fixed
**	
**	Revision 1.12  2001/12/08 19:17:07  sm
**	- Added interactive focal length
**	- Added interactive twirl
**	- Added camera move left/right on right mouse button
**	
**	Revision 1.11  2001/11/25 19:20:32  sm
**	- Added new acting methods:
**	  o Camera move
**	  o Camera turn around itself
**	  o Camera rotate around fulcrum
**	
**	Revision 1.10  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
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

CB3Action::CB3Action(CAppRenderView *window)
{
	m_View   = window;
	m_Doc    = m_View->GetDocument();
	m_Button = B3_MB_UP;
}

b3_matrix *CB3Action::b3GetTransformation()
{
	return &m_Transformation;
}

b3_bool CB3Action::b3IsObject()
{
	return false;
}

b3_bool CB3Action::b3IsCamera()
{
	return false;
}

void CB3Action::b3DispatchMouseMove  (b3_coord x,b3_coord y)
{
	if ((x != m_xLast) || (y != m_yLast))
	{
		CMainFrame *main = CB3GetMainFrame();
		b3_vector   position;
		b3_f64      xRel,yRel;

		if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
		{
			main->b3SetPosition(null);
		}
		else
		{
			b3Vector::b3Init(&position);
			b3GetRelCoord(x,y,xRel,yRel);
			m_View->m_RenderView.b3Unproject(xRel,yRel,&position);
			main->b3SetPosition(&position);
		}

		if (::GetCapture() == m_View->m_hWnd)
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
}

void CB3Action::b3DispatchLButtonDown(b3_coord x,b3_coord y,b3_u32 flags)
{
	m_View->SetCapture();
	m_PressedShift = (flags & MK_SHIFT)   != 0;
	m_PressedCtrl  = (flags & MK_CONTROL) != 0;
	m_Button       = B3_MB_LEFT;
	m_xStart       = m_xLast = x;
	m_yStart       = m_yLast = y;
	b3Matrix::b3Unit(&m_Transformation);
	b3LDown(x,y);
}

void CB3Action::b3DispatchLButtonUp  (b3_coord x,b3_coord y)
{
	if (::GetCapture() == m_View->m_hWnd)
	{
		b3LUp(x,y);
		m_Button = B3_MB_UP;
		::ReleaseCapture();
	}
}

void CB3Action::b3DispatchMButtonDown(b3_coord x,b3_coord y,b3_u32 flags)
{
	m_View->SetCapture();
	m_PressedShift = (flags & MK_SHIFT)   != 0;
	m_PressedCtrl  = (flags & MK_CONTROL) != 0;
	m_Button       = B3_MB_MIDDLE;
	m_xStart       = m_xLast = x;
	m_yStart       = m_yLast = y;
	b3Matrix::b3Unit(&m_Transformation);
	b3MDown(x,y);
}

void CB3Action::b3DispatchMButtonUp(b3_coord x,b3_coord y)
{
	if (::GetCapture() == m_View->m_hWnd)
	{
		b3MUp(x,y);
		m_Button = B3_MB_UP;
		::ReleaseCapture();
	}
}

void CB3Action::b3DispatchRButtonDown(b3_coord x,b3_coord y,b3_u32 flags)
{
	m_View->SetCapture();
	m_PressedShift = (flags & MK_SHIFT)   != 0;
	m_PressedCtrl  = (flags & MK_CONTROL) != 0;
	m_Button       = B3_MB_RIGHT;
	m_xStart       = m_xLast = x;
	m_yStart       = m_yLast = y;
	b3Matrix::b3Unit(&m_Transformation);
	b3RDown(x,y);
}

void CB3Action::b3DispatchRButtonUp(b3_coord x,b3_coord y)
{
	if (::GetCapture() == m_View->m_hWnd)
	{
		b3RUp(x,y);
		m_Button = B3_MB_UP;
		::ReleaseCapture();
	}
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
	b3_vector  diff  = *point;
	b3_f64     xRel,yRel;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		b3GetRelCoord(x,y,xRel,yRel);
		m_View->m_RenderView.b3Unproject(xRel,yRel,&diff);
		m_Doc->m_Info->b3SnapToGrid(&diff);
		if (!b3Vector::b3IsEqual(&diff,point))
		{
			*point = diff;
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_FULCRUM);
		}
	}
}

void CB3Action::b3RUp(b3_coord x,b3_coord y)
{
	b3_vector *point = m_Doc->b3GetFulcrum();
	b3_vector  diff  = *point;
	b3_f64     xRel,yRel;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		b3GetRelCoord(x,y,xRel,yRel);
		m_View->m_RenderView.b3Unproject(xRel,yRel,point);
		m_Doc->m_Info->b3SnapToGrid(&diff);
		if (!b3Vector::b3IsEqual(&diff,point))
		{
			*point = diff;
			m_Doc->UpdateAllViews(NULL,B3_UPDATE_FULCRUM);
			m_Doc->SetModifiedFlag();
		}
	}
}

/*************************************************************************
**                                                                      **
**                        CB3MoveAction implementation                  **
**                                                                      **
*************************************************************************/

CB3MoveAction::CB3MoveAction(CAppRenderView *window) :
	CB3Action(window)
{
}

void CB3MoveAction::b3Transform(b3_matrix *transformation)
{
}

void CB3MoveAction::b3InitTranslation(b3_f64 xRel,b3_f64 yRel)
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

		x        = camera->m_Width.x;
		y        = camera->m_Width.y;
		B3_ASSERT((x != 0.0) || (y != 0.0));
		denom    = max / sqrt(x * x + y * y);
		m_xDir.x = x * denom;
		m_xDir.y = y * denom;
		m_xDir.z = 0;

		x        = camera->m_ViewPoint.x - camera->m_EyePoint.x;
		y        = camera->m_ViewPoint.y - camera->m_EyePoint.y;
		B3_ASSERT((x != 0.0) || (y != 0.0));
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

void CB3MoveAction::b3LDown(b3_coord x,b3_coord y)
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

void CB3MoveAction::b3LMove(b3_coord x,b3_coord y)
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
	if (!b3Vector::b3IsEqual(&diff,&m_LastDiff))
	{
		if (b3Matrix::b3Inverse(&m_Transformation,&inv))
		{
			m_LastDiff = diff;
			b3Matrix::b3Move(null,&m_Transformation,&diff);
			b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
			b3Transform(&activity);
		}
	}
}

void CB3MoveAction::b3LUp(b3_coord x,b3_coord y)
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
	if (b3Matrix::b3Inverse(&m_Transformation,&inv))
	{
		m_Doc->m_Info->b3SnapToGrid(&diff);
		b3Matrix::b3Move(null,&m_Transformation,&diff);
		b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
		b3Transform(&activity);
		m_Doc->b3ComputeBounds();
		m_Doc->SetModifiedFlag();
	}
}

void CB3MoveAction::b3RDown(b3_coord x,b3_coord y)
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

void CB3MoveAction::b3RMove(b3_coord x,b3_coord y)
{
	b3_vector diff;
	b3_matrix inv,activity;
	b3_f64    xRel,xFactor;
	b3_f64    yRel,yFactor;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		// Do default!
		CB3Action::b3RMove(x,y);
		return;
	}

	// Compute delta coefficionts
	b3GetRelCoord(x,y,xRel,yRel);
	xFactor = xRel - m_xRelStart;
	yFactor = m_yRelStart - yRel;

	// Compute moving vector;
	diff.x = xFactor * m_xDir.x + yFactor * m_zDir.x;
	diff.y = xFactor * m_xDir.y + yFactor * m_zDir.y;
	diff.z = xFactor * m_xDir.z + yFactor * m_zDir.z;

	// Do action!
	m_Doc->m_Info->b3SnapToGrid(&diff);
	if (!b3Vector::b3IsEqual(&diff,&m_LastDiff))
	{
		if (b3Matrix::b3Inverse(&m_Transformation,&inv))
		{
			m_LastDiff = diff;
			b3Matrix::b3Move(null,&m_Transformation,&diff);
			b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
			b3Transform(&activity);
		}
	}
}

void CB3MoveAction::b3RUp(b3_coord x,b3_coord y)
{
	b3_vector diff;
	b3_matrix inv,activity;
	b3_f64    xRel,xFactor;
	b3_f64    yRel,yFactor;

	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		// Do default!
		CB3Action::b3RUp(x,y);
		return;
	}

	// Compute delta coefficionts
	b3GetRelCoord(x,y,xRel,yRel);
	xFactor = xRel - m_xRelStart;
	yFactor = m_yRelStart - yRel;

	// Compute moving vector;
	diff.x = xFactor * m_xDir.x + yFactor * m_zDir.x;
	diff.y = xFactor * m_xDir.y + yFactor * m_zDir.y;
	diff.z = xFactor * m_xDir.z + yFactor * m_zDir.z;

	if (b3Matrix::b3Inverse(&m_Transformation,&inv))
	{
		b3Matrix::b3Move(null,&m_Transformation,&diff);
		b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
		b3Transform(&activity);
		m_Doc->b3ComputeBounds();
		m_Doc->SetModifiedFlag();
	}
}

/*************************************************************************
**                                                                      **
**                        CB3CameraRotateAction implementation          **
**                                                                      **
*************************************************************************/

CB3CameraRotateAction::CB3CameraRotateAction(CAppRenderView *window) :
	CB3Action(window)
{
	m_Sign       = 0;
	m_Camera     = null;
	m_UpdateHint = B3_UPDATE_CAMERA;
}

void CB3CameraRotateAction::b3LDown(b3_coord x,b3_coord y)
{
	b3_f64 xRel,yRel;

	B3_ASSERT(m_Camera != null);
	m_StartPoint = *m_Center;
	m_Axis.pos   = *m_Center;
	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_View->m_RenderView.b3GetViewDirection(&m_Axis.dir);
		m_View->m_RenderView.b3Unproject(xRel,yRel,&m_StartPoint);
	
		m_StartAngle = m_View->m_RenderView.b3GetPositionAngle(m_Center,&m_StartPoint);
		m_xLastAngle = m_StartAngle;
	}
	else
	{
		b3_vector view;

		m_UpDown.pos = *m_Center;
		b3Vector::b3Sub(&m_Camera->m_ViewPoint,&m_Camera->m_EyePoint,&view);
		if ((view.x != 0) || (view.y != 0))
		{
			B3_ASSERT(view.z != 0);
			b3Vector::b3Init(&m_Axis.dir,0,0,1);
		}
		else
		{
			b3Vector::b3Init(&m_Axis.dir,1,0,0);
		}
		b3Vector::b3CrossProduct(&view,&m_Axis.dir,&m_UpDown.dir);

		m_xRelStart = xRel;
		m_yRelStart = yRel;
	}
}

void CB3CameraRotateAction::b3LMove(b3_coord x,b3_coord y)
{
	b3_vector point;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;
	b3_f64    angle;
	b3_f64    xAngle,yAngle;
	b3_bool   compute = false;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		point = *m_Center;

		m_View->m_RenderView.b3Unproject(xRel,yRel,&point);

		angle = m_StartAngle - m_View->m_RenderView.b3GetPositionAngle(m_Center,&point);
		m_Doc->m_Info->b3SnapToAngle(angle);
		if (angle != m_xLastAngle)
		{
			if (b3Matrix::b3Inverse(&m_Transformation,&inv))
			{
				m_xLastAngle = angle;
				b3Matrix::b3RotateVector(null,&m_Transformation,&m_Axis,angle);
				compute = true;
			}
		}
	}
	else
	{
		xAngle = (xRel - m_xRelStart) * M_PI * 2;
		yAngle = (yRel - m_yRelStart) * M_PI * 2;
		m_Doc->m_Info->b3SnapToAngle(xAngle);
		m_Doc->m_Info->b3SnapToAngle(yAngle);
		if ((xAngle != m_xLastAngle) || (yAngle != m_yLastAngle))
		{
			if (b3Matrix::b3Inverse(&m_Transformation,&inv))
			{
				m_xLastAngle = xAngle;
				m_yLastAngle = yAngle;
				b3Matrix::b3RotateVector(null,             &m_Transformation,&m_UpDown, yAngle * m_Sign);
				b3Matrix::b3RotateVector(&m_Transformation,&m_Transformation,&m_Axis,   xAngle * m_Sign);
				compute = true;
			}
		}
	}

	if (compute)
	{
		b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
		m_Camera->b3Transform(&activity);
		m_Doc->UpdateAllViews(NULL,m_UpdateHint);
	}
}

void CB3CameraRotateAction::b3LUp(b3_coord x,b3_coord y)
{
	b3_vector point;
	b3_matrix inv,activity;
	b3_f64    xRel,yRel;
	b3_f64    angle;
	b3_f64    xAngle,yAngle;

	b3GetRelCoord(x,y,xRel,yRel);
	if (!m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		point = *m_Center;

		m_View->m_RenderView.b3Unproject(xRel,yRel,&point);

		angle = m_StartAngle - m_View->m_RenderView.b3GetPositionAngle(m_Center,&point);
		m_Doc->m_Info->b3SnapToAngle(angle);
		b3Matrix::b3RotateVector(null,&m_Transformation,&m_Axis,angle);
	}
	else
	{
		xAngle = (xRel - m_xRelStart) * M_PI * 2;
		yAngle = (yRel - m_yRelStart) * M_PI * 2;
		m_Doc->m_Info->b3SnapToAngle(xAngle);
		m_Doc->m_Info->b3SnapToAngle(yAngle);
		b3Matrix::b3RotateVector(null,             &m_Transformation,&m_UpDown, yAngle * m_Sign);
		b3Matrix::b3RotateVector(&m_Transformation,&m_Transformation,&m_Axis,   xAngle * m_Sign);
	}

	if (b3Matrix::b3Inverse(&m_Transformation,&inv))
	{
		b3Matrix::b3MMul(&inv,&m_Transformation,&activity);
		m_Camera->b3Transform(&activity);
		m_Doc->UpdateAllViews(NULL,m_UpdateHint);
		m_Doc->SetModifiedFlag();
	}
	m_Camera = null;
}

/*************************************************************************
**                                                                      **
**                        CB3ShapeAction implementation                 **
**                                                                      **
*************************************************************************/

CB3ShapeAction::CB3ShapeAction(CAppObjectView *window) :
	CB3Action(window)
{
	m_ObjectView = window;
}

void CB3ShapeAction::b3LDown(b3_coord x,b3_coord y)
{
	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		m_xStartAngle = m_ObjectView->m_xAngle;
		m_yStartAngle = m_ObjectView->m_yAngle;
		b3GetRelCoord(x,y,m_xRelStart,m_yRelStart);
	}
	else
	{
		CB3Action::b3LDown(x,y);
	}
}

void CB3ShapeAction::b3LMove(b3_coord x,b3_coord y)
{
	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		b3_f64 xRel,xAngle;
		b3_f64 yRel,yAngle;

		b3GetRelCoord(x,y,xRel,yRel);
		xAngle = (xRel - m_xRelStart) * M_PI * 2;
		yAngle = (yRel - m_yRelStart) * M_PI * 2;

		m_ObjectView->m_xAngle = m_xStartAngle - xAngle;
		m_ObjectView->m_yAngle = m_yStartAngle + yAngle;
		if (m_ObjectView->m_yAngle < -B3_RAD(89.5))
		{
			m_ObjectView->m_yAngle =
			m_yStartAngle = -B3_RAD(89.5);
			m_yRelStart   = yRel;
		}
		if (m_ObjectView->m_yAngle > B3_RAD(89.5))
		{
			m_ObjectView->m_yAngle =
			m_yStartAngle = B3_RAD(89.5);
			m_yRelStart   = yRel;
		}
		m_View->b3Update(B3_UPDATE_CAMERA);
	}
	else
	{
		CB3Action::b3LMove(x,y);
	}
}

void CB3ShapeAction::b3LUp(b3_coord x,b3_coord y)
{
	if (m_View->m_RenderView.b3IsViewMode(B3_VIEW_3D))
	{
		b3_f64 xRel,xAngle;
		b3_f64 yRel,yAngle;

		b3GetRelCoord(x,y,xRel,yRel);
		xAngle = (xRel - m_xRelStart) * M_PI * 2;
		yAngle = (yRel - m_yRelStart) * M_PI * 2;

		m_ObjectView->m_xAngle = m_xStartAngle - xAngle;
		m_ObjectView->m_yAngle = m_yStartAngle + yAngle;
		B3_LIMIT(m_ObjectView->m_yAngle,-B3_RAD(89.5),B3_RAD(89.5));
		m_View->b3Update(B3_UPDATE_CAMERA);
	}
	else
	{
		CB3Action::b3LUp(x,y);
	}
}

