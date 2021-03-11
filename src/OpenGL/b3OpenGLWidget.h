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

#ifndef QB3OPENGLWIDGET_H
#define QB3OPENGLWIDGET_H

#include <QOpenGLWidget>

#include <blz3/raytrace/b3ShapeRenderContext.h>
#include <blz3/raytrace/b3RenderLight.h>
#include <blz3/raytrace/b3RenderView.h>
#include <blz3/raytrace/b3Scene.h>

class QB3OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit QB3OpenGLWidget(QWidget * parent = nullptr);

	void b3Prepare(b3Scene * first);

protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

private:
	void b3SetLights();

	b3Scene       *       m_Scene = nullptr;
	b3ShapeRenderContext  m_Context;
	b3RenderLight         m_Lights;
	b3RenderView          m_View;
	b3_bool               m_AllLights = true;
	b3_bool               m_SpotLight = true;
	b3_bool               m_DoubleBuffered = true;
	b3_res                xWinSize, yWinSize;
	b3_vector             m_Lower, m_Upper;
};

#endif // QB3OPENGLWIDGET_H
