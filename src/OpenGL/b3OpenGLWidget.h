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
class QB3BarInfo;

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

	void b3SetViewMode(
		const b3_view_mode view_mode,
		b3_view_info   &   view_info);
	void b3MoveView(
		const b3_f64   dx,
		const b3_f64   dy,
		b3_view_info & view_info);
	void b3ScaleView(
		const b3_f64   factor,
		b3_view_info & view_info);
	void b3FullView(b3_view_info & view_info);
	void b3PreviousView(b3_view_info & view_info);

protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

private:
	void b3SetLights();

	b3Scene       *       m_Scene = nullptr;
	b3CameraVolume    *   m_CameraVolume = nullptr;
	b3ShapeRenderContext  m_Context;
	b3RenderLight         m_Lights;
	b3RenderView          m_View;
	b3_bool               m_AllLights = true;
	b3_bool               m_SpotLight = true;
	b3_res                xWinSize, yWinSize;
	b3_vector             m_Lower, m_Upper;

	friend class QB3OpenGLScrollArea;
};

#endif // QB3OPENGLWIDGET_H
