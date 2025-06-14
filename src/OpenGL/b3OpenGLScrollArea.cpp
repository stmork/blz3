/*
**
**	$Filename:	b3OpenGLScrollArea.cpp $
**	$Release:	Dortmund 2001 - 2025 $
**
**	Blizzard III - The new Blizzard III raytracer
**
**      (C) Copyright 2001 - 2025  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

#include "b3OpenGLScrollArea.h"

#include <QLayout>
#include <QScrollBar>
#include <QMouseEvent>

/*************************************************************************
**                                                                      **
**                        QB3OpenGLScrollArea Implementation            **
**                                                                      **
*************************************************************************/

QB3OpenGLScrollArea::QB3OpenGLScrollArea(QWidget * parent) :
	QScrollArea(parent), subscription(*this)
{
#if 0
	QB3BarInfo test;

	test.set(false, -30, 50, 10, 60);
	int rel = test.relToBar(-0.5); // This is in view relation
	int pos = test.posToBar(5.0);
	b3_f64 value = test.relFromBar(-test.bar_page_size / 2);

	b3PrintF(B3LOG_FULL, "%d %d %lf\n", rel, pos, value);
#endif
	setSubscription(subscription);
	gui().setOperationCallback(this);
}

void QB3OpenGLScrollArea::setGlWidget(QB3OpenGLWidget * glWidget)
{
	QWidget * parent = glWidget->parentWidget();
	QLayout * layout = parent->layout();

	child = glWidget;
	layout->replaceWidget(glWidget, this);
	setViewport(glWidget);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	dumpObjectTree();
	view().setOperationCallback(glWidget);
	const bool success = subscribe(gui().getSelectionEnd());
	assert(success);

#if 1
	connect(horizontalScrollBar(), &QScrollBar::valueChanged,
		this, &QB3OpenGLScrollArea::xValueChanged);
	connect(verticalScrollBar(),   &QScrollBar::valueChanged,
		this, &QB3OpenGLScrollArea::yValueChanged);
#endif

	assert (check());
	enter();
}

void QB3OpenGLScrollArea::onSelect(bool checked)
{
	if (child != nullptr)
	{
		checked ? gui().raiseOnSelect() : gui().raiseOnDisable();
	}
}

void QB3OpenGLScrollArea::xValueChanged(int value)
{
	const int diff = value - h.bar_pos;

	if ((diff != 0) && (is_interactive))
	{
		const b3_f64 rel = h.relFromBar(diff);
		b3_view_info view_info;

		child->b3MoveView(rel, 0.0, view_info);
		h.bar_pos = value;
	}
}

void QB3OpenGLScrollArea::yValueChanged(int value)
{
	const int diff = value - v.bar_pos;

	if ((diff != 0) && (is_interactive))
	{
		const b3_f64 rel = v.relFromBar(diff);
		b3_view_info view_info;

		child->b3MoveView(0.0, rel, view_info);
		v.bar_pos = value;
	}
}

void QB3OpenGLScrollArea::updateScrolling()
{
	update([this](b3_view_info & view_info)
	{
		child->m_View.b3GetView(view_info);
	});
}

void QB3OpenGLScrollArea::b3SetViewMode(const b3_view_mode mode)
{
	update([this, mode](b3_view_info & view_info)
	{
		child->b3SetViewMode(mode, view_info);
	});
}

void QB3OpenGLScrollArea::b3MoveView(const b3_f64 dx, const b3_f64 dy)
{
	update([this, dx, dy](b3_view_info & view_info)
	{
		child->b3MoveView(dx, dy, view_info);
	});
}

void QB3OpenGLScrollArea::b3ScaleView(const b3_f64 factor)
{
	update([this, factor](b3_view_info & view_info)
	{
		child->b3ScaleView(factor, view_info);
	});
}

void QB3OpenGLScrollArea::b3FullView()
{
	update([this](b3_view_info & view_info)
	{
		child->b3FullView(view_info);
	});
}

void QB3OpenGLScrollArea::b3PreviousView()
{
	update([this](b3_view_info & view_info)
	{
		child->b3PreviousView(view_info);
	});
}

void QB3OpenGLScrollArea::mousePressEvent(QMouseEvent * event)
{
	gui().raiseMouseDown(SCT_point
	{
		event->pos().x(),
		event->pos().y()
	});
}

void QB3OpenGLScrollArea::update(
	const std::function<void(b3_view_info &)> & prepare)
{
	b3_view_info view_info;

	is_interactive = false;

	prepare(view_info);
	b3GetBarInfo(view_info, h, v);
	update(horizontalScrollBar(), h);
	update(verticalScrollBar(), v);

	is_interactive = true;
}

void QB3OpenGLScrollArea::mouseMoveEvent(QMouseEvent * event)
{
	gui().raiseMouseMove(SCT_point
	{
		event->pos().x(),
		event->pos().y()
	});
}

void QB3OpenGLScrollArea::next()
{
	emit selectionEnd();
}

void QB3OpenGLScrollArea::mouseReleaseEvent(QMouseEvent * event)
{
	gui().raiseMouseUp(SCT_point
	{
		event->pos().x(),
		event->pos().y()
	});
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
	/* https://stackoverflow.com/questions/49148073/how-to-reverse-a-qsliders-range
	 *
	 * NOTE: The minimum has to be always smaller than the maximum.
	 * The range is not inverted. The UI orientation is inverted so there is no
	 * need to invert the controls.
	 */
	bar->setInvertedAppearance(info.bar_inv);
	bar->setInvertedControls(info.bar_inv);

	bar->setRange(info.bar_min, info.bar_max);
	bar->setPageStep(info.bar_page_size);
	bar->setSingleStep(info.bar_page_size / 10);
	bar->setValue(info.bar_pos);
}

void QB3OpenGLScrollArea::b3GetBarInfo(
	const b3_view_info & info,
	QB3BarInfo     &     horizontal,
	QB3BarInfo     &     vertical)
{
	bool h_inverse;
	bool v_inverse;

	switch (info.m_ViewMode)
	{
	case B3_VIEW_TOP:
		h_inverse = false;
		v_inverse = true;
		break;
	case B3_VIEW_FRONT:
		h_inverse = false;
		v_inverse = true;
		break;
	case B3_VIEW_RIGHT:
		h_inverse = false;
		v_inverse = true;
		break;
	case B3_VIEW_BACK:
		h_inverse = true;
		v_inverse = true;
		break;
	case B3_VIEW_LEFT:
		h_inverse = true;
		v_inverse = true;
		break;

	default:
		h_inverse = false;
		v_inverse = false;
	}

	b3RenderView::b3Print(info);
	horizontal.set(
		h_inverse,
		info.m_Scene.left, info.m_Scene.right,
		info.m_View.left, info.m_View.right);
	vertical.set(
		v_inverse,
		info.m_Scene.bottom, info.m_Scene.top,
		info.m_View.bottom, info.m_View.top);
}

/*************************************************************************
**                                                                      **
**                        QB3BarInfo Implementation                     **
**                                                                      **
*************************************************************************/

void QB3BarInfo::set(
	const b3_bool  inverse,
	const b3_f32   scene_lower,
	const b3_f32   scene_upper,
	const b3_f32   view_lower,
	const b3_f32   view_upper)
{
	min       = std::min(scene_lower, view_lower);
	max       = std::max(scene_upper, view_upper);
	page_size = std::ceil(view_upper) - std::floor(view_lower);
	range     = std::ceil(max)        - std::floor(min);

	bar_inv       = inverse;
	bar_page_size = page_size;
	bar_min       = min;
	bar_max       = max - page_size;
	bar_pos       = view_lower;
}

b3_f64 QB3BarInfo::relFromBar(const int value) const
{
	return b3_f64(value) / page_size;
}

int QB3BarInfo::relToBar(const b3_f64 value) const
{
	return value * page_size;
}

int QB3BarInfo::posToBar(const b3_f64 pos) const
{
	return pos;
}
