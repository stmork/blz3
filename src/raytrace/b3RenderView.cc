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
#include <assert.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.5  2001/08/12 19:47:48  sm
**      - Now having correct orthogonal projection incl. aspect ratio
**
**      Revision 1.4  2001/08/11 20:17:22  sm
**      - Updated OpenGL on Un*x platform.
**      - This was a great day!
**
**      Revision 1.3  2001/08/11 19:59:16  sm
**      - Added orthogonal projection
**
**      Revision 1.2  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.1  2001/08/11 15:59:59  sm
**      - Rendering cleaned up
**      - CWinApp/CMainFrm derived from Blizzard III classes
**        supporting more effective GUI.
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3RenderView::b3RenderView()
{
	m_ViewMode    = B3_VIEW_3D;
	m_AntiAliased = true;
}

void b3RenderView::b3SetViewMode(b3_view_mode mode)
{
	m_ViewMode = mode;
}

b3_bool b3RenderView::b3IsViewMode(b3_view_mode mode)
{
	return m_ViewMode == mode;
}

void b3RenderView::b3SetCamera(b3CameraPart *camera)
{
	m_EyePoint  = camera->EyePoint;
	m_ViewPoint = camera->ViewPoint;
	m_Width     = camera->Width;
	m_Height    = camera->Height;
}

void b3RenderView::b3SetCamera(b3Scene *scene)
{
	m_EyePoint  = scene->EyePoint;
	m_ViewPoint = scene->ViewPoint;
	m_Width     = scene->Width;
	m_Height    = scene->Height;
}

b3_bool b3RenderView::b3SetBounds(b3Scene *scene)
{
	return scene->b3ComputeBounds(&m_Lower,&m_Upper);
}

void b3RenderView::b3UpdateView(b3_res xSize,b3_res ySize)
{
#ifdef BLZ3_USE_OPENGL
	b3_f64    width,height,nearCP,farCP,distance,factor,relation;
	GLfloat   aspectWindow = (GLfloat)xSize / (GLfloat)ySize;
	GLfloat   aspectCamera;
	GLfloat   min = 0.1f;
	b3_vector eye,look,up,mid;

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

	distance = b3Distance(&m_ViewPoint,&m_EyePoint);
	factor   = min / distance;
	width    = factor * b3Length(&m_Width);
	height   = factor * b3Length(&m_Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,xSize,ySize);
	eye.x = look.x = mid.x = (m_Lower.x + m_Upper.x) * 0.5;
	eye.y = look.y = mid.y = (m_Lower.y + m_Upper.y) * 0.5;
	eye.z = look.z = mid.z = (m_Lower.z + m_Upper.z) * 0.5;
	up.x  =
	up.y  =
	up.z  = 0;
	switch (m_ViewMode)
	{
	case B3_VIEW_3D:
	default:
		nearCP = min;
		farCP  = 10000;

		// Prepare gluLookAt() - it's simple
		eye  = m_EyePoint;
		look = m_ViewPoint;
		up   = m_Height;
		break;
	case B3_VIEW_TOP:
		// Prepare glOrtho();
		width  = (m_Upper.x - m_Lower.x) * 0.5;
		height = (m_Upper.y - m_Lower.y) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.z - m_Lower.z);

		// Prepare gluLookAt()
		eye.z  = m_Upper.z;
		look.z = m_Lower.z;
		up.y   = 1;
		break;

	case B3_VIEW_FRONT:
		// Prepare glOrtho();
		width  = (m_Upper.x - m_Lower.x) * 0.5;
		height = (m_Upper.z - m_Lower.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		eye.y  = m_Lower.y;
		look.y = m_Upper.y;
		up.z   = 1;
		break;

	case B3_VIEW_RIGHT:
		// Prepare glOrtho();
		width  = (m_Upper.y - m_Lower.y) * 0.5;
		height = (m_Upper.z - m_Lower.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		eye.x  = m_Upper.x;
		look.x = m_Lower.x;
		up.z   = 1;
		break;

	case B3_VIEW_LEFT:
		// Prepare glOrtho();
		width  = (m_Upper.y - m_Lower.y) * 0.5;
		height = (m_Upper.z - m_Lower.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.x - m_Lower.x);

		// Prepare gluLookAt()
		eye.x  = m_Lower.x;
		look.x = m_Upper.x;
		up.z   = 1;
		break;

	case B3_VIEW_BACK:
		// Prepare glOrtho();
		width  = (m_Upper.x - m_Lower.x) * 0.5;
		height = (m_Upper.z - m_Lower.z) * 0.5;
		nearCP = 0;
		farCP  = (m_Upper.y - m_Lower.y);

		// Prepare gluLookAt()
		eye.y  = m_Upper.y;
		look.y = m_Lower.y;
		up.z   = 1;
		break;
	}

	// Maintain aspect ratio
	aspectCamera = (GLfloat)(width / height);
	relation     = aspectCamera / aspectWindow;
	if (relation > 1) height *= relation;
	else              width  /= relation;

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
#endif
}
