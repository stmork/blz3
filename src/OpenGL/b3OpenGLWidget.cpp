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

	b3Update();
	b3SetLights();
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
	m_Context.glBgColor.b3Init(0.7f, 0.7f, 1.0f);
	m_Context.b3Init(m_DoubleBuffered);
}

void QB3OpenGLWidget::resizeGL(int xSize, int ySize)
{
	b3PrintF(B3LOG_FULL, ">resizeGL(%d, %d );\n", xSize, ySize);
	m_View.b3SetupView(xWinSize = xSize, yWinSize = ySize);
	m_Lights.b3SetupLight(&m_Context);
	b3PrintF(B3LOG_FULL, "<resizeGL()\n");
}

void QB3OpenGLWidget::paintGL()
{
	b3PrintF(B3LOG_FULL, ">paintGL()\n");
	m_Context.b3StartDrawing();
	m_View.b3SetBounds(&m_Lower, &m_Upper);
	m_View.b3SetCamera(m_Scene);
	m_View.b3SetupView(xWinSize, yWinSize);
	m_Scene->b3Draw(&m_Context);
	b3PrintF(B3LOG_FULL, "<paintGL()\n");
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

void QB3OpenGLWidget::b3Update()
{
	m_Scene->b3SetupVertexMemory(&m_Context);
	m_Scene->b3ResetAnimation();
	m_Scene->b3ComputeBounds(&m_Lower, &m_Upper);

	b3PrintF(B3LOG_NORMAL, "%7d vertices\n",  m_Context.glVertexCount);
	b3PrintF(B3LOG_NORMAL, "%7d triangles\n", m_Context.glPolyCount);
	b3PrintF(B3LOG_NORMAL, "%7d grids\n",     m_Context.glGridCount);

	// Setup view
	m_View.b3SetBounds(&m_Lower, &m_Upper);
	m_View.b3SetCamera(m_Scene);
	m_View.b3SetViewMode(B3_VIEW_3D);
}
