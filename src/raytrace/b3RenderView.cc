/*
**
**	$Filename:	b3RenderView.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Render view mode handling
**
**	(C) Copyright 2001  Steffen A. Mork
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

#include "b3RaytraceInclude.h"
#include "blz3/raytrace/b3RenderView.h"

/*************************************************************************
**                                                                      **
**                        b3RenderViewItem Implementation               **
**                                                                      **
*************************************************************************/

b3RenderViewItem::b3RenderViewItem() :
	b3Link<b3RenderViewItem>(sizeof(b3RenderViewItem))
{
	m_Mid.x  = m_Mid.y  = m_Mid.z  = 0;
	m_Size.x = m_Size.y = m_Size.z = 0;
}

b3RenderViewItem::b3RenderViewItem(
	b3_vector * lower,
	b3_vector * upper) :
	b3Link<b3RenderViewItem>(sizeof(b3RenderViewItem))
{
	b3Set(lower, upper);
}

void b3RenderViewItem::b3Set(
	b3_vector * lower,
	b3_vector * upper)
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

/*************************************************************************
**                                                                      **
**                        b3RenderView Implementation                   **
**                                                                      **
*************************************************************************/

b3RenderView::b3RenderView()
{
	m_ViewMode    = B3_VIEW_3D;
	m_AntiAliased = false;
	m_AspectRatio = true;
	m_xRes        = 0;
	m_yRes        = 0;
	m_Actual      = nullptr;
}

b3RenderView::~b3RenderView()
{
	b3_index i;

	for (i = 1; i < B3_VIEW_MAX; i++)
	{
		m_ViewStack[i].b3Free();
	}

	m_Depot.b3Free();
}

b3RenderViewItem * b3RenderView::b3NewRenderViewItem(b3RenderViewItem * last)
{
	b3RenderViewItem * item;

	item = m_Depot.First;
	if (item != nullptr)
	{
		// Recycle item
		m_Depot.b3Remove(item);
	}
	else
	{
		// Create new item
		item = new b3RenderViewItem();
	}

	if (last != nullptr)
	{
		// Use dimension of last item
		item->m_Mid  = last->m_Mid;
		item->m_Size = last->m_Size;
		item->m_xRelation = last->m_xRelation;
		item->m_yRelation = last->m_yRelation;
	}
	else
	{
		// Use dimension of complete scene
		item->b3Set(&m_Lower, &m_Upper);
	}
	return item;
}

void b3RenderView::b3SetViewMode(b3_view_mode mode)
{
	m_ViewMode = mode;
	if (m_ViewStack[m_ViewMode].Last == nullptr)
	{
		b3Original();
	}
	m_Actual   = m_ViewStack[m_ViewMode].Last;
}

b3_bool b3RenderView::b3IsViewMode(b3_view_mode mode) const
{
	return m_ViewMode == mode;
}

void b3RenderView::b3SetCamera(b3CameraPart * camera)
{
	m_EyePoint  = camera->m_EyePoint;
	m_ViewPoint = camera->m_ViewPoint;
	m_Width     = camera->m_Width;
	m_Height    = camera->m_Height;
}

void b3RenderView::b3SetCamera(b3Scene * scene)
{
	m_EyePoint  = scene->m_EyePoint;
	m_ViewPoint = scene->m_ViewPoint;
	m_Width     = scene->m_Width;
	m_Height    = scene->m_Height;
}

b3_bool b3RenderView::b3SetBounds(b3Scene * scene)
{
	b3_bool result;

	result = scene->b3ComputeBounds(&m_Lower, &m_Upper);
	if (result)
	{
		b3_index i;

		for (i = 1; i < B3_VIEW_MAX; i++)
		{
			m_ViewStack[i].b3Append(b3NewRenderViewItem());
		}
	}
	return result;
}

b3_bool b3RenderView::b3GetDimension(b3_f64 & xSize, b3_f64 & ySize) const
{
	b3_bool success = true;

	switch (m_ViewMode)
	{
	case B3_VIEW_3D:
		xSize = 2.0 * b3Vector::b3Length(&m_Width);
		ySize = 2.0 * b3Vector::b3Length(&m_Height);
		break;
	case B3_VIEW_TOP:
		xSize = m_Upper.x - m_Lower.x;
		ySize = m_Upper.y - m_Lower.y;
		break;
	case B3_VIEW_FRONT:
	case B3_VIEW_BACK:
		xSize = m_Upper.x - m_Lower.x;
		ySize = m_Upper.z - m_Lower.z;
		break;
	case B3_VIEW_RIGHT:
	case B3_VIEW_LEFT:
		xSize = m_Upper.y - m_Lower.y;
		ySize = m_Upper.z - m_Lower.z;
		break;

	default:
		success = false;
	}
	return success;
}

b3_bool b3RenderView::b3GetView(b3_view_info & view_info) const
{
	view_info.m_ViewMode = m_ViewMode;

	switch (m_ViewMode)
	{
	default:
		return false;

	case B3_VIEW_TOP:
		view_info.m_Scene.left   = m_Lower.x;
		view_info.m_Scene.right  = m_Upper.x;
		view_info.m_Scene.bottom = m_Lower.y;
		view_info.m_Scene.top    = m_Upper.y;
		view_info.m_View.left    = m_Actual->m_Mid.x - 0.5 * m_Actual->m_Size.x;
		view_info.m_View.right   = m_Actual->m_Mid.x + 0.5 * m_Actual->m_Size.x;
		view_info.m_View.bottom  = m_Actual->m_Mid.y - 0.5 * m_Actual->m_Size.y;
		view_info.m_View.top     = m_Actual->m_Mid.y + 0.5 * m_Actual->m_Size.y;
		break;

	case B3_VIEW_FRONT:
	case B3_VIEW_BACK:
		view_info.m_Scene.left   = m_Lower.x;
		view_info.m_Scene.right  = m_Upper.x;
		view_info.m_Scene.bottom = m_Lower.z;
		view_info.m_Scene.top    = m_Upper.z;
		view_info.m_View.left    = m_Actual->m_Mid.x - 0.5 * m_Actual->m_Size.x;
		view_info.m_View.right   = m_Actual->m_Mid.x + 0.5 * m_Actual->m_Size.x;
		view_info.m_View.bottom  = m_Actual->m_Mid.z - 0.5 * m_Actual->m_Size.z;
		view_info.m_View.top     = m_Actual->m_Mid.z + 0.5 * m_Actual->m_Size.z;
		break;

	case B3_VIEW_RIGHT:
	case B3_VIEW_LEFT:
		view_info.m_Scene.left   = m_Lower.y;
		view_info.m_Scene.right  = m_Upper.y;
		view_info.m_Scene.bottom = m_Lower.z;
		view_info.m_Scene.top    = m_Upper.z;
		view_info.m_View.left    = m_Actual->m_Mid.y - 0.5 * m_Actual->m_Size.y;
		view_info.m_View.right   = m_Actual->m_Mid.y + 0.5 * m_Actual->m_Size.y;
		view_info.m_View.bottom  = m_Actual->m_Mid.z - 0.5 * m_Actual->m_Size.z;
		view_info.m_View.top     = m_Actual->m_Mid.z + 0.5 * m_Actual->m_Size.z;
		break;
	}
	view_info.m_AspectRatio =
		(view_info.m_View.top   - view_info.m_View.bottom) /
		(view_info.m_View.right - view_info.m_View.left);
	return true;
}

void b3RenderView::b3SetBounds(const b3_vector * lower, const b3_vector * upper)
{
	m_Lower = *lower;
	m_Upper = *upper;
}

b3_bool b3RenderView::b3ViewStackNotEmpty() const
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
	b3RenderViewItem * item;

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
	b3RenderViewItem * item;

	if (m_ViewMode != B3_VIEW_3D)
	{
		// Empty stack
		m_Depot.b3Move(&m_ViewStack[m_ViewMode]);

		// Allocate new top item
		item = b3NewRenderViewItem();
		if (item != nullptr)
		{
			m_ViewStack[m_ViewMode].b3Append(item);
			m_Actual = item;
		}
		else
		{
			b3PrintF(B3LOG_NORMAL, "Not enough memory for allocating topmost view item.\n");
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

void b3RenderView::b3Move(b3_f64 xDir, b3_f64 yDir)
{
	if (m_ViewMode != B3_VIEW_3D)
	{
		B3_ASSERT(m_Actual != null);

		xDir /= m_Actual->m_xRelation;
		yDir *= m_Actual->m_yRelation;
		switch (m_ViewMode)
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

void b3RenderView::b3GetProjectionBase(b3_vector * eye) const
{
	*eye = m_ViewInfo.eye;
}

void b3RenderView::b3GetViewDirection(b3_vector * dir) const
{
	switch (m_ViewMode)
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

b3_f64 b3RenderView::b3GetPositionAngle(
	b3_vector * center,
	b3_vector * position) const
{
	b3_f64 result = 0;

	switch (m_ViewMode)
	{
	case B3_VIEW_3D:
	// Walk through!!
	case B3_VIEW_TOP:
		result = atan2(position->y - center->y, position->x - center->x);
		break;
	case B3_VIEW_FRONT:
		result = atan2(position->z - center->z, position->x - center->x);
		break;
	case B3_VIEW_RIGHT:
		result = atan2(position->z - center->z, position->y - center->y);
		break;
	case B3_VIEW_BACK:
		result = atan2(position->z - center->z, center->x - position->x);
		break;
	case B3_VIEW_LEFT:
		result = atan2(position->z - center->z, center->y - position->y);
		break;
	default:
		break;
	}
	return result;
}

void b3RenderView::b3SetTranslationStepper(
	b3_vector   *   steps,
	b3_vector   *   mover,
	b3_action_mode  mode)
{
	b3Vector::b3Init(mover);
	switch (m_ViewMode)
	{
	case B3_VIEW_TOP:
		switch (mode)
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
		switch (mode)
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
		switch (mode)
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
		switch (mode)
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
		switch (mode)
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
	b3_vector   *   steps,
	b3_vector   *   axis,
	b3_action_mode  mode)
{
	b3_f64 angle = 0;

	// Prepare rotation axis
	b3Vector::b3Init(axis);
	switch (m_ViewMode)
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

void b3RenderView::b3Project(
	const b3_vector * point,
	b3_f64      &     xRel,
	b3_f64      &     yRel,
	b3_f64      &     zRel) const
{
#ifdef BLZ3_USE_OPENGL
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluProject(point->x, point->y, point->z, modelview, projection, viewport, &winX, &winY, &winZ);
	xRel =       winX / m_xRes;
	yRel = 1.0 - winY / m_yRes;
	zRel = winZ;
#endif
}

void b3RenderView::b3UnprojectInternal(
	const b3_f64 x,
	const b3_f64 y,
	const b3_f64 z, b3_vector * point) const
{
#ifdef BLZ3_USE_OPENGL
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluUnProject((GLfloat)x, (GLfloat)y, (GLfloat)z, modelview, projection, viewport, &posX, &posY, &posZ);

	point->x = (b3_f32)posX;
	point->y = (b3_f32)posY;
	point->z = (b3_f32)posZ;
#endif
}

void b3RenderView::b3Select(
	b3_f64 xStart,
	b3_f64 yStart,
	b3_f64 xEnd,
	b3_f64 yEnd)
{
	b3RenderViewItem * item;
	b3_f64            xDiff, yDiff;
	b3_f64            xMove, yMove;

	if (m_ViewMode != B3_VIEW_3D)
	{
		item = b3NewRenderViewItem(m_ViewStack[m_ViewMode].Last);
		if (item != nullptr)
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

			switch (m_ViewMode)
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
			b3PrintF(B3LOG_NORMAL, "Not enogh memory for allocating new selection.\n");
		}
	}
}

inline b3_f64 b3RenderView::b3ComputeFarClippingPlane() const
{
#if 1
	b3_f64    farCP = 1, denom, l;
	b3_vector edge, look, cross;

	B3_ASSERT(b3Vector::b3Distance(&m_Lower, &m_Upper) > b3Scene::epsilon);
	b3Vector::b3Sub(&m_ViewPoint, &m_EyePoint, &look);
	b3Vector::b3CrossProduct(&m_Width, &m_Height, &cross);
	denom = b3Vector::b3Length(&look) / (cross.x * look.x + cross.y * look.y + cross.z * look.z);

	b3Vector::b3Sub(&m_Lower, &m_EyePoint, &edge);
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	edge.x = m_Upper.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	edge.y = m_Upper.y - m_EyePoint.y;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	edge.x = m_Lower.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	edge.z = m_Upper.z - m_EyePoint.z;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	edge.x = m_Upper.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	edge.y = m_Lower.y - m_EyePoint.y;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	edge.x = m_Lower.x - m_EyePoint.x;
	l      = (cross.x * edge.x + cross.y * edge.y + cross.z * edge.z) * denom;
	if (l > farCP)
	{
		farCP = l;
	}

	return farCP; // * 2 + 4;
#else
	return 10000;
#endif
}

void b3RenderView::b3SetupView(
	b3_res   xSize,
	b3_res   ySize,
	b3_f64   xOffset,
	b3_f64   yOffset)
{
#ifdef BLZ3_USE_OPENGL
	b3_f64    distance, factor, relation;
	GLfloat   aspectWindow = (GLfloat)xSize / (GLfloat)ySize;
	GLfloat   aspectCamera;
	GLfloat   min = 0.1f;

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, ">b3RenderView::b3SetupView() # %d\n", m_ViewMode);
#endif

	m_xRes = xSize;
	m_yRes = ySize;
	B3_ASSERT((m_Actual != null) || (m_ViewMode == B3_VIEW_3D));
	glViewport(0, 0, xSize, ySize);
	b3Vector::b3Init(&m_ViewInfo.up);
	switch (m_ViewMode)
	{
	case B3_VIEW_3D:
	default:
		// Prepare glOrtho();
		distance   = b3Vector::b3Distance(&m_ViewPoint, &m_EyePoint);
		factor     = min / distance;
		m_ViewInfo.width   = factor * b3Vector::b3Length(&m_Width);
		m_ViewInfo.height  = factor * b3Vector::b3Length(&m_Height);
		m_ViewInfo.near_cp = min;
		m_ViewInfo.far_cp  = b3ComputeFarClippingPlane();

		// Prepare gluLookAt() - it's simple
		m_ViewInfo.eye  = m_EyePoint;
		m_ViewInfo.look = m_ViewPoint;
		m_ViewInfo.up   = m_Height;

		// Init Offset
		b3Vector::b3Init(&m_ViewInfo.offset);
		break;

	case B3_VIEW_TOP:
		// Prepare glOrtho();
		m_ViewInfo.width   = (m_Actual->m_Size.x) * 0.5;
		m_ViewInfo.height  = (m_Actual->m_Size.y) * 0.5;
		m_ViewInfo.near_cp = 0;
		m_ViewInfo.far_cp  = (m_Upper.z - m_Lower.z);

		// Prepare gluLookAt()
		m_ViewInfo.eye     = m_ViewInfo.look = m_Actual->m_Mid;
		m_ViewInfo.eye.z   = m_Upper.z;
		m_ViewInfo.look.z  = m_Lower.z;
		m_ViewInfo.up.y    = 1;

		// Prepare offset for print tiling
		m_ViewInfo.offset.x = -xOffset;
		m_ViewInfo.offset.y = -yOffset;
		m_ViewInfo.offset.z =  0;
		break;

	case B3_VIEW_FRONT:
		// Prepare glOrtho();
		m_ViewInfo.width   = (m_Actual->m_Size.x) * 0.5;
		m_ViewInfo.height  = (m_Actual->m_Size.z) * 0.5;
		m_ViewInfo.near_cp = 0;
		m_ViewInfo.far_cp  = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		m_ViewInfo.eye    = m_ViewInfo.look = m_Actual->m_Mid;
		m_ViewInfo.eye.y  = m_Lower.y;
		m_ViewInfo.look.y = m_Upper.y;
		m_ViewInfo.up.z   = 1;

		// Prepare offset for print tiling
		m_ViewInfo.offset.x = -xOffset;
		m_ViewInfo.offset.y =  0;
		m_ViewInfo.offset.z = -yOffset;
		break;

	case B3_VIEW_RIGHT:
		// Prepare glOrtho();
		m_ViewInfo.width   = (m_Actual->m_Size.y) * 0.5;
		m_ViewInfo.height  = (m_Actual->m_Size.z) * 0.5;
		m_ViewInfo.near_cp = 0;
		m_ViewInfo.far_cp  = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		m_ViewInfo.eye    = m_ViewInfo.look = m_Actual->m_Mid;
		m_ViewInfo.eye.x  = m_Upper.x;
		m_ViewInfo.look.x = m_Lower.x;
		m_ViewInfo.up.z   = 1;

		// Prepare offset for print tiling
		m_ViewInfo.offset.x =  0;
		m_ViewInfo.offset.y = -xOffset;
		m_ViewInfo.offset.z = -yOffset;
		break;

	case B3_VIEW_LEFT:
		// Prepare glOrtho();
		m_ViewInfo.width   = (m_Actual->m_Size.y) * 0.5;
		m_ViewInfo.height  = (m_Actual->m_Size.z) * 0.5;
		m_ViewInfo.near_cp = 0;
		m_ViewInfo.far_cp  = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		m_ViewInfo.eye    = m_ViewInfo.look = m_Actual->m_Mid;
		m_ViewInfo.eye.x  = m_Lower.x;
		m_ViewInfo.look.x = m_Upper.x;
		m_ViewInfo.up.z   = 1;

		// Prepare offset for print tiling
		m_ViewInfo.offset.x =  0;
		m_ViewInfo.offset.y = -xOffset;
		m_ViewInfo.offset.z = -yOffset;
		break;

	case B3_VIEW_BACK:
		// Prepare glOrtho();
		m_ViewInfo.width   = (m_Actual->m_Size.x) * 0.5;
		m_ViewInfo.height  = (m_Actual->m_Size.z) * 0.5;
		m_ViewInfo.near_cp = 0;
		m_ViewInfo.far_cp  = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		m_ViewInfo.eye    = m_ViewInfo.look = m_Actual->m_Mid;
		m_ViewInfo.eye.y  = m_Upper.y;
		m_ViewInfo.look.y = m_Lower.y;
		m_ViewInfo.up.z   = 1;

		// Prepare offset for print tiling
		m_ViewInfo.offset.x =  xOffset;
		m_ViewInfo.offset.y =  0;
		m_ViewInfo.offset.z = -yOffset;
		break;
	}

	// Maintain aspect ratio
	if (m_AspectRatio)
	{
		aspectCamera = (GLfloat)(m_ViewInfo.width / m_ViewInfo.height);
		relation     = aspectCamera / aspectWindow;
		if (relation > 1)
		{
			if (m_ViewMode != B3_VIEW_3D)
			{
				m_Actual->m_xRelation = 1;
				m_Actual->m_yRelation = relation;
			}
			m_ViewInfo.height *= relation;
		}
		else
		{
			if (m_ViewMode != B3_VIEW_3D)
			{
				m_Actual->m_xRelation = relation;
				m_Actual->m_yRelation = 1;
			}
			m_ViewInfo.width /= relation;
		}
	}

	m_ViewInfo.perspective = m_ViewMode == B3_VIEW_3D;

	// Now initialize view
	b3RenderContext::b3ViewSet(&m_ViewInfo);
	b3RenderContext::b3SetAntiAliasing(m_AntiAliased);

#ifdef _DEBUG
	b3PrintF(B3LOG_FULL, "<b3RenderView::b3SetupView()\n");
#endif
#endif
}

#define B3_RASTER_FLOOR(v,grid)   (floor((v) / (grid)) * (grid))
#define B3_RASTER_CEIL(v,grid)    (ceil((v) / (grid)) * (grid))
#define B3_RASTER_COUNT(a,e,grid) ((b3_count)(floor((e) / (grid)) - ceil((a) / (grid)) + 1))
#define B3_RASTER_MINDIST  8

void b3RenderView::b3DrawRaster(
	const b3_f64    grid,
	const b3Color & color) const
{
#ifdef BLZ3_USE_OPENGL
	b3_vector xDisp, yDisp;
	b3_vector a, b, c, d;
	b3_f64    xStart, xEnd;
	b3_f64    yStart, yEnd;
	b3_f64    depth;
	b3_count  x, y, xCount, yCount;
	b3_f64    aux = B3_RASTER_FLOOR(m_ViewInfo.look.x, grid);

	if (m_Actual == nullptr)
	{
		return;
	}

	b3Vector::b3Init(&xDisp);
	b3Vector::b3Init(&yDisp);
	switch (m_ViewMode)
	{
	case B3_VIEW_TOP:
		xDisp.x = grid;
		yDisp.y = grid;
		xStart  = m_ViewInfo.look.x - m_ViewInfo.width;
		xEnd    = m_ViewInfo.look.x + m_ViewInfo.width;
		yStart  = m_ViewInfo.look.y - m_ViewInfo.height;
		yEnd    = m_ViewInfo.look.y + m_ViewInfo.height;
		depth   = B3_RASTER_CEIL(m_ViewInfo.look.z, grid);

		// horizontal raster
		b3Vector::b3Init(&a, B3_RASTER_CEIL(xStart, grid), yStart, depth);
		b3Vector::b3Init(&b, B3_RASTER_CEIL(xStart, grid), yEnd,  depth);

		// vertical raster
		b3Vector::b3Init(&c, xStart, B3_RASTER_CEIL(yStart, grid), depth);
		b3Vector::b3Init(&d, xEnd,  B3_RASTER_CEIL(yStart, grid), depth);
		break;

	case B3_VIEW_FRONT:
		xDisp.x = grid;
		yDisp.z = grid;
		xStart  = m_ViewInfo.look.x - m_ViewInfo.width;
		xEnd    = m_ViewInfo.look.x + m_ViewInfo.width;
		yStart  = m_ViewInfo.look.z - m_ViewInfo.height;
		yEnd    = m_ViewInfo.look.z + m_ViewInfo.height;
		depth   = B3_RASTER_FLOOR(m_ViewInfo.look.y, grid);

		// horizontal raster
		b3Vector::b3Init(&a, B3_RASTER_CEIL(xStart, grid), depth, yStart);
		b3Vector::b3Init(&b, B3_RASTER_CEIL(xStart, grid), depth, yEnd);

		// vertical raster
		b3Vector::b3Init(&c, xStart, depth, B3_RASTER_CEIL(yStart, grid));
		b3Vector::b3Init(&d, xEnd,  depth, B3_RASTER_CEIL(yStart, grid));
		break;

	case B3_VIEW_BACK:
		xDisp.x = grid;
		yDisp.z = grid;
		xStart  = m_ViewInfo.look.x - m_ViewInfo.width;
		xEnd    = m_ViewInfo.look.x + m_ViewInfo.width;
		yStart  = m_ViewInfo.look.z - m_ViewInfo.height;
		yEnd    = m_ViewInfo.look.z + m_ViewInfo.height;
		depth   = B3_RASTER_CEIL(m_ViewInfo.look.y, grid);

		// horizontal raster
		b3Vector::b3Init(&a, B3_RASTER_CEIL(xStart, grid), depth, yStart);
		b3Vector::b3Init(&b, B3_RASTER_CEIL(xStart, grid), depth, yEnd);

		// vertical raster
		b3Vector::b3Init(&c, xStart, depth, B3_RASTER_CEIL(yStart, grid));
		b3Vector::b3Init(&d, xEnd,  depth, B3_RASTER_CEIL(yStart, grid));
		break;

	case B3_VIEW_RIGHT:
		xDisp.y = grid;
		yDisp.z = grid;
		xStart  = m_ViewInfo.look.y - m_ViewInfo.width;
		xEnd    = m_ViewInfo.look.y + m_ViewInfo.width;
		yStart  = m_ViewInfo.look.z - m_ViewInfo.height;
		yEnd    = m_ViewInfo.look.z + m_ViewInfo.height;
		depth   = B3_RASTER_CEIL(m_ViewInfo.look.x, grid);

		// horizontal raster
		b3Vector::b3Init(&a, depth, B3_RASTER_CEIL(xStart, grid), yStart);
		b3Vector::b3Init(&b, depth, B3_RASTER_CEIL(xStart, grid), yEnd);

		// vertical raster
		b3Vector::b3Init(&c, depth, xStart, B3_RASTER_CEIL(yStart, grid));
		b3Vector::b3Init(&d, depth, xEnd,  B3_RASTER_CEIL(yStart, grid));
		break;

	case B3_VIEW_LEFT:
		xDisp.y = grid;
		yDisp.z = grid;
		xStart  = m_ViewInfo.look.y - m_ViewInfo.width;
		xEnd    = m_ViewInfo.look.y + m_ViewInfo.width;
		yStart  = m_ViewInfo.look.z - m_ViewInfo.height;
		yEnd    = m_ViewInfo.look.z + m_ViewInfo.height;
		depth   = aux; // This is due to a internal compiler problem.

		// horizontal raster
		b3Vector::b3Init(&a, depth, B3_RASTER_CEIL(xStart, grid), yStart);
		b3Vector::b3Init(&b, depth, B3_RASTER_CEIL(xStart, grid), yEnd);

		// vertical raster
		b3Vector::b3Init(&c, depth, xStart, B3_RASTER_CEIL(yStart, grid));
		b3Vector::b3Init(&d, depth, xEnd,  B3_RASTER_CEIL(yStart, grid));
		break;

	default:
		return;
	}

	xCount  = B3_RASTER_COUNT(xStart, xEnd, grid);
	yCount  = B3_RASTER_COUNT(yStart, yEnd, grid);
	if ((xCount == 0 ? true : (m_xRes / xCount) >= B3_RASTER_MINDIST) &&
		(yCount == 0 ? true : (m_yRes / yCount) >= B3_RASTER_MINDIST))
	{
		glBegin(GL_LINES);
		glColor3f(color[b3Color::R], color[b3Color::G], color[b3Color::B]);
		for (x = 0; x < xCount; x++)
		{
			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
			b3Vector::b3Add(&xDisp, &a);
			b3Vector::b3Add(&xDisp, &b);
		}
		for (y = 0; y < yCount; y++)
		{
			glVertex3f(c.x, c.y, c.z);
			glVertex3f(d.x, d.y, d.z);
			b3Vector::b3Add(&yDisp, &c);
			b3Vector::b3Add(&yDisp, &d);
		}
		glEnd();
	}
#endif
}
