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

/*************************************************************************
**                                                                      **
**                        QB3OpenGLScrollArea Implementation            **
**                                                                      **
*************************************************************************/

QB3OpenGLScrollArea::QB3OpenGLScrollArea(QB3OpenGLWidget * glWidget) :
	child(glWidget)
{
	QWidget * parent = glWidget->parentWidget();
	QLayout * layout = parent->layout();

	QB3BarInfo test;

	test.set(-3, 5, 1, 6);
	int rel = test.relToBar(-0.5); // This is in view relation
	int pos = test.posToBar(5.0);
	b3_f64 value = test.relFromBar(-test.bar_page_size / 2);

	b3PrintF(B3LOG_FULL, "%d %d %lf\n", rel, pos, value);

	layout->replaceWidget(glWidget, this);
	setViewport(glWidget);
	dumpObjectTree();

	connect(horizontalScrollBar(), &QScrollBar::valueChanged, [this] (int value)
	{
		child->b3MoveView(h.relFromBar(value - h.bar_pos), 0.0, h, v);
	});

	connect(verticalScrollBar(), &QScrollBar::valueChanged, [this] (int value)
	{
		child->b3MoveView(0.0, v.relFromBar(value - v.bar_pos), h, v);
	});
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

void QB3OpenGLScrollArea::update(QScrollBar * bar, QB3BarInfo & info)
{
	bar->setRange(QB3BarInfo::BAR_RANGE_MIN, QB3BarInfo::BAR_RANGE_MAX - info.bar_page_size);
	bar->setPageStep(info.bar_page_size);
	bar->setValue(info.posToBar(info.page_pos));
}

/*************************************************************************
**                                                                      **
**                        QB3BarInfo Implementation                     **
**                                                                      **
*************************************************************************/

void QB3BarInfo::set(
		const b3_f32 scene_lower,
		const b3_f32 scene_upper,
		const b3_f32 view_lower,
		const b3_f32 view_upper)
{
	min       = B3_MIN(scene_lower, view_lower);
	max       = B3_MAX(scene_upper, view_upper);
	page_size = view_upper - view_lower;
	page_pos  = view_lower;
	scale     = (max - min) / page_size;

	bar_page_size = QB3BarInfo::BAR_RANGE / scale;
}

b3_f64 QB3BarInfo::relFromBar(const int value) const
{
	return (value - BAR_RANGE_MIN) * scale / BAR_RANGE;
}

int QB3BarInfo::relToBar(const b3_f64 value) const
{
	return value * BAR_RANGE / scale + BAR_RANGE_MIN;
}

int QB3BarInfo::posToBar(const b3_f64 pos)
{
	bar_pos = (pos - min) * BAR_RANGE / (max - min) + BAR_RANGE_MIN;
	return bar_pos;
}
