/*
**
**	$Filename:	b3OpenGLWidget.cpp $
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
#include "b3CameraVolume.h"

#include <blz3/raytrace/b3Scene.h>

#include "b3OpenGLScrollArea.h"

QB3OpenGLWidget::QB3OpenGLWidget(QWidget * parent) :
	QOpenGLWidget(parent)
{
}

void QB3OpenGLWidget::b3Prepare(b3Scene * first, b3CameraVolume * volume)
{
	B3_METHOD;

	b3_res   xSize, ySize;

	m_Scene = first;
	m_Scene->b3Reorg();
	m_Scene->b3GetDisplaySize(xSize, ySize);
	m_Scene->b3PrepareScene(xSize, ySize);
	m_Scene->b3SetupVertexMemory(&m_Context);
	m_Scene->b3ResetAnimation();

	m_CameraVolume = volume;
	m_CameraVolume->b3SetupVertexMemory(&m_Context);

	const b3ModellerInfo * info = m_Scene->b3GetModellerInfo();

	m_AllLights = info->m_UseSceneLights;

	b3SetCamera(m_Scene->b3GetFirstCamera(false));
	b3SetLights();

	b3PrintF(B3LOG_NORMAL, "%7zd vertices\n",  m_Context.glVertexCount);
	b3PrintF(B3LOG_NORMAL, "%7zd triangles\n", m_Context.glPolyCount);
	b3PrintF(B3LOG_NORMAL, "%7zd grids\n",     m_Context.glGridCount);
}

void QB3OpenGLWidget::b3SetCamera(b3CameraPart * camera)
{
	m_Scene->b3SetCamera(camera);
	m_CameraVolume->b3Update(camera);
	update();
}

bool QB3OpenGLWidget::b3IsAllLights() const
{
	return m_AllLights;
}

void QB3OpenGLWidget::b3SetAllLights(const bool all)
{
	m_AllLights = all;

	b3SetLights();
	update();
}

bool QB3OpenGLWidget::b3IsSpotLight() const
{
	return m_SpotLight;
}

void QB3OpenGLWidget::b3SetSpotLight(const bool spot)
{
	m_SpotLight = spot;

	b3SetLights();
	update();
}

void QB3OpenGLWidget::b3SetViewmode(
	const b3_view_mode mode,
	QB3BarInfo    &    horizontal,
	QB3BarInfo    &    vertical)
{
	b3_view_info view_info;

	m_ViewMode = mode;
	m_View.b3SetViewMode(m_ViewMode);
	m_View.b3GetView(view_info);
	b3GetBarInfo(view_info, horizontal, vertical);
	update();
}

void QB3OpenGLWidget::b3MoveView(
	const b3_f64 dx, const b3_f64 dy,
	QB3BarInfo & horizontal,
	QB3BarInfo & vertical)
{
	b3_view_info view_info;

	m_View.b3Move(dx, dy);
	m_View.b3GetView(view_info);
	b3GetBarInfo(view_info, horizontal, vertical);
	update();
}

void QB3OpenGLWidget::b3ScaleView(
	const b3_f64 factor,
	QB3BarInfo & horizontal,
	QB3BarInfo & vertical)
{
	b3_view_info view_info;

	m_View.b3Scale(factor);
	m_View.b3GetView(view_info);
	b3GetBarInfo(view_info, horizontal, vertical);
	update();
}

void QB3OpenGLWidget::b3FullView(
	QB3BarInfo & horizontal,
	QB3BarInfo & vertical)
{
	b3_view_info view_info;

	m_View.b3Original();
	m_View.b3GetView(view_info);
	b3GetBarInfo(view_info, horizontal, vertical);
	update();
}

void QB3OpenGLWidget::b3PreviousView(
	QB3BarInfo & horizontal,
	QB3BarInfo & vertical)
{
	b3_view_info view_info;

	m_View.b3PopView();
	m_View.b3GetView(view_info);
	b3GetBarInfo(view_info, horizontal, vertical);
	update();
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

	// Inform OpenGL
	makeCurrent();
	m_Lights.b3SetupLight(&m_Context);
	doneCurrent();
}

void QB3OpenGLWidget::b3GetBarInfo(
	const b3_view_info & info,
	QB3BarInfo     &     horizontal,
	QB3BarInfo     &     vertical)
{
	horizontal.set(info.scene.left, info.scene.right, info.view.left, info.view.right);
	vertical.set(info.scene.bottom, info.scene.top, info.view.bottom, info.view.top);
}

void QB3OpenGLWidget::initializeGL()
{
	B3_METHOD;

	m_Context.glBgColor.b3Init(0.7f, 0.7f, 1.0f);
	m_Context.b3Init(true);
	m_Context.b3SetAntiAliasing(true);
	m_Lights.b3SetupLight(&m_Context);
}

void QB3OpenGLWidget::resizeGL(int xSize, int ySize)
{
	B3_METHOD;

	m_View.b3SetViewMode(m_ViewMode);
	m_View.b3SetupView(xWinSize = xSize, yWinSize = ySize);
}

void QB3OpenGLWidget::paintGL()
{
	B3_METHOD;

	m_Scene->b3ComputeBounds(&m_Lower, &m_Upper);
	m_CameraVolume->b3ComputeBounds(&m_Lower, &m_Upper);

	m_Context.b3StartDrawing();
	m_View.b3SetCamera(m_Scene);
	m_View.b3SetBounds(&m_Lower, &m_Upper);
	m_View.b3SetupView(xWinSize, yWinSize);
	m_Scene->b3Draw(&m_Context);
	if (m_ViewMode != B3_VIEW_3D)
	{
		m_CameraVolume->b3Draw(&m_Context);
	}
}
