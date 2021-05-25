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

#include <QPainter>

#include <blz3/raytrace/b3Scene.h>

#include "b3OpenGLScrollArea.h"
#include "b3OpenGLWidget.h"
#include "b3CameraVolume.h"

QB3OpenGLWidget::QB3OpenGLWidget(QWidget * parent) :
	QOpenGLWidget(parent), rubber_band(QRubberBand::Rectangle, this)
{
}

void QB3OpenGLWidget::b3Prepare(b3Scene * first, b3CameraVolume * volume)
{
	B3_METHOD;

	b3_res   xSize, ySize;

	m_Scene = first;
	m_Scene->b3Reorg();
	m_Scene->b3GetDisplaySize(xSize, ySize);
	m_Scene->b3SetupVertexMemory(&m_Context);
	m_Scene->b3PrepareScene(xSize, ySize);
	m_Scene->b3ResetAnimation();

	// Reset bounds.
	m_View.b3SetBounds(m_Scene);

	// Reset camera.
	m_CameraVolume = volume;
	m_CameraVolume->b3SetupVertexMemory(&m_Context);

	const b3ModellerInfo * info = m_Scene->b3GetModellerInfo();

	m_AllLights = info->m_UseSceneLights;

	b3SetCamera(m_Scene->b3GetFirstCamera(false));
	b3SetLights();

	b3PrintF(B3LOG_NORMAL, "%7d vertices\n",  m_Context.glVertexCount);
	b3PrintF(B3LOG_NORMAL, "%7d triangles\n", m_Context.glPolyCount);
	b3PrintF(B3LOG_NORMAL, "%7d grids\n",     m_Context.glGridCount);
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

void QB3OpenGLWidget::b3SetViewMode(
	const b3_view_mode view_mode,
	b3_view_info   &   view_info)
{
	m_View.b3SetViewMode(view_mode);
	m_View.b3GetView(view_info);
	update();
}

void QB3OpenGLWidget::b3MoveView(
	const b3_f64   dx,
	const b3_f64   dy,
	b3_view_info & view_info)
{
	m_View.b3Move(dx, dy);
	m_View.b3GetView(view_info);
	update();
}

void QB3OpenGLWidget::b3ScaleView(const b3_f64 factor, b3_view_info & view_info)
{
	m_View.b3Scale(factor);
	m_View.b3GetView(view_info);
	update();
}

void QB3OpenGLWidget::b3FullView(b3_view_info & view_info)
{
	m_View.b3Original();
	m_View.b3GetView(view_info);
	update();
}

void QB3OpenGLWidget::b3PreviousView(b3_view_info & view_info)
{
	m_View.b3PopView();
	m_View.b3GetView(view_info);
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

	m_View.b3SetupView(m_xWinSize = xSize, m_yWinSize = ySize);
}

void QB3OpenGLWidget::paintGL()
{
	B3_METHOD;

	m_Scene->b3ComputeBounds(&m_Lower, &m_Upper);
//	m_CameraVolume->b3ComputeBounds(&m_Lower, &m_Upper);

	m_Context.b3StartDrawing();
	m_View.b3SetBounds(&m_Lower, &m_Upper);
	m_View.b3SetCamera(m_Scene);
	m_View.b3SetupView(m_xWinSize, m_yWinSize);
	m_Scene->b3Draw(&m_Context);
	if (!m_View.b3IsViewMode(B3_VIEW_3D))
	{
		m_CameraVolume->b3Draw(&m_Context);
	}
}

void QB3OpenGLWidget::show()
{
	rubber_band.show();
}

void QB3OpenGLWidget::hide()
{
	rubber_band.hide();
}

void QB3OpenGLWidget::cursorPanning()
{
	setCursor(Qt::ClosedHandCursor);
}

void QB3OpenGLWidget::cursorArrow()
{
	setCursor(Qt::ArrowCursor);
}

void QB3OpenGLWidget::setRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	QRect    rect(QPoint(x1, y1), QPoint(x2, y2));

	rubber_band.setGeometry(rect.normalized());
}

void QB3OpenGLWidget::select(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	m_View.b3Select(
		b3_f64(x1) / m_xWinSize, b3_f64(y1) / m_yWinSize,
		b3_f64(x2) / m_xWinSize, b3_f64(y2) / m_yWinSize);
	update();
}

void QB3OpenGLWidget::move(int32_t dx, int32_t dy)
{
	m_View.b3Move(b3_f64(-dx) / m_xWinSize, b3_f64(dy) / m_yWinSize);
	update();
}

bool QB3OpenGLWidget::is3D()
{
	return m_View.b3IsViewMode(B3_VIEW_3D);
}
