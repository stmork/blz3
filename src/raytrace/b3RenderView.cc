/*
**
**      $Filename:      b3RenderView.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Render view mode handling
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

#include "blz3/raytrace/b3Raytrace.h"
#include "blz3/raytrace/b3RenderView.h"
#include "blz3/base/b3Matrix.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.23  2002/01/09 17:47:54  sm
**	- Finished CB3ImageButton implementation.
**	- Finished CDlgObjectCopy
**
**	Revision 1.22  2002/01/08 15:45:50  sm
**	- Added support for repeating CButtons for button movement/rotation mode.
**	
**	Revision 1.21  2001/12/02 17:38:17  sm
**	- Removing nasty CR/LF
**	- Added b3ExtractExt()
**	- Added stricmp() for Un*x
**	- Fixed some defines
**	
**	Revision 1.20  2001/11/26 17:16:37  sm
**	- Linux b3TimeSpan fix
**	
**	Revision 1.19  2001/11/26 17:12:48  sm
**	- Far clipping plane fix for Un*x
**	
**	Revision 1.18  2001/11/18 13:49:26  sm
**	- Introduced new CB3FloatEdit derived from CEdit
**	- DlgNebular implemented
**	- DlgLensFlare implemented
**	- Adjusting far clipping plane inside b3RenderView
**	
**	Revision 1.17  2001/11/03 16:24:16  sm
**	- Added scene property dialog
**	- Added raytrace view title
**	- Added raytrace abort on button press
**	
**	Revision 1.16  2001/10/19 14:46:57  sm
**	- Rotation spline shape bug found.
**	- Major optimizations done.
**	- Cleanups
**	
**	Revision 1.15  2001/09/30 15:53:19  sm
**	- Removing nasty CR/LF
**	
**	Revision 1.14  2001/09/23 14:11:18  sm
**	- A new raytrace is born! But it isn't raytracing yet.
**	
**	Revision 1.13  2001/09/05 15:21:34  sm
**	- Now object moving/rotating on perspective view.
**	   Revision 1.12  2001/09/04 15:15:57  sm
**	- Added rotating objects
**
**	Revision 1.11  2001/09/02 18:54:56  sm
**	- Moving objects
**	- BBox size recomputing fixed. Further cleanups in b3RenderObject
**	  are necessary.
**	- It's really nice to see!
**
**	Revision 1.10  2001/08/15 19:52:57  sm
**	- First polygon rendering with Blizzard III (areas only)
**
**	Revision 1.9  2001/08/14 15:37:50  sm
**	- Made some cleanups when OpenGL isn't available.
**
**	Revision 1.8  2001/08/14 13:34:40  sm
**	- Corredted aspect ratio handling when doing somethiing with
**	  the view
**	- New application icon
**	- Minor GUI bug fixes.
**
**	Revision 1.7  2001/08/14 07:03:28  sm
**	- Made some ASSERT cleanups. New define when _DEBUG is switched on:
**	  B3_ASSERT(condition) abort()s when condition is false.
**
**	Revision 1.6  2001/08/13 15:05:01  sm
**	- Now we can scale and move around with stacked views.
**
**	Revision 1.5  2001/08/12 19:47:48  sm
**	- Now having correct orthogonal projection incl. aspect ratio
**
**	Revision 1.4  2001/08/11 20:17:22  sm
**	- Updated OpenGL on Un*x platform.
**	- This was a great day!
**
**	Revision 1.3  2001/08/11 19:59:16  sm
**	- Added orthogonal projection
**
**	Revision 1.2  2001/08/11 16:29:07  sm
**	- Nasty UnCR done
**	- Compiling but not running OpenGL under Unix
**
**	Revision 1.1  2001/08/11 15:59:59  sm
**	- Rendering cleaned up
**	- CWinApp/CMainFrm derived from Blizzard III classes
**	  supporting more effective GUI.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderViewItem::b3RenderViewItem() :
	b3Link<b3RenderViewItem>(sizeof(b3RenderViewItem))
{
	m_Mid.x =
	m_Mid.y =
	m_Mid.z = 0;
	m_Size.x =
	m_Size.y =
	m_Size.z = 0;
}

b3RenderViewItem::b3RenderViewItem(
	b3_vector *lower,
	b3_vector *upper) :
		b3Link<b3RenderViewItem>(sizeof(b3RenderViewItem))
{
	b3Set(lower,upper);
}

void b3RenderViewItem::b3Set(
	b3_vector *lower,
	b3_vector *upper)
{
	m_Mid.x  = (upper->x + lower->x) * 0.5;
	m_Mid.y  = (upper->y + lower->y) * 0.5;
	m_Mid.z  = (upper->z + lower->z) * 0.5;
	m_Size.x =  upper->x - lower->x;
	m_Size.y =  upper->y - lower->y;
	m_Size.z =  upper->z - lower->z;
	m_xRelation = 1;
	m_yRelation = 1;
}

b3RenderView::b3RenderView()
{
	b3_index          i;

	m_ViewMode    = B3_VIEW_3D;
	m_AntiAliased = false;
	m_AspectRatio = true;

	for (i = 0;i < B3_VIEW_MAX;i++)
	{
		m_ViewStack[i].b3InitBase();
	}
	m_Depot.b3InitBase();
}

b3RenderView::~b3RenderView()
{
	b3RenderViewItem *item;
	b3_index          i;

	for (i = 1;i < B3_VIEW_MAX;i++)
	{
		while((item = m_ViewStack[i].First) != null)
		{
			m_ViewStack[i].b3Remove(item);
			delete item;
		}
	}

	while((item = m_Depot.First) != null)
	{
		m_Depot.b3Remove(item);
		delete item;
	}
}

b3RenderViewItem *b3RenderView::b3NewRenderViewItem(
	b3RenderViewItem *last)
{
	b3RenderViewItem *item;

	item = m_Depot.First;
	if (item != null)
	{
		m_Depot.b3Remove(item);
	}
	else
	{
		item = new b3RenderViewItem();
	}

	if(last != null)
	{
		item->m_Mid  = last->m_Mid;
		item->m_Size = last->m_Size;
		item->m_xRelation = last->m_xRelation;
		item->m_yRelation = last->m_yRelation;
	}
	else
	{
		item->b3Set(&m_Lower,&m_Upper);
	}
	return item;
}

void b3RenderView::b3SetViewMode(b3_view_mode mode)
{
	m_ViewMode = mode;
	if (m_ViewStack[m_ViewMode].Last == null)
	{
		b3Original();
	}
	m_Actual   = m_ViewStack[m_ViewMode].Last;
}

b3_bool b3RenderView::b3IsViewMode(b3_view_mode mode)
{
	return m_ViewMode == mode;
}

void b3RenderView::b3SetCamera(b3CameraPart *camera)
{
	m_EyePoint  = camera->m_EyePoint;
	m_ViewPoint = camera->m_ViewPoint;
	m_Width     = camera->m_Width;
	m_Height    = camera->m_Height;
}

void b3RenderView::b3SetCamera(b3Scene *scene)
{
	m_EyePoint  = scene->m_EyePoint;
	m_ViewPoint = scene->m_ViewPoint;
	m_Width     = scene->m_Width;
	m_Height    = scene->m_Height;
}

b3_bool b3RenderView::b3SetBounds(b3Scene *scene)
{
	b3_bool result;

	result = scene->b3ComputeBounds(&m_Lower,&m_Upper);
	if (result)
	{
		b3_index i;

		for (i = 1;i < B3_VIEW_MAX;i++)
		{
			m_ViewStack[i].b3Append(b3NewRenderViewItem());
		}
	}
	return result;
}

void b3RenderView::b3SetBounds(b3_vector *lower,b3_vector *upper)
{
	m_Lower = *lower;
	m_Upper = *upper;
}

b3_bool b3RenderView::b3ViewStackNotEmpty()
{
	b3_bool result = false;

	if (m_ViewMode != B3_VIEW_3D)
	{
		result =
			m_ViewStack[m_ViewMode].First != m_ViewStack[m_ViewMode].Last;
	}
	return result;
}

void b3RenderView::b3PopView()
{
	b3RenderViewItem *item;

	if (m_ViewMode != B3_VIEW_3D)
	{
		item = m_ViewStack[m_ViewMode].Last;
		B3_ASSERT(item != null);
		if (item != m_ViewStack[m_ViewMode].First)
		{
			m_ViewStack[m_ViewMode].b3Remove(item);
			m_Depot.b3Append(item);
			m_Actual = m_ViewStack[m_ViewMode].Last;
		}
	}
}

void b3RenderView::b3Original()
{
	b3RenderViewItem *item;

	if (m_ViewMode != B3_VIEW_3D)
	{
		// Empty stack
		while((item = m_ViewStack[m_ViewMode].First) != null)
		{
			m_ViewStack[m_ViewMode].b3Remove(item);
			m_Depot.b3Append(item);
		}

		// Allocate new top item
		item = b3NewRenderViewItem();
		if (item != null)
		{
			m_ViewStack[m_ViewMode].b3Append(item);
			m_Actual = item;
		}
		else
		{
			b3PrintF(B3LOG_NORMAL,"Not enogh memory for allocating topmost view item.\n");
		}
	}
}

void b3RenderView::b3Scale(b3_f64 scale)
{
	if (m_ViewMode != B3_VIEW_3D)
	{
		B3_ASSERT(m_Actual != null);
		m_Actual->m_Size.x *= scale;
		m_Actual->m_Size.y *= scale;
		m_Actual->m_Size.z *= scale;
	}
}

void b3RenderView::b3Move(b3_f64 xDir,b3_f64 yDir)
{
	if (m_ViewMode != B3_VIEW_3D)
	{
		B3_ASSERT(m_Actual != null);

		xDir /= m_Actual->m_xRelation;
		yDir *= m_Actual->m_yRelation;
		switch(m_ViewMode)
		{
		case B3_VIEW_TOP:
			m_Actual->m_Mid.x += (m_Actual->m_Size.x * xDir);
			m_Actual->m_Mid.y += (m_Actual->m_Size.y * yDir);
			break;
		case B3_VIEW_FRONT:
			m_Actual->m_Mid.x += (m_Actual->m_Size.x * xDir);
			m_Actual->m_Mid.z += (m_Actual->m_Size.z * yDir);
			break;
		case B3_VIEW_RIGHT:
			m_Actual->m_Mid.y += (m_Actual->m_Size.y * xDir);
			m_Actual->m_Mid.z += (m_Actual->m_Size.z * yDir);
			break;
		case B3_VIEW_BACK:
			m_Actual->m_Mid.x -= (m_Actual->m_Size.x * xDir);
			m_Actual->m_Mid.z += (m_Actual->m_Size.z * yDir);
			break;
		case B3_VIEW_LEFT:
			m_Actual->m_Mid.y -= (m_Actual->m_Size.y * xDir);
			m_Actual->m_Mid.z += (m_Actual->m_Size.z * yDir);
			break;
		default:
			break;
		}
	}
}

void b3RenderView::b3GetViewDirection(b3_vector *dir)
{
	switch(m_ViewMode)
	{
	case B3_VIEW_TOP:
		dir->x =  0;
		dir->y =  0;
		dir->z = -1;
		break;
	case B3_VIEW_FRONT:
		dir->x =  0;
		dir->y =  1;
		dir->z =  0;
		break;
	case B3_VIEW_RIGHT:
		dir->x = -1;
		dir->y =  0;
		dir->z =  0;
		break;
	case B3_VIEW_BACK:
		dir->x =  0;
		dir->y = -1;
		dir->z =  0;
		break;
	case B3_VIEW_LEFT:
		dir->x =  1;
		dir->y =  0;
		dir->z =  0;
		break;
	default:
		break;
	}
}

b3_f64 b3RenderView::b3GetPositionAngle(b3_vector *center,b3_vector *position)
{
	b3_f64 result = 0;

	switch(m_ViewMode)
	{
	case B3_VIEW_3D:
		// Walk through!!
	case B3_VIEW_TOP:
		result = atan2(position->y - center->y,position->x - center->x);
		break;
	case B3_VIEW_FRONT:
		result = atan2(position->z - center->z,position->x - center->x);
		break;
	case B3_VIEW_RIGHT:
		result = atan2(position->z - center->z,position->y - center->y);
		break;
	case B3_VIEW_BACK:
		result = atan2(position->z - center->z,center->x - position->x);
		break;
	case B3_VIEW_LEFT:
		result = atan2(position->z - center->z,center->y - position->y);
		break;
	default:
		break;
	}
	return result;
}

void b3RenderView::b3SetTranslationStepper(
	b3_vector      *steps,
	b3_vector      *mover,
	b3_action_mode  mode)
{
	b3Vector::b3Init(mover);
	switch(m_ViewMode)
	{
	case B3_VIEW_TOP:
		switch(mode)
		{
		case B3_ACTION_MOVE_RIGHT:
			mover->x =  steps->x;
			break;
		case B3_ACTION_MOVE_LEFT:
			mover->x = -steps->x;
			break;
		case B3_ACTION_MOVE_UP:
			mover->y =  steps->y;
			break;
		case B3_ACTION_MOVE_DOWN:
			mover->y = -steps->y;
			break;

		default:
			break;
		}
		break;
	case B3_VIEW_FRONT:
		switch(mode)
		{
		case B3_ACTION_MOVE_RIGHT:
			mover->x =  steps->x;
			break;
		case B3_ACTION_MOVE_LEFT:
			mover->x = -steps->x;
			break;
		case B3_ACTION_MOVE_UP:
			mover->z =  steps->z;
			break;
		case B3_ACTION_MOVE_DOWN:
			mover->z = -steps->z;
			break;

		default:
			break;
		}
		break;
	case B3_VIEW_RIGHT:
		switch(mode)
		{
		case B3_ACTION_MOVE_RIGHT:
			mover->y =  steps->y;
			break;
		case B3_ACTION_MOVE_LEFT:
			mover->y = -steps->y;
			break;
		case B3_ACTION_MOVE_UP:
			mover->z =  steps->z;
			break;
		case B3_ACTION_MOVE_DOWN:
			mover->z = -steps->z;
			break;

		default:
			break;
		}
		break;
	case B3_VIEW_BACK:
		switch(mode)
		{
		case B3_ACTION_MOVE_RIGHT:
			mover->x = -steps->x;
			break;
		case B3_ACTION_MOVE_LEFT:
			mover->x =  steps->x;
			break;
		case B3_ACTION_MOVE_UP:
			mover->z =  steps->z;
			break;
		case B3_ACTION_MOVE_DOWN:
			mover->z = -steps->z;
			break;

		default:
			break;
		}
		break;
	case B3_VIEW_LEFT:
		switch(mode)
		{
		case B3_ACTION_MOVE_RIGHT:
			mover->y = -steps->y;
			break;
		case B3_ACTION_MOVE_LEFT:
			mover->y =  steps->y;
			break;
		case B3_ACTION_MOVE_UP:
			mover->z =  steps->z;
			break;
		case B3_ACTION_MOVE_DOWN:
			mover->z = -steps->z;
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
}

b3_f64 b3RenderView::b3SetRotationStepper(
	b3_vector      *steps,
	b3_vector      *axis,
	b3_action_mode  mode)
{
	b3_f64 angle = 0;

	// Prepare rotation axis
	b3Vector::b3Init(axis);
	switch(m_ViewMode)
	{
	case B3_VIEW_3D:
	case B3_VIEW_TOP:
		axis->z =  1;
		angle   = steps->z;
		break;
	case B3_VIEW_FRONT:
		axis->y = -1;
		angle   = steps->y;
		break;
	case B3_VIEW_RIGHT:
		axis->x =  1;
		angle   = steps->x;
		break;
	case B3_VIEW_BACK:
		axis->y =  1;
		angle   = steps->y;
		break;
	case B3_VIEW_LEFT:
		axis->x = -1;
		angle   = steps->x;
		break;

	default:
		break;
	}

	// Define rotation angle
	switch (mode)
	{
	case B3_ACTION_ROT_LEFT:
		return  angle;
	case B3_ACTION_ROT_RIGHT:
		return -angle;

	default:
		break;
	}
	return 0;
}

void b3RenderView::b3Unproject(b3_f64 xRel,b3_f64 yRel,b3_vector *point)
{
	if (m_ViewMode != B3_VIEW_3D)
	{
		xRel = (xRel - 0.5) / m_Actual->m_xRelation;
		yRel = (yRel - 0.5) * m_Actual->m_yRelation;
		switch(m_ViewMode)
		{
		case B3_VIEW_TOP:
			point->x = m_Actual->m_Mid.x + m_Actual->m_Size.x * xRel;
			point->y = m_Actual->m_Mid.y - m_Actual->m_Size.y * yRel;
			break;
		case B3_VIEW_FRONT:
			point->x = m_Actual->m_Mid.x + m_Actual->m_Size.x * xRel;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel;
			break;
		case B3_VIEW_RIGHT:
			point->y = m_Actual->m_Mid.y + m_Actual->m_Size.y * xRel;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel;
			break;
		case B3_VIEW_LEFT:
			point->y = m_Actual->m_Mid.y - m_Actual->m_Size.y * xRel;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel;
			break;
		case B3_VIEW_BACK:
			point->x = m_Actual->m_Mid.x - m_Actual->m_Size.x * xRel;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel;
			break;
		default:
			break;
		}
	}
}

void b3RenderView::b3Select(
	b3_f64 xStart,
	b3_f64 yStart,
	b3_f64 xEnd,
	b3_f64 yEnd)
{
	b3RenderViewItem *item;
	b3_f64            xDiff,yDiff;
	b3_f64            xMove,yMove;

	if (m_ViewMode != B3_VIEW_3D)
	{
		item = b3NewRenderViewItem(m_ViewStack[m_ViewMode].Last);
		if (item != null)
		{
			// Push view item
			m_ViewStack[m_ViewMode].b3Append(item);
			m_Actual = item;

			// Correct input due to aspect ratio
			xStart = (xStart - 0.5) / m_Actual->m_xRelation + 0.5;
			yStart = (yStart - 0.5) * m_Actual->m_yRelation + 0.5;
			xEnd   = (xEnd   - 0.5) / m_Actual->m_xRelation + 0.5;
			yEnd   = (yEnd   - 0.5) * m_Actual->m_yRelation + 0.5;

			// Compute some values...
			xDiff = (xStart < xEnd ? xEnd - xStart : xStart - xEnd);
			yDiff = (yStart < yEnd ? yEnd - yStart : yStart - yEnd);
			xMove = (xStart + xEnd) * 0.5 - 0.5;
			yMove = (yStart + yEnd) * 0.5 - 0.5;

			switch(m_ViewMode)
			{
			case B3_VIEW_TOP:
				m_Actual->m_Mid.x  += (m_Actual->m_Size.x * xMove);
				m_Actual->m_Mid.y  -= (m_Actual->m_Size.y * yMove);
				m_Actual->m_Size.x *= xDiff;
				m_Actual->m_Size.y *= yDiff;
				break;
			case B3_VIEW_FRONT:
				m_Actual->m_Mid.x  += (m_Actual->m_Size.x * xMove);
				m_Actual->m_Mid.z  -= (m_Actual->m_Size.z * yMove);
				m_Actual->m_Size.x *= xDiff;
				m_Actual->m_Size.z *= yDiff;
				break;
			case B3_VIEW_RIGHT:
				m_Actual->m_Mid.y  += (m_Actual->m_Size.y * xMove);
				m_Actual->m_Mid.z  -= (m_Actual->m_Size.z * yMove);
				m_Actual->m_Size.y *= xDiff;
				m_Actual->m_Size.z *= yDiff;
				break;
			case B3_VIEW_BACK:
				m_Actual->m_Mid.x  -= (m_Actual->m_Size.x * xMove);
				m_Actual->m_Mid.z  -= (m_Actual->m_Size.z * yMove);
				m_Actual->m_Size.x *= xDiff;
				m_Actual->m_Size.z *= yDiff;
				break;
			case B3_VIEW_LEFT:
				m_Actual->m_Mid.y  -= (m_Actual->m_Size.y * xMove);
				m_Actual->m_Mid.z  -= (m_Actual->m_Size.z * yMove);
				m_Actual->m_Size.y *= xDiff;
				m_Actual->m_Size.z *= yDiff;
				break;
			default:
				break;
			}
		}
		else
		{
			b3PrintF(B3LOG_NORMAL,"Not enogh memory for allocating new selection.\n");
		}
	}
}

inline b3_f64 b3RenderView::b3ComputeFarClippingPlane()
{
#if 1
	b3_f64    farCP = 1,denom,l;
	b3_vector edge,look,cross;

	b3Vector::b3Sub(&m_ViewPoint,&m_EyePoint,&look);
	b3Vector::b3CrossProduct(&m_Width,&m_Height,&cross);
	denom = b3Vector::b3Length(&look) / (cross.x * look.x + cross.y * look.y + cross.z * look.z);

	b3Vector::b3Sub(&m_Lower,&m_EyePoint,&edge);
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	edge.x = m_Upper.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	edge.y = m_Upper.y - m_EyePoint.y;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	edge.x = m_Lower.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	edge.z = m_Upper.z - m_EyePoint.z;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	edge.x = m_Upper.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	edge.y = m_Lower.y - m_EyePoint.y;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	edge.x = m_Lower.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP) farCP = l;

	return farCP * 2 + 4;
#else
	return 10000;
#endif
}

void b3RenderView::b3UpdateView(
	b3_coord xPos,
	b3_coord yPos,
	b3_res   xSize,
	b3_res   ySize)
{
#ifdef BLZ3_USE_OPENGL
	b3_f64    width,height,nearCP,farCP,distance,factor,relation;
	GLfloat   aspectWindow = (GLfloat)xSize / (GLfloat)ySize;
	GLfloat   aspectCamera;
	GLfloat   min = 0.1f;
	b3_vector eye,look,up;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	B3_ASSERT ((m_Actual != null) || (m_ViewMode == B3_VIEW_3D));
	glViewport(xPos,yPos,xSize,ySize);
	up.x  =
	up.y  =
	up.z  = 0;
	switch (m_ViewMode)
	{
	case B3_VIEW_3D:
	default:
		// Prepare glOrtho();
		distance = b3Vector::b3Distance(&m_ViewPoint,&m_EyePoint);
		factor   = min / distance;
		width    = factor * b3Vector::b3Length(&m_Width);
		height   = factor * b3Vector::b3Length(&m_Height);
		nearCP   = min;
		farCP    = b3ComputeFarClippingPlane();

		// Prepare gluLookAt() - it's simple
		eye  = m_EyePoint;
		look = m_ViewPoint;
		up   = m_Height;
		break;

	case B3_VIEW_TOP:
		// Prepare glOrtho();
		width  = (m_Actual->m_Size.x) * 0.5;
		height = (m_Actual->m_Size.y) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.z - m_Lower.z);

		// Prepare gluLookAt()
		eye    = look = m_Actual->m_Mid;
		eye.z  = m_Upper.z;
		look.z = m_Lower.z;
		up.y   = 1;
		break;

	case B3_VIEW_FRONT:
		// Prepare glOrtho();
		width  = (m_Actual->m_Size.x) * 0.5;
		height = (m_Actual->m_Size.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		eye    = look = m_Actual->m_Mid;
		eye.y  = m_Lower.y;
		look.y = m_Upper.y;
		up.z   = 1;
		break;

	case B3_VIEW_RIGHT:
		// Prepare glOrtho();
		width  = (m_Actual->m_Size.y) * 0.5;
		height = (m_Actual->m_Size.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		eye    = look = m_Actual->m_Mid;
		eye.x  = m_Upper.x;
		look.x = m_Lower.x;
		up.z   = 1;
		break;

	case B3_VIEW_LEFT:
		// Prepare glOrtho();
		width  = (m_Actual->m_Size.y) * 0.5;
		height = (m_Actual->m_Size.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		eye    = look = m_Actual->m_Mid;
		eye.x  = m_Lower.x;
		look.x = m_Upper.x;
		up.z   = 1;
		break;

	case B3_VIEW_BACK:
		// Prepare glOrtho();
		width  = (m_Actual->m_Size.x) * 0.5;
		height = (m_Actual->m_Size.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		eye    = look = m_Actual->m_Mid;
		eye.y  = m_Upper.y;
		look.y = m_Lower.y;
		up.z   = 1;
		break;
	}

	// Maintain aspect ratio
	if (m_AspectRatio)
	{
		aspectCamera = (GLfloat)(width / height);
		relation     = aspectCamera / aspectWindow;
		if (relation > 1)
		{
			if (m_ViewMode != B3_VIEW_3D)
			{
				m_Actual->m_xRelation = 1;
				m_Actual->m_yRelation = relation;
			}
			height *= relation;
		}
		else
		{
			if (m_ViewMode != B3_VIEW_3D)
			{
				m_Actual->m_xRelation = relation;
				m_Actual->m_yRelation = 1;
			}
			width /= relation;
		}
	}

	// Now initialize view
	if (m_ViewMode == B3_VIEW_3D)
	{
		glFrustum(-width,width,-height,height,nearCP,farCP);
	}
	else
	{
		glOrtho(-width,width,-height,height,nearCP,farCP);
	}
	gluLookAt(
		eye.x, eye.y, eye.z,
		look.x,look.y,look.z,
		up.x,  up.y,  up.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	if (m_AntiAliased)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
		glLineWidth(1.2f);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
		glLineWidth(1.0f);
	}

#endif
}
