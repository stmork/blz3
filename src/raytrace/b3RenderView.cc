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

#include "blz3/raytrace/b3RenderView.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.41  2004/09/23 16:05:28  sm
**	- Some BLZ3_USE_OPENGL caveats removed.
**
**	Revision 1.40  2004/09/19 15:36:18  sm
**	- Changed polygon/grid index data type from short (Hey! Are we
**	  on Windows 3.11???) to long.
**	
**	Revision 1.39  2004/08/19 19:25:55  sm
**	- Fixed ticket no. 7. The perspective unprojection is
**	  correct now.
**	
**	Revision 1.38  2004/08/19 10:12:23  sm
**	- Test if one animation run is enough - but is not.
**	- Using gluUnProject for ticket no. 7.
**	
**	Revision 1.37  2004/04/17 09:40:55  sm
**	- Splitting b3Raytrace.h into their components for
**	  better oversightment.
**	
**	Revision 1.36  2004/04/03 14:07:18  sm
**	- Resolved internal compiler error problem of VC++
**	
**	Revision 1.35  2003/03/04 20:37:38  sm
**	- Introducing new b3Color which brings some
**	  performance!
**	
**	Revision 1.34  2003/02/27 19:39:05  sm
**	- Added two grid colors for configuration.
**	- Beautified properties dialog.
**	
**	Revision 1.33  2003/02/22 17:21:34  sm
**	- Changed some global variables into static class members:
**	  o b3Scene::epsilon
**	  o b3Scene::m_TexturePool et. al.
**	  o b3SplineTemplate<class VECTOR>::bspline_errno
**	
**	Revision 1.32  2002/08/21 10:16:40  sm
**	- Made some changes to the Un*x OpenGL renderer:
**	  o Added animations
**	  o Added camera switching
**	  o Corrected far clipping plane computation.
**	- Configure script tidied up.
**	
**	Revision 1.31  2002/08/03 18:05:10  sm
**	- Cleaning up BL3_USE_OPENGL for linux/m68k without OpenGL
**	- Moved b3PrepareInfo into b3Scene class as member. This
**	  saves memory allocation calls and is an investment into
**	  faster Lines III object transformation.
**	
**	Revision 1.30  2002/02/17 21:58:11  sm
**	- Done UnCR
**	- Modified makefiles
**	
**	Revision 1.29  2002/02/17 21:25:07  sm
**	- Introduced CSG
**	  o Heavily reorganized shape inheritance
**	  o New file b3CSGShape added
**	
**	Revision 1.28  2002/02/14 16:51:49  sm
**	- Done some cleanups concernig doubly linked lists. Moved
**	  obsolete b3Node/b3Head into their own include.
**	
**	Revision 1.27  2002/02/14 16:32:33  sm
**	- Added activation via mouse selection
**	
**	Revision 1.26  2002/02/12 18:39:03  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.25  2002/02/10 20:03:19  sm
**	- Added grid raster
**	- Changed icon colors of shapes
**	
**	Revision 1.24  2002/02/03 21:42:30  sm
**	- Added measurement printing. The measure itself is missing yet.
**	  The support is done in b3RenderView and CAppRenderView.
**	- Added support for units in b3ModellerInfo
**	- Cleaned up some accelerators. Now arrow keys are working
**	  again. The del key is working correctly inside edit controls again.
**	
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
**                        b3RenderViewItem Implementation               **
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
	m_Actual      = null;
}

b3RenderView::~b3RenderView()
{
	b3_index i;

	for (i = 1;i < B3_VIEW_MAX;i++)
	{
		m_ViewStack[i].b3Free();
	}

	m_Depot.b3Free();
}

b3RenderViewItem *b3RenderView::b3NewRenderViewItem(b3RenderViewItem *last)
{
	b3RenderViewItem *item;

	item = m_Depot.First;
	if (item != null)
	{
		// Recycle item
		m_Depot.b3Remove(item);
	}
	else
	{
		// Create new item
		item = new b3RenderViewItem();
	}

	if(last != null)
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

b3_bool b3RenderView::b3GetDimension(b3_f64 &xSize,b3_f64 &ySize)
{
	b3_bool success = true;

	switch(m_ViewMode)
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

void b3RenderView::b3GetProjectionBase(b3_vector *eye)
{
	*eye = m_vvEye;
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

void b3RenderView::b3Project(
	const b3_vector *point,
	      b3_coord  &x,
		  b3_coord  &y)
{
	b3_f64 xRel,yRel;

	b3Project(point,xRel,yRel);
	x = (b3_coord)floor(xRel * m_xRes + 0.5);
	y = (b3_coord)floor(yRel * m_yRes + 0.5);
}

void b3RenderView::b3Project(
	const b3_vector *point,
	      b3_f64    &xRel,
		  b3_f64    &yRel)
{
	if (m_ViewMode != B3_VIEW_3D)
	{
		switch(m_ViewMode)
		{
		case B3_VIEW_TOP:
			xRel =  (point->x - m_Actual->m_Mid.x - m_vvOffset.x) / m_Actual->m_Size.x;
			yRel = -(point->y - m_Actual->m_Mid.y - m_vvOffset.y) / m_Actual->m_Size.y;
			break;								                
		case B3_VIEW_FRONT:						                
			xRel =  (point->x - m_Actual->m_Mid.x - m_vvOffset.x) / m_Actual->m_Size.x;
			yRel = -(point->z - m_Actual->m_Mid.z - m_vvOffset.z) / m_Actual->m_Size.z;
			break;								                
		case B3_VIEW_RIGHT:						                
			xRel =  (point->y - m_Actual->m_Mid.y - m_vvOffset.y) / m_Actual->m_Size.y;
			yRel = -(point->z - m_Actual->m_Mid.z - m_vvOffset.z) / m_Actual->m_Size.z;
			break;								                
		case B3_VIEW_LEFT:						                
			xRel = -(point->y - m_Actual->m_Mid.y - m_vvOffset.y) / m_Actual->m_Size.y;
			yRel = -(point->z - m_Actual->m_Mid.z - m_vvOffset.z) / m_Actual->m_Size.z;
			break;								                
		case B3_VIEW_BACK:						                
			xRel = -(point->x - m_Actual->m_Mid.x - m_vvOffset.x) / m_Actual->m_Size.x;
			yRel = -(point->z - m_Actual->m_Mid.z - m_vvOffset.z) / m_Actual->m_Size.z;
			break;
		default:
			break;
		}
		xRel = xRel * m_Actual->m_xRelation + 0.5;
		yRel = yRel / m_Actual->m_yRelation + 0.5;
	}
}

void b3RenderView::b3Unproject(const b3_coord x,const b3_coord y,b3_vector *point)
{
#ifdef BLZ3_USE_OPENGL
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat  winX, winY, winZ;
	GLdouble posX, posY, posZ;
 
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
 
	winX = (GLfloat)x;
	winY = (GLfloat)viewport[3] - (GLfloat)y;

	// We need simply a reference point for generating a ray.
	winZ = 0.5;

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
	point->x = (b3_f32)posX;
	point->y = (b3_f32)posY;
	point->z = (b3_f32)posZ;
#endif
}

void b3RenderView::b3Unproject(const b3_f64 xRelParam,const b3_f64 yRelParam,b3_vector *point)
{
	if (m_ViewMode != B3_VIEW_3D)
	{
		b3_f64 xRel = (xRelParam - 0.5) / m_Actual->m_xRelation;
		b3_f64 yRel = (yRelParam - 0.5) * m_Actual->m_yRelation;
		
		switch(m_ViewMode)
		{
		case B3_VIEW_TOP:
			point->x = m_Actual->m_Mid.x + m_Actual->m_Size.x * xRel + m_vvOffset.x;
			point->y = m_Actual->m_Mid.y - m_Actual->m_Size.y * yRel + m_vvOffset.y;
			break;
		case B3_VIEW_FRONT:
			point->x = m_Actual->m_Mid.x + m_Actual->m_Size.x * xRel + m_vvOffset.x;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel + m_vvOffset.z;
			break;
		case B3_VIEW_RIGHT:
			point->y = m_Actual->m_Mid.y + m_Actual->m_Size.y * xRel + m_vvOffset.y;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel + m_vvOffset.z;
			break;
		case B3_VIEW_LEFT:
			point->y = m_Actual->m_Mid.y - m_Actual->m_Size.y * xRel + m_vvOffset.y;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel + m_vvOffset.z;
			break;
		case B3_VIEW_BACK:
			point->x = m_Actual->m_Mid.x - m_Actual->m_Size.x * xRel + m_vvOffset.x;
			point->z = m_Actual->m_Mid.z - m_Actual->m_Size.z * yRel + m_vvOffset.z;
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

	B3_ASSERT(b3Vector::b3Distance(&m_Lower,&m_Upper) > b3Scene::epsilon);
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

 	return farCP;// * 2 + 4;
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
	b3_f64    nearCP,farCP,distance,factor,relation;
	GLfloat   aspectWindow = (GLfloat)xSize / (GLfloat)ySize;
	GLfloat   aspectCamera;
	GLfloat   min = 0.1f;
	b3_vector up;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	m_xRes = xSize;
	m_yRes = ySize;
	B3_ASSERT ((m_Actual != null) || (m_ViewMode == B3_VIEW_3D));
	glViewport(0,0,xSize,ySize);
	b3Vector::b3Init(&up);
	switch (m_ViewMode)
	{
	case B3_VIEW_3D:
	default:
		// Prepare glOrtho();
		distance   = b3Vector::b3Distance(&m_ViewPoint,&m_EyePoint);
		factor     = min / distance;
		m_vvWidth  = factor * b3Vector::b3Length(&m_Width);
		m_vvHeight = factor * b3Vector::b3Length(&m_Height);
		nearCP     = min;
		farCP      = b3ComputeFarClippingPlane();

		// Prepare gluLookAt() - it's simple
		m_vvEye  = m_EyePoint;
		m_vvLook = m_ViewPoint;
		up       = m_Height;

		// Init Offset
		b3Vector::b3Init(&m_vvOffset);
		break;

	case B3_VIEW_TOP:
		// Prepare glOrtho();
		m_vvWidth  = (m_Actual->m_Size.x) * 0.5;
		m_vvHeight = (m_Actual->m_Size.y) * 0.5;
		nearCP     = 0;
		farCP      = (m_Upper.z - m_Lower.z);

		// Prepare gluLookAt()
		m_vvEye     = m_vvLook = m_Actual->m_Mid;
		m_vvEye.z   = m_Upper.z;
		m_vvLook.z  = m_Lower.z;
		up.y        = 1;

		// Prepare offset for print tiling
		m_vvOffset.x = -xOffset;
		m_vvOffset.y = -yOffset;
		m_vvOffset.z =  0;
		break;

	case B3_VIEW_FRONT:
		// Prepare glOrtho();
		m_vvWidth  = (m_Actual->m_Size.x) * 0.5;
		m_vvHeight = (m_Actual->m_Size.z) * 0.5;
		nearCP     = 0;
		farCP      = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		m_vvEye    = m_vvLook = m_Actual->m_Mid;
		m_vvEye.y  = m_Lower.y;
		m_vvLook.y = m_Upper.y;
		up.z       = 1;

		// Prepare offset for print tiling
		m_vvOffset.x = -xOffset;
		m_vvOffset.y =  0;
		m_vvOffset.z = -yOffset;
		break;

	case B3_VIEW_RIGHT:
		// Prepare glOrtho();
		m_vvWidth  = (m_Actual->m_Size.y) * 0.5;
		m_vvHeight = (m_Actual->m_Size.z) * 0.5;
		nearCP     = 0;
		farCP      = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		m_vvEye    = m_vvLook = m_Actual->m_Mid;
		m_vvEye.x  = m_Upper.x;
		m_vvLook.x = m_Lower.x;
		up.z       = 1;

		// Prepare offset for print tiling
		m_vvOffset.x =  0;
		m_vvOffset.y = -xOffset;
		m_vvOffset.z = -yOffset;
		break;

	case B3_VIEW_LEFT:
		// Prepare glOrtho();
		m_vvWidth  = (m_Actual->m_Size.y) * 0.5;
		m_vvHeight = (m_Actual->m_Size.z) * 0.5;
		nearCP     = 0;
		farCP      = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		m_vvEye    = m_vvLook = m_Actual->m_Mid;
		m_vvEye.x  = m_Lower.x;
		m_vvLook.x = m_Upper.x;
		up.z       = 1;

		// Prepare offset for print tiling
		m_vvOffset.x =  0;
		m_vvOffset.y = -xOffset;
		m_vvOffset.z = -yOffset;
		break;

	case B3_VIEW_BACK:
		// Prepare glOrtho();
		m_vvWidth  = (m_Actual->m_Size.x) * 0.5;
		m_vvHeight = (m_Actual->m_Size.z) * 0.5;
		nearCP     = 0;
		farCP      = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		m_vvEye    = m_vvLook = m_Actual->m_Mid;
		m_vvEye.y  = m_Upper.y;
		m_vvLook.y = m_Lower.y;
		up.z       = 1;

		// Prepare offset for print tiling
		m_vvOffset.x =  xOffset;
		m_vvOffset.y =  0;
		m_vvOffset.z = -yOffset;
		break;
	}

	// Maintain aspect ratio
	if (m_AspectRatio)
	{
		aspectCamera = (GLfloat)(m_vvWidth / m_vvHeight);
		relation     = aspectCamera / aspectWindow;
		if (relation > 1)
		{
			if (m_ViewMode != B3_VIEW_3D)
			{
				m_Actual->m_xRelation = 1;
				m_Actual->m_yRelation = relation;
			}
			m_vvHeight *= relation;
		}
		else
		{
			if (m_ViewMode != B3_VIEW_3D)
			{
				m_Actual->m_xRelation = relation;
				m_Actual->m_yRelation = 1;
			}
			m_vvWidth /= relation;
		}
	}

	// Now initialize view
	if (m_ViewMode == B3_VIEW_3D)
	{
		glFrustum(-m_vvWidth,m_vvWidth,-m_vvHeight,m_vvHeight,nearCP,farCP);
	}
	else
	{
		glOrtho(-m_vvWidth,m_vvWidth,-m_vvHeight,m_vvHeight,nearCP,farCP);
	}
	gluLookAt(
		m_vvEye.x, m_vvEye.y, m_vvEye.z,
		m_vvLook.x,m_vvLook.y,m_vvLook.z,
		up.x,  up.y,  up.z);
	glTranslatef(m_vvOffset.x,m_vvOffset.y,m_vvOffset.z);

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

#define B3_RASTER_FLOOR(v,grid)   (floor((v) / (grid)) * (grid))
#define B3_RASTER_CEIL(v,grid)    (ceil((v) / (grid)) * (grid))
#define B3_RASTER_COUNT(a,e,grid) ((b3_count)(floor((e) / (grid)) - ceil((a) / (grid)) + 1))
#define B3_RASTER_MINDIST  8

void b3RenderView::b3DrawRaster(b3_f64 grid,b3Color &color)
{
#ifdef BLZ3_USE_OPENGL
	b3_vector xDisp,yDisp;
	b3_vector a,b,c,d;
	b3_f64    xStart,xEnd;
	b3_f64    yStart,yEnd;
	b3_f64    depth;
	b3_count  x,y,xCount,yCount;
	b3_f64 aux = B3_RASTER_FLOOR(m_vvLook.x,grid);

	if (m_Actual == null)
	{
		return;
	}

	b3Vector::b3Init(&xDisp);
	b3Vector::b3Init(&yDisp);
	switch(m_ViewMode)
	{
	case B3_VIEW_TOP:
		xDisp.x = grid;
		yDisp.y = grid;
		xStart  = m_vvLook.x - m_vvWidth;
		xEnd    = m_vvLook.x + m_vvWidth;
		yStart  = m_vvLook.y - m_vvHeight;
		yEnd    = m_vvLook.y + m_vvHeight;
		depth   = B3_RASTER_CEIL(m_vvLook.z,grid);

		// horizontal raster
		b3Vector::b3Init(&a,B3_RASTER_CEIL(xStart,grid),yStart,depth);
		b3Vector::b3Init(&b,B3_RASTER_CEIL(xStart,grid),yEnd,  depth);

		// vertical raster
		b3Vector::b3Init(&c,xStart,B3_RASTER_CEIL(yStart,grid),depth);
		b3Vector::b3Init(&d,xEnd,  B3_RASTER_CEIL(yStart,grid),depth);
		break;

	case B3_VIEW_FRONT:
		xDisp.x = grid;
		yDisp.z = grid;
		xStart  = m_vvLook.x - m_vvWidth;
		xEnd    = m_vvLook.x + m_vvWidth;
		yStart  = m_vvLook.z - m_vvHeight;
		yEnd    = m_vvLook.z + m_vvHeight;
		depth   = B3_RASTER_FLOOR(m_vvLook.y,grid);

		// horizontal raster
		b3Vector::b3Init(&a,B3_RASTER_CEIL(xStart,grid),depth,yStart);
		b3Vector::b3Init(&b,B3_RASTER_CEIL(xStart,grid),depth,yEnd);

		// vertical raster
		b3Vector::b3Init(&c,xStart,depth,B3_RASTER_CEIL(yStart,grid));
		b3Vector::b3Init(&d,xEnd,  depth,B3_RASTER_CEIL(yStart,grid));
		break;

	case B3_VIEW_BACK:
		xDisp.x = grid;
		yDisp.z = grid;
		xStart  = m_vvLook.x - m_vvWidth;
		xEnd    = m_vvLook.x + m_vvWidth;
		yStart  = m_vvLook.z - m_vvHeight;
		yEnd    = m_vvLook.z + m_vvHeight;
		depth   = B3_RASTER_CEIL(m_vvLook.y,grid);

		// horizontal raster
		b3Vector::b3Init(&a,B3_RASTER_CEIL(xStart,grid),depth,yStart);
		b3Vector::b3Init(&b,B3_RASTER_CEIL(xStart,grid),depth,yEnd);

		// vertical raster
		b3Vector::b3Init(&c,xStart,depth,B3_RASTER_CEIL(yStart,grid));
		b3Vector::b3Init(&d,xEnd,  depth,B3_RASTER_CEIL(yStart,grid));
		break;

	case B3_VIEW_RIGHT:
		xDisp.y = grid;
		yDisp.z = grid;
		xStart  = m_vvLook.y - m_vvWidth;
		xEnd    = m_vvLook.y + m_vvWidth;
		yStart  = m_vvLook.z - m_vvHeight;
		yEnd    = m_vvLook.z + m_vvHeight;
		depth   = B3_RASTER_CEIL(m_vvLook.x,grid);

		// horizontal raster
		b3Vector::b3Init(&a,depth,B3_RASTER_CEIL(xStart,grid),yStart);
		b3Vector::b3Init(&b,depth,B3_RASTER_CEIL(xStart,grid),yEnd);

		// vertical raster
		b3Vector::b3Init(&c,depth,xStart,B3_RASTER_CEIL(yStart,grid));
		b3Vector::b3Init(&d,depth,xEnd,  B3_RASTER_CEIL(yStart,grid));
		break;

	case B3_VIEW_LEFT:
		xDisp.y = grid;
		yDisp.z = grid;
		xStart  = m_vvLook.y - m_vvWidth;
		xEnd    = m_vvLook.y + m_vvWidth;
		yStart  = m_vvLook.z - m_vvHeight;
		yEnd    = m_vvLook.z + m_vvHeight;
		depth   = aux; // This is due to a internal compiler problem.

		// horizontal raster
		b3Vector::b3Init(&a,depth,B3_RASTER_CEIL(xStart,grid),yStart);
		b3Vector::b3Init(&b,depth,B3_RASTER_CEIL(xStart,grid),yEnd);

		// vertical raster
		b3Vector::b3Init(&c,depth,xStart,B3_RASTER_CEIL(yStart,grid));
		b3Vector::b3Init(&d,depth,xEnd,  B3_RASTER_CEIL(yStart,grid));
		break;

	default:
		return;
	}

	xCount  = B3_RASTER_COUNT(xStart,xEnd,grid);
	yCount  = B3_RASTER_COUNT(yStart,yEnd,grid);
	if ((xCount == 0 ? true : (m_xRes / xCount) >= B3_RASTER_MINDIST) &&
	    (yCount == 0 ? true : (m_yRes / yCount) >= B3_RASTER_MINDIST))
	{
		glBegin(GL_LINES);
		glColor3f(color[b3Color::R],color[b3Color::G],color[b3Color::B]);
		for (x = 0;x < xCount;x++)
		{
			glVertex3f(a.x,a.y,a.z);
			glVertex3f(b.x,b.y,b.z);
			b3Vector::b3Add(&xDisp,&a);
			b3Vector::b3Add(&xDisp,&b);
		}
		for (y = 0;y < yCount;y++)
		{
			glVertex3f(c.x,c.y,c.z);
			glVertex3f(d.x,d.y,d.z);
			b3Vector::b3Add(&yDisp,&c);
			b3Vector::b3Add(&yDisp,&d);
		}
		glEnd();
	}
#endif
}
