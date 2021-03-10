/*
**
**	$Filename:	qrender.cpp $
**	$Release:	Dortmund 2001 - 2021 $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001 - 2021  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "b3OpenGLWidget.h"

#include <blz3/raytrace/b3Scene.h>

QB3OpenGLWidget::QB3OpenGLWidget(QWidget * parent) :
	QOpenGLWidget(parent)
{

}

void QB3OpenGLWidget::b3Prepare(b3Item * first)
{
	b3_res          xSize, ySize;

	m_Scene = static_cast<b3Scene *>(first);
	m_Scene->b3Reorg();
	m_Scene->b3GetDisplaySize(xSize, ySize);
	m_Scene->b3PrepareScene(xSize, ySize);
	m_Scene->b3SetCamera(m_Scene->b3GetFirstCamera(false));
	m_Scene->b3Activate(false);

	const b3ModellerInfo * info = m_Scene->b3GetModellerInfo();
	m_AllLights = info->m_UseSceneLights;

	m_Scene->b3SetupVertexMemory(&m_Context);
	m_Scene->b3ResetAnimation();
	b3SetLights();

	b3PrintF(B3LOG_NORMAL, "%7zd vertices\n",  m_Context.glVertexCount);
	b3PrintF(B3LOG_NORMAL, "%7zd triangles\n", m_Context.glPolyCount);
	b3PrintF(B3LOG_NORMAL, "%7zd grids\n",     m_Context.glGridCount);
	update();
}

QString QB3OpenGLWidget::timecode(const int frame) const
{
	const b3Animation * animation = *this;

	if (animation != nullptr)
	{
		const b3_count fps    = animation->m_FramesPerSecond;
		const double   t      = animation->m_Start + (double)frame / fps;
		const int      hour   = t / 3600;
		const unsigned minute = abs(int(t) / 60) % 60;
		const unsigned second = abs(int(t)) % 60;
		const unsigned sub    = abs(int(t * fps)) % fps;

		return QString::asprintf("%02d:%02u:%02u.%02u  %3d",
								 hour, minute, second, sub, frame);
	}
	else
	{
		return "";
	}
}

void QB3OpenGLWidget::animate(int frame)
{
	const b3Animation * animation = *this;

	if (animation != nullptr)
	{
		const b3_count fps = animation->m_FramesPerSecond;
		const double   t   = animation->m_Start + (double)frame / fps;

		b3PrintF(B3LOG_DEBUG, "t=%1.3fs\n", t);
		m_Scene->b3SetAnimation(t);
		m_Scene->b3ComputeBounds(&m_Lower, &m_Upper);
		update();
	}
}

void QB3OpenGLWidget::initializeGL()
{
	B3_METHOD;

	m_Context.glBgColor.b3Init(0.7f, 0.7f, 1.0f);
	m_Context.b3Init(m_DoubleBuffered);
}

void QB3OpenGLWidget::resizeGL(int xSize, int ySize)
{
	B3_METHOD;

	m_View.b3SetupView(xWinSize = xSize, yWinSize = ySize);
	m_View.b3SetViewMode(B3_VIEW_3D);
	m_Lights.b3SetupLight(&m_Context);
}

void QB3OpenGLWidget::paintGL()
{
	B3_METHOD;

	m_Context.b3StartDrawing();
	m_View.b3SetBounds(&m_Lower, &m_Upper);
	m_View.b3SetCamera(m_Scene);
	m_View.b3SetupView(xWinSize, yWinSize);
	m_Scene->b3Draw(&m_Context);
}

void QB3OpenGLWidget::b3SetLights()
{
	m_Context.b3LightReset();
	if (m_AllLights)
	{
		b3PrintF(B3LOG_DEBUG, "Using multiple lights with%s spots...\n",
			m_SpotLight ? "" : "out");

		m_Lights.b3SetScene(m_Scene);
		m_Lights.b3SetLightMode(m_SpotLight ? B3_LIGHT_SCENE_SPOT : B3_LIGHT_SCENE);
	}
	else
	{
		b3PrintF(B3LOG_DEBUG, "Using one light...\n");
		m_Lights.b3SetLightMode(B3_LIGHT_SIMPLE);
	}
}
