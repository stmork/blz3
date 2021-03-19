/*
**
**	$Filename:	b3OpenGLScrollArea.cpp $
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

#include "b3OpenGLScrollArea.h"

#include <QLayout>

QB3OpenGLScrollArea::QB3OpenGLScrollArea(QB3OpenGLWidget * glWidget) :
	child(glWidget)
{
	QWidget * parent = glWidget->parentWidget();
	QLayout * layout = parent->layout();

	layout->replaceWidget(glWidget, this);
	setViewport(glWidget);
}

// https://stackoverflow.com/questions/30046006/using-qopenglwidget-as-viewport-in-qabstractscrollarea
void QB3OpenGLScrollArea::resizeEvent(QResizeEvent * event)
{
	child->resizeEvent(event);
}

// https://stackoverflow.com/questions/30046006/using-qopenglwidget-as-viewport-in-qabstractscrollarea
void QB3OpenGLScrollArea::paintEvent(QPaintEvent * event)
{
	child->paintEvent(event);
}
