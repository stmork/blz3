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

class QB3OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit QB3OpenGLWidget(QWidget* parent = nullptr);
};

#endif // QB3OPENGLWIDGET_H
