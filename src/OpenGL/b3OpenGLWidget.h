/*
**
**	$Filename:	b3OpenGLWidget.h $
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

#ifndef QB3OPENGLWIDGET_H
#define QB3OPENGLWIDGET_H

#include <QOpenGLWidget>

#include <blz3/raytrace/b3ShapeRenderContext.h>
#include <blz3/raytrace/b3RenderLight.h>
#include <blz3/raytrace/b3RenderView.h>
#include <blz3/raytrace/b3Scene.h>

class b3CameraVolume;
class QB3OpenGLScrollArea;

struct QB3BarInfo
{
	b3_f64 min;
	b3_f64 max;
	b3_f64 page_size;
	b3_f64 page_pos;
};

class QB3OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit QB3OpenGLWidget(QWidget * parent = nullptr);

	void b3Prepare(b3Scene * first, b3CameraVolume * volume);
	void b3SetCamera(b3CameraPart * camera);

	bool b3IsAllLights() const;
	void b3SetAllLights(const bool all);
	bool b3IsSpotLight() const;
	void b3SetSpotLight(const bool spot);

	void b3SetViewmode(const b3_view_mode mode,
		QB3BarInfo     &     horizontal,
		QB3BarInfo     &     vertical);
	void b3MoveView(const b3_f64 dx, const b3_f64 dy,
		QB3BarInfo     &     horizontal,
		QB3BarInfo     &     vertical);
	void b3ScaleView(const b3_f64 factor,
		QB3BarInfo     &     horizontal,
		QB3BarInfo     &     vertical);
	void b3FullView(
		QB3BarInfo     &     horizontal,
		QB3BarInfo     &     vertical);
	void b3PreviousView(
		QB3BarInfo     &     horizontal,
		QB3BarInfo     &     vertical);

protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

private:
	void b3SetLights();
	static void b3GetBarInfo(
		const b3_view_info & info,
		QB3BarInfo     &     horizontal,
		QB3BarInfo     &     vertical);

	b3Scene       *       m_Scene = nullptr;
	b3CameraVolume    *   m_CameraVolume = nullptr;
	b3ShapeRenderContext  m_Context;
	b3RenderLight         m_Lights;
	b3RenderView          m_View;
	b3_bool               m_AllLights = true;
	b3_bool               m_SpotLight = true;
	b3_res                xWinSize, yWinSize;
	b3_vector             m_Lower, m_Upper;
	b3_view_mode          m_ViewMode = B3_VIEW_3D;

	friend class QB3OpenGLScrollArea;
};

#endif // QB3OPENGLWIDGET_H
