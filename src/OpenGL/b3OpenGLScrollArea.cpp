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
#include <QScrollBar>

QB3OpenGLScrollArea::QB3OpenGLScrollArea(QB3OpenGLWidget * glWidget) :
	child(glWidget)
{
	QWidget * parent = glWidget->parentWidget();
	QLayout * layout = parent->layout();

	layout->replaceWidget(glWidget, this);
	setViewport(glWidget);
	dumpObjectTree();
}

void QB3OpenGLScrollArea::b3SetViewmode(const b3_view_mode mode)
{
	child->b3SetViewmode(mode, h, v);
	update(horizontalScrollBar(), h);
	update(verticalScrollBar(), v);
}

void QB3OpenGLScrollArea::b3MoveView(const b3_f64 dx, const b3_f64 dy)
{
	child->b3MoveView(dx, dy, h, v);
	update(horizontalScrollBar(), h);
	update(verticalScrollBar(), v);
}

void QB3OpenGLScrollArea::b3ScaleView(const b3_f64 factor)
{
	child->b3ScaleView(factor, h, v);
	update(horizontalScrollBar(), h);
	update(verticalScrollBar(), v);
}

void QB3OpenGLScrollArea::b3FullView()
{
	child->b3FullView(h, v);
	update(horizontalScrollBar(), h);
	update(verticalScrollBar(), v);
}

void QB3OpenGLScrollArea::b3PreviousView()
{
	child->b3PreviousView(h, v);
	update(horizontalScrollBar(), h);
	update(verticalScrollBar(), v);
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

void QB3OpenGLScrollArea::update(QScrollBar * bar, const QB3BarInfo & info)
{
	bar->setRange(info.min, info.max - info.page_size);
	bar->setPageStep(info.page_size);
	bar->setValue(info.page_pos);
}
