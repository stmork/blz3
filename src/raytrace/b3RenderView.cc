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
	glFrustum(-width,width,-height,height,min,10000.0f);
	gluLookAt(
		m_EyePoint.x, m_EyePoint.y, m_EyePoint.z,
		m_ViewPoint.x,m_ViewPoint.y,m_ViewPoint.z,
		m_Height.x,   m_Height.y,   m_Height.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
#endif
}
