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

void b3RenderView::b3UpdateView(b3_res xSize,b3_res ySize)
{
#ifdef BLZ3_USE_OPENGL
	b3_f64  width,height,distance,factor,relation;
	GLfloat aspectWindow = (GLfloat)xSize / (GLfloat)ySize;
	GLfloat aspectCamera;
	GLfloat min = 0.1f;

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

	aspectCamera = (GLfloat)(width / height);
	relation     = aspectCamera / aspectWindow;
	if (relation > 1) height *= relation;
	else              width  /= relation;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,xSize,ySize);
	switch (m_ViewMode)
	{
	case B3_VIEW_3D:
	default:
		glFrustum(-width,width,-height,height,min,10000.0f);
		gluLookAt(
			m_EyePoint.x, m_EyePoint.y, m_EyePoint.z,
			m_ViewPoint.x,m_ViewPoint.y,m_ViewPoint.z,
			m_Height.x,   m_Height.y,   m_Height.z);
		break;
	case B3_VIEW_TOP:
		glOrtho(-1000.0,1000.0,-1000.0,1000.0,-1000.0,1000.0);
		break;

	case B3_VIEW_FRONT:
		glRotated(90.0,1.0,0.0,0.0);
		glOrtho(-1000.0,1000.0,-1000.0,1000.0,-1000.0,1000.0);
		break;

	case B3_VIEW_RIGHT:
		glRotated( 90.0,1.0,0.0,0.0);
		glRotated(-90.0,0.0,0.0,1.0);
		glOrtho(-1000.0,1000.0,-1000.0,1000.0,-1000.0,1000.0);
		break;

	case B3_VIEW_LEFT:
		glRotated(90.0,1.0,0.0,0.0);
		glRotated(90.0,0.0,0.0,1.0);
		glOrtho(-1000.0,1000.0,-1000.0,1000.0,-1000.0,1000.0);
		break;

	case B3_VIEW_BACK:
		glRotated( 90.0,1.0,0.0,0.0);
		glRotated(180.0,0.0,0.0,1.0);
		glOrtho(-1000.0,1000.0,-1000.0,1000.0,-1000.0,1000.0);
		break;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
#endif
}
